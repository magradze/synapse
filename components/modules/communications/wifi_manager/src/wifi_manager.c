/**
 * @file wifi_manager.c
 * @brief Implementation of WiFi connection and reconnection management.
 * @author Giorgi Magradze & Synapse AI Team
 * @version 3.2.0
 * @date 2025-08-28
 * @details This module manages the device's WiFi connection. It uses a dedicated
 *          FreeRTOS task to handle connection logic, preventing stack overflows
 *          in system tasks. It uses the injected Storage Manager service to
 *          persist credentials and handles automatic reconnection with an
 *          exponential backoff strategy.
 */

#include "wifi_manager.h"
#include "synapse.h"
#include "cmd_router_interface.h"
#include "storage_interface.h" // <-- დაკარგული ჰედერი, რომელიც იწვევდა შეცდომებს

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "nvs.h"

DEFINE_COMPONENT_TAG("WIFI_MANAGER");

// --- Internal Command Types for the WiFi Task ---
typedef enum
{
    WIFI_CMD_CONNECT,
    WIFI_CMD_DISCONNECT,
    WIFI_CMD_RECONNECT_TIMER_FIRED,
} wifi_cmd_type_t;

// --- Private Data Structure ---
typedef struct
{
    // --- Injected Dependencies (MUST BE FIRST) ---
    storage_api_t *storage_handle;

    // --- Module State & Configuration ---
    char instance_name[CONFIG_WIFI_MANAGER_INSTANCE_NAME_MAX_LEN];
    bool is_connected;
    wifi_config_t wifi_config;
    int retry_num;
    bool has_saved_credentials;

    // --- RTOS Handles ---
    TaskHandle_t task_handle;
    QueueHandle_t cmd_queue;
    TimerHandle_t reconnect_timer;

} wifi_manager_private_data_t;

// --- Dependency Map ---
static const module_dependency_t s_dependencies[] = {
    {"storage_service", offsetof(wifi_manager_private_data_t, storage_handle)},
    {NULL, 0} // Terminator
};

// --- Forward Declarations ---
static esp_err_t wifi_manager_init(module_t *self);
static esp_err_t wifi_manager_start(module_t *self);
static void wifi_manager_deinit(module_t *self);
static void wifi_manager_handle_event(module_t *self, const char *event_name, void *event_data);
static void wifi_task(void *pvParameters);
static void reconnect_timer_callback(TimerHandle_t xTimer);
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static void ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static esp_err_t wifi_cmd_handler(int argc, char **argv, void *context);
static void register_cli_commands(module_t *self);
static esp_err_t save_credentials(module_t *self, const char *ssid, const char *password);
static esp_err_t load_credentials(module_t *self);
static void start_wifi_connection(module_t *self);

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

    module->current_config = (cJSON *)config;
    module->private_data = private_data;
    module->dependency_map = s_dependencies;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    snprintf(module->name, sizeof(module->name), "%s", name_node->valuestring);
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", name_node->valuestring);

    module->init_level = 40;
    module->base.init = wifi_manager_init;
    module->base.start = wifi_manager_start;
    module->base.deinit = wifi_manager_deinit;
    module->base.handle_event = wifi_manager_handle_event;
    module->base.enable = NULL;
    module->base.disable = NULL;

    ESP_LOGI(TAG, "WiFi Manager module created: '%s'", module->name);
    return module;
}

// --- Lifecycle Functions ---
static esp_err_t wifi_manager_init(module_t *self)
{
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing wifi_manager module: %s", self->name);

    // Dependency validation
    if (!private_data->storage_handle)
    {
        ESP_LOGE(TAG, "Dependency injection failed: storage_handle is NULL!");
        return ESP_ERR_INVALID_STATE;
    }

    private_data->cmd_queue = xQueueCreate(5, sizeof(wifi_cmd_type_t));
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

    fmw_event_bus_subscribe("PROV_CREDENTIALS_RECEIVED", self);
    fmw_event_bus_subscribe(FMW_EVENT_SYSTEM_START_COMPLETE, self);

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
        fmw_event_bus_post("WIFI_CREDENTIALS_NOT_FOUND", NULL);
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

    fmw_event_bus_unsubscribe("PROV_CREDENTIALS_RECEIVED", self);
    fmw_event_bus_unsubscribe(FMW_EVENT_SYSTEM_START_COMPLETE, self);

    esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler);
    esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler);

    esp_wifi_stop();
    esp_wifi_deinit();

    if (self->current_config)
        cJSON_Delete(self->current_config);
    free(self);
    ESP_LOGI(TAG, "Module deinitialized successfully");
}

// --- Task & Event Handlers ---

static void wifi_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    wifi_cmd_type_t cmd;

    while (1)
    {
        if (xQueueReceive(private_data->cmd_queue, &cmd, portMAX_DELAY) == pdPASS)
        {
            switch (cmd)
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
            }
        }
    }
}

static void wifi_manager_handle_event(module_t *self, const char *event_name, void *event_data)
{
    // This handler now only deals with events that don't require the task,
    // or it forwards commands to the task.
    // The private_data variable is removed as it's currently unused here.

    if (strcmp(event_name, "PROV_CREDENTIALS_RECEIVED") == 0)
    {
        ESP_LOGI(TAG, "Received provisioning credentials");
        event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;
        if (wrapper && wrapper->payload)
        {
            cJSON *creds_json = cJSON_Parse((char *)wrapper->payload);
            if (creds_json)
            {
                const cJSON *ssid_json = cJSON_GetObjectItem(creds_json, "ssid");
                const cJSON *password_json = cJSON_GetObjectItem(creds_json, "password");
                if (cJSON_IsString(ssid_json) && cJSON_IsString(password_json))
                {
                    if (save_credentials(self, ssid_json->valuestring, password_json->valuestring) == ESP_OK)
                    {
                        load_credentials(self);
                        start_wifi_connection(self);
                    }
                }
                cJSON_Delete(creds_json);
            }
        }
    }
    else if (strcmp(event_name, FMW_EVENT_SYSTEM_START_COMPLETE) == 0)
    {
        ESP_LOGI(TAG, "System start complete. Registering CLI commands now.");
        register_cli_commands(self);
    }

    if (event_data)
    {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    module_t *self = (module_t *)arg;
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    if (event_id == WIFI_EVENT_STA_START)
    {
        ESP_LOGI(TAG, "WiFi STA started, attempting to connect...");
        esp_wifi_connect();
    }
    else if (event_id == WIFI_EVENT_STA_CONNECTED)
    {
        ESP_LOGI(TAG, "Connected to WiFi network");
        private_data->retry_num = 0;
        xTimerStop(private_data->reconnect_timer, 0);
        fmw_event_bus_post("WIFI_EVENT_CONNECTED", NULL);
    }
    else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        private_data->is_connected = false;
        ESP_LOGW(TAG, "Disconnected from WiFi network");
        fmw_event_bus_post("WIFI_EVENT_DISCONNECTED", NULL);

        if (private_data->retry_num < CONFIG_WIFI_MANAGER_MAX_RECONNECT_ATTEMPTS)
        {
            uint32_t delay_ms = 5000 * (1 << private_data->retry_num);
            if (delay_ms > 60000)
                delay_ms = 60000;
            ESP_LOGI(TAG, "Retry %d in %" PRIu32 " ms", private_data->retry_num + 1, delay_ms);
            private_data->retry_num++;
            xTimerChangePeriod(private_data->reconnect_timer, pdMS_TO_TICKS(delay_ms), 0);
            xTimerStart(private_data->reconnect_timer, 0);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to connect after %d attempts. Erasing creds and waiting for provisioning.", CONFIG_WIFI_MANAGER_MAX_RECONNECT_ATTEMPTS);
            if (private_data->storage_handle)
            {
                private_data->storage_handle->erase_key("wifi_manager", "ssid");
                private_data->storage_handle->erase_key("wifi_manager", "password");
            }
            private_data->has_saved_credentials = false;
            memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));
            fmw_event_bus_post("PROV_START_REQUESTED", NULL);
        }
    }
}

static void ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    module_t *self = (module_t *)arg;
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    if (event_id == IP_EVENT_STA_GOT_IP)
    {
        private_data->is_connected = true;
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        fmw_event_bus_post("WIFI_EVENT_IP_ASSIGNED", NULL);
    }
}

static void reconnect_timer_callback(TimerHandle_t xTimer)
{
    module_t *self = (module_t *)pvTimerGetTimerID(xTimer);
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    wifi_cmd_type_t cmd = WIFI_CMD_RECONNECT_TIMER_FIRED;
    xQueueSend(private_data->cmd_queue, &cmd, 0);
}

// --- Helper Functions ---
static void start_wifi_connection(module_t *self)
{
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    wifi_cmd_type_t cmd = WIFI_CMD_CONNECT;
    xQueueSend(private_data->cmd_queue, &cmd, 0);
}

static esp_err_t save_credentials(module_t *self, const char *ssid, const char *password)
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

static esp_err_t load_credentials(module_t *self)
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

// --- CLI Handler ---
static esp_err_t wifi_cmd_handler(int argc, char **argv, void *context)
{
    module_t *self = (module_t *)context;
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    if (argc < 2)
    {
        printf("Usage: wifi <status|scan|connect|disconnect|erase_creds|...>\n");
        return ESP_ERR_INVALID_ARG;
    }

    const char *sub_command = argv[1];

    if (strcmp(sub_command, "status") == 0)
    {
        bool silent_mode = (argc > 2 && strcmp(argv[2], "--silent") == 0);

        cJSON *status_json = cJSON_CreateObject();
        if (!status_json)
        {
            ESP_LOGE(TAG, "Failed to create cJSON object for status.");
            return ESP_ERR_NO_MEM;
        }

        cJSON_AddStringToObject(status_json, "module_name", self->name);
        cJSON_AddStringToObject(status_json, "connection_status", private_data->is_connected ? "Connected" : "Disconnected");

        if (private_data->is_connected)
        {
            wifi_ap_record_t ap_info;
            if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK)
            {
                cJSON_AddStringToObject(status_json, "ssid", (const char *)ap_info.ssid);
                cJSON_AddNumberToObject(status_json, "channel", ap_info.primary);
                cJSON_AddNumberToObject(status_json, "rssi", ap_info.rssi);
            }

            esp_netif_ip_info_t ip_info;
            esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
            if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK)
            {
                char ip_str[16];
                sprintf(ip_str, IPSTR, IP2STR(&ip_info.ip));
                cJSON_AddStringToObject(status_json, "ip_address", ip_str);
            }
        }

        // --- Event Publishing Logic (Refactored for Heap Efficiency) ---
        static char json_buffer[256];
        if (!cJSON_PrintPreallocated(status_json, json_buffer, sizeof(json_buffer), 0))
        {
            ESP_LOGE(TAG, "Failed to print cJSON to preallocated buffer. Buffer might be too small.");
            cJSON_Delete(status_json);
            return ESP_FAIL;
        }

        static fmw_telemetry_payload_t payload;
        snprintf(payload.module_name, sizeof(payload.module_name), "%s", self->name);
        payload.json_data = json_buffer;

        ESP_LOGI(TAG, "Publishing WIFI_STATUS_READY event (silent: %d)", silent_mode);
        event_data_wrapper_t *wrapper;
        if (fmw_event_data_wrap(&payload, NULL, &wrapper) == ESP_OK)
        {
            fmw_event_bus_post(FMW_EVENT_WIFI_STATUS_READY, wrapper);
            fmw_event_data_release(wrapper);
        }

        // --- Console Output Logic ---
        if (!silent_mode)
        {
            char *console_output_string = cJSON_Print(status_json);
            if (console_output_string)
            {
                printf("---------------- WiFi Status ----------------\n%s\n-------------------------------------------\n", console_output_string);
                free(console_output_string);
            }
        }

        cJSON_Delete(status_json);
        return ESP_OK;
    }
    else if (strcmp(sub_command, "connect") == 0)
    {
        if (argc != 4)
        {
            printf("Usage: wifi connect <ssid> <password>\n");
            return ESP_ERR_INVALID_ARG;
        }
        if (save_credentials(self, argv[2], argv[3]) == ESP_OK)
        {
            load_credentials(self);
            start_wifi_connection(self);
        }
        return ESP_OK;
    }
    else if (strcmp(sub_command, "disconnect") == 0)
    {
        wifi_cmd_type_t cmd = WIFI_CMD_DISCONNECT;
        xQueueSend(private_data->cmd_queue, &cmd, 0);
        return ESP_OK;
    }
    else if (strcmp(sub_command, "erase_creds") == 0)
    {
        if (private_data->storage_handle)
        {
            private_data->storage_handle->erase_key("wifi_manager", "ssid");
            private_data->storage_handle->erase_key("wifi_manager", "password");
            printf("Credentials erased. Please reboot or provision the device.\n");
            memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));
            private_data->has_saved_credentials = false;
        }
        else
        {
            printf("Error: Storage service not available.\n");
        }
        return ESP_OK;
    }
    // ... (აქ შეიძლება დაემატოს სხვა ბრძანებები, როგორიცაა scan, set_hostname და ა.შ.) ...

    printf("Error: Unknown or incomplete command.\n");
    return ESP_ERR_INVALID_ARG;
}

static void register_cli_commands(module_t *self)
{
    static cmd_t wifi_command;
    wifi_command = (cmd_t){
        .command = "wifi",
        .help = "Manage and query WiFi status and configuration.",
        .usage = "wifi <status|scan|connect|disconnect|erase_creds|...>",
        .min_args = 2,
        .max_args = 4,
        .handler = wifi_cmd_handler,
        .context = self};
    service_handle_t handle = fmw_service_get("main_cmd_router");
    if (handle)
    {
        ((cmd_router_api_t *)handle)->register_command(&wifi_command);
    }
}