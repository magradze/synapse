/**
 * @file config_manager.c
 * @brief კონფიგურაციის მენეჯერის იმპლემენტაცია.
 * @version 2.1
 * @date 2025-06-26
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს კონფიგურაციის მენეჯერის იმპლემენტაციას, რომელიც
 *          მართავს `system_config.json`-ის ჩატვირთვას, შენახვასა და მასზე
 *          წვდომას მეხსიერებიდან (RAM) ოპტიმიზებული პერფორმანსისთვის.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "config_manager.h" // Public API ჰედერი
#include "logging.h"        // ლოგირების API
#include "nvs_flash.h"      // NVS დრაივერის API
#include "nvs.h"            // NVS დრაივერის API
#include <string.h>         // strcmp, strncpy და ა.შ.
#include <stdlib.h>         // malloc, free

DEFINE_COMPONENT_TAG("CONFIG_MANAGER");

/**
 * @internal
 * @brief NVS namespace-ი და გასაღები, რომელიც გამოიყენება კონფიგურაციის შესანახად.
 */
static const char* NVS_NAMESPACE = "synapse_cfg";
static const char* NVS_CONFIG_KEY = "system_json";

/**
 * @internal
 * @brief კომპონენტის შიდა ცვლადები, რომლებიც ინახავს კონფიგურაციას და მართავს წვდომას.
 */
static cJSON* config_root_node = NULL;
static SemaphoreHandle_t config_mutex = NULL;

// --- შიდა ფუნქციების წინასწარი დეკლარაცია ---
static esp_err_t load_config_from_nvs(void);
static esp_err_t load_config_from_defaults(void);
esp_err_t fmw_config_save(void); // წინასწარი დეკლარაცია fmw_config_manager_init-ში გამოყენებისთვის

// =========================================================================
//                      Public API Implementation
// =========================================================================

esp_err_t fmw_config_manager_init(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS partition corrupted or version mismatch. Erasing...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    config_mutex = xSemaphoreCreateMutex();
    if (!config_mutex) {
        ESP_LOGE(TAG, "Failed to create config mutex!");
        return ESP_ERR_NO_MEM;
    }

    if (load_config_from_nvs() != ESP_OK) {
        ESP_LOGW(TAG, "No config in NVS. Loading from default system_config.json");
        err = load_config_from_defaults();
        if (err == ESP_OK) {
           fmw_config_save();
        }
    }
    
    if (config_root_node == NULL) {
         ESP_LOGE(TAG, "FATAL: Failed to load any configuration!");
         return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Config Manager initialized successfully.");
    return ESP_OK;
}

esp_err_t fmw_config_save(void) {
    if (xSemaphoreTake(config_mutex, portMAX_DELAY) != pdTRUE) return ESP_ERR_TIMEOUT;
    
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS for saving!");
        xSemaphoreGive(config_mutex);
        return err;
    }

    char* json_string = cJSON_PrintUnformatted(config_root_node);
    if (!json_string) {
        ESP_LOGE(TAG, "Failed to print cJSON to string!");
        nvs_close(nvs_handle);
        xSemaphoreGive(config_mutex);
        return ESP_ERR_NO_MEM;
    }

    err = nvs_set_str(nvs_handle, NVS_CONFIG_KEY, json_string);
    if (err == ESP_OK) {
        err = nvs_commit(nvs_handle);
    }

    free(json_string);
    nvs_close(nvs_handle);
    xSemaphoreGive(config_mutex);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to save config to NVS: %s", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Configuration saved to NVS successfully.");
    }
    return err;
}

const cJSON* fmw_config_get_module_config(const char *module_name) {
    if (!config_root_node || !module_name) return NULL;
    
    if (xSemaphoreTake(config_mutex, portMAX_DELAY) != pdTRUE) return NULL;

    const cJSON *modules_array = cJSON_GetObjectItem(config_root_node, "modules");
    const cJSON *result = NULL;

    if (cJSON_IsArray(modules_array)) {
        const cJSON* module_item = NULL;
        cJSON_ArrayForEach(module_item, modules_array) {
            const cJSON* config = cJSON_GetObjectItem(module_item, "config");
            if (!config) continue;
            const cJSON* instance_name = cJSON_GetObjectItem(config, "instance_name");
            if (cJSON_IsString(instance_name) && strcmp(instance_name->valuestring, module_name) == 0) {
                result = config;
                break;
            }
        }
    }
    xSemaphoreGive(config_mutex);
    return result;
}

const cJSON* fmw_config_get_global_config(void) {
    if (!config_root_node) return NULL;
    return cJSON_GetObjectItem(config_root_node, "global_config");
}

const cJSON* fmw_config_get_root(void) {
    return config_root_node;
}

// --- დამხმარე (Helper) ფუნქციების იმპლემენტაცია ---

esp_err_t fmw_config_get_module_string(const char *module_name, const char *key, char *buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0 || !key) return ESP_ERR_INVALID_ARG;

    const cJSON *module_config = fmw_config_get_module_config(module_name);
    if (!module_config) return ESP_ERR_NOT_FOUND;
    
    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE) {
        const cJSON *item = cJSON_GetObjectItem(module_config, key);
        if (cJSON_IsString(item)) {
            strncpy(buffer, item->valuestring, buffer_size - 1);
            buffer[buffer_size - 1] = '\0';
            ret = ESP_OK;
        } else {
            ret = ESP_ERR_NVS_TYPE_MISMATCH;
        }
        xSemaphoreGive(config_mutex);
    }
    return ret;
}

esp_err_t fmw_config_get_module_int(const char *module_name, const char *key, int *out_value) {
    if (!out_value || !key) return ESP_ERR_INVALID_ARG;

    const cJSON *module_config = fmw_config_get_module_config(module_name);
    if (!module_config) return ESP_ERR_NOT_FOUND;

    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE) {
        const cJSON *item = cJSON_GetObjectItem(module_config, key);
        if (cJSON_IsNumber(item)) {
            *out_value = item->valueint;
            ret = ESP_OK;
        } else {
            ret = ESP_ERR_NVS_TYPE_MISMATCH;
        }
        xSemaphoreGive(config_mutex);
    }
    return ret;
}

esp_err_t fmw_config_get_module_bool(const char *module_name, const char *key, bool *out_value) {
    if (!out_value || !key) return ESP_ERR_INVALID_ARG;

    const cJSON *module_config = fmw_config_get_module_config(module_name);
    if (!module_config) return ESP_ERR_NOT_FOUND;

    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE) {
        const cJSON *item = cJSON_GetObjectItem(module_config, key);
        if (cJSON_IsBool(item)) {
            *out_value = cJSON_IsTrue(item);
            ret = ESP_OK;
        } else {
            ret = ESP_ERR_NVS_TYPE_MISMATCH;
        }
        xSemaphoreGive(config_mutex);
    }
    return ret;
}

// =========================================================================
//                      Internal Functions
// =========================================================================

/**
 * @internal
 * @brief ტვირთავს კონფიგურაციას NVS მეხსიერებიდან.
 */
static esp_err_t load_config_from_nvs(void) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) return err;

    size_t required_size = 0;
    err = nvs_get_str(nvs_handle, NVS_CONFIG_KEY, NULL, &required_size);
    if (err != ESP_OK || required_size <= 1) {
        nvs_close(nvs_handle);
        return ESP_ERR_NVS_NOT_FOUND;
    }
    
    char* json_string = malloc(required_size);
    if (!json_string) {
        ESP_LOGE(TAG, "Failed to allocate memory for config string!");
        nvs_close(nvs_handle);
        return ESP_ERR_NO_MEM;
    }

    err = nvs_get_str(nvs_handle, NVS_CONFIG_KEY, json_string, &required_size);
    nvs_close(nvs_handle);

    if (err == ESP_OK) {
        if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE) {
            if (config_root_node) cJSON_Delete(config_root_node);
            config_root_node = cJSON_Parse(json_string);
            xSemaphoreGive(config_mutex);
            if (config_root_node == NULL) {
                ESP_LOGE(TAG, "Failed to parse JSON from NVS!");
                err = ESP_FAIL;
            } else {
                 ESP_LOGI(TAG, "Configuration loaded from NVS.");
            }
        }
    }
    free(json_string);
    return err;
}

/**
 * @internal
 * @brief ტვირთავს default კონფიგურაციას firmware-ში ჩაშენებული `system_config.json`-დან.
 */
static esp_err_t load_config_from_defaults(void) {
    extern const uint8_t _binary_system_config_json_start[] asm("_binary_system_config_json_start");
    extern const uint8_t _binary_system_config_json_end[] asm("_binary_system_config_json_end");
    
    if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE) {
        if (config_root_node) cJSON_Delete(config_root_node);
        config_root_node = cJSON_ParseWithLength((const char*)_binary_system_config_json_start, _binary_system_config_json_end - _binary_system_config_json_start);
        xSemaphoreGive(config_mutex);
        if (config_root_node == NULL) {
            ESP_LOGE(TAG, "Failed to parse embedded system_config.json!");
            return ESP_FAIL;
        }
    }
    return ESP_OK;
}