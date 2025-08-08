/**
 * @file mcp23017_interface.h
 * @brief Public Service API contract for the MCP23017 I/O Expander module.
 * @author Synapse Team
 * @version 1.0.0
 * @date 2025-08-21
 */
#ifndef SYNAPSE_MCP23017_INTERFACE_H
#define SYNAPSE_MCP23017_INTERFACE_H

#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defines the direction for an MCP23017 pin.
 */
typedef enum {
    MCP23017_DIRECTION_OUTPUT = 0,
    MCP23017_DIRECTION_INPUT = 1,
} mcp23017_direction_t;

/**
 * @brief The public Service API structure for the MCP23017 I/O Expander module.
 */
typedef struct {
    /**
     * @brief Sets the direction of a single pin.
     * @param[in] context The context handle for the specific MCP23017 instance.
     * @param[in] pin The pin number to configure (0-15).
     * @param[in] direction The direction to set (INPUT or OUTPUT).
     * @return ESP_OK on success.
     */
    esp_err_t (*set_pin_direction)(void *context, uint8_t pin, mcp23017_direction_t direction);

    /**
     * @brief Enables or disables the internal pull-up resistor for a single input pin.
     * @param[in] context The context handle.
     * @param[in] pin The pin number to configure (0-15).
     * @param[in] enabled `true` to enable the pull-up, `false` to disable.
     * @return ESP_OK on success.
     */
    esp_err_t (*set_pin_pullup)(void *context, uint8_t pin, bool enabled);

    /**
     * @brief Sets the logical level of a single output pin.
     * @param[in] context The context handle.
     * @param[in] pin The pin number to set (0-15).
     * @param[in] level `true` for HIGH, `false` for LOW.
     * @return ESP_OK on success.
     */
    esp_err_t (*set_pin_level)(void *context, uint8_t pin, bool level);

    /**
     * @brief Gets the logical level of a single input pin.
     * @param[in] context The context handle.
     * @param[in] pin The pin number to read (0-15).
     * @param[out] out_level Pointer to a boolean where the pin level will be stored.
     * @return ESP_OK on success.
     */
    esp_err_t (*get_pin_level)(void *context, uint8_t pin, bool *out_level);

    /**
     * @brief Writes a 16-bit value to both ports (A and B) simultaneously.
     * @param[in] context The context handle.
     * @param[in] value The 16-bit value to write. LSB is Port A, MSB is Port B.
     * @return ESP_OK on success.
     */
    esp_err_t (*write_all_pins)(void *context, uint16_t value);

    /**
     * @brief Reads a 16-bit value from both ports (A and B) simultaneously.
     * @param[in] context The context handle.
     * @param[out] out_value Pointer to a 16-bit integer to store the result.
     * @return ESP_OK on success.
     */
    esp_err_t (*read_all_pins)(void *context, uint16_t *out_value);

} mcp23017_api_t;

/**
 * @brief The handle returned by the Service Locator for an MCP23017 service.
 */
typedef struct {
    const mcp23017_api_t *api;
    void* context;
} mcp23017_handle_t;

#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_MCP23017_INTERFACE_H