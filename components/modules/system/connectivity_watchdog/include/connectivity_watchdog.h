/**
 * @file connectivity_watchdog.h
 * @brief Public interface for the Connectivity Watchdog module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-12
 * @details This module monitors the health of network and service connections
 *          (like WiFi and MQTT) and triggers recovery actions upon prolonged failure.
 *          It operates based on the base_module interface of the Synapse Framework.
 */

#ifndef CONNECTIVITY_WATCHDOG_H
#define CONNECTIVITY_WATCHDOG_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the Connectivity Watchdog module.
 *
 * This factory function allocates and initializes the module's data structures.
 * The returned module should be registered with the System Manager.
 *
 * @param[in] config The JSON configuration object for this module instance. Can be NULL for defaults.
 * @return A pointer to the newly created module_t instance, or NULL on failure.
 * @warning The caller is responsible for the deallocation of the module via its `deinit` function.
 */
module_t *connectivity_watchdog_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // CONNECTIVITY_WATCHDOG_H