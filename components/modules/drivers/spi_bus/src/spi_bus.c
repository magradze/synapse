/**
 * @file spi_bus.c
 * @brief Implementation of the SPI Bus module.
 * @author Synapse Team
 * @version 1.1.0
 */

#include "synapse.h"
#include "spi_bus.h"

#include "spi_bus_interface.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("SPI_BUS");

// Private Data Structure
typedef struct
{
    char instance_name[32];
    spi_host_device_t host;
    gpio_num_t miso_pin;
    gpio_num_t mosi_pin;
    gpio_num_t sclk_pin;
    int dma_chan;
    SemaphoreHandle_t api_mutex;
    spi_bus_handle_t service_handle;
    spi_bus_api_t service_api;
} spi_bus_private_data_t;

// Forward Declarations
static esp_err_t spi_bus_init(module_t *self);
static void spi_bus_deinit(module_t *self);
static esp_err_t api_add_device(void *context, const spi_device_interface_config_t *dev_config, spi_device_handle_t *out_handle);
static esp_err_t api_remove_device(void *context, spi_device_handle_t handle);
static esp_err_t api_transmit(void *context, spi_device_handle_t handle, spi_transaction_t *transaction);

module_t *spi_bus_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    spi_bus_private_data_t *private_data = (spi_bus_private_data_t *)calloc(1, sizeof(spi_bus_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        if (config)
            cJSON_Delete((cJSON *)config);
        return NULL;
    }

    module->private_data = private_data;
    module->current_config = (cJSON *)config;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", cJSON_GetObjectItem(config_node, "instance_name")->valuestring);
    private_data->host = (spi_host_device_t)cJSON_GetObjectItem(config_node, "spi_host")->valueint;
    private_data->miso_pin = (gpio_num_t)cJSON_GetObjectItem(config_node, "miso_pin")->valueint;
    private_data->mosi_pin = (gpio_num_t)cJSON_GetObjectItem(config_node, "mosi_pin")->valueint;
    private_data->sclk_pin = (gpio_num_t)cJSON_GetObjectItem(config_node, "sclk_pin")->valueint;
    private_data->dma_chan = cJSON_GetObjectItem(config_node, "dma_chan")->valueint;

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->init_level = 10;

    private_data->api_mutex = xSemaphoreCreateMutex();
    if (!private_data->api_mutex)
    {
        ESP_LOGE(TAG, "Failed to create API mutex");
        spi_bus_deinit(module);
        return NULL;
    }

    private_data->service_api.add_device = api_add_device;
    private_data->service_api.remove_device = api_remove_device;
    private_data->service_api.transmit = api_transmit;

    private_data->service_handle.api = &private_data->service_api;
    private_data->service_handle.context = private_data;

    module->base.init = spi_bus_init;
    module->base.deinit = spi_bus_deinit;
    module->base.start = NULL;
    module->base.handle_event = NULL;

    ESP_LOGI(TAG, "SPI Bus module '%s' created.", module->name);
    return module;
}

static esp_err_t spi_bus_init(module_t *self)
{
    spi_bus_private_data_t *private_data = (spi_bus_private_data_t *)self->private_data;
    esp_err_t ret = ESP_OK;

    ESP_LOGI(TAG, "Locking resources for SPI bus '%s'...", private_data->instance_name);
    ret = fmw_resource_lock(FMW_RESOURCE_TYPE_SPI_HOST, private_data->host, private_data->instance_name);
    if (ret != ESP_OK)
        goto fail;
    ret = fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->miso_pin, private_data->instance_name);
    if (ret != ESP_OK)
        goto fail_host;
    ret = fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->mosi_pin, private_data->instance_name);
    if (ret != ESP_OK)
        goto fail_miso;
    ret = fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, private_data->sclk_pin, private_data->instance_name);
    if (ret != ESP_OK)
        goto fail_mosi;

    spi_bus_config_t bus_cfg = {
        .miso_io_num = private_data->miso_pin,
        .mosi_io_num = private_data->mosi_pin,
        .sclk_io_num = private_data->sclk_pin,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0};
    ret = spi_bus_initialize(private_data->host, &bus_cfg, private_data->dma_chan);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize SPI bus: %s", esp_err_to_name(ret));
        goto fail_sclk;
    }

    ret = fmw_service_register(private_data->instance_name, FMW_SERVICE_TYPE_SPI_BUS_API, &private_data->service_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register SPI bus service: %s", esp_err_to_name(ret));
        spi_bus_free(private_data->host);
        goto fail_sclk;
    }

    ESP_LOGI(TAG, "SPI bus '%s' on host %d initialized successfully.", private_data->instance_name, private_data->host);
    return ESP_OK;

fail_sclk:
    fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->sclk_pin, private_data->instance_name);
fail_mosi:
    fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->mosi_pin, private_data->instance_name);
fail_miso:
    fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->miso_pin, private_data->instance_name);
fail_host:
    fmw_resource_release(FMW_RESOURCE_TYPE_SPI_HOST, private_data->host, private_data->instance_name);
fail:
    ESP_LOGE(TAG, "SPI bus initialization failed for '%s'.", private_data->instance_name);
    return ret;
}

static void spi_bus_deinit(module_t *self)
{
    if (!self || !self->private_data)
        return;
    spi_bus_private_data_t *private_data = (spi_bus_private_data_t *)self->private_data;

    fmw_service_unregister(private_data->instance_name);
    spi_bus_free(private_data->host);
    fmw_resource_release(FMW_RESOURCE_TYPE_SPI_HOST, private_data->host, private_data->instance_name);
    fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->sclk_pin, private_data->instance_name);
    fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->mosi_pin, private_data->instance_name);
    fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->miso_pin, private_data->instance_name);

    if (private_data->api_mutex)
        vSemaphoreDelete(private_data->api_mutex);
    if (self->current_config)
        cJSON_Delete(self->current_config);
    free(private_data);
    free(self);
    ESP_LOGI(TAG, "SPI bus module deinitialized.");
}

static esp_err_t api_add_device(void *context, const spi_device_interface_config_t *dev_config, spi_device_handle_t *out_handle)
{
    spi_bus_private_data_t *private_data = (spi_bus_private_data_t *)context;
    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        ret = spi_bus_add_device(private_data->host, dev_config, out_handle);
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}

static esp_err_t api_remove_device(void *context, spi_device_handle_t handle)
{
    spi_bus_private_data_t *private_data = (spi_bus_private_data_t *)context;
    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        ret = spi_bus_remove_device(handle);
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}

static esp_err_t api_transmit(void *context, spi_device_handle_t handle, spi_transaction_t *transaction)
{
    spi_bus_private_data_t *private_data = (spi_bus_private_data_t *)context;
    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        ret = spi_device_polling_transmit(handle, transaction);
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}