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

/**
 * @internal
 * @enum wifi_cmd_type_t
 * @brief Defines the types of commands that can be sent to the internal WiFi task queue.
 */
typedef enum
{
    WIFI_CMD_CONNECT,               /**< @brief Command to initiate a connection to the configured AP. */
    WIFI_CMD_DISCONNECT,            /**< @brief Command to disconnect from the current AP. */
    WIFI_CMD_RECONNECT_TIMER_FIRED, /**< @brief Internal command triggered by the reconnect timer. */
    WIFI_CMD_GET_STATUS_ASYNC,      /**< @brief Command to asynchronously fetch the current WiFi status. */
} wifi_cmd_type_t;

/**
 * @internal
 * @struct wifi_get_status_cmd_payload_t
 * @brief Payload for the WIFI_CMD_GET_STATUS_ASYNC command.
 */
typedef struct
{
    promise_handle_t promise; /**< @brief The promise handle to be resolved with the status information. */
} wifi_get_status_cmd_payload_t;

/**
 * @internal
 * @struct wifi_queue_msg_t
 * @brief The structure for a message sent to the WiFi task's command queue.
 */
typedef struct
{
    wifi_cmd_type_t type; /**< @brief The type of the command. */
    union
    {
        wifi_get_status_cmd_payload_t get_status; /**< @brief Payload specific to the get_status command. */
    } payload;
} wifi_queue_msg_t;


// --- Private Data Structure ---

/**
 * @internal
 * @struct wifi_manager_private_data_t
 * @brief The complete internal state of a WiFi Manager module instance.
 */
typedef struct
{
    storage_api_t *storage_handle;                                 /**< @brief Injected: Handle to the storage service for credentials. */
    char instance_name[CONFIG_WIFI_MANAGER_INSTANCE_NAME_MAX_LEN]; /**< @brief The unique name of this module instance. */
    bool is_connected;                                             /**< @brief Runtime flag indicating if the device is currently connected to an AP and has an IP. */
    wifi_config_t wifi_config;                                     /**< @brief The current WiFi configuration (SSID/password). */
    int retry_num;                                                 /**< @brief Counter for connection retry attempts. */
    bool has_saved_credentials;                                    /**< @brief Flag indicating if valid credentials were loaded from storage. */
    TaskHandle_t task_handle;                                      /**< @brief Handle to the dedicated WiFi processing task. */
    QueueHandle_t cmd_queue;                                       /**< @brief Queue for sending commands to the WiFi task. */
    TimerHandle_t reconnect_timer;                                 /**< @brief FreeRTOS timer for scheduling reconnection attempts. */
    wifi_api_t service_api;                                        /**< @brief The structure containing the public Service API function pointers. */
} wifi_manager_private_data_t;


// --- Forward Declarations for functions shared across .c files ---

// --- Implemented in: wifi_manager.c ---
/**
 * @internal @brief The main FreeRTOS task for the WiFi Manager.
 * @param[in] pvParameters A pointer to the module instance (`module_t*`).
 */
void wifi_task(void *pvParameters);

/**
 * @internal @brief Callback function for the reconnect timer.
 * @param[in] xTimer The handle of the timer that expired.
 */
void reconnect_timer_callback(TimerHandle_t xTimer);

/**
 * @internal @brief Saves new WiFi credentials to persistent storage.
 * @param[in] self A pointer to the module instance.
 * @param[in] ssid The SSID to save.
 * @param[in] password The password to save.
 * @return ESP_OK on success.
 */
esp_err_t save_credentials(module_t *self, const char *ssid, const char *password);

/**
 * @internal @brief Loads WiFi credentials from persistent storage into the private data struct.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK if credentials were found and loaded, ESP_ERR_NOT_FOUND otherwise.
 */
esp_err_t load_credentials(module_t *self);

/**
 * @internal @brief Posts a connect command to the internal task queue.
 * @param[in] self A pointer to the module instance.
 */
void start_wifi_connection(module_t *self);

// --- Implemented in: wifi_manager_api.c ---
/**
 * @internal @brief Builds a cJSON object representing the current WiFi status.
 * @param[in] self A pointer to the module instance.
 * @return A pointer to the created cJSON object, or NULL on failure. The caller is responsible for deleting the object.
 */
cJSON *build_status_json(module_t *self);

/**
 * @internal @brief Service API implementation for asynchronously getting the WiFi status.
 * @see wifi_api_t
 */
esp_err_t wifi_api_get_status_async(void *context, promise_then_cb then_cb, promise_catch_cb catch_cb, void *user_context);

/**
 * @internal @brief Service API implementation for synchronously checking the connection status.
 * @see wifi_api_t
 */
bool wifi_api_is_connected(void *context);

// --- Implemented in: wifi_manager_cmd.c ---
/**
 * @internal @brief Registers all CLI commands for the WiFi Manager.
 * @param[in] self A pointer to the module instance.
 */
void register_cli_commands(module_t *self);

// --- Implemented in: wifi_manager_events.c ---
/**
 * @internal @brief Handles events from the Synapse Event Bus.
 * @param[in] self A pointer to the module instance.
 * @param[in] event_name The name of the event.
 * @param[in] event_data A pointer to the event's data wrapper.
 */
void wifi_manager_handle_event(module_t *self, const char *event_name, void *event_data);

/**
 * @internal @brief Handles system-level WiFi events from the ESP-IDF event loop.
 */
void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/**
 * @internal @brief Handles system-level IP events from the ESP-IDF event loop.
 */
void ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

// --- Implemented in: wifi_manager_ui.c ---
/**
 * @internal @brief Initializes and registers the module's UI components.
 * @param[in] self A pointer to the module instance.
 */
void wifi_ui_init(module_t *self);

/**
 * @internal @brief Deinitializes and unregisters the module's UI components.
 * @param[in] self A pointer to the module instance.
 */
void wifi_ui_deinit(module_t *self);

#ifdef __cplusplus
}
#endif

#endif // WIFI_MANAGER_INTERNAL_H