/**
 * @file watchdog_manager.h
 * @brief Application-aware watchdog with heartbeat monitoring.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-07-19
 */

#ifndef WATCHDOG_MANAGER_H
#define WATCHDOG_MANAGER_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Creates a new instance of the Watchdog Manager module.
     *
     * @param[in] config Module configuration from system_config.json.
     * @return A pointer to the created module, or NULL on failure.
     */
    module_t *watchdog_manager_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // WATCHDOG_MANAGER_H