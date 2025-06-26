/**
 * @file logger_module.c
 * @brief Logger Module Implementation - Centralized logging and event monitoring
 * @details This module implements comprehensive event monitoring by capturing
 *          all events from the Event Bus and providing debugging output.
 *          It supports runtime control through framework command events.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-06-26
 */

// Standard library includes
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>

// ESP-IDF includes
#include "esp_log.h"

// Framework core includes
#include "base_module.h"
#include "event_bus.h"
#include "logging.h"
// #include "config_manager.h" // May not be available in all configurations

// Framework interface includes
#include "telemetry_events.h"
#include "event_data_wrapper.h"
#include "system_event_ids.h"

// External library includes
#include "cJSON.h"

/** @brief Component tag for ESP-IDF logging system */
DEFINE_COMPONENT_TAG("LOGGER_MODULE");

// =============================================================================
// Module Private Constants and Variables
// =============================================================================

/** @brief Default instance name for logger module */
#define LOGGER_DEFAULT_INSTANCE_NAME "main_logger"

/** @brief Maximum length for instance name string */
#define LOGGER_INSTANCE_NAME_MAX_LEN 32

/**
 * @brief Logger module enable/disable status
 * @details Controls whether the logger outputs events to console
 */
static bool logger_module_enabled = true;

/**
 * @brief Logger module instance name
 * @details Stores the module instance name from configuration
 */
static char logger_module_instance_name[LOGGER_INSTANCE_NAME_MAX_LEN] = LOGGER_DEFAULT_INSTANCE_NAME;

// =============================================================================
// Module Private Function Prototypes
// =============================================================================

/**
 * @brief Logger module initialization function
 * @param[in] module Pointer to the module instance
 * @return ESP_OK on success, error code otherwise
 */
static esp_err_t logger_module_init(module_t *module);

/**
 * @brief Logger module start function
 * @param[in] module Pointer to the module instance
 * @return ESP_OK on success, error code otherwise
 */
static esp_err_t logger_module_start(module_t *module);

/**
 * @brief Logger module event handler
 * @details Handles all events from the Event Bus and outputs debug information
 * @param[in] module Pointer to the module instance
 * @param[in] event_id Event identifier
 * @param[in] event_data Pointer to event data (can be NULL)
 */
static void logger_module_handle_event(module_t *module, core_framework_event_id_t event_id, void *event_data);

/**
 * @brief Enable the logger module at runtime
 * @param[in] module Pointer to the module instance
 * @return ESP_OK on success, error code otherwise
 */
static esp_err_t logger_module_enable(module_t *module);

/**
 * @brief Disable the logger module at runtime
 * @param[in] module Pointer to the module instance
 * @return ESP_OK on success, error code otherwise
 */
static esp_err_t logger_module_disable(module_t *module);

/**
 * @brief Reconfigure the logger module at runtime
 * @param[in] module Pointer to the module instance
 * @param[in] new_config New configuration as JSON object
 * @return ESP_OK on success, error code otherwise
 */
static esp_err_t logger_module_reconfigure(module_t *module, const cJSON *new_config);

/**
 * @brief Get current status of the logger module
 * @param[in] module Pointer to the module instance
 * @return Current module status
 */
static module_status_t logger_module_get_status(module_t *module);

/**
 * @brief Process framework command messages for the logger module
 * @param[in] module Pointer to the module instance
 * @param[in] command Pointer to the telemetry command data
 */
static void logger_module_process_command(module_t *module, const fmw_telemetry_payload_t *command);

// =============================================================================
// Module Private Function Implementations
// =============================================================================


static esp_err_t logger_module_init(module_t *module)
{
    ESP_LOGI(TAG, "Initializing logger module...");
    
    if (module == NULL) {
        ESP_LOGE(TAG, "Module pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    
    // No specific initialization required at this stage
    // Future enhancements could include buffer allocation, file logging setup, etc.
    
    ESP_LOGI(TAG, "Logger module initialized successfully");
    return ESP_OK;
}

static esp_err_t logger_module_start(module_t *module)
{
    ESP_LOGI(TAG, "Starting logger module and subscribing to events...");
    
    if (module == NULL) {
        ESP_LOGE(TAG, "Module pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    
    // The logger module will automatically receive all system events
    // through the System Manager's universal event subscription mechanism
    // No additional manual subscriptions are needed
    
    ESP_LOGI(TAG, "Logger module started successfully");
    return ESP_OK;
}

static void logger_module_handle_event(module_t *module, core_framework_event_id_t event_id, void *event_data)
{
    // Early return if logger is disabled
    if (!logger_module_enabled) {
        return;
    }
    
    if (module == NULL) {
        ESP_LOGW(TAG, "Module pointer is NULL in event handler");
        return;
    }
    
    // Log basic event information for all events
    ESP_LOGI(TAG, "Event received: ID=%d", (int)event_id);
    
    // Handle specific framework command events
    if (event_id == FRAMEWORK_EVENT_COMMAND_RECEIVED && event_data != NULL) {
        const event_data_wrapper_t *wrapper = (const event_data_wrapper_t *)event_data;
        if (wrapper && wrapper->payload) {
            const fmw_telemetry_payload_t *command = (const fmw_telemetry_payload_t *)wrapper->payload;
            logger_module_process_command(module, command);
        }
    }
    
    // For all other events, just log the basic information
    // This provides comprehensive system monitoring without module dependencies
}

static esp_err_t logger_module_enable(module_t *module)
{
    ESP_LOGI(TAG, "Enabling logger module...");
    
    if (module == NULL) {
        ESP_LOGE(TAG, "Module pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    
    logger_module_enabled = true;
    module->status = MODULE_STATUS_RUNNING;
    
    ESP_LOGI(TAG, "Logger module enabled successfully");
    return ESP_OK;
}

static esp_err_t logger_module_disable(module_t *module)
{
    ESP_LOGI(TAG, "Disabling logger module...");
    
    if (module == NULL) {
        ESP_LOGE(TAG, "Module pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    
    logger_module_enabled = false;
    module->status = MODULE_STATUS_INITIALIZED;
    
    ESP_LOGI(TAG, "Logger module disabled successfully");
    return ESP_OK;
}

static esp_err_t logger_module_reconfigure(module_t *module, const cJSON *new_config)
{
    ESP_LOGI(TAG, "Reconfiguring logger module...");
    
    if (module == NULL || new_config == NULL) {
        ESP_LOGE(TAG, "Invalid parameters for reconfiguration");
        return ESP_ERR_INVALID_ARG;
    }
    
    // Extract instance name from new configuration
    cJSON *config_node = cJSON_GetObjectItem(new_config, "config");
    if (config_node && cJSON_IsObject(config_node)) {
        cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
        if (cJSON_IsString(name_node) && name_node->valuestring) {
            strncpy(logger_module_instance_name, name_node->valuestring, 
                    LOGGER_INSTANCE_NAME_MAX_LEN - 1);
            logger_module_instance_name[LOGGER_INSTANCE_NAME_MAX_LEN - 1] = '\0';
            
            ESP_LOGI(TAG, "Updated instance name to: %s", logger_module_instance_name);
        }
    }
    
    ESP_LOGI(TAG, "Logger module reconfigured successfully");
    return ESP_OK;
}

static module_status_t logger_module_get_status(module_t *module)
{
    if (module == NULL) {
        ESP_LOGW(TAG, "Module pointer is NULL in get_status");
        return MODULE_STATUS_ERROR;
    }
    
    if (!logger_module_enabled) {
        return MODULE_STATUS_INITIALIZED;
    }
    
    return MODULE_STATUS_RUNNING;
}

static void logger_module_process_command(module_t *module, const fmw_telemetry_payload_t *command)
{
    if (module == NULL || command == NULL) {
        ESP_LOGW(TAG, "Invalid parameters for command processing");
        return;
    }
    
    // Check if this command is for this specific module
    if (strcmp(command->module_name, logger_module_instance_name) != 0) {
        // This command is not for us, ignore it
        return;
    }
    
    ESP_LOGI(TAG, "Processing command for module: %s", command->module_name);
    
    // Parse JSON command
    cJSON *root = cJSON_Parse(command->json_data);
    if (root == NULL) {
        ESP_LOGW(TAG, "Failed to parse JSON command: %s", command->json_data);
        return;
    }
    
    cJSON *action = cJSON_GetObjectItem(root, "action");
    if (cJSON_IsString(action) && action->valuestring != NULL) {
        if (strcmp(action->valuestring, "enable") == 0) {
            ESP_LOGI(TAG, "Received enable command");
            if (module->base.enable) {
                module->base.enable(module);
            }
        } else if (strcmp(action->valuestring, "disable") == 0) {
            ESP_LOGI(TAG, "Received disable command");
            if (module->base.disable) {
                module->base.disable(module);
            }
        } else {
            ESP_LOGW(TAG, "Unknown action: %s", action->valuestring);
        }
    } else {
        ESP_LOGW(TAG, "Invalid action field in command");
    }
    
    cJSON_Delete(root);
}

// =============================================================================
// Module Public API Implementation
// =============================================================================

module_t *logger_module_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating logger module instance...");
    
    if (config == NULL) {
        ESP_LOGE(TAG, "Configuration is NULL");
        return NULL;
    }
    
    // Extract instance name from configuration
    const char *instance_name = LOGGER_DEFAULT_INSTANCE_NAME;
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (config_node && cJSON_IsObject(config_node)) {
        const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
        if (cJSON_IsString(name_node) && name_node->valuestring) {
            instance_name = name_node->valuestring;
        }
    }
    
    // Store instance name in NVS using config_manager (optional - may not be available in all configurations)
    // esp_err_t ret = fmw_config_set_string("logger.instance", instance_name);
    // if (ret != ESP_OK) {
    //     ESP_LOGE(TAG, "Failed to save instance name to NVS: %s", 
    //              esp_err_to_name(ret));
    //     // Continue anyway, this is not a critical failure
    // } else {
    //     ESP_LOGI(TAG, "Saved instance name '%s' to NVS", instance_name);
    // }
    
    // Store instance name in static variable
    strncpy(logger_module_instance_name, instance_name, LOGGER_INSTANCE_NAME_MAX_LEN - 1);
    logger_module_instance_name[LOGGER_INSTANCE_NAME_MAX_LEN - 1] = '\0';
    
    // Allocate memory for module structure
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (module == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for logger module");
        return NULL;
    }
    
    // Initialize module structure
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->private_data = NULL;  // No private data needed for this module
    
    // Set base module function pointers
    module->base.init = logger_module_init;
    module->base.start = logger_module_start;
    module->base.handle_event = logger_module_handle_event;
    
    // Set runtime control function pointers
    module->base.enable = logger_module_enable;
    module->base.disable = logger_module_disable;
    module->base.reconfigure = logger_module_reconfigure;
    module->base.get_status = logger_module_get_status;
    
    // Initialize runtime control fields
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->current_config = cJSON_Duplicate(config, true);
    
    if (module->current_config == NULL) {
        ESP_LOGW(TAG, "Failed to duplicate configuration JSON");
        // Continue anyway, this is not critical
    }
    
    ESP_LOGI(TAG, "Logger module instance '%s' created successfully", instance_name);
    return module;
}