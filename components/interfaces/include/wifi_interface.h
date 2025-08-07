/**
 * @file wifi_interface.h
 * @brief Public Service API contract for the WiFi Manager module.
 * @author Synapse Framework Team
 * @version 2.0.0
 * @date 2025-08-30
 */

#ifndef FMW_WIFI_INTERFACE_H
#define FMW_WIFI_INTERFACE_H

#include "synapse.h" // For promise callback types

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    /**
     * @brief Asynchronously retrieves the current WiFi status.
     * @details This function is non-blocking. It initiates the status retrieval
     *          and will invoke one of the provided callbacks upon completion.
     *
     * @param[in] context The context handle for the specific WiFi Manager instance.
     * @param[in] then_cb The callback to execute on success. The `result_data` will be a
     *                    statically buffered, null-terminated JSON string.
     * @param[in] catch_cb The callback to execute on failure.
     * @param[in] user_context A user-defined context pointer to be passed to the callbacks.
     *
     * @return 
     *      - ESP_OK: If the asynchronous operation was successfully started.
     *      - ESP_ERR_NO_MEM: If a promise object could not be created.
     *      - ESP_FAIL: If the command could not be queued to the wifi task.
     */
    esp_err_t (*get_status_async)(void* context, promise_then_cb then_cb, promise_catch_cb catch_cb, void* user_context);

    /**
     * @brief Checks if the device is currently connected to a WiFi network.
     */
    bool (*is_connected)(void* context);

} wifi_api_t;

#ifdef __cplusplus
}
#endif

#endif // FMW_WIFI_INTERFACE_H