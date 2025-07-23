/**
 * @file i2c_bus.h
 * @brief Public Service API contract for the I2C Bus module.
 * @author Giorgi Magradze & Synapse AI Expert
 * @version 2.0.0
 * @date 2025-07-25
 * @details This header defines the abstract interface for an I2C bus service.
 *          It provides a thread-safe, resource-managed way for other modules
 *          to interact with a specific I2C hardware bus.
 */
#ifndef I2C_BUS_H
#define I2C_BUS_H

#include "base_module.h"
#include "cJSON.h"
#include "driver/i2c.h" // For i2c_port_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The public Service API structure for the I2C Bus module.
 * @details This structure contains function pointers for all supported I2C operations.
 *          The first parameter for each function is a generic context pointer, which
 *          internally points to the specific I2C bus instance's private data.
 */
typedef struct {
    /**
     * @brief Writes data to a specific I2C device on the bus.
     * @param[in] context The context handle provided by the i2c_bus_handle_t.
     * @param[in] device_address The 7-bit I2C address of the target device.
     * @param[in] write_data Pointer to the data buffer to be written.
     * @param[in] data_size The number of bytes to write.
     * @return esp_err_t Standard ESP-IDF error code. ESP_OK on success.
     */
    esp_err_t (*write)(void* context, uint8_t device_address, const uint8_t *write_data, size_t data_size);

    /**
     * @brief Reads data from a specific I2C device on the bus.
     * @param[in] context The context handle.
     * @param[in] device_address The 7-bit I2C address of the target device.
     * @param[out] read_data Pointer to the buffer where read data will be stored.
     * @param[in] data_size The number of bytes to read.
     * @return esp_err_t ESP_OK on success.
     */
    esp_err_t (*read)(void* context, uint8_t device_address, uint8_t *read_data, size_t data_size);

    /**
     * @brief Performs a write operation followed by a read operation without releasing the bus.
     * @param[in] context The context handle.
     * @param[in] device_address The 7-bit I2C address of the target device.
     * @param[in] write_data Pointer to the data to write (e.g., register address).
     * @param[in] write_size The number of bytes to write.
     * @param[out] read_data Pointer to the buffer for read data.
     * @param[in] read_size The number of bytes to read.
     * @return esp_err_t ESP_OK on success.
     */
    esp_err_t (*write_read)(void* context, uint8_t device_address, const uint8_t *write_data, size_t write_size, uint8_t *read_data, size_t read_size);

    /**
     * @brief Scans the I2C bus for connected devices and prints them to the log.
     * @param[in] context The context handle.
     */
    void (*scan)(void* context);

    /**
     * @brief Gets the I2C port number associated with this bus instance.
     * @param[in] context The context handle.
     * @return i2c_port_t The I2C port number, or I2C_NUM_MAX on error.
     */
    i2c_port_t (*get_port)(void* context);

} i2c_bus_api_t;

/**
 * @brief The handle returned by the Service Locator for an I2C bus service.
 * @details This structure is what consumer modules receive. It bundles the API
 *          function table with the necessary context for that specific bus instance.
 */
typedef struct {
    const i2c_bus_api_t *api; /**< @brief Pointer to the table of I2C API functions. */
    void* context;            /**< @brief Opaque context pointer for the API functions. */
} i2c_bus_handle_t;

/**
 * @brief Creates a new instance of the I2C Bus module.
 * @details This is the factory function for the module, called by the Module Factory.
 *          It takes ownership of the provided config object.
 * @param[in] config A pointer to the cJSON configuration object for this module instance.
 * @return A pointer to the newly created module_t structure, or NULL on failure.
 */
module_t *i2c_bus_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // I2C_BUS_H