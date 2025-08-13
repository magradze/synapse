/**
 * @file relay_actuator.c
 * @brief Implementation of the Relay Actuator module's lifecycle and core logic.
 * @author Synapse Framework Team
 * @version 2.0.0
 * @date 2025-09-12
 */
#include "synapse.h"
#include "relay_actuator_internal.h"
#include "cmd_router_interface.h"
#include "relay_interface.h"

DEFINE_COMPONENT_TAG("RELAY_ACTUATOR", SYNAPSE_LOG_COLOR_GREEN);

// --- Static Globals for CLI handler ---
module_t *s_relay_instances[CONFIG_RELAY_ACTUATOR_MAX_INSTANCES];
uint8_t s_relay_instance_count = 0;

// --- Service API Table ---
static const relay_api_t s_relay_service_api = {
    .set_state = relay_api_set_state,
    .toggle = relay_api_toggle,
    .get_state = relay_api_get_state,
};

// --- Internal Function Prototypes ---
static esp_err_t parse_relay_config(const cJSON *config, relay_private_data_t *p_data, char *instance_name_buf, size_t buf_size);
static void publish_state_change(module_t *self);
static esp_err_t save_state_to_storage(module_t *self);

// =========================================================================
//                      Module Lifecycle Implementation
// =========================================================================

module_t *relay_actuator_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    relay_private_data_t *p_data = (relay_private_data_t *)calloc(1, sizeof(relay_private_data_t));
    SYNAPSE_GUARD(module && p_data, TAG, NULL, "Failed to allocate memory");

    module->private_data = p_data;
    module->current_config = cJSON_Duplicate(config, true);
    if (!module->current_config)
    {
        ESP_LOGE(TAG, "Failed to duplicate config");
        free(p_data);
        free(module);
        return NULL;
    }

    if (parse_relay_config(config, p_data, module->name, sizeof(module->name)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to parse configuration. Aborting creation.");
        relay_actuator_deinit(module); // Uses the deinit function for full cleanup
        return NULL;
    }

    module->init_level = 60;
    module->status = MODULE_STATUS_UNINITIALIZED;

    module->base.init = relay_actuator_init;
    module->base.start = relay_actuator_start;
    module->base.deinit = relay_actuator_deinit;
    module->base.handle_event = relay_module_handle_event;
    module->base.ui_init = relay_ui_init;
    module->base.ui_deinit = relay_ui_deinit;

    esp_err_t ret = synapse_service_register_with_status(
        module->name, SYNAPSE_SERVICE_TYPE_RELAY_API, (void *)&s_relay_service_api, SERVICE_STATUS_REGISTERED);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register service for '%s' (%s).", module->name, esp_err_to_name(ret));
        relay_actuator_deinit(module);
        return NULL;
    }

    if (s_relay_instance_count < CONFIG_RELAY_ACTUATOR_MAX_INSTANCES)
    {
        s_relay_instances[s_relay_instance_count++] = module;
    }
    else
    {
        ESP_LOGE(TAG, "Cannot create more relay instances, limit reached.");
        relay_actuator_deinit(module);
        return NULL;
    }

    ESP_LOGI(TAG, "Relay module created: '%s' on GPIO %d", module->name, p_data->gpio_pin);
    return module;
}

esp_err_t relay_actuator_init(module_t *self)
{
    relay_private_data_t *p_data = (relay_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing module: %s", self->name);

    esp_err_t err = synapse_resource_lock(SYNAPSE_RESOURCE_TYPE_GPIO, p_data->gpio_pin, self->name);
    if (err != ESP_OK)
    {
        const char *owner = NULL;
        synapse_resource_get_owner(SYNAPSE_RESOURCE_TYPE_GPIO, p_data->gpio_pin, &owner);
        ESP_LOGE(TAG, "Failed to lock GPIO %d. Owned by '%s'.", p_data->gpio_pin, owner ? owner : "unknown");
        return err;
    }

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << p_data->gpio_pin),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&io_conf);

    synapse_event_bus_subscribe(SYNAPSE_EVENT_SYSTEM_START_COMPLETE, self);
    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

esp_err_t relay_actuator_start(module_t *self)
{
    relay_private_data_t *p_data = (relay_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting module: %s", self->name);

    bool initial_state_on = false;
    if (p_data->initial_state == INITIAL_STATE_ON)
    {
        initial_state_on = true;
    }
    else if (p_data->initial_state == INITIAL_STATE_LAST)
    {
        storage_api_t *storage = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_NVRAM_API);
        if (storage)
        {
            storage->get_bool(self->name, "last_state", &initial_state_on);
        }
    }

    set_relay_state(self, initial_state_on);
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

void relay_actuator_deinit(module_t *self)
{
    if (!self)
        return;
    relay_private_data_t *p_data = (relay_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Deinitializing module: %s", self->name);

    synapse_service_unregister(self->name);
    if (p_data)
    {
        synapse_resource_release(SYNAPSE_RESOURCE_TYPE_GPIO, p_data->gpio_pin, self->name);
    }
    synapse_event_bus_unsubscribe(SYNAPSE_EVENT_SYSTEM_START_COMPLETE, self);

    // Remove instance from static list
    for (int i = 0; i < s_relay_instance_count; i++)
    {
        if (s_relay_instances[i] == self)
        {
            for (int j = i; j < s_relay_instance_count - 1; j++)
            {
                s_relay_instances[j] = s_relay_instances[j + 1];
            }
            s_relay_instance_count--;
            break;
        }
    }

    // Unregister CLI command only if this was the last relay instance
    if (s_relay_instance_count == 0)
    {
        cmd_router_api_t *cmd_router = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_CMD_ROUTER_API);
        if (cmd_router && cmd_router->is_command_registered("relay"))
        {
            cmd_router->unregister_command("relay");
            ESP_LOGI(TAG, "Unregistered generic 'relay' command as last instance is being deinitialized.");
        }
    }

    if (self->current_config)
        cJSON_Delete(self->current_config);
    free(p_data);
    // The module_t struct itself is freed by the System Manager
}

// =========================================================================
//                      Service API Implementation
// =========================================================================

esp_err_t relay_api_set_state(void *context, bool is_on)
{
    return set_relay_state((module_t *)context, is_on);
}

esp_err_t relay_api_toggle(void *context)
{
    module_t *self = (module_t *)context;
    SYNAPSE_GUARD(self, TAG, ESP_ERR_INVALID_ARG, "Context cannot be NULL");
    relay_private_data_t *p_data = (relay_private_data_t *)self->private_data;
    return set_relay_state(self, !p_data->current_state);
}

bool relay_api_get_state(void *context)
{
    module_t *self = (module_t *)context;
    if (!self)
        return false;
    return ((relay_private_data_t *)self->private_data)->current_state;
}

// =========================================================================
//                      Internal Helper Functions
// =========================================================================

esp_err_t set_relay_state(module_t *self, bool is_on)
{
    SYNAPSE_GUARD(self, TAG, ESP_ERR_INVALID_ARG, "Module handle is NULL");
    relay_private_data_t *p_data = (relay_private_data_t *)self->private_data;

    gpio_set_level(p_data->gpio_pin, is_on ? p_data->active_level : !p_data->active_level);

    if (p_data->current_state != is_on)
    {
        p_data->current_state = is_on;
        ESP_LOGI(TAG, "Relay '%s' state changed to: %s", self->name, is_on ? "ON" : "OFF");
        publish_state_change(self);
        if (p_data->initial_state == INITIAL_STATE_LAST)
        {
            save_state_to_storage(self);
        }
    }
    return ESP_OK;
}

static void publish_state_change(module_t *self)
{
    relay_private_data_t *p_data = (relay_private_data_t *)self->private_data;
    cJSON *root = cJSON_CreateObject();
    if (!root)
        return;
    cJSON_AddStringToObject(root, "state", p_data->current_state ? "on" : "off");
    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (json_string)
    {
        synapse_telemetry_payload_t *payload = malloc(sizeof(synapse_telemetry_payload_t));
        if (payload)
        {
            synapse_safe_strncpy(payload->module_name, self->name, sizeof(payload->module_name));
            payload->json_data = json_string; // Ownership transferred
            event_data_wrapper_t *wrapper;
            if (synapse_event_data_wrap(payload, synapse_telemetry_payload_free, &wrapper) == ESP_OK)
            {
                synapse_event_bus_post(SYNAPSE_EVENT_RELAY_STATE_CHANGED, wrapper);
                synapse_event_data_release(wrapper);
            }
            else
            {
                free(json_string);
                free(payload);
            }
        }
        else
        {
            free(json_string);
        }
    }
}

static esp_err_t save_state_to_storage(module_t *self)
{
    relay_private_data_t *p_data = (relay_private_data_t *)self->private_data;
    storage_api_t *storage = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_NVRAM_API);
    if (storage)
    {
        return storage->set_bool(self->name, "last_state", p_data->current_state);
    }
    return ESP_ERR_NOT_FOUND;
}

static esp_err_t parse_relay_config(const cJSON *config, relay_private_data_t *p_data, char *instance_name_buf, size_t buf_size)
{
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    SYNAPSE_GUARD(cJSON_IsObject(config_node), TAG, ESP_ERR_INVALID_ARG, "Config object missing");

    if (!synapse_config_get_string_from_node(TAG, config_node, "instance_name", instance_name_buf, buf_size))
    {
        ESP_LOGE(TAG, "Mandatory 'instance_name' missing in config");
        return ESP_ERR_INVALID_ARG;
    }

    int pin = -1;
    if (!synapse_config_get_int_from_node(TAG, config_node, "gpio_pin", &pin) || pin < 0)
    {
        ESP_LOGE(TAG, "Mandatory 'gpio_pin' missing or invalid in config");
        return ESP_ERR_INVALID_ARG;
    }
    p_data->gpio_pin = (gpio_num_t)pin;

    int level = 1; // Default active level
    synapse_config_get_int_from_node(TAG, config_node, "active_level", &level);
    p_data->active_level = (level == 0) ? 0 : 1;

    char state_str[8] = "off";
    synapse_config_get_string_from_node(TAG, config_node, "initial_state", state_str, sizeof(state_str));
    if (strcmp(state_str, "on") == 0)
        p_data->initial_state = INITIAL_STATE_ON;
    else if (strcmp(state_str, "last") == 0)
        p_data->initial_state = INITIAL_STATE_LAST;
    else
        p_data->initial_state = INITIAL_STATE_OFF;

    return ESP_OK;
}