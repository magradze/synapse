/**
 * @file system_manager.c
 * @brief System Manager კომპონენტის იმპლემენტაცია.
 * @details ეს ფაილი შეიცავს ფრეიმვორქის ინიციალიზაციის, გაშვების და
 *          runtime მართვის ლოგიკას. System Manager არის სისტემის
 *          "დირიჟორი", რომელიც კოორდინაციას უწევს ბირთვის კომპონენტებისა
 *          და მოდულების მუშაობას.
 * @author Giorgi Magradze
 * @version 2.0.0
 * @date 2025-06-26
 */

#include "system_manager.h"
#include "logging.h"
#include "config_manager.h"
#include "event_bus.h"
#include "module_registry.h"
#include "resource_manager.h"
#include "service_locator.h"
#include "service_types.h"
// #include "event_payloads.h" // წაშლილია, რადგან ფაილი არ არსებობს
#include "freertos/FreeRTOS.h" // დაკარგული ჰედერები
#include "freertos/task.h"
#include "system_manager_interface.h"

DEFINE_COMPONENT_TAG("SYSTEM_MANAGER");

/**
 * @internal
 * @brief ინიციალიზაციისას გაშვებული მოდულების სია.
 */
static const module_t **g_modules = NULL;
static uint8_t g_module_count = 0;

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

    // ინიციალიზაციის თანმიმდევრობა კრიტიკულია
    err = fmw_service_locator_init();
    if (err != ESP_OK) { ESP_LOGE(TAG, "Failed to initialize Service Locator: %s", esp_err_to_name(err)); return err; }
    ESP_LOGI(TAG, "Service Locator initialized.");

    err = fmw_resource_manager_init();
    if (err != ESP_OK) { ESP_LOGE(TAG, "Failed to initialize Resource Manager: %s", esp_err_to_name(err)); return err; }
    ESP_LOGI(TAG, "Resource Manager initialized.");
    
    err = fmw_config_manager_init();
    if (err != ESP_OK) { ESP_LOGE(TAG, "Failed to initialize Config Manager: %s", esp_err_to_name(err)); return err; }
    ESP_LOGI(TAG, "Config Manager initialized.");

    err = fmw_event_bus_init();
    if (err != ESP_OK) { ESP_LOGE(TAG, "Failed to initialize Event Bus: %s", esp_err_to_name(err)); return err; }
    ESP_LOGI(TAG, "Event Bus initialized.");

    // Register the System Manager's own service API
    err = fmw_service_register("system_manager", FMW_SERVICE_TYPE_SYSTEM_API, &system_manager_service_api);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register System Manager service: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "System Manager service registered.");

    err = fmw_module_registry_init();
    if (err != ESP_OK) { ESP_LOGE(TAG, "Failed to initialize Module Registry: %s", esp_err_to_name(err)); return err; }
    ESP_LOGI(TAG, "Module Registry initialized.");

    // მივიღოთ მოდულების სია შემდგომი ოპერაციებისთვის
    fmw_module_registry_get_all(&g_modules, &g_module_count);

    ESP_LOGI(TAG, "--- System Core Initialization Finished: %d modules loaded ---", g_module_count);
    return ESP_OK;
}

esp_err_t fmw_system_start(void)
{
    esp_err_t err;
    ESP_LOGI(TAG, "--- Starting all operational modules ---");

    // ნაბიჯი 1: ყველა მოდულის init() ფუნქციის გაშვება
    for (uint8_t i = 0; i < g_module_count; i++) {
        module_t *module = (module_t *)g_modules[i];
        if (module && module->base.init) {
            ESP_LOGI(TAG, "Initializing module: '%s'", module->name);
            err = module->base.init(module);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Failed to initialize module '%s': %s. Disabling it.", module->name, esp_err_to_name(err));
                module->status = MODULE_STATUS_ERROR;
            } else {
                module->status = MODULE_STATUS_INITIALIZED;
            }
        }
    }

    // ნაბიჯი 2: ყველა წარმატებით ინიციალიზებული მოდულის start() ფუნქციის გაშვება
    for (uint8_t i = 0; i < g_module_count; i++) {
        module_t *module = (module_t *)g_modules[i];
        if (module && module->base.start && module->status == MODULE_STATUS_INITIALIZED) {
            ESP_LOGI(TAG, "Starting module: '%s'", module->name);
            err = module->base.start(module);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Failed to start module '%s': %s", module->name, esp_err_to_name(err));
                module->status = MODULE_STATUS_ERROR;
            } else {
                module->status = MODULE_STATUS_RUNNING;
            }
        }
    }

    ESP_LOGI(TAG, "--- System is running. ---");

    // ★★★ ახალი ლოგიკა ★★★
    // ვაქვეყნებთ ივენთს, რომ სისტემის გაშვება დასრულდა.
    ESP_LOGI(TAG, "Publishing FMW_EVENT_SYSTEM_START_COMPLETE event.");
    fmw_event_bus_post("FMW_EVENT_SYSTEM_START_COMPLETE", NULL); // მონაცემები არ გვჭირდება

    return ESP_OK;
}

// --- Runtime ფუნქციები ---

esp_err_t fmw_module_enable(const char *module_name)
{
    if (!module_name) return ESP_ERR_INVALID_ARG;
    
    const module_t *module = fmw_module_registry_find_by_name(module_name);
    if (!module) return ESP_ERR_NOT_FOUND;

    // `enable` ფუნქცია უნდა იყოს const-safe, რადგან ის ცვლის მხოლოდ სტატუსს.
    // თუ მას სჭირდება private_data-ს შეცვლა, მაშინ find_by_name-მა უნდა დააბრუნოს არა-const მაჩვენებელი.
    // ამ ეტაპზე, ვთვლით, რომ `enable` მხოლოდ სტატუსს ცვლის.
    if (module->base.enable) {
        return module->base.enable((struct module_t *)module);
    }
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t fmw_module_disable(const char *module_name)
{
    if (!module_name) return ESP_ERR_INVALID_ARG;

    const module_t *module = fmw_module_registry_find_by_name(module_name);
    if (!module) return ESP_ERR_NOT_FOUND;

    if (module->base.disable) {
        return module->base.disable((struct module_t *)module);
    }
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t fmw_module_reconfigure(const char *module_name, const cJSON *new_config)
{
    if (!module_name || !new_config) return ESP_ERR_INVALID_ARG;

    const module_t *module = fmw_module_registry_find_by_name(module_name);
    if (!module) return ESP_ERR_NOT_FOUND;

    if (module->base.reconfigure) {
        return module->base.reconfigure((struct module_t *)module, new_config);
    }
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t fmw_module_get_status(const char *module_name, module_status_t *status)
{
    if (!module_name || !status) return ESP_ERR_INVALID_ARG;
    
    const module_t *module = fmw_module_registry_find_by_name(module_name);
    if (!module) return ESP_ERR_NOT_FOUND;

    if (module->base.get_status) {
        *status = module->base.get_status((struct module_t *)module);
    } else {
        *status = module->status; // დავაბრუნოთ შენახული სტატუსი, თუ get_status არ არსებობს
    }
    return ESP_OK;
}

const cJSON* fmw_module_get_config(const char *module_name)
{
    if (!module_name) return NULL;

    const module_t *module = fmw_module_registry_find_by_name(module_name);
    if (!module) return NULL;
    
    return module->current_config;
}

// =========================================================================
//                      Service API Implementation
// =========================================================================

/**
 * @internal
 * @brief Service API function to get the list of all modules.
 */
static esp_err_t system_manager_get_all_modules_api(const module_t ***modules, uint8_t *count)
{
    // This simply wraps the module registry's internal function
    return fmw_module_registry_get_all(modules, count);
}

/**
 * @internal
 * @brief Service API function to reboot the system.
 */
static esp_err_t system_manager_reboot_api(void)
{
    ESP_LOGW(TAG, "Reboot requested via API. Rebooting in 100ms...");
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_restart();
    return ESP_OK; // This line will not be reached
}
