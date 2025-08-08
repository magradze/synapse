/**
 * @file display_driver_interface.h
 * @brief Universal Service API contract for display drivers.
 * @details This file defines the abstract interface that all display drivers
 *          (e.g., SSD1306, ST7789) in the Synapse Framework must implement.
 *          Its purpose is to completely decouple high-level UI logic (from ui_manager)
 *          from the low-level hardware implementation.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-07-31
 */

#ifndef SYNAPSE_DISPLAY_DRIVER_INTERFACE_H
#define SYNAPSE_DISPLAY_DRIVER_INTERFACE_H

#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h> // For va_list used in formatted text functions

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct display_info_t
 * @brief Contains the core physical characteristics of a display.
 * @details This structure is used by the ui_manager to understand the capabilities
 *          of a specific display and adapt the UI rendering accordingly.
 */
typedef struct {
    uint16_t width;     /**< @brief The width of the display in pixels. */
    uint16_t height;    /**< @brief The height of the display in pixels. */
    bool is_color;      /**< @brief True if the display supports color. */
    uint8_t bpp;        /**< @brief Color depth in Bits Per Pixel (e.g., 1, 16, 24). */
} display_info_t;


/**
 * @struct font_metrics_t
 * @brief Contains metrics for a font supported by the driver.
 */
typedef struct {
    uint8_t width;
    uint8_t height;
} font_metrics_t;

/**
 * @enum display_rotation_t
 * @brief Defines the possible screen orientation settings.
 */
typedef enum {
    DISPLAY_ROTATION_0 = 0, /**< @brief Default orientation (0 degrees). */
    DISPLAY_ROTATION_90,    /**< @brief Rotated 90 degrees clockwise. */
    DISPLAY_ROTATION_180,   /**< @brief Rotated 180 degrees. */
    DISPLAY_ROTATION_270,   /**< @brief Rotated 270 degrees clockwise. */
} display_rotation_t;


/**
 * @struct display_driver_api_t
 * @brief The public Service API structure for a display driver.
 * @details This is the "contract" that all display drivers implement.
 *          All functions take a `void* context` parameter, which allows them
 *          to operate on a specific display instance.
 */
typedef struct {
    /**
     * @brief Retrieves information about the display's capabilities.
     * @param[in] context The service context, providing access to a specific instance.
     * @param[out] info_out A pointer to a structure where the display info will be stored.
     * @return ESP_OK on success.
     */
    esp_err_t (*get_info)(void* context, display_info_t* info_out);

    /**
     * @brief Draws a single pixel to the internal screen buffer.
     * @details To reflect the change on the physical screen, `update_screen()` must be called.
     * @param[in] context The service context.
     * @param[in] x The horizontal coordinate of the pixel.
     * @param[in] y The vertical coordinate of the pixel.
     * @param[in] color The color of the pixel. The format is driver-dependent
     *                  (e.g., 0/1 for monochrome, RGB888 for color).
     * @return ESP_OK on success.
     */
    esp_err_t (*draw_pixel)(void* context, int16_t x, int16_t y, uint32_t color);

    /**
     * @brief Fills a rectangular area in the internal buffer with a specified color.
     * @param[in] context The service context.
     * @param[in] x The X coordinate of the top-left corner.
     * @param[in] y The Y coordinate of the top-left corner.
     * @param[in] w The width of the rectangle.
     * @param[in] h The height of the rectangle.
     * @param[in] color The color to fill with.
     * @return ESP_OK on success.
     */
    esp_err_t (*fill_rect)(void* context, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);

    /**
     * @brief Draws a formatted text string to the internal buffer.
     * @details The driver should provide support for at least one basic font.
     *          This function behaves like printf.
     * @param[in] context The service context.
     * @param[in] x The X coordinate for the start of the text.
     * @param[in] y The Y coordinate for the start of the text.
     * @param[in] color The color of the text.
     * @param[in] format A null-terminated, printf-style format string.
     * @param[in] ... Variable arguments for the format string.
     * @return ESP_OK on success.
     */
    esp_err_t (*draw_formatted_text)(void* context, int16_t x, int16_t y, uint32_t color, const char* format, ...);

    /**
     * @brief Pushes the contents of the internal buffer to the physical display.
     * @details This function must be called after any drawing operations to make
     *          the changes visible on the screen.
     * @param[in] context The service context.
     * @return ESP_OK on success.
     */
    esp_err_t (*update_screen)(void* context);

    /**
     * @brief Clears the internal buffer (typically fills with the background color, e.g., black).
     * @param[in] context The service context.
     * @return ESP_OK on success.
     */
    esp_err_t (*clear)(void* context);

    /**
     * @brief Turns the display panel on or off.
     * @param[in] context The service context.
     * @param[in] on `true` to turn on, `false` to turn off.
     * @return ESP_OK on success.
     */
    esp_err_t (*set_power)(void* context, bool on);

    /**
     * @brief Sets the display orientation.
     * @param[in] context The service context.
     * @param[in] rotation The new orientation from the `display_rotation_t` enum.
     * @return ESP_OK on success.
     * @retval ESP_ERR_NOT_SUPPORTED if the driver does not support rotation.
     */
    esp_err_t (*set_rotation)(void* context, display_rotation_t rotation);

    /**
     * @brief Sets the display contrast or brightness.
     * @param[in] context The service context.
     * @param[in] contrast A value from 0 to 255.
     * @return ESP_OK on success.
     */
    esp_err_t (*set_contrast)(void* context, uint8_t contrast);

    /**
     * @brief Gets the metrics for the default small font.
     * @param[in] context The service context.
     * @param[out] metrics_out A pointer to a structure where the font metrics will be stored.
     * @return ESP_OK on success.
     */
    esp_err_t (*get_small_font_metrics)(void* context, font_metrics_t* metrics_out);

} display_driver_api_t;

/**
 * @struct display_driver_handle_t
 * @brief The handle returned by the Service Locator for a display driver service.
 * @details This structure bundles the API function table with the necessary
 *          context for that specific instance.
 */
typedef struct {
    const display_driver_api_t *api; /**< @brief Pointer to the table of display API functions. */
    void* context;                   /**< @brief Opaque context pointer for the API functions. */
} display_driver_handle_t;


#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_DISPLAY_DRIVER_INTERFACE_H