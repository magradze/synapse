/**
 * @file time_of_flight_sensor.h
 * @brief Public header for the Time-of-Flight (TOF) sensor module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-31
 */
#ifndef TIME_OF_FLIGHT_SENSOR_H
#define TIME_OF_FLIGHT_SENSOR_H

#include "base_module.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the TOF sensor module.
 * @param[in] config A cJSON object containing the module's configuration.
 * @return A pointer to the created module (`module_t*`), or NULL on failure.
 */
module_t *time_of_flight_sensor_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // TIME_OF_FLIGHT_SENSOR_H