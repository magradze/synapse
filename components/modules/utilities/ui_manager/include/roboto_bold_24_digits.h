/**
 * @file roboto_bold_24_digits.h
 * @brief Bitmap font data for Roboto Bold 24px (Digits and Colon only).
 * @author Synapse Framework Team
 * @version 1.0.2
 * @date 2025-08-08
 */
#ifndef ROBOTO_BOLD_24_DIGITS_H
#define ROBOTO_BOLD_24_DIGITS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ROBOTO_BOLD_24_HEIGHT 24
#define ROBOTO_BOLD_24_WIDTH 24

// Font bitmaps for characters '0' through ':'
const uint8_t bitmap_digit_48[] = {128,240,248,248,60,28,28,28,60,248,248,224,128,255,255,255,255,0,0,0,0,0,255,255,255,255,0,7,15,15,30,28,28,28,30,15,15,3,0};
const uint8_t bitmap_digit_49[] = {224,240,112,112,248,248,252,252,0,0,0,0,255,255,255,255,0,0,0,0,31,31,31,31};
const uint8_t bitmap_digit_50[] = {224,240,248,248,60,28,28,28,60,248,248,240,0,0,0,0,128,192,224,240,124,62,31,15,3,0,28,30,31,31,31,31,28,28,28,28,28,28,28};
const uint8_t bitmap_digit_51[] = {96,112,120,124,60,28,28,28,60,248,248,240,0,0,0,0,0,28,28,28,28,62,255,247,227,192,3,7,15,31,30,28,28,28,30,15,15,7,1};
const uint8_t bitmap_digit_52[] = {0,0,0,0,128,224,248,252,252,252,252,0,0,224,240,252,255,207,195,193,255,255,255,255,192,192,1,1,1,1,1,1,1,31,31,31,31,1,1};
const uint8_t bitmap_digit_53[] = {0,224,252,252,252,28,28,28,28,28,28,28,0,128,143,143,143,7,7,7,7,15,255,254,252,240,1,7,15,15,30,28,28,28,30,15,15,7,1};
const uint8_t bitmap_digit_54[] = {0,128,224,240,248,120,56,28,28,28,0,0,0,252,255,255,255,14,7,7,7,15,255,254,252,248,0,3,7,15,30,28,28,28,30,15,15,7,1};
const uint8_t bitmap_digit_55[] = {28,28,28,28,28,28,28,28,220,252,252,124,28,0,0,0,0,128,224,252,255,63,15,3,0,0,0,0,24,30,31,31,7,1,0,0,0,0,0};
const uint8_t bitmap_digit_56[] = {0,240,248,248,60,28,28,28,60,248,248,240,0,192,227,247,255,62,28,28,28,62,255,247,227,192,1,7,15,15,30,28,28,28,30,15,15,7,3};
const uint8_t bitmap_digit_57[] = {192,240,248,248,60,28,28,28,124,248,240,224,0,7,31,63,127,120,112,112,112,184,255,255,255,31,0,0,0,28,28,28,30,14,15,7,3,0,0};
const uint8_t bitmap_digit_58[] = {128,128,128,0,7,7,7,3,30,30,30,12};
const uint8_t bitmap_digit_45[] = {0, 0, 0, 0, 60, 60, 60, 60, 0, 0, 0, 0, 0, 0, 0, 0};

const uint8_t char_width_digits[] = {13, 8, 13, 13, 13, 13, 13, 13, 13, 13, 4, 8};

const uint8_t *const char_addr_digits[] = {bitmap_digit_48, bitmap_digit_49, bitmap_digit_50, bitmap_digit_51, bitmap_digit_52, bitmap_digit_53, bitmap_digit_54, bitmap_digit_55, bitmap_digit_56, bitmap_digit_57, bitmap_digit_58, bitmap_digit_45};

#ifdef __cplusplus
}
#endif

#endif // ROBOTO_BOLD_24_DIGITS_H