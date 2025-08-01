/**
 * @file sn74hc595n_writer_interface.h
 * @brief Public Service API contract for the SN74HC595N shift register driver.
 * @author (Your Name) & Synapse AI Assistant
 * @version 1.0.0
 * @date 2025-08-20
 */
#ifndef FMW_SN74HC595N_WRITER_INTERFACE_H
#define FMW_SN74HC595N_WRITER_INTERFACE_H

#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The public Service API structure for the SN74HC595N writer module.
 * @details This is the "contract" that the sn74hc595n_writer module implements
 *          and other modules consume via the Service Locator.
 */
typedef struct {
    /**
     * @brief Sets the state of a single output pin on the shift register.
     * @param[in] context The context handle for the specific shift register instance.
     * @param[in] pin The pin number to set (0 to (num_chips * 8) - 1).
     * @param[in] state `true` for HIGH, `false` for LOW.
     * @return ESP_OK on success.
     */
    esp_err_t (*set_pin_state)(void *context, uint16_t pin, bool state);

    /**
     * @brief Sets the state of all pins for a specific chip in the chain.
     * @param[in] context The context handle.
     * @param[in] chip_index The index of the chip to control (0 for the first chip).
     * @param[in] value An 8-bit value representing the state of all 8 pins.
     * @return ESP_OK on success.
     */
    esp_err_t (*set_chip_pins)(void *context, uint8_t chip_index, uint8_t value);

    /**
     * @brief Gets the last written state of a single pin.
     * @details This reads from the driver's internal buffer, not the physical pin.
     * @param[in] context The context handle.
     * @param[in] pin The pin number to query.
     * @param[out] out_state Pointer to store the retrieved state.
     * @return ESP_OK on success.
     */
    esp_err_t (*get_pin_state)(void *context, uint16_t pin, bool *out_state);

} sn74hc595n_writer_api_t;

/**
 * @brief The handle type for the SN74HC595N writer service.
 * @details This is used to pass around the service instance in a type-safe manner.
 */
typedef struct {
    sn74hc595n_writer_api_t *api;  ///< Pointer to the service API
    void *context;                 ///< Context for the specific instance (e.g., private data)
} sn74hc595n_writer_handle_t;

#ifdef __cplusplus
}
#endif

#endif // FMW_SN74HC595N_WRITER_INTERFACE_H