/**
 * @file time_sync_interface.h
 * @brief Public Service API contract for the Time Sync module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-08-02
 */
#ifndef SYNAPSE_TIME_SYNC_INTERFACE_H
#define SYNAPSE_TIME_SYNC_INTERFACE_H

#include "esp_err.h"
#include <time.h> // For time_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The public Service API structure for the Time Sync module.
 */
typedef struct {
    /**
     * @brief Gets the current synchronized time.
     * @param[out] current_time A pointer to a time_t variable where the
     *                          current time (seconds since epoch) will be stored.
     * @return
     *      - ESP_OK: If the time was successfully retrieved.
     *      - ESP_ERR_INVALID_STATE: If the time has not been synchronized yet.
     */
    esp_err_t (*get_time)(time_t *current_time);

} time_sync_api_t;

#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_TIME_SYNC_INTERFACE_H