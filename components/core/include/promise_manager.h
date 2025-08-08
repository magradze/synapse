/**
 * @file promise_manager.h
 * @brief Public API for consuming asynchronous operations using a Promise pattern.
 * @author Synapse Framework Team
 * @version 2.0.0
 * @date 2025-08-30
 * @details This header defines the types used for promise-based operations.
 *          The actual promise creation and handling is done via service provider APIs.
 */

#ifndef SYNAPSE_PROMISE_MANAGER_H
#define SYNAPSE_PROMISE_MANAGER_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @brief Opaque handle to a promise object.
   * @details This handle is an internal concept and is no longer directly returned to consumers.
   */
  typedef struct promise_t *promise_handle_t;

  /**
   * @brief Callback function type for a successfully resolved promise.
   * @param result_data Pointer to the data returned by the async operation.
   * @param user_context A pointer to user-defined context provided when the operation was initiated.
   */
  typedef void (*promise_then_cb)(void *result_data, void *user_context);

  /**
   * @brief Callback function type for a rejected (failed) promise.
   * @param error_data Pointer to data describing the error.
   * @param user_context A pointer to user-defined context.
   */
  typedef void (*promise_catch_cb)(void *error_data, void *user_context);

  // NOTE: synapse_promise_then, synapse_promise_catch, and synapse_promise_destroy are now removed
  // from the public API as they are no longer needed with the new architecture.

#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_PROMISE_MANAGER_H