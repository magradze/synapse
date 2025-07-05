/**
 * @file storage_backend_spiffs.c
 * @brief SPIFFS (SPI Flash File System) backend for the Storage Manager.
 * @author Synapse Framework Team
 * @version 1.2.1 (Fixes and final implementation)
 * @date 2025-07-05
 */

// --- Includes ---
#include "storage_backend_spiffs.h"
#include "framework_config.h"
#include "esp_spiffs.h"
#include "logging.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h> // ★★★ შესწორება: დაემატა errno.h ★★★

DEFINE_COMPONENT_TAG("BACKEND_SPIFFS");

#define SPIFFS_PATH_MAX 256
#define INT_BUFFER_SIZE 12

// --- Forward Declarations ---
static esp_err_t spiffs_set_string(const char* owner, const char* key, const char* value);
static esp_err_t spiffs_get_string(const char* owner, const char* key, char* buffer, size_t* buffer_size);
static esp_err_t spiffs_set_i32(const char* owner, const char* key, int32_t value);
static esp_err_t spiffs_get_i32(const char* owner, const char* key, int32_t* out_value);
static esp_err_t spiffs_set_bool(const char* owner, const char* key, bool value);
static esp_err_t spiffs_get_bool(const char* owner, const char* key, bool* out_value);
static esp_err_t spiffs_erase_key(const char* owner, const char* key);
static esp_err_t spiffs_write_file(const char* file_path, const void* data, size_t size);
static esp_err_t spiffs_read_file(const char* file_path, void* buffer, size_t* buffer_size);
static esp_err_t spiffs_delete_file(const char* file_path);
static bool spiffs_file_exists(const char* file_path);

// =========================================================================
//                      Helper Functions
// =========================================================================

static esp_err_t build_kv_path(char* full_path, size_t path_size, const char* owner, const char* key) {
    int required_len = snprintf(full_path, path_size, "%s/%s/%s", CONFIG_STORAGE_SPIFFS_MOUNT_POINT, owner, key);
    if (required_len >= path_size) {
        ESP_LOGE(TAG, "Constructed path is too long.");
        return ESP_ERR_INVALID_ARG;
    }
    return ESP_OK;
}

// =========================================================================
//                      API Function Implementations
// =========================================================================

static esp_err_t spiffs_write_file(const char* file_path, const void* data, size_t size) {
    if (!file_path || !data) return ESP_ERR_INVALID_ARG;
    FILE* f = fopen(file_path, "wb");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing: %s. errno: %d (%s)", file_path, errno, strerror(errno));
        return ESP_FAIL;
    }
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? ESP_OK : ESP_FAIL;
}

static esp_err_t spiffs_read_file(const char* file_path, void* buffer, size_t* buffer_size) {
    if (!file_path || !buffer || !buffer_size) return ESP_ERR_INVALID_ARG;
    FILE* f = fopen(file_path, "rb");
    if (f == NULL) return ESP_ERR_NOT_FOUND;
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (file_size > *buffer_size) {
        fclose(f);
        *buffer_size = file_size;
        return ESP_ERR_NO_MEM;
    }
    size_t read_bytes = fread(buffer, 1, file_size, f);
    fclose(f);
    *buffer_size = read_bytes;
    return (read_bytes == file_size) ? ESP_OK : ESP_FAIL;
}

static esp_err_t spiffs_delete_file(const char* file_path) {
    if (!file_path) return ESP_ERR_INVALID_ARG;
    return (remove(file_path) == 0) ? ESP_OK : ESP_FAIL;
}

static bool spiffs_file_exists(const char* file_path) {
    if (!file_path) return false;
    struct stat st;
    return (stat(file_path, &st) == 0);
}

/**
 * @internal
 * @brief Emulates key-value string storage using files.
 * @details For SPIFFS, directories are created implicitly when a file with a
 *          full path is created. There is no need to call mkdir.
 * @see storage_interface.h
 */
static esp_err_t spiffs_set_string(const char* owner, const char* key, const char* value) {
    if (!owner || !key || !value) {
        return ESP_ERR_INVALID_ARG;
    }

    // ★★★ შესწორება: ამოღებულია ensure_directory_exists გამოძახება ★★★
    // SPIFFS does not support or require explicit directory creation.
    // The directory structure is created implicitly when the file is opened.

    // 1. შევქმნათ ფაილის სრული მისამართი
    char file_path[SPIFFS_PATH_MAX];
    if (build_kv_path(file_path, sizeof(file_path), owner, key) != ESP_OK) {
        return ESP_ERR_INVALID_ARG;
    }
    
    // 2. პირდაპირ ჩავწეროთ ფაილში. fopen("w") შექმნის ფაილს, თუ ის არ არსებობს.
    return spiffs_write_file(file_path, value, strlen(value));
}

static esp_err_t spiffs_get_string(const char* owner, const char* key, char* buffer, size_t* buffer_size) {
    char path[SPIFFS_PATH_MAX];
    if (build_kv_path(path, sizeof(path), owner, key) != ESP_OK) return ESP_ERR_INVALID_ARG;
    esp_err_t err = spiffs_read_file(path, buffer, buffer_size);
    if (err == ESP_OK) {
        if (*buffer_size < (size_t)buffer_size) buffer[*buffer_size] = '\0';
        else buffer[(size_t)buffer_size - 1] = '\0';
    }
    return err;
}

static esp_err_t spiffs_set_i32(const char* owner, const char* key, int32_t value) {
    char buffer[INT_BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "%" PRIi32, value);
    return spiffs_set_string(owner, key, buffer);
}

static esp_err_t spiffs_get_i32(const char* owner, const char* key, int32_t* out_value) {
    if (!out_value) return ESP_ERR_INVALID_ARG;
    char buffer[INT_BUFFER_SIZE];
    size_t size = sizeof(buffer);
    esp_err_t err = spiffs_get_string(owner, key, buffer, &size);
    if (err == ESP_OK) *out_value = (int32_t)strtol(buffer, NULL, 10);
    return err;
}

static esp_err_t spiffs_set_bool(const char* owner, const char* key, bool value) {
    return spiffs_set_string(owner, key, value ? "1" : "0");
}

static esp_err_t spiffs_get_bool(const char* owner, const char* key, bool* out_value) {
    if (!out_value) return ESP_ERR_INVALID_ARG;
    char buffer[2];
    size_t size = sizeof(buffer);
    esp_err_t err = spiffs_get_string(owner, key, buffer, &size);
    if (err == ESP_OK) *out_value = (strcmp(buffer, "1") == 0);
    return err;
}

static esp_err_t spiffs_erase_key(const char* owner, const char* key) {
    char path[SPIFFS_PATH_MAX];
    if (build_kv_path(path, sizeof(path), owner, key) != ESP_OK) return ESP_ERR_INVALID_ARG;
    return spiffs_delete_file(path);
}

// =========================================================================
//                      Backend Initialization Function
// =========================================================================

esp_err_t storage_backend_spiffs_init(storage_api_t* api_out) {
    if (!api_out) return ESP_ERR_INVALID_ARG;

    ESP_LOGI(TAG, "Initializing SPIFFS storage backend...");
    esp_log_level_set("vfs", ESP_LOG_DEBUG);
    esp_log_level_set("esp_spiffs", ESP_LOG_DEBUG);

    esp_vfs_spiffs_conf_t conf = {
      .base_path = CONFIG_STORAGE_SPIFFS_MOUNT_POINT,
      .partition_label = CONFIG_STORAGE_SPIFFS_PARTITION_LABEL,
      .max_files = CONFIG_STORAGE_SPIFFS_MAX_FILES,
      .format_if_mount_failed = CONFIG_STORAGE_SPIFFS_FORMAT_IF_MOUNT_FAILED
    };
    
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s).", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "SPIFFS Partition size: total: %d, used: %d", total, used);
    }

    // ★★★ შესწორება: ყველა ფუნქციის მინიჭება ★★★
    api_out->set_string = spiffs_set_string;
    api_out->get_string = spiffs_get_string;
    api_out->set_i32 = spiffs_set_i32;
    api_out->get_i32 = spiffs_get_i32;
    api_out->set_bool = spiffs_set_bool;
    api_out->get_bool = spiffs_get_bool;
    api_out->erase_key = spiffs_erase_key;
    api_out->write_file = spiffs_write_file;
    api_out->read_file = spiffs_read_file;
    api_out->delete_file = spiffs_delete_file;
    api_out->file_exists = spiffs_file_exists;

    ESP_LOGI(TAG, "SPIFFS backend initialized and API table populated.");
    return ESP_OK;
}

void storage_backend_spiffs_deinit(void) {
    ESP_LOGI(TAG, "Deinitializing SPIFFS storage backend...");
    esp_vfs_spiffs_unregister(CONFIG_STORAGE_SPIFFS_PARTITION_LABEL);
    ESP_LOGI(TAG, "SPIFFS unmounted.");
}

