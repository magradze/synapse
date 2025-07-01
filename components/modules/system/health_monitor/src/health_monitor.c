/**
 * @file health_monitor.c
 * @brief Monitors system health metrics like heap, CPU, and tasks
 * @author Synapse Framework Team
 * @version 1.2.0
 * @date 2025-07-01
 * @details Health Monitor მოდულის სრული იმპლემენტაცია. ეს მოდული პერიოდულად
 *          ამოწმებს სისტემის კრიტიკულ პარამეტრებს (მეხსიერება, ტასკები), აქვეყნებს
 *          გაფრთხილებებს Event Bus-ზე და აწვდის Service API-ს სხვა მოდულებს
 *          დიაგნოსტიკისთვის.
 */

#include "health_monitor.h"
#include "health_interface.h" 
#include "base_module.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "event_payloads.h"
#include "service_locator.h"
#include "logging.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"
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
    char name[32];
    health_check_fn_t check_fn;
    void *context;
} custom_check_t;

typedef struct
{
    uint32_t check_interval_sec;
    uint32_t min_free_heap_kb;
    uint32_t min_task_stack_hwm_bytes; // ★★★ შეიცვალა პროცენტიდან ბაიტებზე ★★★
} health_thresholds_t;

typedef struct
{
    char instance_name[CONFIG_HEALTH_MONITOR_INSTANCE_NAME_MAX_LEN];
    TaskHandle_t monitor_task_handle;
    health_thresholds_t thresholds;
    custom_check_t custom_checks[MAX_CUSTOM_CHECKS];
    uint8_t custom_check_count;
} health_monitor_private_data_t;

// --- Forward declarations ---
static esp_err_t health_monitor_init(module_t *self);
static esp_err_t health_monitor_start(module_t *self);
static void health_monitor_deinit(module_t *self);
static module_status_t health_monitor_get_status(module_t *self);
static void health_monitor_task(void *pvParameters);
static esp_err_t parse_config(const cJSON *config, health_monitor_private_data_t *p_data);

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
// Public API - Module Creation
// =============================================================================
module_t *health_monitor_create(const cJSON *config)
{
    // ... (ეს ფუნქცია უცვლელია, რადგან სწორად მუშაობდა) ...
    ESP_LOGI(TAG, "Creating health_monitor module instance");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for module");
        return NULL;
    }

    health_monitor_private_data_t *private_data = (health_monitor_private_data_t *)calloc(1, sizeof(health_monitor_private_data_t));
    if (!private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for private data");
        free(module);
        return NULL;
    }

    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex)
    {
        ESP_LOGE(TAG, "Failed to create state mutex");
        free(private_data);
        free(module);
        return NULL;
    }

    module->private_data = private_data;

    const char *instance_name = CONFIG_HEALTH_MONITOR_DEFAULT_INSTANCE_NAME;
    if (config)
    {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node))
        {
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node) && name_node->valuestring)
            {
                instance_name = name_node->valuestring;
            }
        }
        module->current_config = cJSON_Duplicate(config, true);
    }

    strncpy(private_data->instance_name, instance_name, CONFIG_HEALTH_MONITOR_INSTANCE_NAME_MAX_LEN - 1);
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;

    if (parse_config(config, private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to parse configuration for %s", module->name);
        vSemaphoreDelete(module->state_mutex);
        free(private_data);
        free(module);
        return NULL;
    }

    module->init_level = 90;
    module->base.init = health_monitor_init;
    module->base.start = health_monitor_start;
    module->base.deinit = health_monitor_deinit;
    module->base.get_status = health_monitor_get_status;
    module->base.enable = NULL;
    module->base.disable = NULL;
    module->base.reconfigure = NULL;
    module->base.handle_event = NULL;

    global_health_monitor_instance = module;

    ESP_LOGI(TAG, "Health_Monitor module created: '%s'", instance_name);
    return module;
}


// =============================================================================
// Base Module & Lifecycle Functions
// =============================================================================
static esp_err_t health_monitor_init(module_t *self)
{
    // ... (ეს ფუნქცია უცვლელია) ...
    if (!self)
    {
        return ESP_ERR_INVALID_ARG;
    }
    ESP_LOGI(TAG, "Initializing health_monitor module: %s", self->name);

    esp_err_t ret = fmw_service_register(self->name, FMW_SERVICE_TYPE_HEALTH_API, &health_service_api);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register health service: %s", esp_err_to_name(ret));
        return ret;
    }

    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "Health_Monitor module initialized successfully");
    return ESP_OK;
}

static esp_err_t health_monitor_start(module_t *self)
{
    // ... (ეს ფუნქცია უცვლელია) ...
    if (!self)
    {
        return ESP_ERR_INVALID_ARG;
    }
    if (self->status != MODULE_STATUS_INITIALIZED)
    {
        ESP_LOGE(TAG, "Cannot start uninitialized module");
        return ESP_ERR_INVALID_STATE;
    }

    health_monitor_private_data_t *private_data = (health_monitor_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting health_monitor module: %s", self->name);

    BaseType_t task_created = xTaskCreate(
        health_monitor_task,
        "health_monitor_task",
        CONFIG_HEALTH_MONITOR_TASK_STACK_SIZE,
        self,
        CONFIG_HEALTH_MONITOR_TASK_PRIORITY,
        &private_data->monitor_task_handle);

    if (task_created != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create health monitor task");
        self->status = MODULE_STATUS_ERROR;
        return ESP_FAIL;
    }

    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "Health_Monitor module started successfully");
    return ESP_OK;
}

static module_status_t health_monitor_get_status(module_t *self)
{
    // ... (ეს ფუნქცია უცვლელია) ...
    if (!self)
    {
        return MODULE_STATUS_ERROR;
    }
    return self->status;
}

static void health_monitor_deinit(module_t *self)
{
    // ... (ეს ფუნქცია უცვლელია) ...
    if (!self)
    {
        return;
    }
    ESP_LOGI(TAG, "Deinitializing %s module", self->name);
    health_monitor_private_data_t *private_data = (health_monitor_private_data_t *)self->private_data;

    if (private_data && private_data->monitor_task_handle)
    {
        vTaskDelete(private_data->monitor_task_handle);
    }

    fmw_service_unregister(self->name);
    global_health_monitor_instance = NULL;

    if (self->private_data)
    {
        free(self->private_data);
    }
    if (self->current_config)
    {
        cJSON_Delete(self->current_config);
    }
    if (self->state_mutex)
    {
        vSemaphoreDelete(self->state_mutex);
    }
    free(self);

    ESP_LOGI(TAG, "Module deinitialized successfully");
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

    // ★★★ ტასკების ინფორმაციის წაკითხვის გასწორებული ლოგიკა (Grok-ის მიხედვით) ★★★
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
    // ... (ეს ფუნქცია უცვლელია) ...
    if (!global_health_monitor_instance || !check_name || !check_fn)
    {
        return ESP_ERR_INVALID_ARG;
    }
    health_monitor_private_data_t *p_data = (health_monitor_private_data_t *)global_health_monitor_instance->private_data;

    if (p_data->custom_check_count >= MAX_CUSTOM_CHECKS)
    {
        ESP_LOGE(TAG, "Cannot register more custom health checks. Limit reached (%d).", MAX_CUSTOM_CHECKS);
        return ESP_ERR_NO_MEM;
    }

    custom_check_t *new_check = &p_data->custom_checks[p_data->custom_check_count];
    strncpy(new_check->name, check_name, sizeof(new_check->name) - 1);
    new_check->check_fn = check_fn;
    new_check->context = context;
    p_data->custom_check_count++;

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
static void health_monitor_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    health_monitor_private_data_t *p_data = (health_monitor_private_data_t *)self->private_data;
    TickType_t check_interval_ticks = pdMS_TO_TICKS(p_data->thresholds.check_interval_sec * 1000);

    ESP_LOGI(TAG, "Health monitor task started. Check interval: %" PRIu32 " seconds.", p_data->thresholds.check_interval_sec);

    while (1)
    {
        vTaskDelay(check_interval_ticks);

        ESP_LOGD(TAG, "Performing health check...");

        // 1. შევამოწმოთ მეხსიერება
        size_t free_heap = esp_get_free_heap_size() / 1024;
        if (free_heap < p_data->thresholds.min_free_heap_kb)
        {
            ESP_LOGE(TAG, "HEALTH ALERT: Low heap memory! Free: %u KB, Threshold: %" PRIu32 " KB",
                     (unsigned int)free_heap, p_data->thresholds.min_free_heap_kb);
            
            cJSON *payload_json = cJSON_CreateObject();
            cJSON_AddStringToObject(payload_json, "alert_type", "LOW_HEAP_MEMORY");
            cJSON_AddNumberToObject(payload_json, "value_kb", free_heap);
            char *json_str = cJSON_PrintUnformatted(payload_json);
            
            if (json_str) {
                event_data_wrapper_t *wrapper = NULL;
                if (fmw_event_data_wrap(json_str, free, &wrapper) == ESP_OK) {
                    fmw_event_bus_post(EVT_HEALTH_ALERT, wrapper);
                } else {
                    free(json_str);
                }
            }
            cJSON_Delete(payload_json);
        }

        // 2. შევამოწმოთ Custom Checks
        for (uint8_t i = 0; i < p_data->custom_check_count; i++)
        {
            custom_check_t *check = &p_data->custom_checks[i];
            if (check->check_fn(check->context) != ESP_OK)
            {
                ESP_LOGE(TAG, "HEALTH ALERT: Custom check '%s' failed!", check->name);
                cJSON *payload_json = cJSON_CreateObject();
                cJSON_AddStringToObject(payload_json, "alert_type", "CUSTOM_CHECK_FAILED");
                cJSON_AddStringToObject(payload_json, "check_name", check->name);
                char *json_str = cJSON_PrintUnformatted(payload_json);
                
                if (json_str) {
                    event_data_wrapper_t *wrapper = NULL;
                    if (fmw_event_data_wrap(json_str, free, &wrapper) == ESP_OK) {
                        fmw_event_bus_post(EVT_HEALTH_ALERT, wrapper);
                    } else {
                        free(json_str);
                    }
                }
                cJSON_Delete(payload_json);
            }
        }

        // ★★★ 3. შევამოწმოთ ტასკების სტეკი (გასწორებული ლოგიკა) ★★★
        UBaseType_t num_of_tasks = uxTaskGetNumberOfTasks();
        TaskStatus_t *task_status_array = pvPortMalloc(num_of_tasks * sizeof(TaskStatus_t));
        if (task_status_array != NULL)
        {
            uint32_t total_run_time;
            num_of_tasks = uxTaskGetSystemState(task_status_array, num_of_tasks, &total_run_time);
            for (UBaseType_t i = 0; i < num_of_tasks; i++)
            {
                uint32_t stack_hwm_bytes = task_status_array[i].usStackHighWaterMark * sizeof(StackType_t);
                if (stack_hwm_bytes < p_data->thresholds.min_task_stack_hwm_bytes)
                {
                    ESP_LOGE(TAG, "HEALTH ALERT: Low stack space for task '%s'! Remaining: %" PRIu32 " bytes, Threshold: %" PRIu32 " bytes",
                             task_status_array[i].pcTaskName,
                             stack_hwm_bytes,
                             p_data->thresholds.min_task_stack_hwm_bytes);
                    
                    cJSON *payload_json = cJSON_CreateObject();
                    cJSON_AddStringToObject(payload_json, "alert_type", "LOW_STACK_SPACE");
                    cJSON_AddStringToObject(payload_json, "task_name", task_status_array[i].pcTaskName);
                    cJSON_AddNumberToObject(payload_json, "remaining_bytes", stack_hwm_bytes);
                    char *json_str = cJSON_PrintUnformatted(payload_json);
                    
                    if (json_str) {
                        event_data_wrapper_t *wrapper = NULL;
                        if (fmw_event_data_wrap(json_str, free, &wrapper) == ESP_OK) {
                            fmw_event_bus_post(EVT_HEALTH_ALERT, wrapper);
                        } else {
                            free(json_str);
                        }
                    }
                    cJSON_Delete(payload_json);
                }
            }
            vPortFree(task_status_array);
        }
    }
}

static esp_err_t parse_config(const cJSON *config, health_monitor_private_data_t *p_data)
{
    if (!p_data) return ESP_ERR_INVALID_ARG;

    // Default მნიშვნელობები
    p_data->thresholds.check_interval_sec = 30;
    p_data->thresholds.min_free_heap_kb = 50;
    p_data->thresholds.min_task_stack_hwm_bytes = 256; // ★★★ შეიცვალა ★★★

    if (!config) {
        ESP_LOGW(TAG, "No config provided. Using default values.");
        return ESP_OK;
    }

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (!config_node)
    {
        ESP_LOGW(TAG, "No 'config' object found. Using default values.");
        return ESP_OK;
    }

    const cJSON *interval = cJSON_GetObjectItem(config_node, "check_interval_sec");
    if (cJSON_IsNumber(interval)) p_data->thresholds.check_interval_sec = interval->valueint;

    const cJSON *thresholds_node = cJSON_GetObjectItem(config_node, "thresholds");
    if (cJSON_IsObject(thresholds_node))
    {
        const cJSON *heap = cJSON_GetObjectItem(thresholds_node, "min_free_heap_kb");
        if (cJSON_IsNumber(heap)) p_data->thresholds.min_free_heap_kb = heap->valueint;

        // ★★★ შეიცვალა პარამეტრის სახელი ★★★
        const cJSON *stack = cJSON_GetObjectItem(thresholds_node, "min_task_stack_hwm_bytes");
        if (cJSON_IsNumber(stack)) p_data->thresholds.min_task_stack_hwm_bytes = stack->valueint;
    }
    else
    {
        ESP_LOGW(TAG, "No 'thresholds' object in config. Using all default threshold values.");
    }

    ESP_LOGI(TAG, "Config parsed: Interval=%u, MinHeap=%uKB, MinStackHWM=%uB",
             (unsigned int)p_data->thresholds.check_interval_sec,
             (unsigned int)p_data->thresholds.min_free_heap_kb,
             (unsigned int)p_data->thresholds.min_task_stack_hwm_bytes);

    return ESP_OK;
}