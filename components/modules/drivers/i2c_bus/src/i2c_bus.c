/**
 * @file i2c_bus.c
 * @brief Implementation of the I2C Bus module using the new ESP-IDF v5.x driver.
 * @author Giorgi Magradze
 * @version 2.1.0
 * @date 2025-07-26
 * @details This module provides a thread-safe, resource-managed service for
 *          accessing a hardware I2C bus. It utilizes the modern `i2c_master.h`
 *          driver, locks resources via the ResourceManager, and registers a
 *          service with the ServiceLocator. It adheres to the Synapse v5.0.0+
 *          memory and lifecycle management contracts.
 */

#include "synapse.h"
#include "i2c_bus.h"

#include "driver/i2c_master.h" // Using the new master driver
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <stdlib.h>
#include <string.h>

DEFINE_COMPONENT_TAG("I2C_BUS");

/**
 * @internal
 * @brief Private data structure for an I2C Bus module instance.
 */
typedef struct {
    char instance_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];
    i2c_port_t port;
    gpio_num_t sda_pin;
    gpio_num_t scl_pin;
    uint32_t clk_speed_hz;
    SemaphoreHandle_t bus_mutex;
    i2c_master_bus_handle_t bus_handle; // Handle for the new driver bus
    i2c_bus_handle_t service_handle;    // The handle that will be registered
    i2c_bus_api_t service_api;          // The API function table for this instance
} i2c_bus_private_data_t;

// --- Forward Declarations for API and Lifecycle Functions ---
static esp_err_t i2c_bus_api_write(void* context, uint8_t device_address, const uint8_t *write_data, size_t data_size);
static esp_err_t i2c_bus_api_read(void* context, uint8_t device_address, uint8_t *read_data, size_t data_size);
static esp_err_t i2c_bus_api_write_read(void* context, uint8_t device_address, const uint8_t *write_data, size_t write_size, uint8_t *read_data, size_t read_size);
static void i2c_bus_api_scan(void* context);
static i2c_port_t i2c_bus_api_get_port(void* context);

static esp_err_t i2c_bus_init(module_t *self);
static void i2c_bus_deinit(module_t *self);
static esp_err_t parse_config(const cJSON *config_node, i2c_bus_private_data_t *private_data);

// --- Module Factory Function ---

module_t *i2c_bus_create(const cJSON *config) {
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    i2c_bus_private_data_t *private_data = (i2c_bus_private_data_t *)calloc(1, sizeof(i2c_bus_private_data_t));
    if (!module || !private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory for module");
        free(module);
        free(private_data);
        if (config) cJSON_Delete((cJSON*)config);
        return NULL;
    }

    module->current_config = (cJSON*)config;
    module->private_data = private_data;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (parse_config(config_node, private_data) != ESP_OK) {
        ESP_LOGE(TAG, "Configuration parsing failed.");
        i2c_bus_deinit(module);
        return NULL;
    }

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->init_level = 10;

    private_data->service_api.write = i2c_bus_api_write;
    private_data->service_api.read = i2c_bus_api_read;
    private_data->service_api.write_read = i2c_bus_api_write_read;
    private_data->service_api.scan = i2c_bus_api_scan;
    private_data->service_api.get_port = i2c_bus_api_get_port;

    private_data->service_handle.api = &private_data->service_api;
    private_data->service_handle.context = private_data;

    module->base.init = i2c_bus_init;
    module->base.deinit = i2c_bus_deinit;
    module->base.start = NULL;
    module->base.handle_event = NULL;

    ESP_LOGI(TAG, "I2C Bus module '%s' created.", module->name);
    return module;
}

// --- Module Lifecycle Functions ---

/**
 * @internal
 * @brief Initializes the I2C bus module instance.
 * @details This function performs resource locking, configures and installs the
 *          I2C master driver, creates a mutex for thread-safe access, and
 *          registers the bus service with the Service Locator.
 * @param self Pointer to the module instance.
 * @return ESP_OK on success, or an error code on failure.
 */
static esp_err_t i2c_bus_init(module_t *self) {
    i2c_bus_private_data_t *private_data = (i2c_bus_private_data_t *)self->private_data;
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing I2C bus '%s' on port %d (SDA:%d, SCL:%d)",
             private_data->instance_name, private_data->port, private_data->sda_pin, private_data->scl_pin);

    // 1. Lock resources
    err = fmw_resource_lock(FMW_RESOURCE_TYPE_I2C_PORT, private_data->port, private_data->instance_name);
    if (err != ESP_OK) { return err; }

    err = fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->sda_pin, private_data->instance_name);
    if (err != ESP_OK) { return err; }

    err = fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->scl_pin, private_data->instance_name);
    if (err != ESP_OK) { return err; }

    // 2. Configure and install the new master bus driver
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = private_data->port,
        .sda_io_num = private_data->sda_pin,
        .scl_io_num = private_data->scl_pin,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    err = i2c_new_master_bus(&i2c_bus_config, &private_data->bus_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create new I2C master bus for '%s': %s", private_data->instance_name, esp_err_to_name(err));
        return err;
    }

    // 3. Create mutex for thread-safe API calls
    private_data->bus_mutex = xSemaphoreCreateMutex();
    if (!private_data->bus_mutex) {
        ESP_LOGE(TAG, "Failed to create mutex for I2C bus '%s'", private_data->instance_name);
        return ESP_ERR_NO_MEM;
    }

    // 4. Register the service with the Service Locator
    err = fmw_service_register(private_data->instance_name, FMW_SERVICE_TYPE_I2C_BUS_API, &private_data->service_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register I2C bus service '%s': %s", private_data->instance_name, esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "I2C bus '%s' initialized and service registered.", private_data->instance_name);
    self->status = MODULE_STATUS_INITIALIZED;
    
    // Perform an initial scan for diagnostic purposes
    i2c_bus_api_scan(private_data);

    return ESP_OK;
}

/**
 * @internal
 * @brief Deinitializes the I2C bus module instance.
 * @details This function unregisters the service, deletes the I2C driver,
 *          releases all locked resources, and frees allocated memory.
 * @param self Pointer to the module instance to deinitialize.
 */
static void i2c_bus_deinit(module_t *self) {
    if (!self) return;
    i2c_bus_private_data_t *private_data = (i2c_bus_private_data_t *)self->private_data;

    if (private_data) {
        ESP_LOGI(TAG, "De-initializing I2C bus '%s'", private_data->instance_name);
        
        fmw_service_unregister(private_data->instance_name);
        
        if (private_data->bus_handle) {
            i2c_del_master_bus(private_data->bus_handle);
        }
        
        if (private_data->bus_mutex) {
            vSemaphoreDelete(private_data->bus_mutex);
        }
        
        fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->scl_pin, private_data->instance_name);
        fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->sda_pin, private_data->instance_name);
        fmw_resource_release(FMW_RESOURCE_TYPE_I2C_PORT, private_data->port, private_data->instance_name);
        
        free(private_data);
    }

    if (self->current_config) {
        cJSON_Delete(self->current_config);
    }
    free(self);
}

// --- Internal Helper Functions ---

/**
 * @internal
 * @brief Parses the configuration from a cJSON object.
 * @param config_node Pointer to the "config" cJSON object.
 * @param private_data Pointer to the private data structure to populate.
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG if config is invalid.
 */
static esp_err_t parse_config(const cJSON *config_node, i2c_bus_private_data_t *private_data) {
    if (!config_node) {
        ESP_LOGE(TAG, "Config object is missing.");
        return ESP_ERR_INVALID_ARG;
    }

    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    const cJSON *port_node = cJSON_GetObjectItem(config_node, "port");
    const cJSON *sda_node = cJSON_GetObjectItem(config_node, "sda_pin");
    const cJSON *scl_node = cJSON_GetObjectItem(config_node, "scl_pin");
    const cJSON *speed_node = cJSON_GetObjectItem(config_node, "clk_speed_hz");

    if (!cJSON_IsString(name_node) || !cJSON_IsNumber(port_node) ||
        !cJSON_IsNumber(sda_node) || !cJSON_IsNumber(scl_node) || !cJSON_IsNumber(speed_node)) {
        ESP_LOGE(TAG, "One or more required config parameters are missing or have incorrect types.");
        return ESP_ERR_INVALID_ARG;
    }

    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", name_node->valuestring);
    private_data->port = (i2c_port_t)port_node->valueint;
    private_data->sda_pin = (gpio_num_t)sda_node->valueint;
    private_data->scl_pin = (gpio_num_t)scl_node->valueint;
    private_data->clk_speed_hz = (uint32_t)speed_node->valueint;

    return ESP_OK;
}

// --- Service API Implementations ---

/**
 * @internal
 * @brief API function implementation for writing data to an I2C device.
 */
static esp_err_t i2c_bus_api_write(void* context, uint8_t device_address, const uint8_t *write_data, size_t data_size) {
    i2c_bus_private_data_t *private_data = (i2c_bus_private_data_t *)context;
    if (xSemaphoreTake(private_data->bus_mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to lock bus '%s' for write", private_data->instance_name);
        return ESP_ERR_TIMEOUT;
    }

    // The new driver uses a device handle. We create it on-the-fly for each transaction.
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = device_address,
        .scl_speed_hz = private_data->clk_speed_hz,
    };
    i2c_master_dev_handle_t dev_handle;
    esp_err_t err = i2c_master_bus_add_device(private_data->bus_handle, &dev_cfg, &dev_handle);
    
    if (err == ESP_OK) {
        err = i2c_master_transmit(dev_handle, write_data, data_size, -1); // -1 for blocking mode
        i2c_master_bus_rm_device(dev_handle);
    }
    
    xSemaphoreGive(private_data->bus_mutex);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C write failed on '%s' - Addr: 0x%02X, Error: %s",
                 private_data->instance_name, device_address, esp_err_to_name(err));
    }
    return err;
}

/**
 * @internal
 * @brief API function implementation for reading data from an I2C device.
 */
static esp_err_t i2c_bus_api_read(void* context, uint8_t device_address, uint8_t *read_data, size_t data_size) {
    i2c_bus_private_data_t *private_data = (i2c_bus_private_data_t *)context;
    if (xSemaphoreTake(private_data->bus_mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to lock bus '%s' for read", private_data->instance_name);
        return ESP_ERR_TIMEOUT;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = device_address,
        .scl_speed_hz = private_data->clk_speed_hz,
    };
    i2c_master_dev_handle_t dev_handle;
    esp_err_t err = i2c_master_bus_add_device(private_data->bus_handle, &dev_cfg, &dev_handle);

    if (err == ESP_OK) {
        err = i2c_master_receive(dev_handle, read_data, data_size, -1);
        i2c_master_bus_rm_device(dev_handle);
    }
    
    xSemaphoreGive(private_data->bus_mutex);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C read failed on '%s' - Addr: 0x%02X, Error: %s",
                 private_data->instance_name, device_address, esp_err_to_name(err));
    }
    return err;
}

/**
 * @internal
 * @brief API function implementation for a combined write-read transaction.
 */
static esp_err_t i2c_bus_api_write_read(void* context, uint8_t device_address, const uint8_t *write_data, size_t write_size, uint8_t *read_data, size_t read_size) {
    i2c_bus_private_data_t *private_data = (i2c_bus_private_data_t *)context;
    if (xSemaphoreTake(private_data->bus_mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to lock bus '%s' for write/read", private_data->instance_name);
        return ESP_ERR_TIMEOUT;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = device_address,
        .scl_speed_hz = private_data->clk_speed_hz,
    };
    i2c_master_dev_handle_t dev_handle;
    esp_err_t err = i2c_master_bus_add_device(private_data->bus_handle, &dev_cfg, &dev_handle);

    if (err == ESP_OK) {
        err = i2c_master_transmit_receive(dev_handle, write_data, write_size, read_data, read_size, -1);
        i2c_master_bus_rm_device(dev_handle);
    }
    
    xSemaphoreGive(private_data->bus_mutex);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C write/read failed on '%s' - Addr: 0x%02X, Error: %s",
                 private_data->instance_name, device_address, esp_err_to_name(err));
    }
    return err;
}

/**
 * @internal
 * @brief API function implementation for scanning the I2C bus.
 */
static void i2c_bus_api_scan(void* context) {
    i2c_bus_private_data_t *private_data = (i2c_bus_private_data_t *)context;
    
    ESP_LOGI(TAG, "Scanning I2C bus '%s' on port %d...", private_data->instance_name, private_data->port);
    uint8_t found_count = 0;
    for (uint8_t addr = 1; addr < 127; addr++) {
        // The new driver has a dedicated probe function.
        if (i2c_master_probe(private_data->bus_handle, addr, -1) == ESP_OK) {
            ESP_LOGI(TAG, "-> Found device at address 0x%02X", addr);
            found_count++;
        }
    }
    ESP_LOGI(TAG, "Scan finished. Found %d devices on bus '%s'.", found_count, private_data->instance_name);
}

/**
 * @internal
 * @brief API function implementation for getting the port number.
 */
static i2c_port_t i2c_bus_api_get_port(void* context) {
    i2c_bus_private_data_t *private_data = (i2c_bus_private_data_t *)context;
    return private_data ? private_data->port : I2C_NUM_MAX;
}