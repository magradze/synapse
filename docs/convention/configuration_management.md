# კონფიგურაციის მართვის წესები

## ძირითადი პრინციპი

**ყველა კონფიგურაციული პარამეტრი უნდა იყოს `system_config.json`-ში, არ უნდა იყოს hardcoded კოდში.**

## system_config.json სტრუქტურა

### სტანდარტული ფორმატი

```json
{
    "firmware": {
        "version": "2.3.0",
        "description": "Firmware description"
    },
    "global_config": {
        "device.id.prefix": "MINDLAB"
    },
    "modules": [
        {
            "type": "module_type_name",
            "enabled": true,
            "config": {
                "instance_name": "instance_name",
                // module-specific configuration
            }
        }
    ]
}
```

### მაგალითი სრული კონფიგურაცია

```json
{
    "firmware": {
        "version": "2.3.0",
        "description": "Implemented Service Locator pattern for I2C bus decoupling."
    },
    "global_config": {
        "device.id.prefix": "MINDLAB"
    },
    "modules": [
        {
            "type": "ssd1306_module",
            "enabled": true,
            "config": {
                "instance_name": "main_display",
                "i2c_bus_instance": "i2c_bus_0",
                "i2c_addr": "0x3C",
                "width": 128,
                "height": 64,
                "default_enabled": true,
                "init_text": "MINDLAB"
            }
        },
        {
            "type": "lcd1602_module",
            "enabled": true,
            "config": {
                "instance_name": "main_lcd",
                "i2c_bus_instance": "i2c_bus_0",
                "i2c_addr": "0x27",
                "rows": 2,
                "cols": 16,
                "backlight": true,
                "default_enabled": true,
                "init_text_line1": "MINDLAB ESP32",
                "init_text_line2": "Ready..."
            }
        },
        {
            "type": "relay_module",
            "enabled": true,
            "config": {
                "instance_name": "main_light",
                "pin": 23,
                "active_low": true
            }
        },
        {
            "type": "dht22_module",
            "enabled": true,
            "config": {
                "instance_name": "indoor_sensor",
                "pin": 4,
                "type": "DHT11",
                "update_interval_sec": 30
            }
        },
        {
            "type": "mqtt_module",
            "enabled": true,
            "config": {
                "instance_name": "main_broker",
                "broker_uri": "mqtt://192.168.1.160:1883",
                "username": "",
                "password": "",
                "base_topic": "/mindlab/device"
            }
        }
    ]
}
```

## კონფიგურაციის ტიპები

### 1. Hardware Configuration (Sensor Module)

```json
{
    "type": "dht22_module",
    "enabled": true,
    "config": {
        "instance_name": "indoor_sensor",
        "pin": 4,
        "type": "DHT11",
        "update_interval_sec": 30
    }
}
```

### 2. Display Configuration (SSD1306)

```json
{
    "type": "ssd1306_module",
    "enabled": true,
    "config": {
        "instance_name": "main_display",
        "i2c_bus_instance": "i2c_bus_0",
        "i2c_addr": "0x3C",
        "width": 128,
        "height": 64,
        "default_enabled": true,
        "init_text": "MINDLAB"
    }
}
```

### 3. LCD Display Configuration

```json
{
    "type": "lcd1602_module",
    "enabled": true,
    "config": {
        "instance_name": "main_lcd",
        "i2c_bus_instance": "i2c_bus_0",
        "i2c_addr": "0x27",
        "rows": 2,
        "cols": 16,
        "backlight": true,
        "default_enabled": true,
        "init_text_line1": "MINDLAB ESP32",
        "init_text_line2": "Ready..."
    }
}
```

### 4. Communication Configuration (MQTT)

```json
{
    "type": "mqtt_module",
    "enabled": true,
    "config": {
        "instance_name": "main_broker",
        "broker_uri": "mqtt://192.168.1.160:1883",
        "username": "",
        "password": "",
        "base_topic": "/mindlab/device"
    }
}
```

### 5. I2C Bus Configuration

```json
{
    "type": "i2c_bus_module",
    "enabled": true,
    "config": {
        "instance_name": "i2c_bus_0",
        "port": 0,
        "sda_pin": 21,
        "scl_pin": 22,
        "clk_speed_hz": 100000
    }
}
```

### 6. Relay Configuration

```json
{
    "type": "relay_module",
    "enabled": true,
    "config": {
        "instance_name": "main_light",
        "pin": 23,
        "active_low": true
    }
}
```

## კონფიგურაციის Parsing

### Private Data Structure

```c
typedef struct {
    char instance_name[32];
    
    // Configuration parameters from JSON
    int gpio_pin;
    bool default_enabled;
    char init_text[64];
    uint32_t read_interval_ms;
    float temperature_offset;
    
    // Runtime variables
    TaskHandle_t task_handle;
    bool task_suspended;
    // hardware handles...
} module_private_data_t;
```

### Configuration Parsing Function

```c
static esp_err_t parse_module_config(const cJSON *config, module_private_data_t *module_private_data) {
    if (!config || !module_private_data) {
        return ESP_ERR_INVALID_ARG;
    }
    
    // Parse integer values
    cJSON *gpio_pin = cJSON_GetObjectItem(config, "gpio_pin");
    if (cJSON_IsNumber(gpio_pin)) {
        module_private_data->gpio_pin = gpio_pin->valueint;
    } else {
        ESP_LOGW(TAG, "gpio_pin not found, using default: %d", DEFAULT_GPIO_PIN);
        module_private_data->gpio_pin = DEFAULT_GPIO_PIN;
    }
    
    // Parse boolean values
    cJSON *default_enabled = cJSON_GetObjectItem(config, "default_enabled");
    if (cJSON_IsBool(default_enabled)) {
        module_private_data->default_enabled = cJSON_IsTrue(default_enabled);
    } else {
        ESP_LOGW(TAG, "default_enabled not found, using default: false");
        module_private_data->default_enabled = false;
    }
    
    // Parse string values
    cJSON *init_text = cJSON_GetObjectItem(config, "init_text");
    if (cJSON_IsString(init_text)) {
        strncpy(module_private_data->init_text, init_text->valuestring, sizeof(module_private_data->init_text) - 1);
        module_private_data->init_text[sizeof(module_private_data->init_text) - 1] = '\0';
    } else {
        ESP_LOGW(TAG, "init_text not found, using default");
        strncpy(module_private_data->init_text, "Default Text", sizeof(module_private_data->init_text) - 1);
    }
    
    // Parse float values
    cJSON *temperature_offset = cJSON_GetObjectItem(config, "temperature_offset");
    if (cJSON_IsNumber(temperature_offset)) {
        module_private_data->temperature_offset = (float)temperature_offset->valuedouble;
    } else {
        ESP_LOGW(TAG, "temperature_offset not found, using default: 0.0");
        module_private_data->temperature_offset = 0.0;
    }
    
    return ESP_OK;
}
```

## Default Values

### Constants Definition

```c
// Default configuration values
#define DEFAULT_GPIO_PIN           4
#define DEFAULT_READ_INTERVAL_MS   5000
#define DEFAULT_TASK_PRIORITY      5
#define DEFAULT_STACK_SIZE         4096
#define DEFAULT_INIT_TEXT          "MindLab ESP32"
#define DEFAULT_TEMPERATURE_OFFSET 0.0
#define DEFAULT_HUMIDITY_OFFSET    0.0
```

### Fallback Logic

```c
static esp_err_t apply_default_config(module_private_data_t *module_private_data) {
    // Apply defaults if config parsing failed
    module_private_data->gpio_pin = DEFAULT_GPIO_PIN;
    module_private_data->read_interval_ms = DEFAULT_READ_INTERVAL_MS;
    module_private_data->default_enabled = false;
    module_private_data->temperature_offset = DEFAULT_TEMPERATURE_OFFSET;
    strncpy(module_private_data->init_text, DEFAULT_INIT_TEXT, sizeof(module_private_data->init_text) - 1);
    
    ESP_LOGW(TAG, "Applied default configuration");
    return ESP_OK;
}
```

## Runtime Configuration Changes

### Reconfiguration Support

```c
static esp_err_t module_reconfigure(module_t *module, const cJSON *new_config) {
    if (!module || !new_config) {
        return ESP_ERR_INVALID_ARG;
    }
    
    module_private_data_t *module_private_data = (module_private_data_t *)module->private_data;
    
    // Parse new configuration
    esp_err_t ret = parse_module_config(new_config, module_private_data);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to parse new configuration");
        return ret;
    }
    
    // Apply new configuration
    ret = apply_new_configuration(module_private_data);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to apply new configuration");
        return ret;
    }
    
    ESP_LOGI(TAG, "Module reconfigured successfully");
    return ESP_OK;
}
```

## Configuration Validation

### Input Validation

```c
static esp_err_t validate_module_config(const module_private_data_t *module_private_data) {
    // Validate GPIO pin
    if (module_private_data->gpio_pin < 0 || module_private_data->gpio_pin > 39) {
        ESP_LOGE(TAG, "Invalid GPIO pin: %d", module_private_data->gpio_pin);
        return ESP_ERR_INVALID_ARG;
    }
    
    // Validate intervals
    if (module_private_data->read_interval_ms < 100 || module_private_data->read_interval_ms > 60000) {
        ESP_LOGE(TAG, "Invalid read interval: %lu ms", module_private_data->read_interval_ms);
        return ESP_ERR_INVALID_ARG;
    }
    
    // Validate string lengths
    if (strlen(module_private_data->init_text) == 0) {
        ESP_LOGW(TAG, "Empty init_text, using default");
        strncpy((char *)module_private_data->init_text, DEFAULT_INIT_TEXT, sizeof(module_private_data->init_text) - 1);
    }
    
    return ESP_OK;
}
```

## Configuration Access Patterns

### Using Config Manager

```c
// მოდულის კონფიგურაცია იღება modules მასივიდან type-ის მიხედვით
// Config Manager ავტომატურად პოულობს შესაბამის მოდულს

// Get configuration value from module config
char broker_url[128];
esp_err_t ret = fmw_config_get_string("mqtt_module.config.broker_uri", 
                                      broker_url, sizeof(broker_url));
if (ret != ESP_OK) {
    strncpy(broker_url, DEFAULT_BROKER_URL, sizeof(broker_url) - 1);
}

// Get integer value from module config
int update_interval;
ret = fmw_config_get_int("dht22_module.config.update_interval_sec", &update_interval);
if (ret != ESP_OK) {
    update_interval = DEFAULT_UPDATE_INTERVAL;
}

// Get global configuration
char device_prefix[32];
ret = fmw_config_get_string("global_config.device.id.prefix", 
                           device_prefix, sizeof(device_prefix));
if (ret != ESP_OK) {
    strncpy(device_prefix, "MINDLAB", sizeof(device_prefix) - 1);
}
```

## აკრძალული პრაქტიკები

❌ **არ გავაკეთოთ:**

```c
// Hardcoded values in code
#define DISPLAY_WIDTH  128    // Should be in config!
#define SENSOR_PIN     4      // Should be in config!
#define MQTT_BROKER    "mqtt://broker.example.com"  // Should be in config!

// Magic numbers in code
ssd1306_init(display_width, display_height);  // Width and height should come from config
gpio_set_level(gpio_pin, active_level);   // GPIO pin should come from config
```

✅ **ყოველთვის გავაკეთოთ:**

```c
// All parameters from configuration
ssd1306_init(module_private_data->display_width, module_private_data->display_height);
gpio_set_level(module_private_data->gpio_pin, module_private_data->active_level);
mqtt_connect(module_private_data->broker_url, module_private_data->client_id);
```

## Configuration Categories

### 1. Hardware Parameters

- GPIO pins
- I2C addresses
- SPI frequencies
- UART baud rates

### 2. Timing Parameters

- Task intervals
- Timeouts
- Retry delays
- Watchdog periods

### 3. Display Parameters

- Screen dimensions
- Font sizes
- Default text
- Brightness levels

### 4. Network Parameters

- MQTT broker URLs
- WiFi credentials
- Connection timeouts
- QoS levels

### 5. Sensor Parameters

- Calibration offsets
- Filtering settings
- Sampling rates
- Threshold values

## შეჯამება

1. **ყველაფერი** `system_config.json`-ში
2. **არაფერი** hardcoded კოდში
3. **ყოველთვის** validation და default values
4. **მხარდაჭერა** runtime reconfiguration-ის
5. **ნათელი** error handling და logging