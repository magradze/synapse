/**
 * @file mqtt_manager.c
 * @brief Manages MQTT connection, command forwarding, and status publishing.
 * @author Synapse Framework Team
 * @version 1.2.0
 * @date 2025-07-10
 * @details
 * This module acts as an intelligent communication bridge for the MQTT protocol.
 * It connects to the broker after WiFi is available, uses the Device Identity
 * Service for unique client ID and topics, forwards incoming commands to the
 * Command Router, and publishes status updates from other modules based on a
 * centralized event-to-topic map.
 */

// --- Framework & System Includes ---
#include "mqtt_manager.h"
#include "base_module.h"
#include "logging.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "event_payloads.h"
#include "framework_events.h"
#include "service_locator.h"
#include "device_identity_interface.h"
#include "mqtt_topics_generated.h" // ★★★ Auto-generated MQTT topics ★★★
#include "timer_interface.h"
// --- ESP-IDF & Standard Lib Includes ---
#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_event.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("MQTT_MANAGER");

// --- Event-to-Topic Mapping ---

/**
 * @internal
 * @brief Structure to map an internal event name to an MQTT topic template.
 * @details This allows for a generic event handler that publishes to the correct
 *          MQTT topic without a large if/else chain.
 */
typedef struct {
    const char *event_name;
    const char *mqtt_topic_template;
} event_to_topic_map_t;

/**
 * @internal
 * @brief The map that defines which events should be published to which MQTT topics.
 * @details This is the central point for adding new status publications. The module
 *          will automatically subscribe to these events and publish their payloads
 *          to the corresponding MQTT topic.
 */
static const event_to_topic_map_t event_topic_map[] = {
    { FMW_EVENT_WIFI_STATUS_READY, MQTT_TOPIC_PUB_WIFI_MANAGER_STATUS },
    { FMW_EVENT_DEVICE_INFO_READY, MQTT_TOPIC_PUB_DEVICE_IDENTITY_SERVICE_INFO },
};
static const int event_topic_map_size = sizeof(event_topic_map) / sizeof(event_topic_map[0]);


// --- Private Data & Configuration ---

typedef struct {
    char broker_uri[128];
    char username[64];
    char password[64];
    char base_topic_prefix[64]; // This is now part of the generated topic
    char lwt_topic[128];
    char lwt_message[128];
    int lwt_qos;
    bool lwt_retain;
} mqtt_manager_config_t;

typedef struct {
    char instance_name[CONFIG_MQTT_MANAGER_INSTANCE_NAME_MAX_LEN];
    mqtt_manager_config_t config;
    esp_mqtt_client_handle_t client_handle;
    bool is_connected;
    fmw_timer_handle_t heartbeat_timer;
} mqtt_manager_private_data_t;


// --- Forward Declarations ---
static esp_err_t mqtt_manager_init(module_t *self);
static void mqtt_manager_deinit(module_t *self);
static void mqtt_manager_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t parse_mqtt_config(const cJSON *config_json, mqtt_manager_config_t *mqtt_config);
static void start_mqtt_connection(module_t *self);
static void mqtt_event_handler_cb(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);


// =========================================================================
//                      Module Lifecycle Implementation
// =========================================================================

module_t *mqtt_manager_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating MQTT Manager module instance");
    
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module) {
        ESP_LOGE(TAG, "Failed to allocate memory for module");
        return NULL;
    }
    
    mqtt_manager_private_data_t *private_data = (mqtt_manager_private_data_t *)calloc(1, sizeof(mqtt_manager_private_data_t));
    if (!private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory for private data");
        free(module);
        return NULL;
    }
    
    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex) {
        ESP_LOGE(TAG, "Failed to create state mutex");
        free(private_data);
        free(module);
        return NULL;
    }
    
    module->private_data = private_data;
    
    const char *instance_name = CONFIG_MQTT_MANAGER_DEFAULT_INSTANCE_NAME;
    if (config) {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node)) {
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node) && name_node->valuestring) {
                instance_name = name_node->valuestring;
            }
            if (parse_mqtt_config(config_node, &private_data->config) != ESP_OK) {
                ESP_LOGE(TAG, "Failed to parse MQTT config. Module creation failed.");
                vSemaphoreDelete(module->state_mutex);
                free(private_data);
                free(module);
                return NULL;
            }
        }
        module->current_config = cJSON_Duplicate(config, true);
    }
    
    strncpy(private_data->instance_name, instance_name, sizeof(private_data->instance_name) - 1);
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->init_level = 45;
    
    module->base.init = mqtt_manager_init;
    module->base.start = NULL;
    module->base.deinit = mqtt_manager_deinit;
    module->base.enable = NULL;
    module->base.disable = NULL;
    module->base.reconfigure = NULL;
    module->base.get_status = NULL;
    module->base.handle_event = mqtt_manager_handle_event;
    
    ESP_LOGI(TAG, "MQTT Manager module created: '%s'", instance_name);
    return module;
}

static esp_err_t mqtt_manager_init(module_t *self)
{
    if (!self) return ESP_ERR_INVALID_ARG;
    
    ESP_LOGI(TAG, "Initializing MQTT Manager module: %s", self->name);
    
    // Subscribe to the event that signals WiFi is connected and has an IP
    fmw_event_bus_subscribe("WIFI_EVENT_IP_ASSIGNED", self);

    fmw_event_bus_subscribe("MQTT_HEARTBEAT_TICK", self);

    // Automatically subscribe to all events in the map
    for (int i = 0; i < event_topic_map_size; i++) {
        fmw_event_bus_subscribe(event_topic_map[i].event_name, self);
    }
    
    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "MQTT Manager module initialized and waiting for WiFi IP.");
    return ESP_OK;
}

static void mqtt_manager_deinit(module_t *self)
{
    if (!self) return;
    
    mqtt_manager_private_data_t *p_data = (mqtt_manager_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Deinitializing %s module", self->name);
    
    if (p_data->client_handle) {
        esp_mqtt_client_destroy(p_data->client_handle);
    }

    // Unsubscribe from all events
    fmw_event_bus_unsubscribe("WIFI_EVENT_IP_ASSIGNED", self);
    for (int i = 0; i < event_topic_map_size; i++) {
        fmw_event_bus_unsubscribe(event_topic_map[i].event_name, self);
    }
    
    if (self->private_data) free(self->private_data);
    if (self->current_config) cJSON_Delete(self->current_config);
    if (self->state_mutex) vSemaphoreDelete(self->state_mutex);
    free(self);
    
    ESP_LOGI(TAG, "Module deinitialized successfully");
}

// =========================================================================
//                      Event Handling
// =========================================================================

static void mqtt_manager_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (!self || !event_name) {
        if (event_data) fmw_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }
    
    mqtt_manager_private_data_t *p_data = (mqtt_manager_private_data_t *)self->private_data;
    ESP_LOGD(TAG, "Event received: '%s'", event_name);

    // Handle connection trigger
    if (strcmp(event_name, "WIFI_EVENT_IP_ASSIGNED") == 0) {
        ESP_LOGI(TAG, "WiFi is connected with IP. Starting MQTT connection...");
        start_mqtt_connection(self);
        goto cleanup; // We are done with this event
    }

    if (strcmp(event_name, "MQTT_HEARTBEAT_TICK") == 0)
    {
        if (p_data->is_connected)
        {
            // გამოვაქვეყნოთ ზოგადი "ცოცხალი ვარ" სიგნალი
            fmw_connectivity_payload_t *payload = malloc(sizeof(fmw_connectivity_payload_t));
            if (payload)
            {
                snprintf(payload->check_name, sizeof(payload->check_name), "MQTT_Heartbeat");
                event_data_wrapper_t *wrapper;
                if (fmw_event_data_wrap(payload, free, &wrapper) == ESP_OK)
                {
                    fmw_event_bus_post(FMW_EVENT_CONNECTIVITY_ESTABLISHED, wrapper);
                    fmw_event_data_release(wrapper);
                }
                else
                {
                    free(payload);
                }
            }
        }
    }

    // Handle all other events from the map
    for (int i = 0; i < event_topic_map_size; i++) {
        if (strcmp(event_name, event_topic_map[i].event_name) == 0) {
            if (p_data->is_connected && event_data) {
                event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;
                char *json_payload = (char *)wrapper->payload;

                char final_topic[128];
                service_handle_t id_service_handle = fmw_service_get("main_identity_service");
                if (id_service_handle) {
                    device_identity_api_t *id_api = (device_identity_api_t *)id_service_handle;
                    
                    // Build the topic using the template from the map
                    MQTT_BUILD_TOPIC(final_topic, sizeof(final_topic), 
                                     event_topic_map[i].mqtt_topic_template, 
                                     id_api->get_device_id());
                    
                    ESP_LOGI(TAG, "Publishing event '%s' to topic: %s", event_name, final_topic);
                    esp_mqtt_client_publish(p_data->client_handle, final_topic, json_payload, 0, 1, true);
                }
            }
            goto cleanup; // Found and processed, exit loop
        }
    }

cleanup:
    if (event_data) {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// =========================================================================
//                      MQTT Specific Logic
// =========================================================================

static void start_mqtt_connection(module_t *self) {
    mqtt_manager_private_data_t *p_data = (mqtt_manager_private_data_t *)self->private_data;

    service_handle_t id_service_handle = fmw_service_get("main_identity_service");
    if (!id_service_handle) {
        ESP_LOGE(TAG, "Device Identity Service not found! Cannot start MQTT.");
        return;
    }
    device_identity_api_t *id_api = (device_identity_api_t *)id_service_handle;
    const char* device_id = id_api->get_device_id();

    char client_id[64];
    snprintf(client_id, sizeof(client_id), "synapse-%s", device_id);

    // LWT topic is now static from config, but we can still format it if needed
    // For simplicity, we assume it's a sub-topic relative to the base
    char full_lwt_topic[128];
    MQTT_BUILD_TOPIC(full_lwt_topic, sizeof(full_lwt_topic), "synapse/devices/%s/lwt", device_id);


    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = p_data->config.broker_uri,
        .credentials.client_id = client_id,
        .credentials.username = p_data->config.username,
        .credentials.authentication.password = p_data->config.password,
        .session.last_will = {
            .topic = full_lwt_topic,
            .msg = p_data->config.lwt_message,
            .qos = p_data->config.lwt_qos,
            .retain = p_data->config.lwt_retain,
        },
    };

    p_data->client_handle = esp_mqtt_client_init(&mqtt_cfg);
    if (!p_data->client_handle) {
        ESP_LOGE(TAG, "Failed to initialize MQTT client.");
        return;
    }

    esp_mqtt_client_register_event(p_data->client_handle, ESP_EVENT_ANY_ID, mqtt_event_handler_cb, self);
    esp_mqtt_client_start(p_data->client_handle);
    self->status = MODULE_STATUS_RUNNING;
}

/**
 * @internal
 * @brief Callback function for handling MQTT client events.
 * @param handler_args User-defined arguments (pointer to the module instance).
 * @param base The event base.
 * @param event_id The specific event ID.
 * @param event_data The data associated with the event.
 */
static void mqtt_event_handler_cb(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    module_t *self = (module_t *)handler_args;
    mqtt_manager_private_data_t *p_data = (mqtt_manager_private_data_t *)self->private_data;
    esp_mqtt_event_handle_t event = event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
        { // --- FIX: Start of local scope for this case ---
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            p_data->is_connected = true;

            // Publish generic connectivity event
            fmw_connectivity_payload_t *conn_payload = malloc(sizeof(fmw_connectivity_payload_t));
            if (conn_payload)
            {
                snprintf(conn_payload->check_name, sizeof(conn_payload->check_name), "MQTT_Heartbeat");
                event_data_wrapper_t *wrapper;
                if (fmw_event_data_wrap(conn_payload, free, &wrapper) == ESP_OK)
                {
                    fmw_event_bus_post(FMW_EVENT_CONNECTIVITY_ESTABLISHED, wrapper);
                    fmw_event_data_release(wrapper);
                }
                else
                {
                    free(conn_payload);
                }
            }

            // Schedule periodic heartbeat
            service_handle_t timer_service = fmw_service_get("main_timer_service");
            if (timer_service)
            {
                timer_api_t *timer_api = (timer_api_t *)timer_service;
                if (p_data->heartbeat_timer)
                { // Cancel previous timer if it exists
                    timer_api->cancel_event(p_data->heartbeat_timer);
                }
                p_data->heartbeat_timer = timer_api->schedule_event("MQTT_HEARTBEAT_TICK", 120000, true);
                if (p_data->heartbeat_timer)
                {
                    ESP_LOGI(TAG, "MQTT heartbeat timer scheduled.");
                }
                else
                {
                    ESP_LOGE(TAG, "Failed to schedule MQTT heartbeat timer.");
                }
            }

            // Subscribe to command topic
            char cmd_topic[128];
            service_handle_t id_service_handle = fmw_service_get("main_identity_service");
            if (id_service_handle) {
                device_identity_api_t *id_api = (device_identity_api_t *)id_service_handle;
                MQTT_BUILD_TOPIC(cmd_topic, sizeof(cmd_topic), MQTT_TOPIC_SUB_MQTT_MANAGER_COMMAND_IN, id_api->get_device_id());
                esp_mqtt_client_subscribe(p_data->client_handle, cmd_topic, 1);
                ESP_LOGI(TAG, "Subscribed to command topic: %s", cmd_topic);
            }
            break;
        } // --- FIX: End of local scope ---

        case MQTT_EVENT_DISCONNECTED:
        { // --- FIX: Start of local scope for this case ---
            ESP_LOGW(TAG, "MQTT_EVENT_DISCONNECTED");
            p_data->is_connected = false;

            // Cancel heartbeat timer
            service_handle_t timer_service = fmw_service_get("main_timer_service");
            if (timer_service && p_data->heartbeat_timer)
            {
                timer_api_t *timer_api = (timer_api_t *)timer_service;
                timer_api->cancel_event(p_data->heartbeat_timer);
                p_data->heartbeat_timer = NULL;
                ESP_LOGI(TAG, "MQTT heartbeat timer cancelled.");
            }

            // Publish generic connectivity lost event
            fmw_connectivity_payload_t *disconn_payload = malloc(sizeof(fmw_connectivity_payload_t));
            if (disconn_payload)
            {
                snprintf(disconn_payload->check_name, sizeof(disconn_payload->check_name), "MQTT_Heartbeat");
                event_data_wrapper_t *wrapper;
                if (fmw_event_data_wrap(disconn_payload, free, &wrapper) == ESP_OK)
                {
                    fmw_event_bus_post(FMW_EVENT_CONNECTIVITY_LOST, wrapper);
                    fmw_event_data_release(wrapper);
                }
                else
                {
                    free(disconn_payload);
                }
            }
            break;
        } // --- FIX: End of local scope ---

        case MQTT_EVENT_DATA:
        { // --- FIX: Start of local scope for this case ---
            ESP_LOGI(TAG, "MQTT_EVENT_DATA received on topic '%.*s'", event->topic_len, event->topic);
            
            fmw_command_payload_t* payload = malloc(sizeof(fmw_command_payload_t));
            if (payload) {
                int len = event->data_len < sizeof(payload->command_string) - 1 ? event->data_len : sizeof(payload->command_string) - 1;
                strncpy(payload->command_string, event->data, len);
                payload->command_string[len] = '\0';
                strncpy(payload->source, "mqtt", sizeof(payload->source) - 1);

                event_data_wrapper_t *wrapper;
                if (fmw_event_data_wrap(payload, free, &wrapper) == ESP_OK) {
                    ESP_LOGI(TAG, "Forwarding command to Event Bus: '%s'", payload->command_string);
                    fmw_event_bus_post(FMW_EVENT_EXECUTE_COMMAND_STRING, wrapper);
                    fmw_event_data_release(wrapper);
                } else {
                    free(payload);
                }
            }
            break;
        } // --- FIX: End of local scope ---

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                ESP_LOGE(TAG, "Last error code from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
            }
            break;

        default:
            ESP_LOGD(TAG, "Other MQTT event id: %d", (int)event->event_id);
            break;
    }
}

// =========================================================================
//                      Configuration Parsing
// =========================================================================

static esp_err_t parse_mqtt_config(const cJSON *config_json, mqtt_manager_config_t *mqtt_config) {
    if (!config_json || !mqtt_config) return ESP_ERR_INVALID_ARG;

    memset(mqtt_config, 0, sizeof(mqtt_manager_config_t));
    strcpy(mqtt_config->base_topic_prefix, "synapse/devices");
    strcpy(mqtt_config->lwt_message, "{\"status\":\"offline\"}");
    mqtt_config->lwt_qos = 1;
    mqtt_config->lwt_retain = true;

    const cJSON *uri = cJSON_GetObjectItem(config_json, "broker_uri");
    if (!cJSON_IsString(uri) || !uri->valuestring) {
        ESP_LOGE(TAG, "Missing or invalid 'broker_uri' in config.");
        return ESP_ERR_INVALID_ARG;
    }
    strncpy(mqtt_config->broker_uri, uri->valuestring, sizeof(mqtt_config->broker_uri) - 1);

    const cJSON *user = cJSON_GetObjectItem(config_json, "username");
    if (cJSON_IsString(user)) strncpy(mqtt_config->username, user->valuestring, sizeof(mqtt_config->username) - 1);

    const cJSON *pass = cJSON_GetObjectItem(config_json, "password");
    if (cJSON_IsString(pass)) strncpy(mqtt_config->password, pass->valuestring, sizeof(mqtt_config->password) - 1);

    const cJSON *prefix = cJSON_GetObjectItem(config_json, "base_topic_prefix");
    if (cJSON_IsString(prefix)) strncpy(mqtt_config->base_topic_prefix, prefix->valuestring, sizeof(mqtt_config->base_topic_prefix) - 1);
    
    const cJSON *lwt_topic_json = cJSON_GetObjectItem(config_json, "lwt_topic");
    if (cJSON_IsString(lwt_topic_json)) strncpy(mqtt_config->lwt_topic, lwt_topic_json->valuestring, sizeof(mqtt_config->lwt_topic) - 1);

    const cJSON *lwt_msg_json = cJSON_GetObjectItem(config_json, "lwt_message");
    if (cJSON_IsString(lwt_msg_json)) strncpy(mqtt_config->lwt_message, lwt_msg_json->valuestring, sizeof(mqtt_config->lwt_message) - 1);

    const cJSON *lwt_qos_json = cJSON_GetObjectItem(config_json, "lwt_qos");
    if (cJSON_IsNumber(lwt_qos_json)) mqtt_config->lwt_qos = lwt_qos_json->valueint;

    const cJSON *lwt_retain_json = cJSON_GetObjectItem(config_json, "lwt_retain");
    if (cJSON_IsBool(lwt_retain_json)) mqtt_config->lwt_retain = cJSON_IsTrue(lwt_retain_json);

    return ESP_OK;
}