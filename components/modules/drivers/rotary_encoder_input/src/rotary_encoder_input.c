/**
 * @file rotary_encoder_input.c
 * @brief A universal module to handle multiple rotary encoder inputs and publish standard navigation events.
 * @author Synapse Framework Team
 * @version 2.0.0
 * @date 2025-08-24
 * @details This module reads multiple rotary encoder's rotation and button presses, debounces the signals,
 *          and translates them into standard FMW_EVENT_BUTTON_PRESSED events with "UP", "DOWN",
 *          and "OK" payloads. It supports both direct GPIO connection and connection via an
 *          MCP23017 I/O expander. All instances are managed by a single, shared FreeRTOS task
 *          for resource efficiency.
 */
#include "rotary_encoder_input.h"
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
#include "freertos/semphr.h"
#include <string.h>

DEFINE_COMPONENT_TAG("ROTARY_ENCODER");

// --- Static Globals for Shared Task Management ---

// Array to hold pointers to all active encoder instances
static module_t* g_encoder_instances[CONFIG_ROTARY_ENCODER_MAX_INSTANCES] = { NULL };
static uint8_t g_active_encoder_count = 0;
static TaskHandle_t g_shared_task_handle = NULL;
static SemaphoreHandle_t g_instance_mutex = NULL;

// --- Internal Data Structures ---

typedef enum {
    CONTROL_TYPE_GPIO,
    CONTROL_TYPE_EXPANDER
} control_type_t;

typedef struct {
    char instance_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];
    
    // Pin configuration
    uint8_t pin_a;
    uint8_t pin_b;
    uint8_t pin_sw;
    uint8_t active_level;
    char button_name[16];

    // Control type and dependencies
    control_type_t control_type;
    char expander_service_name[32];
    mcp23017_handle_t *expander_handle;

    // State tracking for rotation and button press
    int8_t encoder_state;
    bool last_sw_state;
    uint32_t last_sw_press_time;

} rotary_private_data_t;

// --- Forward Declarations ---

static esp_err_t rotary_encoder_input_init(module_t *self);
static esp_err_t rotary_encoder_input_start(module_t *self);
static void rotary_encoder_input_deinit(module_t *self);
static esp_err_t parse_config(const cJSON *config_node, rotary_private_data_t *private_data);
static void rotary_task(void *pvParameters);
static void publish_button_event(const char *button_name);

// --- Factory Function ---

module_t *rotary_encoder_input_create(const cJSON *config)
{
    // Initialize the shared mutex once for the first instance
    if (g_instance_mutex == NULL) {
        g_instance_mutex = xSemaphoreCreateMutex();
        if (g_instance_mutex == NULL) {
            ESP_LOGE(TAG, "Failed to create instance mutex");
            return NULL;
        }
    }

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    rotary_private_data_t *private_data = (rotary_private_data_t *)calloc(1, sizeof(rotary_private_data_t));
    if (!module || !private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        if (config) cJSON_Delete((cJSON *)config);
        return NULL;
    }

    module->current_config = (cJSON *)config;
    module->private_data = private_data;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (parse_config(config_node, private_data) != ESP_OK) {
        ESP_LOGE(TAG, "Configuration parsing failed");
        rotary_encoder_input_deinit(module);
        return NULL;
    }

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->init_level = 60;
    module->base.init = rotary_encoder_input_init;
    module->base.start = rotary_encoder_input_start;
    module->base.deinit = rotary_encoder_input_deinit;

    ESP_LOGI(TAG, "Rotary Encoder module '%s' created (mode: %s).",
             module->name, private_data->control_type == CONTROL_TYPE_GPIO ? "GPIO" : "Expander");
    return module;
}

// --- Lifecycle Functions ---

static esp_err_t rotary_encoder_input_init(module_t *self)
{
    rotary_private_data_t *private_data = (rotary_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing Rotary Encoder module '%s'", self->name);

    if (private_data->control_type == CONTROL_TYPE_GPIO) {
        fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->pin_a, self->name);
        fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->pin_b, self->name);
        fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->pin_sw, self->name);

        gpio_config_t io_conf = {
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = (private_data->active_level == 0),
            .pull_down_en = (private_data->active_level == 1),
            .intr_type = GPIO_INTR_DISABLE,
        };
        io_conf.pin_bit_mask = (1ULL << private_data->pin_a) | (1ULL << private_data->pin_b) | (1ULL << private_data->pin_sw);
        gpio_config(&io_conf);
    } else { // Expander Mode
        private_data->expander_handle = (mcp23017_handle_t *)fmw_service_get(private_data->expander_service_name);
        if (!private_data->expander_handle) {
            ESP_LOGE(TAG, "Expander service '%s' not found!", private_data->expander_service_name);
            return ESP_ERR_NOT_FOUND;
        }
        private_data->expander_handle->api->set_pin_direction(private_data->expander_handle->context, private_data->pin_a, MCP23017_DIRECTION_INPUT);
        private_data->expander_handle->api->set_pin_pullup(private_data->expander_handle->context, private_data->pin_a, true);
        private_data->expander_handle->api->set_pin_direction(private_data->expander_handle->context, private_data->pin_b, MCP23017_DIRECTION_INPUT);
        private_data->expander_handle->api->set_pin_pullup(private_data->expander_handle->context, private_data->pin_b, true);
        private_data->expander_handle->api->set_pin_direction(private_data->expander_handle->context, private_data->pin_sw, MCP23017_DIRECTION_INPUT);
        private_data->expander_handle->api->set_pin_pullup(private_data->expander_handle->context, private_data->pin_sw, true);
    }

    private_data->encoder_state = 0;
    private_data->last_sw_state = !(bool)private_data->active_level;
    private_data->last_sw_press_time = 0;

    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

static esp_err_t rotary_encoder_input_start(module_t *self)
{
    if (xSemaphoreTake(g_instance_mutex, portMAX_DELAY) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    // Add this instance to the global array
    if (g_active_encoder_count < CONFIG_ROTARY_ENCODER_MAX_INSTANCES) {
        g_encoder_instances[g_active_encoder_count++] = self;
    } else {
        ESP_LOGE(TAG, "Cannot start encoder '%s', max instance count reached!", self->name);
        xSemaphoreGive(g_instance_mutex);
        return ESP_ERR_NO_MEM;
    }

    // Start the shared task only if it's not already running
    if (g_shared_task_handle == NULL) {
        BaseType_t ret = xTaskCreate(rotary_task, "rotary_shared_task", CONFIG_ROTARY_ENCODER_TASK_STACK_SIZE, NULL, CONFIG_ROTARY_ENCODER_TASK_PRIORITY, &g_shared_task_handle);
        if (ret != pdPASS) {
            ESP_LOGE(TAG, "Failed to create shared rotary task");
            g_active_encoder_count--; // Rollback
            xSemaphoreGive(g_instance_mutex);
            return ESP_FAIL;
        }
        ESP_LOGI(TAG, "Shared rotary task started.");
    }

    xSemaphoreGive(g_instance_mutex);
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void rotary_encoder_input_deinit(module_t *self)
{
    if (!self) return;

    if (xSemaphoreTake(g_instance_mutex, portMAX_DELAY) == pdTRUE) {
        // Remove this instance from the global array
        for (int i = 0; i < g_active_encoder_count; i++) {
            if (g_encoder_instances[i] == self) {
                // Shift remaining elements to the left
                for (int j = i; j < g_active_encoder_count - 1; j++) {
                    g_encoder_instances[j] = g_encoder_instances[j + 1];
                }
                g_encoder_instances[g_active_encoder_count - 1] = NULL;
                g_active_encoder_count--;
                break;
            }
        }

        // If no instances are left, delete the shared task
        if (g_active_encoder_count == 0 && g_shared_task_handle != NULL) {
            vTaskDelete(g_shared_task_handle);
            g_shared_task_handle = NULL;
            ESP_LOGI(TAG, "Shared rotary task deleted.");
        }
        xSemaphoreGive(g_instance_mutex);
    }

    rotary_private_data_t *private_data = (rotary_private_data_t *)self->private_data;
    if (private_data) {
        if (private_data->control_type == CONTROL_TYPE_GPIO) {
            fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->pin_a, self->name);
            fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->pin_b, self->name);
            fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->pin_sw, self->name);
        }
        free(private_data);
    }
    if (self->current_config) cJSON_Delete(self->current_config);
    free(self);
}

// --- Task and Helper Functions ---

static void rotary_task(void *pvParameters)
{
    const int8_t rot_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

    while (1) {
        if (xSemaphoreTake(g_instance_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            
            for (int i = 0; i < g_active_encoder_count; i++) {
                module_t *self = g_encoder_instances[i];
                rotary_private_data_t *private_data = (rotary_private_data_t *)self->private_data;

                bool pin_a_level, pin_b_level, pin_sw_level;

                if (private_data->control_type == CONTROL_TYPE_GPIO) {
                    pin_a_level = gpio_get_level(private_data->pin_a);
                    pin_b_level = gpio_get_level(private_data->pin_b);
                    pin_sw_level = gpio_get_level(private_data->pin_sw);
                } else {
                    private_data->expander_handle->api->get_pin_level(private_data->expander_handle->context, private_data->pin_a, &pin_a_level);
                    private_data->expander_handle->api->get_pin_level(private_data->expander_handle->context, private_data->pin_b, &pin_b_level);
                    private_data->expander_handle->api->get_pin_level(private_data->expander_handle->context, private_data->pin_sw, &pin_sw_level);
                }

                uint8_t current_pins = (pin_a_level << 1) | pin_b_level;
                private_data->encoder_state = (private_data->encoder_state << 2) | current_pins;

                int8_t direction = rot_states[private_data->encoder_state & 0x0f];
                if (direction != 0) {
                    publish_button_event(direction == 1 ? "DOWN" : "UP");
                }

                bool is_sw_pressed = (pin_sw_level == (bool)private_data->active_level);
                if (is_sw_pressed && !private_data->last_sw_state) {
                    uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;
                    if ((now - private_data->last_sw_press_time) > CONFIG_ROTARY_ENCODER_DEBOUNCE_MS) {
                        publish_button_event(private_data->button_name);
                        private_data->last_sw_press_time = now;
                    }
                }
                private_data->last_sw_state = is_sw_pressed;
            }
            xSemaphoreGive(g_instance_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(CONFIG_ROTARY_ENCODER_POLLING_MS));
    }
}

static void publish_button_event(const char *button_name)
{
    fmw_button_payload_t *payload = calloc(1, sizeof(fmw_button_payload_t));
    if (!payload) {
        ESP_LOGE(TAG, "Failed to allocate memory for button payload");
        return;
    }
    
    snprintf(payload->button_name, sizeof(payload->button_name), "%s", button_name);

    event_data_wrapper_t *wrapper;
    if (fmw_event_data_wrap(payload, fmw_payload_common_free, &wrapper) == ESP_OK) {
        fmw_event_bus_post(FMW_EVENT_BUTTON_PRESSED, wrapper);
        fmw_event_data_release(wrapper);
    } else {
        free(payload);
    }
}

// --- Configuration Parsing ---

static esp_err_t parse_config(const cJSON *config_node, rotary_private_data_t *private_data)
{
    if (!config_node) return ESP_ERR_INVALID_ARG;

    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    if (!cJSON_IsString(name_node)) return ESP_ERR_INVALID_ARG;
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", name_node->valuestring);

    const cJSON *control_type_node = cJSON_GetObjectItem(config_node, "control_type");
    if (cJSON_IsString(control_type_node) && strcmp(control_type_node->valuestring, "expander") == 0) {
        private_data->control_type = CONTROL_TYPE_EXPANDER;
        const cJSON *expander_service_node = cJSON_GetObjectItem(config_node, "expander_service");
        if (!cJSON_IsString(expander_service_node)) {
            ESP_LOGE(TAG, "'expander_service' is required for expander control type");
            return ESP_ERR_INVALID_ARG;
        }
        snprintf(private_data->expander_service_name, sizeof(private_data->expander_service_name), "%s", expander_service_node->valuestring);
    } else {
        private_data->control_type = CONTROL_TYPE_GPIO;
    }

    const cJSON *pin_a_node = cJSON_GetObjectItem(config_node, "pin_a");
    const cJSON *pin_b_node = cJSON_GetObjectItem(config_node, "pin_b");
    const cJSON *pin_sw_node = cJSON_GetObjectItem(config_node, "pin_sw");
    if (!cJSON_IsNumber(pin_a_node) || !cJSON_IsNumber(pin_b_node) || !cJSON_IsNumber(pin_sw_node)) {
        ESP_LOGE(TAG, "pin_a, pin_b, and pin_sw are required numeric parameters");
        return ESP_ERR_INVALID_ARG;
    }
    private_data->pin_a = pin_a_node->valueint;
    private_data->pin_b = pin_b_node->valueint;
    private_data->pin_sw = pin_sw_node->valueint;

    const cJSON *btn_name_node = cJSON_GetObjectItem(config_node, "button_name");
    if (cJSON_IsString(btn_name_node)) {
        snprintf(private_data->button_name, sizeof(private_data->button_name), "%s", btn_name_node->valuestring);
    } else {
        snprintf(private_data->button_name, sizeof(private_data->button_name), "OK");
    }

    const cJSON *level_node = cJSON_GetObjectItem(config_node, "active_level");
    private_data->active_level = cJSON_IsNumber(level_node) ? level_node->valueint : 0;

    return ESP_OK;
}