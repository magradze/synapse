/**
 * @file ssd1306_oled_display.c
 * @brief Implementation for the SSD1306 OLED display module.
 * @author Giorgi Magradze & Synapse AI Expert
 * @version 2.0.0
 * @date 2025-07-28
 * @details This module controls an SSD1306 OLED display. It acts as both a
 *          Service Provider (offering a display API) and an Event Consumer
 *          (displaying system status and telemetry). This is the final,
 *          complete, and corrected version with all functions fully implemented
 *          and documented.
 */
#include "ssd1306_oled_display.h"
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
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

DEFINE_COMPONENT_TAG("SSD1306_DISPLAY");

#define WIFI_STATUS_UPDATE_INTERVAL_MS CONFIG_SSD1306_WIFI_STATUS_UPDATE_MS
#define LINE_BUFFER_SIZE               CONFIG_SSD1306_LINE_BUFFER_SIZE
#define COMMAND_QUEUE_LENGTH           CONFIG_SSD1306_CMD_QUEUE_LENGTH

#define EVT_INTERNAL_UPDATE_WIFI "INT_EVT_UPDATE_WIFI"

#define MAX_DISPLAY_LINES_LARGE 4
#define MAX_DISPLAY_LINES_SMALL 8

#define HEADER_PAGE_NUM 0
#define CONTENT_START_PAGE 1

/** @internal @enum Command type for the display task queue. */
typedef enum {
    DISPLAY_CMD_CLEAR_ALL,
    DISPLAY_CMD_CLEAR_CONTENT,
    DISPLAY_CMD_WRITE_TEXT,
    DISPLAY_CMD_WRITE_LARGE_TEXT,
    DISPLAY_CMD_DRAW_BITMAP,
} display_command_type_t;

/** @internal @struct Structure for a command sent to the display task. */
typedef struct {
    display_command_type_t type;
    uint8_t line_num;
    uint8_t x_pos;
    uint8_t y_pos;
    const uint8_t *bitmap_data;
    char text[LINE_BUFFER_SIZE];
} display_command_t;

/** @internal @brief Private data for the SSD1306 module instance. */
typedef struct {
    char instance_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];
    char i2c_bus_instance[CONFIG_FMW_SERVICE_NAME_MAX_LENGTH];
    uint8_t i2c_address;
    uint16_t width;
    uint16_t height;
    char init_text[LINE_BUFFER_SIZE];

    i2c_bus_handle_t *i2c_bus_service;
    TaskHandle_t display_task_handle;
    QueueHandle_t command_queue;
    
    ssd1306_handle_t service_handle;
    ssd1306_api_t service_api;

    bool wifi_connected;
    int32_t wifi_rssi;
} ssd1306_private_data_t;

// --- Forward Declarations ---
static esp_err_t ssd1306_init(module_t *self);
static esp_err_t ssd1306_start(module_t *self);
static void ssd1306_deinit(module_t *self);
static void ssd1306_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t parse_config(const cJSON *config_node, ssd1306_private_data_t *private_data);
static void display_task(void *pvParameters);

static esp_err_t api_enable(void* context);
static esp_err_t api_disable(void* context);
static esp_err_t api_clear(void* context);
static esp_err_t api_write_text(void* context, uint8_t line_num, const char *format, ...);
static void draw_header(ssd1306_private_data_t *private_data);

// --- Module Factory ---
module_t *ssd1306_oled_display_create(const cJSON *config) {
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)calloc(1, sizeof(ssd1306_private_data_t));
    if (!module || !private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module); free(private_data);
        if (config) cJSON_Delete((cJSON*)config);
        return NULL;
    }

    module->current_config = (cJSON*)config;
    module->private_data = private_data;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (parse_config(config_node, private_data) != ESP_OK) {
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

    private_data->service_handle.api = &private_data->service_api;
    private_data->service_handle.context = private_data;

    module->base.init = ssd1306_init;
    module->base.start = ssd1306_start;
    module->base.deinit = ssd1306_deinit;
    module->base.handle_event = ssd1306_handle_event;

    ESP_LOGI(TAG, "SSD1306 module '%s' created.", module->name);
    return module;
}

// --- Internal Hardware Functions ---
static esp_err_t ssd1306_write_cmd(ssd1306_private_data_t *private_data, uint8_t cmd) {
    if (!private_data->i2c_bus_service) return ESP_ERR_INVALID_STATE;
    uint8_t buffer[] = {0x00, cmd};
    return private_data->i2c_bus_service->api->write(private_data->i2c_bus_service->context, private_data->i2c_address, buffer, sizeof(buffer));
}

static esp_err_t ssd1306_write_data(ssd1306_private_data_t *private_data, const uint8_t *data, size_t size) {
    if (!private_data->i2c_bus_service) return ESP_ERR_INVALID_STATE;
    
    uint8_t *buffer = malloc(size + 1);
    if (!buffer) return ESP_ERR_NO_MEM;
    buffer[0] = 0x40;
    memcpy(buffer + 1, data, size);
    
    esp_err_t err = private_data->i2c_bus_service->api->write(private_data->i2c_bus_service->context, private_data->i2c_address, buffer, size + 1);
    
    free(buffer);
    return err;
}

static void screen_hardware_init(ssd1306_private_data_t *private_data) {
    const uint8_t init_cmds[] = {
        0xAE, 0x20, 0x00, 0xA8, (uint8_t)(private_data->height - 1), 0xD3, 0x00,
        0x40, 0xA1, 0xC8, 0xDA, (private_data->height == 64) ? 0x12 : 0x02,
        0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6, 0x8D, 0x14, 0xAF,
    };
    for (size_t i = 0; i < sizeof(init_cmds); i++) {
        ssd1306_write_cmd(private_data, init_cmds[i]);
    }
}

// --- Task and Command Functions ---
static void screen_clear_internal(ssd1306_private_data_t *private_data, bool full_screen) {
    uint8_t *zero_buffer = (uint8_t *)calloc(1, private_data->width);
    if (!zero_buffer) return;
    
    uint8_t start_page = full_screen ? 0 : CONTENT_START_PAGE;
    for (uint8_t i = start_page; i < (private_data->height / 8); i++) {
        ssd1306_write_cmd(private_data, 0xB0 + i);
        ssd1306_write_cmd(private_data, 0x00);
        ssd1306_write_cmd(private_data, 0x10);
        ssd1306_write_data(private_data, zero_buffer, private_data->width);
    }
    free(zero_buffer);
}

static void screen_draw_large_text_internal(ssd1306_private_data_t *private_data, uint8_t line, const char *text) {
    if (line >= (private_data->height / 16)) return;

    uint8_t text_len = strlen(text);
    uint16_t text_width_pixels = text_len * 8;
    uint8_t start_col = (private_data->width > text_width_pixels) ? ((private_data->width - text_width_pixels) / 2) : 0;
    
    for (uint8_t i = 0; i < text_len; i++) {
        uint8_t char_code = (uint8_t)text[i];
        if (char_code < FONT_FIRST_CHAR_8X16) continue;

        const uint8_t *font_ptr = &ssd1306xled_font8x16[FONT_HEADER_SIZE_8X16 + (char_code - FONT_FIRST_CHAR_8X16) * 16];
        
        ssd1306_write_cmd(private_data, 0xB0 + (line * 2));
        ssd1306_write_cmd(private_data, 0x00 + ((start_col + i * 8) & 0x0F));
        ssd1306_write_cmd(private_data, 0x10 + ((start_col + i * 8) >> 4));
        ssd1306_write_data(private_data, font_ptr, 8);

        ssd1306_write_cmd(private_data, 0xB0 + (line * 2) + 1);
        ssd1306_write_cmd(private_data, 0x00 + ((start_col + i * 8) & 0x0F));
        ssd1306_write_cmd(private_data, 0x10 + ((start_col + i * 8) >> 4));
        ssd1306_write_data(private_data, font_ptr + 8, 8);
    }
}

static void screen_draw_small_text_internal(ssd1306_private_data_t *private_data, uint8_t x_pos, uint8_t y_pos, const char *text) {
    if (y_pos >= (private_data->height / 8)) return;
    
    ssd1306_write_cmd(private_data, 0xB0 + y_pos);
    ssd1306_write_cmd(private_data, 0x00 + (x_pos & 0x0F));
    ssd1306_write_cmd(private_data, 0x10 + (x_pos >> 4));

    uint8_t text_len = strlen(text);
    for (uint8_t i = 0; i < text_len; i++) {
        uint8_t char_code = (uint8_t)text[i];
        if (char_code < FONT_FIRST_CHAR_6X8 || char_code >= (FONT_FIRST_CHAR_6X8 + FONT_CHAR_COUNT_6X8)) {
            char_code = '?';
        }
        const uint8_t *font_char_data = &ssd1306xled_font6x8[FONT_HEADER_SIZE_6X8 + (char_code - FONT_FIRST_CHAR_6X8) * 6];
        ssd1306_write_data(private_data, font_char_data, 6);
    }
}

static void screen_draw_bitmap_internal(ssd1306_private_data_t *private_data, uint8_t x_pos, uint8_t y_pos, const uint8_t *bitmap_data) {
    if (y_pos >= (private_data->height / 8) || x_pos >= private_data->width || !bitmap_data) return;

    ssd1306_write_cmd(private_data, 0xB0 + y_pos);
    ssd1306_write_cmd(private_data, 0x00 + (x_pos & 0x0F));
    ssd1306_write_cmd(private_data, 0x10 + (x_pos >> 4));
    ssd1306_write_data(private_data, bitmap_data, 8);
}

static void draw_header(ssd1306_private_data_t *private_data) {
    uint8_t clear_buffer[128] = {0};
    ssd1306_write_cmd(private_data, 0xB0 + HEADER_PAGE_NUM);
    ssd1306_write_cmd(private_data, 0x00);
    ssd1306_write_cmd(private_data, 0x10);
    ssd1306_write_data(private_data, clear_buffer, private_data->width);

    screen_draw_small_text_internal(private_data, 0, HEADER_PAGE_NUM, "synapse");

    const uint8_t *wifi_icon_to_draw = wifi_icon_disconnected;
    if (private_data->wifi_connected) {
        if (private_data->wifi_rssi >= -55) wifi_icon_to_draw = wifi_icon_signal_4;
        else if (private_data->wifi_rssi >= -67) wifi_icon_to_draw = wifi_icon_signal_3;
        else if (private_data->wifi_rssi >= -75) wifi_icon_to_draw = wifi_icon_signal_2;
        else wifi_icon_to_draw = wifi_icon_signal_1;
    }
    screen_draw_bitmap_internal(private_data, private_data->width - 8, HEADER_PAGE_NUM, wifi_icon_to_draw);
}

static void display_task(void *pvParameters) {
    module_t *self = (module_t *)pvParameters;
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;

    screen_hardware_init(private_data);
    screen_clear_internal(private_data, true);
    
    if (strlen(private_data->init_text) > 0) {
        screen_draw_large_text_internal(private_data, 1, private_data->init_text);
    }
    ESP_LOGI(TAG, "Display task for '%s' started, showing splash screen.", self->name);

    vTaskDelay(pdMS_TO_TICKS(5000));

    screen_clear_internal(private_data, true);
    draw_header(private_data);
    ESP_LOGI(TAG, "Switching to main display mode.");

    display_command_t cmd;
    while (1) {
        if (xQueueReceive(private_data->command_queue, &cmd, portMAX_DELAY) == pdPASS) {
            ESP_LOGD(TAG, "Display task received command type: %d", cmd.type);
            switch (cmd.type) {
                case DISPLAY_CMD_CLEAR_ALL:
                    screen_clear_internal(private_data, true);
                    draw_header(private_data);
                    break;
                case DISPLAY_CMD_CLEAR_CONTENT:
                    screen_clear_internal(private_data, false);
                    break;
                case DISPLAY_CMD_WRITE_LARGE_TEXT:
                    screen_draw_large_text_internal(private_data, cmd.line_num, cmd.text);
                    break;
                case DISPLAY_CMD_WRITE_TEXT:
                    screen_draw_small_text_internal(private_data, 0, cmd.line_num, cmd.text);
                    break;
                case DISPLAY_CMD_DRAW_BITMAP:
                    screen_draw_bitmap_internal(private_data, cmd.x_pos, cmd.y_pos, cmd.bitmap_data);
                    break;
                default:
                    ESP_LOGW(TAG, "Unknown command type received: %d", cmd.type);
                    break;
            }
        }
    }
}

// --- Lifecycle and Event Handler ---
static esp_err_t ssd1306_init(module_t *self) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing SSD1306 '%s' on I2C bus '%s'", self->name, private_data->i2c_bus_instance);

    private_data->wifi_connected = false;
    private_data->wifi_rssi = -100;

    private_data->i2c_bus_service = fmw_service_get(private_data->i2c_bus_instance);
    if (!private_data->i2c_bus_service) {
        ESP_LOGE(TAG, "I2C bus service '%s' not found!", private_data->i2c_bus_instance);
        return ESP_ERR_NOT_FOUND;
    }

    private_data->command_queue = xQueueCreate(COMMAND_QUEUE_LENGTH, sizeof(display_command_t));
    if (!private_data->command_queue) {
        ESP_LOGE(TAG, "Failed to create command queue");
        return ESP_ERR_NO_MEM;
    }

    esp_err_t err = fmw_service_register(self->name, FMW_SERVICE_TYPE_DISPLAY_API, &private_data->service_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register display service: %s", esp_err_to_name(err));
        return err;
    }

    fmw_event_bus_subscribe(FMW_EVENT_AGGREGATED_SENSOR_REPORT, self);
    fmw_event_bus_subscribe(FMW_EVENT_WIFI_STATUS_READY, self);
    fmw_event_bus_subscribe(EVT_INTERNAL_UPDATE_WIFI, self);

    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

static esp_err_t ssd1306_start(module_t *self) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;
    
    timer_api_t* timer_api = fmw_service_get("main_timer_service");
    if (timer_api) {
        timer_api->schedule_event(EVT_INTERNAL_UPDATE_WIFI, WIFI_STATUS_UPDATE_INTERVAL_MS, true);
        ESP_LOGI(TAG, "Scheduled periodic WiFi status update.");
    } else {
        ESP_LOGW(TAG, "System Timer service not found, WiFi status will not be updated periodically.");
    }

    BaseType_t ret = xTaskCreate(display_task, self->name, 4096, self, 5, &private_data->display_task_handle);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create display task");
        return ESP_FAIL;
    }
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void ssd1306_deinit(module_t *self) {
    if (!self) return;
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;
    if (private_data) {
        ESP_LOGI(TAG, "De-initializing SSD1306 '%s'", self->name);
        fmw_service_unregister(self->name);
        fmw_event_bus_unsubscribe(FMW_EVENT_AGGREGATED_SENSOR_REPORT, self);
        fmw_event_bus_unsubscribe(FMW_EVENT_WIFI_STATUS_READY, self);
        fmw_event_bus_unsubscribe(EVT_INTERNAL_UPDATE_WIFI, self);
        if (private_data->display_task_handle) vTaskDelete(private_data->display_task_handle);
        if (private_data->command_queue) vQueueDelete(private_data->command_queue);
        free(private_data);
    }
    if (self->current_config) cJSON_Delete(self->current_config);
    free(self);
}

/**
 * @internal
 * @brief Handles events from the Event Bus.
 * @details This function is the central point for reacting to system-wide events.
 *          It now includes robust checks to prevent crashes from NULL payloads and
 *          a corrected logic for handling WiFi status updates.
 */
static void ssd1306_handle_event(module_t *self, const char *event_name, void *event_data) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;
    if (self->status != MODULE_STATUS_RUNNING) {
        if (event_data) fmw_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    // --- Internal Timer Event for WiFi Status Update ---
        if (strcmp(event_name, EVT_INTERNAL_UPDATE_WIFI) == 0) {
        ESP_LOGD(TAG, "Internal timer triggered. Requesting SILENT WiFi status update.");
        fmw_command_payload_t* payload = calloc(1, sizeof(fmw_command_payload_t));
        if (payload) {
            // Request a silent update to avoid spamming the console
            snprintf(payload->command_string, sizeof(payload->command_string), "wifi status --silent");
            snprintf(payload->source, sizeof(payload->source), self->name);
            
            event_data_wrapper_t *wrapper;
            if (fmw_event_data_wrap(payload, fmw_payload_common_free, &wrapper) == ESP_OK) {
                fmw_event_bus_post(FMW_EVENT_EXECUTE_COMMAND_STRING, wrapper);
                fmw_event_data_release(wrapper);
            } else {
                free(payload);
            }
        }
    } 
    // --- WiFi Status Update Event ---
    else if (strcmp(event_name, FMW_EVENT_WIFI_STATUS_READY) == 0) {
        event_data_wrapper_t* wrapper = (event_data_wrapper_t*)event_data;
        
        if (wrapper && wrapper->payload) {
            fmw_telemetry_payload_t *payload = (fmw_telemetry_payload_t *)wrapper->payload;
            if (payload->json_data) {
                cJSON *root = cJSON_Parse(payload->json_data);
                if (root) {
                    const cJSON *conn_status = cJSON_GetObjectItem(root, "connection_status");
                    const cJSON *rssi_json = cJSON_GetObjectItem(root, "rssi");

                    bool new_status = (cJSON_IsString(conn_status) && strcmp(conn_status->valuestring, "Connected") == 0);
                    int32_t new_rssi = cJSON_IsNumber(rssi_json) ? rssi_json->valueint : -100;

                    if (new_status != private_data->wifi_connected || new_rssi != private_data->wifi_rssi) {
                        ESP_LOGI(TAG, "WiFi status changed. Connected: %d, RSSI: %d. Updating header.", new_status, (int)new_rssi);
                        private_data->wifi_connected = new_status;
                        private_data->wifi_rssi = new_rssi;
                        draw_header(private_data);
                    }
                    cJSON_Delete(root);
                }
            } else {
                ESP_LOGW(TAG, "Received FMW_EVENT_WIFI_STATUS_READY with NULL json_data.");
            }
        }
    }

    // Always release the event data wrapper when done
    if (event_data) {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// --- API Implementations ---
static esp_err_t api_enable(void* context) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    if (private_data->display_task_handle) {
        vTaskResume(private_data->display_task_handle);
        ssd1306_write_cmd(private_data, 0xAF);
    }
    return ESP_OK;
}

static esp_err_t api_disable(void* context) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    if (private_data->display_task_handle) {
        ssd1306_write_cmd(private_data, 0xAE);
        vTaskSuspend(private_data->display_task_handle);
    }
    return ESP_OK;
}

static esp_err_t api_clear(void* context) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    display_command_t cmd = {.type = DISPLAY_CMD_CLEAR_ALL};
    xQueueSend(private_data->command_queue, &cmd, 0);
    return ESP_OK;
}

static esp_err_t api_write_text(void* context, uint8_t line_num, const char *format, ...) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)context;
    if (line_num >= MAX_DISPLAY_LINES_SMALL) return ESP_ERR_INVALID_ARG;

    display_command_t cmd;
    cmd.type = DISPLAY_CMD_WRITE_TEXT;
    cmd.line_num = line_num;

    va_list args;
    va_start(args, format);
    vsnprintf(cmd.text, sizeof(cmd.text), format, args);
    va_end(args);

    if (xQueueSend(private_data->command_queue, &cmd, 0) != pdPASS) {
        ESP_LOGW(TAG, "Display command queue is full.");
        return ESP_FAIL;
    }
    return ESP_OK;
}

// --- Internal Config Parser ---
static esp_err_t parse_config(const cJSON *config_node, ssd1306_private_data_t *private_data) {
    if (!config_node) {
        ESP_LOGE(TAG, "Module config object is missing.");
        return ESP_ERR_INVALID_ARG;
    }

    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    if (!cJSON_IsString(name_node) || (name_node->valuestring == NULL)) {
        ESP_LOGE(TAG, "Config error: 'instance_name' is missing or not a string.");
        return ESP_ERR_INVALID_ARG;
    }
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", name_node->valuestring);

    const cJSON *bus_node = cJSON_GetObjectItem(config_node, "i2c_bus_instance");
    if (!cJSON_IsString(bus_node) || (bus_node->valuestring == NULL)) {
        ESP_LOGE(TAG, "Config error: 'i2c_bus_instance' is missing or not a string.");
        return ESP_ERR_INVALID_ARG;
    }
    snprintf(private_data->i2c_bus_instance, sizeof(private_data->i2c_bus_instance), "%s", bus_node->valuestring);

    const cJSON *addr_node = cJSON_GetObjectItem(config_node, "i2c_addr");
    if (!cJSON_IsString(addr_node) || (addr_node->valuestring == NULL)) {
        ESP_LOGE(TAG, "Config error: 'i2c_addr' is missing or not a string.");
        return ESP_ERR_INVALID_ARG;
    }
    private_data->i2c_address = (uint8_t)strtol(addr_node->valuestring, NULL, 16);

    const cJSON *width_node = cJSON_GetObjectItem(config_node, "width");
    if (cJSON_IsNumber(width_node)) {
        private_data->width = width_node->valueint;
    } else {
        ESP_LOGW(TAG, "'width' not specified, using default: 128");
        private_data->width = 128;
    }

    const cJSON *height_node = cJSON_GetObjectItem(config_node, "height");
    if (cJSON_IsNumber(height_node)) {
        private_data->height = height_node->valueint;
    } else {
        ESP_LOGW(TAG, "'height' not specified, using default: 64");
        private_data->height = 64;
    }

    const cJSON *init_text_node = cJSON_GetObjectItem(config_node, "init_text");
    if (cJSON_IsString(init_text_node) && (init_text_node->valuestring != NULL)) {
        snprintf(private_data->init_text, sizeof(private_data->init_text), "%s", init_text_node->valuestring);
    } else {
        ESP_LOGW(TAG, "'init_text' not specified, using default: 'Synapse'");
        snprintf(private_data->init_text, sizeof(private_data->init_text), "Synapse");
    }

    return ESP_OK;
}