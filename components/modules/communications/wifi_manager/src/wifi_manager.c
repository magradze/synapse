/**
 * @file wifi_manager.c
 * @brief WiFi კავშირის და კავშირის აღდგენის მართვის იმპლემენტაცია.
 * @author Giorgi Magradze
 * @version 2.0.0
 * @date 2025-06-28
 */

#include "wifi_manager.h"
#include "base_module.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "framework_config.h"
#include "logging.h"
#include "service_locator.h"
#include "cmd_router_interface.h"
#include "storage_interface.h"
#include "service_locator.h" // თუ უკვე არ არის დამატებული
#include "framework_events.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <wifi_provisioning/manager.h>

DEFINE_COMPONENT_TAG("WIFI_MANAGER");

// --- Synapse ივენთების სახელები ---
#define EVT_PROV_CREDENTIALS_RECEIVED "PROV_CREDENTIALS_RECEIVED"
#define EVT_WIFI_CONNECTED "WIFI_EVENT_CONNECTED"
#define EVT_WIFI_DISCONNECTED "WIFI_EVENT_DISCONNECTED"
#define EVT_WIFI_IP_ASSIGNED "WIFI_EVENT_IP_ASSIGNED"

// Default მნიშვნელობები (თუ Kconfig-ში არ არის)
#ifndef CONFIG_WIFI_MANAGER_MAX_RECONNECT_DELAY_MS
#define CONFIG_WIFI_MANAGER_MAX_RECONNECT_DELAY_MS 60000
#endif

// ==============================================================================
//  პრივატული მონაცემების სტრუქტურა
// ==============================================================================
typedef struct
{
    bool enabled; // მოდულის ჩართვის ფლაგი
    char instance_name[CONFIG_WIFI_MANAGER_INSTANCE_NAME_MAX_LEN];
    bool is_connected;             // მოწყობილობა დაკავშირებულია WiFi-სთან
    bool is_provisioning;          // თუ მოწყობილობა პროვიზირებულია
    wifi_config_t wifi_config;     // WiFi კონფიგურაცია (SSID/პაროლი)
    TimerHandle_t reconnect_timer; // ხელახალი დაკავშირების ტაიმერი
    int retry_num;                 // დაკავშირების მცდელობების მრიცხველი
    bool has_saved_credentials;    // WiFi მონაცემების არსებობის ფლაგი
} wifi_manager_private_data_t;

// ==============================================================================
//  პრივატული ფუნქციების დეკლარაციები
// ==============================================================================
static esp_err_t wifi_manager_init(module_t *self);
static esp_err_t wifi_manager_start(module_t *self);
static esp_err_t wifi_manager_enable(module_t *self);
static esp_err_t wifi_manager_disable(module_t *self);
static esp_err_t wifi_manager_reconfigure(module_t *self, const cJSON *new_config);
static module_status_t wifi_manager_get_status(module_t *self);
static void wifi_manager_deinit(module_t *self);
static void wifi_manager_handle_event(module_t *self, const char *event_name, void *event_data);

static esp_err_t load_credentials(wifi_manager_private_data_t *data);
static esp_err_t save_credentials(const char *ssid, const char *password);
static void start_wifi_connection(module_t *self);
static void reconnect_timer_callback(TimerHandle_t xTimer);

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static void ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

static esp_err_t wifi_cmd_handler(int argc, char **argv, void *context);
static void register_cli_commands(module_t *self);
static void unregister_cli_commands(module_t *self);

// ==============================================================================
//  Public API - მოდულის შემქმნელი ფუნქცია
// ==============================================================================
module_t *wifi_manager_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating wifi_manager module instance");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for module");
        return NULL;
    }

    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)calloc(1, sizeof(wifi_manager_private_data_t));
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

    // დავაყენოთ default კონფიგურაცია
    const char *instance_name = CONFIG_WIFI_MANAGER_DEFAULT_INSTANCE_NAME;
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
        }
        module->current_config = cJSON_Duplicate(config, true);
    }

    // ინიციალიზაცია
    private_data->enabled = true;
    strncpy(private_data->instance_name, instance_name, CONFIG_WIFI_MANAGER_INSTANCE_NAME_MAX_LEN - 1);
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;

    module->init_level = 40;

    // დავაყენოთ ფუნქციების pointers
    module->base.init = wifi_manager_init;
    module->base.start = wifi_manager_start;
    module->base.deinit = wifi_manager_deinit;
    module->base.enable = wifi_manager_enable;
    module->base.disable = wifi_manager_disable;
    module->base.reconfigure = wifi_manager_reconfigure;
    module->base.get_status = wifi_manager_get_status;
    module->base.handle_event = wifi_manager_handle_event;

    ESP_LOGI(TAG, "WiFi Manager module created: '%s'", instance_name);
    return module;
}

// ==============================================================================
//  მოდულის სასიცოცხლო ციკლის ფუნქციები
// ==============================================================================
static esp_err_t wifi_manager_init(module_t *self)
{
    if (!self || !self->private_data)
        return ESP_ERR_INVALID_ARG;

    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing wifi_manager module: %s", self->name);

    // WiFi stack ინიციალიზაცია
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Event handlers რეგისტრაცია
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, private_data));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler, private_data));
    // Reconnect timer შექმნა
    private_data->reconnect_timer = xTimerCreate("wifi_reconnect",
                                                 pdMS_TO_TICKS(CONFIG_WIFI_MANAGER_RECONNECT_DELAY_MS),
                                                 pdFALSE, self, reconnect_timer_callback);

    // Event Bus-ზე გამოწერა
    esp_err_t ret = fmw_event_bus_subscribe(EVT_PROV_CREDENTIALS_RECEIVED, self);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to subscribe to provisioning events: %s", esp_err_to_name(ret));
    }

    // ცდილობს წინა კავშირის მონაცემების ჩატვირთვას
    if (load_credentials(private_data) != ESP_OK)
    {
        ESP_LOGI(TAG, "No saved credentials found. Waiting for provisioning...");
    }

    esp_log_level_set("nvs", ESP_LOG_DEBUG);

    self->status = MODULE_STATUS_INITIALIZED;

    // გამოვიწეროთ სისტემის გაშვების დასრულების ივენთზე.
    // ბრძანებებს დავარეგისტრირებთ მაშინ, როცა ამ ივენთს მივიღებთ.
    esp_err_t err = fmw_event_bus_subscribe(FMW_EVENT_SYSTEM_START_COMPLETE, self);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to subscribe to system start event: %s", esp_err_to_name(err));
        // ეს არ არის კრიტიკული შეცდომა, მოდულმა შეიძლება გააგრძელოს მუშაობა CLI-ს გარეშე.
    }

    ESP_LOGI(TAG, "WiFi Manager module initialized successfully");
    return ESP_OK;
}

static esp_err_t wifi_manager_start(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;

    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    if (self->status != MODULE_STATUS_INITIALIZED)
    {
        ESP_LOGE(TAG, "Cannot start uninitialized module");
        return ESP_ERR_INVALID_STATE;
    }

    if (self->status == MODULE_STATUS_RUNNING)
    {
        ESP_LOGW(TAG, "Module already running");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Starting wifi_manager module: %s", self->name);

    // თუ NVS-დან ჩაიტვირთა მონაცემები, პირდაპირ ვიწყებთ კავშირს.
    if (private_data->has_saved_credentials)
    {
        ESP_LOGI(TAG, "Found saved credentials. Starting connection...");
        start_wifi_connection(self); // ეს ფუნქცია შენ უკვე გაქვს
    }
    else
    {
        ESP_LOGI(TAG, "No credentials. Waiting for provisioning.");
        // აქ შეგვიძლია დაველოდოთ provisioning-ის ივენთს, როგორც ახლაა.
    }

    self->status = MODULE_STATUS_RUNNING;
    private_data->enabled = true;

    ESP_LOGI(TAG, "WiFi Manager module started successfully");
    return ESP_OK;
}

static esp_err_t wifi_manager_enable(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;

    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    ESP_LOGI(TAG, "Enabling wifi_manager module: %s", self->name);

    if (private_data->enabled)
    {
        ESP_LOGW(TAG, "Module already enabled");
        return ESP_OK;
    }

    private_data->enabled = true;
    self->status = MODULE_STATUS_RUNNING;

    // თუ გვაქვს credentials, ხელახლა დავუკავშირდეთ
    if (strlen((const char *)private_data->wifi_config.sta.ssid) > 0)
    {
        start_wifi_connection(self);
    }

    ESP_LOGI(TAG, "WiFi Manager module enabled");
    return ESP_OK;
}

static esp_err_t wifi_manager_disable(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;

    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    ESP_LOGI(TAG, "Disabling wifi_manager module: %s", self->name);

    if (!private_data->enabled)
    {
        ESP_LOGW(TAG, "Module already disabled");
        return ESP_OK;
    }

    // გავთიშოთ WiFi
    esp_wifi_disconnect();
    esp_wifi_stop();

    // გავაჩეროთ reconnect timer
    if (private_data->reconnect_timer)
    {
        xTimerStop(private_data->reconnect_timer, 0);
    }

    private_data->enabled = false;
    self->status = MODULE_STATUS_DISABLED;

    ESP_LOGI(TAG, "WiFi Manager module disabled");
    return ESP_OK;
}

static esp_err_t wifi_manager_reconfigure(module_t *self, const cJSON *new_config)
{
    if (!self || !new_config)
        return ESP_ERR_INVALID_ARG;

    ESP_LOGI(TAG, "Reconfiguring wifi_manager module: %s", self->name);

    // განაახლეთ კონფიგურაცია
    if (self->current_config)
    {
        cJSON_Delete(self->current_config);
    }
    self->current_config = cJSON_Duplicate(new_config, true);

    // TODO: Apply new configuration if needed

    ESP_LOGI(TAG, "WiFi Manager module reconfigured");
    return ESP_OK;
}

static module_status_t wifi_manager_get_status(module_t *self)
{
    if (!self)
        return MODULE_STATUS_ERROR;
    return self->status;
}

static void wifi_manager_deinit(module_t *self)
{
    if (!self)
        return;

    ESP_LOGI(TAG, "Deinitializing %s module", self->name);
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    // Event Bus-დან გამოწერის გაუქმება
    fmw_event_bus_unsubscribe(EVT_PROV_CREDENTIALS_RECEIVED, self);

    fmw_event_bus_unsubscribe(FMW_EVENT_SYSTEM_START_COMPLETE, self);
    unregister_cli_commands(self);

    // ESP event handlers-ის გაუქმება
    esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler);
    esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler);

    // Timer-ის წაშლა
    if (private_data && private_data->reconnect_timer)
    {
        xTimerDelete(private_data->reconnect_timer, portMAX_DELAY);
    }

    // WiFi გათიშვა
    esp_wifi_stop();
    esp_wifi_deinit();

    // მეხსიერების გათავისუფლება
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

// ==============================================================================
//  Synapse და ESP-IDF ივენთების დამუშავებლები
// ==============================================================================

/**
 * @brief ამუშავებს Synapse Event Bus-იდან მიღებულ ივენთებს.
 * @details ამჟამად, რეაგირებს მხოლოდ PROV_CREDENTIALS_RECEIVED ივენთზე,
 *          რომელიც მოდის provisioning მოდულიდან. იღებს WiFi-ს მონაცემებს,
 *          ინახავს მათ NVS-ში და იწყებს კავშირის დამყარების პროცესს.
 * @param self მაჩვენებელი მოდულის ინსტანციაზე.
 * @param event_name მიღებული ივენთის სახელი.
 * @param event_data მაჩვენებელი ივენთის მონაცემებზე.
 */
static void wifi_manager_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (!self || !self->private_data)
    {
        if (event_data)
        {
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        }
        return;
    }

    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    if (!private_data->enabled)
    {
        if (event_data)
        {
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        }
        return;
    }

    if (event_name && strcmp(event_name, EVT_PROV_CREDENTIALS_RECEIVED) == 0)
    {
        ESP_LOGI(TAG, "[%s] Received provisioning credentials", private_data->instance_name);
        private_data->is_provisioning = true;

        event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;
        if (wrapper && wrapper->payload)
        {
            char *json_string = (char *)wrapper->payload;
            cJSON *creds_json = cJSON_Parse(json_string);
            if (creds_json)
            {
                const cJSON *ssid_json = cJSON_GetObjectItem(creds_json, "ssid");
                const cJSON *password_json = cJSON_GetObjectItem(creds_json, "password");

                if (cJSON_IsString(ssid_json) && cJSON_IsString(password_json) && ssid_json->valuestring && password_json->valuestring)
                {
                    // 1. ვასუფთავებთ ძველ კონფიგურაციას
                    memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));

                    // 2. უსაფრთხოდ ვაკოპირებთ ახალ მონაცემებს private_data-ში
                    strncpy((char *)private_data->wifi_config.sta.ssid, ssid_json->valuestring, sizeof(private_data->wifi_config.sta.ssid) - 1);
                    strncpy((char *)private_data->wifi_config.sta.password, password_json->valuestring, sizeof(private_data->wifi_config.sta.password) - 1);

                    if (save_credentials(ssid_json->valuestring, password_json->valuestring) == ESP_OK)
                    {
                        // ვაახლებთ private_data-ს მხოლოდ შენახვის შემდეგ
                        memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));
                        strncpy((char *)private_data->wifi_config.sta.ssid, ssid_json->valuestring, sizeof(private_data->wifi_config.sta.ssid) - 1);
                        strncpy((char *)private_data->wifi_config.sta.password, password_json->valuestring, sizeof(private_data->wifi_config.sta.password) - 1);
                        private_data->has_saved_credentials = true;

                        start_wifi_connection(self);
                    }
                    else
                    {
                        ESP_LOGE(TAG, "Failed to save new credentials using Storage Manager.");
                    }
                }
                cJSON_Delete(creds_json);
            }
        }
    }

    // ★★★ ახალი ლოგიკა ★★★
    if (strcmp(event_name, FMW_EVENT_SYSTEM_START_COMPLETE) == 0)
    {
        ESP_LOGI(TAG, "System start complete. Registering CLI commands now.");
        register_cli_commands(self);
    }

    // ყოველთვის გავათავისუფლოთ event data
    if (event_data)
    {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{

    ESP_LOGI(TAG, "wifi_event_handler: event_base=%p, event_id=%" PRId32, event_base, (int32_t)event_id);
    if (!arg || !event_base)
    {
        ESP_LOGE(TAG, "Invalid arguments in wifi_event_handler");
        return;
    }

    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)arg;

    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
            ESP_LOGI(TAG, "WiFi STA started");
            // Only connect if we have saved credentials
            if (private_data->has_saved_credentials)
            {
                ESP_LOGI(TAG, "Attempting to connect with saved credentials");
                esp_wifi_connect();
            }
            else
            {
                ESP_LOGI(TAG, "No saved credentials, waiting for provisioning");
                fmw_event_bus_post("WIFI_CREDENTIALS_NOT_FOUND", NULL);
            }
            break;

        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "Connected to WiFi network");
            private_data->retry_num = 0;
            xTimerStop(private_data->reconnect_timer, 0);
            fmw_event_bus_post(EVT_WIFI_CONNECTED, NULL);
            break;

        case WIFI_EVENT_STA_DISCONNECTED:
            private_data->is_connected = false;

            ESP_LOGW(TAG, "Disconnected from WiFi network");

            fmw_event_bus_post(EVT_WIFI_DISCONNECTED, NULL);

            if (private_data->retry_num < CONFIG_WIFI_MANAGER_MAX_RECONNECT_ATTEMPTS)
            {
                // ექსპონენციალური backoff ლოგიკა (უცვლელია)
                uint32_t delay_ms = CONFIG_WIFI_MANAGER_RECONNECT_DELAY_MS * (1 << private_data->retry_num);
                if (delay_ms > CONFIG_WIFI_MANAGER_MAX_RECONNECT_DELAY_MS)
                {
                    delay_ms = CONFIG_WIFI_MANAGER_MAX_RECONNECT_DELAY_MS;
                }

                ESP_LOGI(TAG, "Retry %d in %" PRIu32 " ms", private_data->retry_num + 1, delay_ms);
                private_data->retry_num++;

                xTimerChangePeriod(private_data->reconnect_timer, pdMS_TO_TICKS(delay_ms), 0);
                xTimerStart(private_data->reconnect_timer, 0);
            }
            else
            {
                ESP_LOGE(TAG, "Failed to connect to WiFi after %d attempts. Starting provisioning.", CONFIG_WIFI_MANAGER_MAX_RECONNECT_ATTEMPTS);
                // გავასუფთავოთ ძველი, არასწორი credentials
                nvs_flash_erase_partition(CONFIG_WIFI_MANAGER_NVS_NAMESPACE);
                private_data->has_saved_credentials = false;
                memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));

                // გავუშვათ provisioning ივენთი
                fmw_event_bus_post("PROV_START_REQUESTED", NULL); // ვივარაუდოთ, რომ provisioning მოდული ამ ივენთს უსმენს
            }
            break;

        default:
            break;
        }
    }
}

static void ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "ip_event_handler: event_base=%p, event_id=%" PRId32, event_base, (int32_t)event_id);
    if (!arg || !event_base)
    {
        ESP_LOGE(TAG, "Invalid arguments in ip_event_handler");
        return;
    }

    if (event_id == IP_EVENT_STA_GOT_IP)
    {
        wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)arg;
        private_data->is_connected = true;

        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));

        // კოპირება და wrapper-ში შეფუთვა
        esp_netif_ip_info_t *ip_info_copy = malloc(sizeof(esp_netif_ip_info_t));
        if (ip_info_copy)
        {
            *ip_info_copy = event->ip_info;
            event_data_wrapper_t *wrapper = NULL;
            if (fmw_event_data_wrap(ip_info_copy, free, &wrapper) == ESP_OK)
            {
                fmw_event_bus_post(EVT_WIFI_IP_ASSIGNED, wrapper);
            }
            else
            {
                ESP_LOGE(TAG, "Failed to wrap IP info data");
                free(ip_info_copy);
            }
        }
    }
}

// ==============================================================================
//  დამხმარე ფუნქციები
// ==============================================================================

static void reconnect_timer_callback(TimerHandle_t xTimer)
{
    ESP_LOGI(TAG, "Reconnect timer fired, attempting to reconnect...");
    esp_wifi_connect();
}

static void start_wifi_connection(module_t *self)
{
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    ESP_LOGI(TAG, "Starting WiFi connection to SSID: %s", private_data->wifi_config.sta.ssid);

    private_data->retry_num = 0;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &private_data->wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

/**
 * @brief Saves WiFi credentials using the central Storage Manager service.
 */
static esp_err_t save_credentials(const char *ssid, const char *password)
{
    if (!ssid || !password)
    {
        ESP_LOGE(TAG, "Invalid parameters: SSID or password is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    // ... (არგუმენტების სიგრძის ვალიდაცია უცვლელი რჩება) ...

    // 1. ვიღებთ Storage Manager სერვისს
    service_handle_t storage_handle = fmw_service_get("main_storage");
    if (!storage_handle)
    {
        ESP_LOGE(TAG, "Storage Manager service not found. Cannot save credentials.");
        return ESP_ERR_NOT_FOUND;
    }

    storage_api_t *storage = (storage_api_t *)storage_handle;
    const char *owner = "wifi_manager"; // ვიყენებთ მოდულის სახელს, როგორც namespace/directory

    ESP_LOGI(TAG, "Saving credentials via Storage Manager...");

    // 2. ვიძახებთ Storage Manager-ის API-ს SSID-ის შესანახად
    esp_err_t err = storage->set_string(owner, "ssid", ssid);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to save SSID via Storage Manager: %s", esp_err_to_name(err));
        return err;
    }

    // 3. ვიძახებთ Storage Manager-ის API-ს პაროლის შესანახად
    err = storage->set_string(owner, "password", password);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to save password via Storage Manager: %s", esp_err_to_name(err));
        storage->erase_key(owner, "ssid"); // დავასუფთავოთ, რომ არასრული მონაცემი არ დარჩეს
        return err;
    }

    ESP_LOGI(TAG, "Successfully saved WiFi credentials using Storage Manager.");
    return ESP_OK;
}

/**
 * @brief Loads WiFi credentials using the central Storage Manager service.
 */
static esp_err_t load_credentials(wifi_manager_private_data_t *private_data)
{
    if (!private_data)
    {
        return ESP_ERR_INVALID_ARG;
    }

    // 1. ვიღებთ Storage Manager სერვისს
    service_handle_t storage_handle = fmw_service_get("main_storage");
    if (!storage_handle)
    {
        ESP_LOGE(TAG, "Storage Manager service not found. Cannot load credentials.");
        return ESP_ERR_NOT_FOUND;
    }

    storage_api_t *storage = (storage_api_t *)storage_handle;
    const char *owner = "wifi_manager";

    ESP_LOGI(TAG, "Loading credentials via Storage Manager...");
    memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));

    // 2. ვკითხულობთ SSID-ს Storage Manager-ის API-თ
    size_t required_size = sizeof(private_data->wifi_config.sta.ssid);
    esp_err_t err = storage->get_string(owner, "ssid", (char *)private_data->wifi_config.sta.ssid, &required_size);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "Could not retrieve SSID. Assuming no credentials are saved.");
        return err;
    }

    // 3. ვკითხულობთ პაროლს Storage Manager-ის API-თ
    required_size = sizeof(private_data->wifi_config.sta.password);
    err = storage->get_string(owner, "password", (char *)private_data->wifi_config.sta.password, &required_size);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "SSID found, but failed to retrieve password: %s.", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Successfully loaded credentials for SSID: %s", (char *)private_data->wifi_config.sta.ssid);
    private_data->has_saved_credentials = true;
    return ESP_OK;
}

// =========================================================================
//                      Command Router Integration
// =========================================================================

/**
 * @internal
 * @brief Handles 'wifi' command from the CLI.
 * @details Parses subcommands and arguments to manage and query the WiFi state.
 *          This function is registered with the Command Router.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param context Pointer to the module_t instance.
 * @return ESP_OK on success, or an error code on failure.
 */
static esp_err_t wifi_cmd_handler(int argc, char **argv, void *context)
{
    module_t *self = (module_t *)context;
    if (!self || !self->private_data)
    {
        printf("Error: Module context is not valid.\n");
        return ESP_ERR_INVALID_STATE;
    }

    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    if (argc < 2)
    {
        printf("Error: Missing subcommand for 'wifi'.\n");
        printf("Usage: wifi <status|scan|connect|disconnect|enable|disable|erase_creds|set_hostname|set_power_save>\n");
        return ESP_ERR_INVALID_ARG;
    }

    const char *sub_command = argv[1];

    // --- ℹ️ Status and Info Commands ---
    if (strcmp(sub_command, "status") == 0)
    {
        printf("---------------- WiFi Status ----------------\n");
        printf("  Module State:      %s\n", private_data->enabled ? "Enabled" : "Disabled");
        printf("  Connection Status: %s\n", private_data->is_connected ? "Connected" : "Disconnected");

        if (private_data->is_connected)
        {
            wifi_ap_record_t ap_info;
            if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK)
            {
                printf("  SSID:              %s\n", ap_info.ssid);
                printf("  Channel:           %d\n", ap_info.primary);
                printf("  RSSI:              %d dBm\n", ap_info.rssi);
            }

            esp_netif_ip_info_t ip_info;
            esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
            if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK)
            {
                printf("  IP Address:        " IPSTR "\n", IP2STR(&ip_info.ip));
                printf("  Gateway:           " IPSTR "\n", IP2STR(&ip_info.gw));
            }
        }

        uint8_t mac[6];
        esp_wifi_get_mac(WIFI_IF_STA, mac);
        printf("  MAC Address:       %02X:%02X:%02X:%02X:%02X:%02X\n",
               mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

        const char *hostname;
        esp_netif_get_hostname(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &hostname);
        printf("  Hostname:          %s\n", hostname ? hostname : "N/A");

        printf("-------------------------------------------\n");
        return ESP_OK;
    }
    else if (strcmp(sub_command, "scan") == 0)
    {
        printf("Scanning for WiFi networks...\n");
        wifi_scan_config_t scan_config = {.show_hidden = true};

        if (esp_wifi_scan_start(&scan_config, true) != ESP_OK)
        {
            printf("Error: Failed to start WiFi scan.\n");
            return ESP_FAIL;
        }

        uint16_t ap_count = 0;
        esp_wifi_scan_get_ap_num(&ap_count);
        if (ap_count == 0)
        {
            printf("No networks found.\n");
            return ESP_OK;
        }

        wifi_ap_record_t *ap_list = (wifi_ap_record_t *)malloc(ap_count * sizeof(wifi_ap_record_t));
        if (!ap_list)
        {
            printf("Error: Failed to allocate memory for scan results.\n");
            return ESP_ERR_NO_MEM;
        }

        if (esp_wifi_scan_get_ap_records(&ap_count, ap_list) == ESP_OK)
        {
            printf("------------------------------------------------------------------\n");
            printf("  %-32s | %-4s | %-4s | %s\n", "SSID", "RSSI", "CHAN", "AUTH");
            printf("------------------------------------------------------------------\n");
            for (int i = 0; i < ap_count; i++)
            {
                const char *auth_mode;
                switch (ap_list[i].authmode)
                {
                case WIFI_AUTH_OPEN:
                    auth_mode = "OPEN";
                    break;
                case WIFI_AUTH_WEP:
                    auth_mode = "WEP";
                    break;
                case WIFI_AUTH_WPA_PSK:
                    auth_mode = "WPA_PSK";
                    break;
                case WIFI_AUTH_WPA2_PSK:
                    auth_mode = "WPA2_PSK";
                    break;
                case WIFI_AUTH_WPA_WPA2_PSK:
                    auth_mode = "WPA/WPA2";
                    break;
                case WIFI_AUTH_WPA2_ENTERPRISE:
                    auth_mode = "WPA2_ENT";
                    break;
                case WIFI_AUTH_WPA3_PSK:
                    auth_mode = "WPA3_PSK";
                    break;
                case WIFI_AUTH_WPA2_WPA3_PSK:
                    auth_mode = "WPA2/WPA3";
                    break;
                default:
                    auth_mode = "UNKNOWN";
                    break;
                }
                printf("  %-32s | %-4d | %-4d | %s\n", (char *)ap_list[i].ssid, ap_list[i].rssi, ap_list[i].primary, auth_mode);
            }
            printf("------------------------------------------------------------------\n");
        }
        free(ap_list);
        return ESP_OK;
    }

    // --- ⚙️ Control and Action Commands ---
    else if (strcmp(sub_command, "enable") == 0)
    {
        printf("Enabling WiFi module...\n");
        return wifi_manager_enable(self);
    }
    else if (strcmp(sub_command, "disable") == 0)
    {
        printf("Disabling WiFi module...\n");
        return wifi_manager_disable(self);
    }
    else if (strcmp(sub_command, "disconnect") == 0)
    {
        printf("Disconnecting from WiFi network...\n");
        return esp_wifi_disconnect();
    }
    else if (strcmp(sub_command, "connect") == 0)
    {
        if (argc != 4)
        {
            printf("Error: Incorrect arguments for 'connect'.\nUsage: wifi connect <ssid> <password>\n");
            return ESP_ERR_INVALID_ARG;
        }
        const char *ssid = argv[2];
        const char *password = argv[3];
        printf("Attempting to connect to '%s'...\n", ssid);

        if (save_credentials(ssid, password) == ESP_OK)
        {
            memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));
            strncpy((char *)private_data->wifi_config.sta.ssid, ssid, sizeof(private_data->wifi_config.sta.ssid) - 1);
            strncpy((char *)private_data->wifi_config.sta.password, password, sizeof(private_data->wifi_config.sta.password) - 1);
            private_data->has_saved_credentials = true;
            start_wifi_connection(self);
        }
        else
        {
            printf("Error: Failed to save new credentials.\n");
            return ESP_FAIL;
        }
        return ESP_OK;
    }
    else if (strcmp(sub_command, "erase_creds") == 0)
    {
        printf("Erasing saved WiFi credentials from NVS...\n");
        nvs_handle_t nvs_handle;
        if (nvs_open("wifi_creds", NVS_READWRITE, &nvs_handle) == ESP_OK)
        {
            nvs_erase_all(nvs_handle);
            nvs_commit(nvs_handle);
            nvs_close(nvs_handle);
            printf("Credentials erased. Please reboot or provision the device.\n");
            // გაასუფთავეთ მიმდინარე credentials-იც
            memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));
            private_data->has_saved_credentials = false;
        }
        else
        {
            printf("Error: Could not open NVS to erase credentials.\n");
            return ESP_FAIL;
        }
        return ESP_OK;
    }
    else if (strcmp(sub_command, "set_hostname") == 0)
    {
        if (argc < 3)
        {
            printf("Error: Missing hostname.\nUsage: wifi set_hostname <new_hostname> [--reconnect]\n");
            return ESP_ERR_INVALID_ARG;
        }
        const char *hostname = argv[2];
        esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
        if (esp_netif_set_hostname(netif, hostname) == ESP_OK)
        {
            printf("Hostname set to '%s'.\n", hostname);

            // შევამოწმოთ, მომხმარებელს სურს თუ არა დაუყოვნებლივ ხელახლა დაკავშირება
            bool reconnect = (argc == 4 && strcmp(argv[3], "--reconnect") == 0);

            if (reconnect)
            {
                printf("Reconnecting to apply new hostname...\n");
                esp_wifi_disconnect();
                // მცირე დაყოვნება, რათა disconnect-მა მოასწროს დამუშავება
                vTaskDelay(pdMS_TO_TICKS(500));
                esp_wifi_connect();
            }
            else
            {
                printf("The new hostname will be effective on the next connection.\n");
            }
        }
        else
        {
            printf("Error: Failed to set hostname.\n");
            return ESP_FAIL;
        }
        return ESP_OK;
    }
    else if (strcmp(sub_command, "set_power_save") == 0)
    {
        if (argc != 3)
        {
            printf("Error: Missing power save mode.\nUsage: wifi set_power_save <off|min|max>\n");
            return ESP_ERR_INVALID_ARG;
        }
        const char *mode_str = argv[2];
        wifi_ps_type_t ps_mode;
        if (strcmp(mode_str, "off") == 0)
        {
            ps_mode = WIFI_PS_NONE;
        }
        else if (strcmp(mode_str, "min") == 0)
        {
            ps_mode = WIFI_PS_MIN_MODEM;
        }
        else if (strcmp(mode_str, "max") == 0)
        {
            ps_mode = WIFI_PS_MAX_MODEM;
        }
        else
        {
            printf("Error: Invalid power save mode '%s'.\n", mode_str);
            return ESP_ERR_INVALID_ARG;
        }

        if (esp_wifi_set_ps(ps_mode) == ESP_OK)
        {
            printf("WiFi power save mode set to '%s'.\n", mode_str);
        }
        else
        {
            printf("Error: Failed to set power save mode.\n");
            return ESP_FAIL;
        }
        return ESP_OK;
    }

    // --- Fallback for unknown command ---
    else
    {
        printf("Error: Unknown subcommand '%s'.\n", sub_command);
        printf("Usage: wifi <status|scan|connect|disconnect|enable|disable|erase_creds|set_hostname|set_power_save>\n");
        return ESP_ERR_INVALID_ARG;
    }
}

/**
 * @internal
 * @brief Registers the 'wifi' command and its subcommands with the Command Router.
 * @details This function prepares the command structure and uses the Command Router
 *          service to make the 'wifi' command available in the system shell.
 * @param self Pointer to the module_t instance.
 */
static void register_cli_commands(module_t *self)
{
    // cmd_t სტრუქტურა უნდა იყოს static, რათა მისი მეხსიერება ვალიდური დარჩეს
    // რეგისტრაციის შემდეგაც.
    static cmd_t wifi_command;

    // შევავსოთ სტრუქტურა განახლებული ინფორმაციით
    wifi_command = (cmd_t){
        .command = "wifi",
        .help = "Manage and query WiFi status and configuration.",
        .usage = "wifi <status|scan|connect|disconnect|enable|disable|erase_creds|set_hostname|set_power_save>",
        .min_args = 2, // მინიმალური ბრძანება, მაგ: `wifi status`
        .max_args = 4, // მაქსიმალური ბრძანება, მაგ: `wifi connect <ssid> <password>`
        .handler = wifi_cmd_handler,
        .context = self // გადავცეთ მოდულის ინსტანცია, როგორც კონტექსტი
    };

    // მოვძებნოთ Command Router სერვისი Service Locator-ის მეშვეობით
    service_handle_t handle = fmw_service_get("main_cmd_router");
    if (handle)
    {
        // თუ სერვისი ნაპოვნია, ავიღოთ მისი API
        cmd_router_api_t *cmd_api = (cmd_router_api_t *)handle;

        // დავარეგისტრიროთ ჩვენი ბრძანება
        esp_err_t err = cmd_api->register_command(&wifi_command);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to register 'wifi' command: %s", esp_err_to_name(err));
        }
        else
        {
            ESP_LOGI(TAG, "'wifi' command registered successfully.");
        }
    }
    else
    {
        // თუ სერვისი ვერ მოიძებნა, დავწეროთ გაფრთხილება
        ESP_LOGW(TAG, "Command Router service not found. CLI commands for WiFi will not be available.");
    }
}

static void unregister_cli_commands(module_t *self)
{
    service_handle_t handle = fmw_service_get("main_cmd_router");
    if (handle)
    {
        cmd_router_api_t *cmd_api = (cmd_router_api_t *)handle;
        esp_err_t err = cmd_api->unregister_command("wifi");
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to unregister 'wifi' command: %s", esp_err_to_name(err));
        }
        else
        {
            ESP_LOGI(TAG, "'wifi' command unregistered successfully.");
        }
    }
}