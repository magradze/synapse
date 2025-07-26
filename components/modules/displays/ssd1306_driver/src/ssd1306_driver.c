/**
 * @file ssd1306_driver.c
 * @brief Low-level driver implementation for SSD1306 OLED displays.
 * @author Synapse Framework Team
 * @version 1.2.0
 * @date 2025-08-01
 */

#include "ssd1306_driver.h"
#include "logging.h"
#include "service_locator.h"
#include "display_driver_interface.h"
#include "i2c_bus_interface.h"
#include "ssd1306_fonts.h"
#include "freertos/task.h"
#include <string.h>
#include <stdio.h>

DEFINE_COMPONENT_TAG("SSD1306_DRIVER");

#define SSD1306_SET_CONTRAST        0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME 0xA4
#define SSD1306_DISPLAY_ALL_ON      0xA5
#define SSD1306_NORMAL_DISPLAY      0xA6
#define SSD1306_INVERT_DISPLAY      0xA7
#define SSD1306_DISPLAY_OFF         0xAE
#define SSD1306_DISPLAY_ON          0xAF
#define SSD1306_SET_DISPLAY_OFFSET  0xD3
#define SSD1306_SET_COM_PINS        0xDA
#define SSD1306_SET_VCOM_DETECT     0xDB
#define SSD1306_SET_DISPLAY_CLOCK_DIV 0xD5
#define SSD1306_SET_PRECHARGE       0xD9
#define SSD1306_SET_MULTIPLEX       0xA8
#define SSD1306_SET_LOW_COLUMN      0x00
#define SSD1306_SET_HIGH_COLUMN     0x10
#define SSD1306_SET_START_LINE      0x40
#define SSD1306_MEMORY_MODE         0x20
#define SSD1306_COLUMN_ADDR         0x21
#define SSD1306_PAGE_ADDR           0x22
#define SSD1306_COM_SCAN_INC        0xC0
#define SSD1306_COM_SCAN_DEC        0xC8
#define SSD1306_SEG_REMAP           0xA0
#define SSD1306_CHARGE_PUMP         0x8D

typedef struct {
    module_t *module;
    display_driver_handle_t service_handle;
    i2c_bus_handle_t *i2c_bus;
    uint8_t i2c_addr;
    uint16_t width;
    uint16_t height;
    int8_t reset_pin;
    uint8_t *screen_buffer;
    size_t buffer_size;
} ssd1306_private_data_t;

// --- Forward Declarations ---
static esp_err_t ssd1306_driver_init(module_t *self);
static void ssd1306_driver_deinit(module_t *self);
static esp_err_t api_get_info(void* context, display_info_t* info_out);
static esp_err_t api_draw_pixel(void* context, int16_t x, int16_t y, uint32_t color);
static esp_err_t api_fill_rect(void* context, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
static esp_err_t api_draw_formatted_text(void* context, int16_t x, int16_t y, uint32_t color, const char* format, ...);
static esp_err_t api_update_screen(void* context);
static esp_err_t api_clear(void* context);
static esp_err_t api_set_power(void* context, bool on);
static esp_err_t api_set_contrast(void* context, uint8_t contrast);
static esp_err_t api_unsupported_rotation(void* context, display_rotation_t rotation) { return ESP_ERR_NOT_SUPPORTED; }
static esp_err_t api_get_small_font_metrics(void* context, font_metrics_t* metrics_out);

static const display_driver_api_t ssd1306_api_table = {
    .get_info = api_get_info,
    .draw_pixel = api_draw_pixel,
    .fill_rect = api_fill_rect,
    .draw_formatted_text = api_draw_formatted_text,
    .update_screen = api_update_screen,
    .clear = api_clear,
    .set_power = api_set_power,
    .set_rotation = api_unsupported_rotation,
    .set_contrast = api_set_contrast,
    .get_small_font_metrics = api_get_small_font_metrics,
};

/**
 * @internal
 * @brief Sends a single command byte to the SSD1306 controller.
 * @param[in] private_data Pointer to the module's private data.
 * @param[in] cmd The command byte to send.
 * @return ESP_OK on success, or an error code from the I2C driver.
 */
static esp_err_t send_cmd(ssd1306_private_data_t *private_data, uint8_t cmd) {
    uint8_t buffer[] = {0x00, cmd};
    return private_data->i2c_bus->api->write(private_data->i2c_bus->context, private_data->i2c_addr, buffer, sizeof(buffer));
}

/**
 * @internal
 * @brief Sends a buffer of data bytes to the SSD1306 controller's GDDRAM.
 * @param[in] private_data Pointer to the module's private data.
 * @param[in] data Pointer to the data buffer to send.
 * @param[in] size The number of bytes to send from the buffer.
 * @return ESP_OK on success, or an error code from the I2C driver.
 */
static esp_err_t send_data(ssd1306_private_data_t *private_data, const uint8_t *data, size_t size) {
    uint8_t *buffer = malloc(size + 1);
    if (!buffer) return ESP_ERR_NO_MEM;
    buffer[0] = 0x40;
    memcpy(buffer + 1, data, size);
    esp_err_t ret = private_data->i2c_bus->api->write(private_data->i2c_bus->context, private_data->i2c_addr, buffer, size + 1);
    free(buffer);
    return ret;
}

/**
 * @brief Creates a new instance of the SSD1306 driver module.
 * @details This function is the entry point for the Module Factory. It allocates
 *          memory for the module, parses its configuration, and sets up its
 *          base lifecycle function pointers.
 * @param[in] config A cJSON object containing the configuration for this instance.
 *                   The function takes ownership of this object.
 * @return A pointer to the newly created module_t instance, or NULL on failure.
 */
module_t* ssd1306_driver_create(const cJSON *config) {
    module_t *module = calloc(1, sizeof(module_t));
    ssd1306_private_data_t *private_data = calloc(1, sizeof(ssd1306_private_data_t));
    if (!module || !private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        if (config) cJSON_Delete((cJSON*)config);
        return NULL;
    }

    module->private_data = private_data;
    module->current_config = (cJSON*)config;
    private_data->module = module;

    module->init_level = 15;

    // Parse config
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    snprintf(module->name, sizeof(module->name), "%s", cJSON_GetObjectItem(config_node, "instance_name")->valuestring);
    private_data->i2c_addr = (uint8_t)strtol(cJSON_GetObjectItem(config_node, "i2c_addr")->valuestring, NULL, 16);
    private_data->width = cJSON_GetObjectItem(config_node, "width")->valueint;
    private_data->height = cJSON_GetObjectItem(config_node, "height")->valueint;
    private_data->reset_pin = cJSON_GetObjectItem(config_node, "reset_pin")->valueint;

    module->base.init = ssd1306_driver_init;
    module->base.deinit = ssd1306_driver_deinit;

    return module;
}

/**
 * @internal
 * @brief Deinitializes the SSD1306 driver module instance.
 * @details This function is responsible for cleaning up all resources allocated
 *          by the module. It unregisters the service from the Service Locator
 *          and frees all allocated memory, including the screen buffer, private
 *          data, and the configuration object.
 * @param[in] self A pointer to the module instance to deinitialize.
 */
static void ssd1306_driver_deinit(module_t *self) {
    if (!self) return;
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;
    
    fmw_service_unregister(self->name);
    
    if (private_data->screen_buffer) {
        free(private_data->screen_buffer);
    }
    if (self->current_config) {
        cJSON_Delete(self->current_config);
    }
    free(private_data);
    free(self);
    ESP_LOGI(TAG, "SSD1306 driver deinitialized.");
}

/**
 * @internal
 * @brief Initializes the SSD1306 driver module instance.
 * @details This function is called by the System Manager. It acquires the I2C bus
 *          service, allocates the screen buffer, performs the hardware initialization
 *          sequence by sending commands to the display, and finally registers its
 *          own service API with the Service Locator.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK on success, or an error code on failure.
 */
static esp_err_t ssd1306_driver_init(module_t *self) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;
    const cJSON *config_node = cJSON_GetObjectItem(self->current_config, "config");
    const char* i2c_bus_service_name = cJSON_GetObjectItem(config_node, "i2c_bus_service")->valuestring;

    private_data->i2c_bus = fmw_service_get(i2c_bus_service_name);
    if (!private_data->i2c_bus) {
        ESP_LOGE(TAG, "I2C bus service '%s' not found!", i2c_bus_service_name);
        return ESP_ERR_NOT_FOUND;
    }

    private_data->buffer_size = (private_data->width * private_data->height) / 8;
    private_data->screen_buffer = malloc(private_data->buffer_size);
    if (!private_data->screen_buffer) {
        ESP_LOGE(TAG, "Failed to allocate screen buffer");
        return ESP_ERR_NO_MEM;
    }
    memset(private_data->screen_buffer, 0, private_data->buffer_size);

    send_cmd(private_data, SSD1306_DISPLAY_OFF);
    send_cmd(private_data, SSD1306_SET_DISPLAY_CLOCK_DIV);
    send_cmd(private_data, 0x80);
    send_cmd(private_data, SSD1306_SET_MULTIPLEX);
    send_cmd(private_data, private_data->height - 1);
    send_cmd(private_data, SSD1306_SET_DISPLAY_OFFSET);
    send_cmd(private_data, 0x0);
    send_cmd(private_data, SSD1306_SET_START_LINE | 0x0);
    send_cmd(private_data, SSD1306_CHARGE_PUMP);
    send_cmd(private_data, 0x14);
    send_cmd(private_data, SSD1306_MEMORY_MODE);
    send_cmd(private_data, 0x00);
    send_cmd(private_data, SSD1306_SEG_REMAP | 0x1);
    send_cmd(private_data, SSD1306_COM_SCAN_DEC);
    send_cmd(private_data, SSD1306_SET_COM_PINS);
    send_cmd(private_data, 0x12);
    send_cmd(private_data, SSD1306_SET_CONTRAST);
    send_cmd(private_data, 0xCF);
    send_cmd(private_data, SSD1306_SET_PRECHARGE);
    send_cmd(private_data, 0xF1);
    send_cmd(private_data, SSD1306_SET_VCOM_DETECT);
    send_cmd(private_data, 0x40);
    send_cmd(private_data, SSD1306_DISPLAY_ALL_ON_RESUME);
    send_cmd(private_data, SSD1306_NORMAL_DISPLAY);
    send_cmd(private_data, SSD1306_DISPLAY_ON);

    private_data->service_handle.api = &ssd1306_api_table;
    private_data->service_handle.context = private_data;
    fmw_service_register(self->name, FMW_SERVICE_TYPE_DISPLAY_API, &private_data->service_handle);

    ESP_LOGI(TAG, "SSD1306 driver '%s' initialized on I2C addr 0x%02X", self->name, private_data->i2c_addr);
    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for getting display information.
 * @param[in] context A pointer to the module's private data.
 * @param[out] info_out A pointer to a structure to be filled with display metrics.
 * @return Always returns ESP_OK.
 */
static esp_err_t api_get_info(void* context, display_info_t* info_out) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    info_out->width = private_data->width;
    info_out->height = private_data->height;
    info_out->is_color = false;
    info_out->bpp = 1;
    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for drawing a single pixel.
 * @details This is the fundamental drawing primitive. It calculates the correct byte
 *          and bit position within the internal screen buffer for a given (x, y)
 *          coordinate and modifies it.
 * @param[in] context A pointer to the module's private data.
 * @param[in] x The horizontal coordinate of the pixel.
 * @param[in] y The vertical coordinate of the pixel.
 * @param[in] color The color of the pixel (0 for black, non-zero for white).
 * @return ESP_OK on success, or ESP_ERR_INVALID_ARG if coordinates are out of bounds.
 */
static esp_err_t api_draw_pixel(void* context, int16_t x, int16_t y, uint32_t color) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    if (x < 0 || x >= private_data->width || y < 0 || y >= private_data->height) {
        return ESP_ERR_INVALID_ARG;
    }
    if (color) {
        private_data->screen_buffer[x + (y / 8) * private_data->width] |= (1 << (y & 7));
    } else {
        private_data->screen_buffer[x + (y / 8) * private_data->width] &= ~(1 << (y & 7));
    }
    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for drawing a filled rectangle.
 * @details This function iterates through the specified rectangular area and calls
 *          `api_draw_pixel` for each pixel to fill the region.
 * @param[in] context A pointer to the module's private data.
 * @param[in] x The top-left X coordinate of the rectangle.
 * @param[in] y The top-left Y coordinate of the rectangle.
 * @param[in] w The width of the rectangle.
 * @param[in] h The height of the rectangle.
 * @param[in] color The color to fill the rectangle with.
 * @return Always returns ESP_OK.
 */
static esp_err_t api_fill_rect(void* context, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color) {
    for (int16_t i = x; i < x + w; i++) {
        for (int16_t j = y; j < y + h; j++) {
            api_draw_pixel(context, i, j, color);
        }
    }
    return ESP_OK;
}

/**
 * @internal
 * @brief Draws a single character to the screen buffer using the built-in 6x8 font.
 * @details This helper function looks up the character's bitmap data from the
 *          `ssd1306xled_font6x8` array and renders it pixel by pixel.
 * @param[in] context A pointer to the module's private data.
 * @param[in] x The top-left X coordinate for the character.
 * @param[in] y The top-left Y coordinate for the character.
 * @param[in] c The character to draw.
 * @param[in] color The color of the character.
 */
static void draw_char_6x8(void* context, int16_t x, int16_t y, char c, uint32_t color) {
    if (c < FONT_FIRST_CHAR_6X8 || c >= (FONT_FIRST_CHAR_6X8 + FONT_CHAR_COUNT_6X8)) {
        c = '?'; // Fallback for unsupported characters
    }

    // Font data starts after a 4-byte header
    const uint8_t *font_char_data = &ssd1306xled_font6x8[FONT_HEADER_SIZE_6X8 + (c - FONT_FIRST_CHAR_6X8) * 6];

    for (uint8_t i = 0; i < 6; i++) { // 6 columns
        uint8_t line = font_char_data[i];
        for (uint8_t j = 0; j < 8; j++) { // 8 rows
            if ((line >> j) & 0x01) {
                api_draw_pixel(context, x + i, y + j, color);
            }
        }
    }
}

/**
 * @internal
 * @brief Service API implementation for drawing formatted text.
 * @details This function takes a printf-style format string and arguments,
 *          formats them into a temporary buffer, and then renders the resulting
 *          string using the `draw_char_6x8` helper function.
 * @param[in] context A pointer to the module's private data.
 * @param[in] x The starting X coordinate of the text.
 * @param[in] y The starting Y coordinate of the text.
 * @param[in] color The color of the text.
 * @param[in] format A printf-style format string.
 * @param[in] ... Variable arguments for the format string.
 * @return Always returns ESP_OK.
 */
static esp_err_t api_draw_formatted_text(void* context, int16_t x, int16_t y, uint32_t color, const char* format, ...) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    char buffer[128]; 

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    int16_t current_x = x;
    for (char *s = buffer; *s; s++) {
        if (current_x + 6 > private_data->width) {
            break; 
        }
        draw_char_6x8(context, current_x, y, *s, color);
        current_x += 6;
    }

    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for pushing the buffer to the physical display.
 * @details This function sends the necessary commands to the SSD1306 to set the
 *          memory address pointers and then transfers the entire content of the
 *          internal screen buffer via I2C.
 * @param[in] context A pointer to the module's private data.
 * @return ESP_OK on success, or an error code from the I2C driver.
 */
static esp_err_t api_update_screen(void* context) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    send_cmd(private_data, SSD1306_COLUMN_ADDR);
    send_cmd(private_data, 0);
    send_cmd(private_data, private_data->width - 1);
    send_cmd(private_data, SSD1306_PAGE_ADDR);
    send_cmd(private_data, 0);
    send_cmd(private_data, (private_data->height / 8) - 1);
    return send_data(private_data, private_data->screen_buffer, private_data->buffer_size);
}

/**
 * @internal
 * @brief Service API implementation for clearing the internal screen buffer.
 * @details This function simply fills the entire `screen_buffer` with zeros.
 *          `api_update_screen` must be called afterwards to see the change.
 * @param[in] context A pointer to the module's private data.
 * @return Always returns ESP_OK.
 */
static esp_err_t api_clear(void* context) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    memset(private_data->screen_buffer, 0, private_data->buffer_size);
    return ESP_OK;
}

/**
 * @internal
 * @brief Service API implementation for turning the display panel on or off.
 * @param[in] context A pointer to the module's private data.
 * @param[in] on `true` to turn the display on, `false` to turn it off.
 * @return ESP_OK on success.
 */
static esp_err_t api_set_power(void* context, bool on) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    return send_cmd(private_data, on ? SSD1306_DISPLAY_ON : SSD1306_DISPLAY_OFF);
}

/**
 * @internal
 * @brief Service API implementation for setting the display's contrast.
 * @param[in] context A pointer to the module's private data.
 * @param[in] contrast The contrast value (0-255).
 * @return ESP_OK on success.
 */
static esp_err_t api_set_contrast(void* context, uint8_t contrast) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    send_cmd(private_data, SSD1306_SET_CONTRAST);
    return send_cmd(private_data, contrast);
}

/**
 * @internal
 * @brief Service API implementation for getting the metrics of the built-in font.
 * @param[in] context A pointer to the module's private data (unused).
 * @param[out] metrics_out A pointer to a structure to be filled with the font's dimensions.
 * @return ESP_OK on success, or ESP_ERR_INVALID_ARG if `metrics_out` is NULL.
 */
static esp_err_t api_get_small_font_metrics(void* context, font_metrics_t* metrics_out) {
    if (!metrics_out) return ESP_ERR_INVALID_ARG;
    metrics_out->width = SSD1306_SMALL_FONT_WIDTH;
    metrics_out->height = SSD1306_SMALL_FONT_HEIGHT;
    return ESP_OK;
}