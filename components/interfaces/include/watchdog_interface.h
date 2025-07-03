/**
 * @file watchdog_interface.h
 * @brief Public Service API contract for the Watchdog Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-02
 * @details This header defines the abstract interface for the Watchdog Manager,
 *          allowing other modules to register for heartbeat monitoring.
 */

#ifndef FMW_WATCHDOG_INTERFACE_H
#define FMW_WATCHDOG_INTERFACE_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Watchdog Manager Service API structure.
 */
typedef struct
{
    /**
     * @brief Registers a module for heartbeat monitoring.
     * @details After registration, the Watchdog Manager will expect this module
     *          to periodically call send_heartbeat().
     * @param[in] module_name The unique instance name of the module to monitor.
     * @return ESP_OK on success, ESP_ERR_NO_MEM if the list is full.
     */
    esp_err_t (*register_heartbeat)(const char *module_name);

    /**
     * @brief Unregisters a module from heartbeat monitoring.
     * @details Should be called in the module's deinit function.
     * @param[in] module_name The name of the module to unregister.
     * @return ESP_OK on success, ESP_ERR_NOT_FOUND if not found.
     */
    esp_err_t (*unregister_heartbeat)(const char *module_name);

    /**
     * @brief A monitored module calls this function to signal it is alive.
     * @param[in] module_name The name of the module sending the heartbeat.
     * @return ESP_OK on success, ESP_ERR_NOT_FOUND if the module is not registered.
     */
    esp_err_t (*send_heartbeat)(const char *module_name);

} watchdog_api_t;


#ifdef __cplusplus
}
#endif

#endif // FMW_WATCHDOG_INTERFACE_H