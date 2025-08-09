/**
 * @file time_sync.c
 * @brief SNTP-based implementation of the Time Sync module.
 * @author Synapse Framework Team
 * @version 2.0.0
 * @date 2025-08-15
 */

#include "synapse.h"
#include "time_sync.h"
#include "time_sync_interface.h"
#include "esp_sntp.h"
#include "esp_netif.h"
#include <time.h>

DEFINE_COMPONENT_TAG("TIME_SYNC", SYNAPSE_LOG_COLOR_BLUE);

#define EVT_WIFI_IP_ASSIGNED "WIFI_EVENT_IP_ASSIGNED"

typedef struct {
    module_t *module;
    time_sync_api_t service_api;
    bool is_synced;
} time_sync_private_data_t;

// --- Forward Declarations ---
static esp_err_t time_sync_init(module_t *self);
static void time_sync_deinit(module_t *self);
static void time_sync_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t api_get_time(time_t *current_time);
static void time_sync_notification_cb(struct timeval *tv);
static void start_sntp_sync(void);

// --- Module Create & Deinit ---
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
    module->current_config = cJSON_Duplicate(config, true);
    if (!module->current_config)
    {
        ESP_LOGE(TAG, "Failed to duplicate configuration object.");
        // Note: This assumes 'private_data' and 'module' are allocated.
        // Manual check might be needed for each file's cleanup logic.
        free(private_data);
        free(module);
        return NULL;
    }
    private_data->module = module;
    module->init_level = 40;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    snprintf(module->name, sizeof(module->name), "%s", cJSON_GetObjectItem(config_node, "instance_name")->valuestring);

    module->base.init = time_sync_init;
    module->base.deinit = time_sync_deinit;
    module->base.handle_event = time_sync_handle_event;

    private_data->is_synced = false;
    private_data->service_api.get_time = api_get_time;

    return module;
}

static void time_sync_deinit(module_t *self)
{
    if (!self)
        return;
    ESP_LOGI(TAG, "Deinitializing Time Sync module.");
    esp_sntp_stop();
    synapse_service_unregister(self->name);
    synapse_event_bus_unsubscribe(EVT_WIFI_IP_ASSIGNED, self);
    if (self->private_data)
        free(self->private_data);
    if (self->current_config)
        cJSON_Delete(self->current_config);
}

// --- Module Lifecycle & Event Handling ---
static esp_err_t time_sync_init(module_t *self)
{
    time_sync_private_data_t *private_data = (time_sync_private_data_t *)self->private_data;

    esp_err_t err = synapse_service_register(self->name, SYNAPSE_SERVICE_TYPE_TIME_SYNC_API, &private_data->service_api);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register time_sync service!");
        return err;
    }

    synapse_event_bus_subscribe(EVT_WIFI_IP_ASSIGNED, self);

    // Set timezone from config
    const cJSON *config_node = cJSON_GetObjectItem(self->current_config, "config");
    const cJSON *tz_node = cJSON_GetObjectItem(config_node, "timezone");
    if (cJSON_IsString(tz_node) && tz_node->valuestring != NULL)
    {
        setenv("TZ", tz_node->valuestring, 1);
        tzset();
        ESP_LOGI(TAG, "Timezone set to: %s", tz_node->valuestring);
    }
    else
    {
        ESP_LOGW(TAG, "Timezone not found in config. Using default UTC.");
    }

    ESP_LOGI(TAG, "Time Sync service registered. Waiting for IP address.");
    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

static void time_sync_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (strcmp(event_name, EVT_WIFI_IP_ASSIGNED) == 0)
    {
        ESP_LOGI(TAG, "IP address obtained. Starting SNTP synchronization.");
        start_sntp_sync();
        self->status = MODULE_STATUS_RUNNING;
    }

    if (event_data)
    {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// --- Service API Implementation ---
static esp_err_t api_get_time(time_t *current_time)
{
    // This function is called from other modules, so we need to find our own private_data.
    module_t *self = synapse_module_registry_find_by_name("main_time_sync"); // Assuming this instance name
    if (!self)
        return ESP_ERR_NOT_FOUND;

    time_sync_private_data_t *private_data = (time_sync_private_data_t *)self->private_data;
    if (!private_data->is_synced)
    {
        return ESP_ERR_INVALID_STATE; // Not synced yet
    }

    time(current_time);
    return ESP_OK;
}

// --- SNTP Logic ---
static void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Time successfully synchronized with NTP server.");

    module_t *self = synapse_module_registry_find_by_name("main_time_sync");
    if (self)
    {
        time_sync_private_data_t *private_data = (time_sync_private_data_t *)self->private_data;
        private_data->is_synced = true;
    }
}

static void start_sntp_sync(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    esp_sntp_init();
}