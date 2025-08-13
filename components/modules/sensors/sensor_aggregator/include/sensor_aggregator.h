/**
 * @file sensor_aggregator.h
 * @brief Public header for the Sensor Aggregator module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-17
 * @details This file only contains the create function, as the module
 *          is purely event-driven and configured via JSON.
 */

#ifndef SENSOR_AGGREGATOR_H
#define SENSOR_AGGREGATOR_H

#include "base_module.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the Sensor Aggregator module.
 * 
 * @param[in] config Module configuration from system_config.json.
 * @return A pointer to the created module, or NULL on failure.
 */
module_t *sensor_aggregator_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // SENSOR_AGGREGATOR_H