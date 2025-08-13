/**
 * @file task_pool_manager_internal.h
 * @brief Internal Core API for the Task Pool Manager.
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-09-06
 */

#ifndef SYNAPSE_TASK_POOL_MANAGER_INTERNAL_H
#define SYNAPSE_TASK_POOL_MANAGER_INTERNAL_H

#include "synapse.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the Task Pool Manager component.
 * @details Must be called once by the System Manager during startup.
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t synapse_task_pool_init(void);

#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_TASK_POOL_MANAGER_INTERNAL_H