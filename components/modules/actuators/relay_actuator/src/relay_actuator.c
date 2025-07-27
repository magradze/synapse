/**
 * @file relay_actuator.c
 * @brief Implementation of the Relay Actuator module.
 * @author Synapse Framework Team
 * @version 1.3.0
 * @date 2025-07-19
 * @details This module provides a standardized interface to control a relay.
 *          It is controlled via a CLI command and publishes state change
 *          events to the Event Bus for system-wide status updates. It uses the
 *          Resource Manager to lock its GPIO pin and the Storage Manager to
 *          persist its state if configured to do so. This version is simplified
 *          and does not expose a direct Service API, as control is centralized
 *          through the Command Router.
 */

// --- Framework & System Includes ---
#include "relay_actuator.h"
#include "base_module.h"
#include "logging.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "event_payloads.h"
#include "framework_events.h"
#include "cmd_router_interface.h"
#include "resource_manager.h"
#include "storage_interface.h"
#include "service_locator.h"
#include "relay_interface.h"
#include "service_types.h"

// --- ESP-IDF & Standard Lib Includes ---
#include "esp_log.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("RELAY_MODULE");

// =========================================================================
//                      Private Data & Definitions
// =========================================================================

/**
 * @internal
 * @brief A static array to hold pointers to all created relay instances.
 * @details This allows the CLI handler to find the correct module instance
 *          by its name, enabling control over multiple relay instances.
 */
static module_t *relay_instances[CONFIG_RELAY_ACTUATOR_MAX_INSTANCES];
static uint8_t relay_instance_count = 0;

/** @internal @enum initial_state_mode_t Defines the relay's power-on behavior. */
typedef enum
{
    INITIAL_STATE_OFF,
    INITIAL_STATE_ON,
    INITIAL_STATE_LAST, // Restore the last known state from storage
} initial_state_mode_t;

/** @internal @struct relay_private_data_t Private data for the Relay module. */
typedef struct
{
    char instance_name[CONFIG_RELAY_ACTUATOR_INSTANCE_NAME_MAX_LEN];
    // Configuration
    gpio_num_t gpio_pin;
    uint8_t active_level;
    initial_state_mode_t initial_state;
    // Runtime State
    bool current_state; // true = ON, false = OFF
} relay_private_data_t;

// --- Forward Declarations ---
static esp_err_t relay_module_init(module_t *self);
static esp_err_t relay_module_start(module_t *self);
static void relay_module_deinit(module_t *self);
static void relay_module_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t cmd_handler(int argc, char **argv, void *context);
static esp_err_t parse_relay_config(const cJSON *config, relay_private_data_t *private_data);
static void publish_state_change(module_t *self);
static esp_err_t save_state_to_storage(module_t *self);
static esp_err_t set_relay_state(module_t *self, bool is_on);

// --- Forward Declarations for Service API ---
static esp_err_t api_set_state(void *context, bool is_on);
static esp_err_t api_toggle(void *context);
static bool api_get_state(void *context);

/**
 * @internal
 * @brief The static instance of the Service API function table for relays.
 */
static const relay_api_t relay_service_api = {
    .set_state = api_set_state,
    .toggle = api_toggle,
    .get_state = api_get_state,
};

// =========================================================================
//                      Module Lifecycle Implementation
// =========================================================================

module_t *relay_actuator_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    relay_private_data_t *private_data = (relay_private_data_t *)calloc(1, sizeof(relay_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        return NULL;
    }

    module->private_data = private_data;

    if (parse_relay_config(config, private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to parse configuration. Aborting creation.");
        free(private_data);
        free(module);
        return NULL;
    }

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->init_level = 60;
    module->status = MODULE_STATUS_UNINITIALIZED;

    module->base.init = relay_module_init;
    module->base.start = relay_module_start;
    module->base.deinit = relay_module_deinit;
    module->base.handle_event = relay_module_handle_event;

    if (relay_instance_count < CONFIG_RELAY_ACTUATOR_MAX_INSTANCES)
    {
        relay_instances[relay_instance_count++] = module;
    }
    else
    {
        ESP_LOGE(TAG, "Cannot create more relay instances, limit reached.");
        free(private_data);
        free(module);
        return NULL;
    }

    ESP_LOGI(TAG, "Relay module created: '%s' on GPIO %d", private_data->instance_name, private_data->gpio_pin);
    return module;
}

static esp_err_t relay_module_init(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    relay_private_data_t *private_data = (relay_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing module: %s", self->name);

    esp_err_t err = fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->gpio_pin, self->name);
    if (err != ESP_OK)
    {
        const char *owner = NULL;
        fmw_resource_get_owner(FMW_RESOURCE_TYPE_GPIO, private_data->gpio_pin, &owner);
        ESP_LOGE(TAG, "Failed to lock GPIO %d. It is already owned by '%s'.", private_data->gpio_pin, owner ? owner : "unknown");
        return err;
    }

    gpio_config_t io_conf = {.pin_bit_mask = (1ULL << private_data->gpio_pin), .mode = GPIO_MODE_OUTPUT};
    gpio_config(&io_conf);

    // Register the service API for this specific instance
    esp_err_t service_err = fmw_service_register(self->name, FMW_SERVICE_TYPE_RELAY_API, (void *)&relay_service_api);
    if (service_err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register relay service for '%s'", self->name);
        // Cleanup locked resource before failing
        fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->gpio_pin, self->name);
        return service_err;
    }

    fmw_event_bus_subscribe(FMW_EVENT_SYSTEM_START_COMPLETE, self);

    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

static esp_err_t relay_module_start(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    relay_private_data_t *private_data = (relay_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting module: %s", self->name);

    bool initial_state_on = false;
    if (private_data->initial_state == INITIAL_STATE_ON)
    {
        initial_state_on = true;
    }
    else if (private_data->initial_state == INITIAL_STATE_LAST)
    {
        service_handle_t storage = fmw_service_lookup_by_type(FMW_SERVICE_TYPE_NVRAM_API);
        if (storage)
        {
            ((storage_api_t *)storage)->get_bool(self->name, "last_state", &initial_state_on);
        }
    }

    set_relay_state(self, initial_state_on);
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void relay_module_deinit(module_t *self)
{
    if (!self)
        return;
    relay_private_data_t *private_data = (relay_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Deinitializing module: %s", self->name);

    fmw_service_unregister(self->name);

    fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->gpio_pin, self->name);
    fmw_event_bus_unsubscribe(FMW_EVENT_SYSTEM_START_COMPLETE, self);

    service_handle_t cmd_router = fmw_service_get("main_cmd_router");
    if (cmd_router)
    {
        ((cmd_router_api_t *)cmd_router)->unregister_command("relay");
    }

    for (int i = 0; i < relay_instance_count; i++)
    {
        if (relay_instances[i] == self)
        {
            for (int j = i; j < relay_instance_count - 1; j++)
            {
                relay_instances[j] = relay_instances[j + 1];
            }
            relay_instance_count--;
            break;
        }
    }

    free(private_data);
    free(self);
}

/**
 * @internal
 * @brief Handles events from the Event Bus.
 * @details This function's primary role is to register the generic 'relay'
 *          command once the system has fully started. It uses the new
 *          `is_command_registered` API to ensure that only the first
 *          relay_actuator instance performs this registration, avoiding conflicts.
 */
static void relay_module_handle_event(module_t *self, const char *event_name, void *event_data)
{
    // We only care about the system start event.
    if (strcmp(event_name, FMW_EVENT_SYSTEM_START_COMPLETE) == 0)
    {

        service_handle_t cmd_router = fmw_service_get("main_cmd_router");
        if (cmd_router)
        {
            cmd_router_api_t *cmd_api = (cmd_router_api_t *)cmd_router;

            // --- START OF NEW, CONFLICT-FREE REGISTRATION LOGIC ---
            if (!cmd_api->is_command_registered("relay"))
            {
                ESP_LOGI(TAG, "Module '%s' is registering the generic 'relay' command.", self->name);

                // This static struct is shared by all relay instances, but that's okay
                // because it's registered only once and the handler is generic.
                static cmd_t relay_cmd = {
                    .command = "relay",
                    .help = "Controls a relay actuator.",
                    .usage = "relay <instance_name> <on|off|toggle>",
                    .min_args = 3,
                    .max_args = 3,
                    .handler = cmd_handler,
                    .context = NULL,
                };

                esp_err_t err = cmd_api->register_command(&relay_cmd);
                if (err != ESP_OK)
                {
                    ESP_LOGE(TAG, "Failed to register 'relay' command: %s", esp_err_to_name(err));
                }
            }
            else
            {
                ESP_LOGD(TAG, "Module '%s' sees that 'relay' command is already registered. Skipping.", self->name);
            }
            // --- END OF NEW LOGIC ---
        }
    }

    // Always release the data wrapper if it exists
    if (event_data)
    {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// =========================================================================
//                      CLI Implementation
// =========================================================================

/**
 * @internal
 * @brief Handler for the 'relay' CLI command.
 * @details This function is registered with the Command Router. It finds the
 *          target relay module by its instance name from the static instance
 *          list and calls the internal `set_relay_state` function.
 */
static esp_err_t cmd_handler(int argc, char **argv, void *context)
{
    if (argc != 3)
    {
        printf("Usage: relay <instance_name> <on|off|toggle>\n");
        return ESP_ERR_INVALID_ARG;
    }
    const char *instance_name = argv[1];
    const char *action = argv[2];

    module_t *target_module = NULL;
    for (int i = 0; i < relay_instance_count; i++)
    {
        if (strcmp(relay_instances[i]->name, instance_name) == 0)
        {
            target_module = relay_instances[i];
            break;
        }
    }

    if (!target_module)
    {
        printf("Error: Relay with name '%s' not found.\n", instance_name);
        return ESP_ERR_NOT_FOUND;
    }

    relay_private_data_t *private_data = (relay_private_data_t *)target_module->private_data;

    if (strcmp(action, "on") == 0)
    {
        return set_relay_state(target_module, true);
    }
    else if (strcmp(action, "off") == 0)
    {
        return set_relay_state(target_module, false);
    }
    else if (strcmp(action, "toggle") == 0)
    {
        return set_relay_state(target_module, !private_data->current_state);
    }
    else
    {
        printf("Error: Unknown action '%s'. Use 'on', 'off', or 'toggle'.\n", action);
        return ESP_ERR_INVALID_ARG;
    }
    return ESP_OK;
}

// =========================================================================
//                      Internal Helper Functions
// =========================================================================

/**
 * @internal
 * @brief The core logic to change the relay's physical state and notify the system.
 * @param[in] self Pointer to the module instance to act upon.
 * @param[in] is_on The desired new state (true for ON, false for OFF).
 * @return ESP_OK on success.
 */
static esp_err_t set_relay_state(module_t *self, bool is_on)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    relay_private_data_t *private_data = (relay_private_data_t *)self->private_data;

    gpio_set_level(private_data->gpio_pin, is_on ? private_data->active_level : !private_data->active_level);

    if (private_data->current_state != is_on)
    {
        private_data->current_state = is_on;
        ESP_LOGI(TAG, "Relay '%s' state changed to: %s", self->name, is_on ? "ON" : "OFF");
        publish_state_change(self);
        if (private_data->initial_state == INITIAL_STATE_LAST)
        {
            save_state_to_storage(self);
        }
    }
    return ESP_OK;
}

/**
 * @internal
 * @brief Publishes a FMW_EVENT_RELAY_STATE_CHANGED event to the Event Bus.
 * @param[in] self Pointer to the module instance whose state has changed.
 */
static void publish_state_change(module_t *self)
{
    relay_private_data_t *private_data = (relay_private_data_t *)self->private_data;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "state", private_data->current_state ? "on" : "off");
    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (json_string)
    {
        fmw_telemetry_payload_t *payload = malloc(sizeof(fmw_telemetry_payload_t));
        if (payload)
        {
            strncpy(payload->module_name, self->name, sizeof(payload->module_name) - 1);
            payload->json_data = json_string;
            event_data_wrapper_t *wrapper;
            if (fmw_event_data_wrap(payload, fmw_telemetry_payload_free, &wrapper) == ESP_OK)
            {
                fmw_event_bus_post(FMW_EVENT_RELAY_STATE_CHANGED, wrapper);
                fmw_event_data_release(wrapper);
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

/**
 * @internal
 * @brief Saves the current state of the relay to NVS.
 * @param[in] self Pointer to the module instance.
 * @return ESP_OK on success, or an error from the storage service.
 */
static esp_err_t save_state_to_storage(module_t *self)
{
    relay_private_data_t *private_data = (relay_private_data_t *)self->private_data;
    service_handle_t storage = fmw_service_lookup_by_type(FMW_SERVICE_TYPE_NVRAM_API);
    if (storage)
    {
        return ((storage_api_t *)storage)->set_bool(self->name, "last_state", private_data->current_state);
    }
    return ESP_ERR_NOT_FOUND;
}

/**
 * @internal
 * @brief Parses the module's configuration from the provided cJSON object.
 * @param[in] config The root cJSON object for this module.
 * @param[out] private_data The private data structure to be filled.
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG if parsing fails.
 */
static esp_err_t parse_relay_config(const cJSON *config, relay_private_data_t *private_data)
{
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (!cJSON_IsObject(config_node))
        return ESP_ERR_INVALID_ARG;

    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    if (!cJSON_IsString(name_node))
        return ESP_ERR_INVALID_ARG;
    strncpy(private_data->instance_name, name_node->valuestring, sizeof(private_data->instance_name) - 1);

    const cJSON *pin_node = cJSON_GetObjectItem(config_node, "gpio_pin");
    if (!cJSON_IsNumber(pin_node))
        return ESP_ERR_INVALID_ARG;
    private_data->gpio_pin = pin_node->valueint;

    const cJSON *level_node = cJSON_GetObjectItem(config_node, "active_level");
    private_data->active_level = cJSON_IsNumber(level_node) ? level_node->valueint : 1;

    const cJSON *state_node = cJSON_GetObjectItem(config_node, "initial_state");
    if (cJSON_IsString(state_node))
    {
        if (strcmp(state_node->valuestring, "on") == 0)
            private_data->initial_state = INITIAL_STATE_ON;
        else if (strcmp(state_node->valuestring, "last") == 0)
            private_data->initial_state = INITIAL_STATE_LAST;
        else
            private_data->initial_state = INITIAL_STATE_OFF;
    }
    else
    {
        private_data->initial_state = INITIAL_STATE_OFF;
    }
    return ESP_OK;
}

// =========================================================================
//                      Service API Implementation
// =========================================================================

static esp_err_t api_set_state(void *context, bool is_on)
{
    module_t *self = (module_t *)context;
    if (!self)
        return ESP_ERR_INVALID_ARG;
    return set_relay_state(self, is_on);
}

static esp_err_t api_toggle(void *context)
{
    module_t *self = (module_t *)context;
    if (!self)
        return ESP_ERR_INVALID_ARG;
    relay_private_data_t *private_data = (relay_private_data_t *)self->private_data;
    return set_relay_state(self, !private_data->current_state);
}

static bool api_get_state(void *context)
{
    module_t *self = (module_t *)context;
    if (!self)
        return false;
    relay_private_data_t *private_data = (relay_private_data_t *)self->private_data;
    return private_data->current_state;
}