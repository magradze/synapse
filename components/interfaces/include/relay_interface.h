/**
 * @file relay_interface.h
 * @brief Public Service API contract for the Relay Actuator module.
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-07-19
 * @details This header defines the abstract interface (API structure) for a
 *          generic relay service. Any module wishing to control a relay
 *          should include this file and use the Service Locator to get a
 *          handle to this API.
 */

#ifndef SYNAPSE_RELAY_INTERFACE_H
#define SYNAPSE_RELAY_INTERFACE_H

#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief The public Service API structure for a Relay module.
     */
    typedef struct
    {
        /**
         * @brief Sets the state of the relay.
         * @param[in] context The context handle for the specific relay instance.
         * @param[in] is_on `true` to turn the relay on, `false` to turn it off.
         * @return ESP_OK on success, or an error code on failure.
         */
        esp_err_t (*set_state)(void *context, bool is_on);

        /**
         * @brief Toggles the current state of the relay.
         * @param[in] context The context handle for the specific relay instance.
         * @return ESP_OK on success.
         */
        esp_err_t (*toggle)(void *context);

        /**
         * @brief Gets the current state of the relay.
         * @param[in] context The context handle for the specific relay instance.
         * @return `true` if the relay is on, `false` if it is off.
         */
        bool (*get_state)(void *context);

    } relay_api_t;

#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_RELAY_INTERFACE_H