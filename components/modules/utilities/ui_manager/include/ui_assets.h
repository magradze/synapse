/**
 * @file ui_assets.h
 * @brief Declares external UI assets like fonts and icons.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-08-02
 */
#ifndef UI_ASSETS_H
#define UI_ASSETS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Large numeric font (example: 24 pixels high)
extern const uint8_t font_large_numeric[][48];

// Battery icons (example: 20x10 pixels)
extern const uint8_t icon_battery_full[];
extern const uint8_t icon_battery_75[];
extern const uint8_t icon_battery_50[];
extern const uint8_t icon_battery_25[];
extern const uint8_t icon_battery_empty[];
extern const uint8_t font_16x24_numeric[][48];

#ifdef __cplusplus
}
#endif

#endif // UI_ASSETS_H