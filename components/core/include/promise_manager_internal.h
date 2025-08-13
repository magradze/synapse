/**
 * @file promise_manager_internal.h
 * @brief Internal Core API for creating and fulfilling promises.
 * @author Giorgi Magradze
 * @version 2.0.0
 * @date 2025-08-30
 * @details This header is for use by service provider modules that implement
 *          asynchronous, promise-based APIs.
 */

#ifndef SYNAPSE_PROMISE_MANAGER_INTERNAL_H
#define SYNAPSE_PROMISE_MANAGER_INTERNAL_H

#include "promise_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @brief Initializes the Promise Manager component.
   */
  esp_err_t synapse_promise_manager_init(void);

  /**
   * @brief Creates a new promise with pre-registered callbacks.
   * @details Called by a service provider at the start of an async operation.
   * @param[in] then_cb The function to call when the promise is resolved.
   * @param[in] catch_cb The function to call when the promise is rejected.
   * @param[in] user_context A context pointer to be passed to the callbacks.
   * @return A new promise_handle_t, or NULL on failure.
   */
  promise_handle_t synapse_promise_create(promise_then_cb then_cb, promise_catch_cb catch_cb, void *user_context);

  /**
   * @brief Resolves a promise with a successful result.
   * @param[in] handle The handle of the promise to resolve.
   * @param[in] result_data A pointer to the result data.
   * @param[in] free_fn A function to free the result_data. If NULL, data is assumed static.
   * @return ESP_OK on success.
   */
  esp_err_t synapse_promise_resolve(promise_handle_t handle, void *result_data, void (*free_fn)(void *));

  /**
   * @brief Rejects a promise with an error.
   * @param[in] handle The handle of the promise to reject.
   * @param[in] error_data A pointer to the error data.
   * @param[in] free_fn A function to free the error_data. If NULL, data is assumed static.
   * @return ESP_OK on success.
   */
  esp_err_t synapse_promise_reject(promise_handle_t handle, void *error_data, void (*free_fn)(void *));

#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_PROMISE_MANAGER_INTERNAL_H