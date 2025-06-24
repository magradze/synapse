# მოდულის სტრუქტურის წესები

## საქაღალდის სტრუქტურა

### სტანდარტული მოდულის layout

```
components/modules/{category}/{module_name}/
├── CMakeLists.txt
├── module.json
├── README.md
├── include/
│   └── {module_name}.h
└── {module_name}.c
```

### კატეგორიები

```
components/modules/
├── actuators/          # მოქმედი მოწყობილობები
│   └── relay_module/
├── communication/      # კომუნიკაციის მოდულები
│   ├── mqtt_module/
│   └── wifi_module/
├── display/           # ეკრანები და ინდიკატორები
│   ├── ssd1306_module/
│   └── lcd1602_module/
├── drivers/           # დრაივერები
│   ├── i2c_bus_module/
│   └── spi_bus_module/
├── provisioning/      # კონფიგურაციის მოდულები
│   └── ble_prov_module/
├── sensors/           # სენსორები
│   └── dht22_module/
├── system/            # სისტემური მოდულები
│   ├── ota_module/
│   └── version_module/
└── utility/           # დამხმარე მოდულები
    └── logger_module/
```

## ფაილის სტრუქტურა

### 1. Header ფაილი (`include/{module_name}.h`)

```c
#ifndef {MODULE_NAME}_H
#define {MODULE_NAME}_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

// === Service API Structure (თუ Service Locator-ს იყენებს) ===
typedef struct {
    esp_err_t (*enable)(void);
    esp_err_t (*disable)(void);
    // სხვა API ფუნქციები...
} {module_name}_api_t;

// === Public Functions ===
module_t *{module_name}_create(const cJSON *config);

// === Service API Functions (თუ Service Locator-ს იყენებს) ===
esp_err_t {module_name}_api_enable(void);
esp_err_t {module_name}_api_disable(void);

#endif // {MODULE_NAME}_H
```

### 2. Source ფაილი (`{module_name}.c`)

```c
// === Includes ===
#include "{module_name}.h"

// Core framework headers
#include "service_locator.h"  // თუ Service Locator-ს იყენებს
#include "event_bus.h"        // თუ Event Bus-ს იყენებს
#include "config_manager.h"
#include "logging.h"

// ESP-IDF headers
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
// სხვა საჭირო headers...

// === Component Tag ===
DEFINE_COMPONENT_TAG("{MODULE_NAME}");

// === Global Variables (თუ Service Locator API აქვს) ===
static module_t *global_{module_name}_instance = NULL;
static {module_name}_api_t {module_name}_service_api = {
    .enable = {module_name}_api_enable,
    .disable = {module_name}_api_disable,
    // სხვა ფუნქციები...
};

// === Private Data Structure ===
typedef struct {
    char module_instance_name[32];
    // კონფიგურაციის პარამეტრები
    // runtime ცვლადები
    // hardware handles
}} {module_name}_private_data_t;

// === Function Declarations ===
// Module lifecycle
static esp_err_t {module_name}_init(module_t *module);
static esp_err_t {module_name}_start(module_t *module);
static esp_err_t {module_name}_deinit(module_t *module);

// Runtime management (თუ runtime module-ია)
static esp_err_t {module_name}_enable(module_t *module);
static esp_err_t {module_name}_disable(module_t *module);
static esp_err_t {module_name}_reconfigure(module_t *module, const cJSON *config);
static module_status_t {module_name}_get_status(module_t *module);

// Event handling (თუ events-ს იყენებს)
static void {module_name}_handle_event(module_t *module, int32_t event_id, void *event_data);

// Private helper functions
static esp_err_t parse_{module_name}_config(const cJSON *config, {module_name}_private_data_t *module_private_data);
// სხვა helper ფუნქციები...

// === Implementation ===
// [ყველა ფუნქციის იმპლემენტაცია]

// === Public API Implementation ===
module_t *{module_name}_create(const cJSON *config) {
    // Module creation logic
}

// === Service API Implementation (თუ Service Locator-ს იყენებს) ===
esp_err_t {module_name}_api_enable(void) {
    if (!global_{module_name}_instance) {
        ESP_LOGE(TAG, "Module not initialized for API access");
        return ESP_ERR_INVALID_STATE;
    }
    return {module_name}_enable(global_{module_name}_instance);
}
```

## მოდულის ტიპები

### 1. Event-Driven Module (მაგ., DHT22)

- **იყენებს:** Event Bus-ს მონაცემების გასაგზავნად
- **არ იყენებს:** Service Locator-ს
- **მაგალითი:** sensor modules, health monitor

```c
// Event-driven module structure
static void dht22_task(void *pvParameters);
static esp_err_t publish_sensor_data(dht22_private_data_t *dht22_data, float temperature, float humidity);
static void dht22_handle_event(module_t *module, int32_t event_id, void *event_data);
```

### 2. Service-Driven Module (მაგ., SSD1306)

- **იყენებს:** Service Locator-ს API-ის გასაზიარებლად
- **შეიძლება იყენებდეს:** Event Bus-ს notification-ებისთვის
- **მაგალითი:** display modules, actuator modules

```c
// Service-driven module structure
static ssd1306_api_t ssd1306_service_api = { /* API functions */ };
esp_err_t ssd1306_api_enable(void);  // Service API
static esp_err_t ssd1306_enable(module_t *module);  // Internal implementation
```

### 3. Driver Module (მაგ., I2C Bus)

- **იყენებს:** Service Locator-ს driver API-ის გასაზიარებლად
- **არ იყენებს:** Event Bus-ს (ძირითადად)
- **მაგალითი:** i2c_bus_module, spi_bus_module

```c
// Driver module structure
typedef struct {
    esp_err_t (*read)(void *handle, uint8_t device_address, uint8_t *data, size_t data_length);
    esp_err_t (*write)(void *handle, uint8_t device_address, const uint8_t *data, size_t data_length);
    esp_err_t (*scan)(void *handle);
}} i2c_bus_api_t;
```

## CMakeLists.txt სტრუქტურა

```cmake
# Standard structure
idf_component_register(
    SRCS "{module_name}.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        core 
        interfaces 
        # სხვა dependencies...
)
```

### Dependencies მიხედვით

```cmake
# Event-driven module
REQUIRES core interfaces

# Service-driven module (uses other services)
REQUIRES core interfaces i2c_bus_module

# Driver module
REQUIRES core interfaces driver

# Communication module
REQUIRES core interfaces esp_wifi mqtt
```

## module.json სტრუქტურა

```json
{
    "name": "{module_name}",
    "init_function": "{module_name}_create",
    "description": "Module description",
    "version": "1.0.0",
    "dependencies": [
        "core",
        "interfaces"
    ]
}
```

## აკრძალული პრაქტიკები

❌ **არ გავაკეთოთ:**

- პირდაპირი #include სხვა მოდულების header-ების
- Global variables without proper naming
- Mixed communication patterns (Event Bus + Service Locator არასწორად)
- Hardcoded configuration values
- Non-standard directory structure

✅ **ყოველთვის გავაკეთოთ:**

- Service Locator for direct API calls
- Event Bus for broadcast notifications
- Config-based parameters
- Standard module lifecycle
- Proper error handling
- Comprehensive logging

## მაგალითი სრული მოდული სტრუქტურა

```
components/modules/display/ssd1306_module/
├── CMakeLists.txt
│   └── REQUIRES core interfaces i2c_bus_module
├── module.json
│   └── "init_function": "ssd1306_module_create"
├── README.md
├── include/
│   └── ssd1306_module.h
│       ├── ssd1306_api_t structure
│       ├── ssd1306_module_create() declaration
│       └── ssd1306_api_*() declarations
└── ssd1306_module.c
    ├── DEFINE_COMPONENT_TAG("SSD1306_MODULE")
    ├── global_ssd1306_instance
    ├── ssd1306_service_api
    ├── ssd1306_private_data_t
    ├── Lifecycle functions (init, start, deinit)
    ├── Runtime functions (enable, disable, get_status)
    ├── Service API functions
    └── Private helper functions
```