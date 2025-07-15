/**
 * @file sensor_aggregator.c
 * @brief Implementation of the Sensor Aggregator module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-17
 * @details This module listens for individual sensor events, aggregates
 *          their data into a single snapshot, and publishes a consolidated
 *          report based on a configured strategy (time window or trigger event).
 */

// --- Framework & System Includes ---
#include "sensor_aggregator.h"
#include "base_module.h"
#include "logging.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "event_payloads.h"
#include "service_locator.h"
#include "timer_interface.h"
#include "framework_events.h"

// --- ESP-IDF & Standard Lib Includes ---
#include "esp_log.h"
#include "esp_timer.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>
#include "sdkconfig.h"

DEFINE_COMPONENT_TAG("SENSOR_AGGR");

// --- Internal Definitions & Structures ---

#define AGGREGATOR_PUBLISH_TICK_EVENT "AGGREGATOR_PUBLISH_TICK"

/** @internal @enum aggregation_strategy_t Defines how aggregation is triggered. */
typedef enum
{
    STRATEGY_UNKNOWN,
    STRATEGY_TIME_WINDOW,
    STRATEGY_ON_TRIGGER,
} aggregation_strategy_t;

/** @internal @struct sensor_mapping_t Maps an event name to a JSON key. */
typedef struct
{
    char event_name[CONFIG_SENSOR_AGGREGATOR_EVENT_NAME_MAX_LEN];
    char json_key[32];
} sensor_mapping_t;

/** @internal @brief Private data structure for the Sensor Aggregator module. */
typedef struct
{
    char instance_name[CONFIG_SENSOR_AGGREGATOR_INSTANCE_NAME_MAX_LEN];

    // Configuration
    aggregation_strategy_t strategy;
    uint32_t aggregation_period_ms;
    char trigger_event[32];
    sensor_mapping_t sensors[CONFIG_SENSOR_AGGREGATOR_MAX_SENSORS];
    uint8_t num_sensors;

    // Runtime State
    cJSON *current_snapshot;
    fmw_timer_handle_t publish_timer;
    SemaphoreHandle_t snapshot_mutex;

} sensor_aggregator_private_data_t;

// --- Forward Declarations ---
static esp_err_t sensor_aggregator_init(module_t *self);
static esp_err_t sensor_aggregator_start(module_t *self);
static void sensor_aggregator_deinit(module_t *self);
static void sensor_aggregator_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t parse_aggregator_config(const cJSON *config_json, sensor_aggregator_private_data_t *private_data);
static void publish_report(module_t *self);

// =========================================================================
//                      Module Lifecycle & Core Logic
// =========================================================================

module_t *sensor_aggregator_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    sensor_aggregator_private_data_t *private_data = (sensor_aggregator_private_data_t *)calloc(1, sizeof(sensor_aggregator_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        return NULL;
    }

    private_data->snapshot_mutex = xSemaphoreCreateMutex();
    if (!private_data->snapshot_mutex)
    {
        ESP_LOGE(TAG, "Failed to create snapshot mutex");
        free(private_data);
        free(module);
        return NULL;
    }

    private_data->current_snapshot = cJSON_CreateObject();
    if (!private_data->current_snapshot)
    {
        ESP_LOGE(TAG, "Failed to create snapshot cJSON object");
        vSemaphoreDelete(private_data->snapshot_mutex);
        free(private_data);
        free(module);
        return NULL;
    }

    module->private_data = private_data;

    if (parse_aggregator_config(config, private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to parse configuration. Aborting creation.");
        cJSON_Delete(private_data->current_snapshot);
        vSemaphoreDelete(private_data->snapshot_mutex);
        free(private_data);
        free(module);
        return NULL;
    }

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->init_level = 48;
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->base.init = sensor_aggregator_init;
    module->base.start = sensor_aggregator_start;
    module->base.deinit = sensor_aggregator_deinit;
    module->base.handle_event = sensor_aggregator_handle_event;

    ESP_LOGI(TAG, "Sensor Aggregator module created: '%s'", private_data->instance_name);
    return module;
}

static esp_err_t sensor_aggregator_init(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    sensor_aggregator_private_data_t *private_data = (sensor_aggregator_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing module: %s", self->name);

    // Subscribe to all sensor events defined in the config
    for (int i = 0; i < private_data->num_sensors; i++)
    {
        ESP_LOGI(TAG, "Subscribing to sensor event: '%s'", private_data->sensors[i].event_name);
        fmw_event_bus_subscribe(private_data->sensors[i].event_name, self);
    }

    // Subscribe to our own timer tick if needed
    if (private_data->strategy == STRATEGY_TIME_WINDOW)
    {
        fmw_event_bus_subscribe(AGGREGATOR_PUBLISH_TICK_EVENT, self);
    }

    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

static esp_err_t sensor_aggregator_start(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    sensor_aggregator_private_data_t *private_data = (sensor_aggregator_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting module: %s", self->name);

    if (private_data->strategy == STRATEGY_TIME_WINDOW)
    {
        service_handle_t timer_service = fmw_service_lookup_by_type(FMW_SERVICE_TYPE_TIMER_API);
        if (!timer_service)
        {
            ESP_LOGE(TAG, "System Timer service not found! Cannot start time_window strategy.");
            self->status = MODULE_STATUS_ERROR;
            return ESP_FAIL;
        }
        timer_api_t *timer_api = (timer_api_t *)timer_service;
        private_data->publish_timer = timer_api->schedule_event(AGGREGATOR_PUBLISH_TICK_EVENT, private_data->aggregation_period_ms, true);
        if (!private_data->publish_timer)
        {
            ESP_LOGE(TAG, "Failed to schedule aggregation timer.");
            self->status = MODULE_STATUS_ERROR;
            return ESP_FAIL;
        }
        ESP_LOGI(TAG, "Aggregation timer scheduled every %lu ms.", private_data->aggregation_period_ms);
    }

    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void sensor_aggregator_deinit(module_t *self)
{
    if (!self)
        return;
    sensor_aggregator_private_data_t *private_data = (sensor_aggregator_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Deinitializing module: %s", self->name);

    // Unsubscribe from all events
    for (int i = 0; i < private_data->num_sensors; i++)
    {
        fmw_event_bus_unsubscribe(private_data->sensors[i].event_name, self);
    }
    if (private_data->strategy == STRATEGY_TIME_WINDOW)
    {
        fmw_event_bus_unsubscribe(AGGREGATOR_PUBLISH_TICK_EVENT, self);
        service_handle_t timer_service = fmw_service_lookup_by_type(FMW_SERVICE_TYPE_TIMER_API);
        if (timer_service && private_data->publish_timer)
        {
            ((timer_api_t *)timer_service)->cancel_event(private_data->publish_timer);
        }
    }

    if (private_data->snapshot_mutex)
        vSemaphoreDelete(private_data->snapshot_mutex);
    if (private_data->current_snapshot)
        cJSON_Delete(private_data->current_snapshot);
    free(private_data);
    free(self);
}

static void sensor_aggregator_handle_event(module_t *self, const char *event_name, void *event_data) {
    if (!self || !event_name) {
        if (event_data) fmw_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }
    sensor_aggregator_private_data_t *private_data = (sensor_aggregator_private_data_t *)self->private_data;

    if (strcmp(event_name, AGGREGATOR_PUBLISH_TICK_EVENT) == 0) {
        publish_report(self);
        goto cleanup;
    }

    for (int i = 0; i < private_data->num_sensors; i++) {
        if (strcmp(event_name, private_data->sensors[i].event_name) == 0) {
            if (event_data) {
                event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;
                cJSON *payload_json = cJSON_Parse((char*)wrapper->payload);
                if (payload_json) {
                    const cJSON *value_node = cJSON_GetObjectItem(payload_json, "value");
                    if (value_node) {
                        if (xSemaphoreTake(private_data->snapshot_mutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                            
                            // ★★★ THE FIX IS HERE ★★★
                            // Detach the value from the parsed payload to use it.
                            cJSON *value_to_add = cJSON_DetachItemFromObject(payload_json, "value");

                            // Check if an item with this key already exists. If so, replace it.
                            // Otherwise, add it as a new item.
                            if (cJSON_HasObjectItem(private_data->current_snapshot, private_data->sensors[i].json_key)) {
                                cJSON_ReplaceItemInObject(private_data->current_snapshot, private_data->sensors[i].json_key, value_to_add);
                            } else {
                                cJSON_AddItemToObject(private_data->current_snapshot, private_data->sensors[i].json_key, value_to_add);
                            }
                            
                            xSemaphoreGive(private_data->snapshot_mutex);
                            ESP_LOGD(TAG, "Updated/Added data for '%s'", private_data->sensors[i].json_key);
                        }
                    }
                    cJSON_Delete(payload_json);
                }
            }
            if (private_data->strategy == STRATEGY_ON_TRIGGER && strcmp(event_name, private_data->trigger_event) == 0) {
                publish_report(self);
            }
            goto cleanup;
        }
    }

cleanup:
    if (event_data) {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// =========================================================================
//                      Internal Helper Functions
// =========================================================================

/**
 * @internal
 * @brief Publishes the aggregated sensor report.
 * @details This function creates a JSON string from the current snapshot,
 *          wraps it in a telemetry payload, and posts it to the Event Bus.
 *          Crucially, it then deletes the old snapshot and creates a new
 *          empty one for the next aggregation cycle to prevent data duplication.
 * @param[in] self A pointer to the module instance.
 */
static void publish_report(module_t *self) {
    sensor_aggregator_private_data_t *private_data = (sensor_aggregator_private_data_t *)self->private_data;

    if (xSemaphoreTake(private_data->snapshot_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to lock snapshot for publishing.");
        return;
    }

    // Check if there is any data to report
    if (cJSON_GetArraySize(private_data->current_snapshot) == 0) {
        ESP_LOGI(TAG, "Snapshot is empty, skipping report publication.");
        xSemaphoreGive(private_data->snapshot_mutex);
        return;
    }

    // Add timestamp to the report
    cJSON_AddNumberToObject(private_data->current_snapshot, "timestamp", esp_timer_get_time() / 1000);

    char *json_string = cJSON_PrintUnformatted(private_data->current_snapshot);
    
    // ★★★ FIX: Clear the snapshot for the next cycle ★★★
    cJSON_Delete(private_data->current_snapshot);
    private_data->current_snapshot = cJSON_CreateObject();
    if (!private_data->current_snapshot) {
        ESP_LOGE(TAG, "Failed to re-create snapshot cJSON object! This will cause issues.");
    }
    
    xSemaphoreGive(private_data->snapshot_mutex);

    if (!json_string) {
        ESP_LOGE(TAG, "Failed to generate JSON string from snapshot.");
        return;
    }

    ESP_LOGI(TAG, "Generated aggregated JSON: %s", json_string);

    // Create and post the event
    fmw_telemetry_payload_t *payload = malloc(sizeof(fmw_telemetry_payload_t));
    if (payload) {
        strncpy(payload->module_name, self->name, sizeof(payload->module_name) - 1);
        payload->json_data = json_string; // Ownership of json_string is transferred

        event_data_wrapper_t *wrapper;
        if (fmw_event_data_wrap(payload, fmw_telemetry_payload_free, &wrapper) == ESP_OK) {
            ESP_LOGI(TAG, "Publishing aggregated report event: %s", FMW_EVENT_AGGREGATED_SENSOR_REPORT);
            fmw_event_bus_post(FMW_EVENT_AGGREGATED_SENSOR_REPORT, wrapper);
            fmw_event_data_release(wrapper);
        } else {
            free(json_string);
            free(payload);
        }
    } else {
        free(json_string);
        ESP_LOGE(TAG, "Failed to allocate memory for telemetry payload.");
    }
}

/**
 * @internal
 * @brief Parses the module's configuration from a cJSON object.
 * @param[in] config The root cJSON object for the module.
 * @param[out] private_data Pointer to the private data structure to be filled.
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG on parsing error.
 */
static esp_err_t parse_aggregator_config(const cJSON *config, sensor_aggregator_private_data_t *private_data)
{
    if (!config || !private_data)
        return ESP_ERR_INVALID_ARG;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (!cJSON_IsObject(config_node))
    {
        ESP_LOGE(TAG, "Missing 'config' object in JSON.");
        return ESP_ERR_INVALID_ARG;
    }

    // Parse instance_name
    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    if (cJSON_IsString(name_node))
    {
        strncpy(private_data->instance_name, name_node->valuestring, sizeof(private_data->instance_name) - 1);
    }
    else
    {
        strncpy(private_data->instance_name, CONFIG_SENSOR_AGGREGATOR_DEFAULT_INSTANCE_NAME, sizeof(private_data->instance_name) - 1);
    }

    // Parse aggregation_strategy
    const cJSON *strategy_node = cJSON_GetObjectItem(config_node, "aggregation_strategy");
    if (cJSON_IsString(strategy_node))
    {
        if (strcmp(strategy_node->valuestring, "time_window") == 0)
        {
            private_data->strategy = STRATEGY_TIME_WINDOW;
        }
        else if (strcmp(strategy_node->valuestring, "on_trigger") == 0)
        {
            private_data->strategy = STRATEGY_ON_TRIGGER;
        }
    }
    if (private_data->strategy == STRATEGY_UNKNOWN)
    {
        ESP_LOGE(TAG, "Invalid or missing 'aggregation_strategy'. Must be 'time_window' or 'on_trigger'.");
        return ESP_ERR_INVALID_ARG;
    }

    // Parse strategy-specific parameters
    if (private_data->strategy == STRATEGY_TIME_WINDOW)
    {
        const cJSON *period_node = cJSON_GetObjectItem(config_node, "aggregation_period_sec");
        if (cJSON_IsNumber(period_node))
        {
            private_data->aggregation_period_ms = period_node->valueint * 1000;
        }
        else
        {
            private_data->aggregation_period_ms = 60000; // Default 60s
        }
    }
    else
    { // STRATEGY_ON_TRIGGER
        const cJSON *trigger_node = cJSON_GetObjectItem(config_node, "trigger_event");
        if (cJSON_IsString(trigger_node))
        {
            strncpy(private_data->trigger_event, trigger_node->valuestring, sizeof(private_data->trigger_event) - 1);
        }
        else
        {
            ESP_LOGE(TAG, "Missing 'trigger_event' for 'on_trigger' strategy.");
            return ESP_ERR_INVALID_ARG;
        }
    }

    // Parse sensors_to_aggregate
    const cJSON *sensors_array = cJSON_GetObjectItem(config_node, "sensors_to_aggregate");
    if (!cJSON_IsArray(sensors_array))
    {
        ESP_LOGE(TAG, "Missing or invalid 'sensors_to_aggregate' array in config.");
        return ESP_ERR_INVALID_ARG;
    }

    private_data->num_sensors = 0;
    cJSON *sensor_item;
    cJSON_ArrayForEach(sensor_item, sensors_array)
    {
        if (private_data->num_sensors >= CONFIG_SENSOR_AGGREGATOR_MAX_SENSORS)
        {
            ESP_LOGW(TAG, "Max sensors to aggregate reached (%d). Ignoring further entries.", CONFIG_SENSOR_AGGREGATOR_MAX_SENSORS);
            break;
        }
        const cJSON *event_name = cJSON_GetObjectItem(sensor_item, "event_name");
        const cJSON *json_key = cJSON_GetObjectItem(sensor_item, "json_key");
        if (cJSON_IsString(event_name) && cJSON_IsString(json_key))
        {
            strncpy(private_data->sensors[private_data->num_sensors].event_name, event_name->valuestring, 31);
            strncpy(private_data->sensors[private_data->num_sensors].json_key, json_key->valuestring, 31);
            private_data->num_sensors++;
        }
    }

    ESP_LOGI(TAG, "Parsed %d sensors to aggregate.", private_data->num_sensors);
    return ESP_OK;
}