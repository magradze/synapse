/**
 * @file wifi_manager_internal.h
 * @brief Internal declarations for the WiFi Manager module.
 * @author Synapse Framework Team
 * @version 1.3.0
 * @date 2025-09-03
 * @details This header is for internal use within the wifi_manager module's
 *          source files. It defines the private data structure, internal
 *          command types, and forward-declares ALL functions that are shared
 *          across the module's different C files.
 *          It should NOT be included by any external module.
 */

#ifndef WIFI_MANAGER_INTERNAL_H
#define WIFI_MANAGER_INTERNAL_H

#include "wifi_manager.h"
#include "synapse.h"
#include "wifi_interface.h"
#include "storage_interface.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "esp_wifi.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Internal Command & Queue Message Structures ---

typedef enum
{
    WIFI_CMD_CONNECT,
    WIFI_CMD_DISCONNECT,
    WIFI_CMD_RECONNECT_TIMER_FIRED,
    WIFI_CMD_GET_STATUS_ASYNC,
} wifi_cmd_type_t;

typedef struct
{
    promise_handle_t promise;
} wifi_get_status_cmd_payload_t;

typedef struct
{
    wifi_cmd_type_t type;
    union
    {
        wifi_get_status_cmd_payload_t get_status;
    } payload;
} wifi_queue_msg_t;


// --- Private Data Structure ---

typedef struct
{
    storage_api_t *storage_handle;
    char instance_name[CONFIG_WIFI_MANAGER_INSTANCE_NAME_MAX_LEN];
    bool is_connected;
    wifi_config_t wifi_config;
    int retry_num;
    bool has_saved_credentials;
    TaskHandle_t task_handle;
    QueueHandle_t cmd_queue;
    TimerHandle_t reconnect_timer;
    wifi_api_t service_api;
} wifi_manager_private_data_t;


// --- Forward Declarations for functions shared across .c files ---

// Implemented in: wifi_manager.c
void wifi_task(void *pvParameters);
void reconnect_timer_callback(TimerHandle_t xTimer);
esp_err_t save_credentials(module_t *self, const char *ssid, const char *password);
esp_err_t load_credentials(module_t *self);
void start_wifi_connection(module_t *self);

// Implemented in: wifi_manager_api.c
cJSON *build_status_json(module_t *self);
esp_err_t wifi_api_get_status_async(void *context, promise_then_cb then_cb, promise_catch_cb catch_cb, void *user_context);
bool wifi_api_is_connected(void *context);

// Implemented in: wifi_manager_cmd.c
void register_cli_commands(module_t *self);

// Implemented in: wifi_manager_events.c
void wifi_manager_handle_event(module_t *self, const char *event_name, void *event_data);
void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);


#ifdef __cplusplus
}
#endif

#endif // WIFI_MANAGER_INTERNAL_H