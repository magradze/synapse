/**
 * @file rgb_led_indicator.c
 * @brief RGB LED ინდიკატორის მართვა (Shift Register ვერსია)
 * @author Synapse Team
 * @version 2.1.0
 */

#include "synapse.h"
#include "rgb_led_indicator.h"
#include "rgb_led_interface.h"
#include "ble_prov_interface.h"

#include "esp_log.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include "cmd_router_interface.h"
#include "sn74hc595n_writer_interface.h"

DEFINE_COMPONENT_TAG("RGB_LED_INDICATOR", SYNAPSE_LOG_COLOR_GREEN);

#define COMMAND_QUEUE_LENGTH 10
#define TASK_STACK_SIZE 3072

typedef enum
{
    LED_MODE_STATIC,
    LED_MODE_BLINK,
    LED_MODE_PULSE,
    LED_MODE_OFF,
} led_mode_t;

typedef struct
{
    led_mode_t mode;
    uint8_t r, g, b;
    uint32_t param1;
    bool is_manual;
} led_command_t;

typedef struct
{
    const char *event_name;
    led_command_t command;
} event_to_command_map_t;

typedef struct
{
    char sr_writer_service_name[32];
    sn74hc595n_writer_handle_t *sr_handle;
    uint16_t red_pin;
    uint16_t green_pin;
    uint16_t blue_pin;
    bool is_common_anode;
    bool is_manual_override;
    TaskHandle_t led_task_handle;
    QueueHandle_t command_queue;
    led_command_t last_system_command;
} rgb_led_private_data_t;

static esp_err_t rgb_led_indicator_init(module_t *self);
static esp_err_t rgb_led_indicator_start(module_t *self);
static void rgb_led_indicator_deinit(module_t *self);
static void rgb_led_indicator_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t parse_config(const cJSON *config, rgb_led_private_data_t *private_data);
static void led_control_task(void *pvParameters);
static esp_err_t send_command_to_task(module_t *self, led_command_t *cmd);
static void set_led_color(rgb_led_private_data_t *private_data, uint8_t r, uint8_t g, uint8_t b);
static esp_err_t api_set_color(uint8_t r, uint8_t g, uint8_t b);
static esp_err_t api_turn_off();
static esp_err_t api_start_blink(uint8_t r, uint8_t g, uint8_t b, uint32_t interval_ms);
static esp_err_t api_start_pulse(uint8_t r, uint8_t g, uint8_t b, uint32_t period_ms);
static esp_err_t api_release_control();
static void register_cli_commands(module_t *self);
static void unregister_cli_commands(module_t *self);

static const event_to_command_map_t event_map[] = {
    {"WIFI_CREDENTIALS_NOT_FOUND", {LED_MODE_PULSE, 255, 162, 0, 3000, false}},
    {"WIFI_EVENT_CONNECTED", {LED_MODE_BLINK, 0, 255, 0, 300, false}},
    {"WIFI_EVENT_DISCONNECTED", {LED_MODE_STATIC, 255, 0, 0, 0, false}},
    {"SYSTEM_HEALTH_ALERT", {LED_MODE_BLINK, 255, 0, 0, 150, false}},
};
static const size_t event_map_size = sizeof(event_map) / sizeof(event_map[0]);

static module_t *global_rgb_led_instance = NULL;
static rgb_led_api_t rgb_led_service_api = {
    .set_color = api_set_color, .turn_off = api_turn_off, .start_blink = api_start_blink, .start_pulse = api_start_pulse, .release_control = api_release_control};

module_t *rgb_led_indicator_create(const cJSON *config)
{
    // --- 1. მეხსიერების გამოყოფა ---
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)calloc(1, sizeof(rgb_led_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        // `config` ობიექტის მფლობელობა ეკუთვნის გამომძახებელს (Module Factory),
        // ამიტომ აქ მისი წაშლა არ არის საჭირო, თუ ის არ არის დუბლირებული.
        return NULL;
    }

    module->private_data = private_data;

    // --- 2. კონფიგურაციის დუბლირება (მფლობელობის აღება) ---
    module->current_config = cJSON_Duplicate(config, true);
    if (!module->current_config)
    {
        ESP_LOGE(TAG, "Failed to duplicate configuration object.");
        free(private_data);
        free(module);
        return NULL;
    }

    // --- 3. კონფიგურაციის პარსინგი და ვალიდაცია ---
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (parse_config(config_node, private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to parse configuration");
        rgb_led_indicator_deinit(module); // deinit გაასუფთავებს ყველაფერს, მათ შორის current_config-ს.
        return NULL;
    }

    // --- 4. მოდულის საბაზისო ველების შევსება ---
    snprintf(module->name, sizeof(module->name), "%s", cJSON_GetObjectItem(config_node, "instance_name")->valuestring);
    module->init_level = 60; // მნიშვნელოვანია, რომ ეს იყოს სწორად განსაზღვრული
    module->status = MODULE_STATUS_UNINITIALIZED;

    // --- 5. სიცოცხლის ციკლის ფუნქციების მინიჭება ---
    module->base.init = rgb_led_indicator_init;
    module->base.start = rgb_led_indicator_start;
    module->base.handle_event = rgb_led_indicator_handle_event;
    module->base.deinit = rgb_led_indicator_deinit;
    module->base.enable = NULL; // ეს მოდული არ უჭერს მხარს runtime enable/disable-ს
    module->base.disable = NULL;
    module->base.reconfigure = NULL; // არ უჭერს მხარს reconfigure-ს
    module->base.get_status = NULL;  // გამოიყენებს default სტატუსის ველს

    // --- 6. სერვისის რეგისტრაცია (ახალი ლოგიკა) ---
    // რეგისტრაცია ხდება create ფაზაში, რათა System Manager-მა შეძლოს მისი სტატუსის მართვა init-ის დაწყებამდე.
    esp_err_t ret = synapse_service_register_with_status(
        module->name,
        SYNAPSE_SERVICE_TYPE_RGB_LED_API,
        &rgb_led_service_api,
        SERVICE_STATUS_REGISTERED);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register service for '%s' (%s). Cleaning up.", module->name, esp_err_to_name(ret));
        rgb_led_indicator_deinit(module); // deinit გამოიძახებს სრულ გასუფთავებას.
        return NULL;
    }

    // --- 7. გლობალური ინსტანციის მინიჭება (თუ საჭიროა) ---
    // შენიშვნა: გლობალური ცვლადების გამოყენება არ არის რეკომენდებული Synapse-ის არქიტექტურაში.
    // თუ შესაძლებელია, უმჯობესია ამისგან თავის არიდება და Service Locator-ზე დაყრდნობა.
    global_rgb_led_instance = module;

    ESP_LOGI(TAG, "Rgb_Led_Indicator module created: '%s'", module->name);

    // --- 8. მოდულის ობიექტის დაბრუნება ---
    return module;
}

static esp_err_t rgb_led_indicator_init(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing module: %s (Shift Register Mode)", self->name);

    private_data->sr_handle = (sn74hc595n_writer_handle_t *)synapse_service_get(private_data->sr_writer_service_name);
    if (!private_data->sr_handle)
    {
        ESP_LOGE(TAG, "Shift register writer service '%s' not found!", private_data->sr_writer_service_name);
        return ESP_ERR_NOT_FOUND;
    }

    private_data->command_queue = xQueueCreate(COMMAND_QUEUE_LENGTH, sizeof(led_command_t));
    if (!private_data->command_queue)
    {
        ESP_LOGE(TAG, "Failed to create command queue");
        return ESP_ERR_NO_MEM;
    }

    synapse_event_bus_subscribe("WIFI_CREDENTIALS_NOT_FOUND", self);
    synapse_event_bus_subscribe("WIFI_EVENT_DISCONNECTED", self);
    synapse_event_bus_subscribe("WIFI_EVENT_CONNECTED", self);
    synapse_event_bus_subscribe("SYSTEM_HEALTH_ALERT", self);

    register_cli_commands(self);
    set_led_color(private_data, 0, 0, 0);

    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "Module initialized successfully");
    return ESP_OK;
}

static esp_err_t rgb_led_indicator_start(module_t *self)
{
    if (!self || self->status != MODULE_STATUS_INITIALIZED)
        return ESP_ERR_INVALID_STATE;
    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting module: %s", self->name);

    if (xTaskCreate(led_control_task, "led_task", TASK_STACK_SIZE, self, 5, &private_data->led_task_handle) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create LED control task");
        return ESP_FAIL;
    }

    service_handle_t prov_handle = synapse_service_get("main_ble_provisioning");
    if (prov_handle)
    {
        ble_prov_api_t *prov_api = (ble_prov_api_t *)prov_handle;
        if (prov_api->is_provisioning_active())
        {
            ESP_LOGI(TAG, "Provisioning is already active. Starting pulse effect.");
            led_command_t cmd = {LED_MODE_PULSE, 255, 162, 0, 3000, false};
            send_command_to_task(self, &cmd);
        }
    }
    else
    {
        ESP_LOGW(TAG, "BLE Provisioning service not found. Cannot check initial state.");
    }

    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "Module started successfully");
    return ESP_OK;
}

static void rgb_led_indicator_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (!self || !self->private_data || !event_name)
    {
        if (event_data)
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;
    if (self->status != MODULE_STATUS_RUNNING || private_data->is_manual_override)
    {
        if (private_data->is_manual_override)
            ESP_LOGD(TAG, "Ignoring event '%s', manual override is active.", event_name);
        if (event_data)
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    ESP_LOGI(TAG, "EVENT HANDLER: Received '%s'", event_name);

    for (size_t i = 0; i < event_map_size; ++i)
    {
        if (strcmp(event_name, event_map[i].event_name) == 0)
        {
            ESP_LOGI(TAG, "Found matching command for event '%s'", event_name);

            led_command_t cmd = event_map[i].command;

            private_data->last_system_command = cmd;

            send_command_to_task(self, &cmd);

            goto cleanup;
        }
    }

    ESP_LOGD(TAG, "No specific LED action defined for event '%s'", event_name);

cleanup:
    if (event_data)
    {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

static void rgb_led_indicator_deinit(module_t *self)
{
    if (!self)
        return;
    ESP_LOGI(TAG, "Deinitializing module '%s'...", self->name);

    if (self->private_data)
    {
        rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;
        if (private_data->led_task_handle)
            vTaskDelete(private_data->led_task_handle);
        if (private_data->command_queue)
            vQueueDelete(private_data->command_queue);
        free(self->private_data);
    }

    synapse_event_bus_unsubscribe("WIFI_CREDENTIALS_NOT_FOUND", self);
    synapse_event_bus_unsubscribe("WIFI_EVENT_DISCONNECTED", self);
    synapse_event_bus_unsubscribe("WIFI_EVENT_CONNECTED", self);
    synapse_event_bus_unsubscribe("SYSTEM_HEALTH_ALERT", self);

    synapse_service_unregister(self->name);
    unregister_cli_commands(self);

    if (global_rgb_led_instance == self)
        global_rgb_led_instance = NULL;
    if (self->current_config)
        cJSON_Delete(self->current_config);

    ESP_LOGI(TAG, "Module deinitialized successfully.");
}

static esp_err_t parse_config(const cJSON *config_node, rgb_led_private_data_t *private_data)
{
    if (!config_node || !private_data)
    {
        return ESP_ERR_INVALID_ARG;
    }

    // --- Step 1: Set default values ---
    private_data->is_common_anode = false;

    // --- Step 2: Use utility functions to parse values ---
    bool service_ok = synapse_config_get_string_from_node(TAG, config_node, "sr_writer_service",
                                                          private_data->sr_writer_service_name, sizeof(private_data->sr_writer_service_name));

    bool red_ok = synapse_config_get_int_from_node(TAG, config_node, "red_pin", (int *)&private_data->red_pin);
    bool green_ok = synapse_config_get_int_from_node(TAG, config_node, "green_pin", (int *)&private_data->green_pin);
    bool blue_ok = synapse_config_get_int_from_node(TAG, config_node, "blue_pin", (int *)&private_data->blue_pin);

    // Optional parameter
    synapse_config_get_bool_from_node(TAG, config_node, "is_common_anode", &private_data->is_common_anode);

    // --- Step 3: Validate that all required parameters were found ---
    if (!service_ok || !red_ok || !green_ok || !blue_ok)
    {
        ESP_LOGE(TAG, "Required config parameters (sr_writer_service, red_pin, green_pin, blue_pin) are missing.");
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Config parsed: SR_Service='%s', R=%d, G=%d, B=%d, CommonAnode=%s",
             private_data->sr_writer_service_name, private_data->red_pin, private_data->green_pin, private_data->blue_pin,
             private_data->is_common_anode ? "true" : "false");

    return ESP_OK;
}

static void set_led_color(rgb_led_private_data_t *private_data, uint8_t r, uint8_t g, uint8_t b)
{
    if (!private_data || !private_data->sr_handle || !private_data->sr_handle->api)
    {
        ESP_LOGE(TAG, "set_led_color: Shift register handle or API is not available!");
        return;
    }

    ESP_LOGD(TAG, "Setting color to R:%d, G:%d, B:%d. Common Anode: %s", r, g, b, private_data->is_common_anode ? "Yes" : "No");

    // 1. განვსაზღვროთ, რომელი ლოგიკური დონე (true/false) შეესაბამება "ანთებულ" მდგომარეობას.
    //    - Common Cathode: ON = HIGH = true
    //    - Common Anode:   ON = LOW  = false
    bool state_when_on = !private_data->is_common_anode;

    // 2. განვსაზღვროთ თითოეული პინის საბოლოო მდგომარეობა
    bool r_state = (r > 127) ? state_when_on : !state_when_on;
    bool g_state = (g > 127) ? state_when_on : !state_when_on;
    bool b_state = (b > 127) ? state_when_on : !state_when_on;

    ESP_LOGD(TAG, "Pin states to send: R_PIN(%d)=%d, G_PIN(%d)=%d, B_PIN(%d)=%d",
             private_data->red_pin, r_state,
             private_data->green_pin, g_state,
             private_data->blue_pin, b_state);

    // 3. ავაწყოთ 8-ბიტიანი მნიშვნელობა (ბაიტი)
    uint8_t new_chip_state = 0;
    if (r_state)
        new_chip_state |= (1 << (private_data->red_pin % 8));
    if (g_state)
        new_chip_state |= (1 << (private_data->green_pin % 8));
    if (b_state)
        new_chip_state |= (1 << (private_data->blue_pin % 8));

    ESP_LOGD(TAG, "Calculated byte to send to chip 0: 0x%02X", new_chip_state);

    // 4. გავაგზავნოთ ბაიტი shift register-ში ერთი გამოძახებით
    esp_err_t err = private_data->sr_handle->api->set_chip_pins(private_data->sr_handle->context, 0, new_chip_state);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set chip pins via service: %s", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGD(TAG, "Successfully sent new state to shift register.");
    }
}

static void led_control_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;

    led_command_t active_cmd;
    memset(&active_cmd, 0, sizeof(led_command_t));
    active_cmd.mode = LED_MODE_OFF;

    bool blink_state_on = false;
    uint8_t blink_cycle_count = 0;
    TickType_t last_effect_update = 0;

    ESP_LOGI(TAG, "TASK: LED control task started.");

    while (1)
    {
        // 1. ვამოწმებთ ახალ ბრძანებას, მაგრამ არ ვბლოკავთ ტასკს დიდხანს
        if (xQueueReceive(private_data->command_queue, &active_cmd, pdMS_TO_TICKS(10)) == pdPASS)
        {
            ESP_LOGI(TAG, "TASK: New command received: Mode=%d", active_cmd.mode);
            private_data->is_manual_override = active_cmd.is_manual;
            blink_state_on = true;
            blink_cycle_count = 0;
            last_effect_update = xTaskGetTickCount();

            // თუ ბრძანება სტატიკურია ან გამორთვა, ვასრულებთ დაუყოვნებლივ
            if (active_cmd.mode == LED_MODE_STATIC)
            {
                set_led_color(private_data, active_cmd.r, active_cmd.g, active_cmd.b);
            }
            else if (active_cmd.mode == LED_MODE_OFF)
            {
                set_led_color(private_data, 0, 0, 0);
            }
        }

        // 2. ვამუშავებთ მხოლოდ აქტიურ ეფექტებს
        if (active_cmd.mode != LED_MODE_BLINK && active_cmd.mode != LED_MODE_PULSE)
        {
            continue; // თუ ეფექტი არ არის, გადავდივართ შემდეგ იტერაციაზე
        }

        if (active_cmd.param1 == 0)
        {
            active_cmd.mode = LED_MODE_OFF;
            continue;
        }

        // 3. ვამოწმებთ, გავიდა თუ არა ეფექტის განახლების დრო
        if ((xTaskGetTickCount() - last_effect_update) * portTICK_PERIOD_MS < (active_cmd.param1 / 2))
        {
            continue;
        }
        last_effect_update = xTaskGetTickCount();

        // 4. ვანახლებთ ეფექტს
        blink_state_on = !blink_state_on;

        if (!active_cmd.is_manual && active_cmd.g == 255)
        { // WiFi connected
            if (blink_cycle_count < 6)
            {
                set_led_color(private_data, 0, blink_state_on ? 255 : 0, 0);
                blink_cycle_count++;
            }
            else
            {
                active_cmd.mode = LED_MODE_OFF;
                set_led_color(private_data, 0, 0, 0);
            }
        }
        else
        { // ზოგადი ციმციმი (BLINK და PULSE)
            set_led_color(private_data,
                          blink_state_on ? active_cmd.r : 0,
                          blink_state_on ? active_cmd.g : 0,
                          blink_state_on ? active_cmd.b : 0);
        }
    }
}

static esp_err_t send_command_to_task(module_t *self, led_command_t *cmd)
{
    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;
    if (xQueueSend(private_data->command_queue, cmd, pdMS_TO_TICKS(100)) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to send command to LED task queue");
        return ESP_FAIL;
    }
    return ESP_OK;
}

static esp_err_t api_set_color(uint8_t r, uint8_t g, uint8_t b)
{
    if (!global_rgb_led_instance)
        return ESP_ERR_INVALID_STATE;
    led_command_t cmd = {.mode = LED_MODE_STATIC, .r = r, .g = g, .b = b, .is_manual = true};
    return send_command_to_task(global_rgb_led_instance, &cmd);
}

static esp_err_t api_turn_off()
{
    if (!global_rgb_led_instance)
        return ESP_ERR_INVALID_STATE;
    led_command_t cmd = {.mode = LED_MODE_OFF, .is_manual = true};
    return send_command_to_task(global_rgb_led_instance, &cmd);
}

static esp_err_t api_start_blink(uint8_t r, uint8_t g, uint8_t b, uint32_t interval_ms)
{
    if (!global_rgb_led_instance)
        return ESP_ERR_INVALID_STATE;
    led_command_t cmd = {.mode = LED_MODE_BLINK, .r = r, .g = g, .b = b, .param1 = interval_ms, .is_manual = true};
    return send_command_to_task(global_rgb_led_instance, &cmd);
}

static esp_err_t api_start_pulse(uint8_t r, uint8_t g, uint8_t b, uint32_t period_ms)
{
    if (!global_rgb_led_instance)
        return ESP_ERR_INVALID_STATE;
    led_command_t cmd = {.mode = LED_MODE_PULSE, .r = r, .g = g, .b = b, .param1 = period_ms, .is_manual = true};
    return send_command_to_task(global_rgb_led_instance, &cmd);
}

static esp_err_t api_release_control()
{
    if (!global_rgb_led_instance)
    {
        return ESP_ERR_INVALID_STATE;
    }

    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)global_rgb_led_instance->private_data;

    // 1. ვათავისუფლებთ მექანიკურ კონტროლს
    private_data->is_manual_override = false;
    ESP_LOGI(TAG, "Manual control released. Restoring last known system state.");

    // 2. ვაგზავნით ბოლო შენახულ სისტემურ ბრძანებას ტასკში
    //    ეს ბრძანება ინახება handle_event ფუნქციაში ყოველი სისტემური ივენთისას.
    return send_command_to_task(global_rgb_led_instance, &private_data->last_system_command);
}

// =========================================================================
//                      Command Router Integration
// =========================================================================

static esp_err_t led_cmd_handler(int argc, char **argv, void *context)
{
    if (argc < 2)
    {
        printf("Error: Missing subcommand for 'led'.\nUsage: led <set|blink|pulse|off|release> [params...]\n");
        return ESP_ERR_INVALID_ARG;
    }

    const char *sub_command = argv[1];

    if (strcmp(sub_command, "set") == 0)
    {
        if (argc != 5)
        {
            printf("Usage: led set <r> <g> <b>\n");
            return ESP_ERR_INVALID_ARG;
        }
        uint8_t r = atoi(argv[2]);
        uint8_t g = atoi(argv[3]);
        uint8_t b = atoi(argv[4]);
        printf("Setting color to R:%d G:%d B:%d\n", r, g, b);
        return api_set_color(r, g, b);
    }
    else if (strcmp(sub_command, "blink") == 0)
    {
        if (argc != 6)
        {
            printf("Usage: led blink <r> <g> <b> <interval_ms>\n");
            return ESP_ERR_INVALID_ARG;
        }
        uint8_t r = atoi(argv[2]);
        uint8_t g = atoi(argv[3]);
        uint8_t b = atoi(argv[4]);
        uint32_t interval = atoi(argv[5]);
        printf("Starting blink with R:%d G:%d B:%d, Interval:%" PRIu32 "ms\n", r, g, b, interval);
        return api_start_blink(r, g, b, interval);
    }
    else if (strcmp(sub_command, "pulse") == 0)
    {
        if (argc != 6)
        {
            printf("Usage: led pulse <r> <g> <b> <period_ms>\n");
            return ESP_ERR_INVALID_ARG;
        }
        uint8_t r = atoi(argv[2]);
        uint8_t g = atoi(argv[3]);
        uint8_t b = atoi(argv[4]);
        uint32_t period = atoi(argv[5]);
        printf("Starting pulse with R:%d G:%d B:%d, Period:%" PRIu32 "ms\n", r, g, b, period);
        return api_start_pulse(r, g, b, period);
    }
    else if (strcmp(sub_command, "off") == 0)
    {
        printf("Turning LED off.\n");
        return api_turn_off();
    }
    else if (strcmp(sub_command, "release") == 0)
    {
        printf("Releasing manual control.\n");
        return api_release_control();
    }
    else
    {
        printf("Error: Unknown subcommand '%s'.\n", sub_command);
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

static void register_cli_commands(module_t *self)
{
    // 1. მოვამზადოთ ბრძანების აღწერის სტრუქტურა.
    // ის უნდა იყოს static, რათა მისი მეხსიერება ვალიდური დარჩეს.
    static cmd_t led_command;
    led_command = (cmd_t){
        .command = "led",
        .help = "Control the RGB LED indicator.",
        .usage = "led <set|blink|pulse|off|release> [params...]",
        .min_args = 2, // მაგ: led off
        .max_args = 6, // მაგ: led pulse 255 0 255 2000
        .handler = led_cmd_handler,
        .context = self // გადავცეთ module_t* როგორც კონტექსტი
    };

    // 2. მოვძებნოთ Command Router სერვისი.
    service_handle_t handle = synapse_service_get("main_cmd_router");
    if (handle)
    {
        cmd_router_api_t *cmd_api = (cmd_router_api_t *)handle;
        esp_err_t err = cmd_api->register_command(&led_command);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to register 'led' command: %s", esp_err_to_name(err));
        }
        else
        {
            ESP_LOGI(TAG, "'led' command registered successfully.");
        }
    }
    else
    {
        ESP_LOGW(TAG, "Command Router service not found. CLI commands will not be available.");
    }
}

static void unregister_cli_commands(module_t *self)
{
    service_handle_t handle = synapse_service_get("main_cmd_router");
    if (handle)
    {
        cmd_router_api_t *cmd_api = (cmd_router_api_t *)handle;
        esp_err_t err = cmd_api->unregister_command("led");
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to unregister 'led' command: %s", esp_err_to_name(err));
        }
        else
        {
            ESP_LOGI(TAG, "'led' command unregistered successfully.");
        }
    }
}