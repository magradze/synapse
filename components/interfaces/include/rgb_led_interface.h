/**
 * @file rgb_led_interface.h
 * @brief Public Service API contract for the RGB LED Indicator module.
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-07-01
 * @details This header defines the abstract interface (API structure) for the
 *          RGB LED Indicator service. Any module wishing to control an RGB LED
 *          via the Service Locator should include this file. This ensures that
 *          the consumer module is decoupled from the specific implementation
 *          of the RGB LED driver.
 */

#ifndef SYNAPSE_RGB_LED_INTERFACE_H
#define SYNAPSE_RGB_LED_INTERFACE_H

#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief RGB LED Indicator Service API structure.
 * @details This is the "contract" that the rgb_led_indicator module implements
 *          and other modules (like ota_manager) consume.
 */
typedef struct
{
    /**
     * @brief Sets a static color on the LED.
     * @details This action overrides any active effect (blink, pulse) and takes
     *          manual control over the LED.
     * @param r Red component (0-255).
     * @param g Green component (0-255).
     * @param b Blue component (0-255).
     * @return ESP_OK on success.
     */
    esp_err_t (*set_color)(uint8_t r, uint8_t g, uint8_t b);

    /**
     * @brief Turns the LED off.
     * @details Equivalent to set_color(0, 0, 0).
     * @return ESP_OK on success.
     */
    esp_err_t (*turn_off)(void);

    /**
     * @brief Starts a blinking effect with a specified color and interval.
     * @param r Red component (0-255).
     * @param g Green component (0-255).
     * @param b Blue component (0-255).
     * @param interval_ms The interval for one on/off cycle in milliseconds.
     * @return ESP_OK on success.
     */
    esp_err_t (*start_blink)(uint8_t r, uint8_t g, uint8_t b, uint32_t interval_ms);

    /**
     * @brief Starts a pulsing (breathing) effect with a specified color and period.
     * @param r Red component (0-255).
     * @param g Green component (0-255).
     * @param b Blue component (0-255).
     * @param period_ms The period for a full brightness cycle in milliseconds.
     * @return ESP_OK on success.
     */
    esp_err_t (*start_pulse)(uint8_t r, uint8_t g, uint8_t b, uint32_t period_ms);

    /**
     * @brief Releases manual control of the LED.
     * @details Returns the LED to its automatic, event-driven state, where it
     *          reflects the overall system status (e.g., WiFi connection).
     * @return ESP_OK on success.
     */
    esp_err_t (*release_control)(void);

} rgb_led_api_t;


#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_RGB_LED_INTERFACE_H