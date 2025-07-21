/**
 * @file config_manager.c
 * @brief Implementation of the Configuration Manager.
 * @author Giorgi Magradze (Refactored by AI)
 * @version 4.0.1
 * @date 2025-06-29
 *
 * @details
 * This file implements the Configuration Manager for the Synapse Framework.
 * This version utilizes a fully decentralized storage strategy in NVS to
 * overcome the 4KB value size limit.
 *
 * (The rest of the file header remains the same)
 */

#include "config_manager.h"
#include "logging.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <string.h>
#include <stdlib.h>

#include "embedded_configs.h"

DEFINE_COMPONENT_TAG("CONFIG_MANAGER");

static const char *NVS_NAMESPACE = "synapse_cfg";
static const char *NVS_GLOBAL_CONFIG_KEY = "global_config";

static cJSON *config_root_node = NULL;
static SemaphoreHandle_t config_mutex = NULL;

// --- Internal Function Prototypes ---
static esp_err_t load_config_from_nvs(void);
static esp_err_t load_config_from_defaults(void);
static esp_err_t save_config_to_nvs_decentralized(void);
static const cJSON *find_module_config_by_name(const char *module_name);
static const cJSON *get_node_by_key(const char *key);

// =========================================================================
//                      Public API Implementation
// =========================================================================

esp_err_t fmw_config_manager_init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_LOGW(TAG, "NVS partition corrupted or version mismatch. Erasing...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    config_mutex = xSemaphoreCreateMutex();
    if (!config_mutex)
    {
        ESP_LOGE(TAG, "Failed to create config mutex!");
        return ESP_ERR_NO_MEM;
    }

    if (load_config_from_nvs() != ESP_OK)
    {
        ESP_LOGI(TAG, "Config not found in NVS. Assembling from embedded defaults.");
        err = load_config_from_defaults();
        if (err == ESP_OK)
        {
            save_config_to_nvs_decentralized();
        }
    }

    if (config_root_node == NULL)
    {
        ESP_LOGE(TAG, "CRITICAL: Failed to load any configuration!");
        vSemaphoreDelete(config_mutex);
        config_mutex = NULL;
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Config Manager initialized successfully.");
    return ESP_OK;
}

esp_err_t fmw_config_save(void)
{
    return save_config_to_nvs_decentralized();
}

esp_err_t fmw_config_get_string(const char *key, char *buffer, size_t buffer_size)
{
    if (!key || !buffer || buffer_size == 0)
        return ESP_ERR_INVALID_ARG;
    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE)
    {
        const cJSON *item = get_node_by_key(key);
        if (cJSON_IsString(item))
        {
            strncpy(buffer, item->valuestring, buffer_size - 1);
            buffer[buffer_size - 1] = '\0';
            ret = ESP_OK;
        }
        else
        {
            ret = item ? ESP_ERR_NVS_TYPE_MISMATCH : ESP_ERR_NOT_FOUND;
        }
        xSemaphoreGive(config_mutex);
    }
    else
    {
        ret = ESP_ERR_TIMEOUT;
    }
    return ret;
}

esp_err_t fmw_config_get_int(const char *key, int *out_value)
{
    if (!key || !out_value)
        return ESP_ERR_INVALID_ARG;
    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE)
    {
        const cJSON *item = get_node_by_key(key);
        if (cJSON_IsNumber(item))
        {
            *out_value = item->valueint;
            ret = ESP_OK;
        }
        else
        {
            ret = item ? ESP_ERR_NVS_TYPE_MISMATCH : ESP_ERR_NOT_FOUND;
        }
        xSemaphoreGive(config_mutex);
    }
    else
    {
        ret = ESP_ERR_TIMEOUT;
    }
    return ret;
}

esp_err_t fmw_config_get_bool(const char *key, bool *out_value)
{
    if (!key || !out_value)
        return ESP_ERR_INVALID_ARG;
    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE)
    {
        const cJSON *item = get_node_by_key(key);
        if (cJSON_IsBool(item))
        {
            *out_value = cJSON_IsTrue(item);
            ret = ESP_OK;
        }
        else
        {
            ret = item ? ESP_ERR_NVS_TYPE_MISMATCH : ESP_ERR_NOT_FOUND;
        }
        xSemaphoreGive(config_mutex);
    }
    else
    {
        ret = ESP_ERR_TIMEOUT;
    }
    return ret;
}

const cJSON *fmw_config_get_root(void)
{
    return config_root_node;
}

// =========================================================================
//                      Internal Functions
// =========================================================================

static esp_err_t save_config_to_nvs_decentralized(void)
{
    if (xSemaphoreTake(config_mutex, portMAX_DELAY) != pdTRUE)
        return ESP_ERR_TIMEOUT;

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open NVS for writing!");
        xSemaphoreGive(config_mutex);
        return err;
    }

    nvs_erase_all(nvs_handle);

    cJSON *global_config = cJSON_GetObjectItem(config_root_node, "global_config");
    if (global_config)
    {
        char *global_string = cJSON_PrintUnformatted(global_config);
        if (global_string)
        {
            nvs_set_str(nvs_handle, NVS_GLOBAL_CONFIG_KEY, global_string);
            free(global_string);
        }
    }

    cJSON *modules_array = cJSON_GetObjectItem(config_root_node, "modules");
    if (cJSON_IsArray(modules_array))
    {
        cJSON *module_item;
        cJSON_ArrayForEach(module_item, modules_array)
        {
            cJSON *config = cJSON_GetObjectItem(module_item, "config");
            cJSON *instance_name_json = cJSON_GetObjectItem(config, "instance_name");
            if (cJSON_IsString(instance_name_json))
            {
                char *module_string = cJSON_PrintUnformatted(module_item);
                if (module_string)
                {
                    nvs_set_str(nvs_handle, instance_name_json->valuestring, module_string);
                    free(module_string);
                }
            }
        }
    }

    err = nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
    xSemaphoreGive(config_mutex);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to save decentralized config to NVS: %s", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGI(TAG, "Decentralized config saved to NVS successfully.");
    }
    return err;
}

/******************************************************************************
 * @brief Loads configuration from NVS by iterating and assembling decentralized entries.
 *
 * @details This function has been updated to use the ESP-IDF v5.x NVS iterator API.
 *          It finds all string entries in the component's namespace and assembles
 *          them into a single cJSON object in RAM.
 *
 * @return ESP_OK on success, or an error code on failure.
 *****************************************************************************/
static esp_err_t load_config_from_nvs(void)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK)
        return err;

    cJSON *root = cJSON_CreateObject();
    cJSON *modules_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "modules", modules_array);

    // Load global_config first
    size_t required_size = 0;
    err = nvs_get_str(nvs_handle, NVS_GLOBAL_CONFIG_KEY, NULL, &required_size);
    if (err == ESP_OK && required_size > 0)
    {
        char *global_string = malloc(required_size);
        nvs_get_str(nvs_handle, NVS_GLOBAL_CONFIG_KEY, global_string, &required_size);
        cJSON *global_json = cJSON_Parse(global_string);
        if (global_json)
        {
            cJSON_AddItemToObject(root, "global_config", global_json);
        }
        free(global_string);
    }
    else
    {
        cJSON_AddItemToObject(root, "global_config", cJSON_CreateObject());
    }

    // *** START OF CORRECTED NVS ITERATOR LOGIC ***
    nvs_iterator_t it = NULL;
    esp_err_t find_err = nvs_entry_find(NVS_DEFAULT_PART_NAME, NVS_NAMESPACE, NVS_TYPE_STR, &it);

    if (find_err != ESP_OK)
    {
        ESP_LOGD(TAG, "No string entries found in NVS namespace or error occurred: %s", esp_err_to_name(find_err));
        nvs_close(nvs_handle);
        cJSON_Delete(root);
        return ESP_ERR_NOT_FOUND;
    }

    bool found_modules = false;
    while (it != NULL)
    {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);

        if (strcmp(info.key, NVS_GLOBAL_CONFIG_KEY) != 0)
        {
            required_size = 0;
            nvs_get_str(nvs_handle, info.key, NULL, &required_size);
            if (required_size > 0)
            {
                char *module_string = malloc(required_size);
                if (module_string)
                {
                    nvs_get_str(nvs_handle, info.key, module_string, &required_size);
                    cJSON *module_json = cJSON_Parse(module_string);
                    if (module_json)
                    {
                        cJSON_AddItemToArray(modules_array, module_json);
                        found_modules = true;
                    }
                    free(module_string);
                }
            }
        }
        // Move to the next entry
        esp_err_t next_err = nvs_entry_next(&it);
        if (next_err != ESP_OK)
        {
            break; // No more entries
        }
    }
    // *** END OF CORRECTED NVS ITERATOR LOGIC ***

    nvs_release_iterator(it);
    nvs_close(nvs_handle);

    if (!found_modules)
    {
        ESP_LOGW(TAG, "No module configurations found in NVS namespace.");
        cJSON_Delete(root);
        return ESP_ERR_NOT_FOUND;
    }

    if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE)
    {
        if (config_root_node)
        {
            cJSON_Delete(config_root_node);
        }
        config_root_node = root;
        xSemaphoreGive(config_mutex);
        ESP_LOGI(TAG, "Configuration assembled successfully from NVS.");
        return ESP_OK;
    }
    else
    {
        cJSON_Delete(root);
        return ESP_ERR_TIMEOUT;
    }
}

/**
 * @brief Assembles the default configuration from embedded files.
 *
 * @details This function first loads the base `system_config.json`. Then, it
 *          iterates through the auto-generated `embedded_module_configs` manifest.
 *          For each module's `config.json`, it checks if the content is a JSON
 *          object or a JSON array.
 *          - If it's an object, it's added directly to the main `modules` array.
 *          - If it's an array, each element of that array is added individually
 *            to the main `modules` array.
 *
 * @return ESP_OK on success, or an error code on failure.
 **/
static esp_err_t load_config_from_defaults(void)
{
    extern const uint8_t _binary_system_config_json_start[] asm("_binary_system_config_json_start");
    extern const uint8_t _binary_system_config_json_end[] asm("_binary_system_config_json_end");

    size_t base_json_size = _binary_system_config_json_end - _binary_system_config_json_start;
    cJSON *root = cJSON_ParseWithLength((const char *)_binary_system_config_json_start, base_json_size);

    if (!root)
    {
        ESP_LOGE(TAG, "Failed to parse embedded base system_config.json!");
        return ESP_FAIL;
    }

    cJSON *modules_array = cJSON_GetObjectItem(root, "modules");
    if (!modules_array)
    {
        modules_array = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "modules", modules_array);
    }
    else if (!cJSON_IsArray(modules_array))
    {
        ESP_LOGE(TAG, "'modules' key exists but is not an array.");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    int modules_added = 0;
    for (int i = 0; embedded_module_configs[i].module_name != NULL; i++)
    {
        const embedded_config_t *config_entry = &embedded_module_configs[i];
        size_t module_config_size = get_embedded_config_size(config_entry);

        if (module_config_size == 0)
            continue;

        cJSON *parsed_json = cJSON_ParseWithLength((const char *)config_entry->start, module_config_size);
        if (!parsed_json)
        {
            ESP_LOGE(TAG, "Failed to parse config for module '%s'. Skipping.", config_entry->module_name);
            ESP_LOGE(TAG, "JSON parse error near: %s", cJSON_GetErrorPtr());
            continue;
        }

        // *** START OF NEW LOGIC TO HANDLE ARRAYS ***
        if (cJSON_IsArray(parsed_json))
        {
            ESP_LOGD(TAG, "Config for '%s' is an array. Adding elements individually.", config_entry->module_name);
            cJSON *element = NULL;
            while ((element = cJSON_DetachItemFromArray(parsed_json, 0)) != NULL)
            {
                cJSON_AddItemToArray(modules_array, element);
                modules_added++;
            }
            cJSON_Delete(parsed_json); // Delete the now-empty array
        }
        else if (cJSON_IsObject(parsed_json))
        {
            ESP_LOGD(TAG, "Config for '%s' is an object. Adding directly.", config_entry->module_name);
            cJSON_AddItemToArray(modules_array, parsed_json);
            modules_added++;
        }
        else
        {
            ESP_LOGW(TAG, "Config for '%s' is not a valid JSON object or array. Skipping.", config_entry->module_name);
            cJSON_Delete(parsed_json);
        }
        // *** END OF NEW LOGIC ***
    }

    ESP_LOGI(TAG, "Assembled default configuration with %d modules.", modules_added);

    if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE)
    {
        if (config_root_node)
        {
            cJSON_Delete(config_root_node);
        }
        config_root_node = root;
        xSemaphoreGive(config_mutex);
    }
    else
    {
        cJSON_Delete(root);
        return ESP_ERR_TIMEOUT;
    }

    return ESP_OK;
}

static const cJSON *find_module_config_by_name(const char *module_name)
{
    if (!config_root_node || !module_name)
        return NULL;
    const cJSON *modules_array = cJSON_GetObjectItem(config_root_node, "modules");
    if (!cJSON_IsArray(modules_array))
        return NULL;

    const cJSON *module_item = NULL;
    cJSON_ArrayForEach(module_item, modules_array)
    {
        const cJSON *config = cJSON_GetObjectItem(module_item, "config");
        if (!config)
            continue;
        const cJSON *instance_name = cJSON_GetObjectItem(config, "instance_name");
        if (cJSON_IsString(instance_name) && (strcmp(instance_name->valuestring, module_name) == 0))
        {
            return config;
        }
    }
    return NULL;
}

static const cJSON *get_node_by_key(const char *key)
{
    if (!config_root_node || !key)
        return NULL;

    char *key_copy = strdup(key);
    if (!key_copy)
        return NULL;

    const cJSON *current_node = NULL;
    char *token = strtok(key_copy, ".");
    if (!token)
    {
        free(key_copy);
        return NULL;
    }

    if (strcmp(token, "global_config") == 0)
    {
        current_node = cJSON_GetObjectItem(config_root_node, "global_config");
    }
    else
    {
        current_node = find_module_config_by_name(token);
    }

    while (current_node && (token = strtok(NULL, ".")))
    {
        current_node = cJSON_GetObjectItem(current_node, token);
    }

    free(key_copy);
    return current_node;
}