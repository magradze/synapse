/**
 * @file alarms_manager.h
 * @brief Public API and definitions for the Alarms Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-08
 * @details This header file defines the public interface for the Alarms Manager
 *          module, primarily the create function required by the Module Factory.
 *          The module itself is a system-level component responsible for centralized
 *          alarm and critical event handling.
 */

#ifndef ALARMS_MANAGER_H
#define ALARMS_MANAGER_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the Alarms Manager module.
 *
 * @details This function is the entry point for creating the Alarms Manager.
 *          It is called by the Module Factory during system initialization based
 *          on the configuration found in `system_config.json`.
 *
 * @param[in] config A cJSON object containing the module's specific configuration.
 *                   This includes the list of alarm rules to be monitored.
 *
 * @return A pointer to the newly created module_t structure on success.
 * @return NULL if memory allocation fails or the initial configuration is invalid.
 */
module_t *alarms_manager_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // ALARMS_MANAGER_H