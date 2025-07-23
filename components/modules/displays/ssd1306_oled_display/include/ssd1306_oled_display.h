/**
 * @file ssd1306_oled_display.h
 * @brief A module to control SSD1306 OLED displays via I2C.
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-07-26
 */
#ifndef SSD1306_OLED_DISPLAY_H
#define SSD1306_OLED_DISPLAY_H

#include "base_module.h"
#include "cJSON.h"
#include "ssd1306_interface.h" // Include the actual contract

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the SSD1306 OLED Display module.
 * @param[in] config A pointer to the cJSON configuration object for this instance.
 * @return A pointer to the newly created module_t structure, or NULL on failure.
 */
module_t *ssd1306_oled_display_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // SSD1306_OLED_DISPLAY_H