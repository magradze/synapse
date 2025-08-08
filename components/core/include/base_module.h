/**
 * @file base_module.h
 * @brief Defines the base module interface and status types.
 * @author Giorgi Magradze
 * @version 2.1.0
 * @date 2025-08-26
 *
 * @details
 * This file defines the core contracts for all modules within the Synapse Framework.
 * It includes the module status enumeration, function pointer types for base
 * operations, and the primary `module_t` structure. This ensures a consistent
 * contract for module registration, initialization, lifecycle management,
 * event handling, and status monitoring across the entire system.
 */
#ifndef BASE_MODULE_H
#define BASE_MODULE_H

#include <sdkconfig.h>
#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"
#include "cJSON.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// Forward declaration for event_data_wrapper_t to break circular dependencies
struct event_data_wrapper_t;

/**
 * @enum module_status_t
 * @brief Enumeration of possible module statuses.
 * @details Defines all possible states within a module's lifecycle, used for
 *          monitoring and managing the module's operation.
 */
typedef enum
{
    MODULE_STATUS_UNINITIALIZED = 0, /**< @brief The module has been created but not initialized. */
    MODULE_STATUS_INITIALIZED,       /**< @brief The module is initialized but not running (e.g., its task is suspended). */
    MODULE_STATUS_RUNNING,           /**< @brief The module is running and fully operational. */
    MODULE_STATUS_DISABLED,          /**< @brief The module is disabled and not performing operations. */
    MODULE_STATUS_ERROR,             /**< @brief The module has encountered a critical error. */
    MODULE_STATUS_UNKNOWN            /**< @brief The module's status is unknown or cannot be determined. */
} module_status_t;

// --- Core Module Function Pointer Types ---

/**
 * @brief Forward declaration of the module_t struct.
 */
typedef struct module_t module_t;

/**
 * @brief Function pointer type for module initialization.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK on successful initialization, or an error code otherwise.
 */
typedef esp_err_t (*module_init_fn)(module_t *self);

/**
 * @brief Function pointer type for starting a module's operation.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK if the module started successfully, or an error code otherwise.
 */
typedef esp_err_t (*module_start_fn)(module_t *self);

/**
 * @brief Function pointer type for enabling a module at runtime.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK if the module was enabled successfully.
 */
typedef esp_err_t (*module_enable_fn)(module_t *self);

/**
 * @brief Function pointer type for disabling a module at runtime.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK if the module was disabled successfully.
 */
typedef esp_err_t (*module_disable_fn)(module_t *self);

/**
 * @brief Function pointer type for module deinitialization (cleanup).
 * @param[in] self A pointer to the module instance to be deinitialized.
 */
typedef void (*module_deinit_fn)(module_t *self);

/**
 * @brief Function pointer type for reconfiguring a module at runtime.
 * @param[in] self A pointer to the module instance.
 * @param[in] new_config A cJSON object containing the new configuration.
 * @return ESP_OK if the reconfiguration was successful.
 * @retval ESP_ERR_NOT_SUPPORTED if the module does not support runtime reconfiguration.
 */
typedef esp_err_t (*module_reconfigure_fn)(module_t *self, const cJSON *new_config);

/**
 * @brief Function pointer type for getting a module's current status.
 * @param[in] self A pointer to the module instance.
 * @return The current status of the module as a `module_status_t` enum.
 */
typedef module_status_t (*module_get_status_fn)(module_t *self);

/**
 * @brief Function pointer type for a module's event handler.
 * @details This function does not return a value as it is called asynchronously
 *          from the Event Bus task.
 * @param[in] self A pointer to the module instance.
 * @param[in] event_name The name of the event that was triggered.
 * @param[in] data A pointer to the event's data wrapper (`event_data_wrapper_t*`).
 *                 IMPORTANT: After using the data, `synapse_event_data_release(data)`
 *                 must be called to prevent memory leaks.
 */
typedef void (*module_event_handler_fn)(module_t *self, const char *event_name, void *data);

/**
 * @struct module_dependency_t
 * @brief Describes a single service dependency for a module.
 * @details This structure is used to create a map that tells the System Manager
 *          how to inject dependencies. It links a key from the config.json
 *          to a specific field within the module's private_data struct.
 */
typedef struct
{
    const char *config_key; /**< @brief The key in config.json ending with "_service" (e.g., "expander_service"). */
    size_t offset;          /**< @brief The offset of the handle pointer within the private_data struct, calculated using offsetof(). */
} module_dependency_t;

/**
 * @struct module_t
 * @brief The primary structure defining a framework module.
 * @details This structure encapsulates all essential information and functionality
 *          for a module, including its name, status, configuration, base API
 *          functions, and a pointer to its private data. All modules must be
 *          based on this structure.
 */
struct module_t
{
    char name[CONFIG_SYNAPSE_MODULE_NAME_MAX_LENGTH]; /**< @brief The unique instance name of the module. */
    uint8_t init_level;                           /**< @brief The initialization priority level (lower value = earlier init). */
    module_status_t status;                       /**< @brief The current operational status of the module. */
    cJSON *current_config;                        /**< @brief A pointer to the module's current configuration object. The module owns this memory. */
    SemaphoreHandle_t state_mutex;                /**< @brief A mutex to protect the module's internal state from concurrent access. */

    /**
     * @brief A collection of base API function pointers for the module's lifecycle.
     * @details All modules must implement these functions to ensure standard
     *          framework lifecycle management.
     */
    struct
    {
        module_init_fn init;
        module_start_fn start;
        module_deinit_fn deinit;
        module_enable_fn enable;
        module_disable_fn disable;
        module_reconfigure_fn reconfigure;
        module_get_status_fn get_status;
        module_event_handler_fn handle_event;
    } base;

    void *private_data;                        /**< @brief A pointer to the module's internal, private data structure. */
    const module_dependency_t *dependency_map; /**< @brief (Optional) A NULL-terminated array describing the module's service dependencies for injection. */
};

// --- Helper Macros ---

/**
 * @brief Checks if a module supports runtime operations (enable/disable).
 * @param module A pointer to the module instance.
 * @return true if both enable and disable functions are defined, false otherwise.
 */
#define MODULE_SUPPORTS_RUNTIME(module) ((module) != NULL && (module)->base.enable && (module)->base.disable)

/**
 * @brief Checks if a module is currently in the running state.
 * @param module A pointer to the module instance.
 * @return true if the module's status is MODULE_STATUS_RUNNING, false otherwise.
 */
#define MODULE_IS_RUNNING(module) ((module) != NULL && (module)->status == MODULE_STATUS_RUNNING)

/**
 * @brief Checks if a module is in an operational state (i.e., not in an error state).
 * @param module A pointer to the module instance.
 * @return true if the module's status is not MODULE_STATUS_ERROR, false otherwise.
 */
#define MODULE_IS_OPERATIONAL(module) ((module) != NULL && (module)->status != MODULE_STATUS_ERROR)

#endif // BASE_MODULE_H