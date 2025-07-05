/**
 * @file storage_backend_sd.c
 * @brief SD Card (FAT filesystem) backend for the Storage Manager.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-05
 * @details This file provides the concrete implementation of the storage_api_t
 *          interface using the ESP-IDF FAT filesystem driver for SD cards over
 *          an SPI bus. It supports all file and key-value operations.
 */

// --- Includes ---
#include "logging.h"
#include "storage_backend_sd.h"
#include "framework_config.h" // For Kconfig values
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> // For mkdir
#include <dirent.h>   // For opendir

// --- Component Tag ---
DEFINE_COMPONENT_TAG("BACKEND_SD");

// --- Helper Macros ---
#define SD_PATH_MAX 256 // Maximum path length for SD card operations
#define INT_BUFFER_SIZE 12  // Buffer size for int32_t to string conversion

// --- Global state for the backend ---
static sdmmc_card_t* card; // Global handle for the mounted SD card

// =========================================================================
//                      Forward Declarations
// =========================================================================
// Note: The implementation of these functions is identical to the SPIFFS backend
// because both use the standard C file I/O API over VFS. We can reuse them.
static esp_err_t fatfs_set_string(const char* owner, const char* key, const char* value);
static esp_err_t fatfs_get_string(const char* owner, const char* key, char* buffer, size_t* buffer_size);
static esp_err_t fatfs_set_i32(const char* owner, const char* key, int32_t value);
static esp_err_t fatfs_get_i32(const char* owner, const char* key, int32_t* out_value);
static esp_err_t fatfs_set_bool(const char* owner, const char* key, bool value);
static esp_err_t fatfs_get_bool(const char* owner, const char* key, bool* out_value);
static esp_err_t fatfs_erase_key(const char* owner, const char* key);
static esp_err_t fatfs_write_file(const char* file_path, const void* data, size_t size);
static esp_err_t fatfs_read_file(const char* file_path, void* buffer, size_t* buffer_size);
static esp_err_t fatfs_delete_file(const char* file_path);
static bool fatfs_file_exists(const char* file_path);

// =========================================================================
//                      Helper Functions
// =========================================================================

/**
 * @internal
 * @brief Constructs a full file path from the owner and key for the SD card.
 * @details Creates a path in the format: /mount_point/owner/key
 */
static esp_err_t build_kv_path(char* full_path, size_t path_size, const char* owner, const char* key) {
    int required_len = snprintf(full_path, path_size, "%s/%s/%s", CONFIG_STORAGE_SD_CARD_MOUNT_POINT, owner, key);
    if (required_len >= path_size) {
        ESP_LOGE(TAG, "Constructed path is too long: %s/%s/%s", CONFIG_STORAGE_SD_CARD_MOUNT_POINT, owner, key);
        return ESP_ERR_INVALID_ARG;
    }
    return ESP_OK;
}

/**
 * @internal
 * @brief Ensures that a directory exists on the SD card, creating it if necessary.
 */
static esp_err_t ensure_directory_exists(const char* dir_path) {
    struct stat st;
    if (stat(dir_path, &st) == 0 && S_ISDIR(st.st_mode)) {
        return ESP_OK; // Directory already exists
    }
    if (mkdir(dir_path, 0755) != 0) {
        ESP_LOGE(TAG, "Failed to create directory: %s", dir_path);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Created directory: %s", dir_path);
    return ESP_OK;
}

// =========================================================================
//                      API Function Implementations
// =========================================================================

// The implementation for file and key-value operations is identical to SPIFFS
// because they both rely on the VFS layer and standard C file I/O functions.

static esp_err_t fatfs_write_file(const char* file_path, const void* data, size_t size) {
    if (!file_path || !data) return ESP_ERR_INVALID_ARG;
    FILE* f = fopen(file_path, "wb");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing: %s", file_path);
        return ESP_FAIL;
    }
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? ESP_OK : ESP_FAIL;
}

static esp_err_t fatfs_read_file(const char* file_path, void* buffer, size_t* buffer_size) {
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

static esp_err_t fatfs_delete_file(const char* file_path) {
    if (!file_path) return ESP_ERR_INVALID_ARG;
    return (remove(file_path) == 0) ? ESP_OK : ESP_FAIL;
}

static bool fatfs_file_exists(const char* file_path) {
    if (!file_path) return false;
    struct stat st;
    return (stat(file_path, &st) == 0);
}

static esp_err_t fatfs_set_string(const char* owner, const char* key, const char* value) {
    char path[SD_PATH_MAX], dir_path[SD_PATH_MAX];
    snprintf(dir_path, sizeof(dir_path), "%s/%s", CONFIG_STORAGE_SD_CARD_MOUNT_POINT, owner);
    if (ensure_directory_exists(dir_path) != ESP_OK) return ESP_FAIL;
    if (build_kv_path(path, sizeof(path), owner, key) != ESP_OK) return ESP_ERR_INVALID_ARG;
    return fatfs_write_file(path, value, strlen(value));
}

static esp_err_t fatfs_get_string(const char* owner, const char* key, char* buffer, size_t* buffer_size) {
    char path[SD_PATH_MAX];
    if (build_kv_path(path, sizeof(path), owner, key) != ESP_OK) return ESP_ERR_INVALID_ARG;
    esp_err_t err = fatfs_read_file(path, buffer, buffer_size);
    if (err == ESP_OK) {
        if (*buffer_size < (size_t)buffer_size) buffer[*buffer_size] = '\0';
        else buffer[(size_t)buffer_size - 1] = '\0';
    }
    return err;
}

static esp_err_t fatfs_set_i32(const char* owner, const char* key, int32_t value) {
    char buffer[INT_BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "%" PRIi32, value);
    return fatfs_set_string(owner, key, buffer);
}

static esp_err_t fatfs_get_i32(const char* owner, const char* key, int32_t* out_value) {
    if (!out_value) return ESP_ERR_INVALID_ARG;
    char buffer[INT_BUFFER_SIZE];
    size_t size = sizeof(buffer);
    esp_err_t err = fatfs_get_string(owner, key, buffer, &size);
    if (err == ESP_OK) *out_value = (int32_t)strtol(buffer, NULL, 10);
    return err;
}

static esp_err_t fatfs_set_bool(const char* owner, const char* key, bool value) {
    return fatfs_set_string(owner, key, value ? "1" : "0");
}

static esp_err_t fatfs_get_bool(const char* owner, const char* key, bool* out_value) {
    if (!out_value) return ESP_ERR_INVALID_ARG;
    char buffer[2];
    size_t size = sizeof(buffer);
    esp_err_t err = fatfs_get_string(owner, key, buffer, &size);
    if (err == ESP_OK) *out_value = (strcmp(buffer, "1") == 0);
    return err;
}

static esp_err_t fatfs_erase_key(const char* owner, const char* key) {
    char path[SD_PATH_MAX];
    if (build_kv_path(path, sizeof(path), owner, key) != ESP_OK) return ESP_ERR_INVALID_ARG;
    return fatfs_delete_file(path);
}

// =========================================================================
//                      Backend Initialization Function
// =========================================================================

/**
 * @brief Initializes the SD Card backend and populates the API function table.
 * @see storage_backend_sd.h
 */
esp_err_t storage_backend_sd_init(storage_api_t* api_out) {
    if (!api_out) {
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Initializing SD Card storage backend...");

    esp_err_t ret;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = CONFIG_STORAGE_SD_SPI_HOST;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = CONFIG_STORAGE_SD_PIN_MOSI,
        .miso_io_num = CONFIG_STORAGE_SD_PIN_MISO,
        .sclk_io_num = CONFIG_STORAGE_SD_PIN_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPI bus.");
        return ESP_FAIL;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = CONFIG_STORAGE_SD_PIN_CS;
    slot_config.host_id = host.slot;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    ret = esp_vfs_fat_sdspi_mount(CONFIG_STORAGE_SD_CARD_MOUNT_POINT, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. If you want to format card on error, enable format_if_mount_failed in mount config.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). Make sure SD card is inserted and wired correctly.", esp_err_to_name(ret));
        }
        spi_bus_free(host.slot);
        return ESP_FAIL;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);

    // Populate the API structure with pointers to our FAT FS functions
    api_out->set_string = fatfs_set_string;
    api_out->get_string = fatfs_get_string;
    api_out->set_i32 = fatfs_set_i32;
    api_out->get_i32 = fatfs_get_i32;
    api_out->set_bool = fatfs_set_bool;
    api_out->get_bool = fatfs_get_bool;
    api_out->erase_key = fatfs_erase_key;
    api_out->write_file = fatfs_write_file;
    api_out->read_file = fatfs_read_file;
    api_out->delete_file = fatfs_delete_file;
    api_out->file_exists = fatfs_file_exists;

    ESP_LOGI(TAG, "SD Card backend initialized and mounted at %s.", CONFIG_STORAGE_SD_CARD_MOUNT_POINT);
    return ESP_OK;
}

/**
 * @brief Deinitializes the SD Card backend.
 * @see storage_backend_sd.h
 */
void storage_backend_sd_deinit(void) {
    ESP_LOGI(TAG, "Deinitializing SD Card storage backend...");
    
    // Unmount partition and disable SDMMC or SPI host
    esp_vfs_fat_sdcard_unmount(CONFIG_STORAGE_SD_CARD_MOUNT_POINT, card);
    ESP_LOGI(TAG, "Card unmounted.");

    // Deinitialize the bus after all devices are removed
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = CONFIG_STORAGE_SD_SPI_HOST;
    spi_bus_free(host.slot);
    ESP_LOGI(TAG, "SPI bus freed.");
}