/**
 * @file button_input.c
 * @brief A module to handle hardware button presses from GPIO or I/O expanders.
 * @author Synapse Team
 * @version 2.0.0
 */
#include "button_input.h"
#include "logging.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "event_payloads.h"
#include "framework_events.h"
#include "resource_manager.h"
#include "service_locator.h"
#include "mcp23017_interface.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <inttypes.h>

DEFINE_COMPONENT_TAG("BUTTON_INPUT");

#define MAX_BUTTONS 16
#define DEBOUNCE_TIME_MS 50
#define POLLING_INTERVAL_MS 50

typedef enum
{
    CONTROL_TYPE_GPIO,
    CONTROL_TYPE_EXPANDER
} control_type_t;

typedef struct
{
    char name[16];
    uint8_t pin; // Can be GPIO or Expander pin
    uint8_t active_level;
    bool last_state;
    uint32_t last_state_change_time;
} button_config_t;

typedef struct
{
    char instance_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];
    TaskHandle_t task_handle;
    QueueHandle_t gpio_evt_queue;
    button_config_t buttons[MAX_BUTTONS];
    uint8_t button_count;
    control_type_t control_type;
    char expander_service_name[32];
    mcp23017_handle_t *expander_handle;
} button_input_private_data_t;

static QueueHandle_t g_button_gpio_evt_queue = NULL;

static esp_err_t button_input_init(module_t *self);
static esp_err_t button_input_start(module_t *self);
static void button_input_deinit(module_t *self);
static esp_err_t parse_config(const cJSON *config_node, button_input_private_data_t *private_data);
static void button_task(void *pvParameters);
static void gpio_isr_handler(void *arg);

module_t *button_input_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    button_input_private_data_t *private_data = (button_input_private_data_t *)calloc(1, sizeof(button_input_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        if (config)
            cJSON_Delete((cJSON *)config);
        return NULL;
    }

    module->current_config = (cJSON *)config;
    module->private_data = private_data;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (parse_config(config_node, private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Configuration parsing failed");
        button_input_deinit(module);
        return NULL;
    }

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->init_level = 60;
    module->base.init = button_input_init;
    module->base.start = button_input_start;
    module->base.deinit = button_input_deinit;

    ESP_LOGI(TAG, "Button Input module '%s' created with %d buttons (mode: %s).",
             module->name, private_data->button_count, private_data->control_type == CONTROL_TYPE_GPIO ? "GPIO" : "Expander");
    return module;
}

static void publish_button_event(const char *button_name)
{
    fmw_button_payload_t *payload = calloc(1, sizeof(fmw_button_payload_t));
    if (payload)
    {
        snprintf(payload->button_name, sizeof(payload->button_name), "%s", button_name);

        event_data_wrapper_t *wrapper;
        if (fmw_event_data_wrap(payload, fmw_payload_common_free, &wrapper) == ESP_OK)
        {
            fmw_event_bus_post(FMW_EVENT_BUTTON_PRESSED, wrapper);
            fmw_event_data_release(wrapper);
        }
        else
        {
            free(payload);
        }
    }
}

static void gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)(intptr_t)arg;
    if (g_button_gpio_evt_queue)
    {
        xQueueSendFromISR(g_button_gpio_evt_queue, &gpio_num, NULL);
    }
}

static void button_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    button_input_private_data_t *private_data = (button_input_private_data_t *)self->private_data;

    if (private_data->control_type == CONTROL_TYPE_EXPANDER)
    {
        // --- I/O Expander Polling Mode (შესწორებული ლოგიკა) ---
        while (1)
        {
            uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;

            // გავიაროთ მხოლოდ ამ მოდულისთვის კონფიგურირებული ღილაკები
            for (int i = 0; i < private_data->button_count; i++)
            {
                button_config_t *btn = &private_data->buttons[i];
                bool current_pin_level;

                // წავიკითხოთ მხოლოდ ერთი, კონკრეტული პინის მდგომარეობა
                esp_err_t ret = private_data->expander_handle->api->get_pin_level(
                    private_data->expander_handle->context,
                    btn->pin,
                    &current_pin_level);

                if (ret != ESP_OK)
                {
                    continue; // გადავიდეთ შემდეგ ღილაკზე თუ წაკითხვა ვერ მოხერხდა
                }

                bool is_pressed = (current_pin_level == (bool)btn->active_level);

                // შევამოწმოთ დაჭერის მომენტი (falling edge)
                if (is_pressed && !btn->last_state)
                {
                    // გამოვიყენოთ debouncing ლოგიკა
                    if ((now - btn->last_state_change_time) > DEBOUNCE_TIME_MS)
                    {
                        ESP_LOGD(TAG, "Button '%s' PRESSED on Expander Pin %d", btn->name, btn->pin);
                        publish_button_event(btn->name);
                    }
                }

                // დავიმახსოვროთ პინის ბოლო მდგომარეობა და ცვლილების დრო
                if (is_pressed != btn->last_state)
                {
                    btn->last_state = is_pressed;
                    btn->last_state_change_time = now;
                }
            }
            vTaskDelay(pdMS_TO_TICKS(20)); // Polling ინტერვალი
        }
    }
    else
    {
        uint32_t last_event_time[MAX_BUTTONS] = {0};

        while (1)
        {
            uint16_t all_pins_value;
            if (private_data->expander_handle->api->read_all_pins(private_data->expander_handle->context, &all_pins_value) == ESP_OK)
            {
                uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;

                for (int i = 0; i < private_data->button_count; i++)
                {
                    button_config_t *btn = &private_data->buttons[i];

                    bool is_pressed = (((all_pins_value >> btn->pin) & 1) == (bool)btn->active_level);

                    if (is_pressed && !btn->last_state)
                    {
                        // ღილაკი ახლახანს დააჭირეს
                        if ((now - last_event_time[i]) > DEBOUNCE_TIME_MS)
                        {
                            ESP_LOGI(TAG, "Button '%s' press confirmed on Expander Pin %d", btn->name, btn->pin);
                            publish_button_event(btn->name);
                            last_event_time[i] = now; // დავიმახსოვროთ დრო, რომ თავიდან ავიცილოთ სპამი
                        }
                    }
                    btn->last_state = is_pressed;
                }
            }

    if (private_data->control_type == CONTROL_TYPE_EXPANDER)
    {
        // --- I/O Expander Polling Mode (ულტრა-გამარტივებული) ---
        uint16_t last_pins_value = 0xFFFF; // ვივარაუდოთ, რომ თავიდან ყველა აშვებულია (HIGH)

        while (1)
        {
            uint16_t current_pins_value;
            esp_err_t ret = private_data->expander_handle->api->read_all_pins(private_data->expander_handle->context, &current_pins_value);

            if (ret == ESP_OK)
            {
                // შევამოწმოთ, შეიცვალა თუ არა რომელიმე პინის მდგომარეობა
                if (current_pins_value != last_pins_value)
                {
                    ESP_LOGW(TAG, "Pin state change detected! Old: 0x%04X, New: 0x%04X", last_pins_value, current_pins_value);

                    // გავიაროთ ყველა ღილაკი და ვნახოთ, რომელი დააჭირეს
                    for (int i = 0; i < private_data->button_count; i++)
                    {
                        button_config_t *btn = &private_data->buttons[i];

                        bool was_pressed = !((last_pins_value >> btn->pin) & 1); // ვივარაუდოთ active_level=0
                        bool is_pressed = !((current_pins_value >> btn->pin) & 1);

                        if (is_pressed && !was_pressed)
                        {
                            ESP_LOGI(TAG, "Button '%s' PRESSED on Expander Pin %d", btn->name, btn->pin);
                            publish_button_event(btn->name);
                        }
                    }
                    last_pins_value = current_pins_value;
                }
            }
            vTaskDelay(pdMS_TO_TICKS(50)); // Polling ინტერვალი
        }
    }
    else
    {
        uint32_t last_event_time[MAX_BUTTONS] = {0};

        while (1)
        {
            uint16_t all_pins_value;
            if (private_data->expander_handle->api->read_all_pins(private_data->expander_handle->context, &all_pins_value) == ESP_OK)
            {
                uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;

                for (int i = 0; i < private_data->button_count; i++)
                {
                    button_config_t *btn = &private_data->buttons[i];

                    bool is_pressed = (((all_pins_value >> btn->pin) & 1) == (bool)btn->active_level);

                    if (is_pressed && !btn->last_state)
                    {
                        // ღილაკი ახლახანს დააჭირეს
                        if ((now - last_event_time[i]) > DEBOUNCE_TIME_MS)
                        {
                            ESP_LOGI(TAG, "Button '%s' press confirmed on Expander Pin %d", btn->name, btn->pin);
                            publish_button_event(btn->name);
                            last_event_time[i] = now; // დავიმახსოვროთ დრო, რომ თავიდან ავიცილოთ სპამი
                        }
                    }
                    btn->last_state = is_pressed;
                }
            }
            vTaskDelay(pdMS_TO_TICKS(20));
        }
    }
}

static esp_err_t button_input_init(module_t *self)
{
    button_input_private_data_t *private_data = (button_input_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing Button Input module '%s'", self->name);

    if (private_data->control_type == CONTROL_TYPE_GPIO)
    {
        // ... (GPIO ლოგიკა უცვლელია) ...
        g_button_gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
        private_data->gpio_evt_queue = g_button_gpio_evt_queue;
        if (!private_data->gpio_evt_queue)
            return ESP_ERR_NO_MEM;
        gpio_install_isr_service(0);
        for (int i = 0; i < private_data->button_count; i++)
        {
            button_config_t *btn = &private_data->buttons[i];
            fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, btn->pin, self->name);
            gpio_config_t io_conf = {
                .pin_bit_mask = (1ULL << btn->pin),
                .mode = GPIO_MODE_INPUT,
                .pull_up_en = (btn->active_level == 0),
                .pull_down_en = (btn->active_level == 1),
                .intr_type = GPIO_INTR_ANYEDGE,
            };
            gpio_config(&io_conf);
            gpio_isr_handler_add(btn->pin, gpio_isr_handler, (void *)(intptr_t)btn->pin);
            ESP_LOGI(TAG, "Button '%s' configured on GPIO %d", btn->name, btn->pin);
        }
    }
    else
    { // Expander Mode
        private_data->expander_handle = (mcp23017_handle_t *)fmw_service_get(private_data->expander_service_name);
        if (!private_data->expander_handle)
        {
            ESP_LOGE(TAG, "Expander service '%s' not found!", private_data->expander_service_name);
            return ESP_ERR_NOT_FOUND;
        }

        for (int i = 0; i < private_data->button_count; i++)
        {
            button_config_t *btn = &private_data->buttons[i];
            private_data->expander_handle->api->set_pin_direction(private_data->expander_handle->context, btn->pin, MCP23017_DIRECTION_INPUT);
            private_data->expander_handle->api->set_pin_pullup(private_data->expander_handle->context, btn->pin, true);
            btn->last_state = !(bool)btn->active_level;
            ESP_LOGI(TAG, "Button '%s' configured on Expander Pin %d", btn->name, btn->pin);
        }
    }

    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

static esp_err_t button_input_start(module_t *self)
{
    button_input_private_data_t *private_data = (button_input_private_data_t *)self->private_data;
    BaseType_t ret = xTaskCreate(button_task, self->name, 3072, self, 10, &private_data->task_handle);
    if (ret != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create button task");
        return ESP_FAIL;
    }
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void button_input_deinit(module_t *self)
{
    if (!self)
        return;
    button_input_private_data_t *private_data = (button_input_private_data_t *)self->private_data;
    if (private_data)
    {
        if (private_data->task_handle)
            vTaskDelete(private_data->task_handle);
        if (private_data->control_type == CONTROL_TYPE_GPIO)
        {
            if (private_data->gpio_evt_queue)
            {
                vQueueDelete(private_data->gpio_evt_queue);
                g_button_gpio_evt_queue = NULL;
            }
            for (int i = 0; i < private_data->button_count; i++)
            {
                gpio_isr_handler_remove(private_data->buttons[i].pin);
                fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->buttons[i].pin, self->name);
            }
        }
        free(private_data);
    }
    if (self->current_config)
        cJSON_Delete(self->current_config);
    free(self);
}

static esp_err_t parse_config(const cJSON *config_node, button_input_private_data_t *private_data)
{
    if (!config_node)
        return ESP_ERR_INVALID_ARG;

    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", cJSON_GetObjectItem(config_node, "instance_name")->valuestring);

    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", cJSON_GetObjectItem(config_node, "instance_name")->valuestring);

    const cJSON *control_type_node = cJSON_GetObjectItem(config_node, "control_type");
    if (cJSON_IsString(control_type_node) && strcmp(control_type_node->valuestring, "expander") == 0)
    {
        private_data->control_type = CONTROL_TYPE_EXPANDER;
        const cJSON *expander_service_node = cJSON_GetObjectItem(config_node, "expander_service");
        if (!cJSON_IsString(expander_service_node))
        {
            ESP_LOGE(TAG, "'expander_service' must be specified for expander control type");
            return ESP_ERR_INVALID_ARG;
        }
        snprintf(private_data->expander_service_name, sizeof(private_data->expander_service_name), "%s", expander_service_node->valuestring);
    }
    else
    {
        private_data->control_type = CONTROL_TYPE_GPIO;
    }

    const cJSON *buttons_array = cJSON_GetObjectItem(config_node, "buttons");
    if (!cJSON_IsArray(buttons_array))
        return ESP_ERR_INVALID_ARG;

    private_data->button_count = 0;
    const cJSON *button_node;
    cJSON_ArrayForEach(button_node, buttons_array)
    {
        if (private_data->button_count >= MAX_BUTTONS)
            break;

        const cJSON *btn_name = cJSON_GetObjectItem(button_node, "name");
        const cJSON *btn_pin = (private_data->control_type == CONTROL_TYPE_GPIO)
                                   ? cJSON_GetObjectItem(button_node, "gpio_pin")
                                   : cJSON_GetObjectItem(button_node, "expander_pin");
        const cJSON *btn_level = cJSON_GetObjectItem(button_node, "active_level");

        if (cJSON_IsString(btn_name) && cJSON_IsNumber(btn_pin))
        {
            button_config_t *btn = &private_data->buttons[private_data->button_count];
            snprintf(btn->name, sizeof(btn->name), "%s", btn_name->valuestring);
            btn->pin = btn_pin->valueint;
            btn->active_level = cJSON_IsNumber(btn_level) ? btn_level->valueint : 0;
            private_data->button_count++;
        }
    }
    return ESP_OK;
}