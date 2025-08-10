/**
 * @file system_timer.c
 * @brief A centralized service for scheduling event publishing.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-06
 * @details This module provides a centralized service that allows other modules
 *          to schedule one-shot or periodic events to be posted on the Event Bus.
 *          It abstracts the underlying `esp_timer` mechanism, providing a simpler,
 *          decoupled, and resource-efficient way to handle timed tasks.
 */

// --- Synapse Framework Includes ---
#include "synapse.h"
#include "system_timer.h"
#include "timer_interface.h"

// --- Standard & ESP-IDF Includes ---
#include "esp_timer.h"
#include <string.h>
#include <stdlib.h>
#include <inttypes.h> // For PRIu32 macro

// --- Component Tag ---
DEFINE_COMPONENT_TAG("SYSTEM_TIMER", SYNAPSE_LOG_COLOR_BLUE);

// --- Private Data Structures ---

/**
 * @internal
 * @struct timer_entry_t
 * @brief Internal structure to hold information about a single scheduled timer.
 */
typedef struct {
    esp_timer_handle_t esp_handle; /**< The underlying handle from esp_timer. */
    const char* event_name;        /**< The event name to post when the timer fires. Must be a persistent string. */
    bool is_active;                /**< Flag to indicate if this slot in the pool is in use. */
} timer_entry_t;

/**
 * @internal
 * @struct system_timer_private_data_t
 * @brief Private data for the System Timer module.
 */
typedef struct {
    char instance_name[CONFIG_SYSTEM_TIMER_INSTANCE_NAME_MAX_LEN];
    timer_entry_t timer_pool[CONFIG_SYSTEM_TIMER_MAX_TIMERS]; /**< A static pool of available timers. */
    SemaphoreHandle_t pool_mutex; /**< Mutex to protect access to the timer pool. */
} system_timer_private_data_t;

// --- Forward Declarations ---
static esp_err_t system_timer_init(module_t *self);
static void system_timer_deinit(module_t *self);
static void timer_callback(void* arg);

static synapse_timer_handle_t api_schedule_event(const char *event_name, uint32_t interval_ms, bool is_periodic);
static esp_err_t api_cancel_event(synapse_timer_handle_t handle);

// --- Global Static Variables ---
static module_t *global_timer_service_instance = NULL;

static timer_api_t timer_service_api = {
    .schedule_event = api_schedule_event,
    .cancel_event = api_cancel_event,
};

// =========================================================================
//                      Module Lifecycle Implementation
// =========================================================================

module_t *system_timer_create(const cJSON *config) {
    ESP_LOGI(TAG, "Creating System Timer Service module...");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    system_timer_private_data_t *private_data = (system_timer_private_data_t *)calloc(1, sizeof(system_timer_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(private_data);
        free(module);
        return NULL;
    }

    private_data->pool_mutex = xSemaphoreCreateMutex();
    if (!private_data->pool_mutex) {
        ESP_LOGE(TAG, "Failed to create pool mutex.");
        free(private_data);
        free(module);
        return NULL;
    }
    
    module->private_data = private_data;

    // --- Configuration Parsing (Corrected) ---
    const char *instance_name = CONFIG_SYSTEM_TIMER_DEFAULT_INSTANCE_NAME;
    if (config) {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node)) {
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node) && name_node->valuestring) {
                instance_name = name_node->valuestring;
            }
        }
        module->current_config = cJSON_Duplicate(config, true);
    }
    
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->init_level = 35;

    module->base.init = system_timer_init;
    module->base.deinit = system_timer_deinit;
    module->base.start = NULL;
    module->base.handle_event = NULL;
    module->base.enable = NULL;
    module->base.disable = NULL;
    module->base.reconfigure = NULL;
    module->base.get_status = NULL;

    // --- Service Registration (Corrected for new pattern) ---
    esp_err_t ret = synapse_service_register_with_status(
        module->name,
        SYNAPSE_SERVICE_TYPE_TIMER_API,
        &timer_service_api, // Assuming global static API struct
        SERVICE_STATUS_REGISTERED);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register service for '%s' (%s). Cleaning up.", module->name, esp_err_to_name(ret));
        system_timer_deinit(module);
        return NULL;
    }

    global_timer_service_instance = module;
    ESP_LOGI(TAG, "System Timer Service module '%s' created and service registered.", module->name);
    return module;
}

static esp_err_t system_timer_init(module_t *self) {
    ESP_LOGI(TAG, "Initializing System Timer Service: %s", self->name);
    // The module is a pure service, so it can be considered running after init.
    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "System Timer service is active.");
    return ESP_OK;
}
/**
 * @internal
 * @brief Deinitializes the module, cancelling all active timers and freeing resources.
 * @param self Pointer to the module instance.
 */
static void system_timer_deinit(module_t *self) {
    if (!self) return;
    system_timer_private_data_t *private_data = (system_timer_private_data_t *)self->private_data;
    
    ESP_LOGI(TAG, "Deinitializing System Timer Service: %s", self->name);
    synapse_service_unregister(self->name);

    // Cancel and delete all active timers to prevent callbacks after deinit
    for (int i = 0; i < CONFIG_SYSTEM_TIMER_MAX_TIMERS; i++) {
        if (private_data->timer_pool[i].is_active) {
            esp_timer_stop(private_data->timer_pool[i].esp_handle);
            esp_timer_delete(private_data->timer_pool[i].esp_handle);
        }
    }
    
    if (private_data->pool_mutex) vSemaphoreDelete(private_data->pool_mutex);
    if (self->private_data) free(self->private_data);
    if (self->current_config) cJSON_Delete(self->current_config);
    if (self->state_mutex) vSemaphoreDelete(self->state_mutex);

    global_timer_service_instance = NULL;
}

// =========================================================================
//                      Timer Callback & Service API
// =========================================================================

/**
 * @internal
 * @brief The single callback function used for all esp_timer instances.
 * @details When a timer fires, this function posts the associated event name
 *          to the Event Bus.
 * @param arg A pointer to the `timer_entry_t` for the fired timer.
 */
static void timer_callback(void* arg) {
    timer_entry_t* entry = (timer_entry_t*)arg;
    if (entry && entry->event_name) {
        ESP_LOGD(TAG, "Timer fired. Posting event: %s", entry->event_name);
        // Note: No data is passed with the event.
        synapse_event_bus_post(entry->event_name, NULL);
    }
}

/**
 * @internal
 * @brief Implements the schedule_event API function.
 * @see timer_interface.h
 */
static synapse_timer_handle_t api_schedule_event(const char *event_name, uint32_t interval_ms, bool is_periodic)
{
    if (!event_name || interval_ms == 0) {
        return NULL;
    }

    if (!global_timer_service_instance) {
        ESP_LOGE(TAG, "Cannot schedule event, service is not initialized.");
        return NULL;
    }
    system_timer_private_data_t *private_data = (system_timer_private_data_t *)global_timer_service_instance->private_data;

    if (xSemaphoreTake(private_data->pool_mutex, pdMS_TO_TICKS(50)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to take pool mutex for scheduling.");
        return NULL;
    }

    // Find a free slot in the timer pool
    int free_slot = -1;
    for (int i = 0; i < CONFIG_SYSTEM_TIMER_MAX_TIMERS; i++) {
        if (!private_data->timer_pool[i].is_active) {
            free_slot = i;
            break;
        }
    }

    if (free_slot == -1) {
        ESP_LOGE(TAG, "Failed to schedule event '%s': No free timer slots available (Max: %d).", 
                 event_name, CONFIG_SYSTEM_TIMER_MAX_TIMERS);
        xSemaphoreGive(private_data->pool_mutex);
        return NULL;
    }

    timer_entry_t* new_entry = &private_data->timer_pool[free_slot];
    new_entry->event_name = event_name;

    esp_timer_create_args_t timer_args = {
        .callback = &timer_callback,
        .arg = new_entry,
        .name = event_name // For debugging with esp_timer_dump
    };

    esp_err_t err = esp_timer_create(&timer_args, &new_entry->esp_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create esp_timer for event '%s': %s", event_name, esp_err_to_name(err));
        xSemaphoreGive(private_data->pool_mutex);
        return NULL;
    }

    uint64_t period_us = (uint64_t)interval_ms * 1000;
    if (is_periodic) {
        err = esp_timer_start_periodic(new_entry->esp_handle, period_us);
    } else {
        err = esp_timer_start_once(new_entry->esp_handle, period_us);
    }

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start esp_timer for event '%s': %s", event_name, esp_err_to_name(err));
        esp_timer_delete(new_entry->esp_handle);
        xSemaphoreGive(private_data->pool_mutex);
        return NULL;
    }

    new_entry->is_active = true;
    xSemaphoreGive(private_data->pool_mutex);

    ESP_LOGI(TAG, "Scheduled event '%s' with interval %" PRIu32 "ms. Periodic: %s", event_name, interval_ms, is_periodic ? "Yes" : "No");
    return (synapse_timer_handle_t)new_entry;
}

/**
 * @internal
 * @brief Implements the cancel_event API function.
 * @see timer_interface.h
 */
static esp_err_t api_cancel_event(synapse_timer_handle_t handle)
{
    if (!handle) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!global_timer_service_instance) {
        return ESP_ERR_INVALID_STATE;
    }
    system_timer_private_data_t *private_data = (system_timer_private_data_t *)global_timer_service_instance->private_data;

    timer_entry_t* entry_to_cancel = (timer_entry_t*)handle;
    esp_err_t ret = ESP_ERR_NOT_FOUND;

    if (xSemaphoreTake(private_data->pool_mutex, pdMS_TO_TICKS(50)) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    // Verify that the handle points to a valid, active entry in our pool
    bool is_valid_handle = false;
    for (int i = 0; i < CONFIG_SYSTEM_TIMER_MAX_TIMERS; i++) {
        if (&private_data->timer_pool[i] == entry_to_cancel && entry_to_cancel->is_active) {
            is_valid_handle = true;
            break;
        }
    }

    if (is_valid_handle) {
        esp_timer_stop(entry_to_cancel->esp_handle);
        esp_timer_delete(entry_to_cancel->esp_handle);
        
        // Clear the entry to mark it as free
        entry_to_cancel->is_active = false;
        entry_to_cancel->event_name = NULL;
        entry_to_cancel->esp_handle = NULL;
        
        ESP_LOGI(TAG, "Cancelled scheduled event successfully.");
        ret = ESP_OK;
    } else {
        ESP_LOGW(TAG, "Attempted to cancel an invalid or inactive timer handle.");
    }

    xSemaphoreGive(private_data->pool_mutex);
    return ret;
}