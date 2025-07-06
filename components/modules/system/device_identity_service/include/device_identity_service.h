/**
 * @file device_identity_service.h
 * @brief Provides unique device identifiers and firmware information.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-09
 */

#ifndef DEVICE_IDENTITY_SERVICE_H
#define DEVICE_IDENTITY_SERVICE_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the Device Identity Service module.
 * @param[in] config A cJSON object for configuration (currently unused).
 * @return A pointer to the newly created module_t structure, or NULL on failure.
 */
module_t *device_identity_service_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // DEVICE_IDENTITY_SERVICE_H