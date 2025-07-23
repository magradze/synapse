/**
 * @file ssd1306_interface.h
 * @brief Public Service API contract for SSD1306-compatible OLED displays.
 * @author Giorgi Magradze & Synapse AI Expert
 * @version 1.0.0
 * @date 2025-07-26
 */
#ifndef SSD1306_INTERFACE_H
#define SSD1306_INTERFACE_H

#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The public Service API structure for an SSD1306 display module.
 */
typedef struct {
    /**
     * @brief Enables the display and its background task.
     * @param[in] context The context handle for the specific display instance.
     * @return ESP_OK on success.
     */
    esp_err_t (*enable)(void* context);

    /**
     * @brief Disables the display, turns it off, and suspends its task.
     * @param[in] context The context handle.
     * @return ESP_OK on success.
     */
    esp_err_t (*disable)(void* context);

    /**
     * @brief Clears the entire display screen.
     * @param[in] context The context handle.
     * @return ESP_OK on success.
     */
    esp_err_t (*clear)(void* context);

    /**
     * @brief Writes a line of text to the display.
     * @param[in] context The context handle.
     * @param[in] line_num The line number to write to (e.g., 0 for the first line).
     * @param[in] format A printf-style format string.
     * @param[in] ... Variable arguments for the format string.
     * @return ESP_OK on success.
     */
    esp_err_t (*write_text)(void* context, uint8_t line_num, const char *format, ...);

} ssd1306_api_t;

/**
 * @brief The handle returned by the Service Locator for a display service.
 */
typedef struct {
    const ssd1306_api_t *api; /**< @brief Pointer to the table of display API functions. */
    void* context;            /**< @brief Opaque context pointer for the API functions. */
} ssd1306_handle_t;

#ifdef __cplusplus
}
#endif

#endif // SSD1306_INTERFACE_H