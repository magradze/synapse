/**
 * @file wifi_manager_api.c
 * @brief Implements the public Service API for the WiFi Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-09-02
 */
#include "wifi_manager_internal.h"

DEFINE_COMPONENT_TAG("WIFI_MANAGER_API");

// --- Service API Implementation ---

esp_err_t wifi_api_get_status_async(void *context, promise_then_cb then_cb, promise_catch_cb catch_cb, void *user_context)
{
    module_t *self = (module_t *)context;
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

    promise_handle_t promise = synapse_promise_create(then_cb, catch_cb, user_context);
    if (!promise) {
        ESP_LOGE(TAG, "Failed to create a promise for status request.");
        return ESP_ERR_NO_MEM;
    }

    wifi_queue_msg_t msg = {
        .type = WIFI_CMD_GET_STATUS_ASYNC,
        .payload.get_status.promise = promise
    };

    if (xQueueSend(private_data->cmd_queue, &msg, 0) != pdPASS) {
        ESP_LOGE(TAG, "Failed to queue status request. Promise will not be fulfilled.");
        return ESP_FAIL;
    }

    return ESP_OK;
}

bool wifi_api_is_connected(void *context)
{
    module_t *self = (module_t *)context;
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    return private_data->is_connected;
}

cJSON *build_status_json(module_t *self)
{
    wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;
    cJSON *status_json = cJSON_CreateObject();
    if (!status_json) {
        ESP_LOGE(TAG, "Failed to create cJSON object for status.");
        return NULL;
    }

    cJSON_AddStringToObject(status_json, "module_name", self->name);
    cJSON_AddStringToObject(status_json, "connection_status", private_data->is_connected ? "Connected" : "Disconnected");

    if (private_data->is_connected) {
        wifi_ap_record_t ap_info;
        if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
            cJSON_AddStringToObject(status_json, "ssid", (const char *)ap_info.ssid);
            cJSON_AddNumberToObject(status_json, "channel", ap_info.primary);
            cJSON_AddNumberToObject(status_json, "rssi", ap_info.rssi);
        }

        esp_netif_ip_info_t ip_info;
        esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
        if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
            char ip_str[16];
            sprintf(ip_str, IPSTR, IP2STR(&ip_info.ip));
            cJSON_AddStringToObject(status_json, "ip_address", ip_str);
        }
    }
    return status_json;
}