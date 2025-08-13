/**
 * @file task_pool_manager.c
 * @brief Implementation of the Shared Task Pool Manager.
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-09-06
 */
#include "task_pool_manager.h"
#include "task_pool_manager_internal.h"
#include "logging.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include <sys/queue.h>

DEFINE_COMPONENT_TAG("TASK_POOL_MANAGER", SYNAPSE_LOG_COLOR_BLUE);

// --- Internal Structures ---

typedef struct synapse_job_t {
    synapse_job_cb function;
    void* context;
    uint32_t interval_ms;
    bool is_periodic;
    uint64_t next_execution_time_us;
    SLIST_ENTRY(synapse_job_t) entries;
} synapse_job_t;

// --- Static Globals ---

static SLIST_HEAD(job_list_head, synapse_job_t) scheduled_jobs_list;
static QueueHandle_t job_execution_queue = NULL;
static SemaphoreHandle_t job_list_mutex = NULL;
static TimerHandle_t scheduler_timer = NULL;

// --- Forward Declarations ---
static void worker_task(void* pvParameters);
static void scheduler_timer_callback(TimerHandle_t xTimer);

// --- Core Initialization ---

esp_err_t synapse_task_pool_init(void)
{
    ESP_LOGI(TAG, "Initializing Task Pool Manager...");

    SLIST_INIT(&scheduled_jobs_list);

    job_list_mutex = xSemaphoreCreateMutex();
    if (!job_list_mutex) {
        ESP_LOGE(TAG, "Failed to create job list mutex");
        return ESP_ERR_NO_MEM;
    }

    job_execution_queue = xQueueCreate(CONFIG_SYNAPSE_TASK_POOL_QUEUE_LENGTH, sizeof(synapse_job_t*));
    if (!job_execution_queue) {
        ESP_LOGE(TAG, "Failed to create job execution queue");
        vSemaphoreDelete(job_list_mutex);
        return ESP_ERR_NO_MEM;
    }

    for (int i = 0; i < CONFIG_SYNAPSE_TASK_POOL_SIZE; i++) {
        char task_name[16];
        snprintf(task_name, sizeof(task_name), "worker_task_%d", i);
        if (xTaskCreate(worker_task, task_name, CONFIG_SYNAPSE_TASK_POOL_TASK_STACK_SIZE, NULL, CONFIG_SYNAPSE_TASK_POOL_TASK_PRIORITY, NULL) != pdPASS) {
            ESP_LOGE(TAG, "Failed to create worker task %d", i);
            // Cleanup previously created resources
            vQueueDelete(job_execution_queue);
            vSemaphoreDelete(job_list_mutex);
            return ESP_FAIL;
        }
    }

    scheduler_timer = xTimerCreate("scheduler_timer", pdMS_TO_TICKS(10), pdTRUE, NULL, scheduler_timer_callback);
    if (!scheduler_timer || xTimerStart(scheduler_timer, 0) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create or start scheduler timer");
        // Cleanup...
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Task Pool Manager initialized with %d worker tasks.", CONFIG_SYNAPSE_TASK_POOL_SIZE);
    return ESP_OK;
}

// --- Public API Implementation ---

synapse_job_handle_t synapse_task_pool_schedule_job(synapse_job_cb job_function, void* user_context, uint32_t interval_ms, bool is_periodic)
{
    if (!job_function) {
        return NULL;
    }

    synapse_job_t* new_job = (synapse_job_t*)calloc(1, sizeof(synapse_job_t));
    if (!new_job) {
        ESP_LOGE(TAG, "Failed to allocate memory for a new job.");
        return NULL;
    }

    new_job->function = job_function;
    new_job->context = user_context;
    new_job->interval_ms = interval_ms;
    new_job->is_periodic = is_periodic;
    new_job->next_execution_time_us = esp_timer_get_time() + (is_periodic ? (uint64_t)interval_ms * 1000 : 0);

    if (xSemaphoreTake(job_list_mutex, portMAX_DELAY) == pdTRUE) {
        SLIST_INSERT_HEAD(&scheduled_jobs_list, new_job, entries);
        xSemaphoreGive(job_list_mutex);
        ESP_LOGD(TAG, "Scheduled new job. Periodic: %d, Interval: %" PRIu32 "ms", is_periodic, interval_ms);
        return (synapse_job_handle_t)new_job;
    }

    free(new_job);
    return NULL;
}

esp_err_t synapse_task_pool_cancel_job(synapse_job_handle_t handle)
{
    if (!handle) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t result = ESP_ERR_NOT_FOUND;
    if (xSemaphoreTake(job_list_mutex, portMAX_DELAY) == pdTRUE) {
        synapse_job_t* job_to_remove = (synapse_job_t*)handle;
        
        // Check if the job is in the list before removing
        synapse_job_t* it;
        bool found = false;
        SLIST_FOREACH(it, &scheduled_jobs_list, entries) {
            if (it == job_to_remove) {
                found = true;
                break;
            }
        }

        if (found) {
            SLIST_REMOVE(&scheduled_jobs_list, job_to_remove, synapse_job_t, entries);
            free(job_to_remove);
            result = ESP_OK;
            ESP_LOGD(TAG, "Cancelled and freed job handle %p", handle);
        }
        xSemaphoreGive(job_list_mutex);
    }
    return result;
}

// --- Internal Task and Timer Callback ---

static void worker_task(void* pvParameters)
{
    while (1) {
        synapse_job_t* job_to_execute;
        if (xQueueReceive(job_execution_queue, &job_to_execute, portMAX_DELAY) == pdPASS) {
            if (job_to_execute && job_to_execute->function) {
                ESP_LOGD(TAG, "Worker task executing job %p", job_to_execute);
                job_to_execute->function(job_to_execute->context);
            }

            // If the job was a one-shot, it's now done. Free it.
            if (job_to_execute && !job_to_execute->is_periodic) {
                free(job_to_execute);
            }
        }
    }
}

static void scheduler_timer_callback(TimerHandle_t xTimer)
{
    if (xSemaphoreTake(job_list_mutex, 0) == pdTRUE) {
        uint64_t current_time_us = esp_timer_get_time();
        synapse_job_t* job;
        synapse_job_t* temp_job;

        SLIST_FOREACH_SAFE(job, &scheduled_jobs_list, entries, temp_job) {
            if (current_time_us >= job->next_execution_time_us) {
                // Time to execute this job. Remove from scheduled list.
                SLIST_REMOVE(&scheduled_jobs_list, job, synapse_job_t, entries);

                // Send to worker queue
                if (xQueueSend(job_execution_queue, &job, 0) != pdPASS) {
                    ESP_LOGW(TAG, "Job queue is full. Dropping job.");
                    // If it's periodic, we must re-insert it. Otherwise, free it.
                    if (job->is_periodic) {
                        job->next_execution_time_us = current_time_us + (uint64_t)job->interval_ms * 1000;
                        SLIST_INSERT_HEAD(&scheduled_jobs_list, job, entries);
                    } else {
                        free(job);
                    }
                } else {
                    // If it's periodic, calculate next run time and re-insert into the list
                    if (job->is_periodic) {
                        job->next_execution_time_us = current_time_us + (uint64_t)job->interval_ms * 1000;
                        SLIST_INSERT_HEAD(&scheduled_jobs_list, job, entries);
                    }
                }
            }
        }
        xSemaphoreGive(job_list_mutex);
    }
}