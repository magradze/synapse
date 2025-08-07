/**
 * @file promise_manager.h
 * @brief Public API for consuming asynchronous operations using a Promise pattern.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-08-29
 * @details This header defines the consumer-facing API for interacting with promises.
 *          Modules that initiate an asynchronous operation will receive a promise handle
 *          and can use the functions defined here to register callbacks for success
 *          or failure scenarios.
 */

#ifndef FMW_PROMISE_MANAGER_H
#define FMW_PROMISE_MANAGER_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque handle to a promise object.
 * @details This handle represents a future result of an asynchronous operation.
 */
typedef struct promise_t* promise_handle_t;

/**
 * @brief Callback function type for a successfully resolved promise.
 * @param result_data Pointer to the data returned by the async operation.
 *                    The callback is responsible for casting it to the correct type
 *                    and for understanding its memory model (e.g., if it needs freeing).
 * @param user_context A pointer to user-defined context provided when the callback was registered.
 */
typedef void (*promise_then_cb)(void* result_data, void* user_context);

/**
 * @brief Callback function type for a rejected (failed) promise.
 * @param error_data Pointer to data describing the error.
 * @param user_context A pointer to user-defined context.
 */
typedef void (*promise_catch_cb)(void* error_data, void* user_context);

/**
 * @brief Registers a callback to be executed upon successful completion of the promise.
 * @details If the promise has already been resolved, the callback may be executed immediately
 *          from the context of the promise manager's task.
 * @param[in] handle The promise handle returned by an asynchronous API call.
 * @param[in] then_cb The function to call when the promise is resolved.
 * @param[in] user_context A context pointer to be passed to the callback. Can be NULL.
 * @return
 *      - ESP_OK: If the callback was successfully registered.
 *      - ESP_ERR_INVALID_ARG: If the handle or callback is NULL.
 *      - ESP_ERR_INVALID_STATE: If a 'then' callback is already registered for this promise.
 */
esp_err_t fmw_promise_then(promise_handle_t handle, promise_then_cb then_cb, void* user_context);

/**
 * @brief Registers a callback to be executed upon failure of the promise.
 * @details If the promise has already been rejected, the callback may be executed immediately.
 * @param[in] handle The promise handle.
 * @param[in] catch_cb The function to call when the promise is rejected.
 * @param[in] user_context A context pointer to be passed to the callback. Can be NULL.
 * @return
 *      - ESP_OK: If the callback was successfully registered.
 *      - ESP_ERR_INVALID_ARG: If the handle or callback is NULL.
 *      - ESP_ERR_INVALID_STATE: If a 'catch' callback is already registered for this promise.
 */
esp_err_t fmw_promise_catch(promise_handle_t handle, promise_catch_cb catch_cb, void* user_context);

/**
 * @brief Destroys a promise handle and releases its associated resources.
 * @details This function should be called by the consumer if it is no longer interested
 *          in the result of the promise. It signals to the promise manager that the
 *          resources can be cleaned up. The manager will also automatically clean up
 *          promises after they are executed.
 * @param[in] handle The promise handle to destroy. Can be NULL.
 */
void fmw_promise_destroy(promise_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif // FMW_PROMISE_MANAGER_H