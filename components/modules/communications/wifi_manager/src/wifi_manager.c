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

static esp_err_t load_credentials_from_nvs(wifi_manager_private_data_t *data);
static esp_err_t save_credentials_to_nvs(const char *ssid, const char *password);
static void start_wifi_connection(module_t *self);
static void reconnect_timer_callback(TimerHandle_t xTimer);

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static void ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

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

    module->init_level = 2;

    // დავაყენოთ ფუნქციების pointers
    module->base.init = wifi_manager_init;
    module->base.start = wifi_manager_start;
    module->base.handle_event = wifi_manager_handle_event;
    module->base.deinit = wifi_manager_deinit;
    module->base.enable = wifi_manager_enable;
    module->base.disable = wifi_manager_disable;
    module->base.reconfigure = wifi_manager_reconfigure;
    module->base.get_status = wifi_manager_get_status;

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
    if (load_credentials_from_nvs(private_data) != ESP_OK)
    {
        ESP_LOGI(TAG, "No saved credentials found. Waiting for provisioning...");
    }

    esp_log_level_set("nvs", ESP_LOG_DEBUG);

    self->status = MODULE_STATUS_INITIALIZED;
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

                    if (save_credentials_to_nvs(ssid_json->valuestring, password_json->valuestring) == ESP_OK)
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
                        ESP_LOGE(TAG, "Failed to save new credentials to NVS.");
                    }
                }
                cJSON_Delete(creds_json);
            }
        }
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

static esp_err_t save_credentials_to_nvs(const char *ssid, const char *password)
{
    if (!ssid || !password)
    {
        ESP_LOGE(TAG, "Invalid parameters: SSID or password is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    // Validate input lengths
    size_t ssid_len = strlen(ssid);
    size_t pass_len = strlen(password);

    if (ssid_len == 0 || ssid_len >= sizeof(((wifi_config_t *)0)->sta.ssid))
    {
        ESP_LOGE(TAG, "SSID length invalid: %zu (max: %zu)", ssid_len, sizeof(((wifi_config_t *)0)->sta.ssid) - 1);
        return ESP_ERR_INVALID_ARG;
    }

    if (pass_len >= sizeof(((wifi_config_t *)0)->sta.password))
    {
        ESP_LOGE(TAG, "Password length invalid: %zu (max: %zu)", pass_len, sizeof(((wifi_config_t *)0)->sta.password) - 1);
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t nvs_handle;
    esp_err_t err;

    ESP_LOGI(TAG, "Opening NVS for saving WiFi credentials...");
    err = nvs_open("wifi_creds", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(err));
        return err;
    }

    // Save SSID with explicit length
    ESP_LOGI(TAG, "Saving SSID (length: %zu): %.32s", ssid_len, ssid);
    err = nvs_set_str(nvs_handle, "wifi_ssid", ssid);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to save SSID: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return err;
    }

    // Save Password with explicit length
    ESP_LOGI(TAG, "Saving password (length: %zu)", pass_len);
    err = nvs_set_str(nvs_handle, "wifi_pass", password);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to save password: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return err;
    }

    // Commit changes
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to commit NVS changes: %s", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGI(TAG, "Successfully saved WiFi credentials to NVS");
    }

    nvs_close(nvs_handle);
    return err;
}

static esp_err_t load_credentials_from_nvs(wifi_manager_private_data_t *private_data)
{
    if (!private_data)
    {
        ESP_LOGE(TAG, "Invalid parameter: private_data is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t nvs_handle;
    esp_err_t err;

    ESP_LOGI(TAG, "Opening NVS with READONLY access to load credentials...");
    err = nvs_open("wifi_creds", NVS_READONLY, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(err));
        return err;
    }

    // Clear the WiFi configuration first
    memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));

    // Use temporary buffers for safer reading
    char temp_ssid[33] = {0};     // 32 + 1 for null terminator
    char temp_password[65] = {0}; // 64 + 1 for null terminator

    // Read SSID - get required size first
    size_t required_size = sizeof(temp_ssid);
    err = nvs_get_str(nvs_handle, "wifi_ssid", temp_ssid, &required_size);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read SSID from NVS: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return err;
    }

    ESP_LOGI(TAG, "Read SSID from NVS: %s", temp_ssid);

    // Validate SSID length
    size_t ssid_len = strlen(temp_ssid);
    if (ssid_len == 0 || ssid_len >= sizeof(private_data->wifi_config.sta.ssid))
    {
        ESP_LOGE(TAG, "Invalid SSID length: %zu", ssid_len);
        nvs_close(nvs_handle);
        return ESP_ERR_INVALID_SIZE;
    }

    // Read Password
    required_size = sizeof(temp_password);
    err = nvs_get_str(nvs_handle, "wifi_pass", temp_password, &required_size);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read password from NVS: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return err;
    }

    ESP_LOGI(TAG, "Read Password from NVS: [***]");

    // Validate password length
    size_t pass_len = strlen(temp_password);
    if (pass_len >= sizeof(private_data->wifi_config.sta.password))
    {
        ESP_LOGE(TAG, "Invalid password length: %zu", pass_len);
        nvs_close(nvs_handle);
        return ESP_ERR_INVALID_SIZE;
    }

    nvs_close(nvs_handle);

    // Copy to final destination with explicit bounds checking
    strncpy((char *)private_data->wifi_config.sta.ssid, temp_ssid, sizeof(private_data->wifi_config.sta.ssid) - 1);
    strncpy((char *)private_data->wifi_config.sta.password, temp_password, sizeof(private_data->wifi_config.sta.password) - 1);

    // Ensure null termination (though wifi_config fields are not strings, this is defensive)
    private_data->wifi_config.sta.ssid[sizeof(private_data->wifi_config.sta.ssid) - 1] = '\0';
    private_data->wifi_config.sta.password[sizeof(private_data->wifi_config.sta.password) - 1] = '\0';

    ESP_LOGI(TAG, "Successfully loaded and validated credentials from NVS for SSID: %s",
             (char *)private_data->wifi_config.sta.ssid);

    private_data->has_saved_credentials = true;
    return ESP_OK;
}