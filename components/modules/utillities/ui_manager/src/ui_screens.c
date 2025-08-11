/**
 * @file ui_screens.c
 * @brief Implements rendering functions for different UI screens.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-08-13
 */

#include "synapse.h"
#include "ui_manager_internal.h"
#include "roboto_bold_24.h"
#include "roboto_bold_24_digits.h"
#include "wifi_icons.h"
#include "ssd1306_fonts.h"
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "relay_interface.h"

#define SYSTEM_NAME "SYNAPSE"

// --- Helper Functions (now static as they are used only in this file) ---
static void draw_bitmap(ui_manager_private_data_t *private_data, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint32_t color);
static void draw_large_ascii_string(ui_manager_private_data_t *private_data, int16_t x, int16_t y, const char *str, uint32_t color);
static void draw_large_digits_string(ui_manager_private_data_t *private_data, int16_t x, int16_t y, const char *str, uint32_t color);
void format_module_name(const char *original, char *formatted, size_t size);

/**
 * @brief Renders the status header at the top of the screen.
 * @details The header contains status icons such as WiFi signal strength. This
 *          function determines which icon to display based on the cached WiFi
 *          status and calls the `draw_bitmap` helper to render it.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] title The title text to display on the left side of the header.
 */
void render_header(ui_manager_private_data_t *private_data, const char *title)
{
    const display_driver_api_t *display = private_data->display->api;
    void *context = private_data->display->context;

    // Draw title on the left
    if (title)
    {
        display->draw_formatted_text(context, 2, 1, 1, title);
    }

    // Draw WiFi icon on the right
    const uint8_t *icon = NULL;
    bool connected = private_data->wifi_connected;
    int32_t rssi = private_data->wifi_rssi;

    if (connected)
    {
        if (rssi >= -67)
            icon = wifi_icon_signal_4;
        else if (rssi >= -70)
            icon = wifi_icon_signal_3;
        else if (rssi >= -80)
            icon = wifi_icon_signal_2;
        else
            icon = wifi_icon_signal_1;
    }
    else
    {
        icon = wifi_icon_disconnected;
    }

    if (icon)
    {
        draw_bitmap(private_data, private_data->display_width - WIFI_ICON_WIDTH - 2, 1, icon, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, 1);
    }
}

/**
 * @brief Renders the initial splash screen.
 * @details This function calculates the centered position for the "SYNAPSE" text
 *          and uses the large ASCII font to draw it. This screen is displayed
 *          for a short duration on startup.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
void render_splash_screen(ui_manager_private_data_t *private_data)
{
    const char *text = SYSTEM_NAME;
    int total_width = 0;
    for (const char *s = text; *s; s++)
    {
        if (*s >= FONT_ASCII_START_CHAR && *s <= FONT_ASCII_END_CHAR)
        {
            int char_index = *s - FONT_ASCII_START_CHAR;
            total_width += char_width[char_index] + 2;
        }
    }
    total_width -= 2;

    int16_t start_x = (private_data->display_width - total_width) / 2;
    int16_t start_y = (private_data->display_height - ROBOTO_BOLD_24_HEIGHT) / 2;

    draw_large_ascii_string(private_data, start_x, start_y, text, 1);
}

/**
 * @brief Renders the main home screen.
 * @details This is the default screen after the splash screen. It displays the
 *          current time (using the large digits font), the "SYNAPSE" brand name
 *          (using the small font), and the "MENU" footer button.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
void render_home_screen(ui_manager_private_data_t *private_data)
{
    const display_driver_api_t *display = private_data->display->api;
    void *context = private_data->display->context;

    char time_str[9];

    time_t now;
    // Check if the service exists AND if the time is successfully retrieved (synced)
    if (private_data->time_sync && private_data->time_sync->get_time(&now) == ESP_OK)
    {
        struct tm timeinfo;
        localtime_r(&now, &timeinfo);
        strftime(time_str, sizeof(time_str), "%H:%M", &timeinfo);
    }
    else
    {
        // Fallback if service is missing OR time is not yet synced
        strcpy(time_str, "--:--");
    }

    int total_width = 0;
    for (const char *s = time_str; *s; s++)
    {
        int char_index = (*s >= '0' && *s <= '9') ? (*s - '0') : 10;
        total_width += char_width_digits[char_index] + 2;
    }
    total_width -= 2;

    int16_t start_x = (private_data->display_width - total_width) / 2;
    draw_large_digits_string(private_data, start_x, 18, time_str, 1);

    int synapse_text_width = strlen(SYSTEM_NAME) * private_data->small_font_metrics.width;
    int synapse_start_x = (private_data->display_width - synapse_text_width) / 2;
    display->draw_formatted_text(context, synapse_start_x, 45, 1, SYSTEM_NAME);

    render_footer_button(private_data, "MENU", false);
}

/**
 * @brief Renders a generic button in the footer area (bottom-right).
 * @details This function dynamically calculates the button's size based on the
 *          text length and applies different styling if the button is selected.
 *          It is used for both "MENU" and "BACK" buttons to ensure a consistent look.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] text The text to display inside the button.
 * @param[in] is_selected If true, the button is rendered with inverted colors.
 */
void render_footer_button(ui_manager_private_data_t *private_data, const char *text, bool is_selected)
{
    const display_driver_api_t *display = private_data->display->api;
    void *context = private_data->display->context;

    int padding_x = 4;
    int padding_y = 2;
    int margin = 2;

    int text_width = strlen(text) * private_data->small_font_metrics.width;
    int button_width = text_width + (padding_x * 2);
    int button_height = private_data->small_font_metrics.height + (padding_y * 2);

    int button_x = private_data->display_width - button_width - margin;
    int button_y = private_data->display_height - button_height - margin;

    uint32_t bg_color = is_selected ? 1 : 0;
    uint32_t text_color = is_selected ? 0 : 1;

    if (private_data->current_state == UI_STATE_HOME)
    {
        bg_color = 1;
        text_color = 0;
    }

    display->fill_rect(context, button_x, button_y, button_width, button_height, bg_color);
    display->draw_formatted_text(context, button_x + padding_x, button_y + padding_y, text_color, text);
}

/**
 * @internal
 * @brief Draws a monochrome bitmap to the screen buffer.
 * @details This is a low-level helper function that iterates through an 8-bit-per-row
 *          bitmap array and calls the driver's `draw_pixel` function for each set bit.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] x The top-left X coordinate where the bitmap should be drawn.
 * @param[in] y The top-left Y coordinate where the bitmap should be drawn.
 * @param[in] bitmap A pointer to the bitmap data array.
 * @param[in] w The width of the bitmap in pixels.
 * @param[in] h The height of the bitmap in pixels.
 * @param[in] color The color to use for drawing the set pixels.
 */
static void draw_bitmap(ui_manager_private_data_t *private_data, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint32_t color)
{
    const display_driver_api_t *display = private_data->display->api;
    void *context = private_data->display->context;
    for (int16_t j = 0; j < h; j++, y++)
    {
        for (int16_t i = 0; i < w; i++)
        {
            if ((bitmap[j] >> i) & 0x1)
            {
                display->draw_pixel(context, x + i, y, color);
            }
        }
    }
}

/**
 * @internal
 * @brief Draws a string using the full ASCII proportional font.
 * @details This function renders a string using the complete Roboto Bold 24px font set.
 *          It looks up each character's width and bitmap data from the `char_width` and
 *          `char_addr` lookup tables and renders it pixel by pixel.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] x The starting X coordinate of the string.
 * @param[in] y The starting Y coordinate of the string.
 * @param[in] str The null-terminated string to draw.
 * @param[in] color The color of the text.
 */
static void draw_large_ascii_string(ui_manager_private_data_t *private_data, int16_t x, int16_t y, const char *str, uint32_t color)
{
    int16_t current_x = x;
    for (const char *s = str; *s; s++)
    {
        char c = *s;
        if (c < FONT_ASCII_START_CHAR || c > FONT_ASCII_END_CHAR)
            c = '?';

        int char_index = c - FONT_ASCII_START_CHAR;

        const uint8_t *bitmap = char_addr[char_index];
        int width = char_width[char_index];

        int byte_index = 0;
        for (int page = 0; page < (ROBOTO_BOLD_24_HEIGHT / 8); page++)
        {
            for (int col = 0; col < width; col++)
            {
                uint8_t byte_data = bitmap[byte_index++];
                for (int bit = 0; bit < 8; bit++)
                {
                    if ((byte_data >> bit) & 0x01)
                    {
                        private_data->display->api->draw_pixel(private_data->display->context, current_x + col, y + (page * 8) + bit, color);
                    }
                }
            }
        }
        current_x += width + 2;
    }
}

/**
 * @internal
 * @brief Draws a string using the digits-only proportional font.
 * @details This function is optimized for rendering strings containing only numbers
 *          and colons (like time). It uses the smaller `char_width_digits` and
 *          `char_addr_digits` lookup tables for efficiency.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] x The starting X coordinate of the string.
 * @param[in] y The starting Y coordinate of the string.
 * @param[in] str The null-terminated string to draw (e.g., "14:05").
 * @param[in] color The color of the text.
 */
static void draw_large_digits_string(ui_manager_private_data_t *private_data, int16_t x, int16_t y, const char *str, uint32_t color)
{
    int16_t current_x = x;
    for (const char *s = str; *s; s++)
    {
        char c = *s;
        int char_index = -1;

        if (c >= '0' && c <= '9')
            char_index = c - '0';
        else if (c == ':')
            char_index = 10;
        else if (c == '-')
            char_index = 11;

        if (char_index != -1)
        {
            const uint8_t *bitmap = char_addr_digits[char_index];
            int width = char_width_digits[char_index];

            int byte_index = 0;
            for (int page = 0; page < (ROBOTO_BOLD_24_HEIGHT / 8); page++)
            {
                for (int col = 0; col < width; col++)
                {
                    uint8_t byte_data = bitmap[byte_index++];
                    for (int bit = 0; bit < 8; bit++)
                    {
                        if ((byte_data >> bit) & 0x01)
                        {
                            private_data->display->api->draw_pixel(private_data->display->context, current_x + col, y + (page * 8) + bit, color);
                        }
                    }
                }
            }
            current_x += width + 2;
        }
    }
}

/**
 * @brief Formats a snake_case module name into a Title Case string for display.
 * @param[in] original The original module name (e.g., "relay_actuator").
 * @param[out] formatted The buffer to store the formatted name (e.g., "Relay Actuator").
 * @param[in] size The size of the formatted buffer.
 */
void format_module_name(const char *original, char *formatted, size_t size)
{
    if (!original || !formatted || size == 0)
    {
        if (formatted && size > 0)
            formatted[0] = '\0';
        return;
    }

    size_t i = 0;
    bool capitalize = true;
    while (*original && i < size - 1)
    {
        if (*original == '_')
        {
            formatted[i++] = ' ';
            capitalize = true;
        }
        else
        {
            if (capitalize)
            {
                formatted[i++] = toupper((unsigned char)*original);
                capitalize = false;
            }
            else
            {
                formatted[i++] = *original;
            }
        }
        original++;
    }
    formatted[i] = '\0';
}