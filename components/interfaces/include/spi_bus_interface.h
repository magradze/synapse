/**
 * @file spi_bus_interface.h
 * @brief Public Service API contract for the SPI Bus module.
 * @author (Your Name) & Synapse AI Assistant
 * @version 1.0.0
 * @date 2025-08-20
 */
#ifndef SYNAPSE_SPI_BUS_INTERFACE_H
#define SYNAPSE_SPI_BUS_INTERFACE_H

#include "esp_err.h"
#include "driver/spi_master.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration for the opaque device handle
typedef struct spi_device_t* spi_device_handle_t;

/**
 * @brief The public Service API structure for the SPI Bus module.
 */
typedef struct {
    /**
     * @brief Adds a new device to the SPI bus.
     * @param[in] context The context handle for the specific SPI bus instance.
     * @param[in] dev_config Pointer to the device configuration structure.
     * @param[out] out_handle Pointer to store the handle of the newly added device.
     * @return ESP_OK on success.
     */
    esp_err_t (*add_device)(void *context, const spi_device_interface_config_t *dev_config, spi_device_handle_t *out_handle);

    /**
     * @brief Removes a device from the SPI bus.
     * @param[in] context The context handle.
     * @param[in] handle The handle of the device to remove.
     * @return ESP_OK on success.
     */
    esp_err_t (*remove_device)(void *context, spi_device_handle_t handle);

    /**
     * @brief Performs a transaction on a specific SPI device.
     * @param[in] context The context handle.
     * @param[in] handle The handle of the target device.
     * @param[in,out] transaction Pointer to the transaction descriptor.
     * @return ESP_OK on success.
     */
    esp_err_t (*transmit)(void *context, spi_device_handle_t handle, spi_transaction_t *transaction);

} spi_bus_api_t;

/**
 * @brief The handle returned by the Service Locator for an SPI bus service.
 */
typedef struct {
    const spi_bus_api_t *api;
    void* context;
} spi_bus_handle_t;

#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_SPI_BUS_INTERFACE_H