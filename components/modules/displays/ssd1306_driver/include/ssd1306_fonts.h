/**
 * @file ssd1306_fonts.h
 * @brief Public declarations for bitmap fonts used by the SSD1306 driver.
 * @details This file provides external declarations for the font data arrays
 *          and defines useful macros for their dimensions and character ranges.
 *          By centralizing these definitions, other modules or utilities can
 *          easily access font metrics without needing to know the internal
 *          implementation details of the font data.
 * @author Giorgi Magradze
 * @version 1.1.0
 * @date 2025-08-14
 */
#ifndef SSD1306_FONTS_H
#define SSD1306_FONTS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- Macros for 8x16 Font ---
#define SSD1306_LARGE_FONT_WIDTH 8      /**< @brief The width of a character in the large font, in pixels. */
#define SSD1306_LARGE_FONT_HEIGHT 16    /**< @brief The height of a character in the large font, in pixels. */
#define FONT_HEADER_SIZE_8X16 4         /**< @brief The size of the header in the font data array (in bytes). */
#define FONT_FIRST_CHAR_8X16 0x20       /**< @brief The ASCII code of the first character defined in the large font. */

// --- Macros for 6x8 Font ---
#define SSD1306_SMALL_FONT_WIDTH 6      /**< @brief The width of a character in the small font, in pixels. */
#define SSD1306_SMALL_FONT_HEIGHT 8     /**< @brief The height of a character in the small font, in pixels. */
#define FONT_HEADER_SIZE_6X8 4          /**< @brief The size of the header in the font data array (in bytes). */
#define FONT_FIRST_CHAR_6X8 0x20        /**< @brief The ASCII code of the first character defined in the small font. */
#define FONT_CHAR_COUNT_6X8 96          /**< @brief The total number of characters defined in the small font. */

/**
 * @brief External declaration for the 8x16 pixel font data array.
 * @details The actual definition of this array is in `ssd1306_fonts.c`.
 */
extern const uint8_t ssd1306xled_font8x16[];

/**
 * @brief External declaration for the 6x8 pixel font data array.
 * @details The actual definition of this array is in `ssd1306_fonts.c`.
 */
extern const uint8_t ssd1306xled_font6x8[];

#ifdef __cplusplus
}
#endif

#endif // SSD1306_FONTS_H