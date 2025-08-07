/**
 * @file wifi_manager.h
 * @brief WiFi connection management.
 * @author Synapse Framework Team
 * @version 2.0.0
 * @date 2025-08-27
 * @details Header for the Wifi_Manager module, which handles all WiFi
 *          connection and reconnection logic. This version is updated to
 *          use a dedicated task for connection management to prevent
 *          stack overflows in system tasks.
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "synapse.h" // Includes base_module.h and cJSON.h

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Creates a new instance of the wifi_manager module.
     *
     * @param[in] config The module's configuration as a cJSON object.
     * @return A pointer to the created module, or NULL on failure.
     */
    module_t *wifi_manager_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // WIFI_MANAGER_H