/**
 * @file sn74hc595n_writer.c
 * @brief Implementation of the SN74HC595N shift register driver module.
 * @author Synapse Team
 * @version 1.2.0
 */

#include "synapse.h"
#include "sn74hc595n_writer.h"

#include "spi_bus_interface.h"
#include "sn74hc595n_writer_interface.h"
#include "driver/gpio.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("SN74HC595N_WRITER");

typedef struct
{
    char instance_name[32];
    char spi_bus_service_name[32];
    spi_bus_handle_t *spi_bus_handle;
    spi_device_handle_t spi_device_handle;
    gpio_num_t latch_pin;
    uint8_t num_chips;
    uint8_t *pin_states_buffer;
    SemaphoreHandle_t api_mutex;
    sn74hc595n_writer_handle_t service_handle;
    sn74hc595n_writer_api_t service_api;
} sn74hc595n_private_data_t;

static esp_err_t sn74hc595n_writer_init(module_t *self);
static void sn74hc595n_writer_deinit(module_t *self);
static esp_err_t update_shift_registers(sn74hc595n_private_data_t *private_data);
static esp_err_t api_set_pin_state(void *context, uint16_t pin, bool state);
static esp_err_t api_set_chip_pins(void *context, uint8_t chip_index, uint8_t value);
static esp_err_t api_get_pin_state(void *context, uint16_t pin, bool *out_state);

module_t *sn74hc595n_writer_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    sn74hc595n_private_data_t *private_data = (sn74hc595n_private_data_t *)calloc(1, sizeof(sn74hc595n_private_data_t));
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
    module->current_config = cJSON_Duplicate(config, true);
    if (!module->current_config)
    {
        ESP_LOGE(TAG, "Failed to duplicate configuration object.");
        // Note: This assumes 'private_data' and 'module' are allocated.
        // Manual check might be needed for each file's cleanup logic.
        free(private_data);
        free(module);
        return NULL;
    }

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", cJSON_GetObjectItem(config_node, "instance_name")->valuestring);
    snprintf(private_data->spi_bus_service_name, sizeof(private_data->spi_bus_service_name), "%s", cJSON_GetObjectItem(config_node, "spi_bus_service")->valuestring);
    private_data->latch_pin = (gpio_num_t)cJSON_GetObjectItem(config_node, "latch_pin")->valueint;
    private_data->num_chips = (uint8_t)cJSON_GetObjectItem(config_node, "num_chips")->valueint;

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->init_level = 15;

    private_data->pin_states_buffer = calloc(private_data->num_chips, sizeof(uint8_t));
    private_data->api_mutex = xSemaphoreCreateMutex();
    if (!private_data->pin_states_buffer || !private_data->api_mutex)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for buffer or mutex");
        sn74hc595n_writer_deinit(module);
        return NULL;
    }

    private_data->service_api.set_pin_state = api_set_pin_state;
    private_data->service_api.set_chip_pins = api_set_chip_pins;
    private_data->service_api.get_pin_state = api_get_pin_state;

    private_data->service_handle.api = &private_data->service_api;
    private_data->service_handle.context = private_data;

    module->base.init = sn74hc595n_writer_init;
    module->base.deinit = sn74hc595n_writer_deinit;
    module->base.start = NULL;
    module->base.handle_event = NULL;

    ESP_LOGI(TAG, "SN74HC595N writer module '%s' created.", module->name);
    return module;
}

static esp_err_t sn74hc595n_writer_init(module_t *self)
{
    sn74hc595n_private_data_t *private_data = (sn74hc595n_private_data_t *)self->private_data;
    esp_err_t ret;

    private_data->spi_bus_handle = (spi_bus_handle_t *)synapse_service_get(private_data->spi_bus_service_name);
    if (!private_data->spi_bus_handle)
    {
        ESP_LOGE(TAG, "SPI bus service '%s' not found!", private_data->spi_bus_service_name);
        return ESP_ERR_NOT_FOUND;
    }

    spi_device_interface_config_t dev_cfg = {
        .mode = 0,
        .clock_speed_hz = 10 * 1000 * 1000,
        .spics_io_num = -1,
        .queue_size = 1,
    };
    ret = private_data->spi_bus_handle->api->add_device(private_data->spi_bus_handle->context, &dev_cfg, &private_data->spi_device_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add SPI device: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = synapse_resource_lock(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->latch_pin, private_data->instance_name);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to lock latch pin %d.", private_data->latch_pin);
        private_data->spi_bus_handle->api->remove_device(private_data->spi_bus_handle->context, private_data->spi_device_handle);
        return ret;
    }
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << private_data->latch_pin),
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&io_conf);
    gpio_set_level(private_data->latch_pin, 0);

    ret = synapse_service_register(private_data->instance_name, SYNAPSE_SERVICE_TYPE_SN74HC595N_WRITER_API, &private_data->service_handle);
    if (ret != ESP_OK)
    {
        synapse_resource_release(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->latch_pin, private_data->instance_name);
        private_data->spi_bus_handle->api->remove_device(private_data->spi_bus_handle->context, private_data->spi_device_handle);
        return ret;
    }

    ESP_LOGI(TAG, "SN74HC595N writer '%s' initialized.", self->name);
    return ESP_OK;
}

static void sn74hc595n_writer_deinit(module_t *self)
{
    if (!self || !self->private_data)
        return;
    sn74hc595n_private_data_t *private_data = (sn74hc595n_private_data_t *)self->private_data;

    synapse_service_unregister(private_data->instance_name);
    if (private_data->spi_bus_handle && private_data->spi_device_handle)
    {
        private_data->spi_bus_handle->api->remove_device(private_data->spi_bus_handle->context, private_data->spi_device_handle);
    }
    synapse_resource_release(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->latch_pin, private_data->instance_name);

    if (private_data->api_mutex)
        vSemaphoreDelete(private_data->api_mutex);
    free(private_data->pin_states_buffer);
    if (self->current_config)
        cJSON_Delete(self->current_config);
    free(private_data);

    ESP_LOGI(TAG, "SN74HC595N writer module deinitialized.");
}

static esp_err_t update_shift_registers(sn74hc595n_private_data_t *private_data)
{
    ESP_LOGD(TAG, "Updating shift registers... Number of chips: %d", private_data->num_chips);
    for (int i = 0; i < private_data->num_chips; i++)
    {
        ESP_LOGD(TAG, "  Chip %d buffer state: 0x%02X", i, private_data->pin_states_buffer[i]);
    }

    gpio_set_level(private_data->latch_pin, 0);

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = private_data->num_chips * 8;
    t.tx_buffer = private_data->pin_states_buffer;

    esp_err_t ret = private_data->spi_bus_handle->api->transmit(private_data->spi_bus_handle->context, private_data->spi_device_handle, &t);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "SPI transmit failed: %s", esp_err_to_name(ret));
        gpio_set_level(private_data->latch_pin, 1);
        return ret;
    }

    gpio_set_level(private_data->latch_pin, 1);
    vTaskDelay(pdMS_TO_TICKS(1));
    gpio_set_level(private_data->latch_pin, 0);

    ESP_LOGD(TAG, "Shift registers updated successfully.");
    return ESP_OK;
}

static esp_err_t api_set_pin_state(void *context, uint16_t pin, bool state)
{
    sn74hc595n_private_data_t *private_data = (sn74hc595n_private_data_t *)context;
    uint16_t max_pins = private_data->num_chips * 8;
    if (pin >= max_pins)
        return ESP_ERR_INVALID_ARG;

    uint8_t chip_index = (max_pins - 1 - pin) / 8;
    uint8_t pin_in_chip = pin % 8;

    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        if (state)
        {
            private_data->pin_states_buffer[chip_index] |= (1 << pin_in_chip);
        }
        else
        {
            private_data->pin_states_buffer[chip_index] &= ~(1 << pin_in_chip);
        }
        ret = update_shift_registers(private_data);
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}

static esp_err_t api_set_chip_pins(void *context, uint8_t chip_index, uint8_t value)
{
    sn74hc595n_private_data_t *private_data = (sn74hc595n_private_data_t *)context;
    if (chip_index >= private_data->num_chips)
        return ESP_ERR_INVALID_ARG;

    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        private_data->pin_states_buffer[private_data->num_chips - 1 - chip_index] = value;
        ret = update_shift_registers(private_data);
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}

static esp_err_t api_get_pin_state(void *context, uint16_t pin, bool *out_state)
{
    sn74hc595n_private_data_t *private_data = (sn74hc595n_private_data_t *)context;
    uint16_t max_pins = private_data->num_chips * 8;
    if (pin >= max_pins || !out_state)
        return ESP_ERR_INVALID_ARG;

    uint8_t chip_index = (max_pins - 1 - pin) / 8;
    uint8_t pin_in_chip = pin % 8;

    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        *out_state = (private_data->pin_states_buffer[chip_index] >> pin_in_chip) & 1;
        ret = ESP_OK;
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}