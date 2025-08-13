/**
 * @file wifi_manager_events.c
 * @brief Implements the event handlers for the WiFi Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-09-02
 */
#include "wifi_manager_internal.h"

DEFINE_COMPONENT_TAG("WIFI_MANAGER_EVENTS", SYNAPSE_LOG_COLOR_CYAN);

void wifi_manager_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (strcmp(event_name, "PROV_CREDENTIALS_RECEIVED") == 0) {
        ESP_LOGI(TAG, "Received provisioning credentials");
        event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;
        if (wrapper && wrapper->payload) {
            cJSON *creds_json = cJSON_Parse((char *)wrapper->payload);
            if (creds_json) {
                const cJSON *ssid_json = cJSON_GetObjectItem(creds_json, "ssid");
                const cJSON *password_json = cJSON_GetObjectItem(creds_json, "password");
                if (cJSON_IsString(ssid_json) && cJSON_IsString(password_json)) {
                    if (save_credentials(self, ssid_json->valuestring, password_json->valuestring) == ESP_OK) {
                        load_credentials(self);
                        start_wifi_connection(self);
                    }
                }
                cJSON_Delete(creds_json);
            }
        }
    } else if (strcmp(event_name, SYNAPSE_EVENT_SYSTEM_START_COMPLETE) == 0) {
        ESP_LOGI(TAG, "System start complete. Registering CLI commands now.");
        register_cli_commands(self);
    }

    if (event_data) {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    module_t *self = (module_t *)arg;
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    if (event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "WiFi STA started, attempting to connect...");
        esp_wifi_connect();
    } else if (event_id == WIFI_EVENT_STA_CONNECTED) {
        ESP_LOGI(TAG, "Connected to WiFi network");
        private_data->retry_num = 0;
        xTimerStop(private_data->reconnect_timer, 0);
        synapse_event_bus_post("WIFI_EVENT_CONNECTED", NULL);
    } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
        private_data->is_connected = false;
        ESP_LOGW(TAG, "Disconnected from WiFi network");
        synapse_event_bus_post("WIFI_EVENT_DISCONNECTED", NULL);

        if (private_data->retry_num < CONFIG_WIFI_MANAGER_MAX_RECONNECT_ATTEMPTS) {
            uint32_t delay_ms = 5000 * (1 << private_data->retry_num);
            if (delay_ms > 60000) delay_ms = 60000;
            ESP_LOGI(TAG, "Retry %d in %" PRIu32 " ms", private_data->retry_num + 1, delay_ms);
            private_data->retry_num++;
            xTimerChangePeriod(private_data->reconnect_timer, pdMS_TO_TICKS(delay_ms), 0);
            xTimerStart(private_data->reconnect_timer, 0);
        } else {
            ESP_LOGE(TAG, "Failed to connect after %d attempts. Erasing creds and waiting for provisioning.", CONFIG_WIFI_MANAGER_MAX_RECONNECT_ATTEMPTS);
            if (private_data->storage_handle) {
                private_data->storage_handle->erase_key("wifi_manager", "ssid");
                private_data->storage_handle->erase_key("wifi_manager", "password");
            }
            private_data->has_saved_credentials = false;
            memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));
            synapse_event_bus_post("PROV_START_REQUESTED", NULL);
        }
    }
}

void ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    module_t *self = (module_t *)arg;
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    if (event_id == IP_EVENT_STA_GOT_IP) {
        private_data->is_connected = true;
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        synapse_event_bus_post("WIFI_EVENT_IP_ASSIGNED", NULL);
    }
}