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

DEFINE_COMPONENT_TAG("SYSTEM_MANAGER");

/**
 * @internal
 * @brief A static list of all registered and sorted module instances.
 */
static const module_t **s_modules = NULL;
static uint8_t s_module_count = 0;

// --- Forward declarations for internal functions ---
static esp_err_t resolve_dependencies_for_module(module_t *module);

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

esp_err_t fmw_system_init(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "--- System Core Initialization ---");

    err = fmw_service_locator_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Service Locator: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Service Locator initialized.");

    err = fmw_resource_manager_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Resource Manager: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Resource Manager initialized.");

    err = fmw_config_manager_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Config Manager: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Config Manager initialized.");

    err = fmw_event_bus_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Event Bus: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Event Bus initialized.");

    err = fmw_promise_manager_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Promise Manager: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Promise Manager initialized.");

    err = fmw_service_register("system_manager", FMW_SERVICE_TYPE_SYSTEM_API, &system_manager_service_api);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register System Manager service: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "System Manager service registered.");

    err = fmw_module_registry_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Module Registry: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "Module Registry initialized.");

    fmw_module_registry_get_all(&s_modules, &s_module_count);

    ESP_LOGI(TAG, "--- System Core Initialization Finished: %d modules loaded ---", s_module_count);
    return ESP_OK;
}

esp_err_t fmw_system_start(void)
{
    esp_err_t err;
    ESP_LOGI(TAG, "--- Starting all operational modules ---");

    // --- STAGE 1: Initialize modules in sequence, injecting dependencies just-in-time ---
    for (uint8_t i = 0; i < s_module_count; i++)
    {
        module_t *module = (module_t *)s_modules[i];

        // Step 1.1: Resolve and inject dependencies for the current module.
        // Because modules are sorted by init_level, any required service from a
        // lower-level module will have already been registered in its own init phase.
        err = resolve_dependencies_for_module(module);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to resolve dependencies for module '%s'. Disabling it.", module->name);
            module->status = MODULE_STATUS_ERROR;
            continue; // Skip to the next module
        }

        // Step 1.2: Initialize the module itself.
        if (module && module->base.init)
        {
            ESP_LOGI(TAG, "Initializing module: '%s' (level %d)", module->name, module->init_level);
            err = module->base.init(module);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to initialize module '%s': %s. Disabling it.", module->name, esp_err_to_name(err));
                module->status = MODULE_STATUS_ERROR;
            }
            else
            {
                module->status = MODULE_STATUS_INITIALIZED;
            }
        }
    }

    // --- STAGE 2: Start all successfully initialized modules ---
    for (uint8_t i = 0; i < s_module_count; i++)
    {
        module_t *module = (module_t *)s_modules[i];
        if (module && module->base.start && module->status == MODULE_STATUS_INITIALIZED)
        {
            ESP_LOGI(TAG, "Starting module: '%s'", module->name);
            err = module->base.start(module);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Failed to start module '%s': %s", module->name, esp_err_to_name(err));
                module->status = MODULE_STATUS_ERROR;
            }
            else
            {
                module->status = MODULE_STATUS_RUNNING;
            }
        }
    }

    ESP_LOGI(TAG, "--- System is running. ---");

    // --- STAGE 3: Notify system that startup is complete ---
    ESP_LOGI(TAG, "Publishing FMW_EVENT_SYSTEM_START_COMPLETE event.");
    fmw_event_bus_post(FMW_EVENT_SYSTEM_START_COMPLETE, NULL);

    return ESP_OK;
}

/**
 * @internal
 * @brief Scans a single module's configuration and injects its service dependencies.
 * @details This function scans the configuration of the provided module, looking for keys
 *          that end with the "_service" suffix. For each such key found, it
 *          retrieves the corresponding service from the Service Locator and
 *          injects the service handle into the module's private_data structure.
 *
 *          **Injection Convention:**
 *          This mechanism relies on a strict convention. The module's `private_data`
 *          struct MUST declare a pointer for the service handle as its VERY FIRST member.
 *          The name of this pointer does not matter, but its position is critical.
 *
 * @param[in] module The module instance to process.
 * @return ESP_OK on success, or ESP_FAIL if a dependency cannot be resolved.
 */
static esp_err_t resolve_dependencies_for_module(module_t *module)
{
    if (!module || !module->dependency_map || !module->current_config || !module->private_data)
    {
        return ESP_OK; // This module has no dependencies to inject.
    }

    ESP_LOGD(TAG, "Checking dependencies for module '%s'...", module->name);

    const cJSON *config_node = cJSON_GetObjectItem(module->current_config, "config");
    if (!cJSON_IsObject(config_node))
    {
        // If a dependency map exists, a config node should also exist.
        ESP_LOGE(TAG, "Module '%s' has a dependency map but no 'config' object in its configuration.", module->name);
        return ESP_FAIL;
    }

    // Iterate through the dependency map provided by the module
    for (int i = 0; module->dependency_map[i].config_key != NULL; i++)
    {
        const module_dependency_t *dep = &module->dependency_map[i];

        // Find the service name in the module's config using the key from the map
        const cJSON *service_name_node = cJSON_GetObjectItem(config_node, dep->config_key);
        if (!cJSON_IsString(service_name_node))
        {
            ESP_LOGW(TAG, "Dependency key '%s' not found or not a string in config for module '%s'. Skipping.", dep->config_key, module->name);
            continue; // This dependency is optional
        }

        const char *service_name = service_name_node->valuestring;
        ESP_LOGD(TAG, "Module '%s' depends on service '%s'. Resolving...", module->name, service_name);

        // Find the service handle in the Service Locator
        service_handle_t service_handle = fmw_service_get(service_name);
        if (!service_handle)
        {
            ESP_LOGE(TAG, "FATAL: Dependency not met for module '%s'. Service '%s' not found!", module->name, service_name);
            return ESP_FAIL;
        }

        // --- Safe Injection Logic ---
        // Calculate the absolute memory address of the target pointer field
        // inside the module's private_data struct.
        void **target_handle_ptr = (void **)((uint8_t *)module->private_data + dep->offset);

        // Write the found service handle to that address.
        *target_handle_ptr = service_handle;

        ESP_LOGI(TAG, "Successfully injected service '%s' into module '%s'.", service_name, module->name);
    }

    return ESP_OK;
}

// --- Runtime Functions ---

esp_err_t fmw_module_enable(const char *module_name)
{
    if (!module_name)
        return ESP_ERR_INVALID_ARG;

    module_t *module = fmw_module_registry_find_by_name(module_name);
    if (!module)
        return ESP_ERR_NOT_FOUND;

    if (module->base.enable)
    {
        return module->base.enable(module);
    }
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t fmw_module_disable(const char *module_name)
{
    if (!module_name)
        return ESP_ERR_INVALID_ARG;

    module_t *module = fmw_module_registry_find_by_name(module_name);
    if (!module)
        return ESP_ERR_NOT_FOUND;

    if (module->base.disable)
    {
        return module->base.disable(module);
    }
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t fmw_module_reconfigure(const char *module_name, const cJSON *new_config)
{
    if (!module_name || !new_config)
        return ESP_ERR_INVALID_ARG;

    module_t *module = fmw_module_registry_find_by_name(module_name);
    if (!module)
        return ESP_ERR_NOT_FOUND;

    if (module->base.reconfigure)
    {
        return module->base.reconfigure(module, new_config);
    }
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t fmw_module_get_status(const char *module_name, module_status_t *status)
{
    if (!module_name || !status)
        return ESP_ERR_INVALID_ARG;

    module_t *module = fmw_module_registry_find_by_name(module_name);
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

const cJSON *fmw_module_get_config(const char *module_name)
{
    if (!module_name)
        return NULL;

    const module_t *module = fmw_module_registry_find_by_name(module_name);
    if (!module)
        return NULL;

    return module->current_config;
}

// =========================================================================
//                      Service API Implementation
// =========================================================================

static esp_err_t system_manager_get_all_modules_api(const module_t ***modules, uint8_t *count)
{
    return fmw_module_registry_get_all(modules, count);
}

static esp_err_t system_manager_reboot_api(void)
{
    ESP_LOGW(TAG, "Reboot requested via API. Rebooting in 100ms...");
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_restart();
    return ESP_OK; // This line will not be reached
}