/**
 * @file system_manager.c
 * @brief Implements the System Manager component.
 * @author Giorgi Magradze
 * @version 3.1.0
 * @date 2025-08-27
 *
 * @details
 * This file contains the core logic for the framework's initialization, startup,
 * and runtime management. The System Manager acts as the system's orchestrator,
 * coordinating the lifecycle of all core components and modules.
 *
 * This version implements a robust, multi-stage startup process that correctly
 * handles dependency injection by resolving dependencies just-in-time before
 * each module's initialization, respecting the `init_level` order.
 */

#include "synapse.h"
#include "system_manager_interface.h"

DEFINE_COMPONENT_TAG("SYSTEM_MANAGER", SYNAPSE_LOG_COLOR_BLUE);

// --- Structure for passing data to the temporary deinit task ---
typedef struct
{
    module_t *module_to_deinit;
    SemaphoreHandle_t done_semaphore;
} deinit_task_params_t;

/**
 * @internal
 * @brief A static list of all registered and sorted module instances.
 */
static const module_t **s_registered_modules = NULL;
static uint8_t s_registered_module_count = 0;

// --- Forward declarations for internal functions ---
static esp_err_t resolve_dependencies_for_module(module_t *module);
static void temporary_deinit_task(void *pvParameters);

// --- Forward declarations for API implementation ---
static esp_err_t system_manager_get_all_modules_api(const module_t ***modules, uint8_t *count);
static esp_err_t system_manager_reboot_api(void);

/**
 * @internal
 * @brief The static instance of the System Manager's service API.
 */
static system_manager_api_t system_manager_service_api = {
    .get_all_modules = system_manager_get_all_modules_api,
    .reboot_system = system_manager_reboot_api,
};

esp_err_t synapse_system_init(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "--- System Core Initialization ---");

    err = synapse_service_locator_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Service Locator: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Service Locator initialized.");

    err = synapse_resource_manager_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Resource Manager: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Resource Manager initialized.");

    err = synapse_config_manager_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Config Manager: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Config Manager initialized.");

    err = synapse_event_bus_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Event Bus: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Event Bus initialized.");

    err = synapse_promise_manager_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Promise Manager: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Promise Manager initialized.");

    err = synapse_task_pool_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Task Pool Manager: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Task Pool Manager initialized.");

    err = synapse_service_register_with_status(
        "system_manager",
        SYNAPSE_SERVICE_TYPE_SYSTEM_API,
        &system_manager_service_api,
        SERVICE_STATUS_ACTIVE // System Manager service is active as soon as core init is done.
    );
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register System Manager service: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "System Manager service registered and active.");

    err = synapse_module_registry_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Module Registry: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Module Registry initialized.");

    synapse_module_registry_get_all(&s_registered_modules, &s_registered_module_count);

    ESP_LOGI(TAG, "--- System Core Initialization Finished: %d modules loaded ---", s_registered_module_count);
    return ESP_OK;
}

esp_err_t synapse_system_start(void)
{
    esp_err_t err;
    ESP_LOGI(TAG, "--- Starting all operational modules ---");

    // --- STAGE 1: Initialize modules in sequence ---
    for (uint8_t i = 0; i < s_registered_module_count; i++)
    {
        module_t *module = (module_t *)s_registered_modules[i];

        err = resolve_dependencies_for_module(module);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to resolve dependencies for module '%s'. Disabling it.", module->name);
            module->status = MODULE_STATUS_ERROR;
            // Set service status to ERROR as well, if the service was registered
            synapse_service_set_status(module->name, SERVICE_STATUS_ERROR);
            continue;
        }

        if (module && module->base.init)
        {
            ESP_LOGI(TAG, "Initializing module: '%s' (level %d)", module->name, module->init_level);

            // Set status to INITIALIZING only if the service exists
            if (synapse_service_get(module->name) != NULL)
            {
                synapse_service_set_status(module->name, SERVICE_STATUS_INITIALIZING);
            }

            err = module->base.init(module);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to initialize module '%s': %s. Disabling it.", module->name, esp_err_to_name(err));
                module->status = MODULE_STATUS_ERROR;
                if (synapse_service_get(module->name) != NULL)
                {
                    synapse_service_set_status(module->name, SERVICE_STATUS_ERROR);
                }
            }
            else
            {
                module->status = MODULE_STATUS_INITIALIZED;
                // Status remains INITIALIZING until start() is complete
            }
        }
    }

    // --- STAGE 2: Start all successfully initialized modules ---
    for (uint8_t i = 0; i < s_registered_module_count; i++)
    {
        module_t *module = (module_t *)s_registered_modules[i];
        if (module && module->base.start && module->status == MODULE_STATUS_INITIALIZED)
        {
            ESP_LOGI(TAG, "Starting module: '%s'", module->name);
            err = module->base.start(module);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to start module '%s': %s", module->name, esp_err_to_name(err));
                module->status = MODULE_STATUS_ERROR;
                if (synapse_service_get(module->name) != NULL)
                {
                    synapse_service_set_status(module->name, SERVICE_STATUS_ERROR);
                }
            }
            else
            {
                module->status = MODULE_STATUS_RUNNING;
                if (synapse_service_get(module->name) != NULL)
                {
                    synapse_service_set_status(module->name, SERVICE_STATUS_ACTIVE);
                }

                if (module->base.ui_init)
                {
                    ESP_LOGI(TAG, "Initializing UI for module '%s'", module->name);
                    module->base.ui_init(module);
                }
            }
        }
    }

    ESP_LOGI(TAG, "--- System is running. ---");

    ESP_LOGI(TAG, "Publishing SYNAPSE_EVENT_SYSTEM_START_COMPLETE event.");
    synapse_event_bus_post(SYNAPSE_EVENT_SYSTEM_START_COMPLETE, NULL);

    return ESP_OK;
}

static esp_err_t resolve_dependencies_for_module(module_t *module)
{
    if (!module || !module->dependency_map || !module->private_data)
    {
        return ESP_OK; // No dependencies to inject.
    }

    ESP_LOGD(TAG, "Checking dependencies for module '%s'...", module->name);

    const cJSON *config_node = cJSON_GetObjectItem(module->current_config, "config");
    if (!cJSON_IsObject(config_node))
    {
        ESP_LOGE(TAG, "Module '%s' has a dependency map but no 'config' object.", module->name);
        return ESP_FAIL;
    }

    for (int i = 0; module->dependency_map[i].config_key != NULL; i++)
    {
        const module_dependency_t *dep = &module->dependency_map[i];
        const cJSON *name_node = cJSON_GetObjectItem(config_node, dep->config_key);

        if (!cJSON_IsString(name_node))
        {
            ESP_LOGW(TAG, "Dependency key '%s' not found in config for module '%s'. Skipping.", dep->config_key, module->name);
            continue;
        }

        const char *name_to_find = name_node->valuestring;
        void *handle_to_inject = NULL;

        if (strstr(dep->config_key, "_service") != NULL)
        {
            ESP_LOGD(TAG, "Module '%s' depends on SERVICE '%s'. Resolving...", module->name, name_to_find);

#if CONFIG_SYNAPSE_SERVICE_LOCATOR_STRICT_MODE
            service_status_t dep_status;
            esp_err_t status_err = synapse_service_get_status(name_to_find, &dep_status);

            if (status_err != ESP_OK)
            {
                ESP_LOGE(TAG, "FATAL (Strict Mode): Dependency service '%s' for module '%s' not found!", name_to_find, module->name);
                return ESP_FAIL;
            }
            if (dep_status != SERVICE_STATUS_ACTIVE)
            {
                ESP_LOGE(TAG, "FATAL (Strict Mode): Dependency service '%s' for module '%s' is not ACTIVE (current: %s).",
                         name_to_find, module->name, service_status_to_string(dep_status));
                return ESP_FAIL;
            }
#endif
            handle_to_inject = synapse_service_get(name_to_find);
            if (!handle_to_inject)
            { // This check is redundant in strict mode, but good for safety
                ESP_LOGE(TAG, "FATAL: Service '%s' not found for module '%s'!", name_to_find, module->name);
                return ESP_FAIL;
            }
        }
        else if (strstr(dep->config_key, "_handle") != NULL)
        {
            ESP_LOGD(TAG, "Module '%s' depends on MODULE HANDLE '%s'. Resolving...", module->name, name_to_find);
            handle_to_inject = synapse_module_registry_find_by_name(name_to_find);
            if (!handle_to_inject)
            {
                ESP_LOGE(TAG, "FATAL: Module handle '%s' not found for module '%s'!", name_to_find, module->name);
                return ESP_FAIL;
            }
        }
        else
        {
            ESP_LOGW(TAG, "Skipping unknown dependency type for key '%s' in module '%s'.", dep->config_key, module->name);
            continue;
        }

        // --- Safe Injection Logic ---
        void **target_handle_ptr = (void **)((uint8_t *)module->private_data + dep->offset);
        *target_handle_ptr = handle_to_inject;

        ESP_LOGI(TAG, "Successfully injected dependency '%s' into module '%s'.", name_to_find, module->name);
    }

    return ESP_OK;
}

// --- Runtime Functions ---

esp_err_t synapse_module_enable(const char *module_name)
{
    if (!module_name)
        return ESP_ERR_INVALID_ARG;

    module_t *module = synapse_module_registry_find_by_name(module_name);
    if (!module)
        return ESP_ERR_NOT_FOUND;

    if (module->base.enable)
    {
        return module->base.enable(module);
    }
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t synapse_module_disable(const char *module_name)
{
    if (!module_name)
        return ESP_ERR_INVALID_ARG;

    module_t *module = synapse_module_registry_find_by_name(module_name);
    if (!module)
        return ESP_ERR_NOT_FOUND;

    if (module->base.disable)
    {
        return module->base.disable(module);
    }
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t synapse_module_reconfigure(const char *module_name, const cJSON *new_config)
{
    if (!module_name || !new_config)
        return ESP_ERR_INVALID_ARG;

    module_t *module = synapse_module_registry_find_by_name(module_name);
    if (!module)
        return ESP_ERR_NOT_FOUND;

    if (module->base.reconfigure)
    {
        return module->base.reconfigure(module, new_config);
    }
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t synapse_module_get_status(const char *module_name, module_status_t *status)
{
    if (!module_name || !status)
        return ESP_ERR_INVALID_ARG;

    module_t *module = synapse_module_registry_find_by_name(module_name);
    if (!module)
        return ESP_ERR_NOT_FOUND;

    if (module->base.get_status)
    {
        *status = module->base.get_status(module);
    }
    else
    {
        *status = module->status;
    }
    return ESP_OK;
}

const cJSON *synapse_module_get_config(const char *module_name)
{
    if (!module_name)
        return NULL;

    const module_t *module = synapse_module_registry_find_by_name(module_name);
    if (!module)
        return NULL;

    return module->current_config;
}

// =========================================================================
//                      Service API Implementation
// =========================================================================

static esp_err_t system_manager_get_all_modules_api(const module_t ***modules, uint8_t *count)
{
    return synapse_module_registry_get_all(modules, count);
}

void synapse_system_shutdown(void)
{
    ESP_LOGW(TAG, "--- GRACEFUL SHUTDOWN INITIATED ---");

    // Step 1: Notify all modules about the impending shutdown
    synapse_event_bus_post(SYNAPSE_EVENT_SYSTEM_SHUTDOWN_REQUESTED, NULL);

    // Step 2: Provide a short grace period for modules to finish critical tasks
    vTaskDelay(pdMS_TO_TICKS(200));

    ESP_LOGI(TAG, "Starting deinitialization of %d modules in reverse order...", s_registered_module_count);

    // Step 3: Deinitialize modules in reverse init_level order
    for (int i = s_registered_module_count - 1; i >= 0; i--)
    {
        module_t *module = (module_t *)s_registered_modules[i];

        if (module && module->base.ui_deinit)
        {
            ESP_LOGI(TAG, "Deinitializing UI for module '%s'", module->name);
            module->base.ui_deinit(module);
        }

        if (module && module->base.deinit)
        {
            // Set status to STOPPING before calling deinit()
            synapse_service_set_status(module->name, SERVICE_STATUS_STOPPING);

            ESP_LOGI(TAG, "Deinitializing module: '%s' (level %d)...", module->name, module->init_level);

            deinit_task_params_t params = {
                .module_to_deinit = module,
                .done_semaphore = xSemaphoreCreateBinary()};

            if (params.done_semaphore == NULL)
            {
                ESP_LOGE(TAG, "Failed to create semaphore for deinit task of '%s'. Skipping.", module->name);
                continue;
            }

            TaskHandle_t deinit_task_handle;
            BaseType_t task_created = xTaskCreate(
                temporary_deinit_task,
                "deinit_task",
                CONFIG_SYNAPSE_RUNTIME_TASK_STACK_SIZE,
                &params,
                5,
                &deinit_task_handle);

            if (task_created != pdPASS)
            {
                ESP_LOGE(TAG, "Failed to create deinit task for '%s'. Skipping.", module->name);
                vSemaphoreDelete(params.done_semaphore);
                continue;
            }

            if (xSemaphoreTake(params.done_semaphore, pdMS_TO_TICKS(CONFIG_SYNAPSE_DEINIT_TIMEOUT_MS)) == pdTRUE)
            {
                // --- შეცვლილი ლოგიკა ---
                // ლოგი იწერება აქ, სანამ module ობიექტი განადგურდება
                ESP_LOGI(TAG, "Module '%s' deinitialized successfully.", module->name);
                // ახლა კი, როდესაც ლოგირება დასრულდა, ვათავისუფლებთ module ობიექტს
                free(module);
                s_registered_modules[i] = NULL; // ვასუფთავებთ მასივს
            }
            else
            {
                ESP_LOGE(TAG, "Timeout waiting for deinit of module '%s'! Forcibly deleting task.", module->name);
                vTaskDelete(deinit_task_handle);
                // ამ შემთხვევაშიც, module ობიექტი რჩება მეხსიერებაში (leak), მაგრამ ეს უკეთესია, ვიდრე კრახი.
            }

            vSemaphoreDelete(params.done_semaphore);
        }
        else if (module) // თუ deinit არ აქვს, მაინც გავათავისუფლოთ
        {
            ESP_LOGI(TAG, "Module '%s' has no deinit function. Freeing structure.", module->name);
            free(module);
            s_registered_modules[i] = NULL;
        }
    }
    ESP_LOGW(TAG, "--- GRACEFUL SHUTDOWN COMPLETE. REBOOTING NOW. ---");
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_restart();
}

static esp_err_t system_manager_reboot_api(void)
{
    // The API call now triggers the graceful shutdown instead of an immediate restart
    synapse_system_shutdown();
    return ESP_OK; // This line will not be reached
}

static void temporary_deinit_task(void *pvParameters)
{
    deinit_task_params_t *params = (deinit_task_params_t *)pvParameters;

    // 1. Execute the module's internal cleanup
    params->module_to_deinit->base.deinit(params->module_to_deinit);

    // 2. Now, the System Manager frees the module structure itself
    // free(params->module_to_deinit);

    // 3. Signal that we are done
    xSemaphoreGive(params->done_semaphore);

    // The task has completed its purpose and can be deleted
    vTaskDelete(NULL);
}