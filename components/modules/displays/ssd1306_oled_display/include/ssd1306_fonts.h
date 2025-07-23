/**
 * @file ssd1306_fonts.h
 * @brief Extern declarations for font data.
 * @author Giorgi Magradze
 */
#ifndef SSD1306_FONTS_H
#define SSD1306_FONTS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- Macros for 8x16 Font ---
#define FONT_HEADER_SIZE_8X16 4
#define FONT_FIRST_CHAR_8X16 0x20

// --- Macros for 6x8 Font ---
#define FONT_HEADER_SIZE_6X8 4
#define FONT_FIRST_CHAR_6X8 0x20
#define FONT_CHAR_COUNT_6X8 96

// Extern declaration for the font data defined in ssd1306_fonts.c
extern const uint8_t ssd1306xled_font8x16[];
extern const uint8_t ssd1306xled_font6x8[];

#ifdef __cplusplus
}
#endif

#endif // SSD1306_FONTS_H