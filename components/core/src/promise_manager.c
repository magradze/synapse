/**
 * @file promise_manager.c
 * @brief Implementation of the asynchronous Promise Manager.
 * @author Giorgi Magradze
 * @version 2.0.0
 * @date 2025-08-30
 * @details This component manages the lifecycle of promises. It uses a dedicated
 *          FreeRTOS task to execute callbacks, ensuring they run in a safe,
 *          isolated context. The new architecture requires callbacks to be
 *          provided at promise creation time, eliminating race conditions.
 */
#include "promise_manager.h"
#include "promise_manager_internal.h"
#include "logging.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <sys/queue.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("PROMISE_MANAGER", SYNAPSE_LOG_COLOR_BLUE);

// --- Kconfig Definitions ---
#define PROMISE_QUEUE_LENGTH CONFIG_SYNAPSE_PROMISE_QUEUE_LENGTH
#define PROMISE_TASK_STACK_SIZE CONFIG_SYNAPSE_PROMISE_TASK_STACK_SIZE
#define PROMISE_TASK_PRIORITY CONFIG_SYNAPSE_PROMISE_TASK_PRIORITY

// --- Internal Structures ---

typedef enum
{
    PROMISE_STATE_PENDING,
    PROMISE_STATE_RESOLVED,
    PROMISE_STATE_REJECTED
} promise_state_t;

typedef struct promise_t
{
    uint32_t id;
    promise_state_t state;

    promise_then_cb then_callback;
    promise_catch_cb catch_callback;
    void *user_context;

    void *result_data;
    void (*free_fn)(void *);

    SLIST_ENTRY(promise_t)
    entries;
} promise_t;

typedef struct
{
    promise_handle_t handle;
} promise_message_t;

// --- Static Globals ---

static SLIST_HEAD(promise_list_head, promise_t) promise_registry_head;
static SemaphoreHandle_t registry_mutex = NULL;
static QueueHandle_t promise_execution_queue = NULL;
static uint32_t next_promise_id = 1;

// --- Forward Declarations ---
static void promise_manager_task(void *params);
static void cleanup_promise(promise_t *promise);

// --- Core Initialization ---

esp_err_t synapse_promise_manager_init(void)
{
    ESP_LOGI(TAG, "Initializing Promise Manager...");
    SLIST_INIT(&promise_registry_head);

    registry_mutex = xSemaphoreCreateMutex();
    if (!registry_mutex)
    {
        ESP_LOGE(TAG, "Failed to create registry mutex");
        return ESP_ERR_NO_MEM;
    }

    promise_execution_queue = xQueueCreate(PROMISE_QUEUE_LENGTH, sizeof(promise_message_t));
    if (!promise_execution_queue)
    {
        ESP_LOGE(TAG, "Failed to create execution queue");
        vSemaphoreDelete(registry_mutex);
        return ESP_ERR_NO_MEM;
    }

    if (xTaskCreate(promise_manager_task, "promise_task", PROMISE_TASK_STACK_SIZE, NULL, PROMISE_TASK_PRIORITY, NULL) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create promise manager task");
        vQueueDelete(promise_execution_queue);
        vSemaphoreDelete(registry_mutex);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Promise Manager initialized successfully.");
    return ESP_OK;
}

// --- Internal API Implementation ---

promise_handle_t synapse_promise_create(promise_then_cb then_cb, promise_catch_cb catch_cb, void *user_context)
{
    promise_t *promise = (promise_t *)calloc(1, sizeof(promise_t));
    if (!promise)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for new promise");
        return NULL;
    }

    if (xSemaphoreTake(registry_mutex, portMAX_DELAY) != pdTRUE)
    {
        free(promise);
        return NULL;
    }

    promise->id = next_promise_id++;
    promise->state = PROMISE_STATE_PENDING;
    promise->then_callback = then_cb;
    promise->catch_callback = catch_cb;
    promise->user_context = user_context;

    SLIST_INSERT_HEAD(&promise_registry_head, promise, entries);
    xSemaphoreGive(registry_mutex);

    ESP_LOGD(TAG, "Created promise with ID %" PRIu32, promise->id);
    return (promise_handle_t)promise;
}

static esp_err_t synapse_promise_fulfill(promise_handle_t handle, void *data, void (*free_fn)(void *), bool is_resolve)
{
    promise_t *promise = (promise_t *)handle;
    if (!promise)
        return ESP_ERR_INVALID_ARG;

    if (xSemaphoreTake(registry_mutex, portMAX_DELAY) != pdTRUE)
    {
        return ESP_ERR_TIMEOUT;
    }

    // Verify the promise is still pending before fulfilling
    if (promise->state != PROMISE_STATE_PENDING)
    {
        xSemaphoreGive(registry_mutex);
        ESP_LOGW(TAG, "Attempted to fulfill a promise (ID %" PRIu32 ") that is not pending.", promise->id);
        if (data && free_fn)
        {
            free_fn(data);
        }
        return ESP_ERR_INVALID_STATE;
    }

    promise->state = is_resolve ? PROMISE_STATE_RESOLVED : PROMISE_STATE_REJECTED;
    promise->result_data = data;
    promise->free_fn = free_fn;

    xSemaphoreGive(registry_mutex);

    promise_message_t msg = {.handle = handle};
    if (xQueueSend(promise_execution_queue, &msg, 0) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to queue promise (ID %" PRIu32 ") for execution.", promise->id);
        cleanup_promise(promise);
        return ESP_FAIL;
    }

    ESP_LOGD(TAG, "Queued promise ID %" PRIu32 " for %s.", promise->id, is_resolve ? "resolution" : "rejection");
    return ESP_OK;
}

esp_err_t synapse_promise_resolve(promise_handle_t handle, void *result_data, void (*free_fn)(void *))
{
    return synapse_promise_fulfill(handle, result_data, free_fn, true);
}

esp_err_t synapse_promise_reject(promise_handle_t handle, void *error_data, void (*free_fn)(void *))
{
    return synapse_promise_fulfill(handle, error_data, free_fn, false);
}

// --- Internal Helper Functions ---

static void cleanup_promise(promise_t *promise)
{
    if (!promise)
        return;

    if (xSemaphoreTake(registry_mutex, portMAX_DELAY) == pdTRUE)
    {
        SLIST_REMOVE(&promise_registry_head, promise, promise_t, entries);
        xSemaphoreGive(registry_mutex);
    }

    if (promise->result_data && promise->free_fn)
    {
        promise->free_fn(promise->result_data);
    }

    free(promise);
}

static void promise_manager_task(void *params)
{
    promise_message_t msg;
    while (1)
    {
        if (xQueueReceive(promise_execution_queue, &msg, portMAX_DELAY) == pdPASS)
        {
            promise_t *promise = (promise_t *)msg.handle;
            if (!promise)
                continue;

            if (promise->state == PROMISE_STATE_RESOLVED && promise->then_callback)
            {
                ESP_LOGD(TAG, "Executing 'then' callback for promise ID %" PRIu32, promise->id);
                promise->then_callback(promise->result_data, promise->user_context);
            }
            else if (promise->state == PROMISE_STATE_REJECTED && promise->catch_callback)
            {
                ESP_LOGD(TAG, "Executing 'catch' callback for promise ID %" PRIu32, promise->id);
                promise->catch_callback(promise->result_data, promise->user_context);
            }

            cleanup_promise(promise);
        }
    }
}