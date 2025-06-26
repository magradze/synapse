/**
 * @file module_registry.c
 * @brief მოდულების რეგისტრაციისა და მართვის იმპლემენტაცია.
 * @version 2.0
 * @date 2025-06-26
 * @author Giorgi Magradze
 * @details ეს ფაილი იყენებს Module Factory-ს, რათა მოახდინოს მოდულების
 *          დინამიური რეგისტრაცია. ის კითხულობს `system_config.json`-ს,
 *          ქმნის საჭირო მოდულების ინსტანციებს და მართავს მათ სიცოცხლის ციკლს.
 */
#include "module_registry.h"
#include "module_factory.h"   // ვიყენებთ Factory-ს მოდულების შესაქმნელად
#include "config_manager.h"   // ვიყენებთ კონფიგურაციის წასაკითხად
#include "logging.h"
#include "framework_config.h" // Kconfig-ის პარამეტრებისთვის
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "base_module.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("MODULE_REGISTRY");

// --- კომპონენტის შიდა ცვლადები ---

/**
 * @internal
 * @brief რეგისტრირებული მოდულების მასივი შიდა აღრიცხვისთვის.
 */
static module_t* registered_modules[CONFIG_FMW_MAX_MODULES];
static uint8_t registered_modules_count = 0;
static SemaphoreHandle_t registry_mutex = NULL;

// --- შიდა ფუნქციების წინასწარი დეკლარაცია ---
static esp_err_t register_module(module_t *module);

// =========================================================================
//                      Public API Implementation
// =========================================================================

esp_err_t fmw_module_registry_init(void) {
    ESP_LOGI(TAG, "--- Module Registry Initialization ---");

    registry_mutex = xSemaphoreCreateMutex();
    if (!registry_mutex) {
        ESP_LOGE(TAG, "Failed to create registry mutex!");
        return ESP_ERR_NO_MEM;
    }
    
    /* Step 1: გავასუფთავოთ რეესტრის მდგომარეობა */
    memset(registered_modules, 0, sizeof(registered_modules));
    registered_modules_count = 0;

    /* Step 2: მივიღოთ კონფიგურაციის root ობიექტი */
    const cJSON *root = fmw_config_get_root();
    if (!root) {
        ESP_LOGE(TAG, "Failed to get configuration root. Cannot initialize modules.");
        return ESP_FAIL;
    }

    const cJSON *modules_array = cJSON_GetObjectItem(root, "modules");
    if (!cJSON_IsArray(modules_array)) {
        ESP_LOGW(TAG, "'modules' key not found or not an array in config. No modules will be loaded.");
        return ESP_OK; // ეს არ არის კრიტიკული შეცდომა
    }

    /* Step 3: გავიაროთ კონფიგურაცია და შევქმნათ მოდულების ინსტანციები */
    cJSON *module_config_json;
    cJSON_ArrayForEach(module_config_json, modules_array) {
        const cJSON *type_json = cJSON_GetObjectItem(module_config_json, "type");
        const cJSON *enabled_json = cJSON_GetObjectItem(module_config_json, "enabled");

        if (!cJSON_IsString(type_json) || !type_json->valuestring) {
            ESP_LOGW(TAG, "Skipping module with missing or invalid 'type' field.");
            continue;
        }

        if (cJSON_IsBool(enabled_json) && cJSON_IsFalse(enabled_json)) {
            ESP_LOGI(TAG, "Module type '%s' is disabled in config, skipping.", type_json->valuestring);
            continue;
        }

        module_t *new_module = fmw_module_factory_create(type_json->valuestring, module_config_json);

        if (new_module) {
            register_module(new_module);
        }
    }

    ESP_LOGI(TAG, "--- Module Registry Initialization Complete: %d modules registered ---", registered_modules_count);
    return ESP_OK;
}

module_t* fmw_module_registry_find_by_name(const char *name) {
    if (!name) return NULL;
    
    if (xSemaphoreTake(registry_mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to take mutex for find_by_name.");
        return NULL;
    }

    module_t *found_module = NULL;
    for (uint8_t i = 0; i < registered_modules_count; i++) {
        if (registered_modules[i] && registered_modules[i]->name[0] != '\0' && strcmp(registered_modules[i]->name, name) == 0) {
            found_module = registered_modules[i];
            break;
        }
    }
    
    xSemaphoreGive(registry_mutex);
    return found_module;
}

esp_err_t fmw_module_registry_get_all(const module_t ***modules, uint8_t *count) {
    if (!modules || !count) {
        return ESP_ERR_INVALID_ARG;
    }
    *modules = (const module_t **)registered_modules;
    *count = registered_modules_count;
    return ESP_OK;
}

// =========================================================================
//                      Internal Functions
// =========================================================================

/**
 * @internal
 * @brief არეგისტრირებს ერთ მოდულს შიდა რეესტრში.
 * @details ეს ფუნქცია ამატებს მოდულს `registered_modules` მასივში.
 *          ოპერაცია დაცულია mutex-ით, რათა უზრუნველყოს thread-safety.
 *          თუ რეგისტრაცია ვერ ხერხდება (მაგ. მასივი სავსეა), ის ასუფთავებს
 *          ახლად შექმნილი მოდულისთვის გამოყოფილ მეხსიერებას.
 * @param[in] module დასარეგისტრირებელი მოდულის მაჩვენებელი.
 * @return esp_err_t ოპერაციის სტატუსი.
 * @retval ESP_OK თუ მოდული წარმატებით დარეგისტრირდა.
 * @retval ESP_ERR_INVALID_ARG თუ `module` არის NULL.
 * @retval ESP_ERR_NO_MEM თუ მოდულების მაქსიმალური ლიმიტი მიღწეულია.
 * @retval ESP_ERR_TIMEOUT თუ mutex-ის დაკავება ვერ მოხერხდა.
 */
static esp_err_t register_module(module_t *module) {
    if (!module) return ESP_ERR_INVALID_ARG;

    if (xSemaphoreTake(registry_mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to take mutex for module registration.");
        if (module->private_data) free(module->private_data);
        if (module->current_config) cJSON_Delete(module->current_config);
        free(module);
        return ESP_ERR_TIMEOUT;
    }

    if (registered_modules_count >= CONFIG_FMW_MAX_MODULES) {
        ESP_LOGE(TAG, "Cannot register module '%s', max module count (%d) reached!",
                 module->name, CONFIG_FMW_MAX_MODULES);
        xSemaphoreGive(registry_mutex);
        if (module->private_data) free(module->private_data);
        if (module->current_config) cJSON_Delete(module->current_config);
        free(module);
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(TAG, "Registering module instance: '%s'", module->name);
    registered_modules[registered_modules_count++] = module;
    
    xSemaphoreGive(registry_mutex);
    return ESP_OK;
}