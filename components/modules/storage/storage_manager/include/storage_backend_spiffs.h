/**
 * @file storage_backend_spiffs.h
 * @brief Header for the SPIFFS storage backend implementation.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-05
 * @details This header declares the initialization and deinitialization
 *          functions for the SPIFFS backend. It is intended for internal
 *          use by the Storage Manager module only.
 */

#ifndef STORAGE_BACKEND_SPIFFS_H
#define STORAGE_BACKEND_SPIFFS_H

#include "storage_interface.h" // We need the definition of storage_api_t
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the SPIFFS backend and populates the API function table.
 *
 * @details This function mounts the SPIFFS partition and assigns the
 *          SPIFFS-specific implementation functions to the provided API structure.
 *
 * @param[out] api_out A pointer to the storage_api_t structure that will be
 *                     filled with function pointers for SPIFFS operations.
 * @return
 *      - ESP_OK: If the SPIFFS backend was initialized successfully.
 *      - ESP_FAIL: If a critical error occurred during initialization.
 */
esp_err_t storage_backend_spiffs_init(storage_api_t* api_out);

/**
 * @brief Deinitializes the SPIFFS backend.
 *
 * @details This function unmounts the SPIFFS partition, freeing up resources.
 *          It should be called by the Storage Manager during its deinit phase.
 */
void storage_backend_spiffs_deinit(void);

#ifdef __cplusplus
}
#endif

#endif // STORAGE_BACKEND_SPIFFS_H