/**
 * @file ssd1306_oled_display.c
 * @brief Implementation for the SSD1306 OLED display module.
 * @author Giorgi Magradze
 * @version 2.3.0
 * @date 2025-07-28
 * @details This is the final, fully functional, and corrected version. It
 *          implements a synchronous graphics API operating on an internal
 *          screen buffer, displays a splash screen, and features a dynamic
 *          header with a periodically updated WiFi status icon.
 */
#include "ssd1306_oled_display.h"
#include "ssd1306_interface.h"
#include "logging.h"
#include "service_locator.h"
#include "service_types.h"
#include "i2c_bus_interface.h"
#include "timer_interface.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "event_payloads.h"
#include "framework_events.h"
#include "ssd1306_fonts.h"
#include "wifi_icons.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

DEFINE_COMPONENT_TAG("SSD1306_DISPLAY");

#define WIFI_STATUS_UPDATE_INTERVAL_MS 5000
#define EVT_INTERNAL_UPDATE_WIFI "INT_EVT_UPDATE_WIFI"
#define LINE_BUFFER_SIZE 32
#define HEADER_PAGE_NUM 0
#define CONTENT_START_PAGE 1

/** @internal @brief Private data for the SSD1306 module instance. */
typedef struct
{
    char instance_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];
    char i2c_bus_instance[CONFIG_FMW_SERVICE_NAME_MAX_LENGTH];
    uint8_t i2c_address;
    uint16_t width;
    uint16_t height;
    char init_text[LINE_BUFFER_SIZE];

    i2c_bus_handle_t *i2c_bus_service;
    uint8_t *screen_buffer;

    ssd1306_handle_t service_handle;
    ssd1306_api_t service_api;

    bool is_enabled;
    bool wifi_connected;
    int32_t wifi_rssi;
} ssd1306_private_data_t;

// --- Forward Declarations ---
static esp_err_t ssd1306_init(module_t *self);
static esp_err_t ssd1306_start(module_t *self);
static void ssd1306_deinit(module_t *self);
static void ssd1306_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t parse_config(const cJSON *config_node, ssd1306_private_data_t *private_data);

static esp_err_t api_enable(void *context);
static esp_err_t api_disable(void *context);
static esp_err_t api_clear(void *context);
static esp_err_t api_write_text(void *context, uint8_t x, uint8_t y, const char *format, ...);
static esp_err_t api_draw_pixel(void *context, uint8_t x, uint8_t y, bool color);
static esp_err_t api_draw_hline(void *context, uint8_t x, uint8_t y, uint8_t width);
static esp_err_t api_draw_vline(void *context, uint8_t x, uint8_t y, uint8_t height);
static esp_err_t api_draw_rect(void *context, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
static esp_err_t api_fill_rect(void *context, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
static esp_err_t api_update_screen(void *context);

static void draw_pixel_internal(ssd1306_private_data_t *private_data, uint8_t x, uint8_t y, bool color);
static void screen_draw_large_text_internal(ssd1306_private_data_t *private_data, uint8_t line, const char *text);
static void draw_header(ssd1306_private_data_t *private_data);

/**
 * @brief Creates a new instance of the SSD1306 OLED Display module.
 * @details This is the factory function for the module, called by the Module Factory.
 *          It allocates memory for the module and its private data, takes ownership
 *          of the provided config object, parses it, and sets up the module's
 *          base function pointers and service API.
 * @param[in] config A pointer to the cJSON configuration object for this instance.
 *                   The function takes ownership of this object.
 * @return A pointer to the newly created module_t structure, or NULL on failure.
 */
module_t *ssd1306_oled_display_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)calloc(1, sizeof(ssd1306_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        if (config)
            cJSON_Delete((cJSON *)config);
        return NULL;
    }

    module->current_config = (cJSON *)config;
    module->private_data = private_data;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (parse_config(config_node, private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Configuration parsing failed");
        ssd1306_deinit(module);
        return NULL;
    }

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->init_level = 70;

    private_data->service_api.enable = api_enable;
    private_data->service_api.disable = api_disable;
    private_data->service_api.clear = api_clear;
    private_data->service_api.write_text = api_write_text;
    private_data->service_api.draw_pixel = api_draw_pixel;
    private_data->service_api.draw_hline = api_draw_hline;
    private_data->service_api.draw_vline = api_draw_vline;
    private_data->service_api.draw_rect = api_draw_rect;
    private_data->service_api.fill_rect = api_fill_rect;
    private_data->service_api.update_screen = api_update_screen;

    private_data->service_handle.api = &private_data->service_api;
    private_data->service_handle.context = private_data;

    module->base.init = ssd1306_init;
    module->base.start = ssd1306_start;
    module->base.deinit = ssd1306_deinit;
    module->base.handle_event = ssd1306_handle_event;

    ESP_LOGI(TAG, "SSD1306 module '%s' created.", module->name);
    return module;
}

/**
 * @internal
 * @brief Sends a single command byte to the SSD1306 controller.
 * @param private_data Pointer to the module's private data.
 * @param cmd The command byte to send.
 * @return ESP_OK on success, or an error code from the I2C driver.
 */
static esp_err_t ssd1306_write_cmd(ssd1306_private_data_t *private_data, uint8_t cmd)
{
    if (!private_data->i2c_bus_service)
        return ESP_ERR_INVALID_STATE;
    uint8_t buffer[] = {0x00, cmd};
    return private_data->i2c_bus_service->api->write(private_data->i2c_bus_service->context, private_data->i2c_address, buffer, sizeof(buffer));
}

/**
 * @internal
 * @brief Sends a buffer of data bytes to the SSD1306 controller's GDDRAM.
 * @param private_data Pointer to the module's private data.
 * @param data Pointer to the data buffer to send.
 * @param size The number of bytes to send from the buffer.
 * @return ESP_OK on success, or an error code from the I2C driver.
 */
static esp_err_t ssd1306_write_data(ssd1306_private_data_t *private_data, const uint8_t *data, size_t size)
{
    if (!private_data->i2c_bus_service)
        return ESP_ERR_INVALID_STATE;

    uint8_t *buffer = malloc(size + 1);
    if (!buffer)
        return ESP_ERR_NO_MEM;
    buffer[0] = 0x40;
    memcpy(buffer + 1, data, size);

    esp_err_t err = private_data->i2c_bus_service->api->write(private_data->i2c_bus_service->context, private_data->i2c_address, buffer, size + 1);

    free(buffer);
    return err;
}

/**
 * @internal
 * @brief Sends the specific initialization command sequence to the SSD1306 controller.
 * @details This sequence configures the display's addressing mode, contrast, orientation,
 *          and enables the charge pump before turning the display on.
 * @param private_data Pointer to the module's private data.
 */
static void screen_hardware_init(ssd1306_private_data_t *private_data)
{
    const uint8_t init_cmds[] = {
        0xAE,
        0x20,
        0x00,
        0xA8,
        (uint8_t)(private_data->height - 1),
        0xD3,
        0x00,
        0x40,
        0xA1,
        0xC8,
        0xDA,
        (private_data->height == 64) ? 0x12 : 0x02,
        0x81,
        0xCF,
        0xD9,
        0xF1,
        0xDB,
        0x40,
        0xA4,
        0xA6,
        0x8D,
        0x14,
        0xAF,
    };
    for (size_t i = 0; i < sizeof(init_cmds); i++)
    {
        ssd1306_write_cmd(private_data, init_cmds[i]);
    }
}

/**
 * @internal
 * @brief Pushes the entire content of the internal screen buffer to the physical display.
 * @details This function iterates through each page of the display's memory and
 *          sends the corresponding segment of the internal buffer via I2C.
 * @param private_data Pointer to the module's private data.
 * @return ESP_OK on success, or an error code on failure.
 */
static esp_err_t update_screen_internal(ssd1306_private_data_t *private_data)
{
    if (!private_data->is_enabled)
        return ESP_ERR_INVALID_STATE;
    for (uint8_t i = 0; i < (private_data->height / 8); i++)
    {
        ssd1306_write_cmd(private_data, 0xB0 + i);
        ssd1306_write_cmd(private_data, 0x00);
        ssd1306_write_cmd(private_data, 0x10);
        ssd1306_write_data(private_data, &private_data->screen_buffer[private_data->width * i], private_data->width);
    }
    return ESP_OK;
}

/**
 * @internal
 * @brief Sets or clears a single pixel in the internal screen buffer.
 * @details This is the fundamental drawing primitive. It calculates the correct byte
 *          and bit position within the buffer for a given (x, y) coordinate and
 *          modifies it.
 * @param private_data Pointer to the module's private data.
 * @param x The horizontal coordinate of the pixel.
 * @param y The vertical coordinate of the pixel.
 * @param color `true` to set the pixel (white), `false` to clear it (black).
 */
static void draw_pixel_internal(ssd1306_private_data_t *private_data, uint8_t x, uint8_t y, bool color)
{
    if (x >= private_data->width || y >= private_data->height)
        return;
    if (color)
    {
        private_data->screen_buffer[x + (y / 8) * private_data->width] |= (1 << (y % 8));
    }
    else
    {
        private_data->screen_buffer[x + (y / 8) * private_data->width] &= ~(1 << (y % 8));
    }
}

/**
 * @internal
 * @brief Draws a text string in the internal buffer using the small 6x8 font.
 * @param private_data Pointer to the module's private data.
 * @param x_pos The starting horizontal position (column) of the text.
 * @param y_pos The starting vertical position (page/line, 0-7) of the text.
 * @param text The null-terminated string to draw.
 */
static void screen_draw_small_text_internal(ssd1306_private_data_t *private_data, uint8_t x_pos, uint8_t y_pos, const char *text)
{
    if (y_pos >= (private_data->height / 8))
        return;

    for (uint8_t i = 0; text[i] != '\0'; i++)
    {
        uint8_t char_code = (uint8_t)text[i];
        if (char_code < FONT_FIRST_CHAR_6X8 || char_code >= (FONT_FIRST_CHAR_6X8 + FONT_CHAR_COUNT_6X8))
        {
            char_code = '?';
        }
        const uint8_t *font_char_data = &ssd1306xled_font6x8[FONT_HEADER_SIZE_6X8 + (char_code - FONT_FIRST_CHAR_6X8) * 6];

        for (uint8_t col = 0; col < 6; col++)
        {
            for (uint8_t bit = 0; bit < 8; bit++)
            {
                if ((font_char_data[col] >> bit) & 1)
                {
                    draw_pixel_internal(private_data, x_pos + i * 6 + col, y_pos * 8 + bit, true);
                }
            }
        }
    }
}

/**
 * @internal
 * @brief Draws a text string in the internal buffer using the large 8x16 font.
 * @details The text is automatically centered horizontally on the screen.
 * @param private_data Pointer to the module's private data.
 * @param line The line number for the large font (0-3 for a 64px high display).
 * @param text The null-terminated string to draw.
 */
static void screen_draw_large_text_internal(ssd1306_private_data_t *private_data, uint8_t line, const char *text)
{
    if (line >= (private_data->height / 16))
        return;

    uint8_t text_len = strlen(text);
    uint16_t text_width_pixels = text_len * 8;
    uint8_t start_col = (private_data->width > text_width_pixels) ? ((private_data->width - text_width_pixels) / 2) : 0;

    for (uint8_t i = 0; i < text_len; i++)
    {
        uint8_t char_code = (uint8_t)text[i];
        if (char_code < FONT_FIRST_CHAR_8X16)
            continue;

        const uint8_t *font_ptr = &ssd1306xled_font8x16[FONT_HEADER_SIZE_8X16 + (char_code - FONT_FIRST_CHAR_8X16) * 16];

        for (uint8_t col = 0; col < 8; col++)
        {
            for (uint8_t bit = 0; bit < 8; bit++)
            {
                if ((font_ptr[col] >> bit) & 1)
                {
                    draw_pixel_internal(private_data, start_col + i * 8 + col, (line * 16) + bit, true);
                }
                if ((font_ptr[col + 8] >> bit) & 1)
                {
                    draw_pixel_internal(private_data, start_col + i * 8 + col, (line * 16) + 8 + bit, true);
                }
            }
        }
    }
}

/**
 * @internal
 * @brief Draws an 8x8 monochrome bitmap in the internal buffer.
 * @param private_data Pointer to the module's private data.
 * @param x_pos The top-left horizontal position (column) of the bitmap.
 * @param y_pos The top-left vertical position (page/line, 0-7) of the bitmap.
 * @param bitmap_data A pointer to the 8-byte array containing the bitmap data.
 */
static void screen_draw_bitmap_internal(ssd1306_private_data_t *private_data, uint8_t x_pos, uint8_t y_pos, const uint8_t *bitmap_data)
{
    if (y_pos >= (private_data->height / 8) || x_pos > (private_data->width - 8) || !bitmap_data)
        return;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if ((bitmap_data[i] >> j) & 1)
            {
                draw_pixel_internal(private_data, x_pos + j, y_pos * 8 + i, true);
            }
        }
    }
}

/**
 * @internal
 * @brief Renders the status header on the top line of the internal screen buffer.
 * @details This function clears the header area, draws the "synapse" logo, and
 *          renders the current WiFi status icon based on the module's state.
 * @param private_data Pointer to the module's private data.
 */
static void draw_header(ssd1306_private_data_t *private_data)
{
    for (uint16_t i = 0; i < private_data->width; i++)
    {
        private_data->screen_buffer[i] = 0x00;
    }

    screen_draw_small_text_internal(private_data, 0, 0, "synapse");

    const uint8_t *wifi_icon_to_draw = wifi_icon_disconnected;
    if (private_data->wifi_connected)
    {
        if (private_data->wifi_rssi >= -55)
            wifi_icon_to_draw = wifi_icon_signal_4;
        else if (private_data->wifi_rssi >= -67)
            wifi_icon_to_draw = wifi_icon_signal_3;
        else if (private_data->wifi_rssi >= -75)
            wifi_icon_to_draw = wifi_icon_signal_2;
        else
            wifi_icon_to_draw = wifi_icon_signal_1;
    }
    screen_draw_bitmap_internal(private_data, private_data->width - 8, 0, wifi_icon_to_draw);
}

/**
 * @internal
 * @brief Initializes the SSD1306 module instance.
 * @details This function gets the required I2C bus service, allocates the screen
 *          buffer, registers the module's own display service, and subscribes
 *          to necessary system events.
 * @param self Pointer to the module instance.
 * @return ESP_OK on success, or an error code on failure.
 */
static esp_err_t ssd1306_init(module_t *self)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing SSD1306 '%s'", self->name);

    private_data->is_enabled = false;
    private_data->wifi_connected = false;
    private_data->wifi_rssi = -100;

    private_data->i2c_bus_service = fmw_service_get(private_data->i2c_bus_instance);
    if (!private_data->i2c_bus_service)
    {
        ESP_LOGE(TAG, "I2C bus service '%s' not found!", private_data->i2c_bus_instance);
        return ESP_ERR_NOT_FOUND;
    }

    size_t buffer_size = (private_data->width * private_data->height) / 8;
    private_data->screen_buffer = (uint8_t *)calloc(buffer_size, sizeof(uint8_t));
    if (!private_data->screen_buffer)
    {
        ESP_LOGE(TAG, "Failed to allocate screen buffer");
        return ESP_ERR_NO_MEM;
    }

    esp_err_t err = fmw_service_register(self->name, FMW_SERVICE_TYPE_DISPLAY_API, &private_data->service_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register display service: %s", esp_err_to_name(err));
        return err;
    }

    fmw_event_bus_subscribe(FMW_EVENT_WIFI_STATUS_READY, self);
    fmw_event_bus_subscribe(EVT_INTERNAL_UPDATE_WIFI, self);
    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

/**
 * @internal
 * @brief Starts the SSD1306 module's operation.
 * @details This function schedules the periodic WiFi status update timer,
 *          initializes the display hardware, shows the splash screen, and then
 *          transitions to the main operational mode.
 * @param self Pointer to the module instance.
 * @return ESP_OK on success, or an error code on failure.
 */
static esp_err_t ssd1306_start(module_t *self)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;

    timer_api_t *timer_api = fmw_service_get("main_timer_service");
    if (timer_api)
    {
        timer_api->schedule_event(EVT_INTERNAL_UPDATE_WIFI, WIFI_STATUS_UPDATE_INTERVAL_MS, true);
        ESP_LOGI(TAG, "Scheduled periodic WiFi status update.");
    }
    else
    {
        ESP_LOGW(TAG, "System Timer service not found, WiFi status will not be updated periodically.");
    }

    screen_hardware_init(private_data);
    private_data->is_enabled = true;

    if (strlen(private_data->init_text) > 0)
    {
        api_clear(private_data);
        screen_draw_large_text_internal(private_data, 1, private_data->init_text);
        api_update_screen(private_data);
    }
    ESP_LOGI(TAG, "Display for '%s' started, showing splash screen.", self->name);

    vTaskDelay(pdMS_TO_TICKS(3000));

    api_clear(private_data);
    draw_header(private_data);
    api_update_screen(private_data);
    ESP_LOGI(TAG, "Switching to main display mode.");

    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

/**
 * @internal
 * @brief Deinitializes the SSD1306 module instance.
 * @details This function unregisters the service, unsubscribes from all events,
 *          and frees all allocated resources, including the screen buffer and
 *          the module structures themselves.
 * @param self Pointer to the module instance to deinitialize.
 */
static void ssd1306_deinit(module_t *self)
{
    if (!self)
        return;
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;
    if (private_data)
    {
        ESP_LOGI(TAG, "De-initializing SSD1306 '%s'", self->name);
        fmw_service_unregister(self->name);
        fmw_event_bus_unsubscribe(FMW_EVENT_WIFI_STATUS_READY, self);
        fmw_event_bus_unsubscribe(EVT_INTERNAL_UPDATE_WIFI, self);
        if (private_data->screen_buffer)
        {
            free(private_data->screen_buffer);
        }
        free(private_data);
    }
    if (self->current_config)
        cJSON_Delete(self->current_config);
    free(self);
}

/**
 * @internal
 * @brief Handles events received from the Event Bus.
 * @details This function processes events to update the display content. It
 *          triggers periodic WiFi status requests and updates the header icon
 *          when a status report is received.
 * @param self Pointer to the module instance.
 * @param event_name The name of the received event.
 * @param event_data A pointer to the event data wrapper.
 */
static void ssd1306_handle_event(module_t *self, const char *event_name, void *event_data)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;
    if (!private_data->is_enabled)
    {
        if (event_data)
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    if (strcmp(event_name, EVT_INTERNAL_UPDATE_WIFI) == 0)
    {
        ESP_LOGD(TAG, "Internal timer triggered. Requesting WiFi status update.");
        fmw_command_payload_t *payload = calloc(1, sizeof(fmw_command_payload_t));
        if (payload)
        {
            snprintf(payload->command_string, sizeof(payload->command_string), "wifi status --silent");
            snprintf(payload->source, sizeof(payload->source), self->name);

            event_data_wrapper_t *wrapper;
            if (fmw_event_data_wrap(payload, fmw_payload_common_free, &wrapper) == ESP_OK)
            {
                fmw_event_bus_post(FMW_EVENT_EXECUTE_COMMAND_STRING, wrapper);
                fmw_event_data_release(wrapper);
            }
            else
            {
                free(payload);
            }
        }
    }
    else if (strcmp(event_name, FMW_EVENT_WIFI_STATUS_READY) == 0)
    {
        event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;
        if (wrapper && wrapper->payload)
        {
            fmw_telemetry_payload_t *payload = (fmw_telemetry_payload_t *)wrapper->payload;
            if (payload->json_data)
            {
                cJSON *root = cJSON_Parse(payload->json_data);
                if (root)
                {
                    const cJSON *conn_status = cJSON_GetObjectItem(root, "connection_status");
                    const cJSON *rssi_json = cJSON_GetObjectItem(root, "rssi");

                    bool new_status = (cJSON_IsString(conn_status) && strcmp(conn_status->valuestring, "Connected") == 0);
                    int32_t new_rssi = cJSON_IsNumber(rssi_json) ? rssi_json->valueint : -100;

                    if (new_status != private_data->wifi_connected || new_rssi != private_data->wifi_rssi)
                    {
                        ESP_LOGI(TAG, "WiFi status changed. Connected: %d, RSSI: %d. Updating header.", new_status, (int)new_rssi);
                        private_data->wifi_connected = new_status;
                        private_data->wifi_rssi = new_rssi;
                        draw_header(private_data);
                        api_update_screen(private_data);
                    }
                    cJSON_Delete(root);
                }
            }
        }
    }

    if (event_data)
        fmw_event_data_release((event_data_wrapper_t *)event_data);
}

/**
 * @internal
 * @brief Service API implementation for enabling the display.
 * @details Turns the display panel on and redraws the current buffer content.
 * @param context A pointer to the module's private data.
 * @return ESP_OK on success.
 */
static esp_err_t api_enable(void *context)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    if (private_data->is_enabled)
        return ESP_OK;
    screen_hardware_init(private_data);
    private_data->is_enabled = true;
    return update_screen_internal(private_data);
}

/**
 * @internal
 * @brief Service API implementation for disabling the display.
 * @details Turns the display panel off to save power. The buffer content is preserved.
 * @param context A pointer to the module's private data.
 * @return ESP_OK on success.
 */
static esp_err_t api_disable(void *context)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    if (!private_data->is_enabled)
        return ESP_OK;
    private_data->is_enabled = false;
    return ssd1306_write_cmd(private_data, 0xAE);
}

/**
 * @internal
 * @brief Service API implementation for clearing the internal screen buffer.
 * @param context A pointer to the module's private data.
 * @return ESP_OK.
 */
static esp_err_t api_clear(void *context)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    memset(private_data->screen_buffer, 0, (private_data->width * private_data->height) / 8);
    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for writing formatted text to the buffer.
 * @param context A pointer to the module's private data.
 * @param x The starting horizontal position (column).
 * @param y The starting vertical position (page/line, 0-7).
 * @param format A printf-style format string.
 * @param ... Variable arguments for the format string.
 * @return ESP_OK on success, or an error code on failure.
 */
static esp_err_t api_write_text(void *context, uint8_t x, uint8_t y, const char *format, ...)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    if (!private_data || !format)
    {
        return ESP_ERR_INVALID_ARG;
    }

    char buffer[LINE_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    screen_draw_small_text_internal(private_data, x, y, buffer);
    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for drawing a single pixel.
 * @param context A pointer to the module's private data.
 * @param x The horizontal position.
 * @param y The vertical position.
 * @param color `true` for white, `false` for black.
 * @return ESP_OK.
 */
static esp_err_t api_draw_pixel(void *context, uint8_t x, uint8_t y, bool color)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    draw_pixel_internal(private_data, x, y, color);
    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for drawing a horizontal line.
 * @param context A pointer to the module's private data.
 * @param x The starting horizontal position.
 * @param y The vertical position.
 * @param width The width of the line.
 * @return ESP_OK.
 */
static esp_err_t api_draw_hline(void *context, uint8_t x, uint8_t y, uint8_t width)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    for (uint8_t i = 0; i < width; i++)
    {
        draw_pixel_internal(private_data, x + i, y, true);
    }
    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for drawing a vertical line.
 * @param context A pointer to the module's private data.
 * @param x The horizontal position.
 * @param y The starting vertical position.
 * @param height The height of the line.
 * @return ESP_OK.
 */
static esp_err_t api_draw_vline(void *context, uint8_t x, uint8_t y, uint8_t height)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    for (uint8_t i = 0; i < height; i++)
    {
        draw_pixel_internal(private_data, x, y + i, true);
    }
    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for drawing a rectangle outline.
 * @param context A pointer to the module's private data.
 * @param x Top-left corner X coordinate.
 * @param y Top-left corner Y coordinate.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @return ESP_OK.
 */
static esp_err_t api_draw_rect(void *context, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    api_draw_hline(private_data, x, y, width);
    api_draw_hline(private_data, x, y + height - 1, width);
    api_draw_vline(private_data, x, y, height);
    api_draw_vline(private_data, x + width - 1, y, height);
    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for drawing a filled rectangle.
 * @param context A pointer to the module's private data.
 * @param x Top-left corner X coordinate.
 * @param y Top-left corner Y coordinate.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @return ESP_OK.
 */
static esp_err_t api_fill_rect(void *context, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    for (uint8_t i = 0; i < height; i++)
    {
        api_draw_hline(private_data, x, y + i, width);
    }
    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for pushing the buffer to the display.
 * @param context A pointer to the module's private data.
 * @return ESP_OK on success.
 */
static esp_err_t api_update_screen(void *context)
{
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    return update_screen_internal(private_data);
}

/**
 * @internal
 * @brief Parses the configuration from a cJSON object with robust validation.
 * @param config_node Pointer to the "config" cJSON object.
 * @param private_data Pointer to the private data structure to populate.
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG if config is invalid.
 */
static esp_err_t parse_config(const cJSON *config_node, ssd1306_private_data_t *private_data)
{
    if (!config_node)
    {
        ESP_LOGE(TAG, "Module config object is missing.");
        return ESP_ERR_INVALID_ARG;
    }

    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    if (!cJSON_IsString(name_node) || (name_node->valuestring == NULL))
    {
        ESP_LOGE(TAG, "Config error: 'instance_name' is missing or not a string.");
        return ESP_ERR_INVALID_ARG;
    }
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", name_node->valuestring);

    const cJSON *bus_node = cJSON_GetObjectItem(config_node, "i2c_bus_instance");
    if (!cJSON_IsString(bus_node) || (bus_node->valuestring == NULL))
    {
        ESP_LOGE(TAG, "Config error: 'i2c_bus_instance' is missing or not a string.");
        return ESP_ERR_INVALID_ARG;
    }
    snprintf(private_data->i2c_bus_instance, sizeof(private_data->i2c_bus_instance), "%s", bus_node->valuestring);

    const cJSON *addr_node = cJSON_GetObjectItem(config_node, "i2c_addr");
    if (!cJSON_IsString(addr_node) || (addr_node->valuestring == NULL))
    {
        ESP_LOGE(TAG, "Config error: 'i2c_addr' is missing or not a string.");
        return ESP_ERR_INVALID_ARG;
    }
    private_data->i2c_address = (uint8_t)strtol(addr_node->valuestring, NULL, 16);

    const cJSON *width_node = cJSON_GetObjectItem(config_node, "width");
    if (cJSON_IsNumber(width_node))
    {
        private_data->width = width_node->valueint;
    }
    else
    {
        ESP_LOGW(TAG, "'width' not specified, using default: 128");
        private_data->width = 128;
    }

    const cJSON *height_node = cJSON_GetObjectItem(config_node, "height");
    if (cJSON_IsNumber(height_node))
    {
        private_data->height = height_node->valueint;
    }
    else
    {
        ESP_LOGW(TAG, "'height' not specified, using default: 64");
        private_data->height = 64;
    }

    const cJSON *init_text_node = cJSON_GetObjectItem(config_node, "init_text");
    if (cJSON_IsString(init_text_node) && (init_text_node->valuestring != NULL))
    {
        snprintf(private_data->init_text, sizeof(private_data->init_text), "%s", init_text_node->valuestring);
    }
    else
    {
        ESP_LOGW(TAG, "'init_text' not specified, using default: 'Synapse'");
        snprintf(private_data->init_text, sizeof(private_data->init_text), "Synapse");
    }

    return ESP_OK;
}