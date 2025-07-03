/**
 * @file watchdog_manager.c
 * @brief Intelligent watchdog timer management with heartbeat monitoring.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-02
 * @details This module implements an application-aware watchdog. It subscribes
 *          the main task to the ESP-IDF Task Watchdog Timer (TWDT) and only
 *          "feeds" it if all registered critical modules send a periodic
 *          heartbeat, ensuring the entire system is responsive, not just a
 *          single task.
 */

#include "watchdog_manager.h"
#include "watchdog_interface.h"
#include "base_module.h"
#include "event_bus.h"
#include "service_locator.h"
#include "logging.h"

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("WATCHDOG_MANAGER");

// --- Definitions ---
#define MAX_HEARTBEAT_MODULES CONFIG_WATCHDOG_MANAGER_MAX_HEARTBEAT_MODULES

// --- Event Names ---
#define EVT_HEARTBEAT_MISSED "WATCHDOG_HEARTBEAT_MISSED"
#define EVT_REBOOT_TRIGGERED "WATCHDOG_TRIGGERED_REBOOT"

// --- Private Data Structures ---
typedef struct
{
    char module_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];
    int64_t last_heartbeat_time;
    bool is_alive;
} heartbeat_client_t;

typedef struct
{
    char instance_name[CONFIG_WATCHDOG_MANAGER_INSTANCE_NAME_MAX_LEN];
    TaskHandle_t watchdog_task_handle;
    SemaphoreHandle_t client_list_mutex;
    
    uint32_t check_interval_ms;
    uint32_t heartbeat_timeout_ms;

    heartbeat_client_t clients[MAX_HEARTBEAT_MODULES];
    uint8_t client_count;
} watchdog_manager_private_data_t;

// --- Forward declarations ---
static esp_err_t watchdog_manager_init(module_t *self);
static esp_err_t watchdog_manager_start(module_t *self);
static void watchdog_manager_deinit(module_t *self);
static void watchdog_task(void *pvParameters);
static esp_err_t parse_config(const cJSON *config, watchdog_manager_private_data_t *p_data);

static esp_err_t api_register_heartbeat(const char *module_name);
static esp_err_t api_unregister_heartbeat(const char *module_name);
static esp_err_t api_send_heartbeat(const char *module_name);

// --- Global Variables for Service API ---
static module_t *global_watchdog_instance = NULL;
static watchdog_api_t watchdog_service_api = {
    .register_heartbeat = api_register_heartbeat,
    .unregister_heartbeat = api_unregister_heartbeat,
    .send_heartbeat = api_send_heartbeat,
};

// =============================================================================
// Public API - Module Creation
// =============================================================================
module_t *watchdog_manager_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating Watchdog Manager module instance");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module) {
        ESP_LOGE(TAG, "Failed to allocate memory for module");
        return NULL;
    }

    watchdog_manager_private_data_t *private_data = (watchdog_manager_private_data_t *)calloc(1, sizeof(watchdog_manager_private_data_t));
    if (!private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory for private data");
        free(module);
        return NULL;
    }

    private_data->client_list_mutex = xSemaphoreCreateMutex();
    if (!private_data->client_list_mutex) {
        ESP_LOGE(TAG, "Failed to create client list mutex");
        free(private_data);
        free(module);
        return NULL;
    }

    module->private_data = private_data;

    const char *instance_name = CONFIG_WATCHDOG_MANAGER_DEFAULT_INSTANCE_NAME;
    // ... (instance_name-ის წაკითხვა config-დან, თუ საჭიროა) ...
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    strncpy(private_data->instance_name, instance_name, sizeof(private_data->instance_name) - 1);

    if (parse_config(config, private_data) != ESP_OK) {
        // ... cleanup ...
        return NULL;
    }

    module->init_level = 95;
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->base.init = watchdog_manager_init;
    module->base.start = watchdog_manager_start;
    module->base.deinit = watchdog_manager_deinit;
    // ამ მოდულს არ სჭირდება სხვა base ფუნქციები
    module->base.get_status = NULL;
    module->base.enable = NULL;
    module->base.disable = NULL;
    module->base.reconfigure = NULL;
    module->base.handle_event = NULL;

    global_watchdog_instance = module;
    return module;
}

// =============================================================================
// Base Module & Lifecycle Functions
// =============================================================================
/**
 * @brief Initializes the Watchdog Manager module.
 * @details This function registers the module's service API and verifies that
 *          the underlying ESP-IDF Task Watchdog Timer (TWDT) has been enabled
 *          in menuconfig. It does not initialize the TWDT itself, as this is
 *          handled by the system startup code based on Kconfig settings.
 * @param self A pointer to the module instance.
 * @return ESP_OK on success, or ESP_FAIL if the required TWDT is not configured.
 */
static esp_err_t watchdog_manager_init(module_t *self)
{
    ESP_LOGI(TAG, "Initializing Watchdog Manager.");

    // 1. დავარეგისტრიროთ ჩვენი Service API, რათა სხვა მოდულებმა შეძლონ
    //    heartbeat-ების რეგისტრაცია.
    fmw_service_register(self->name, FMW_SERVICE_TYPE_WATCHDOG_API, &watchdog_service_api);

    // 2. შევამოწმოთ, რომ Task Watchdog Timer (TWDT) ჩართულია menuconfig-ში.
    //    ეს არის კრიტიკული წინაპირობა ამ მოდულის მუშაობისთვის.
    //    თუ ეს ოფცია გამორთულია, ჩვენ ვაფიქსირებთ შეცდომას და ვაჩერებთ
    //    ინიციალიზაციას, რათა თავიდან ავიცილოთ runtime კრაში.
#if !CONFIG_ESP_TASK_WDT_INIT
    ESP_LOGE(TAG, "CRITICAL: Watchdog Manager requires 'Initialize Task Watchdog Timer on startup' (CONFIG_ESP_TASK_WDT_INIT) to be enabled in menuconfig!");
    ESP_LOGE(TAG, "Please enable this option under 'Component config' -> 'ESP System Settings'.");
    // ვაბრუნებთ შეცდომას, რათა System Manager-მა იცოდეს, რომ მოდული ვერ ჩაირთო.
    return ESP_FAIL;
#endif

    ESP_LOGI(TAG, "Task Watchdog Timer is configured to be initialized by the system.");

    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

static esp_err_t watchdog_manager_start(module_t *self)
{
    ESP_LOGI(TAG, "Starting Watchdog Manager.");
    watchdog_manager_private_data_t *p_data = (watchdog_manager_private_data_t *)self->private_data;

    BaseType_t task_created = xTaskCreate(
        watchdog_task, "watchdog_task", CONFIG_WATCHDOG_MANAGER_TASK_STACK_SIZE,
        self, CONFIG_WATCHDOG_MANAGER_TASK_PRIORITY, &p_data->watchdog_task_handle);

    if (task_created != pdPASS) {
        ESP_LOGE(TAG, "Failed to create watchdog task");
        return ESP_FAIL;
    }

    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void watchdog_manager_deinit(module_t *self)
{
    if (!self) return;
    watchdog_manager_private_data_t *p_data = (watchdog_manager_private_data_t *)self->private_data;

    if (p_data && p_data->watchdog_task_handle) {
        esp_task_wdt_delete(p_data->watchdog_task_handle);
        vTaskDelete(p_data->watchdog_task_handle);
    }
    
    esp_task_wdt_deinit();
    fmw_service_unregister(self->name);
    
    if (p_data && p_data->client_list_mutex) vSemaphoreDelete(p_data->client_list_mutex);
    if (self->private_data) free(self->private_data);
    free(self);
}

// =============================================================================
// Service API Implementations
// =============================================================================
static esp_err_t api_register_heartbeat(const char *module_name)
{
    if (!global_watchdog_instance || !module_name) return ESP_ERR_INVALID_ARG;
    watchdog_manager_private_data_t *p_data = (watchdog_manager_private_data_t *)global_watchdog_instance->private_data;

    xSemaphoreTake(p_data->client_list_mutex, portMAX_DELAY);
    if (p_data->client_count >= MAX_HEARTBEAT_MODULES) {
        xSemaphoreGive(p_data->client_list_mutex);
        ESP_LOGE(TAG, "Cannot register heartbeat for '%s', list is full.", module_name);
        return ESP_ERR_NO_MEM;
    }

    heartbeat_client_t *client = &p_data->clients[p_data->client_count++];
    strncpy(client->module_name, module_name, sizeof(client->module_name) - 1);
    client->last_heartbeat_time = esp_timer_get_time();
    client->is_alive = true;
    xSemaphoreGive(p_data->client_list_mutex);

    ESP_LOGI(TAG, "Module '%s' registered for heartbeat monitoring.", module_name);
    return ESP_OK;
}

static esp_err_t api_send_heartbeat(const char *module_name)
{
    if (!global_watchdog_instance || !module_name) return ESP_ERR_INVALID_ARG;
    watchdog_manager_private_data_t *p_data = (watchdog_manager_private_data_t *)global_watchdog_instance->private_data;
    
    esp_err_t ret = ESP_ERR_NOT_FOUND;
    xSemaphoreTake(p_data->client_list_mutex, portMAX_DELAY);
    for (uint8_t i = 0; i < p_data->client_count; i++) {
        if (strcmp(p_data->clients[i].module_name, module_name) == 0) {
            p_data->clients[i].last_heartbeat_time = esp_timer_get_time();
            p_data->clients[i].is_alive = true;
            ret = ESP_OK;
            break;
        }
    }
    xSemaphoreGive(p_data->client_list_mutex);

    if (ret == ESP_OK) {
        ESP_LOGD(TAG, "Heartbeat received from '%s'", module_name);
    } else {
        ESP_LOGW(TAG, "Heartbeat received from unregistered module '%s'", module_name);
    }
    return ret;
}

static esp_err_t api_unregister_heartbeat(const char *module_name)
{
    // TODO: იმპლემენტაცია საჭიროებისამებრ
    return ESP_ERR_NOT_SUPPORTED;
}

// =============================================================================
// Internal Helper Functions
// =============================================================================
static void watchdog_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    watchdog_manager_private_data_t *p_data = (watchdog_manager_private_data_t *)self->private_data;

    // დავამატოთ ეს ტასკი TWDT-ის მონიტორინგის სიაში
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    ESP_LOGI(TAG, "Watchdog monitoring task started.");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(p_data->check_interval_ms));

        bool all_alive = true;
        char *failed_module = NULL;

        xSemaphoreTake(p_data->client_list_mutex, portMAX_DELAY);
        int64_t current_time = esp_timer_get_time();
        for (uint8_t i = 0; i < p_data->client_count; i++)
        {
            int64_t time_since_last_hb = (current_time - p_data->clients[i].last_heartbeat_time) / 1000;
            if (time_since_last_hb > p_data->heartbeat_timeout_ms)
            {
                if (p_data->clients[i].is_alive) { // შევამოწმოთ, რომ alert-ი მხოლოდ ერთხელ გამოქვეყნდეს
                    p_data->clients[i].is_alive = false;
                    failed_module = p_data->clients[i].module_name;
                }
                all_alive = false;
            }
        }
        xSemaphoreGive(p_data->client_list_mutex);

        if (failed_module) {
            ESP_LOGE(TAG, "WATCHDOG ALERT: Heartbeat missed from module '%s'!", failed_module);
            // TODO: გამოვაქვეყნოთ WATCHDOG_HEARTBEAT_MISSED ივენთი
        }

        if (all_alive)
        {
            ESP_LOGD(TAG, "All systems nominal. Feeding watchdog.");
            esp_task_wdt_reset();
        }
        else
        {
            ESP_LOGE(TAG, "Heartbeat missed! Not feeding watchdog. System will reboot soon.");
            // TODO: გამოვაქვეყნოთ WATCHDOG_TRIGGERED_REBOOT ივენთი
        }
    }
}

static esp_err_t parse_config(const cJSON *config, watchdog_manager_private_data_t *p_data)
{
    if (!p_data) return ESP_ERR_INVALID_ARG;

    p_data->check_interval_ms = 2000;
    p_data->heartbeat_timeout_ms = 5000;

    if (config) {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node))
        {
            const cJSON *check_interval = cJSON_GetObjectItem(config_node, "check_interval_ms");
            if (cJSON_IsNumber(check_interval)) p_data->check_interval_ms = check_interval->valueint;

            const cJSON *hb_timeout = cJSON_GetObjectItem(config_node, "heartbeat_timeout_ms");
            if (cJSON_IsNumber(hb_timeout)) p_data->heartbeat_timeout_ms = hb_timeout->valueint;
        }
    }
    
    // ★★★ ვალიდაცია: შემოწმების ინტერვალი უნდა იყოს Watchdog-ის ტაიმაუტზე ნაკლები ★★★
    if (p_data->check_interval_ms >= (CONFIG_ESP_TASK_WDT_TIMEOUT_S * 1000)) {
        ESP_LOGE(TAG, "Configuration error: check_interval_ms (%" PRIu32 ") must be less than TWDT timeout (%d s)!",
                 p_data->check_interval_ms, CONFIG_ESP_TASK_WDT_TIMEOUT_S);
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}