/**
 * @file sensor_simulator.c
 * @brief Simulates sensor data for testing purposes.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @details Sensor Simulator მოდულის იმპლემენტაცია Synapse Framework-ისთვის.
 *          აღზევს ყველა საჭირო base_module interface ფუნქციას.
 */

#include "synapse.h"
#include "sensor_simulator.h"
#include "timer_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DEFINE_COMPONENT_TAG("SENSOR_SIM");

#define EVT_SIM_TICK "SIMULATOR_TICK"
#define SENSOR_DATA_TEMPERATURE "SENSOR_DATA_TEMPERATURE"
#define SENSOR_DATA_HUMIDITY "SENSOR_DATA_HUMIDITY"
#define SENSOR_DATA_LIGHT "SENSOR_DATA_LIGHT"

/**
 * @brief Sensor Simulator მოდულის private მონაცემების სტრუქტურა
 * @details შეიცავს მოდულის შიდა მდგომარეობას და კონფიგურაციას
 */
typedef struct {
    bool enabled;                                           /**< ჩართვის ფლაგი */
    char instance_name[CONFIG_SENSOR_SIMULATOR_INSTANCE_NAME_MAX_LEN]; /**< მოდულის ინსტანციის სახელი */
    // TODO: დაამატეთ მოდულის სპეციფიკური ველები
} sensor_simulator_private_data_t;

// --- Forward declarations ---
static esp_err_t sensor_simulator_init(module_t *self);
static esp_err_t sensor_simulator_start(module_t *self);
static esp_err_t sensor_simulator_enable(module_t *self);
static esp_err_t sensor_simulator_disable(module_t *self);
static void sensor_simulator_deinit(module_t *self);
static esp_err_t sensor_simulator_reconfigure(module_t *self, const cJSON *new_config);
static module_status_t sensor_simulator_get_status(module_t *self);
static void sensor_simulator_handle_event(module_t *self, const char *event_name, void *event_data);

module_t *sensor_simulator_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating sensor_simulator module instance");
    
    // გამოვყოთ მეხსიერება module_t სტრუქტურისთვის
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module) {
        ESP_LOGE(TAG, "Failed to allocate memory for module");
        return NULL;
    }
    
    // გამოვყოთ მეხსიერება private data-სთვის
    sensor_simulator_private_data_t *private_data = (sensor_simulator_private_data_t *)calloc(1, sizeof(sensor_simulator_private_data_t));
    if (!private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory for private data");
        free(module);
        return NULL;
    }
    
    // შევქმნათ state mutex
    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex) {
        ESP_LOGE(TAG, "Failed to create state mutex");
        free(private_data);
        free(module);
        return NULL;
    }
    
    // დავაკავშიროთ private data
    module->private_data = private_data;
    
    // დავაყენოთ default კონფიგურაცია
    const char *instance_name = CONFIG_SENSOR_SIMULATOR_DEFAULT_INSTANCE_NAME;
    if (config) {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node)) {
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node) && name_node->valuestring) {
                instance_name = name_node->valuestring;
            }
        }
        module->current_config = cJSON_Duplicate(config, true);
    }
    
    // ინიციალიზაცია
    private_data->enabled = true;
    strncpy(private_data->instance_name, instance_name, CONFIG_SENSOR_SIMULATOR_INSTANCE_NAME_MAX_LEN - 1);
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;
    
    // დავაყენოთ ფუნქციების pointers
    module->init_level = 100;  // TODO:შეცვალე საჭირო ლეველზე
    module->base.init = sensor_simulator_init;
    module->base.start = sensor_simulator_start;
    module->base.handle_event = sensor_simulator_handle_event;
    module->base.deinit = sensor_simulator_deinit;
    module->base.enable = sensor_simulator_enable;
    module->base.disable = sensor_simulator_disable;
    module->base.reconfigure = sensor_simulator_reconfigure;
    module->base.get_status = sensor_simulator_get_status;
    
    ESP_LOGI(TAG, "Sensor Simulator module created: '%s'", instance_name);
    return module;
}

static esp_err_t sensor_simulator_init(module_t *self)
{
    if (!self) {
        return ESP_ERR_INVALID_ARG;
    }
    
    ESP_LOGI(TAG, "Initializing sensor_simulator module: %s", self->name);

    // TODO: დაამატეთ ინიციალიზაციის ლოგიკა
    // მაგალითად, Event Bus-ზე გამოწერა:
    // esp_err_t ret = synapse_event_bus_subscribe("some_event", self);

    self->status = MODULE_STATUS_INITIALIZED;

    synapse_event_bus_subscribe(EVT_SIM_TICK, self);

    ESP_LOGI(TAG, "Sensor Simulator module initialized successfully");
    return ESP_OK;
}

static esp_err_t sensor_simulator_start(module_t *self)
{
    if (!self) {
        return ESP_ERR_INVALID_ARG;
    }
    
    sensor_simulator_private_data_t *private_data = (sensor_simulator_private_data_t *)self->private_data;
    
    if (self->status != MODULE_STATUS_INITIALIZED) {
        ESP_LOGE(TAG, "Cannot start uninitialized module");
        return ESP_ERR_INVALID_STATE;
    }
    
    if (self->status == MODULE_STATUS_RUNNING) {
        ESP_LOGW(TAG, "Module already running");
        return ESP_OK;
    }
    
    ESP_LOGI(TAG, "Starting sensor_simulator module: %s", self->name);
    
    // TODO: Implement module start logic
    
    self->status = MODULE_STATUS_RUNNING;
    private_data->enabled = true;

    service_handle_t timer_service = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_TIMER_API);
    if (timer_service) {
        ((timer_api_t *)timer_service)->schedule_event(EVT_SIM_TICK, 15000, true); // ყოველ 15 წამში
        ESP_LOGI(TAG, "Sensor simulation scheduled.");
    }
    
    ESP_LOGI(TAG, "Sensor Simulator module started successfully");
    return ESP_OK;
}

static esp_err_t sensor_simulator_enable(module_t *self)
{
    if (!self) {
        return ESP_ERR_INVALID_ARG;
    }
    
    sensor_simulator_private_data_t *private_data = (sensor_simulator_private_data_t *)self->private_data;
    
    ESP_LOGI(TAG, "Enabling sensor_simulator module: %s", self->name);
    
    if (private_data->enabled) {
        ESP_LOGW(TAG, "Module already enabled");
        return ESP_OK;
    }
    
    // TODO: Implement enable logic
    
    private_data->enabled = true;
    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "Sensor Simulator module enabled");
    
    return ESP_OK;
}

static esp_err_t sensor_simulator_disable(module_t *self)
{
    if (!self) {
        return ESP_ERR_INVALID_ARG;
    }
    
    sensor_simulator_private_data_t *private_data = (sensor_simulator_private_data_t *)self->private_data;
    
    ESP_LOGI(TAG, "Disabling sensor_simulator module: %s", self->name);
    
    if (!private_data->enabled) {
        ESP_LOGW(TAG, "Module already disabled");
        return ESP_OK;
    }
    
    // TODO: Implement disable logic
    
    private_data->enabled = false;
    self->status = MODULE_STATUS_DISABLED;
    ESP_LOGI(TAG, "Sensor Simulator module disabled");
    
    return ESP_OK;
}

static esp_err_t sensor_simulator_reconfigure(module_t *self, const cJSON *new_config)
{
    if (!self || !new_config) {
        return ESP_ERR_INVALID_ARG;
    }
    
    ESP_LOGI(TAG, "Reconfiguring sensor_simulator module: %s", self->name);
    
    // TODO: Implement reconfiguration logic
    // Update self->current_config with new_config
    
    if (self->current_config) {
        cJSON_Delete(self->current_config);
    }
    self->current_config = cJSON_Duplicate(new_config, true);
    
    ESP_LOGI(TAG, "Sensor Simulator module reconfigured");
    return ESP_OK;
}

static module_status_t sensor_simulator_get_status(module_t *self)
{
    if (!self) {
        return MODULE_STATUS_ERROR;
    }
    
    return self->status;
}

static void sensor_simulator_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (!self || !self->private_data) {
        if (event_data) {
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        }
        return;
    }
    
    sensor_simulator_private_data_t *private_data = (sensor_simulator_private_data_t *)self->private_data;
    
    if (!private_data->enabled) {
        if (event_data) {
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        }
        return;
    }
    
    if (strcmp(event_name, EVT_SIM_TICK) == 0) {
        char payload_str[32];
        event_data_wrapper_t *wrapper;

        // --- Publish Temperature ---
        float temp = 20.0 + (rand() % 50) / 10.0; // 20.0 - 24.9
        snprintf(payload_str, sizeof(payload_str), "{\"value\":%.2f}", temp);
        synapse_event_data_wrap(strdup(payload_str), free, &wrapper);
        synapse_event_bus_post(SENSOR_DATA_TEMPERATURE, wrapper);
        synapse_event_data_release(wrapper);
        ESP_LOGI(TAG, "Published Temp: %s", payload_str);

        // --- Publish Humidity ---
        float hum = 40.0 + (rand() % 200) / 10.0; // 40.0 - 59.9
        snprintf(payload_str, sizeof(payload_str), "{\"value\":%.2f}", hum);
        synapse_event_data_wrap(strdup(payload_str), free, &wrapper);
        synapse_event_bus_post(SENSOR_DATA_HUMIDITY, wrapper);
        synapse_event_data_release(wrapper);
        ESP_LOGI(TAG, "Published Hum: %s", payload_str);

        // --- Publish Light ---
        int light = 100 + (rand() % 500); // 100 - 599
        snprintf(payload_str, sizeof(payload_str), "{\"value\":%d}", light);
        synapse_event_data_wrap(strdup(payload_str), free, &wrapper);
        synapse_event_bus_post(SENSOR_DATA_LIGHT, wrapper);
        synapse_event_data_release(wrapper);
        ESP_LOGI(TAG, "Published Light: %s", payload_str);
    }
    
    // Always release event data
    if (event_data) {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

static void sensor_simulator_deinit(module_t *self)
{
    if (!self) {
        return;
    }
    
    ESP_LOGI(TAG, "Deinitializing %s module", self->name);

    // TODO: Unsubscribe from events if needed
    // synapse_event_bus_unsubscribe("some_event", self);

    if (self->private_data) {
        free(self->private_data);
    }
    
    if (self->current_config) {
        cJSON_Delete(self->current_config);
    }
    
    if (self->state_mutex) {
        vSemaphoreDelete(self->state_mutex);
    }
    
    free(self);
    
    ESP_LOGI(TAG, "Module deinitialized successfully");
}
