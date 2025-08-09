/**
 * @file time_of_flight_sensor.c
 * @brief Driver for I2C-based Time-of-Flight (TOF) distance sensor VL6180X.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-07-31
 * @details This module periodically reads distance data from a VL6180X sensor
 *          via I2C and publishes the result in millimeters to the Event Bus.
 */

// --- Framework & System Includes ---
#include "synapse.h"
#include "time_of_flight_sensor.h"

#include "i2c_bus_interface.h"
#include "timer_interface.h"

// --- ESP-IDF & Standard Lib Includes ---
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("TOF_SENSOR", SYNAPSE_LOG_COLOR_GREEN);

// --- VL6180X Register Definitions ---
#define VL6180X_I2C_ADDR 0x29
#define VL6180X_REG_IDENTIFICATION_MODEL_ID 0x000
#define VL6180X_REG_SYSTEM_INTERRUPT_CLEAR 0x015
#define VL6180X_REG_SYSRANGE_START 0x018
#define VL6180X_REG_RESULT_RANGE_VAL 0x062
#define VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO 0x04F

// --- Internal Definitions & Structures ---
#define TOF_READ_TICK_EVENT "TOF_READ_TICK"
#define SENSOR_EVENT_DISTANCE "SENSOR_DATA_DISTANCE_MM"

/** @internal @brief Private data for the TOF sensor module. */
typedef struct
{
    char instance_name[32];
    char i2c_bus_name[32];
    uint8_t i2c_address;
    uint32_t update_interval_ms;

    // Runtime handles
    i2c_bus_handle_t *i2c_handle;
    synapse_timer_handle_t read_timer;
} tof_private_data_t;

// --- Forward Declarations ---
static esp_err_t tof_init(module_t *self);
static esp_err_t tof_start(module_t *self);
static void tof_deinit(module_t *self);
static void tof_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t parse_config(const cJSON *config, tof_private_data_t *private_data);
static esp_err_t read_and_publish_data(module_t *self);
static esp_err_t publish_sensor_value(const char *event_name, double value);
static esp_err_t vl6180x_init_sensor(tof_private_data_t *private_data);
static esp_err_t vl6180x_write_reg(tof_private_data_t *private_data, uint16_t reg, uint8_t val);
static esp_err_t vl6180x_read_reg(tof_private_data_t *private_data, uint16_t reg, uint8_t *val);

// =========================================================================
//                      Module Lifecycle & Core Logic
// =========================================================================

module_t *time_of_flight_sensor_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    tof_private_data_t *private_data = (tof_private_data_t *)calloc(1, sizeof(tof_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        return NULL;
    }
    module->private_data = private_data;

    if (parse_config(config, private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to parse configuration. Aborting creation.");
        free(private_data);
        free(module);
        return NULL;
    }

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->init_level = 60;
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->base.init = tof_init;
    module->base.start = tof_start;
    module->base.deinit = tof_deinit;
    module->base.handle_event = tof_handle_event;

    ESP_LOGI(TAG, "Time of Flight Sensor module created: '%s'", private_data->instance_name);
    return module;
}

static esp_err_t tof_init(module_t *self)
{
    tof_private_data_t *private_data = (tof_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing module: %s", self->name);

    private_data->i2c_handle = (i2c_bus_handle_t *)synapse_service_get(private_data->i2c_bus_name);
    if (!private_data->i2c_handle)
    {
        ESP_LOGE(TAG, "I2C bus service '%s' not found!", private_data->i2c_bus_name);
        return ESP_FAIL;
    }

    if (vl6180x_init_sensor(private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize VL6180X sensor.");
        return ESP_FAIL;
    }

    synapse_event_bus_subscribe(TOF_READ_TICK_EVENT, self);
    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

static esp_err_t tof_start(module_t *self)
{
    tof_private_data_t *private_data = (tof_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting module: %s", self->name);

    service_handle_t timer_service = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_TIMER_API);
    if (!timer_service)
    {
        ESP_LOGE(TAG, "System Timer service not found!");
        return ESP_FAIL;
    }
    timer_api_t *timer_api = (timer_api_t *)timer_service;
    private_data->read_timer = timer_api->schedule_event(TOF_READ_TICK_EVENT, private_data->update_interval_ms, true);

    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void tof_deinit(module_t *self)
{
    if (!self) return;
    tof_private_data_t *private_data = (tof_private_data_t *)self->private_data;

    service_handle_t timer_service = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_TIMER_API);
    if (timer_service && private_data->read_timer)
    {
        ((timer_api_t *)timer_service)->cancel_event(private_data->read_timer);
    }
    synapse_event_bus_unsubscribe(TOF_READ_TICK_EVENT, self);

    free(private_data);
}

static void tof_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (strcmp(event_name, TOF_READ_TICK_EVENT) == 0)
    {
        read_and_publish_data(self);
    }

    if (event_data)
    {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// =========================================================================
//                      Internal Helper Functions
// =========================================================================

static esp_err_t read_and_publish_data(module_t *self)
{
    tof_private_data_t *private_data = (tof_private_data_t *)self->private_data;
    uint8_t status = 0;

    // Wait for device to be ready
    vl6180x_read_reg(private_data, 0x04d, &status);
    if (!(status & 0x01)) {
        ESP_LOGW(TAG, "Sensor not ready for new measurement.");
        return ESP_ERR_INVALID_STATE;
    }

    // Start single-shot measurement
    vl6180x_write_reg(private_data, VL6180X_REG_SYSRANGE_START, 0x01);

    // Poll for measurement completion
    for (int i = 0; i < 100; i++) {
        vTaskDelay(pdMS_TO_TICKS(1));
        vl6180x_read_reg(private_data, VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO, &status);
        if (status & 0x04) {
            break;
        }
    }

    if (!(status & 0x04)) {
        ESP_LOGW(TAG, "Measurement timed out.");
        return ESP_ERR_TIMEOUT;
    }

    // Read distance value
    uint8_t distance = 0;
    vl6180x_read_reg(private_data, VL6180X_REG_RESULT_RANGE_VAL, &distance);

    // Clear interrupt
    vl6180x_write_reg(private_data, VL6180X_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);

    ESP_LOGI(TAG, "Distance: %u mm", distance);
    publish_sensor_value(SENSOR_EVENT_DISTANCE, (double)distance);

    return ESP_OK;
}

static esp_err_t publish_sensor_value(const char *event_name, double value)
{
    cJSON *payload_json = cJSON_CreateObject();
    if (!payload_json) return ESP_ERR_NO_MEM;

    cJSON_AddNumberToObject(payload_json, "value", value);
    char *json_string = cJSON_PrintUnformatted(payload_json);
    cJSON_Delete(payload_json);

    if (!json_string) return ESP_ERR_NO_MEM;

    event_data_wrapper_t *wrapper;
    esp_err_t err = synapse_event_data_wrap(json_string, free, &wrapper);
    if (err == ESP_OK)
    {
        synapse_event_bus_post(event_name, wrapper);
        synapse_event_data_release(wrapper);
    }
    else
    {
        free(json_string);
    }
    return err;
}

static esp_err_t parse_config(const cJSON *config, tof_private_data_t *private_data)
{
    if (!private_data || !config)
    {
        return ESP_ERR_INVALID_ARG;
    }

    // --- Step 1: Set default values from Kconfig ---
    synapse_safe_strncpy(private_data->instance_name, CONFIG_TOF_SENSOR_DEFAULT_INSTANCE_NAME, sizeof(private_data->instance_name));
    synapse_safe_strncpy(private_data->i2c_bus_name, CONFIG_TOF_SENSOR_DEFAULT_I2C_BUS, sizeof(private_data->i2c_bus_name));
    private_data->update_interval_ms = CONFIG_TOF_SENSOR_DEFAULT_UPDATE_INTERVAL * 1000;
    private_data->i2c_address = VL6180X_I2C_ADDR; // This seems to be a fixed hardware address

    // --- Step 2: Get the main "config" node ---
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (!config_node)
    {
        ESP_LOGW(TAG, "No 'config' object in JSON. Using default values from Kconfig.");
        // Log the final values for clarity
        ESP_LOGI(TAG, "Config parsed for '%s': Interval=%lums, I2C Bus='%s'",
                 private_data->instance_name, private_data->update_interval_ms, private_data->i2c_bus_name);
        return ESP_OK;
    }

    // --- Step 3: Use utility functions to override defaults ---
    synapse_config_get_string_from_node(TAG, config_node, "instance_name",
                                        private_data->instance_name, sizeof(private_data->instance_name));

    synapse_config_get_string_from_node(TAG, config_node, "i2c_bus_name",
                                        private_data->i2c_bus_name, sizeof(private_data->i2c_bus_name));

    int interval_sec = private_data->update_interval_ms / 1000;
    if (synapse_config_get_int_from_node(TAG, config_node, "update_interval_sec", &interval_sec))
    {
        private_data->update_interval_ms = interval_sec * 1000;
    }

    // The I2C address is typically fixed for this sensor, but we can make it configurable if needed.
    // synapse_config_get_int_from_node(TAG, config_node, "i2c_address", (int*)&private_data->i2c_address);

    ESP_LOGI(TAG, "Config parsed for '%s': Interval=%lums, I2C Bus='%s'",
             private_data->instance_name, private_data->update_interval_ms, private_data->i2c_bus_name);

    return ESP_OK;
}

static esp_err_t vl6180x_init_sensor(tof_private_data_t *private_data)
{
    uint8_t model_id = 0;
    esp_err_t err = vl6180x_read_reg(private_data, VL6180X_REG_IDENTIFICATION_MODEL_ID, &model_id);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read Model ID register: %s", esp_err_to_name(err));
        return err;
    }

    // The datasheet for VL6180X says the model ID is 0xB4.
    if (model_id != 0xB4) {
        ESP_LOGE(TAG, "VL6180X not found. Model ID = 0x%02X (expected 0xB4)", model_id);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "VL6180X sensor found successfully! Model ID: 0x%02X", model_id);

    // Mandatory init sequence from datasheet
    vl6180x_write_reg(private_data, 0x0207, 0x01);
    vl6180x_write_reg(private_data, 0x0208, 0x01);
    vl6180x_write_reg(private_data, 0x0096, 0x00);
    vl6180x_write_reg(private_data, 0x0097, 0xfd);
    vl6180x_write_reg(private_data, 0x00e3, 0x00);
    vl6180x_write_reg(private_data, 0x00e4, 0x04);
    vl6180x_write_reg(private_data, 0x00e5, 0x02);
    vl6180x_write_reg(private_data, 0x00e6, 0x01);
    vl6180x_write_reg(private_data, 0x00e7, 0x03);
    vl6180x_write_reg(private_data, 0x00f5, 0x02);
    vl6180x_write_reg(private_data, 0x00d9, 0x05);
    vl6180x_write_reg(private_data, 0x00db, 0xce);
    vl6180x_write_reg(private_data, 0x00dc, 0x03);
    vl6180x_write_reg(private_data, 0x00dd, 0xf8);
    vl6180x_write_reg(private_data, 0x009f, 0x00);
    vl6180x_write_reg(private_data, 0x00a3, 0x3c);
    vl6180x_write_reg(private_data, 0x00b7, 0x00);
    vl6180x_write_reg(private_data, 0x00bb, 0x3c);
    vl6180x_write_reg(private_data, 0x00b2, 0x09);
    vl6180x_write_reg(private_data, 0x00ca, 0x09);
    vl6180x_write_reg(private_data, 0x0198, 0x01);
    vl6180x_write_reg(private_data, 0x01b0, 0x17);
    vl6180x_write_reg(private_data, 0x01ad, 0x00);
    vl6180x_write_reg(private_data, 0x00ff, 0x05);
    vl6180x_write_reg(private_data, 0x0100, 0x05);
    vl6180x_write_reg(private_data, 0x0199, 0x05);
    vl6180x_write_reg(private_data, 0x01a6, 0x1b);
    vl6180x_write_reg(private_data, 0x01ac, 0x3e);
    vl6180x_write_reg(private_data, 0x01a7, 0x1f);
    vl6180x_write_reg(private_data, 0x0030, 0x00);

    // Recommended settings from datasheet
    vl6180x_write_reg(private_data, 0x0011, 0x10); // Enables polling for 'New Sample ready'
    vl6180x_write_reg(private_data, 0x010a, 0x30); // Set Avg sample period
    vl6180x_write_reg(private_data, 0x003f, 0x46); // Set ALS integration period to 100ms
    vl6180x_write_reg(private_data, 0x0031, 0xFF); // Set 100 Lux ALS gain
    vl6180x_write_reg(private_data, 0x0040, 0x63); // Set ALS analog gain to 10
    vl6180x_write_reg(private_data, 0x002e, 0x01); // Set default ranging inter-measurement period to 100ms
    vl6180x_write_reg(private_data, 0x001b, 0x09); // Set default ALS inter-measurement period to 100ms
    vl6180x_write_reg(private_data, 0x003e, 0x31); // Set default ALS thresholds
    vl6180x_write_reg(private_data, 0x0014, 0x24); // Configure interrupt on new sample ready

    return ESP_OK;
}

// --- I2C Helper Functions for 16-bit Registers ---

static esp_err_t vl6180x_write_reg(tof_private_data_t *private_data, uint16_t reg, uint8_t val)
{
    uint8_t write_buf[3] = {(reg >> 8) & 0xFF, reg & 0xFF, val};
    return private_data->i2c_handle->api->write(private_data->i2c_handle->context, private_data->i2c_address, write_buf, 3);
}

static esp_err_t vl6180x_read_reg(tof_private_data_t *private_data, uint16_t reg, uint8_t *val)
{
    uint8_t write_buf[2] = {(reg >> 8) & 0xFF, reg & 0xFF};
    return private_data->i2c_handle->api->write_read(private_data->i2c_handle->context, private_data->i2c_address, write_buf, 2, val, 1);
}