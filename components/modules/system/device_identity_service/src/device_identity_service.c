/**
 * @file device_identity_service.c
 * @brief Implementation of the Device Identity Service module with CLI support.
 * @author Synapse Framework Team
 * @version 1.2.3
 * @date 2025-07-09
 * @details
 * This module provides a centralized service to retrieve unique device
 * identifiers (based on MAC address) and firmware version information.
 * It is a fundamental system service designed to initialize early and provide
 * consistent identity data to all other modules.
 *
 * To ensure robust integration with other services that may initialize later,
 * it uses a delayed registration pattern for its CLI command. It subscribes to
 * the `FMW_EVENT_SYSTEM_START_COMPLETE` event and registers its 'device info'
 * command only after the entire system is up and running, guaranteeing that
 * the Command Router service is available.
 */

// --- Synapse Framework Includes ---
#include "device_identity_service.h"
#include "device_identity_interface.h"
#include "cmd_router_interface.h"
#include "base_module.h"
#include "service_locator.h"
#include "service_types.h"
#include "logging.h"
#include "framework_config.h"
#include "framework_events.h"
#include "event_bus.h"
#include "event_data_wrapper.h"

// --- Standard & ESP-IDF Includes ---
#include "esp_log.h"
#include "esp_system.h"
#include "esp_app_desc.h"
#include "esp_mac.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// --- Component Tag ---
DEFINE_COMPONENT_TAG("DEVICE_IDENTITY");

/**
 * @internal
 * @struct device_identity_private_data_t
 * @brief Private data structure for the Device Identity Service module.
 * @details This structure holds the cached identity information, which is read
 *          once during initialization to ensure fast access for subsequent API calls.
 */
typedef struct {
    char device_id[13];         /**< @brief Null-terminated string holding the 12-char hex device ID. */
    const char* firmware_version; /**< @brief Pointer to the firmware version string from app description. */
} device_identity_private_data_t;

// --- Forward Declarations for Internal Functions ---
static esp_err_t device_identity_init(module_t *self);
static esp_err_t device_identity_start(module_t *self);
static void device_identity_deinit(module_t *self);
static void device_identity_handle_event(module_t *self, const char *event_name, void *event_data);

static const char* api_get_device_id(void);
static const char* api_get_firmware_version(void);

static esp_err_t cmd_handler_device(int argc, char **argv, void *context);
static void register_cli_commands(module_t *self);
static void unregister_cli_commands(void);

// --- Global Static Variables ---

/**
 * @internal
 * @brief Global pointer to the single instance of this module.
 * @details Used by the static API functions to access the module's private data.
 */
static module_t *global_identity_service_instance = NULL;

/**
 * @internal
 * @brief Static instance of the service API table.
 * @details This table of function pointers is registered with the Service Locator.
 */
static device_identity_api_t identity_service_api = {
    .get_device_id = api_get_device_id,
    .get_firmware_version = api_get_firmware_version,
};

/**
 * @internal
 * @brief Static instance of the CLI command structure.
 * @details Declared as static to ensure its memory remains valid throughout the
 *          program's lifetime after being registered with the Command Router.
 */
static cmd_t device_info_command;

// =========================================================================
//                      Module Lifecycle Implementation
// =========================================================================

/**
 * @brief Creates a new instance of the Device Identity Service module.
 * @see device_identity_service.h
 */
module_t *device_identity_service_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating Device Identity Service module instance...");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module) {
        ESP_LOGE(TAG, "Failed to allocate memory for module_t.");
        return NULL;
    }

    device_identity_private_data_t *private_data = (device_identity_private_data_t *)calloc(1, sizeof(device_identity_private_data_t));
    if (!private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory for private_data.");
        free(module);
        return NULL;
    }
    module->private_data = private_data;

    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex) {
        ESP_LOGE(TAG, "Failed to create state mutex.");
        free(private_data);
        free(module);
        return NULL;
    }

    char instance_name_buffer[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];
    strncpy(instance_name_buffer, CONFIG_DEVICE_IDENTITY_SERVICE_DEFAULT_INSTANCE_NAME, sizeof(instance_name_buffer) - 1);
    instance_name_buffer[sizeof(instance_name_buffer) - 1] = '\0';

    if (config) {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node)) {
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node) && name_node->valuestring) {
                strncpy(instance_name_buffer, name_node->valuestring, sizeof(instance_name_buffer) - 1);
            }
        }
        module->current_config = cJSON_Duplicate(config, true);
    }

    snprintf(module->name, sizeof(module->name), "%s", instance_name_buffer);
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->init_level = 60;

    module->base.init = device_identity_init;
    module->base.start = device_identity_start;
    module->base.deinit = device_identity_deinit;
    module->base.enable = NULL;
    module->base.disable = NULL;
    module->base.reconfigure = NULL;
    module->base.get_status = NULL;
    module->base.handle_event = device_identity_handle_event;

    global_identity_service_instance = module;

    ESP_LOGI(TAG, "Device Identity Service module '%s' created.", module->name);
    return module;
}

/**
 * @internal
 * @brief Initializes the Device Identity Service module.
 * @param[in] self Pointer to the module instance.
 * @return ESP_OK on success, or an error code on failure.
 */
static esp_err_t device_identity_init(module_t *self)
{
    if (!self || !self->private_data) return ESP_ERR_INVALID_ARG;
    device_identity_private_data_t *p_data = (device_identity_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing Device Identity Service: %s", self->name);

    uint8_t mac[6];
    esp_efuse_mac_get_default(mac);
    snprintf(p_data->device_id, sizeof(p_data->device_id),
             "%02X%02X%02X%02X%02X%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    const esp_app_desc_t *app_desc = esp_app_get_description();
    p_data->firmware_version = app_desc->version;

    ESP_LOGI(TAG, "Device ID: %s", p_data->device_id);
    ESP_LOGI(TAG, "Firmware Version: %s", p_data->firmware_version);

    esp_err_t err = fmw_service_register(self->name, FMW_SERVICE_TYPE_DEVICE_IDENTITY_API, &identity_service_api);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register Device Identity service: %s", esp_err_to_name(err));
        return err;
    }

    err = fmw_event_bus_subscribe(FMW_EVENT_SYSTEM_START_COMPLETE, self);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to subscribe to system start event: %s", esp_err_to_name(err));
        fmw_service_unregister(self->name);
        return err;
    }

    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "Device Identity Service initialized and service registered.");
    return ESP_OK;
}

static esp_err_t device_identity_start(module_t *self) {
    ESP_LOGE(TAG, "🔥 START CALLED for Device Identity!");
    return ESP_OK;
}


/**
 * @internal
 * @brief Deinitializes the module and frees all allocated resources.
 * @param[in] self Pointer to the module instance to be deinitialized.
 */
static void device_identity_deinit(module_t *self)
{
    if (!self) return;
    ESP_LOGI(TAG, "Deinitializing Device Identity Service: %s", self->name);

    unregister_cli_commands();
    fmw_event_bus_unsubscribe(FMW_EVENT_SYSTEM_START_COMPLETE, self);
    fmw_service_unregister(self->name);

    if (self->state_mutex) vSemaphoreDelete(self->state_mutex);
    if (self->private_data) free(self->private_data);
    if (self->current_config) cJSON_Delete(self->current_config);
    free(self);

    global_identity_service_instance = NULL;
}

// =========================================================================
//                      Event Handling
// =========================================================================

/**
 * @internal
 * @brief Handles events received from the Event Bus.
 * @param[in] self Pointer to the module instance.
 * @param[in] event_name The name of the received event.
 * @param[in] event_data Pointer to the event data wrapper.
 */
static void device_identity_handle_event(module_t *self, const char *event_name, void *event_data)
{

    ESP_LOGI(TAG, "****************************************************");
    if (strcmp(event_name, FMW_EVENT_SYSTEM_START_COMPLETE) == 0) {
        ESP_LOGI(TAG, "System start complete. Registering CLI commands now.");
        register_cli_commands(self);
    }

    if (event_data) {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// =========================================================================
//                      Service API Implementation
// =========================================================================

/**
 * @brief Implements the get_device_id API function.
 * @see device_identity_interface.h
 */
static const char* api_get_device_id(void)
{
    if (!global_identity_service_instance || !global_identity_service_instance->private_data) {
        return "unknown_id";
    }
    device_identity_private_data_t *p_data = (device_identity_private_data_t *)global_identity_service_instance->private_data;
    return p_data->device_id;
}

/**
 * @brief Implements the get_firmware_version API function.
 * @see device_identity_interface.h
 */
static const char* api_get_firmware_version(void)
{
    if (!global_identity_service_instance || !global_identity_service_instance->private_data) {
        return "0.0.0";
    }
    device_identity_private_data_t *p_data = (device_identity_private_data_t *)global_identity_service_instance->private_data;
    return p_data->firmware_version;
}

// =========================================================================
//                      CLI Command Implementation
// =========================================================================

/**
 * @internal
 * @brief Handler function for the 'device' command.
 * @details
 * This function is executed when the 'device info' command is received from
 * any source (CLI, MQTT, etc.). It performs two main actions:
 * 1. Prints the device identity information directly to the console for
 *    immediate feedback to the user.
 * 2. Creates a cJSON object with the same information, and publishes it
 *    to the Event Bus under the `FMW_EVENT_DEVICE_INFO_READY` event. This
 *    allows other modules, like the mqtt_manager, to react to the command
 *    and forward the response to external systems.
 *
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 * @param[in] context User-defined context (not used in this handler).
 * @return ESP_OK on success, or an error code on failure.
 */
static esp_err_t cmd_handler_device(int argc, char **argv, void *context)
{
    if (argc != 2) {
        printf("Error: Invalid arguments.\nUsage: device info\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (strcmp(argv[1], "info") == 0) {

        // --- Action 1: Print information to the console (for direct CLI users) ---
        printf("----------------------------------\n");
        printf("  Device Information\n");
        printf("----------------------------------\n");
        printf("  %-20s: %s\n", "Device ID", api_get_device_id());
        printf("  %-20s: %s\n", "Firmware Version", api_get_firmware_version());
        printf("----------------------------------\n");

        // --- Action 2: Create JSON and publish an event (for remote/async responses) ---
        cJSON *info_json = cJSON_CreateObject();
        if (info_json == NULL)
        {
            ESP_LOGE(TAG, "Failed to create cJSON object for device info.");
            return ESP_ERR_NO_MEM;
        }

        // Populate the JSON object
        cJSON_AddStringToObject(info_json, "device_id", api_get_device_id());
        cJSON_AddStringToObject(info_json, "firmware_version", api_get_firmware_version());

        // Convert the JSON object to a string
        char *json_string = cJSON_PrintUnformatted(info_json);
        if (json_string)
        {
            ESP_LOGI(TAG, "Publishing DEVICE_INFO_READY event.");

            event_data_wrapper_t *wrapper;
            // Wrap the dynamically allocated string so it can be safely freed by the event bus
            if (fmw_event_data_wrap(strdup(json_string), free, &wrapper) == ESP_OK)
            {
                fmw_event_bus_post(FMW_EVENT_DEVICE_INFO_READY, wrapper);
                fmw_event_data_release(wrapper); // Release our ownership
            }

            free(json_string);
        }

        // Clean up the cJSON object
        cJSON_Delete(info_json);
    } else {
        printf("Error: Unknown subcommand '%s'.\nUsage: device info\n", argv[1]);
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

/**
 * @internal
 * @brief Registers the 'device' command with the Command Router service.
 * @param[in] self Pointer to the module instance, used as context for the handler.
 */
static void register_cli_commands(module_t *self)
{
    service_handle_t cmd_router_handle = fmw_service_get("main_cmd_router");
    if (!cmd_router_handle) {
        ESP_LOGW(TAG, "Command Router service not found. Cannot register 'device' command.");
        return;
    }

    cmd_router_api_t *cmd_api = (cmd_router_api_t *)cmd_router_handle;

    device_info_command = (cmd_t){
        .command = "device",
        .help = "Get device hardware and firmware information",
        .usage = "device info",
        .min_args = 2,
        .max_args = 2,
        .handler = cmd_handler_device,
        .context = self
    };

    esp_err_t err = cmd_api->register_command(&device_info_command);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register 'device' command: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "'device' command registered successfully.");
    }
}

/**
 * @internal
 * @brief Unregisters the 'device' command from the Command Router.
 * @details This is called during deinitialization to ensure no dangling command
 *          registrations are left in the system.
 */
static void unregister_cli_commands(void)
{
    service_handle_t cmd_router_handle = fmw_service_get("main_cmd_router");
    if (cmd_router_handle) {
        cmd_router_api_t *cmd_api = (cmd_router_api_t *)cmd_router_handle;
        cmd_api->unregister_command("device");
        ESP_LOGI(TAG, "'device' command unregistered.");
    }
}