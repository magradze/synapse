/**
 * @file ota_update_manager.c
 * @brief Handles secure Over-The-Air firmware updates
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-01
 * @details This module provides a high-level, robust, and secure OTA update
 *          mechanism. It integrates with other framework services like Health
 *          Monitor and RGB LED Indicator to provide pre-update checks and
 *          visual feedback to the user.
 */

#include "synapse.h"
#include "ota_update_manager.h"
#include "ota_interface.h"
#include "health_interface.h"
#include "rgb_led_interface.h"

#include "esp_log.h"
#include "esp_https_ota.h"
#include "esp_app_format.h"
#include "esp_crt_bundle.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("OTA_MANAGER", SYNAPSE_LOG_COLOR_BLUE);

// --- Event Names ---
#define EVT_OTA_STARTED "OTA_STARTED"
#define EVT_OTA_SUCCESS "OTA_SUCCESS"
#define EVT_OTA_FAILED "OTA_FAILED"
#define EVT_OTA_PROGRESS "OTA_PROGRESS"

// --- Private Data Structures ---
typedef struct
{
    char instance_name[CONFIG_OTA_UPDATE_MANAGER_INSTANCE_NAME_MAX_LEN];
    TaskHandle_t ota_task_handle;
    char *update_url;
    bool ota_in_progress;
    uint32_t pre_check_min_heap_kb;
} ota_update_manager_private_data_t;

// --- Forward declarations ---
static esp_err_t ota_update_manager_init(module_t *self);
static esp_err_t ota_update_manager_start(module_t *self);
static void ota_update_manager_deinit(module_t *self);
static module_status_t ota_update_manager_get_status(module_t *self);
static esp_err_t parse_config(const cJSON *config, ota_update_manager_private_data_t *private_data);

static void ota_task(void *pvParameters);
static esp_err_t api_start_update(const char *firmware_url);

// --- Global Variables for Service API ---
static module_t *global_ota_instance = NULL;
static ota_api_t ota_service_api = {
    .start_update = api_start_update,
};

// =============================================================================
// Public API - Module Creation
// =============================================================================
module_t *ota_update_manager_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating ota_update_manager module instance");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    ota_update_manager_private_data_t *private_data = (ota_update_manager_private_data_t *)calloc(1, sizeof(ota_update_manager_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for module structures");
        free(private_data);
        free(module);
        return NULL;
    }

    module->private_data = private_data;
    private_data->ota_in_progress = false;

    // --- Configuration Handling ---
    // Create a copy of the config first to ensure it's always available for deinit
    if (config)
    {
        module->current_config = cJSON_Duplicate(config, true);
        if (!module->current_config)
        {
            ESP_LOGE(TAG, "Failed to duplicate configuration object.");
            free(private_data);
            free(module);
            return NULL;
        }
    }

    // Parse the configuration
    if (parse_config(module->current_config, private_data) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to parse configuration for OTA manager.");
        ota_update_manager_deinit(module); // Use deinit for full cleanup
        return NULL;
    }

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);

    module->init_level = 80;
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->base.init = ota_update_manager_init;
    module->base.start = ota_update_manager_start;
    module->base.deinit = ota_update_manager_deinit;
    module->base.get_status = ota_update_manager_get_status;
    module->base.enable = NULL;
    module->base.disable = NULL;
    module->base.reconfigure = NULL;
    module->base.handle_event = NULL;

    // --- Service Registration Moved to Create Phase ---
    esp_err_t ret = synapse_service_register_with_status(
        module->name,
        SYNAPSE_SERVICE_TYPE_OTA_API,
        &ota_service_api, // Assuming global static API struct
        SERVICE_STATUS_REGISTERED);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register service for '%s' (%s). Cleaning up.", module->name, esp_err_to_name(ret));
        ota_update_manager_deinit(module);
        return NULL;
    }

    global_ota_instance = module;

    ESP_LOGI(TAG, "OTA Update Manager module '%s' created and service registered.", module->name);
    return module;
}

static esp_err_t ota_update_manager_init(module_t *self)
{
    ESP_LOGI(TAG, "Initializing OTA Update Manager.");
    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

static esp_err_t ota_update_manager_start(module_t *self)
{
    ESP_LOGI(TAG, "Starting OTA Update Manager.");
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void ota_update_manager_deinit(module_t *self)
{
    if (!self)
        return;
    ESP_LOGI(TAG, "Deinitializing %s module", self->name);
    ota_update_manager_private_data_t *private_data = (ota_update_manager_private_data_t *)self->private_data;

    if (private_data && private_data->ota_task_handle)
    {
        vTaskDelete(private_data->ota_task_handle);
    }
    if (private_data && private_data->update_url)
    {
        free(private_data->update_url);
    }

    synapse_service_unregister(self->name);
    global_ota_instance = NULL;

    if (self->private_data)
        free(self->private_data);
    if (self->current_config)
        cJSON_Delete(self->current_config);
    if (self->state_mutex)
        vSemaphoreDelete(self->state_mutex);
}

static module_status_t ota_update_manager_get_status(module_t *self)
{
    return self->status;
}

// =============================================================================
// Service API Implementation
// =============================================================================
static esp_err_t api_start_update(const char *firmware_url)
{
    if (!global_ota_instance || !firmware_url)
    {
        return ESP_ERR_INVALID_ARG;
    }

    ota_update_manager_private_data_t *private_data = (ota_update_manager_private_data_t *)global_ota_instance->private_data;

    if (private_data->ota_in_progress)
    {
        ESP_LOGE(TAG, "OTA update is already in progress.");
        return ESP_ERR_INVALID_STATE;
    }

    // 1. Health Check
    service_handle_t health_handle = synapse_service_get("main_health_monitor");
    if (health_handle)
    {
        health_api_t *health_api = (health_api_t *)health_handle;
        cJSON *report = NULL;
        if (health_api->get_system_health_report(&report) == ESP_OK && report)
        {
            const cJSON *heap = cJSON_GetObjectItem(report, "free_heap_bytes");
            if (cJSON_IsNumber(heap) && heap->valueint < (private_data->pre_check_min_heap_kb * 1024))
            {
                ESP_LOGE(TAG, "Not enough memory for OTA update. Required: %lu KB, Available: %d bytes. Aborting.",
                         private_data->pre_check_min_heap_kb, heap->valueint);
                cJSON_Delete(report);
                return ESP_ERR_NO_MEM;
            }
            cJSON_Delete(report);
        }
    }

    // 2. URL-ის კოპირება
    if (private_data->update_url)
    {
        free(private_data->update_url);
    }
    private_data->update_url = strdup(firmware_url);
    if (!private_data->update_url)
    {
        return ESP_ERR_NO_MEM;
    }

    // 3. OTA ტასკის გაშვება
    BaseType_t task_created = xTaskCreate(
        ota_task,
        "ota_task",
        8192, // OTA-ს სჭირდება დიდი სტეკი
        global_ota_instance,
        5,
        &private_data->ota_task_handle);

    if (task_created != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create OTA task");
        free(private_data->update_url);
        private_data->update_url = NULL;
        return ESP_FAIL;
    }

    return ESP_OK;
}

// =============================================================================
// Internal Helper Functions
// =============================================================================
static void ota_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    ota_update_manager_private_data_t *private_data = (ota_update_manager_private_data_t *)self->private_data;

    private_data->ota_in_progress = true;

    // ვიზუალური ინდიკაცია და ივენთი
    ESP_LOGI(TAG, "Starting OTA update from: %s", private_data->update_url);
    synapse_event_bus_post(EVT_OTA_STARTED, NULL);
    service_handle_t led_handle = synapse_service_get("status_led");
    rgb_led_api_t *led_api = led_handle ? (rgb_led_api_t *)led_handle : NULL;
    if (led_api)
        led_api->start_pulse(0, 0, 255, 2000); // ლურჯი პულსაცია

    esp_http_client_config_t config = {
        .url = private_data->update_url,
        .crt_bundle_attach = esp_crt_bundle_attach, // HTTPS-ისთვის
        .keep_alive_enable = true,
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };

    esp_err_t ret = esp_https_ota(&ota_config);

    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "OTA Update successful! Rebooting...");
        synapse_event_bus_post(EVT_OTA_SUCCESS, NULL);
        if (led_api)
            led_api->set_color(0, 255, 0); // მწვანე
        vTaskDelay(pdMS_TO_TICKS(2000));
        esp_restart();
    }
    else
    {
        ESP_LOGE(TAG, "OTA Update failed: %s", esp_err_to_name(ret));
        synapse_event_bus_post(EVT_OTA_FAILED, NULL);
        if (led_api)
            led_api->start_blink(255, 0, 0, 200); // სწრაფი წითელი ციმციმი
        // არ ვათავისუფლებთ კონტროლს, რათა შეცდომა ხილული დარჩეს
    }

    // ტასკის დასრულება
    private_data->ota_in_progress = false;
    free(private_data->update_url);
    private_data->update_url = NULL;
    private_data->ota_task_handle = NULL;
    vTaskDelete(NULL);
}

static esp_err_t parse_config(const cJSON *config, ota_update_manager_private_data_t *private_data)
{
    if (!private_data || !config)
    {
        return ESP_ERR_INVALID_ARG;
    }

    // --- Step 1: Set default values ---
    private_data->pre_check_min_heap_kb = 80; // Default minimum required heap in KB
    // Set a default instance name in case it's missing in the config
    synapse_safe_strncpy(private_data->instance_name, "main_ota_manager", sizeof(private_data->instance_name));

    // --- Step 2: Get the main "config" node ---
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (!config_node)
    {
        ESP_LOGW(TAG, "No 'config' object in JSON. Using default values.");
        return ESP_OK;
    }

    // --- Step 3: Use utility functions to parse values ---
    synapse_config_get_string_from_node(TAG, config_node, "instance_name",
                                        private_data->instance_name, sizeof(private_data->instance_name));

    synapse_config_get_int_from_node(TAG, config_node, "pre_check_min_heap_kb",
                                     (int *)&private_data->pre_check_min_heap_kb);

    ESP_LOGI(TAG, "Config parsed for '%s': OTA pre-check min heap is %" PRIu32 " KB",
             private_data->instance_name, private_data->pre_check_min_heap_kb);

    return ESP_OK;
}