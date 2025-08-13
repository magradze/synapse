/**
 * @file system_manager_interface.h
 * @brief Public Service API contract for the System Manager.
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-07-03
 * @details
 * This header defines the abstract interface (API structure) for the
 * System Manager service. Any module wishing to interact with core system
 * functionalities (like getting the module list) via the Service Locator
 * should include this file. This ensures that the consumer module is
 * decoupled from the specific implementation of the System Manager.
 */

#ifndef SYNAPSE_SYSTEM_MANAGER_INTERFACE_H
#define SYNAPSE_SYSTEM_MANAGER_INTERFACE_H

#include "base_module.h" // For module_t definition
#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The public Service API structure for the System Manager.
 * @details This is the "contract" that the system_manager module implements
 *          and other modules (like command_router) consume.
 */
typedef struct
{
    /**
     * @brief Retrieves a read-only list of all currently registered modules.
     * @details This function provides a snapshot of the module registry, allowing
     *          other modules to inspect the system's composition and module statuses.
     *          The memory for the returned list is managed by the module registry
     *          and should NOT be freed by the caller.
     *
     * @param[out] modules A pointer that will be updated to point to an array of
     *                     `const module_t*`.
     * @param[out] count A pointer to a variable where the number of modules in the
     *                   array will be stored.
     *
     * @return
     *      - ESP_OK: If the list was retrieved successfully.
     *      - ESP_ERR_INVALID_ARG: If `modules` or `count` pointers are NULL.
     */
    esp_err_t (*get_all_modules)(const module_t ***modules, uint8_t *count);

    /**
     * @brief Initiates a graceful system reboot.
     * @details This function provides a standardized way to restart the device.
     *          It may perform cleanup operations before calling esp_restart().
     *
     * @return This function does not return on success. It will return an error
     *         code if the reboot cannot be initiated.
     */
    esp_err_t (*reboot_system)(void);

    // Add other future system-level API functions here, for example:
    // esp_err_t (*get_system_uptime)(uint64_t *uptime_ms);
    // esp_err_t (*get_heap_status)(size_t *free_heap, size_t *min_free_heap);

} system_manager_api_t;


#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_SYSTEM_MANAGER_INTERFACE_H