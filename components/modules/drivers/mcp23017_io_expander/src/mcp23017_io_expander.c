/**
 * @file mcp23017_io_expander.c
 * @brief Implementation of the MCP23017 I/O Expander module.
 * @author Synapse Team
 * @version 1.0.0
 * @date 2025-08-21
 */

#include "synapse.h"
#include "mcp23017_io_expander.h"

#include "i2c_bus_interface.h"
#include "mcp23017_interface.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("MCP23017_EXPANDER", SYNAPSE_LOG_COLOR_MAGENTA);

// MCP23017 Register Addresses (BANK=0)
#define MCP23017_REG_IODIRA 0x00
#define MCP23017_REG_IODIRB 0x01
#define MCP23017_REG_GPPUA 0x0C
#define MCP23017_REG_GPPUB 0x0D
#define MCP23017_REG_GPIOA 0x12
#define MCP23017_REG_GPIOB 0x13
#define MCP23017_REG_OLATA 0x14
#define MCP23017_REG_OLATB 0x15
#define MCP23017_REG_IOCON 0x0A

// Private Data Structure
typedef struct
{
    char instance_name[32];
    char i2c_bus_service_name[32];
    uint8_t i2c_addr;
    i2c_bus_handle_t *i2c_handle;
    SemaphoreHandle_t api_mutex;
    mcp23017_handle_t service_handle;
    mcp23017_api_t service_api;
    uint8_t iodir_cache[2];
    uint8_t gpio_cache[2];
} mcp23017_private_data_t;

// Forward Declarations
static esp_err_t mcp23017_io_expander_init(module_t *self);
static void mcp23017_io_expander_deinit(module_t *self);
static esp_err_t api_set_pin_direction(void *context, uint8_t pin, mcp23017_direction_t direction);
static esp_err_t api_set_pin_pullup(void *context, uint8_t pin, bool enabled);
static esp_err_t api_set_pin_level(void *context, uint8_t pin, bool level);
static esp_err_t api_get_pin_level(void *context, uint8_t pin, bool *out_level);
static esp_err_t api_write_all_pins(void *context, uint16_t value);
static esp_err_t api_read_all_pins(void *context, uint16_t *out_value);
static esp_err_t read_register(mcp23017_private_data_t *private_data, uint8_t reg, uint8_t *value);
static esp_err_t write_register(mcp23017_private_data_t *private_data, uint8_t reg, uint8_t value);

module_t *mcp23017_io_expander_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    mcp23017_private_data_t *private_data = (mcp23017_private_data_t *)calloc(1, sizeof(mcp23017_private_data_t));
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
    snprintf(private_data->i2c_bus_service_name, sizeof(private_data->i2c_bus_service_name), "%s", cJSON_GetObjectItem(config_node, "i2c_bus_service")->valuestring);
    private_data->i2c_addr = (uint8_t)strtol(cJSON_GetObjectItem(config_node, "i2c_addr")->valuestring, NULL, 16);

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->init_level = 15;

    private_data->api_mutex = xSemaphoreCreateMutex();
    if (!private_data->api_mutex)
    {
        ESP_LOGE(TAG, "Failed to create API mutex");
        mcp23017_io_expander_deinit(module);
        return NULL;
    }

    private_data->service_api.set_pin_direction = api_set_pin_direction;
    private_data->service_api.set_pin_pullup = api_set_pin_pullup;
    private_data->service_api.set_pin_level = api_set_pin_level;
    private_data->service_api.get_pin_level = api_get_pin_level;
    private_data->service_api.write_all_pins = api_write_all_pins;
    private_data->service_api.read_all_pins = api_read_all_pins;

    private_data->service_handle.api = &private_data->service_api;
    private_data->service_handle.context = private_data;

    module->base.init = mcp23017_io_expander_init;
    module->base.deinit = mcp23017_io_expander_deinit;

    ESP_LOGI(TAG, "MCP23017 Expander module '%s' created.", module->name);
    return module;
}

static esp_err_t mcp23017_io_expander_init(module_t *self)
{
    mcp23017_private_data_t *private_data = (mcp23017_private_data_t *)self->private_data;

    private_data->i2c_handle = (i2c_bus_handle_t *)synapse_service_get(private_data->i2c_bus_service_name);
    if (!private_data->i2c_handle)
    {
        ESP_LOGE(TAG, "I2C bus service '%s' not found!", private_data->i2c_bus_service_name);
        return ESP_ERR_NOT_FOUND;
    }

    // Configure MCP23017 to default state (all pins as inputs)
    // Set BANK=0, MIRROR=0, SEQOP=1, DISSLW=0, HAEN=0, ODR=0, INTPOL=0
    esp_err_t ret = write_register(private_data, MCP23017_REG_IOCON, 0b00100000);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to configure IOCON register on 0x%02X", private_data->i2c_addr);
        return ret;
    }

    // Set all pins to input
    ret = write_register(private_data, MCP23017_REG_IODIRA, 0xFF);
    if (ret == ESP_OK)
    {
        ret = write_register(private_data, MCP23017_REG_IODIRB, 0xFF);
    }
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set default pin directions on 0x%02X", private_data->i2c_addr);
        return ret;
    }
    private_data->iodir_cache[0] = 0xFF;
    private_data->iodir_cache[1] = 0xFF;

    // Register our service
    ret = synapse_service_register(private_data->instance_name, SYNAPSE_SERVICE_TYPE_MCP23017_EXPANDER_API, &private_data->service_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register MCP23017 service '%s'", private_data->instance_name);
        return ret;
    }

    ESP_LOGI(TAG, "MCP23017 Expander '%s' on I2C addr 0x%02X initialized.", self->name, private_data->i2c_addr);
    return ESP_OK;
}

static void mcp23017_io_expander_deinit(module_t *self)
{
    if (!self)
        return;
    mcp23017_private_data_t *private_data = (mcp23017_private_data_t *)self->private_data;

    synapse_service_unregister(private_data->instance_name);

    if (private_data->api_mutex)
        vSemaphoreDelete(private_data->api_mutex);
    if (self->current_config)
        cJSON_Delete(self->current_config);
    free(private_data);
    ESP_LOGI(TAG, "MCP23017 Expander module deinitialized.");
}

// --- Helper Functions ---
static esp_err_t read_register(mcp23017_private_data_t *private_data, uint8_t reg, uint8_t *value)
{
    return private_data->i2c_handle->api->write_read(private_data->i2c_handle->context, private_data->i2c_addr, &reg, 1, value, 1);
}

static esp_err_t write_register(mcp23017_private_data_t *private_data, uint8_t reg, uint8_t value)
{
    uint8_t buffer[2] = {reg, value};
    return private_data->i2c_handle->api->write(private_data->i2c_handle->context, private_data->i2c_addr, buffer, 2);
}

// --- API Implementations ---
static esp_err_t api_set_pin_direction(void *context, uint8_t pin, mcp23017_direction_t direction)
{
    mcp23017_private_data_t *private_data = (mcp23017_private_data_t *)context;
    if (pin > 15)
        return ESP_ERR_INVALID_ARG;

    uint8_t reg = (pin < 8) ? MCP23017_REG_IODIRA : MCP23017_REG_IODIRB;
    uint8_t pin_mask = 1 << (pin % 8);

    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        uint8_t iodir = private_data->iodir_cache[reg];
        if (direction == MCP23017_DIRECTION_INPUT)
        {
            iodir |= pin_mask;
        }
        else
        {
            iodir &= ~pin_mask;
        }

        ret = write_register(private_data, reg, iodir);
        if (ret == ESP_OK)
        {
            private_data->iodir_cache[reg] = iodir;
        }
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}

static esp_err_t api_set_pin_pullup(void *context, uint8_t pin, bool enabled)
{
    mcp23017_private_data_t *private_data = (mcp23017_private_data_t *)context;
    if (pin > 15)
        return ESP_ERR_INVALID_ARG;

    uint8_t reg = (pin < 8) ? MCP23017_REG_GPPUA : MCP23017_REG_GPPUB;
    uint8_t pin_mask = 1 << (pin % 8);

    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        uint8_t gppu;
        ret = read_register(private_data, reg, &gppu);
        if (ret == ESP_OK)
        {
            if (enabled)
            {
                gppu |= pin_mask;
            }
            else
            {
                gppu &= ~pin_mask;
            }
            ret = write_register(private_data, reg, gppu);
        }
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}

static esp_err_t api_set_pin_level(void *context, uint8_t pin, bool level)
{
    mcp23017_private_data_t *private_data = (mcp23017_private_data_t *)context;
    if (pin > 15)
        return ESP_ERR_INVALID_ARG;

    uint8_t reg = (pin < 8) ? MCP23017_REG_GPIOA : MCP23017_REG_GPIOB;
    uint8_t pin_mask = 1 << (pin % 8);

    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        uint8_t gpio = private_data->gpio_cache[reg - MCP23017_REG_GPIOA];
        if (level)
        {
            gpio |= pin_mask;
        }
        else
        {
            gpio &= ~pin_mask;
        }

        ret = write_register(private_data, reg, gpio);
        if (ret == ESP_OK)
        {
            private_data->gpio_cache[reg - MCP23017_REG_GPIOA] = gpio;
        }
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}

static esp_err_t api_get_pin_level(void *context, uint8_t pin, bool *out_level)
{
    mcp23017_private_data_t *private_data = (mcp23017_private_data_t *)context;
    if (pin > 15 || !out_level)
        return ESP_ERR_INVALID_ARG;

    uint8_t reg = (pin < 8) ? MCP23017_REG_GPIOA : MCP23017_REG_GPIOB;
    uint8_t pin_mask = 1 << (pin % 8);

    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        uint8_t gpio;
        ret = read_register(private_data, reg, &gpio);
        if (ret == ESP_OK)
        {
            *out_level = (gpio & pin_mask) ? true : false;
        }
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}

static esp_err_t api_write_all_pins(void *context, uint16_t value)
{
    mcp23017_private_data_t *private_data = (mcp23017_private_data_t *)context;

    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        uint8_t port_a_val = value & 0xFF;
        uint8_t port_b_val = (value >> 8) & 0xFF;

        ret = write_register(private_data, MCP23017_REG_GPIOA, port_a_val);
        if (ret == ESP_OK)
        {
            ret = write_register(private_data, MCP23017_REG_GPIOB, port_b_val);
        }

        if (ret == ESP_OK)
        {
            private_data->gpio_cache[0] = port_a_val;
            private_data->gpio_cache[1] = port_b_val;
        }
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}

static esp_err_t api_read_all_pins(void *context, uint16_t *out_value)
{
    mcp23017_private_data_t *private_data = (mcp23017_private_data_t *)context;
    if (!out_value)
        return ESP_ERR_INVALID_ARG;

    esp_err_t ret = ESP_FAIL;
    if (xSemaphoreTake(private_data->api_mutex, portMAX_DELAY) == pdTRUE)
    {
        uint8_t port_a_val, port_b_val;

        // ვკითხულობთ GPIOA რეგისტრს
        ret = read_register(private_data, MCP23017_REG_GPIOA, &port_a_val);
        if (ret == ESP_OK)
        {
            // ვკითხულობთ GPIOB რეგისტრს
            ret = read_register(private_data, MCP23017_REG_GPIOB, &port_b_val);
        }

        if (ret == ESP_OK)
        {
            *out_value = (port_b_val << 8) | port_a_val;
            // ★★★ ლოგირების დონე შეცვლილია DEBUG-ზე ★★★
            ESP_LOGD(TAG, "Read All Pins: PortA=0x%02X, PortB=0x%02X, Combined=0x%04X", port_a_val, port_b_val, *out_value);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read GPIO registers!");
        }
        xSemaphoreGive(private_data->api_mutex);
    }
    return ret;
}