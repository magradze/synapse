/**
 * @file task_pool_manager.h
 * @brief Public API for the Shared Task Pool Manager.
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-09-06
 * @details This core component provides a pool of worker tasks to execute
 *          short, periodic, or one-shot jobs on behalf of other modules.
 *          This significantly reduces system overhead by avoiding the need
 *          for each module to create its own task for simple background work.
 */

#ifndef SYNAPSE_TASK_POOL_MANAGER_H
#define SYNAPSE_TASK_POOL_MANAGER_H

#include "synapse.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque handle to a scheduled job.
 */
typedef struct synapse_job_t* synapse_job_handle_t;

/**
 * @brief Callback function type for a job to be executed by a worker task.
 * @param user_context A pointer to user-defined context provided when the job was scheduled.
 */
typedef void (*synapse_job_cb)(void* user_context);

/**
 * @brief Schedules a new job for execution by the task pool.
 *
 * @param[in] job_function The function to be executed.
 * @param[in] user_context A context pointer to be passed to the job function.
 * @param[in] interval_ms The interval in milliseconds for periodic execution.
 *                        If 0, the job is a one-shot and will run only once.
 * @param[in] is_periodic If true, the job will be rescheduled after each execution.
 *
 * @return A handle to the scheduled job, or NULL on failure. The handle is
 *         required to cancel the job later.
 */
synapse_job_handle_t synapse_task_pool_schedule_job(
    synapse_job_cb job_function,
    void* user_context,
    uint32_t interval_ms,
    bool is_periodic);

/**
 * @brief Cancels a previously scheduled job.
 *
 * @param[in] handle The handle of the job to cancel, obtained from schedule_job.
 * @return
 *      - ESP_OK: If the job was successfully found and cancelled.
 *      - ESP_ERR_INVALID_ARG: If the handle is NULL.
 *      - ESP_ERR_NOT_FOUND: If no job with the given handle was found.
 */
esp_err_t synapse_task_pool_cancel_job(synapse_job_handle_t handle);


#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_TASK_POOL_MANAGER_H