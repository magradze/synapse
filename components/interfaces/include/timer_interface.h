/**
 * @file timer_interface.h
 * @brief Public Service API contract for the System Timer Service module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-06
 */

#ifndef SYNAPSE_TIMER_INTERFACE_H
#define SYNAPSE_TIMER_INTERFACE_H

#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque handle to a scheduled timer.
 * @details This handle is returned by schedule_event and used by cancel_event
 *          to identify a specific timer.
 */
typedef void *synapse_timer_handle_t;

/**
 * @brief The public Service API structure for the System Timer Service.
 */
typedef struct
{
    /**
     * @brief Schedules an event to be posted on the Event Bus after a specified interval.
     *
     * @param[in] event_name The name of the event to post. The string must be persistent.
     * @param[in] interval_ms The interval in milliseconds after which to post the event.
     * @param[in] is_periodic If true, the event will be posted repeatedly at the given
     *                        interval. If false, it will be a one-shot timer.
     * @return A handle to the scheduled timer, or NULL on failure (e.g., no more timer slots).
     */
    synapse_timer_handle_t (*schedule_event)(const char *event_name, uint32_t interval_ms, bool is_periodic);

    /**
     * @brief Cancels a previously scheduled event.
     *
     * @param[in] handle The handle of the timer to cancel, obtained from schedule_event.
     * @return
     *      - ESP_OK: If the timer was successfully found and cancelled.
     *      - ESP_ERR_INVALID_ARG: If the handle is NULL or invalid.
     *      - ESP_ERR_NOT_FOUND: If no timer with the given handle was found.
     */
    esp_err_t (*cancel_event)(synapse_timer_handle_t handle);

} timer_api_t;


#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_TIMER_INTERFACE_H