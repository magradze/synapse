/**
 * @file rgb_led_indicator.c
 * @brief RGB LED ინდიკატორის მართვა
 * @author Giorgi Magradze & Synapse AI
 * @version 1.3.0
 * @details RGB LED ინდიკატორის იმპლემენტაცია, რომელიც რეაგირებს სისტემურ ივენთებზე და უზრუნველყოფს API-ს სხვა მოდულებისთვის.
 *          გამოიყენებს LEDC დრაივერს PWM კონტროლისთვის და GPIO-ს სრული გათიშვისთვის.
 */

// --- Includes ---
#include "rgb_led_indicator.h"
#include "rgb_led_interface.h"
#include "base_module.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "service_locator.h"
#include "resource_manager.h"
#include "logging.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

// --- Component Tag ---
DEFINE_COMPONENT_TAG("RGB_LED_INDICATOR");

// --- Definitions ---
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_RESOLUTION LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY (5000)

#define LEDC_TIMER (CONFIG_RGB_LED_INDICATOR_LEDC_TIMER)
#define LEDC_CHANNEL_R (CONFIG_RGB_LED_INDICATOR_LEDC_CHANNEL_R)
#define LEDC_CHANNEL_G (CONFIG_RGB_LED_INDICATOR_LEDC_CHANNEL_G)
#define LEDC_CHANNEL_B (CONFIG_RGB_LED_INDICATOR_LEDC_CHANNEL_B)

#define COMMAND_QUEUE_LENGTH 10
#define TASK_STACK_SIZE 2560 // ოდნავ გავზარდეთ, მათემატიკური ოპერაციების გამო

// --- Private Data Structures ---
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
    int red_pin;
    int green_pin;
    int blue_pin;
    bool is_common_anode;
    bool is_manual_override;
    bool is_ledc_active;
    TaskHandle_t led_task_handle;
    QueueHandle_t command_queue;
    led_command_t last_system_command;
} rgb_led_private_data_t;

// --- Forward Declarations ---
static esp_err_t rgb_led_indicator_init(module_t *self);
static esp_err_t rgb_led_indicator_start(module_t *self);
static esp_err_t rgb_led_indicator_enable(module_t *self);
static esp_err_t rgb_led_indicator_disable(module_t *self);
static void rgb_led_indicator_deinit(module_t *self);
static esp_err_t rgb_led_indicator_reconfigure(module_t *self, const cJSON *new_config);
static module_status_t rgb_led_indicator_get_status(module_t *self);
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

// --- Event to Command Mapping ---
typedef struct
{
    const char *event_name;
    led_command_t command;
} event_to_command_map_t;

static const event_to_command_map_t event_map[] = {
    {"WIFI_CREDENTIALS_NOT_FOUND", {LED_MODE_PULSE, 255, 162, 0, 3000, false}}, // ყვითელი (Provisioning)
    {"WIFI_EVENT_CONNECTED", {LED_MODE_BLINK, 0, 255, 0, 300, false}},          // მწვანე ციმციმი
    {"WIFI_EVENT_DISCONNECTED", {LED_MODE_STATIC, 255, 0, 0, 0, false}},        // წითელი
    {"SYSTEM_HEALTH_ALERT", {LED_MODE_BLINK, 255, 0, 0, 150, false}},
    // მომავალში აქ დაემატება სხვა ივენთები...
};
static const size_t event_map_size = sizeof(event_map) / sizeof(event_map[0]);

// --- Global Variables for Service API ---
static module_t *global_rgb_led_instance = NULL;
static rgb_led_api_t rgb_led_service_api = {
    .set_color = api_set_color, .turn_off = api_turn_off, .start_blink = api_start_blink, .start_pulse = api_start_pulse, .release_control = api_release_control};

// --- Module Lifecycle & Base Functions ---
module_t *rgb_led_indicator_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating rgb_led_indicator module instance");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for module");
        return NULL;
    }

    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)calloc(1, sizeof(rgb_led_private_data_t));
    if (!private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for private data");
        free(module);
        return NULL;
    }
    private_data->is_manual_override = false;

    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex)
    {
        ESP_LOGE(TAG, "Failed to create state mutex");
        free(private_data);
        free(module);
        return NULL;
    }

    module->private_data = private_data;

    const cJSON *config_node = config ? cJSON_GetObjectItem(config, "config") : NULL;
    const char *instance_name = CONFIG_RGB_LED_INDICATOR_DEFAULT_INSTANCE_NAME;
    if (config_node)
    {
        const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
        if (cJSON_IsString(name_node) && name_node->valuestring)
        {
            instance_name = name_node->valuestring;
        }
    }
    snprintf(module->name, sizeof(module->name), "%s", instance_name);

    if (parse_config(config_node, private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to parse configuration for %s", module->name);
        vSemaphoreDelete(module->state_mutex);
        free(private_data);
        free(module);
        return NULL;
    }

    if (config)
    {
        module->current_config = cJSON_Duplicate(config, true);
    }

    module->init_level = 1;

    module->status = MODULE_STATUS_UNINITIALIZED;
    module->base.init = rgb_led_indicator_init;
    module->base.start = rgb_led_indicator_start;
    module->base.handle_event = rgb_led_indicator_handle_event;
    module->base.deinit = rgb_led_indicator_deinit;
    module->base.enable = rgb_led_indicator_enable;
    module->base.disable = rgb_led_indicator_disable;
    module->base.reconfigure = rgb_led_indicator_reconfigure;
    module->base.get_status = rgb_led_indicator_get_status;
    global_rgb_led_instance = module;

    ESP_LOGI(TAG, "Rgb_Led_Indicator module created: '%s'", module->name);
    return module;
}

static esp_err_t rgb_led_indicator_init(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing module: %s", self->name);

    // რესურსების დალოქვა (უცვლელია)
    fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->red_pin, self->name);
    fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->green_pin, self->name);
    fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->blue_pin, self->name);

    // LEDC კონფიგურაცია
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE, .timer_num = LEDC_TIMER, .duty_resolution = LEDC_RESOLUTION, .freq_hz = LEDC_FREQUENCY, .clk_cfg = LEDC_USE_APB_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel_conf = {
        .speed_mode = LEDC_MODE, .timer_sel = LEDC_TIMER, .duty = 0, .hpoint = 0, .intr_type = LEDC_INTR_DISABLE};

    ledc_channel_conf.channel = LEDC_CHANNEL_R;
    ledc_channel_conf.gpio_num = private_data->red_pin;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_conf));

    ledc_channel_conf.channel = LEDC_CHANNEL_G;
    ledc_channel_conf.gpio_num = private_data->green_pin;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_conf));

    ledc_channel_conf.channel = LEDC_CHANNEL_B;
    ledc_channel_conf.gpio_num = private_data->blue_pin;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_conf));

    ESP_ERROR_CHECK(ledc_fade_func_install(0));
    private_data->is_ledc_active = true; // LEDC ახლა ყოველთვის აქტიურია

    // Queue-ს შექმნა (უცვლელია)
    private_data->command_queue = xQueueCreate(COMMAND_QUEUE_LENGTH, sizeof(led_command_t));
    if (!private_data->command_queue)
    {
        ESP_LOGE(TAG, "Failed to create command queue");
        return ESP_ERR_NO_MEM;
    }

    // ★★★ გამოწერების გამარტივება ★★★
    fmw_event_bus_subscribe("WIFI_CREDENTIALS_NOT_FOUND", self); // ამას wifi_manager აქვეყნებს
    fmw_event_bus_subscribe("WIFI_EVENT_DISCONNECTED", self);    // ამას wifi_manager აქვეყნებს
    fmw_event_bus_subscribe("WIFI_EVENT_CONNECTED", self);       // ამას wifi_manager აქვეყნებს
    fmw_event_bus_subscribe("SYSTEM_HEALTH_ALERT", self);        // ამას health_monitor აქვეყნებს

    // სერვისის რეგისტრაცია (უცვლელია)
    fmw_service_register(self->name, FMW_SERVICE_TYPE_RGB_LED_API, &rgb_led_service_api);

    // LED-ის საწყისი გამორთვა
    set_led_color(private_data, 0, 0, 0);

    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "Module initialized successfully");
    return ESP_OK;
}

static esp_err_t rgb_led_indicator_start(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    if (self->status != MODULE_STATUS_INITIALIZED)
        return ESP_ERR_INVALID_STATE;

    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting module: %s", self->name);

    BaseType_t task_created = xTaskCreate(
        led_control_task, "led_task", TASK_STACK_SIZE, self, 5, &private_data->led_task_handle);
    if (task_created != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create LED control task");
        return ESP_FAIL;
    }

    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "Module started successfully");
    return ESP_OK;
}

static esp_err_t rgb_led_indicator_enable(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;
    if (self->status != MODULE_STATUS_DISABLED)
        return ESP_OK;

    ESP_LOGI(TAG, "Enabling module %s", self->name);
    if (private_data->led_task_handle)
        vTaskResume(private_data->led_task_handle);
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static esp_err_t rgb_led_indicator_disable(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;
    if (self->status != MODULE_STATUS_RUNNING)
        return ESP_OK;

    ESP_LOGI(TAG, "Disabling module %s", self->name);
    api_turn_off();
    if (private_data->led_task_handle)
        vTaskSuspend(private_data->led_task_handle);
    self->status = MODULE_STATUS_DISABLED;
    return ESP_OK;
}

static esp_err_t rgb_led_indicator_reconfigure(module_t *self, const cJSON *new_config)
{
    if (!self || !new_config)
        return ESP_ERR_INVALID_ARG;
    ESP_LOGW(TAG, "Reconfiguration for %s is not yet implemented.", self->name);
    return ESP_ERR_NOT_SUPPORTED;
}

static module_status_t rgb_led_indicator_get_status(module_t *self)
{
    if (!self)
        return MODULE_STATUS_ERROR;
    return self->status;
}

static void rgb_led_indicator_handle_event(module_t *self, const char *event_name, void *event_data)
{
    // --- საწყისი შემოწმებები (უცვლელია) ---
    if (!self || !self->private_data || !event_name)
    {
        if (event_data)
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;
    if (self->status != MODULE_STATUS_RUNNING || private_data->is_manual_override)
    {
        if (private_data->is_manual_override)
            ESP_LOGD(TAG, "Ignoring event '%s', manual override is active.", event_name);
        if (event_data)
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    // --- ლოგიკა იწყება აქ ---

    ESP_LOGI(TAG, "EVENT HANDLER: Received '%s'", event_name);

    for (size_t i = 0; i < event_map_size; ++i)
    {
        if (strcmp(event_name, event_map[i].event_name) == 0)
        {
            ESP_LOGI(TAG, "Found matching command for event '%s'", event_name);

            // ვიღებთ ბრძანებას ცხრილიდან
            led_command_t cmd = event_map[i].command;

            // ★★★ ვინახავთ მას, როგორც ბოლო სისტემურ ბრძანებას ★★★
            private_data->last_system_command = cmd;

            // ვაგზავნით ბრძანებას ტასკში
            send_command_to_task(self, &cmd);

            // ვწყვეტთ ძებნას
            goto cleanup;
        }
    }

    ESP_LOGD(TAG, "No specific LED action defined for event '%s'", event_name);

cleanup:
    if (event_data)
    {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

static void rgb_led_indicator_deinit(module_t *self)
{
    if (!self)
        return;
    ESP_LOGI(TAG, "Deinitializing %s module", self->name);
    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;

    if (private_data->led_task_handle)
        vTaskDelete(private_data->led_task_handle);
    if (private_data->command_queue)
        vQueueDelete(private_data->command_queue);

    fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->red_pin, self->name);
    fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->green_pin, self->name);
    fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->blue_pin, self->name);

    fmw_event_bus_unsubscribe("WIFI_CREDENTIALS_NOT_FOUND", self);
    fmw_event_bus_unsubscribe("WIFI_EVENT_DISCONNECTED", self);
    fmw_event_bus_unsubscribe("WIFI_EVENT_CONNECTED", self);
    fmw_event_bus_unsubscribe("SYSTEM_HEALTH_ALERT", self);

    fmw_service_unregister(self->name);
    global_rgb_led_instance = NULL;

    if (self->private_data)
        free(self->private_data);
    if (self->current_config)
        cJSON_Delete(self->current_config);
    if (self->state_mutex)
        vSemaphoreDelete(self->state_mutex);
    free(self);

    ESP_LOGI(TAG, "Module deinitialized successfully");
}

static esp_err_t parse_config(const cJSON *config, rgb_led_private_data_t *private_data)
{
    if (!config)
    {
        ESP_LOGE(TAG, "Config JSON is NULL. Cannot parse pins.");
        return ESP_ERR_INVALID_ARG;
    }

    const cJSON *red_pin = cJSON_GetObjectItem(config, "red_pin");
    const cJSON *green_pin = cJSON_GetObjectItem(config, "green_pin");
    const cJSON *blue_pin = cJSON_GetObjectItem(config, "blue_pin");

    if (!cJSON_IsNumber(red_pin) || !cJSON_IsNumber(green_pin) || !cJSON_IsNumber(blue_pin))
    {
        ESP_LOGE(TAG, "red_pin, green_pin, and blue_pin must be specified as numbers in config");
        return ESP_ERR_INVALID_ARG;
    }

    private_data->red_pin = red_pin->valueint;
    private_data->green_pin = green_pin->valueint;
    private_data->blue_pin = blue_pin->valueint;
    private_data->is_common_anode = cJSON_IsTrue(cJSON_GetObjectItem(config, "is_common_anode"));

    ESP_LOGI(TAG, "Config parsed: R=%d, G=%d, B=%d, CommonAnode=%s",
             private_data->red_pin, private_data->green_pin, private_data->blue_pin,
             private_data->is_common_anode ? "true" : "false");
    return ESP_OK;
}

static void set_led_color(rgb_led_private_data_t *private_data, uint8_t r, uint8_t g, uint8_t b)
{
    if (!private_data)
    {
        ESP_LOGE(TAG, "set_led_color: private_data is NULL!");
        return;
    }

    ESP_LOGD(TAG, "set_led_color: Request to set RGB(%u, %u, %u)", r, g, b);

    // გამოვთვალოთ duty cycle-ები is_common_anode-ის გათვალისწინებით
    uint32_t duty_r = private_data->is_common_anode ? (255 - r) : r;
    uint32_t duty_g = private_data->is_common_anode ? (255 - g) : g;
    uint32_t duty_b = private_data->is_common_anode ? (255 - b) : b;

    ESP_LOGD(TAG, "set_led_color: Calculated duties - R:%" PRIu32 ", G:%" PRIu32 ", B:%" PRIu32, duty_r, duty_g, duty_b);

    // დავაყენოთ duty და განვაახლოთ LEDC არხები
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, duty_r);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_G, duty_g);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_G);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_B, duty_b);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_B);

    ESP_LOGD(TAG, "set_led_color: Duties updated.");
}
/**
 * @brief LED-ის მმართველი ტასკი.
 * @details ეს არის მოდულის "გული", რომელიც მუშაობს ფონურ რეჟიმში. ის იღებს
 *          ბრძანებებს Queue-ს საშუალებით და მართავს LED-ის მდგომარეობასა და ეფექტებს
 *          (სტატიკური ფერი, ციმციმი, პულსაცია) არაბლოკირებად რეჟიმში.
 * @param pvParameters მაჩვენებელი მოდულის ინსტანციაზე (`module_t*`).
 */
static void led_control_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    rgb_led_private_data_t *private_data = (rgb_led_private_data_t *)self->private_data;

    // ეს სტრუქტურა ინახავს მიმდინარე აქტიურ ბრძანებას.
    led_command_t active_cmd;
    memset(&active_cmd, 0, sizeof(led_command_t));
    active_cmd.mode = LED_MODE_OFF;

    // ეფექტების მართვის ცვლადები
    uint32_t last_update_time = 0;
    bool blink_state_on = false;
    uint8_t blink_cycle_count = 0;

    ESP_LOGI(TAG, "TASK: LED control task started.");

    while (1)
    {
        // ველოდებით ახალ ბრძანებას. თუ ეფექტი აქტიურია (blink/pulse),
        // ველოდებით 50ms, რათა ციკლმა შეძლოს ეფექტის განახლება.
        // თუ სტატიკურია, ველოდებით უსასრულოდ.
        TickType_t wait_ticks = (active_cmd.mode == LED_MODE_BLINK || active_cmd.mode == LED_MODE_PULSE) ? pdMS_TO_TICKS(50) : portMAX_DELAY;

        // ვცდილობთ ახალი ბრძანების მიღებას.
        if (xQueueReceive(private_data->command_queue, &active_cmd, wait_ticks) == pdPASS)
        {
            ESP_LOGI(TAG, "TASK: New command received: Mode=%d", active_cmd.mode);
            private_data->is_manual_override = active_cmd.is_manual;

            // ახალი ბრძანების მიღებისას ვანულებთ ეფექტის მდგომარეობას
            last_update_time = xTaskGetTickCount();
            blink_state_on = true;
            blink_cycle_count = 0;

            // თუ ბრძანება არის STATIC ან OFF, ვასრულებთ დაუყოვნებლივ და ვწყვეტთ ეფექტს.
            if (active_cmd.mode == LED_MODE_STATIC)
            {
                set_led_color(private_data, active_cmd.r, active_cmd.g, active_cmd.b);
                active_cmd.mode = LED_MODE_OFF; // ვაჩერებთ ეფექტს, ვტოვებთ ფერს
            }
            else if (active_cmd.mode == LED_MODE_OFF)
            {
                set_led_color(private_data, 0, 0, 0);
            }
        }

        // --- მიმდინარე ეფექტის დამუშავება ---

        switch (active_cmd.mode)
        {
        case LED_MODE_BLINK:
        {
            uint32_t now = xTaskGetTickCount();
            if ((now - last_update_time) * portTICK_PERIOD_MS < active_cmd.param1)
            {
                break; // ჯერ არ გასულა ინტერვალი
            }
            last_update_time = now;

            // WIFI connected ივენთის სპეციალური, ერთჯერადი დამუშავება
            if (!active_cmd.is_manual && active_cmd.g == 255)
            {
                if (blink_cycle_count < 6) // 3 ციმციმი = 6 ცვლილება (on/off)
                {
                    blink_state_on = !blink_state_on;
                    set_led_color(private_data, 0, blink_state_on ? 255 : 0, 0);
                    blink_cycle_count++;
                }
                else
                {
                    active_cmd.mode = LED_MODE_OFF; // დავასრულეთ, ვთიშავთ
                    set_led_color(private_data, 0, 0, 0);
                }
            }
            else // ზოგადი, უწყვეტი ციმციმი
            {
                blink_state_on = !blink_state_on;
                set_led_color(private_data,
                              blink_state_on ? active_cmd.r : 0,
                              blink_state_on ? active_cmd.g : 0,
                              blink_state_on ? active_cmd.b : 0);
            }
            break;
        }
        case LED_MODE_PULSE:
        {
            if (active_cmd.param1 == 0)
                break;
            uint32_t now = xTaskGetTickCount();
            float sine_wave = sinf((float)now * 2.0f * M_PI / (active_cmd.param1 / portTICK_PERIOD_MS));
            float brightness = (sine_wave + 1.0f) / 2.0f;
            set_led_color(private_data,
                          (uint8_t)(active_cmd.r * brightness),
                          (uint8_t)(active_cmd.g * brightness),
                          (uint8_t)(active_cmd.b * brightness));
            break;
        }
        default:
            // STATIC და OFF რეჟიმებში არაფერს ვაკეთებთ, უბრალოდ ველოდებით ახალ ბრძანებას
            break;
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

/**
 * @brief ათავისუფლებს მექანიკურ კონტროლს და აბრუნებს LED-ს ავტომატურ რეჟიმში.
 * @details ეს ფუნქცია აუქმებს is_manual_override ფლაგს და ხელახლა აგზავნის
 *          ბოლო შენახულ სისტემურ ბრძანებას LED-ის ტასკში, რითაც აღადგენს
 *          ინდიკატორის იმ მდგომარეობას, რომელიც შეესაბამება სისტემის
 *          მიმდინარე სტატუსს.
 */
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