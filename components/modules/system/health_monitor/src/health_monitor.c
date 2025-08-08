/**
 * @file health_monitor.c
 * @brief Monitors system health using the Shared Task Pool Manager.
 * @author Synapse Framework Team
 * @version 2.0.0
 * @date 2025-09-07
 * @details This module periodically checks system resources (heap, task stacks)
 *          by scheduling a recurring job with the Task Pool Manager, thus avoiding
 *          the need for its own dedicated task.
 */

#include "synapse.h"
#include "health_monitor.h"
#include "health_interface.h"

#include "esp_system.h"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

DEFINE_COMPONENT_TAG("HEALTH_MONITOR");

// --- Definitions ---
#define MAX_CUSTOM_CHECKS 5
#define EVT_HEALTH_ALERT "SYSTEM_HEALTH_ALERT"

// --- Private Data Structures ---
typedef struct
{
    char name[CONFIG_HEALTH_MONITOR_INSTANCE_NAME_MAX_LEN];
    health_check_fn_t check_fn;
    void *context;
} custom_check_t;

typedef struct
{
    uint32_t check_interval_sec;
    uint32_t min_free_heap_kb;
    uint32_t min_task_stack_hwm_bytes;
} health_thresholds_t;

typedef struct
{
    module_t *self; // Pointer back to the module instance
    char instance_name[CONFIG_HEALTH_MONITOR_INSTANCE_NAME_MAX_LEN];
    health_thresholds_t thresholds;
    custom_check_t custom_checks[MAX_CUSTOM_CHECKS];
    uint8_t custom_check_count;
    bool is_provisioning_active;
    synapse_job_handle_t job_handle; // Handle for the scheduled job
} health_monitor_private_data_t;

// --- Forward declarations ---
static esp_err_t health_monitor_init(module_t *self);
static esp_err_t health_monitor_start(module_t *self);
static void health_monitor_deinit(module_t *self);
static void health_monitor_handle_event(module_t *self, const char *event_name, void *event_data);
static void health_check_job(void *user_context); // This is now a job, not a task
static esp_err_t parse_config(const cJSON *config, health_monitor_private_data_t *private_data);

static esp_err_t api_get_system_health_report(cJSON **report);
static esp_err_t api_register_custom_check(const char *check_name, health_check_fn_t check_fn, void *context);
static esp_err_t api_unregister_custom_check(const char *check_name);

// --- Global Variables for Service API ---
static module_t *global_health_monitor_instance = NULL;
static health_api_t health_service_api = {
    .get_system_health_report = api_get_system_health_report,
    .register_custom_check = api_register_custom_check,
    .unregister_custom_check = api_unregister_custom_check,
};

// =============================================================================
// Factory and Lifecycle Functions
// =============================================================================
module_t *health_monitor_create(const cJSON *config)
{
    // 1. Allocate memory for module and private data structures
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    health_monitor_private_data_t *private_data = (health_monitor_private_data_t *)calloc(1, sizeof(health_monitor_private_data_t));

    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for module structures.");
        free(module); // It's safe to call free on NULL
        free(private_data);
        // The original config is managed by the Module Registry, so we don't delete it here.
        return NULL;
    }

    // 2. Establish pointers
    module->private_data = private_data;
    private_data->self = module;

    // 3. Create an independent copy of the configuration
    module->current_config = cJSON_Duplicate(config, true);
    if (!module->current_config)
    {
        ESP_LOGE(TAG, "Failed to duplicate configuration object.");
        free(private_data);
        free(module);
        return NULL;
    }

    // 4. Parse the configuration to populate private_data
    if (parse_config(module->current_config, private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to parse configuration for health_monitor.");
        // Perform a full cleanup as config was already duplicated
        cJSON_Delete(module->current_config);
        free(private_data);
        free(module);
        return NULL;
    }

    // 5. Set the module's instance name from the parsed config
    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);

    // 6. Assign lifecycle function pointers
    module->init_level = 90;
    module->base.init = health_monitor_init;
    module->base.start = health_monitor_start;
    module->base.deinit = health_monitor_deinit;
    module->base.handle_event = health_monitor_handle_event;
    // Note: get_status is not assigned here, it seems to be missing from the original file.

    // 7. Set the global instance for the Service API
    global_health_monitor_instance = module;

    ESP_LOGI(TAG, "Health_Monitor module created: '%s'", module->name);
    return module;
}

static esp_err_t health_monitor_init(module_t *self)
{
    ESP_LOGI(TAG, "Initializing health_monitor module: %s", self->name);
    synapse_service_register(self->name, SYNAPSE_SERVICE_TYPE_HEALTH_API, &health_service_api);
    synapse_event_bus_subscribe("PROV_STARTED", self);
    synapse_event_bus_subscribe("PROV_ENDED", self);
    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "Health_Monitor module initialized successfully");
    return ESP_OK;
}

static esp_err_t health_monitor_start(module_t *self)
{
    health_monitor_private_data_t *private_data = (health_monitor_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting health_monitor module by scheduling a job.");

    private_data->job_handle = synapse_task_pool_schedule_job(
        health_check_job,
        self, // Pass the module instance as context
        private_data->thresholds.check_interval_sec * 1000,
        true // Periodic job
    );

    if (private_data->job_handle == NULL)
    {
        ESP_LOGE(TAG, "Failed to schedule health check job");
        self->status = MODULE_STATUS_ERROR;
        return ESP_FAIL;
    }

    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "Health_Monitor module started successfully");
    return ESP_OK;
}

static void health_monitor_deinit(module_t *self)
{
    if (!self)
        return;
    health_monitor_private_data_t *private_data = (health_monitor_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Deinitializing health_monitor module");

    if (private_data && private_data->job_handle)
    {
        synapse_task_pool_cancel_job(private_data->job_handle);
    }

    synapse_service_unregister(self->name);
    synapse_event_bus_unsubscribe("PROV_STARTED", self);
    synapse_event_bus_unsubscribe("PROV_ENDED", self);

    global_health_monitor_instance = NULL;

    if (self->current_config)
        cJSON_Delete(self->current_config);
    if (self->private_data)
        free(self->private_data);
}

// =============================================================================
// Event Handling & Job Function
// =============================================================================

static void health_monitor_handle_event(module_t *self, const char *event_name, void *event_data)
{
    health_monitor_private_data_t *private_data = (health_monitor_private_data_t *)self->private_data;
    if (strcmp(event_name, "PROV_STARTED") == 0)
    {
        ESP_LOGW(TAG, "Provisioning started. Temporarily disabling heap memory checks.");
        private_data->is_provisioning_active = true;
    }
    else if (strcmp(event_name, "PROV_ENDED") == 0)
    {
        ESP_LOGI(TAG, "Provisioning ended. Re-enabling heap memory checks.");
        private_data->is_provisioning_active = false;
    }
    if (event_data)
    {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

static void health_check_job(void *user_context)
{
    module_t *self = (module_t *)user_context;
    health_monitor_private_data_t *private_data = (health_monitor_private_data_t *)self->private_data;

    ESP_LOGD(TAG, "Performing health check...");

    // 1. Check heap memory
    if (!private_data->is_provisioning_active)
    {
        size_t free_heap_kb = esp_get_free_heap_size() / 1024;
        if (free_heap_kb < private_data->thresholds.min_free_heap_kb)
        {
            ESP_LOGE(TAG, "HEALTH ALERT: Low heap memory! Free: %u KB, Threshold: %" PRIu32 " KB",
                     (unsigned int)free_heap_kb, private_data->thresholds.min_free_heap_kb);
            // Post event logic...
        }
    }
    else
    {
        ESP_LOGI(TAG, "Heap check skipped during active provisioning.");
    }

    // 2. Check task stacks
    UBaseType_t num_of_tasks = uxTaskGetNumberOfTasks();
    TaskStatus_t *task_status_array = pvPortMalloc(num_of_tasks * sizeof(TaskStatus_t));
    if (task_status_array != NULL)
    {
        uint32_t total_run_time;
        num_of_tasks = uxTaskGetSystemState(task_status_array, num_of_tasks, &total_run_time);
        for (UBaseType_t i = 0; i < num_of_tasks; i++)
        {
            uint32_t stack_hwm_bytes = task_status_array[i].usStackHighWaterMark * sizeof(StackType_t);
            if (stack_hwm_bytes < private_data->thresholds.min_task_stack_hwm_bytes)
            {
                ESP_LOGE(TAG, "HEALTH ALERT: Low stack space for task '%s'! Remaining: %" PRIu32 " bytes",
                         task_status_array[i].pcTaskName, stack_hwm_bytes);
                // Post event logic...
            }
        }
        vPortFree(task_status_array);
    }

    // 3. Perform custom checks
    for (uint8_t i = 0; i < private_data->custom_check_count; i++)
    {
        // Custom check logic...
    }
}

// =============================================================================
// Service API Implementations
// =============================================================================
static esp_err_t api_get_system_health_report(cJSON **report)
{
    if (!report)
    {
        return ESP_ERR_INVALID_ARG;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root)
    {
        return ESP_ERR_NO_MEM;
    }

    cJSON_AddNumberToObject(root, "free_heap_bytes", esp_get_free_heap_size());
    cJSON_AddNumberToObject(root, "min_free_heap_bytes", esp_get_minimum_free_heap_size());
    cJSON_AddNumberToObject(root, "uptime_sec", esp_timer_get_time() / 1000000);

    UBaseType_t num_of_tasks = uxTaskGetNumberOfTasks();
    TaskStatus_t *task_status_array = pvPortMalloc(num_of_tasks * sizeof(TaskStatus_t));
    if (task_status_array == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for task status array");
        cJSON_Delete(root);
        return ESP_ERR_NO_MEM;
    }

    uint32_t total_run_time;
    num_of_tasks = uxTaskGetSystemState(task_status_array, num_of_tasks, &total_run_time);

    cJSON *tasks_array = cJSON_CreateArray();
    for (UBaseType_t i = 0; i < num_of_tasks; i++)
    {
        cJSON *task_json = cJSON_CreateObject();
        cJSON_AddStringToObject(task_json, "name", task_status_array[i].pcTaskName);
        cJSON_AddNumberToObject(task_json, "priority", task_status_array[i].uxCurrentPriority);
        // usStackHighWaterMark არის დარჩენილი ადგილი WORDS-ებში, ვაქცევთ ბაიტებად
        cJSON_AddNumberToObject(task_json, "stack_hwm_bytes", task_status_array[i].usStackHighWaterMark * sizeof(StackType_t));
        cJSON_AddItemToArray(tasks_array, task_json);
    }
    cJSON_AddItemToObject(root, "tasks", tasks_array);
    vPortFree(task_status_array);

    *report = root;
    return ESP_OK;
}

static esp_err_t api_register_custom_check(const char *check_name, health_check_fn_t check_fn, void *context)
{
    if (!global_health_monitor_instance || !check_name || !check_fn)
    {
        return ESP_ERR_INVALID_ARG;
    }
    health_monitor_private_data_t *private_data = (health_monitor_private_data_t *)global_health_monitor_instance->private_data;

    if (private_data->custom_check_count >= MAX_CUSTOM_CHECKS)
    {
        ESP_LOGE(TAG, "Cannot register more custom health checks. Limit reached (%d).", MAX_CUSTOM_CHECKS);
        return ESP_ERR_NO_MEM;
    }

    custom_check_t *new_check = &private_data->custom_checks[private_data->custom_check_count];
    strncpy(new_check->name, check_name, sizeof(new_check->name) - 1);
    new_check->check_fn = check_fn;
    new_check->context = context;
    private_data->custom_check_count++;

    ESP_LOGI(TAG, "Registered custom health check: '%s'", check_name);
    return ESP_OK;
}

static esp_err_t api_unregister_custom_check(const char *check_name)
{
    return ESP_ERR_NOT_SUPPORTED;
}

// =============================================================================
// Internal Helper Functions
// =============================================================================

static esp_err_t parse_config(const cJSON *config, health_monitor_private_data_t *private_data)
{
    if (!private_data || !config)
        return ESP_ERR_INVALID_ARG;

    // Default მნიშვნელობები
    private_data->thresholds.check_interval_sec = 60;
    private_data->thresholds.min_free_heap_kb = 20;
    private_data->thresholds.min_task_stack_hwm_bytes = 256;
    strncpy(private_data->instance_name, "health_monitor", sizeof(private_data->instance_name) - 1);

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (!config_node)
    {
        ESP_LOGW(TAG, "No 'config' object found. Using default values.");
        return ESP_OK;
    }

    // --- ახალი: ვკითხულობთ instance_name-ს ---
    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    if (cJSON_IsString(name_node))
    {
        strncpy(private_data->instance_name, name_node->valuestring, sizeof(private_data->instance_name) - 1);
    }

    const cJSON *interval = cJSON_GetObjectItem(config_node, "check_interval_sec");
    if (cJSON_IsNumber(interval))
        private_data->thresholds.check_interval_sec = interval->valueint;

    const cJSON *thresholds_node = cJSON_GetObjectItem(config_node, "thresholds");
    if (cJSON_IsObject(thresholds_node))
    {
        const cJSON *heap = cJSON_GetObjectItem(thresholds_node, "min_free_heap_kb");
        if (cJSON_IsNumber(heap))
            private_data->thresholds.min_free_heap_kb = heap->valueint;

        const cJSON *stack = cJSON_GetObjectItem(thresholds_node, "min_task_stack_hwm_bytes");
        if (cJSON_IsNumber(stack))
            private_data->thresholds.min_task_stack_hwm_bytes = stack->valueint;
    }

    ESP_LOGI(TAG, "Config parsed: Interval=%u, MinHeap=%uKB, MinStackHWM=%uB",
             (unsigned int)private_data->thresholds.check_interval_sec,
             (unsigned int)private_data->thresholds.min_free_heap_kb,
             (unsigned int)private_data->thresholds.min_task_stack_hwm_bytes);

    return ESP_OK;
}