/**
 * @file watchdog_manager.c
 * @brief Intelligent watchdog timer management with heartbeat monitoring.
 * @author Synapse Framework Team
 * @version 1.2.0
 * @date 2025-07-19
 * @details This module implements an application-aware watchdog. It creates a
 *          dedicated, high-priority task that subscribes itself to the ESP-IDF
 *          Task Watchdog Timer (TWDT). This task periodically checks for heartbeats
 *          from critical registered modules. It only "feeds" the TWDT if all
 *          modules are responsive, ensuring the entire system is healthy, not
 *          just a single task.
 */

// --- Framework & System Includes ---
#include "synapse.h"
#include "watchdog_manager.h"
#include "watchdog_interface.h"

// --- ESP-IDF & Standard Lib Includes ---
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("WATCHDOG_MANAGER", SYNAPSE_LOG_COLOR_BLUE);

// =========================================================================
//                      Private Data & Definitions
// =========================================================================

/** @internal @brief Holds the state for a single monitored module (client). */
typedef struct
{
    char module_name[CONFIG_SYNAPSE_MODULE_NAME_MAX_LENGTH];
    int64_t last_heartbeat_time_us;
    bool is_alive;
} heartbeat_client_t;

/** @internal @brief Private data for the Watchdog Manager module. */
typedef struct
{
    char instance_name[CONFIG_WATCHDOG_MANAGER_INSTANCE_NAME_MAX_LEN];
    TaskHandle_t watchdog_task_handle;
    SemaphoreHandle_t client_list_mutex;

    // Configuration
    uint32_t check_interval_ms;
    uint32_t heartbeat_timeout_ms;

    // Runtime State
    heartbeat_client_t clients[CONFIG_WATCHDOG_MANAGER_MAX_HEARTBEAT_MODULES];
    uint8_t client_count;
} watchdog_manager_private_data_t;

// --- Forward declarations ---
static esp_err_t watchdog_manager_init(module_t *self);
static esp_err_t watchdog_manager_start(module_t *self);
static void watchdog_manager_deinit(module_t *self);
static void watchdog_task(void *pvParameters);
static esp_err_t parse_config(const cJSON *config, watchdog_manager_private_data_t *private_data);

// API Implementations
static esp_err_t api_register_heartbeat(const char *module_name);
static esp_err_t api_unregister_heartbeat(const char *module_name);
static esp_err_t api_send_heartbeat(const char *module_name);

// --- Global Variables for Service API ---
static module_t *watchdog_manager_self = NULL;
static watchdog_api_t watchdog_service_api = {
    .register_heartbeat = api_register_heartbeat,
    .unregister_heartbeat = api_unregister_heartbeat,
    .send_heartbeat = api_send_heartbeat,
};

// =============================================================================
// Module Lifecycle Implementation
// =============================================================================

/**
 * @brief Creates a new instance of the Watchdog Manager module.
 * @param[in] config Module configuration from system_config.json. This function
 *                   takes ownership of the config pointer.
 * @return A pointer to the created module, or NULL on failure.
 */
module_t *watchdog_manager_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating Watchdog Manager module instance");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    watchdog_manager_private_data_t *private_data = (watchdog_manager_private_data_t *)calloc(1, sizeof(watchdog_manager_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        // Since we are about to own `config`, we must free it on failure.
        if (config)
            cJSON_Delete((cJSON *)config);
        return NULL;
    }

    module->private_data = private_data;
    module->current_config = cJSON_Duplicate(config, true);
    if (!module->current_config)
    {
        ESP_LOGE(TAG, "Failed to duplicate configuration object.");
        // Note: This assumes 'private_data' and 'module' are allocated.
        // Manual check might be needed for each file's cleanup logic.
        free(private_data);
        free(module);
        return NULL;
    } // Take ownership of the config object

    private_data->client_list_mutex = xSemaphoreCreateMutex();
    if (!private_data->client_list_mutex)
    {
        ESP_LOGE(TAG, "Failed to create client list mutex");
        watchdog_manager_deinit(module); // Use deinit for consistent cleanup
        return NULL;
    }

    // --- START OF CORRECTED CONFIGURATION & NAME PARSING ---
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (!config_node)
    {
        ESP_LOGE(TAG, "Module config is missing the required 'config' object wrapper.");
        watchdog_manager_deinit(module);
        return NULL;
    }

    // 1. Parse instance_name FIRST
    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    const char *instance_name = cJSON_IsString(name_node) ? name_node->valuestring : CONFIG_WATCHDOG_MANAGER_DEFAULT_INSTANCE_NAME;

    // 2. Copy the name to both the module's public name and private data
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", instance_name);

    // 3. Parse the rest of the configuration
    if (parse_config(config_node, private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to parse configuration for '%s'.", module->name);
        watchdog_manager_deinit(module);
        return NULL;
    }
    // --- END OF CORRECTED LOGIC ---

    module->init_level = 36; // Should be after System Timer (35)
    module->status = MODULE_STATUS_UNINITIALIZED;

    module->base.init = watchdog_manager_init;
    module->base.start = watchdog_manager_start;
    module->base.deinit = watchdog_manager_deinit;
    module->base.handle_event = NULL; // This module does not need to handle events

    watchdog_manager_self = module;

    ESP_LOGI(TAG, "Watchdog Manager module '%s' created.", module->name);
    return module;
}

/**
 * @internal
 * @brief Initializes the Watchdog Manager module.
 * @details Registers the service API and verifies that the underlying ESP-IDF
 *          Task Watchdog Timer (TWDT) is enabled in the project configuration.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK on success, or ESP_FAIL if the TWDT is not configured.
 */
static esp_err_t watchdog_manager_init(module_t *self)
{
    ESP_LOGI(TAG, "Initializing Watchdog Manager.");

#if !CONFIG_ESP_TASK_WDT_INIT
    ESP_LOGE(TAG, "CRITICAL: Watchdog Manager requires 'Initialize Task Watchdog Timer on startup' (CONFIG_ESP_TASK_WDT_INIT) to be enabled in menuconfig!");
    return ESP_FAIL;
#endif

    synapse_service_register(self->name, SYNAPSE_SERVICE_TYPE_WATCHDOG_API, &watchdog_service_api);
    ESP_LOGI(TAG, "Task Watchdog Timer is configured to be initialized by the system.");

    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

/**
 * @internal
 * @brief Starts the Watchdog Manager's monitoring process.
 * @details Creates a dedicated FreeRTOS task to perform periodic heartbeat checks.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK on success, or ESP_FAIL if the task could not be created.
 */
static esp_err_t watchdog_manager_start(module_t *self)
{
    ESP_LOGI(TAG, "Starting Watchdog Manager.");
    watchdog_manager_private_data_t *private_data = (watchdog_manager_private_data_t *)self->private_data;

    BaseType_t task_created = xTaskCreate(
        watchdog_task, "watchdog_task", CONFIG_WATCHDOG_MANAGER_TASK_STACK_SIZE,
        self, CONFIG_WATCHDOG_MANAGER_TASK_PRIORITY, &private_data->watchdog_task_handle);

    if (task_created != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create watchdog task");
        return ESP_FAIL;
    }

    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

/**
 * @internal
 * @brief Deinitializes the module, freeing all resources.
 * @param[in] self A pointer to the module instance.
 */
static void watchdog_manager_deinit(module_t *self)
{
    if (!self)
        return;
    watchdog_manager_private_data_t *private_data = (watchdog_manager_private_data_t *)self->private_data;

    if (private_data && private_data->watchdog_task_handle)
    {
        // Unsubscribe the task from TWDT before deleting it
        esp_task_wdt_delete(private_data->watchdog_task_handle);
        vTaskDelete(private_data->watchdog_task_handle);
    }

    synapse_service_unregister(self->name);

    if (private_data && private_data->client_list_mutex)
        vSemaphoreDelete(private_data->client_list_mutex);
    if (self->private_data)
        free(self->private_data);

    watchdog_manager_self = NULL;
}

// =============================================================================
// Service API Implementations
// =============================================================================

/**
 * @internal
 * @brief API function to register a module for heartbeat monitoring.
 * @see watchdog_interface.h
 */
static esp_err_t api_register_heartbeat(const char *module_name)
{
    if (!watchdog_manager_self || !module_name)
        return ESP_ERR_INVALID_ARG;
    watchdog_manager_private_data_t *private_data = (watchdog_manager_private_data_t *)watchdog_manager_self->private_data;

    if (xSemaphoreTake(private_data->client_list_mutex, portMAX_DELAY) != pdTRUE)
        return ESP_ERR_TIMEOUT;

    if (private_data->client_count >= CONFIG_WATCHDOG_MANAGER_MAX_HEARTBEAT_MODULES)
    {
        xSemaphoreGive(private_data->client_list_mutex);
        ESP_LOGE(TAG, "Cannot register heartbeat for '%s', list is full.", module_name);
        return ESP_ERR_NO_MEM;
    }

    heartbeat_client_t *client = &private_data->clients[private_data->client_count++];
    strncpy(client->module_name, module_name, sizeof(client->module_name) - 1);
    client->last_heartbeat_time_us = esp_timer_get_time();
    client->is_alive = true;

    xSemaphoreGive(private_data->client_list_mutex);

    ESP_LOGI(TAG, "Module '%s' registered for heartbeat monitoring.", module_name);
    return ESP_OK;
}

/**
 * @internal
 * @brief API function for a module to send a heartbeat signal.
 * @see watchdog_interface.h
 */
static esp_err_t api_send_heartbeat(const char *module_name)
{
    if (!watchdog_manager_self || !module_name)
        return ESP_ERR_INVALID_ARG;
    watchdog_manager_private_data_t *private_data = (watchdog_manager_private_data_t *)watchdog_manager_self->private_data;

    esp_err_t ret = ESP_ERR_NOT_FOUND;
    if (xSemaphoreTake(private_data->client_list_mutex, portMAX_DELAY) != pdTRUE)
        return ESP_ERR_TIMEOUT;

    for (uint8_t i = 0; i < private_data->client_count; i++)
    {
        if (strcmp(private_data->clients[i].module_name, module_name) == 0)
        {
            private_data->clients[i].last_heartbeat_time_us = esp_timer_get_time();
            if (!private_data->clients[i].is_alive)
            {
                ESP_LOGI(TAG, "Module '%s' has recovered and sent a heartbeat.", module_name);
                private_data->clients[i].is_alive = true;
            }
            ret = ESP_OK;
            break;
        }
    }
    xSemaphoreGive(private_data->client_list_mutex);

    if (ret == ESP_OK)
    {
        ESP_LOGD(TAG, "Heartbeat received from '%s'", module_name);
    }
    else
    {
        ESP_LOGW(TAG, "Heartbeat received from unregistered module '%s'", module_name);
    }
    return ret;
}

/**
 * @internal
 * @brief API function to unregister a module from monitoring.
 * @see watchdog_interface.h
 */
static esp_err_t api_unregister_heartbeat(const char *module_name)
{
    // Implementation can be added if dynamic unregistration is needed.
    return ESP_ERR_NOT_SUPPORTED;
}

// =============================================================================
// Internal Helper Functions
// =============================================================================

/**
 * @internal
 * @brief The dedicated FreeRTOS task for monitoring heartbeats.
 * @details This task subscribes itself to the TWDT. It then enters an infinite
 *          loop, periodically checking all registered clients. If all clients
 *          are responsive, it resets the TWDT. If any client times out, it
 *          stops resetting the TWDT, which will cause a hardware reboot.
 * @param[in] pvParameters A pointer to the module instance (`self`).
 */
static void watchdog_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    watchdog_manager_private_data_t *private_data = (watchdog_manager_private_data_t *)self->private_data;

    // Subscribe this task to the TWDT
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    ESP_LOGI(TAG, "Watchdog monitoring task started and subscribed to TWDT.");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(private_data->check_interval_ms));

        bool all_clients_alive = true;
        char *first_failed_module = NULL;

        if (xSemaphoreTake(private_data->client_list_mutex, pdMS_TO_TICKS(50)) != pdTRUE)
        {
            ESP_LOGE(TAG, "Could not take mutex to check heartbeats. Skipping this check cycle.");
            continue; // Skip this cycle but keep trying
        }

        int64_t current_time_us = esp_timer_get_time();
        for (uint8_t i = 0; i < private_data->client_count; i++)
        {
            int64_t time_since_hb_ms = (current_time_us - private_data->clients[i].last_heartbeat_time_us) / 1000;

            if (time_since_hb_ms > private_data->heartbeat_timeout_ms)
            {
                all_clients_alive = false;
                if (private_data->clients[i].is_alive)
                {
                    private_data->clients[i].is_alive = false;
                    ESP_LOGE(TAG, "WATCHDOG ALERT: Heartbeat missed from module '%s'!", private_data->clients[i].module_name);
                    if (!first_failed_module)
                    {
                        first_failed_module = private_data->clients[i].module_name;
                    }
                }
            }
        }
        xSemaphoreGive(private_data->client_list_mutex);

        if (all_clients_alive)
        {
            ESP_LOGD(TAG, "All systems nominal. Feeding hardware watchdog.");
            esp_task_wdt_reset();
        }
        else
        {
            ESP_LOGE(TAG, "Heartbeat missed from '%s'! Not feeding watchdog. System will reboot soon.",
                     first_failed_module ? first_failed_module : "unknown module");

            synapse_telemetry_payload_t *payload = malloc(sizeof(synapse_telemetry_payload_t));
            if (payload)
            {
                snprintf(payload->module_name, sizeof(payload->module_name), "%s", first_failed_module ? first_failed_module : "unknown");
                payload->json_data = NULL;

                event_data_wrapper_t *wrapper;
                if (synapse_event_data_wrap(payload, synapse_telemetry_payload_free, &wrapper) == ESP_OK)
                {
                    synapse_event_bus_post(SYNAPSE_EVENT_HEARTBEAT_MISSED, wrapper);
                    synapse_event_data_release(wrapper);
                }
                else
                {
                    free(payload);
                }
            }
        }
    }
}

/**
 * @internal
 * @brief Parses the module's configuration from the provided cJSON object.
 */
static esp_err_t parse_config(const cJSON *config, watchdog_manager_private_data_t *private_data)
{
    if (!private_data)
        return ESP_ERR_INVALID_ARG;

    private_data->check_interval_ms = CONFIG_WATCHDOG_MANAGER_CHECK_INTERVAL_MS;
    private_data->heartbeat_timeout_ms = CONFIG_WATCHDOG_MANAGER_HEARTBEAT_TIMEOUT_MS;

    if (config)
    {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node))
        {
            const cJSON *check_interval = cJSON_GetObjectItem(config_node, "check_interval_ms");
            if (cJSON_IsNumber(check_interval))
                private_data->check_interval_ms = check_interval->valueint;

            const cJSON *hb_timeout = cJSON_GetObjectItem(config_node, "heartbeat_timeout_ms");
            if (cJSON_IsNumber(hb_timeout))
                private_data->heartbeat_timeout_ms = hb_timeout->valueint;
        }
    }

    if (private_data->check_interval_ms >= (CONFIG_ESP_TASK_WDT_TIMEOUT_S * 1000))
    {
        ESP_LOGE(TAG, "Configuration error: check_interval_ms (%" PRIu32 ") must be less than TWDT timeout (%d s)!",
                 private_data->check_interval_ms, CONFIG_ESP_TASK_WDT_TIMEOUT_S);
        return ESP_ERR_INVALID_ARG;
    }
    return ESP_OK;
}