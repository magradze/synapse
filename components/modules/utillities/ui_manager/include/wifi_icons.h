/**
 * @file wifi_icons.h
 * @brief Bitmap data for WiFi signal strength icons (8x8 pixels).
 * @author Giorgi Magradze
 * @date 2025-07-27
 */
#ifndef WIFI_ICONS_H
#define WIFI_ICONS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_ICON_WIDTH 8
#define WIFI_ICON_HEIGHT 8

// A simple "X" icon for disconnected state
const uint8_t wifi_icon_disconnected[] = {
    0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81};

// WiFi signal strength 1 bar
const uint8_t wifi_icon_signal_1[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18};

// WiFi signal strength 2 bars
const uint8_t wifi_icon_signal_2[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x18};

// WiFi signal strength 3 bars
const uint8_t wifi_icon_signal_3[] = {
    0x00, 0x00, 0x00, 0x7E, 0x00, 0x3C, 0x00, 0x18};

// WiFi signal strength 4 bars (full)
const uint8_t wifi_icon_signal_4[] = {
    0x00, 0xFF, 0x00, 0x7E, 0x00, 0x3C, 0x00, 0x18};

#ifdef __cplusplus
}
#endif

#endif // WIFI_ICONS_H