/**
 * @file storage_backend_sd.h
 * @brief Header for the SD Card storage backend implementation.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-05
 * @details This header declares the initialization and deinitialization
 *          functions for the SD Card backend. It is intended for internal
 *          use by the Storage Manager module only.
 */

#ifndef STORAGE_BACKEND_SD_H
#define STORAGE_BACKEND_SD_H

#include "storage_interface.h" // We need the definition of storage_api_t
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the SD Card backend and populates the API function table.
 *
 * @details This function configures the SPI bus, mounts the SD card using
 *          the FAT filesystem, and assigns the appropriate implementation
 *          functions to the provided API structure.
 *
 * @param[out] api_out A pointer to the storage_api_t structure that will be
 *                     filled with function pointers for SD Card operations.
 * @return
 *      - ESP_OK: If the SD Card backend was initialized successfully.
 *      - ESP_FAIL: If a critical error occurred during initialization.
 */
esp_err_t storage_backend_sd_init(storage_api_t* api_out);

/**
 * @brief Deinitializes the SD Card backend.
 *
 * @details This function unmounts the SD card and releases the SPI bus,
 *          freeing up all associated resources.
 */
void storage_backend_sd_deinit(void);

#ifdef __cplusplus
}
#endif

#endif // STORAGE_BACKEND_SD_H