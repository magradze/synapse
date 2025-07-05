/**
 * @file storage_backend_nvs.h
 * @brief Header for the NVS storage backend implementation.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-05
 * @details This header declares the initialization function for the NVS
 *          backend. It is intended for internal use by the Storage Manager
 *          module only.
 */

#ifndef STORAGE_BACKEND_NVS_H
#define STORAGE_BACKEND_NVS_H

#include "storage_interface.h" // We need the definition of storage_api_t
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the NVS backend and populates the API function table.
 *
 * @details This function performs the necessary setup for using NVS as the
 *          storage medium and assigns the NVS-specific implementation functions
 *          to the provided API structure.
 *
 * @param[out] api_out A pointer to the storage_api_t structure that will be
 *                     filled with function pointers for NVS operations.
 * @return
 *      - ESP_OK: If the NVS backend was initialized successfully.
 *      - ESP_FAIL: If a critical error occurred during initialization.
 */
esp_err_t storage_backend_nvs_init(storage_api_t* api_out);

#ifdef __cplusplus
}
#endif

#endif // STORAGE_BACKEND_NVS_H