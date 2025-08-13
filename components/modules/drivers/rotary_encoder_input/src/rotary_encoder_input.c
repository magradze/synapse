/**
 * @file rotary_encoder_input.c
 * @brief A universal module to handle rotary encoder inputs using the Task Pool Manager.
 * @author Synapse Framework Team
 * @version 3.0.0
 * @date 2025-09-07
 * @details This module reads rotary encoders and translates their actions into standard
 *          SYNAPSE_EVENT_BUTTON_PRESSED events ("UP", "DOWN", "OK"). It no longer
 *          creates its own task; instead, it schedules a periodic job with the
 *          Shared Task Pool Manager for maximum resource efficiency.
 */

#include "synapse.h"
#include "rotary_encoder_input.h"
#include "mcp23017_interface.h"
#include "driver/gpio.h"
#include <string.h>

DEFINE_COMPONENT_TAG("ROTARY_ENCODER", SYNAPSE_LOG_COLOR_MAGENTA);

// --- Internal Data Structures ---

typedef enum
{
    CONTROL_TYPE_GPIO,
    CONTROL_TYPE_EXPANDER
} control_type_t;

typedef struct
{
    // --- Module Identification & Configuration ---
    module_t *self;
    char instance_name[CONFIG_SYNAPSE_MODULE_NAME_MAX_LENGTH];
    char button_name[16];
    uint8_t active_level;
    uint16_t polling_interval_ms;

    // --- Pin configuration ---
    uint8_t pin_a;
    uint8_t pin_b;
    uint8_t pin_sw;

    // --- Control type and dependencies ---
    control_type_t control_type;
    char expander_service_name[32];
    mcp23017_handle_t *expander_handle;

    // --- State tracking ---
    int8_t encoder_state;
    bool last_sw_state;
    uint32_t last_sw_press_time;

    // --- Task Pool Handle ---
    synapse_job_handle_t job_handle;

} rotary_private_data_t;

// --- Forward Declarations ---

static esp_err_t rotary_encoder_input_init(module_t *self);
static esp_err_t rotary_encoder_input_start(module_t *self);
static void rotary_encoder_input_deinit(module_t *self);
static esp_err_t parse_config(const cJSON *config_node, rotary_private_data_t *private_data);
static void rotary_poll_job(void *user_context); // This is now a job, not a task
static void publish_button_event(const char *button_name);

// --- Factory Function ---

module_t *rotary_encoder_input_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    rotary_private_data_t *private_data = (rotary_private_data_t *)calloc(1, sizeof(rotary_private_data_t));
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
        free(private_data);
        free(module);
        return NULL;
    }

    module->private_data = private_data;
    private_data->self = module; // Store back-pointer for the job context

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (parse_config(config_node, private_data) != ESP_OK)
    {
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

    if (private_data->control_type == CONTROL_TYPE_GPIO)
    {
        synapse_resource_lock(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->pin_a, self->name);
        synapse_resource_lock(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->pin_b, self->name);
        synapse_resource_lock(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->pin_sw, self->name);

        gpio_config_t io_conf = {
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = (private_data->active_level == 0),
            .pull_down_en = (private_data->active_level == 1),
            .intr_type = GPIO_INTR_DISABLE,
        };
        io_conf.pin_bit_mask = (1ULL << private_data->pin_a) | (1ULL << private_data->pin_b) | (1ULL << private_data->pin_sw);
        gpio_config(&io_conf);
    }
    else
    { // Expander Mode
        private_data->expander_handle = (mcp23017_handle_t *)synapse_service_get(private_data->expander_service_name);
        if (!private_data->expander_handle)
        {
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
    rotary_private_data_t *private_data = (rotary_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting '%s' by scheduling a job in the Task Pool.", self->name);

    private_data->job_handle = synapse_task_pool_schedule_job(
        rotary_poll_job,
        private_data, // Pass private_data as context
        private_data->polling_interval_ms,
        true // It's a periodic job
    );

    if (private_data->job_handle == NULL)
    {
        ESP_LOGE(TAG, "Failed to schedule job for '%s'.", self->name);
        return ESP_FAIL;
    }

    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void rotary_encoder_input_deinit(module_t *self)
{
    if (!self)
        return;
    rotary_private_data_t *private_data = (rotary_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Deinitializing '%s'.", self->name);

    if (private_data)
    {
        if (private_data->job_handle)
        {
            synapse_task_pool_cancel_job(private_data->job_handle);
            private_data->job_handle = NULL;
        }

        if (private_data->control_type == CONTROL_TYPE_GPIO)
        {
            synapse_resource_release(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->pin_a, self->name);
            synapse_resource_release(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->pin_b, self->name);
            synapse_resource_release(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->pin_sw, self->name);
        }
        free(private_data);
    }
    if (self->current_config)
    {
        cJSON_Delete(self->current_config);
    }
}

// --- Job Function and Helpers ---

static void rotary_poll_job(void *user_context)
{
    rotary_private_data_t *private_data = (rotary_private_data_t *)user_context;
    const int8_t rot_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

    bool pin_a_level, pin_b_level, pin_sw_level;

    if (private_data->control_type == CONTROL_TYPE_GPIO)
    {
        pin_a_level = gpio_get_level(private_data->pin_a);
        pin_b_level = gpio_get_level(private_data->pin_b);
        pin_sw_level = gpio_get_level(private_data->pin_sw);
    }
    else
    {
        private_data->expander_handle->api->get_pin_level(private_data->expander_handle->context, private_data->pin_a, &pin_a_level);
        private_data->expander_handle->api->get_pin_level(private_data->expander_handle->context, private_data->pin_b, &pin_b_level);
        private_data->expander_handle->api->get_pin_level(private_data->expander_handle->context, private_data->pin_sw, &pin_sw_level);
    }

    uint8_t current_pins = (pin_a_level << 1) | pin_b_level;
    private_data->encoder_state = (private_data->encoder_state << 2) | current_pins;

    int8_t direction = rot_states[private_data->encoder_state & 0x0f];
    if (direction != 0)
    {
        publish_button_event(direction == 1 ? "DOWN" : "UP");
    }

    bool is_sw_pressed = (pin_sw_level == (bool)private_data->active_level);
    if (is_sw_pressed && !private_data->last_sw_state)
    {
        uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;
        if ((now - private_data->last_sw_press_time) > CONFIG_ROTARY_ENCODER_DEBOUNCE_MS)
        {
            publish_button_event(private_data->button_name);
            private_data->last_sw_press_time = now;
        }
    }
    private_data->last_sw_state = is_sw_pressed;
}

static void publish_button_event(const char *button_name)
{
    synapse_button_payload_t *payload = calloc(1, sizeof(synapse_button_payload_t));
    if (!payload)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for button payload");
        return;
    }

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

// --- Configuration Parsing ---

static esp_err_t parse_config(const cJSON *config_node, rotary_private_data_t *private_data)
{
    if (!config_node || !private_data)
    {
        return ESP_ERR_INVALID_ARG;
    }

    // --- Step 1: Set default values from Kconfig ---
    private_data->active_level = 0; // Default active level
    private_data->polling_interval_ms = CONFIG_ROTARY_ENCODER_POLLING_MS;
    synapse_safe_strncpy(private_data->button_name, "OK", sizeof(private_data->button_name));

    // --- Step 2: Parse instance name (required) ---
    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    if (!cJSON_IsString(name_node) || !name_node->valuestring)
    {
        ESP_LOGE(TAG, "Required config 'instance_name' is missing or not a string.");
        return ESP_ERR_INVALID_ARG;
    }
    synapse_safe_strncpy(private_data->instance_name, name_node->valuestring, sizeof(private_data->instance_name));

    // --- Step 3: Parse control type and set the mode ---
    const cJSON *type_node = cJSON_GetObjectItem(config_node, "control_type");
    if (cJSON_IsString(type_node) && strcmp(type_node->valuestring, "expander") == 0)
    {
        private_data->control_type = CONTROL_TYPE_EXPANDER;
    }
    else
    {
        private_data->control_type = CONTROL_TYPE_GPIO; // Default is GPIO
    }

    // --- Step 4: Parse pin numbers (required) ---
    const cJSON *pin_a_node = cJSON_GetObjectItem(config_node, "pin_a");
    const cJSON *pin_b_node = cJSON_GetObjectItem(config_node, "pin_b");
    const cJSON *pin_sw_node = cJSON_GetObjectItem(config_node, "pin_sw");
    if (!cJSON_IsNumber(pin_a_node) || !cJSON_IsNumber(pin_b_node) || !cJSON_IsNumber(pin_sw_node))
    {
        ESP_LOGE(TAG, "Required config parameters (pin_a, pin_b, pin_sw) are missing or not numbers.");
        return ESP_ERR_INVALID_ARG;
    }
    private_data->pin_a = pin_a_node->valueint;
    private_data->pin_b = pin_b_node->valueint;
    private_data->pin_sw = pin_sw_node->valueint;

    // --- Step 5: Parse optional parameters ---
    const cJSON *btn_name_node = cJSON_GetObjectItem(config_node, "button_name");
    if (cJSON_IsString(btn_name_node))
    {
        synapse_safe_strncpy(private_data->button_name, btn_name_node->valuestring, sizeof(private_data->button_name));
    }

    const cJSON *active_level_node = cJSON_GetObjectItem(config_node, "active_level");
    if (cJSON_IsNumber(active_level_node))
    {
        private_data->active_level = active_level_node->valueint;
    }

    const cJSON *polling_node = cJSON_GetObjectItem(config_node, "polling_interval_ms");
    if (cJSON_IsNumber(polling_node))
    {
        private_data->polling_interval_ms = polling_node->valueint;
    }

    // --- Step 6: Validate expander-specific parameters ---
    if (private_data->control_type == CONTROL_TYPE_EXPANDER)
    {
        const cJSON *service_name_node = cJSON_GetObjectItem(config_node, "expander_service");
        if (!cJSON_IsString(service_name_node) || !service_name_node->valuestring)
        {
            ESP_LOGE(TAG, "'expander_service' is required for 'expander' control type.");
            return ESP_ERR_INVALID_ARG;
        }
        synapse_safe_strncpy(private_data->expander_service_name, service_name_node->valuestring, sizeof(private_data->expander_service_name));
    }

    return ESP_OK;
}