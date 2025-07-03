/**
 * @file ota_interface.h
 * @brief Public Service API contract for the OTA Update Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 */
#ifndef FMW_OTA_INTERFACE_H
#define FMW_OTA_INTERFACE_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief OTA Update Manager Service API structure.
 */
typedef struct {
    /**
     * @brief Starts the OTA update process from the given URL.
     * @details This function is non-blocking. It initiates the update process
     *          in a background task and returns immediately. The status of the
     *          update can be monitored via system events.
     * @param[in] firmware_url The HTTPS URL of the firmware binary (.bin file).
     * @return 
     *      - ESP_OK: If the update process was successfully initiated.
     *      - ESP_ERR_INVALID_ARG: If the URL is null or invalid.
     *      - ESP_ERR_INVALID_STATE: If an OTA update is already in progress.
     *      - ESP_FAIL: If the OTA task could not be created.
     */
    esp_err_t (*start_update)(const char *firmware_url);

} ota_api_t;

#ifdef __cplusplus
}
#endif

#endif // FMW_OTA_INTERFACE_H