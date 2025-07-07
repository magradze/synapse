/**
 * @file mqtt_manager.h
 * @brief Public header for the MQTT Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 */

#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the MQTT Manager module.
 * 
 * @param[in] config Module configuration from system_config.json.
 * @return A pointer to the created module, or NULL on failure.
 */
module_t *mqtt_manager_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // MQTT_MANAGER_H