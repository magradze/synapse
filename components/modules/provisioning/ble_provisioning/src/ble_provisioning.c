/**
 * @file ble_provisioning.c
 * @brief ESP-IDF native BLE provisioning implementation
 * @author Synapse Framework Team
 * @version 2.0.0
 */

#include "ble_provisioning.h"
#include "base_module.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "framework_config.h"
#include "logging.h"

#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include <wifi_provisioning/manager.h>
#include <wifi_provisioning/scheme_ble.h>
#include <protocomm_security0.h>
#include <protocomm_security1.h>

DEFINE_COMPONENT_TAG("BLE_PROV");

// Event names
#define EVT_PROV_CREDENTIALS_RECEIVED "PROV_CREDENTIALS_RECEIVED"
#define EVT_WIFI_CONNECTED "WIFI_EVENT_CONNECTED"
#define EVT_WIFI_DISCONNECTED "WIFI_EVENT_DISCONNECTED"
#define EVT_WIFI_IP_ASSIGNED "WIFI_EVENT_IP_ASSIGNED"
#define EVT_PROV_STARTED "PROV_STARTED"
#define EVT_PROV_START_REQUESTED "PROV_START_REQUESTED"

// Private data structure
typedef struct
{
    bool enabled;
    char instance_name[64];
    bool provisioning_active;
    char device_name[32];
    char pop[9]; // Proof of possession
    int security_version;
} ble_provisioning_private_data_t;

// Forward declarations
static esp_err_t ble_provisioning_init(module_t *self);
static esp_err_t ble_provisioning_start(module_t *self);
static esp_err_t ble_provisioning_enable(module_t *self);
static esp_err_t ble_provisioning_disable(module_t *self);
static esp_err_t ble_provisioning_reconfigure(module_t *self, const cJSON *new_config);
static module_status_t ble_provisioning_get_status(module_t *self);
static void ble_provisioning_deinit(module_t *self);
static void ble_provisioning_handle_event(module_t *self, const char *event_name, void *event_data);

// Provisioning event handler
static void prov_event_handler(void *handler_arg, wifi_prov_cb_event_t event, void *event_data)
{
    module_t *self = (module_t *)handler_arg;
    ble_provisioning_private_data_t *private_data =
        (ble_provisioning_private_data_t *)self->private_data;

    switch (event)
    {
    case WIFI_PROV_INIT:
        ESP_LOGI(TAG, "Provisioning initialized");
        break;

    case WIFI_PROV_START:
        ESP_LOGI(TAG, "Provisioning started");
        private_data->provisioning_active = true;
        fmw_event_bus_post(EVT_PROV_STARTED, NULL);
        break;

    case WIFI_PROV_CRED_RECV:
    {
        wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
        ESP_LOGI(TAG, "Received Wi-Fi credentials: SSID: %s", (char *)wifi_sta_cfg->ssid);

        // ★★★ ვიყენებთ პირდაპირ კასტინგს, რადგან cJSON ამას უმკლავდება ★★★
        cJSON *creds = cJSON_CreateObject();
        cJSON_AddStringToObject(creds, "ssid", (char *)wifi_sta_cfg->ssid);
        cJSON_AddStringToObject(creds, "password", (char *)wifi_sta_cfg->password);

        char *json_str = cJSON_PrintUnformatted(creds);
        if (json_str)
        {
            ESP_LOGI(TAG, "Publishing credentials JSON: %s", json_str);
            event_data_wrapper_t *wrapper = NULL;
            if (fmw_event_data_wrap(json_str, free, &wrapper) == ESP_OK)
            {
                fmw_event_bus_post(EVT_PROV_CREDENTIALS_RECEIVED, wrapper);
            }
            else
            {
                free(json_str);
            }
        }
        cJSON_Delete(creds);
        break;
    }

    case WIFI_PROV_CRED_FAIL:
    {
        wifi_prov_sta_fail_reason_t *reason = (wifi_prov_sta_fail_reason_t *)event_data;
        ESP_LOGE(TAG, "Provisioning failed! reason: %d", *reason);
        break;
    }

    case WIFI_PROV_CRED_SUCCESS:
        ESP_LOGI(TAG, "Provisioning successful");
        break;

    case WIFI_PROV_END:
        ESP_LOGI(TAG, "Provisioning ended");
        private_data->provisioning_active = false;
        wifi_prov_mgr_deinit();
        break;

    default:
        break;
    }
}

module_t *ble_provisioning_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating ble_provisioning module instance");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for module");
        return NULL;
    }

    ble_provisioning_private_data_t *private_data =
        (ble_provisioning_private_data_t *)calloc(1, sizeof(ble_provisioning_private_data_t));
    if (!private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for private data");
        free(module);
        return NULL;
    }

    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex)
    {
        ESP_LOGE(TAG, "Failed to create state mutex");
        free(private_data);
        free(module);
        return NULL;
    }

    module->private_data = private_data;

    // Set default configuration
    private_data->enabled = true;
    strncpy(private_data->device_name, CONFIG_ESP_PROV_DEVICE_NAME, sizeof(private_data->device_name) - 1);
    strncpy(private_data->pop, CONFIG_ESP_PROV_POP, sizeof(private_data->pop) - 1);
    private_data->security_version = CONFIG_ESP_PROV_SECURITY_VERSION;

    const char *instance_name = "ble_provisioning";
    if (config)
    {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node))
        {
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node) && name_node->valuestring)
            {
                instance_name = name_node->valuestring;
            }

            // Override device name if provided
            const cJSON *device_name_node = cJSON_GetObjectItem(config_node, "device_name");
            if (cJSON_IsString(device_name_node) && device_name_node->valuestring)
            {
                strncpy(private_data->device_name, device_name_node->valuestring,
                        sizeof(private_data->device_name) - 1);
            }

            // Override POP if provided
            const cJSON *pop_node = cJSON_GetObjectItem(config_node, "pop");
            if (cJSON_IsString(pop_node) && pop_node->valuestring)
            {
                strncpy(private_data->pop, pop_node->valuestring,
                        sizeof(private_data->pop) - 1);
            }
        }
        module->current_config = cJSON_Duplicate(config, true);
    }

    strncpy(private_data->instance_name, instance_name, sizeof(private_data->instance_name) - 1);
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;

    // Set function pointers
    module->init_level = 2;
    module->base.init = ble_provisioning_init;
    module->base.start = ble_provisioning_start;
    module->base.handle_event = ble_provisioning_handle_event;
    module->base.deinit = ble_provisioning_deinit;
    module->base.enable = ble_provisioning_enable;
    module->base.disable = ble_provisioning_disable;
    module->base.reconfigure = ble_provisioning_reconfigure;
    module->base.get_status = ble_provisioning_get_status;

    ESP_LOGI(TAG, "BLE Provisioning module created: '%s'", instance_name);
    return module;
}

static esp_err_t ble_provisioning_init(module_t *self)
{
    if (!self || !self->private_data)
        return ESP_ERR_INVALID_ARG;

    ESP_LOGI(TAG, "Initializing ble_provisioning module: %s", self->name);

    // Subscribe to WiFi events
    esp_err_t ret = fmw_event_bus_subscribe(EVT_WIFI_CONNECTED, self);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to subscribe to WiFi connected event");
    }

    ret = fmw_event_bus_subscribe(EVT_WIFI_IP_ASSIGNED, self);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to subscribe to WiFi IP assigned event");
    }

    ret = fmw_event_bus_subscribe(EVT_WIFI_DISCONNECTED, self);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to subscribe to WiFi disconnected event");
    }

    ret = fmw_event_bus_subscribe(EVT_PROV_START_REQUESTED, self);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to subscribe to provisioning start request event");
    }

    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "BLE Provisioning module initialized successfully");
    return ESP_OK;
}

static esp_err_t ble_provisioning_start(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;

    ble_provisioning_private_data_t *private_data = (ble_provisioning_private_data_t *)self->private_data;

    if (self->status != MODULE_STATUS_INITIALIZED)
    {
        ESP_LOGE(TAG, "Cannot start uninitialized module");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "Starting ble_provisioning module: %s", self->name);

    // Check if already provisioned
    bool provisioned = false;
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));

    if (provisioned)
    {
        ESP_LOGI(TAG, "Already provisioned, starting WiFi");
        wifi_prov_mgr_deinit();
        self->status = MODULE_STATUS_RUNNING;
        return ESP_OK;
    }

    // Initialize provisioning manager
    wifi_prov_mgr_config_t config = {
        .scheme = wifi_prov_scheme_ble,
        .scheme_event_handler = WIFI_PROV_SCHEME_BLE_EVENT_HANDLER_FREE_BTDM,
        .app_event_handler = {
            .event_cb = prov_event_handler,
            .user_data = self}};

    ESP_ERROR_CHECK(wifi_prov_mgr_init(config));

    // Start provisioning
    const char *service_name = private_data->device_name;
    const char *pop = private_data->pop;
    wifi_prov_security_t security = private_data->security_version == 0 ? WIFI_PROV_SECURITY_0 : WIFI_PROV_SECURITY_1;

    ESP_LOGI(TAG, "Starting provisioning with:");
    ESP_LOGI(TAG, "  Device Name: %s", service_name);
    ESP_LOGI(TAG, "  Security: v%d", private_data->security_version);
    if (security == WIFI_PROV_SECURITY_1)
    {
        ESP_LOGI(TAG, "  POP: %s", pop);
    }

    ESP_ERROR_CHECK(wifi_prov_mgr_start_provisioning(security, pop, service_name, NULL));

    self->status = MODULE_STATUS_RUNNING;
    private_data->enabled = true;

    ESP_LOGI(TAG, "BLE Provisioning module started successfully");
    return ESP_OK;
}

static esp_err_t ble_provisioning_disable(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;

    ble_provisioning_private_data_t *private_data = (ble_provisioning_private_data_t *)self->private_data;

    ESP_LOGI(TAG, "Disabling ble_provisioning module: %s", self->name);

    if (private_data->provisioning_active)
    {
        wifi_prov_mgr_stop_provisioning();
    }

    private_data->enabled = false;
    self->status = MODULE_STATUS_DISABLED;

    ESP_LOGI(TAG, "BLE Provisioning module disabled");
    return ESP_OK;
}

static esp_err_t ble_provisioning_enable(module_t *self)
{
    return ble_provisioning_start(self);
}

static esp_err_t ble_provisioning_reconfigure(module_t *self, const cJSON *new_config)
{
    if (!self || !new_config)
        return ESP_ERR_INVALID_ARG;

    ESP_LOGI(TAG, "Reconfiguring ble_provisioning module: %s", self->name);

    if (self->current_config)
    {
        cJSON_Delete(self->current_config);
    }
    self->current_config = cJSON_Duplicate(new_config, true);

    ESP_LOGI(TAG, "BLE Provisioning module reconfigured");
    return ESP_OK;
}

static module_status_t ble_provisioning_get_status(module_t *self)
{
    if (!self)
        return MODULE_STATUS_ERROR;
    return self->status;
}

static void ble_provisioning_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (!self || !self->private_data)
    {
        if (event_data)
        {
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        }
        return;
    }

    ble_provisioning_private_data_t *private_data = (ble_provisioning_private_data_t *)self->private_data;

    if (!private_data->enabled)
    {
        if (event_data)
        {
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        }
        return;
    }

    if (event_name)
    {
        ESP_LOGD(TAG, "[%s] Event received: '%s'", private_data->instance_name, event_name);

        if (strcmp(event_name, EVT_PROV_START_REQUESTED) == 0)
        {
            ESP_LOGW(TAG, "Provisioning start requested. Re-initializing provisioning manager...");

            // თუ provisioning უკვე აქტიურია, ჯერ გავაჩეროთ
            if (private_data->provisioning_active)
            {
                wifi_prov_mgr_stop_provisioning();
                private_data->provisioning_active = false;
            }
            // დეინიციალიზაცია, რათა სუფთად დავიწყოთ
            wifi_prov_mgr_deinit();

            // 1. ხელახლა დავაკონფიგურიროთ და დავაინიციალიზოთ provisioning manager-ი
            wifi_prov_mgr_config_t config = {
                .scheme = wifi_prov_scheme_ble,
                .scheme_event_handler = WIFI_PROV_SCHEME_BLE_EVENT_HANDLER_FREE_BTDM,
                .app_event_handler = {
                    .event_cb = prov_event_handler,
                    .user_data = self}};
            ESP_ERROR_CHECK(wifi_prov_mgr_init(config));

            // 2. დავიწყოთ provisioning პროცესი
            const char *service_name = private_data->device_name;
            const char *pop = private_data->pop;
            wifi_prov_security_t security = private_data->security_version == 0 ? WIFI_PROV_SECURITY_0 : WIFI_PROV_SECURITY_1;

            ESP_LOGI(TAG, "Starting provisioning with:");
            ESP_LOGI(TAG, "  Device Name: %s", service_name);
            ESP_LOGI(TAG, "  Security: v%d", private_data->security_version);
            if (security == WIFI_PROV_SECURITY_1)
            {
                ESP_LOGI(TAG, "  POP: %s", pop);
            }
            ESP_ERROR_CHECK(wifi_prov_mgr_start_provisioning(security, pop, service_name, NULL));

            // 3. განვაახლოთ მოდულის სტატუსი
            self->status = MODULE_STATUS_RUNNING;
        }

        if (strcmp(event_name, EVT_WIFI_CONNECTED) == 0)
        {
            ESP_LOGI(TAG, "WiFi connected, stopping provisioning");
            // if (private_data->provisioning_active)
            // {
            //     wifi_prov_mgr_stop_provisioning();
            // }
        }

        if (strcmp(event_name, EVT_WIFI_IP_ASSIGNED) == 0)
        {
            ESP_LOGI(TAG, "Provisioning: Got IP assigned, finishing BLE provisioning");
            // if (private_data->provisioning_active)
            // {
            //     wifi_prov_mgr_stop_provisioning();
            // }
        }
    }

    // Always release event data
    if (event_data)
    {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

static void ble_provisioning_deinit(module_t *self)
{
    if (!self)
        return;

    ESP_LOGI(TAG, "Deinitializing %s module", self->name);
    ble_provisioning_private_data_t *private_data = (ble_provisioning_private_data_t *)self->private_data;

    // Unsubscribe from events
    fmw_event_bus_unsubscribe(EVT_WIFI_CONNECTED, self);
    fmw_event_bus_unsubscribe(EVT_WIFI_DISCONNECTED, self);
    fmw_event_bus_unsubscribe(EVT_PROV_START_REQUESTED, self);

    // Stop provisioning if active
    if (private_data->provisioning_active)
    {
        wifi_prov_mgr_stop_provisioning();
        wifi_prov_mgr_deinit();
    }

    // Free resources
    if (self->private_data)
    {
        free(self->private_data);
    }

    if (self->current_config)
    {
        cJSON_Delete(self->current_config);
    }

    if (self->state_mutex)
    {
        vSemaphoreDelete(self->state_mutex);
    }

    free(self);

    ESP_LOGI(TAG, "Module deinitialized successfully");
}