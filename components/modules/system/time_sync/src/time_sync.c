/**
 * @file time_sync.c
 * @brief Mock implementation of the Time Sync module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-08-02
 */
#include "time_sync.h"
#include "logging.h"
#include "service_locator.h"
#include "time_sync_interface.h"

DEFINE_COMPONENT_TAG("TIME_SYNC");

typedef struct {
    module_t *module;
    time_sync_api_t service_api;
} time_sync_private_data_t;

// --- API Function (Mock) ---
static esp_err_t mock_get_time(time_t *current_time) {
    if (current_time) {
        // Return a fixed time for demonstration
        *current_time = 1767209640; // 2025-12-31 14:34:00 UTC
    }
    return ESP_OK;
}

// --- Lifecycle Functions ---
static esp_err_t time_sync_init(module_t *self) {
    time_sync_private_data_t *private_data = (time_sync_private_data_t *)self->private_data;
    
    private_data->service_api.get_time = mock_get_time;

    esp_err_t err = fmw_service_register(self->name, FMW_SERVICE_TYPE_TIME_SYNC_API, &private_data->service_api);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register time_sync service!");
        return err;
    }
    
    ESP_LOGI(TAG, "Mock Time Sync service registered successfully.");
    return ESP_OK;
}

static void time_sync_deinit(module_t *self) {
    if (!self) return;
    fmw_service_unregister(self->name);
    if (self->private_data) free(self->private_data);
    if (self->current_config) cJSON_Delete(self->current_config);
    free(self);
}

module_t* time_sync_create(const cJSON *config) {
    module_t *module = calloc(1, sizeof(module_t));
    time_sync_private_data_t *private_data = calloc(1, sizeof(time_sync_private_data_t));
    if (!module || !private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module); free(private_data);
        if (config) cJSON_Delete((cJSON*)config);
        return NULL;
    }

    module->private_data = private_data;
    module->current_config = (cJSON*)config;
    private_data->module = module;
    module->init_level = 40;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    snprintf(module->name, sizeof(module->name), "%s", cJSON_GetObjectItem(config_node, "instance_name")->valuestring);

    module->base.init = time_sync_init;
    module->base.deinit = time_sync_deinit;

    return module;
}