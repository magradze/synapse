/**
 * @file storage_interface.h
 * @brief Public Service API contract for the Storage Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-05
 */

#ifndef SYNAPSE_STORAGE_INTERFACE_H
#define SYNAPSE_STORAGE_INTERFACE_H

#include "esp_err.h"
#include <stddef.h> // For size_t
#include <stdbool.h> // For bool type

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The public Service API structure for the Storage Manager.
 */
typedef struct
{
    /**
     * @brief Sets a string value for a specific owner (module) and key.
     * @param[in] owner The name of the module owning this key (acts as a namespace).
     * @param[in] key The key for the value.
     * @param[in] value The string value to store.
     * @return ESP_OK on success.
     */
    esp_err_t (*set_string)(const char* owner, const char* key, const char* value);

    /**
     * @brief Gets a string value for a specific owner and key.
     * @param[in] owner The name of the module owning this key.
     * @param[in] key The key of the value to retrieve.
     * @param[out] buffer The buffer to store the retrieved string.
     * @param[in,out] buffer_size On input, the size of the buffer. On output, the actual length of the string.
     * @return ESP_OK on success, ESP_ERR_NOT_FOUND if not found.
     */
    esp_err_t (*get_string)(const char* owner, const char* key, char* buffer, size_t* buffer_size);

    /**
     * @brief Sets a 32-bit integer value.
     * @param[in] owner The owner/namespace.
     * @param[in] key The key.
     * @param[in] value The integer value.
     * @return ESP_OK on success.
     */
    esp_err_t (*set_i32)(const char* owner, const char* key, int32_t value);

    /**
     * @brief Gets a 32-bit integer value.
     * @param[in] owner The owner/namespace.
     * @param[in] key The key.
     * @param[out] out_value Pointer to store the retrieved value.
     * @return ESP_OK on success.
     */
    esp_err_t (*get_i32)(const char* owner, const char* key, int32_t* out_value);

        /**
     * @brief Sets a boolean value.
     * @details Internally stored as a uint8_t (0 for false, 1 for true).
     * @param[in] owner The owner/namespace.
     * @param[in] key The key.
     * @param[in] value The boolean value.
     * @return ESP_OK on success.
     */
    esp_err_t (*set_bool)(const char* owner, const char* key, bool value);

    /**
     * @brief Gets a boolean value.
     * @param[in] owner The owner/namespace.
     * @param[in] key The key.
     * @param[out] out_value Pointer to store the retrieved boolean value.
     * @return ESP_OK on success.
     */
    esp_err_t (*get_bool)(const char* owner, const char* key, bool* out_value);

    /**
     * @brief Erases a specific key-value pair for an owner.
     * @param[in] owner The owner/namespace.
     * @param[in] key The key to erase.
     * @return ESP_OK on success.
     */
    esp_err_t (*erase_key)(const char* owner, const char* key);

    /**
     * @brief Writes data to a file in the selected backend.
     * @details Creates the file if it doesn't exist, overwrites it if it does.
     * @param[in] file_path The full path to the file (e.g., "/config/settings.json").
     * @param[in] data Pointer to the data to be written.
     * @param[in] size The size of the data in bytes.
     * @return ESP_OK on success.
     */
    esp_err_t (*write_file)(const char* file_path, const void* data, size_t size);

    /**
     * @brief Reads data from a file.
     * @param[in] file_path The full path to the file.
     * @param[out] buffer Buffer to store the read data.
     * @param[in,out] buffer_size On input, the size of the buffer. On output, the actual bytes read.
     * @return ESP_OK on success, ESP_ERR_NOT_FOUND if file doesn't exist.
     */
    esp_err_t (*read_file)(const char* file_path, void* buffer, size_t* buffer_size);

    /**
     * @brief Deletes a file.
     * @param[in] file_path The path to the file to be deleted.
     * @return ESP_OK on success.
     */
    esp_err_t (*delete_file)(const char* file_path);

    /**
     * @brief Checks if a file exists.
     * @param[in] file_path The path to the file.
     * @return true if the file exists, false otherwise.
     */
    bool (*file_exists)(const char* file_path);

} storage_api_t;

#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_STORAGE_INTERFACE_H