/**
 * @file storage_backend_nvs.c
 * @brief NVS (Non-Volatile Storage) backend implementation for the Storage Manager.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-07-05
 * @details This file provides the concrete implementation of the storage_api_t
 *          interface using the ESP-IDF NVS API. It handles key-value operations
 *          and gracefully reports that file operations are not supported.
 */

// --- Includes ---
#include "synapse.h"
#include "storage_backend_nvs.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>

// --- Component Tag ---
DEFINE_COMPONENT_TAG("BACKEND_NVS", SYNAPSE_LOG_COLOR_YELLOW);

// =========================================================================
//                      API Function Implementations
// =========================================================================

/**
 * @internal
 * @brief Sets a string value in NVS under a given owner (namespace) and key.
 * @see storage_interface.h
 */
// ★★★ შესწორება: ფუნქციებს გადაერქვა სახელები კონფლიქტის თავიდან ასაცილებლად ★★★
static esp_err_t backend_nvs_set_string(const char* owner, const char* key, const char* value) {
    if (!owner || !key || !value) {
        ESP_LOGE(TAG, "set_string failed: owner, key, or value is NULL.");
        return ESP_ERR_INVALID_ARG;
    }
    
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(owner, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS namespace '%s': %s", owner, esp_err_to_name(err));
        return err;
    }

    err = nvs_set_str(nvs_handle, key, value);
    if (err == ESP_OK) {
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to commit to NVS for owner '%s': %s", owner, esp_err_to_name(err));
        }
    } else {
        ESP_LOGE(TAG, "Failed to set string in NVS for owner '%s', key '%s': %s", owner, key, esp_err_to_name(err));
    }

    nvs_close(nvs_handle);
    return err;
}

/**
 * @internal
 * @brief Gets a string value from NVS.
 * @see storage_interface.h
 */
static esp_err_t backend_nvs_get_string(const char* owner, const char* key, char* buffer, size_t* buffer_size) {
    if (!owner || !key || !buffer || !buffer_size) {
        ESP_LOGE(TAG, "get_string failed: owner, key, buffer, or buffer_size is NULL.");
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(owner, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            ESP_LOGD(TAG, "NVS namespace '%s' not found.", owner);
        } else {
            ESP_LOGE(TAG, "Failed to open NVS namespace '%s': %s", owner, esp_err_to_name(err));
        }
        return err;
    }

    err = nvs_get_str(nvs_handle, key, buffer, buffer_size);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGD(TAG, "Key '%s' not found in namespace '%s'.", key, owner);
    } else if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get string from NVS for owner '%s', key '%s': %s", owner, key, esp_err_to_name(err));
    }

    nvs_close(nvs_handle);
    return err;
}

/**
 * @internal
 * @brief Sets a 32-bit integer value in NVS.
 * @see storage_interface.h
 */
static esp_err_t backend_nvs_set_i32(const char* owner, const char* key, int32_t value) {
    if (!owner || !key) return ESP_ERR_INVALID_ARG;
    
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(owner, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) return err;

    err = nvs_set_i32(nvs_handle, key, value);
    if (err == ESP_OK) {
        err = nvs_commit(nvs_handle);
    }
    nvs_close(nvs_handle);
    return err;
}

/**
 * @internal
 * @brief Gets a 32-bit integer value from NVS.
 * @see storage_interface.h
 */
static esp_err_t backend_nvs_get_i32(const char* owner, const char* key, int32_t* out_value) {
    if (!owner || !key || !out_value) return ESP_ERR_INVALID_ARG;

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(owner, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) return err;

    err = nvs_get_i32(nvs_handle, key, out_value);
    nvs_close(nvs_handle);
    return err;
}

/**
 * @internal
 * @brief Sets a boolean value in NVS (as a uint8_t).
 * @see storage_interface.h
 */
static esp_err_t backend_nvs_set_bool(const char* owner, const char* key, bool value) {
    if (!owner || !key) return ESP_ERR_INVALID_ARG;
    
    uint8_t val_to_store = value ? 1 : 0;
    
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(owner, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) return err;

    err = nvs_set_u8(nvs_handle, key, val_to_store);
    if (err == ESP_OK) {
        err = nvs_commit(nvs_handle);
    }
    nvs_close(nvs_handle);
    return err;
}

/**
 * @internal
 * @brief Gets a boolean value from NVS.
 * @see storage_interface.h
 */
static esp_err_t backend_nvs_get_bool(const char* owner, const char* key, bool* out_value) {
    if (!owner || !key || !out_value) return ESP_ERR_INVALID_ARG;

    uint8_t stored_val;
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(owner, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) return err;

    err = nvs_get_u8(nvs_handle, key, &stored_val);
    if (err == ESP_OK) {
        *out_value = (stored_val == 1);
    }
    
    nvs_close(nvs_handle);
    return err;
}

/**
 * @internal
 * @brief Erases a key-value pair from NVS.
 * @see storage_interface.h
 */
static esp_err_t backend_nvs_erase_key(const char* owner, const char* key) {
    if (!owner || !key) return ESP_ERR_INVALID_ARG;

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(owner, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) return err;

    err = nvs_erase_key(nvs_handle, key);
    if (err == ESP_OK) {
        err = nvs_commit(nvs_handle);
    }
    nvs_close(nvs_handle);
    return err;
}

/**
 * @internal
 * @brief Placeholder function for unsupported file operations on NVS.
 * @return Always returns ESP_ERR_NOT_SUPPORTED.
 */
static esp_err_t backend_nvs_unsupported_file_op_write(const char* path, const void* data, size_t size) {
    ESP_LOGE(TAG, "File operations are not supported by the NVS backend.");
    return ESP_ERR_NOT_SUPPORTED;
}

static esp_err_t backend_nvs_unsupported_file_op_read(const char* path, void* buffer, size_t* size) {
    ESP_LOGE(TAG, "File operations are not supported by the NVS backend.");
    return ESP_ERR_NOT_SUPPORTED;
}

static esp_err_t backend_nvs_unsupported_file_op_delete(const char* path) {
    ESP_LOGE(TAG, "File operations are not supported by the NVS backend.");
    return ESP_ERR_NOT_SUPPORTED;
}

static bool backend_nvs_unsupported_file_op_exists(const char* path) {
    ESP_LOGE(TAG, "File operations are not supported by the NVS backend.");
    return false;
}


// =========================================================================
//                      Backend Initialization Function
// =========================================================================

/**
 * @brief Initializes the NVS backend and populates the API function table.
 * @see storage_backend_nvs.h
 */
esp_err_t storage_backend_nvs_init(storage_api_t* api_out) {
    if (!api_out) {
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Initializing NVS storage backend...");
    
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS partition was corrupted or new version found. Erasing and re-initializing.");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Populate the API structure with pointers to our NVS-specific functions
    api_out->set_string = backend_nvs_set_string;
    api_out->get_string = backend_nvs_get_string;
    api_out->set_i32 = backend_nvs_set_i32;
    api_out->get_i32 = backend_nvs_get_i32;
    api_out->set_bool = backend_nvs_set_bool;
    api_out->get_bool = backend_nvs_get_bool;
    api_out->erase_key = backend_nvs_erase_key;

    // Assign placeholder functions for unsupported file operations
    api_out->write_file = backend_nvs_unsupported_file_op_write;
    api_out->read_file = backend_nvs_unsupported_file_op_read;
    api_out->delete_file = backend_nvs_unsupported_file_op_delete;
    api_out->file_exists = backend_nvs_unsupported_file_op_exists;

    ESP_LOGI(TAG, "NVS backend initialized and API table populated.");
    return ESP_OK;
}