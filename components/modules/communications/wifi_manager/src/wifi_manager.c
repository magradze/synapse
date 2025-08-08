/**
 * @file wifi_manager.c
 * @brief Main lifecycle and task management for the WiFi Manager module.
 * @author Synapse Framework Team
 * @version 4.1.0
 * @date 2025-09-02
 */
#include "wifi_manager_internal.h"

DEFINE_COMPONENT_TAG("WIFI_MANAGER");

// --- Forward Declarations for static functions in this file ---
static esp_err_t wifi_manager_init(module_t *self);
static esp_err_t wifi_manager_start(module_t *self);
static void wifi_manager_deinit(module_t *self);

// --- Dependency Map ---
static const module_dependency_t s_dependencies[] = {
    {"storage_service", offsetof(wifi_manager_private_data_t, storage_handle)},
    {NULL, 0} // Terminator
};

// --- Factory Function ---
module_t *wifi_manager_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)calloc(1, sizeof(wifi_manager_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        if (config)
            cJSON_Delete((cJSON *)config);
        return NULL;
    }

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
    module->private_data = private_data;
    module->dependency_map = s_dependencies;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    snprintf(module->name, sizeof(module->name), "%s", name_node->valuestring);
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", name_node->valuestring);

    private_data->service_api.get_status_async = wifi_api_get_status_async;
    private_data->service_api.is_connected = wifi_api_is_connected;
    synapse_service_register(module->name, SYNAPSE_SERVICE_TYPE_WIFI_API, &private_data->service_api);

    module->init_level = 40;
    module->base.init = wifi_manager_init;
    module->base.start = wifi_manager_start;
    module->base.deinit = wifi_manager_deinit;
    module->base.handle_event = wifi_manager_handle_event;

    ESP_LOGI(TAG, "WiFi Manager module created: '%s'", module->name);
    return module;
}

// --- Lifecycle Functions ---
static esp_err_t wifi_manager_init(module_t *self)
{
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing wifi_manager module: %s", self->name);

    if (!private_data->storage_handle)
    {
        ESP_LOGE(TAG, "Dependency injection failed: storage_handle is NULL!");
        return ESP_ERR_INVALID_STATE;
    }

    private_data->cmd_queue = xQueueCreate(5, sizeof(wifi_queue_msg_t));
    if (!private_data->cmd_queue)
    {
        ESP_LOGE(TAG, "Failed to create command queue");
        return ESP_ERR_NO_MEM;
    }

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, self));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler, self));

    private_data->reconnect_timer = xTimerCreate("wifi_reconnect", pdMS_TO_TICKS(5000), pdFALSE, self, reconnect_timer_callback);

    synapse_event_bus_subscribe("PROV_CREDENTIALS_RECEIVED", self);
    synapse_event_bus_subscribe(SYNAPSE_EVENT_SYSTEM_START_COMPLETE, self);

    if (load_credentials(self) != ESP_OK)
    {
        ESP_LOGI(TAG, "No saved credentials found. Waiting for provisioning...");
    }

    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "WiFi Manager module initialized successfully");
    return ESP_OK;
}

static esp_err_t wifi_manager_start(module_t *self)
{
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting wifi_manager module: %s", self->name);

    BaseType_t ret = xTaskCreate(wifi_task, self->name, 4096, self, 12, &private_data->task_handle);
    if (ret != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create wifi task");
        return ESP_FAIL;
    }

    if (private_data->has_saved_credentials)
    {
        ESP_LOGI(TAG, "Found saved credentials. Requesting connection...");
        start_wifi_connection(self);
    }
    else
    {
        ESP_LOGI(TAG, "No credentials. Waiting for provisioning.");
        synapse_event_bus_post("WIFI_CREDENTIALS_NOT_FOUND", NULL);
    }

    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "WiFi Manager module started successfully");
    return ESP_OK;
}

static void wifi_manager_deinit(module_t *self)
{
    if (!self)
        return;
    ESP_LOGI(TAG, "Deinitializing %s module", self->name);
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    if (private_data)
    {
        if (private_data->task_handle)
            vTaskDelete(private_data->task_handle);
        if (private_data->cmd_queue)
            vQueueDelete(private_data->cmd_queue);
        if (private_data->reconnect_timer)
            xTimerDelete(private_data->reconnect_timer, portMAX_DELAY);
        free(private_data);
    }

    synapse_event_bus_unsubscribe("PROV_CREDENTIALS_RECEIVED", self);
    synapse_event_bus_unsubscribe(SYNAPSE_EVENT_SYSTEM_START_COMPLETE, self);

    esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler);
    esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler);

    esp_wifi_stop();
    esp_wifi_deinit();

    if (self->current_config)
        cJSON_Delete(self->current_config);

    ESP_LOGI(TAG, "Module deinitialized successfully");
}

// --- Main Task ---
void wifi_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    wifi_queue_msg_t msg;
    static char status_json_buffer[512];

    while (1)
    {
        if (xQueueReceive(private_data->cmd_queue, &msg, portMAX_DELAY) == pdPASS)
        {
            switch (msg.type)
            {
            case WIFI_CMD_CONNECT:
                ESP_LOGI(TAG, "TASK: Received CONNECT command.");
                ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
                ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &private_data->wifi_config));
                ESP_ERROR_CHECK(esp_wifi_start());
                break;
            case WIFI_CMD_DISCONNECT:
                ESP_LOGI(TAG, "TASK: Received DISCONNECT command.");
                esp_wifi_disconnect();
                break;
            case WIFI_CMD_RECONNECT_TIMER_FIRED:
                ESP_LOGI(TAG, "TASK: Received RECONNECT command from timer.");
                esp_wifi_connect();
                break;
            case WIFI_CMD_GET_STATUS_ASYNC:
            {
                ESP_LOGD(TAG, "TASK: Received GET_STATUS_ASYNC command.");
                cJSON *status_json = build_status_json(self);
                if (status_json)
                {
                    if (cJSON_PrintPreallocated(status_json, status_json_buffer, sizeof(status_json_buffer), 0))
                    {
                        synapse_promise_resolve(msg.payload.get_status.promise, status_json_buffer, NULL);
                    }
                    else
                    {
                        ESP_LOGE(TAG, "Failed to print status JSON to preallocated buffer.");
                        synapse_promise_reject(msg.payload.get_status.promise, NULL, NULL);
                    }
                    cJSON_Delete(status_json);
                }
                else
                {
                    ESP_LOGE(TAG, "Failed to build status JSON.");
                    synapse_promise_reject(msg.payload.get_status.promise, NULL, NULL);
                }
            }
            break;
            }
        }
    }
}

// --- Helper Functions ---
void reconnect_timer_callback(TimerHandle_t xTimer)
{
    module_t *self = (module_t *)pvTimerGetTimerID(xTimer);
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    wifi_queue_msg_t msg = {.type = WIFI_CMD_RECONNECT_TIMER_FIRED};
    xQueueSend(private_data->cmd_queue, &msg, 0);
}

void start_wifi_connection(module_t *self)
{
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    wifi_queue_msg_t msg = {.type = WIFI_CMD_CONNECT};
    xQueueSend(private_data->cmd_queue, &msg, 0);
}

esp_err_t save_credentials(module_t *self, const char *ssid, const char *password)
{
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    if (!private_data->storage_handle)
    {
        ESP_LOGE(TAG, "Storage service not available. Cannot save credentials.");
        return ESP_ERR_INVALID_STATE;
    }
    ESP_LOGI(TAG, "Saving credentials via Storage Manager...");
    esp_err_t err = private_data->storage_handle->set_string("wifi_manager", "ssid", ssid);
    if (err == ESP_OK)
    {
        err = private_data->storage_handle->set_string("wifi_manager", "password", password);
    }
    return err;
}

esp_err_t load_credentials(module_t *self)
{
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    if (!private_data->storage_handle)
    {
        ESP_LOGW(TAG, "Storage service not available. Cannot load credentials.");
        return ESP_ERR_INVALID_STATE;
    }
    ESP_LOGI(TAG, "Loading credentials via Storage Manager...");
    memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));
    size_t required_size = sizeof(private_data->wifi_config.sta.ssid);
    esp_err_t err = private_data->storage_handle->get_string("wifi_manager", "ssid", (char *)private_data->wifi_config.sta.ssid, &required_size);
    if (err == ESP_OK)
    {
        required_size = sizeof(private_data->wifi_config.sta.password);
        err = private_data->storage_handle->get_string("wifi_manager", "password", (char *)private_data->wifi_config.sta.password, &required_size);
    }
    private_data->has_saved_credentials = (err == ESP_OK);
    return err;
}