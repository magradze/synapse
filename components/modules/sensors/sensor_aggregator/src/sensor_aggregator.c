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
#include "synapse.h"
#include "sensor_aggregator.h"

#include "timer_interface.h"
#include "storage_interface.h"

// --- ESP-IDF & Standard Lib Includes ---
#include "esp_log.h"
#include "esp_timer.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include <sys/stat.h> // For stat() and mkdir()
#include <dirent.h>   // For opendir(), readdir(), closedir()
#include "esp_vfs.h"

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

/** @internal @enum aggregation_mode_t Defines the data aggregation mode for a sensor. */
typedef enum
{
    AGG_MODE_LAST, /**< Report the last received value. */
    AGG_MODE_AVG,  /**< Report the average of all received values in the window. */
    AGG_MODE_MIN,  /**< Report the minimum value received in the window. */
    AGG_MODE_MAX   /**< Report the maximum value received in the window. */
} aggregation_mode_t;

/** @internal @struct sensor_runtime_data_t Holds the runtime aggregated data for a single sensor. */
typedef struct
{
    uint8_t count;     /**< Number of readings received in the current window. */
    double sum;        /**< Sum of all readings for calculating the average. */
    double min;        /**< Minimum value recorded in the window. */
    double max;        /**< Maximum value recorded in the window. */
    double last_value; /**< The most recent value received. */
} sensor_runtime_data_t;

/** @internal @struct sensor_mapping_t Maps an event name to a JSON key. */
typedef struct
{
    char event_name[CONFIG_SENSOR_AGGREGATOR_EVENT_NAME_MAX_LEN];
    char json_key[32];
    aggregation_mode_t mode;
    bool has_valid_range;
    double valid_min;
    double valid_max;
    sensor_runtime_data_t runtime_data;
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
    synapse_timer_handle_t publish_timer;
    SemaphoreHandle_t snapshot_mutex;

    bool buffering_enabled;
    bool is_offline;

} sensor_aggregator_private_data_t;

// --- Forward Declarations ---
static esp_err_t sensor_aggregator_init(module_t *self);
static esp_err_t sensor_aggregator_start(module_t *self);
static void sensor_aggregator_deinit(module_t *self);
static void sensor_aggregator_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t parse_aggregator_config(const cJSON *config_json, sensor_aggregator_private_data_t *private_data);
static void publish_report(module_t *self);
static void publish_buffered_reports(module_t *self);

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

    private_data->buffering_enabled = CONFIG_SENSOR_AGGREGATOR_BUFFERING_ENABLED;
    private_data->is_offline = true; // Assume offline until connectivity is confirmed

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
        synapse_event_bus_subscribe(private_data->sensors[i].event_name, self);
    }

    // Subscribe to our own timer tick if needed
    if (private_data->strategy == STRATEGY_TIME_WINDOW)
    {
        synapse_event_bus_subscribe(AGGREGATOR_PUBLISH_TICK_EVENT, self);
    }

    // Subscribe to connectivity events if buffering is enabled
    if (private_data->buffering_enabled)
    {
        synapse_event_bus_subscribe(SYNAPSE_EVENT_CONNECTIVITY_ESTABLISHED, self);
        synapse_event_bus_subscribe(SYNAPSE_EVENT_CONNECTIVITY_LOST, self);
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
        service_handle_t timer_service = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_TIMER_API);
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
        synapse_event_bus_unsubscribe(private_data->sensors[i].event_name, self);
    }
    if (private_data->strategy == STRATEGY_TIME_WINDOW)
    {
        synapse_event_bus_unsubscribe(AGGREGATOR_PUBLISH_TICK_EVENT, self);
        service_handle_t timer_service = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_TIMER_API);
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
}

static void sensor_aggregator_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (!self || !event_name)
    {
        if (event_data)
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }
    sensor_aggregator_private_data_t *private_data = (sensor_aggregator_private_data_t *)self->private_data;

    // Handle connectivity events for buffering
    if (private_data->buffering_enabled)
    {
        if (strcmp(event_name, SYNAPSE_EVENT_CONNECTIVITY_ESTABLISHED) == 0)
        {
            ESP_LOGI(TAG, "Connectivity established. Flushing buffered reports.");
            private_data->is_offline = false;
            publish_buffered_reports(self); // Publish any stored reports
            goto cleanup;
        }
        else if (strcmp(event_name, SYNAPSE_EVENT_CONNECTIVITY_LOST) == 0)
        {
            ESP_LOGW(TAG, "Connectivity lost. Buffering will be enabled.");
            private_data->is_offline = true;
            goto cleanup;
        }
    }

    if (strcmp(event_name, AGGREGATOR_PUBLISH_TICK_EVENT) == 0)
    {
        publish_report(self);
        goto cleanup;
    }

    for (int i = 0; i < private_data->num_sensors; i++)
    {
        if (strcmp(event_name, private_data->sensors[i].event_name) == 0)
        {
            sensor_mapping_t *sensor_map = &private_data->sensors[i];

            if (event_data)
            {
                event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;
                cJSON *payload_json = cJSON_Parse((char *)wrapper->payload);
                if (payload_json)
                {
                    const cJSON *value_node = cJSON_GetObjectItem(payload_json, "value");
                    if (cJSON_IsNumber(value_node))
                    {
                        double value = value_node->valuedouble;

                        // 1. Validate data if range is specified
                        if (sensor_map->has_valid_range && (value < sensor_map->valid_min || value > sensor_map->valid_max))
                        {
                            ESP_LOGW(TAG, "Sensor '%s' value %.2f is out of valid range (%.2f to %.2f). Ignoring.",
                                     sensor_map->event_name, value, sensor_map->valid_min, sensor_map->valid_max);
                            // Optionally, post a SENSOR_DATA_INVALID event here
                        }
                        else
                        {
                            // 2. Accumulate data
                            if (xSemaphoreTake(private_data->snapshot_mutex, pdMS_TO_TICKS(50)) == pdTRUE)
                            {
                                sensor_runtime_data_t *rt_data = &sensor_map->runtime_data;
                                rt_data->last_value = value;
                                rt_data->sum += value;
                                rt_data->count++;
                                if (value < rt_data->min)
                                    rt_data->min = value;
                                if (value > rt_data->max)
                                    rt_data->max = value;
                                xSemaphoreGive(private_data->snapshot_mutex);
                                ESP_LOGD(TAG, "Aggregated data for '%s'", sensor_map->json_key);
                            }
                        }
                    }
                    cJSON_Delete(payload_json);
                }
            }
            // Check for on_trigger strategy
            if (private_data->strategy == STRATEGY_ON_TRIGGER && strcmp(event_name, private_data->trigger_event) == 0)
            {
                publish_report(self);
            }
            goto cleanup;
        }
    }

cleanup:
    if (event_data)
    {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// =========================================================================
//                      Internal Helper Functions
// =========================================================================

/**
 * @internal
 * @brief Generates and publishes the aggregated sensor report.
 * @details This function is the core of the reporting logic. It iterates
 *          through all monitored sensors, calculates the final value for each
 *          based on its configured aggregation mode (last, avg, min, max),
 *          builds a new cJSON object with the results, and posts it to the
 *          Event Bus. After publishing, it resets the runtime data for each
 *          sensor to prepare for the next aggregation window.
 * @param[in] self A pointer to the module instance.
 */
static void publish_report(module_t *self)
{
    sensor_aggregator_private_data_t *private_data = (sensor_aggregator_private_data_t *)self->private_data;

    // Create a new JSON object for this specific report.
    cJSON *report_json = cJSON_CreateObject();
    if (!report_json)
    {
        ESP_LOGE(TAG, "Failed to create cJSON object for report.");
        return;
    }

    bool has_data_to_report = false;

    // Lock the mutex to safely access and then reset the runtime data.
    if (xSemaphoreTake(private_data->snapshot_mutex, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Failed to lock runtime data for publishing.");
        cJSON_Delete(report_json);
        return;
    }

    // Iterate through all configured sensors to build the report
    for (int i = 0; i < private_data->num_sensors; i++)
    {
        sensor_mapping_t *sensor_map = &private_data->sensors[i];
        sensor_runtime_data_t *rt_data = &sensor_map->runtime_data;

        // Only include sensors that have received at least one valid reading
        if (rt_data->count > 0)
        {
            has_data_to_report = true;
            double final_value = 0.0;

            // Calculate the final value based on the configured aggregation mode
            switch (sensor_map->mode)
            {
            case AGG_MODE_AVG:
                final_value = rt_data->sum / rt_data->count;
                break;
            case AGG_MODE_MIN:
                final_value = rt_data->min;
                break;
            case AGG_MODE_MAX:
                final_value = rt_data->max;
                break;
            case AGG_MODE_LAST:
            default:
                final_value = rt_data->last_value;
                break;
            }
            cJSON_AddNumberToObject(report_json, sensor_map->json_key, final_value);

            // Reset runtime data for the next aggregation window
            rt_data->count = 0;
            rt_data->sum = 0;
            rt_data->min = __DBL_MAX__;
            rt_data->max = -__DBL_MAX__;
        }
    }

    xSemaphoreGive(private_data->snapshot_mutex);

    // If no sensors reported any data in this window, don't send an empty report.
    if (!has_data_to_report)
    {
        ESP_LOGI(TAG, "No new sensor data in this window, skipping report publication.");
        cJSON_Delete(report_json);
        return;
    }

    // Add a timestamp to the final report
    cJSON_AddNumberToObject(report_json, "timestamp", esp_timer_get_time() / 1000);

    char *json_string = cJSON_PrintUnformatted(report_json);
    cJSON_Delete(report_json); // We are done with the cJSON object, free its memory.

    if (!json_string)
    {
        ESP_LOGE(TAG, "Failed to generate JSON string from report object.");
        return;
    }

    // If buffering is enabled and we are offline, save to file instead of publishing
    if (private_data->buffering_enabled && private_data->is_offline)
    {
        ESP_LOGI(TAG, "Device is offline. Buffering report to filesystem.");
        service_handle_t storage_handle = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_NVRAM_API);
        if (storage_handle)
        {
            storage_api_t *storage_api = (storage_api_t *)storage_handle;
            // Use a buffer that is guaranteed to be safe for the path
            char file_path[CONFIG_SENSOR_AGGREGATOR_PATH_BUFFER_SIZE];

            // Create a filename that is guaranteed to be short enough for SPIFFS
            // Use %lu for uint32_t and cast to unsigned long for strict type matching.
            uint32_t short_ts = (uint32_t)(esp_timer_get_time() & 0xFFFFFFFF);
            int path_len = snprintf(file_path, sizeof(file_path), "%s/%lu.json", CONFIG_SENSOR_AGGREGATOR_CACHE_DIR, (unsigned long)short_ts);

            // Check for truncation
            if (path_len < 0 || (size_t)path_len >= sizeof(file_path))
            {
                ESP_LOGE(TAG, "Failed to construct file path (too long).");
                free(json_string);
                return;
            }

            // Ensure cache directory exists
            // Note: This simple mkdir might not be sufficient for nested dirs on all backends.
            // For SPIFFS, it's not needed. For SD, it is.
            struct stat st;
            if (stat(CONFIG_SENSOR_AGGREGATOR_CACHE_DIR, &st) != 0)
            {
                mkdir(CONFIG_SENSOR_AGGREGATOR_CACHE_DIR, 0755);
            }

            if (storage_api->write_file(file_path, json_string, strlen(json_string)) != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to write buffered report to %s", file_path);
            }
        }
        else
        {
            ESP_LOGE(TAG, "Storage service not found. Cannot buffer report.");
        }
        free(json_string); // Free the string as it's now saved or failed to save
        return;            // Do not proceed to publish
    }

    ESP_LOGI(TAG, "Generated aggregated JSON: %s", json_string);

    // Create and post the event with the generated JSON string
    synapse_telemetry_payload_t *payload = malloc(sizeof(synapse_telemetry_payload_t));
    if (payload)
    {
        strncpy(payload->module_name, self->name, sizeof(payload->module_name) - 1);
        payload->json_data = json_string; // Ownership of json_string is transferred

        event_data_wrapper_t *wrapper;
        if (synapse_event_data_wrap(payload, synapse_telemetry_payload_free, &wrapper) == ESP_OK)
        {
            ESP_LOGI(TAG, "Publishing aggregated report event: %s", SYNAPSE_EVENT_AGGREGATED_SENSOR_REPORT);
            synapse_event_bus_post(SYNAPSE_EVENT_AGGREGATED_SENSOR_REPORT, wrapper);
            synapse_event_data_release(wrapper);
        }
        else
        {
            free(json_string);
            free(payload);
        }
    }
    else
    {
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
        sensor_mapping_t *sensor_map = &private_data->sensors[private_data->num_sensors];
        // Initialize runtime data for this sensor
        memset(&sensor_map->runtime_data, 0, sizeof(sensor_runtime_data_t));
        sensor_map->runtime_data.min = __DBL_MAX__;
        sensor_map->runtime_data.max = -__DBL_MAX__;

        const cJSON *event_name = cJSON_GetObjectItem(sensor_item, "event_name");
        const cJSON *json_key = cJSON_GetObjectItem(sensor_item, "json_key");

        if (cJSON_IsString(event_name) && cJSON_IsString(json_key))
        {
            strncpy(sensor_map->event_name, event_name->valuestring, sizeof(sensor_map->event_name) - 1);
            strncpy(sensor_map->json_key, json_key->valuestring, sizeof(sensor_map->json_key) - 1);

            // Parse aggregation_mode (optional, default is "last")
            sensor_map->mode = AGG_MODE_LAST; // Default
            const cJSON *mode_node = cJSON_GetObjectItem(sensor_item, "aggregation_mode");
            if (cJSON_IsString(mode_node))
            {
                if (strcmp(mode_node->valuestring, "avg") == 0)
                    sensor_map->mode = AGG_MODE_AVG;
                else if (strcmp(mode_node->valuestring, "min") == 0)
                    sensor_map->mode = AGG_MODE_MIN;
                else if (strcmp(mode_node->valuestring, "max") == 0)
                    sensor_map->mode = AGG_MODE_MAX;
            }

            // Parse valid_range (optional)
            sensor_map->has_valid_range = false;
            const cJSON *range_node = cJSON_GetObjectItem(sensor_item, "valid_range");
            if (cJSON_IsObject(range_node))
            {
                const cJSON *min_node = cJSON_GetObjectItem(range_node, "min");
                const cJSON *max_node = cJSON_GetObjectItem(range_node, "max");
                if (cJSON_IsNumber(min_node) && cJSON_IsNumber(max_node))
                {
                    sensor_map->has_valid_range = true;
                    sensor_map->valid_min = min_node->valuedouble;
                    sensor_map->valid_max = max_node->valuedouble;
                }
            }
            private_data->num_sensors++;
        }
    }

    ESP_LOGI(TAG, "Parsed %d sensors to aggregate.", private_data->num_sensors);
    return ESP_OK;
}

/**
 * @internal
 * @brief Reads and publishes all cached reports from the filesystem.
 */
static void publish_buffered_reports(module_t *self)
{
    service_handle_t storage_handle = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_NVRAM_API);
    if (!storage_handle)
    {
        ESP_LOGE(TAG, "Storage service not found. Cannot publish buffered reports.");
        return;
    }
    storage_api_t *storage_api = (storage_api_t *)storage_handle;

    DIR *dir = opendir(CONFIG_SENSOR_AGGREGATOR_CACHE_DIR);
    if (!dir)
    {
        ESP_LOGI(TAG, "Cache directory '%s' not found or empty. Nothing to publish.", CONFIG_SENSOR_AGGREGATOR_CACHE_DIR);
        return;
    }

    ESP_LOGI(TAG, "Found cached reports. Starting to publish...");

    struct dirent *entry;
    // Use a buffer that is guaranteed to be large enough
    char file_path[CONFIG_SENSOR_AGGREGATOR_PATH_BUFFER_SIZE];
    char *file_buffer = malloc(CONFIG_SENSOR_AGGREGATOR_FILE_BUFFER_SIZE);
    if (!file_buffer)
    {
        ESP_LOGE(TAG, "Failed to allocate buffer for reading cached files.");
        closedir(dir);
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            const char *cache_dir = CONFIG_SENSOR_AGGREGATOR_CACHE_DIR;
            const char *filename = entry->d_name;
            size_t cache_dir_len = strlen(cache_dir);
            size_t filename_len = strlen(filename);

            // Check if the path will fit: dir + '/' + file + '\0'
            if (cache_dir_len + 1 + filename_len + 1 > sizeof(file_path))
            {
                ESP_LOGE(TAG, "Constructed file path for '%s' is too long, skipping.", filename);
                continue;
            }

            // Manually and safely construct the path
            strcpy(file_path, cache_dir);
            strcat(file_path, "/");
            strcat(file_path, filename);

            size_t buffer_size = CONFIG_SENSOR_AGGREGATOR_FILE_BUFFER_SIZE;
            if (storage_api->read_file(file_path, file_buffer, &buffer_size) == ESP_OK)
            {
                if (buffer_size < CONFIG_SENSOR_AGGREGATOR_FILE_BUFFER_SIZE)
                {
                    file_buffer[buffer_size] = '\0'; // Ensure null termination
                }
                else
                {
                    file_buffer[CONFIG_SENSOR_AGGREGATOR_FILE_BUFFER_SIZE - 1] = '\0';
                }

                // ... (The rest of the publishing logic remains the same) ...
                synapse_telemetry_payload_t *payload = malloc(sizeof(synapse_telemetry_payload_t));
                if (payload)
                {
                    strncpy(payload->module_name, self->name, sizeof(payload->module_name) - 1);
                    payload->json_data = strdup(file_buffer);

                    event_data_wrapper_t *wrapper;
                    if (payload->json_data && synapse_event_data_wrap(payload, synapse_telemetry_payload_free, &wrapper) == ESP_OK)
                    {
                        ESP_LOGI(TAG, "Publishing buffered report from %s", file_path);
                        synapse_event_bus_post(SYNAPSE_EVENT_AGGREGATED_SENSOR_REPORT, wrapper);
                        synapse_event_data_release(wrapper);
                        storage_api->delete_file(file_path);
                    }
                    else
                    {
                        if (payload->json_data)
                            free(payload->json_data);
                        free(payload);
                    }
                }
            }
            else
            {
                ESP_LOGE(TAG, "Failed to read buffered report: %s", file_path);
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }

    free(file_buffer);
    closedir(dir);
}