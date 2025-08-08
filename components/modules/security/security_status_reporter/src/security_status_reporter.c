/**
 * @file security_status_reporter.c
 * @brief Reports Secure Boot and Flash Encryption status.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-18
 * @details This module provides a centralized service to query the device's
 *          critical security features. It reads the Secure Boot and Flash
 *          Encryption status once during initialization, caches the values,
 *          and exposes them through a service API. It also provides a CLI
 *          command to display the status on the console.
 */

// --- Framework & System Includes ---

#include "synapse.h"
#include "security_status_reporter.h"
#include "security_status_interface.h"

#include "cmd_router_interface.h"

// --- ESP-IDF & Standard Lib Includes ---
#include "esp_log.h"
#include "esp_secure_boot.h"
#include "esp_flash_encrypt.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

DEFINE_COMPONENT_TAG("SEC_STATUS");

// =========================================================================
//                      Private Data & API Implementation
// =========================================================================

/**
 * @internal
 * @struct security_status_private_data_t
 * @brief Private data for the Security Status Reporter module.
 */
typedef struct {
    char instance_name[CONFIG_SECURITY_STATUS_REPORTER_INSTANCE_NAME_MAX_LEN];
    bool secure_boot_enabled;
    bool flash_encryption_enabled;
} security_status_private_data_t;

// --- Forward declarations ---
static esp_err_t security_status_reporter_init(module_t *self);
static void security_status_reporter_deinit(module_t *self);
static void security_status_reporter_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t cmd_handler(int argc, char **argv, void *context);

// --- API Function Implementations ---
static bool api_is_secure_boot_enabled(void);
static bool api_is_flash_encryption_enabled(void);

/**
 * @internal
 * @brief The static instance of the module's service API.
 */
static security_status_api_t security_service_api = {
    .is_secure_boot_enabled = api_is_secure_boot_enabled,
    .is_flash_encryption_enabled = api_is_flash_encryption_enabled,
};

/**
 * @internal
 * @brief The static instance of the module itself (singleton pattern).
 */
static module_t *security_status_self = NULL;

// =========================================================================
//                      Module Lifecycle Implementation
// =========================================================================

module_t *security_status_reporter_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating security_status_reporter module instance");
    
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module) {
        ESP_LOGE(TAG, "Failed to allocate memory for module");
        return NULL;
    }
    
    security_status_private_data_t *private_data = (security_status_private_data_t *)calloc(1, sizeof(security_status_private_data_t));
    if (!private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory for private data");
        free(module);
        return NULL;
    }
    
    module->private_data = private_data;
    
    const char *instance_name = CONFIG_SECURITY_STATUS_REPORTER_DEFAULT_INSTANCE_NAME;
    if (config) {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node)) {
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node) && name_node->valuestring) {
                instance_name = name_node->valuestring;
            }
        }
    }
    
    strncpy(private_data->instance_name, instance_name, sizeof(private_data->instance_name) - 1);
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;
    
    module->init_level = 22;
    module->base.init = security_status_reporter_init;
    module->base.deinit = security_status_reporter_deinit;
    module->base.handle_event = security_status_reporter_handle_event;
    // This is a service module, no start/stop/enable/disable logic needed.
    module->base.start = NULL;
    module->base.enable = NULL;
    module->base.disable = NULL;
    module->base.reconfigure = NULL;
    module->base.get_status = NULL;
    
    security_status_self = module;
    ESP_LOGI(TAG, "Security Status Reporter module created: '%s'", instance_name);
    return module;
}

static esp_err_t security_status_reporter_init(module_t *self)
{
    if (!self) return ESP_ERR_INVALID_ARG;
    
    security_status_private_data_t *private_data = (security_status_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing module: %s", self->name);
    
    // Read and cache security status once at startup
    private_data->secure_boot_enabled = esp_secure_boot_enabled();
    private_data->flash_encryption_enabled = esp_flash_encryption_enabled();

    ESP_LOGI(TAG, "Security Status: Secure Boot is %s", private_data->secure_boot_enabled ? "ENABLED" : "DISABLED");
    ESP_LOGI(TAG, "Security Status: Flash Encryption is %s", private_data->flash_encryption_enabled ? "ENABLED" : "DISABLED");

    // Register the service API
    esp_err_t err = synapse_service_register(self->name, SYNAPSE_SERVICE_TYPE_SECURITY_API, &security_service_api);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register Security Status service: %s", esp_err_to_name(err));
        return err;
    }

    // Subscribe to event to register CLI command later
    synapse_event_bus_subscribe(SYNAPSE_EVENT_SYSTEM_START_COMPLETE, self);

    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "Module initialized and service registered successfully.");
    return ESP_OK;
}

static void security_status_reporter_deinit(module_t *self)
{
    if (!self) return;
    
    ESP_LOGI(TAG, "Deinitializing %s module", self->name);

    synapse_service_unregister(self->name);
    synapse_event_bus_unsubscribe(SYNAPSE_EVENT_SYSTEM_START_COMPLETE, self);

    service_handle_t cmd_router = synapse_service_get("main_cmd_router");
    if (cmd_router) {
        ((cmd_router_api_t *)cmd_router)->unregister_command("security");
    }

    if (self->private_data) {
        free(self->private_data);
    }
    
    security_status_self = NULL;
    free(self);
    
    ESP_LOGI(TAG, "Module deinitialized successfully");
}

static void security_status_reporter_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (strcmp(event_name, SYNAPSE_EVENT_SYSTEM_START_COMPLETE) == 0)
    {
        service_handle_t cmd_router = synapse_service_get("main_cmd_router");
        if (cmd_router) {
            // Initialize with compile-time constants first
            static cmd_t security_cmd = {
                .command = "security",
                .help = "Displays device security status (Secure Boot, Flash Encryption).",
                .usage = "security status",
                .min_args = 2,
                .max_args = 2,
                .handler = cmd_handler,
                // .context is set at runtime
            };
            // Set the runtime context
            security_cmd.context = self;

            ((cmd_router_api_t *)cmd_router)->register_command(&security_cmd);
        }
    }

    if (event_data) {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// =========================================================================
//                      Service & CLI Implementation
// =========================================================================

/**
 * @internal
 * @brief API function to get the Secure Boot status.
 * @return True if Secure Boot is enabled, false otherwise.
 */
static bool api_is_secure_boot_enabled(void) {
    if (!security_status_self) return false;
    security_status_private_data_t *private_data = (security_status_private_data_t *)security_status_self->private_data;
    return private_data->secure_boot_enabled;
}

/**
 * @internal
 * @brief API function to get the Flash Encryption status.
 * @return True if Flash Encryption is enabled, false otherwise.
 */
static bool api_is_flash_encryption_enabled(void) {
    if (!security_status_self) return false;
    security_status_private_data_t *private_data = (security_status_private_data_t *)security_status_self->private_data;
    return private_data->flash_encryption_enabled;
}

/**
 * @brief Handles the "security status" command, prints device security status, and publishes a JSON report.
 *
 * This command handler checks if the command arguments are valid for querying the device's security status.
 * It prints a human-readable report to the console, indicating whether Secure Boot and Flash Encryption are enabled.
 * If either security feature is disabled, a warning is displayed.
 *
 * Additionally, the function creates a JSON report containing the security status and publishes it as an event
 * using the telemetry and event bus system.
 *
 * @param[in] argc     Number of command-line arguments.
 * @param[in] argv     Array of command-line argument strings.
 * @param[in] context  Pointer to the module context (expected to be of type module_t*).
 *
 * @return
 *      - ESP_OK on success.
 *      - ESP_ERR_INVALID_ARG if the command arguments are invalid.
 */
static esp_err_t cmd_handler(int argc, char **argv, void *context) {
    if (argc != 2 || strcmp(argv[1], "status") != 0) {
        printf("Usage: security status\n");
        return ESP_ERR_INVALID_ARG;
    }

    module_t *self = (module_t *)context;
    bool sb_enabled = api_is_secure_boot_enabled();
    bool fe_enabled = api_is_flash_encryption_enabled();

    // Print human-readable report to console
    printf("----------------------------------\n");
    printf("  Device Security Status\n");
    printf("----------------------------------\n");
    printf("  Secure Boot       : %s\n", sb_enabled ? "ENABLED" : "DISABLED");
    printf("  Flash Encryption  : %s\n", fe_enabled ? "ENABLED" : "DISABLED");
    printf("----------------------------------\n");

    if (!sb_enabled || !fe_enabled) {
        printf("WARNING: Device is not in a fully secure state.\n");
    }

    // --- Publish JSON report as an event ---
    cJSON *report_json = cJSON_CreateObject();
    if (report_json) {
        cJSON_AddBoolToObject(report_json, "secure_boot_enabled", sb_enabled);
        cJSON_AddBoolToObject(report_json, "flash_encryption_enabled", fe_enabled);
        
        char *json_string = cJSON_PrintUnformatted(report_json);
        if (json_string) {
            synapse_telemetry_payload_t *payload = malloc(sizeof(synapse_telemetry_payload_t));
            if (payload) {
                strncpy(payload->module_name, self->name, sizeof(payload->module_name) - 1);
                payload->json_data = json_string;

                event_data_wrapper_t *wrapper;
                if (synapse_event_data_wrap(payload, synapse_telemetry_payload_free, &wrapper) == ESP_OK)
                {
                    synapse_event_bus_post(SYNAPSE_EVENT_SECURITY_STATUS_READY, wrapper);
                    synapse_event_data_release(wrapper);
                }
                else
                {
                    free(json_string);
                    free(payload);
                }
            } else {
                free(json_string);
            }
        }
        cJSON_Delete(report_json);
    }

    return ESP_OK;
}