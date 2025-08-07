/**
 * @file promise_manager_internal.h
 * @brief Internal Core API for creating and fulfilling promises.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-08-29
 * @details This header is intended for use only by service provider modules within
 *          the Synapse Framework that need to implement asynchronous, promise-based APIs.
 *          It should NOT be included by general-purpose modules (consumers).
 */

#ifndef FMW_PROMISE_MANAGER_INTERNAL_H
#define FMW_PROMISE_MANAGER_INTERNAL_H

#include "promise_manager.h" // Includes the public handle and callback types

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the Promise Manager component.
 * @details Must be called once by the System Manager during startup.
 *          This sets up the internal registry, queue, and the background task.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t fmw_promise_manager_init(void);

/**
 * @brief Creates a new promise and returns its handle.
 * @details Called by a service provider (e.g., wifi_manager) at the start of an
 *          asynchronous operation. The returned handle should be given to the
 *          original caller.
 * @return A new promise_handle_t, or NULL on failure (e.g., out of memory).
 */
promise_handle_t fmw_promise_create(void);

/**
 * @brief Resolves a promise with a successful result.
 * @details This action will queue the promise for execution, eventually triggering
 *          the 'then' callback on the consumer's side. The promise manager takes
 *          ownership of the result_data pointer and will free it according to free_fn.
 * @param[in] handle The handle of the promise to resolve.
 * @param[in] result_data A pointer to the result data. Can be NULL.
 * @param[in] free_fn A function pointer to free the result_data. If NULL, standard `free()` is used.
 * @return
 *      - ESP_OK: If the promise was successfully queued for resolution.
 *      - ESP_ERR_INVALID_ARG: If the handle is invalid or not found.
 *      - ESP_ERR_INVALID_STATE: If the promise is not in a PENDING state.
 */
esp_err_t fmw_promise_resolve(promise_handle_t handle, void* result_data, void (*free_fn)(void*));

/**
 * @brief Rejects a promise with an error.
 * @details This action will queue the promise for execution, eventually triggering
 *          the 'catch' callback on the consumer's side. The promise manager takes
 *          ownership of the error_data pointer.
 * @param[in] handle The handle of the promise to reject.
 * @param[in] error_data A pointer to the error data. Can be NULL.
 * @param[in] free_fn A function pointer to free the error_data. If NULL, standard `free()` is used.
 * @return
 *      - ESP_OK: If the promise was successfully queued for rejection.
 *      - ESP_ERR_INVALID_ARG: If the handle is invalid or not found.
 *      - ESP_ERR_INVALID_STATE: If the promise is not in a PENDING state.
 */
esp_err_t fmw_promise_reject(promise_handle_t handle, void* error_data, void (*free_fn)(void*));

#ifdef __cplusplus
}
#endif

#endif // FMW_PROMISE_MANAGER_INTERNAL_H