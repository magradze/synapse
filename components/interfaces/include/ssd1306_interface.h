/**
 * @file ssd1306_interface.h
 * @brief Public Service API contract for SSD1306-compatible OLED displays.
 * @author Giorgi Magradze & Synapse AI Expert
 * @version 2.0.0
 * @date 2025-07-28
 */
#ifndef SSD1306_INTERFACE_H
#define SSD1306_INTERFACE_H

#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h> // For bool type
#include <stdarg.h>  // For va_list

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The public Service API structure for an SSD1306 display module.
 */
typedef struct {
    /**
     * @brief Enables the display.
     * @param[in] context The context handle for the specific display instance.
     * @return ESP_OK on success.
     */
    esp_err_t (*enable)(void* context);

    /**
     * @brief Disables the display (turns off the screen).
     * @param[in] context The context handle.
     * @return ESP_OK on success.
     */
    esp_err_t (*disable)(void* context);

    /**
     * @brief Clears the internal screen buffer. Call update_screen() to see the change.
     * @param[in] context The context handle.
     * @return ESP_OK on success.
     */
    esp_err_t (*clear)(void* context);

    /**
     * @brief Writes a formatted text string to the buffer using the small 6x8 font.
     * @param[in] context The context handle.
     * @param[in] x The starting horizontal position (column).
     * @param[in] y The starting vertical position (page/line, 0-7).
     * @param[in] format A printf-style format string.
     * @param[in] ... Variable arguments for the format string.
     * @return ESP_OK on success.
     */
    esp_err_t (*write_text)(void *context, uint8_t x, uint8_t y, const char *format, ...);

    /**
     * @brief Draws a single pixel in the internal buffer.
     * @param[in] context The context handle.
     * @param[in] x The horizontal position.
     * @param[in] y The vertical position.
     * @param[in] color `true` for white (pixel on), `false` for black (pixel off).
     * @return ESP_OK on success.
     */
    esp_err_t (*draw_pixel)(void *context, uint8_t x, uint8_t y, bool color);

    /**
     * @brief Draws a horizontal line in the internal buffer.
     * @param[in] context The context handle.
     * @param[in] x The starting horizontal position.
     * @param[in] y The vertical position.
     * @param[in] width The width of the line in pixels.
     * @return ESP_OK on success.
     */
    esp_err_t (*draw_hline)(void *context, uint8_t x, uint8_t y, uint8_t width);

    /**
     * @brief Draws a vertical line in the internal buffer.
     * @param[in] context The context handle.
     * @param[in] x The horizontal position.
     * @param[in] y The starting vertical position.
     * @param[in] height The height of the line in pixels.
     * @return ESP_OK on success.
     */
    esp_err_t (*draw_vline)(void *context, uint8_t x, uint8_t y, uint8_t height);

    /**
     * @brief Draws a rectangle outline in the internal buffer.
     * @param[in] context The context handle.
     * @param[in] x The top-left corner's horizontal position.
     * @param[in] y The top-left corner's vertical position.
     * @param[in] width The width of the rectangle.
     * @param[in] height The height of the rectangle.
     * @return ESP_OK on success.
     */
    esp_err_t (*draw_rect)(void *context, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

    /**
     * @brief Draws a filled rectangle in the internal buffer.
     * @param[in] context The context handle.
     * @param[in] x The top-left corner's horizontal position.
     * @param[in] y The top-left corner's vertical position.
     * @param[in] width The width of the rectangle.
     * @param[in] height The height of the rectangle.
     * @return ESP_OK on success.
     */
    esp_err_t (*fill_rect)(void *context, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

    /**
     * @brief Pushes the content of the internal buffer to the physical display.
     * @details All `draw_*` operations modify an internal memory buffer. This
     *          function must be called to make the changes visible on the screen.
     * @param[in] context The context handle.
     * @return ESP_OK on success.
     */
    esp_err_t (*update_screen)(void *context);

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