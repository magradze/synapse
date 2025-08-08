/**
 * @file module_registry.c
 * @brief მოდულების რეგისტრაციისა და მართვის იმპლემენტაცია.
 * @version 2.2
 * @date 2025-06-27
 * @author Giorgi Magradze
 * @details ეს ფაილი პასუხისმგებელია სისტემის ინიციალიზაციისას `system_config.json`-ის
 *          წაკითხვაზე, Module Factory-ის გამოყენებით მოდულების დინამიურ შექმნასა
 *          და მათ ცენტრალიზებულ რეესტრში რეგისტრაციაზე.
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
static module_t *registered_modules[CONFIG_SYNAPSE_MAX_MODULES];
static uint8_t registered_modules_count = 0;
static SemaphoreHandle_t registry_mutex = NULL;

// --- შიდა ფუნქციების წინასწარი დეკლარაცია ---
static esp_err_t register_module(module_t *module);

/**
 * @internal
 * @brief qsort-ისთვის განკუთვნილი შედარების ფუნქცია.
 * @details ადარებს ორ მოდულს მათი init_level ველის მიხედვით.
 *          არგუმენტები a და b არიან მაჩვენებლები module_t*-ზე (ანუ module_t**).
 */
static int compare_modules_by_init_level(const void *a, const void *b)
{
    // 1. არგუმენტების სწორი ტიპზე მიყვანა: void* -> module_t**
    // 2. Dereference (*), რომ მივიღოთ module_t*
    const module_t *mod_a = *(const module_t **)a;
    const module_t *mod_b = *(const module_t **)b;

    // შედარება
    if (mod_a->init_level < mod_b->init_level)
    {
        return -1; // a უნდა იყოს b-ზე წინ
    }
    if (mod_a->init_level > mod_b->init_level)
    {
        return 1; // b უნდა იყოს a-ზე წინ
    }
    return 0; // თანმიმდევრობას მნიშვნელობა არ აქვს
}

// =========================================================================
//                      Public API Implementation
// =========================================================================

esp_err_t synapse_module_registry_init(void)
{
    ESP_LOGI(TAG, "--- Module Registry Initialization ---");

    registry_mutex = xSemaphoreCreateMutex();
    if (!registry_mutex) {
        ESP_LOGE(TAG, "Failed to create registry mutex!");
        return ESP_ERR_NO_MEM;
    }

    // Step 1: გავასუფთავოთ რეესტრის მდგომარეობა
    memset(registered_modules, 0, sizeof(registered_modules));
    registered_modules_count = 0;

    // Step 2: მივიღოთ კონფიგურაციის root ობიექტი
    const cJSON *root = synapse_config_get_root();
    if (!root) {
        ESP_LOGE(TAG, "Failed to get configuration root. Cannot initialize modules.");
        // Mutex-ის განადგურება რესურსის გაჟონვის თავიდან ასაცილებლად
        vSemaphoreDelete(registry_mutex);
        registry_mutex = NULL;
        return ESP_FAIL;
    }

    const cJSON *modules_array = cJSON_GetObjectItem(root, "modules");
    if (!cJSON_IsArray(modules_array)) {
        ESP_LOGW(TAG, "'modules' key not found or not an array in config. No modules will be loaded.");
        return ESP_OK; // ეს არ არის კრიტიკული შეცდომა
    }

    ESP_LOGD(TAG, "Found %d module configurations to process.", cJSON_GetArraySize(modules_array));

    // Step 3: გავიაროთ კონფიგურაცია და შევქმნათ მოდულების ინსტანციები
    cJSON *module_config_json;
    cJSON_ArrayForEach(module_config_json, modules_array)
    {
        const cJSON *type_json = cJSON_GetObjectItem(module_config_json, "type");
        const cJSON *enabled_json = cJSON_GetObjectItem(module_config_json, "enabled");

        if (!cJSON_IsString(type_json) || !type_json->valuestring) {
            ESP_LOGW(TAG, "Skipping module configuration with missing or invalid 'type' field.");
            continue;
        }

        // `enabled` ველი თუ არ არსებობს, ჩავთვალოთ რომ ჩართულია (default true)
        if (enabled_json && cJSON_IsBool(enabled_json) && cJSON_IsFalse(enabled_json))
        {
            ESP_LOGI(TAG, "Module type '%s' is disabled in config, skipping.", type_json->valuestring);
            continue;
        }

        module_t *new_module = synapse_module_factory_create(type_json->valuestring, module_config_json);

        if (new_module) {
            if (register_module(new_module) != ESP_OK)
            {
                // register_module ფუნქცია თავად ასუფთავებს მოდულს შეცდომისას,
                // ამიტომ აქ დამატებითი მოქმედება არ არის საჭირო.
                ESP_LOGE(TAG, "Failed to register created module of type '%s'.", type_json->valuestring);
            }
        }
        else
        {
            ESP_LOGE(TAG, "Failed to create module of type '%s'.", type_json->valuestring);
        }
    }

    if (registered_modules_count > 1)
    {
        ESP_LOGI(TAG, "Sorting %d modules by init_level...", registered_modules_count);
        qsort(registered_modules, registered_modules_count, sizeof(module_t *), compare_modules_by_init_level);
    }

    ESP_LOGI(TAG, "--- Module Registry Initialization Complete: %d modules registered ---", registered_modules_count);
    return ESP_OK;
}

module_t *synapse_module_registry_find_by_name(const char *name)
{
    if (!name)
    {
        return NULL;
    }

    if (!registry_mutex)
    {
        ESP_LOGE(TAG, "Registry not initialized, cannot find module.");
        return NULL;
    }

    if (xSemaphoreTake(registry_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_MUTEX_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Failed to take mutex for find_by_name.");
        return NULL;
    }

    module_t *found_module = NULL;
    for (uint8_t i = 0; i < registered_modules_count; i++) {
        // ⭐️ შესწორებულია: მოშორებულია ზედმეტი `&& registered_modules[i]->name` შემოწმება
        if (registered_modules[i] && strcmp(registered_modules[i]->name, name) == 0)
        {
            found_module = registered_modules[i];
            break;
        }
    }
    
    xSemaphoreGive(registry_mutex);
    return found_module;
}

esp_err_t synapse_module_registry_get_all(const module_t ***modules, uint8_t *count)
{
    if (!modules || !count) {
        return ESP_ERR_INVALID_ARG;
    }

    // ეს ოპერაცია არის read-only, ამიტომ mutex-ის აღება არ არის კრიტიკული,
    // თუ ვივარაუდებთ, რომ მოდულები runtime-ში არ ემატება/იშლება.
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
 */
static esp_err_t register_module(module_t *module) {
    if (!module)
    {
        return ESP_ERR_INVALID_ARG;
    }

    if (xSemaphoreTake(registry_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_MUTEX_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Failed to take mutex for module registration of '%s'.", module->name);
        // თუ რეგისტრაციისას mutex-ს ვერ ვიღებთ, მოდული უნდა განადგურდეს, რათა არ დაიკარგოს მეხსიერება
        if (module->base.deinit)
        {
            module->base.deinit(module);
        }
        return ESP_ERR_TIMEOUT;
    }

    if (registered_modules_count >= CONFIG_SYNAPSE_MAX_MODULES)
    {
        ESP_LOGE(TAG, "Cannot register module '%s', max module count (%d) reached!",
                 module->name, CONFIG_SYNAPSE_MAX_MODULES);
        xSemaphoreGive(registry_mutex);

        // თუ რეგისტრაციისას ადგილი არ არის, მოდული უნდა განადგურდეს
        if (module->base.deinit)
        {
            module->base.deinit(module);
        }
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(TAG, "Registering module instance: '%s'", module->name);
    registered_modules[registered_modules_count++] = module;
    
    xSemaphoreGive(registry_mutex);
    return ESP_OK;
}