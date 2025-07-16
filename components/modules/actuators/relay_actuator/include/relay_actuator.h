/**
 * @file relay_module.h
 * @brief Public header for the Relay Actuator module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-19
 * @details This file only contains the create function, as the module
 *          is controlled via its registered Service API and the Command Router.
 */

#ifndef RELAY_MODULE_H
#define RELAY_MODULE_H

#include "base_module.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the Relay Actuator module.
 * 
 * @param[in] config Module configuration from system_config.json.
 * @return A pointer to the created module, or NULL on failure.
 */
module_t *relay_actuator_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // RELAY_MODULE_H