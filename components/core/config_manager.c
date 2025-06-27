/**
 * @file config_manager.c
 * @brief კონფიგურაციის მენეჯერის იმპლემენტაცია.
 * @version 3.0
 * @date 2025-06-27
 * @author Giorgi Magradze (Refactored by AI)
 * @details ეს ფაილი შეიცავს კონფიგურაციის მენეჯერის იმპლემენტაციას, რომელიც
 *          მართავს `system_config.json`-ის ჩატვირთვას, შენახვასა და მასზე
 *          წვდომას მეხსიერებიდან (RAM) ოპტიმიზებული პერფორმანსისთვის.
 *          რეფაქტორინგის შედეგად გაუმჯობესდა ნაკად-უსაფრთხოება, დაემატა
 *          წერტილით გამოყოფილი გასაღებით წვდომის API და აღმოიფხვრა
 *          მეხსიერების გაჟონვის პოტენციური რისკები.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "config_manager.h" // Public API ჰედერი
#include "logging.h"        // ლოგირების API
#include "nvs_flash.h"      // NVS დრაივერის API
#include "nvs.h"            // NVS დრაივერის API
#include <string.h>         // strcmp, strncpy, strdup, strtok
#include <stdlib.h>         // malloc, free

DEFINE_COMPONENT_TAG("CONFIG_MANAGER");

/**
 * @internal
 * @brief NVS namespace-ი და გასაღები, რომელიც გამოიყენება კონფიგურაციის შესანახად.
 */
static const char *NVS_NAMESPACE = "synapse_cfg";
static const char *NVS_CONFIG_KEY = "system_json";

/**
 * @internal
 * @brief კომპონენტის შიდა ცვლადები, რომლებიც ინახავს კონფიგურაციას და მართავს წვდომას.
 */
static cJSON *config_root_node = NULL;
static SemaphoreHandle_t config_mutex = NULL;

// --- შიდა ფუნქციების წინასწარი დეკლარაცია ---
static esp_err_t load_config_from_nvs(void);
static esp_err_t load_config_from_defaults(void);
static const cJSON *find_module_config_by_name(const char *module_name);
static const cJSON *get_node_by_key(const char *key);
esp_err_t fmw_config_save(void); // წინასწარი დეკლარაცია fmw_config_manager_init-ში გამოყენებისთვის

// =========================================================================
//                      Public API Implementation
// =========================================================================

esp_err_t fmw_config_manager_init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_LOGW(TAG, "NVS პარტიშენი დაზიანებულია ან ვერსია არ ემთხვევა. მიმდინარეობს წაშლა...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    config_mutex = xSemaphoreCreateMutex();
    if (!config_mutex)
    {
        ESP_LOGE(TAG, "კონფიგურაციის mutex-ის შექმნა ვერ მოხერხდა!");
        return ESP_ERR_NO_MEM;
    }

    if (load_config_from_nvs() != ESP_OK)
    {
        ESP_LOGW(TAG, "NVS-ში კონფიგურაცია ვერ მოიძებნა. იტვირთება საწყისი system_config.json");
        err = load_config_from_defaults();
        if (err == ESP_OK)
        {
            // პირველადი ჩატვირთვისას შევინახოთ NVS-ში
            fmw_config_save();
        }
    }

    if (config_root_node == NULL)
    {
        ESP_LOGE(TAG, "კრიტიკული შეცდომა: ვერცერთი კონფიგურაციის ჩატვირთვა ვერ მოხერხდა!");
        vSemaphoreDelete(config_mutex);
        config_mutex = NULL;
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "კონფიგურაციის მენეჯერი წარმატებით ინიციალიზდა.");
    return ESP_OK;
}

esp_err_t fmw_config_save(void)
{
    if (xSemaphoreTake(config_mutex, portMAX_DELAY) != pdTRUE)
        return ESP_ERR_TIMEOUT;

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "NVS-ის გახსნა შენახვისთვის ვერ მოხერხდა!");
        xSemaphoreGive(config_mutex);
        return err;
    }

    char *json_string = cJSON_PrintUnformatted(config_root_node);
    if (!json_string)
    {
        ESP_LOGE(TAG, "cJSON ობიექტის სტრიქონში გადაყვანა ვერ მოხერხდა!");
        nvs_close(nvs_handle);
        xSemaphoreGive(config_mutex);
        return ESP_ERR_NO_MEM;
    }

    err = nvs_set_str(nvs_handle, NVS_CONFIG_KEY, json_string);
    if (err == ESP_OK)
    {
        err = nvs_commit(nvs_handle);
    }

    free(json_string);
    nvs_close(nvs_handle);
    xSemaphoreGive(config_mutex);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "კონფიგურაციის NVS-ში შენახვა ვერ მოხერხდა: %s", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGI(TAG, "კონფიგურაცია NVS-ში წარმატებით შეინახა.");
    }
    return err;
}

esp_err_t fmw_config_get_string(const char *key, char *buffer, size_t buffer_size)
{
    if (!key || !buffer || buffer_size == 0)
    {
        return ESP_ERR_INVALID_ARG;
    }

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
    {
        return ESP_ERR_INVALID_ARG;
    }

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
    {
        return ESP_ERR_INVALID_ARG;
    }

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

const cJSON *fmw_config_get_global_config(void)
{
    if (!config_root_node)
        return NULL;
    // ეს ფუნქცია არ იღებს mutex-ს, რადგან ის იძახებს get_node_by_key-ს,
    // რომელიც თავის მხრივ არ არის ნაკად-უსაფრთხო.
    // გარე გამოყენებისთვის გამოიძახეთ fmw_config_get_* ფუნქციები.
    return cJSON_GetObjectItem(config_root_node, "global_config");
}

/**
 * @brief იღებს კონფიგურაციის მთავარ (root) cJSON კვანძზე const მაჩვენებელს.
 * @warning ეს ფუნქცია აბრუნებს მაჩვენებელს შიდა cJSON ობიექტზე. მიუხედავად იმისა,
 *          რომ მაჩვენებელი არის `const`, რაც კრძალავს პირდაპირ ცვლილებებს,
 *          მისი გამოყენება მაინც მოითხოვს სიფრთხილეს. თავი აარიდეთ მის გამოყენებას,
 *          თუ ეს აბსოლუტურად აუცილებელი არ არის. გამოიყენეთ `fmw_config_get_*`
 *          ფუნქციები კონკრეტული მნიშვნელობების უსაფრთხოდ მისაღებად.
 * @return const cJSON* მაჩვენებელი მთავარ კვანძზე, ან NULL თუ კონფიგურაცია არ არის ჩატვირთული.
 */
const cJSON *fmw_config_get_root(void)
{
    return config_root_node;
}

// =========================================================================
//                      Internal Functions
// =========================================================================

/**
 * @internal
 * @brief პოულობს მოდულის კონფიგურაციას მისი `instance_name`-ის მიხედვით.
 * @details ეს ფუნქცია არ არის ნაკად-უსაფრთხო და უნდა გამოიძახოს მხოლოდ mutex-ით დაცულ კონტექსტში.
 * @param module_name მოსაძებნი მოდულის სახელი.
 * @return const cJSON* მოდულის `config` ობიექტზე, ან NULL თუ ვერ მოიძებნა.
 */
static const cJSON *find_module_config_by_name(const char *module_name)
{
    if (!config_root_node || !module_name)
        return NULL;

    const cJSON *modules_array = cJSON_GetObjectItem(config_root_node, "modules");
    if (!cJSON_IsArray(modules_array))
    {
        ESP_LOGD(TAG, "კონფიგურაციაში 'modules' მასივი არ არის ან არასწორი ტიპისაა.");
        return NULL;
    }

    const cJSON *module_item = NULL;
    cJSON_ArrayForEach(module_item, modules_array)
    {
        const cJSON *config = cJSON_GetObjectItem(module_item, "config");
        if (!config)
            continue;

        const cJSON *instance_name = cJSON_GetObjectItem(config, "instance_name");
        if (cJSON_IsString(instance_name) && (strcmp(instance_name->valuestring, module_name) == 0))
        {
            return config; // ვიპოვეთ შესაბამისი მოდული
        }
    }

    return NULL; // მოდული ვერ მოიძებნა
}

/**
 * @internal
 * @brief პოულობს cJSON კვანძს წერტილით გამოყოფილი გასაღების მიხედვით.
 * @details ეს ფუნქცია არ არის ნაკად-უსაფრთხო და უნდა გამოიძახოს მხოლოდ mutex-ით დაცულ კონტექსტში.
 *          იგი არჩევს გასაღებს, მაგ: "module_name.key.subkey" ან "global_config.key".
 * @param key წერტილით გამოყოფილი გასაღები.
 * @return const cJSON* ნაპოვნი კვანძი ან NULL თუ ვერ მოიძებნა.
 */
static const cJSON *get_node_by_key(const char *key)
{
    if (!config_root_node || !key)
    {
        return NULL;
    }

    char *key_copy = strdup(key);
    if (!key_copy)
    {
        ESP_LOGE(TAG, "გასაღების დუბლირებისთვის მეხსიერება ვერ გამოიყო.");
        return NULL;
    }

    const cJSON *current_node = NULL;
    char *token = strtok(key_copy, ".");

    if (!token)
    {
        free(key_copy);
        return NULL;
    }

    // შევამოწმოთ, გლობალური კონფიგურაციაა თუ მოდულის
    if (strcmp(token, "global_config") == 0)
    {
        current_node = cJSON_GetObjectItem(config_root_node, "global_config");
    }
    else
    {
        // ვივარაუდოთ, რომ პირველი ტოკენი არის მოდულის instance_name
        current_node = find_module_config_by_name(token);
    }

    // დანარჩენი გასაღების დამუშავება
    while (current_node && (token = strtok(NULL, ".")))
    {
        current_node = cJSON_GetObjectItem(current_node, token);
    }

    free(key_copy);
    return current_node;
}

/**
 * @internal
 * @brief ტვირთავს კონფიგურაციას NVS მეხსიერებიდან.
 */
static esp_err_t load_config_from_nvs(void)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK)
        return err;

    size_t required_size = 0;
    err = nvs_get_str(nvs_handle, NVS_CONFIG_KEY, NULL, &required_size);
    if (err != ESP_OK || required_size <= 1)
    {
        nvs_close(nvs_handle);
        return ESP_ERR_NVS_NOT_FOUND;
    }

    char *json_string = malloc(required_size);
    if (!json_string)
    {
        ESP_LOGE(TAG, "კონფიგურაციის სტრიქონისთვის მეხსიერება ვერ გამოიყო!");
        nvs_close(nvs_handle);
        return ESP_ERR_NO_MEM;
    }

    err = nvs_get_str(nvs_handle, NVS_CONFIG_KEY, json_string, &required_size);
    nvs_close(nvs_handle);

    if (err == ESP_OK)
    {
        cJSON *temp_node = cJSON_Parse(json_string);
        if (temp_node)
        {
            if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE)
            {
                if (config_root_node)
                {
                    cJSON_Delete(config_root_node);
                }
                config_root_node = temp_node;
                xSemaphoreGive(config_mutex);
                ESP_LOGI(TAG, "კონფიგურაცია წარმატებით ჩაიტვირთა NVS-დან.");
            }
        }
        else
        {
            ESP_LOGE(TAG, "NVS-დან JSON-ის პარსირება ვერ მოხერხდა!");
            err = ESP_FAIL;
        }
    }
    free(json_string);
    return err;
}

/**
 * @internal
 * @brief ტვირთავს default კონფიგურაციას firmware-ში ჩაშენებული `system_config.json`-დან.
 */
static esp_err_t load_config_from_defaults(void)
{
    extern const uint8_t _binary_system_config_json_start[] asm("_binary_system_config_json_start");
    extern const uint8_t _binary_system_config_json_end[] asm("_binary_system_config_json_end");

    size_t json_size = _binary_system_config_json_end - _binary_system_config_json_start;
    ESP_LOGD(TAG, "ჩაშენებული JSON-ის ზომა: %zu ბაიტი", json_size);

    cJSON *temp_node = cJSON_ParseWithLength((const char *)_binary_system_config_json_start, json_size);

    if (temp_node)
    {
        if (xSemaphoreTake(config_mutex, portMAX_DELAY) == pdTRUE)
        {
            if (config_root_node)
            {
                cJSON_Delete(config_root_node);
            }
            config_root_node = temp_node;
            xSemaphoreGive(config_mutex);
            ESP_LOGI(TAG, "საწყისი კონფიგურაცია წარმატებით ჩაიტვირთა.");

#if (CONFIG_LOG_DEFAULT_LEVEL >= 4) // Only print if log level is INFO or higher
            char *json_debug = cJSON_Print(config_root_node);
            if (json_debug)
            {
                ESP_LOGD(TAG, "ჩატვირთული კონფიგურაცია:\n%s", json_debug);
                free(json_debug);
            }
#endif

            return ESP_OK;
        }
        else
        {
            cJSON_Delete(temp_node); // Clean up if mutex fails
            return ESP_ERR_TIMEOUT;
        }
    }
    else
    {
        ESP_LOGE(TAG, "ჩაშენებული system_config.json-ის პარსირება ვერ მოხერხდა!");
        ESP_LOGE(TAG, "JSON პარსირების შეცდომა: %s", cJSON_GetErrorPtr());
        return ESP_FAIL;
    }
}