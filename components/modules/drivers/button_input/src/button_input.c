/**
 * @file button_input.c
 * @brief A module to handle hardware button presses from GPIO or I/O expanders.
 * @author Synapse Team
 * @version 3.0.0
 * @date 2025-08-27
 * @details This module provides a robust and flexible way to handle user button
 *          inputs. It supports multiple instances, can operate on direct GPIOs
 *          (using interrupts) or via an MCP23017 I/O expander (using polling),
 *          and is fully compatible with the framework's dependency injection system.
 *          It is designed to be multi-instance safe by avoiding global variables.
 */

#include "button_input.h"
#include "synapse.h"
#include "mcp23017_interface.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <stddef.h> // For offsetof

DEFINE_COMPONENT_TAG("BUTTON_INPUT");

#define MAX_BUTTONS_PER_INSTANCE 16
#define DEBOUNCE_TIME_MS 50
#define POLLING_INTERVAL_MS 20
#define GPIO_INTR_QUEUE_LEN 10

// --- Internal Data Structures ---

typedef enum
{
    CONTROL_TYPE_GPIO,
    CONTROL_TYPE_EXPANDER
} control_type_t;

typedef struct
{
    char name[16];
    uint8_t pin;
    uint8_t active_level;
    bool last_state;
    uint32_t last_state_change_time;
} button_config_t;

typedef struct
{
    // --- Injected Dependencies (MUST BE FIRST) ---
    mcp23017_handle_t *expander_handle;

    // --- Module State & Configuration ---
    char instance_name[CONFIG_SYNAPSE_MODULE_NAME_MAX_LENGTH];
    TaskHandle_t task_handle;
    QueueHandle_t gpio_evt_queue; // Each instance has its own queue
    button_config_t buttons[MAX_BUTTONS_PER_INSTANCE];
    uint8_t button_count;
    control_type_t control_type;
} button_input_private_data_t;

// --- Forward Declarations ---
static esp_err_t button_input_init(module_t *self);
static esp_err_t button_input_start(module_t *self);
static void button_input_deinit(module_t *self);
static esp_err_t parse_config(const cJSON *config_node, button_input_private_data_t *private_data);
static void button_task(void *pvParameters);
static void gpio_isr_handler(void *arg);

// --- Dependency Map ---
static const module_dependency_t s_dependencies[] = {
    {"expander_service", offsetof(button_input_private_data_t, expander_handle)},
    {NULL, 0} // Terminator
};

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

    module->current_config = cJSON_Duplicate(config, true);
    if (!module->current_config)
    {
        ESP_LOGE(TAG, "Failed to duplicate configuration object.");
        // Note: This assumes 'private_data' and 'module' are allocated.
        // Manual check might be needed for each file's cleanup logic.
        free(private_data);
        free(module);
        return NULL;
    }
    module->private_data = private_data;
    module->dependency_map = s_dependencies; // Assign the dependency map

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
    synapse_button_payload_t *payload = calloc(1, sizeof(synapse_button_payload_t));
    if (payload)
    {
        snprintf(payload->button_name, sizeof(payload->button_name), "%s", button_name);
        event_data_wrapper_t *wrapper;
        if (synapse_event_data_wrap(payload, synapse_payload_common_free, &wrapper) == ESP_OK)
        {
            synapse_event_bus_post(SYNAPSE_EVENT_BUTTON_PRESSED, wrapper);
            synapse_event_data_release(wrapper);
        }
        else
        {
            free(payload);
        }
    }
}

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    module_t *self = (module_t *)arg;
    button_input_private_data_t *private_data = (button_input_private_data_t *)self->private_data;
    // We send the module pointer itself to the queue
    xQueueSendFromISR(private_data->gpio_evt_queue, &self, NULL);
}

static void button_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    button_input_private_data_t *private_data = (button_input_private_data_t *)self->private_data;

    if (private_data->control_type == CONTROL_TYPE_EXPANDER)
    {
        while (1)
        {
            uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;
            for (int i = 0; i < private_data->button_count; i++)
            {
                button_config_t *btn = &private_data->buttons[i];
                bool current_pin_level;
                esp_err_t ret = private_data->expander_handle->api->get_pin_level(
                    private_data->expander_handle->context, btn->pin, &current_pin_level);

                if (ret != ESP_OK)
                    continue;

                bool is_pressed = (current_pin_level == (bool)btn->active_level);
                if (is_pressed && !btn->last_state)
                {
                    if ((now - btn->last_state_change_time) > DEBOUNCE_TIME_MS)
                    {
                        ESP_LOGD(TAG, "Button '%s' PRESSED on Expander Pin %d", btn->name, btn->pin);
                        publish_button_event(btn->name);
                    }
                }
                if (is_pressed != btn->last_state)
                {
                    btn->last_state = is_pressed;
                    btn->last_state_change_time = now;
                }
            }
            vTaskDelay(pdMS_TO_TICKS(POLLING_INTERVAL_MS));
        }
    }
    else // CONTROL_TYPE_GPIO
    {
        module_t *triggered_module;
        for (;;)
        {
            if (xQueueReceive(private_data->gpio_evt_queue, &triggered_module, portMAX_DELAY))
            {
                // This task is shared, but the ISR sends the specific module context
                button_input_private_data_t *p_data = (button_input_private_data_t *)triggered_module->private_data;

                // Simple debounce by delaying after any interrupt
                vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_TIME_MS));

                for (int i = 0; i < p_data->button_count; i++)
                {
                    button_config_t *btn = &p_data->buttons[i];
                    bool is_pressed = (gpio_get_level(btn->pin) == btn->active_level);
                    if (is_pressed && !btn->last_state)
                    {
                        ESP_LOGI(TAG, "Button '%s' PRESSED on GPIO %d", btn->name, btn->pin);
                        publish_button_event(btn->name);
                    }
                    btn->last_state = is_pressed;
                }
            }
        }
    }
}

static esp_err_t button_input_init(module_t *self)
{
    button_input_private_data_t *private_data = (button_input_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing Button Input module '%s'", self->name);

    if (private_data->control_type == CONTROL_TYPE_GPIO)
    {
        private_data->gpio_evt_queue = xQueueCreate(GPIO_INTR_QUEUE_LEN, sizeof(module_t *));
        if (!private_data->gpio_evt_queue)
            return ESP_ERR_NO_MEM;

        gpio_install_isr_service(0);
        for (int i = 0; i < private_data->button_count; i++)
        {
            button_config_t *btn = &private_data->buttons[i];
            synapse_resource_lock(SYNAPSE_RESOURCE_TYPE_GPIO, btn->pin, self->name);
            gpio_config_t io_conf = {
                .pin_bit_mask = (1ULL << btn->pin),
                .mode = GPIO_MODE_INPUT,
                .pull_up_en = (btn->active_level == 0),
                .pull_down_en = (btn->active_level == 1),
                .intr_type = GPIO_INTR_ANYEDGE,
            };
            gpio_config(&io_conf);
            // Pass the module instance pointer as the ISR argument
            gpio_isr_handler_add(btn->pin, gpio_isr_handler, (void *)self);
            ESP_LOGI(TAG, "Button '%s' configured on GPIO %d", btn->name, btn->pin);
        }
    }
    else // Expander Mode
    {
        // Validate the injected handle
        if (!private_data->expander_handle)
        {
            ESP_LOGE(TAG, "Dependency injection failed: expander_handle is NULL for '%s'!", self->name);
            return ESP_ERR_INVALID_STATE;
        }

        for (int i = 0; i < private_data->button_count; i++)
        {
            button_config_t *btn = &private_data->buttons[i];
            private_data->expander_handle->api->set_pin_direction(private_data->expander_handle->context, btn->pin, MCP23017_DIRECTION_INPUT);
            private_data->expander_handle->api->set_pin_pullup(private_data->expander_handle->context, btn->pin, true);
            btn->last_state = !(bool)btn->active_level;
            btn->last_state_change_time = 0;
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
            }
            for (int i = 0; i < private_data->button_count; i++)
            {
                gpio_isr_handler_remove(private_data->buttons[i].pin);
                synapse_resource_release(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->buttons[i].pin, self->name);
            }
        }
        free(private_data);
    }
    if (self->current_config)
        cJSON_Delete(self->current_config);
}

static esp_err_t parse_config(const cJSON *config_node, button_input_private_data_t *private_data)
{
    if (!config_node)
        return ESP_ERR_INVALID_ARG;

    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", name_node->valuestring);

    const cJSON *control_type_node = cJSON_GetObjectItem(config_node, "control_type");
    if (cJSON_IsString(control_type_node) && strcmp(control_type_node->valuestring, "expander") == 0)
    {
        private_data->control_type = CONTROL_TYPE_EXPANDER;
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
        if (private_data->button_count >= MAX_BUTTONS_PER_INSTANCE)
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