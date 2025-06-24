# Logging-ის სტანდარტები

## Component Tag Definition

### სტანდარტული ფორმატი

```c
// ყოველ მოდულში
DEFINE_COMPONENT_TAG("MODULE_NAME");
```

### მაგალითები

```c
// Display modules
DEFINE_COMPONENT_TAG("SSD1306_MODULE");
DEFINE_COMPONENT_TAG("LCD1602_MODULE");

// Communication modules  
DEFINE_COMPONENT_TAG("MQTT_MODULE");
DEFINE_COMPONENT_TAG("WIFI_MODULE");

// Sensor modules
DEFINE_COMPONENT_TAG("DHT22_MODULE");

// Core components
DEFINE_COMPONENT_TAG("SERVICE_LOCATOR");
DEFINE_COMPONENT_TAG("EVENT_BUS");
DEFINE_COMPONENT_TAG("CONFIG_MANAGER");
```

## Log Levels

### 1. ESP_LOGE - Error Level

**გამოიყენება:** კრიტიკული შეცდომებისთვის რომლებიც აფერხებენ ნორმალურ მუშაობას

```c
ESP_LOGE(TAG, "Failed to initialize hardware: %s", esp_err_to_name(operation_result));
ESP_LOGE(TAG, "Module creation failed for '%s': %s", module_instance_name, esp_err_to_name(operation_result));
ESP_LOGE(TAG, "Critical resource allocation failed: %s", esp_err_to_name(operation_result));
ESP_LOGE(TAG, "I2C communication error - addr: 0x%02X, error: %s", device_address, esp_err_to_name(operation_result));
```

### 2. ESP_LOGW - Warning Level

**გამოიყენება:** გაფრთხილებებისთვის, default values-ისთვის, non-critical issues-ისთვის

```c
ESP_LOGW(TAG, "Configuration key '%s' not found, using default: %d", config_key, default_value);
ESP_LOGW(TAG, "Service '%s' not available, operation skipped", service_name);
ESP_LOGW(TAG, "Retry attempt %d/%d failed: %s", attempt_number, max_attempts, esp_err_to_name(operation_result));
ESP_LOGW(TAG, "Display already disabled, ignoring disable command");
```

### 3. ESP_LOGI - Info Level

**გამოიყენება:** მნიშვნელოვანი events-ისთვის, lifecycle changes-ისთვის, status updates-ისთვის

```c
ESP_LOGI(TAG, "Module '%s' created successfully", module_instance_name);
ESP_LOGI(TAG, "Service registered: %s (type: %s)", service_name, service_type);
ESP_LOGI(TAG, "Display enabled - resolution: %dx%d", display_width, display_height);
ESP_LOGI(TAG, "MQTT connected to broker: %s", broker_url);
ESP_LOGI(TAG, "Sensor reading: temp=%.1f°C, hum=%.1f%%", temperature_celsius, humidity_percent);
```

### 4. ESP_LOGD - Debug Level

**გამოიყენება:** debugging information-ისთვის, detailed operation tracking-ისთვის

```c
ESP_LOGD(TAG, "Processing command: action='%s', target='%s'", command_action, command_target);
ESP_LOGD(TAG, "Event received: id=%ld, data_size=%d", event_id, event_data_size);
ESP_LOGD(TAG, "Task state changed: %s -> %s", task_previous_state, task_current_state);
ESP_LOGD(TAG, "Queue status: used=%d, free=%d", used_queue_slots, free_queue_slots);
```

### 5. ESP_LOGV - Verbose Level

**გამოიყენება:** ძალიან დეტალური debugging-ისთვის, internal state tracking-ისთვის

```c
ESP_LOGV(TAG, "Internal buffer updated: size=%d, checksum=0x%08X", buffer_size, buffer_checksum);
ESP_LOGV(TAG, "Memory allocation: ptr=%p, size=%d", memory_pointer, memory_size);
ESP_LOGV(TAG, "Timer callback executed: interval=%lu ms", timer_interval_ms);
```

## Logging Patterns

### 1. Module Lifecycle Logging

```c
module_t *ssd1306_module_create(const cJSON *config) {
    ESP_LOGI(TAG, "Creating SSD1306 module...");
    
    // ... module creation logic ...
    
    if (operation_result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create SSD1306 module: %s", esp_err_to_name(operation_result));
        return NULL;
    }
    
    ESP_LOGI(TAG, "SSD1306 module '%s' created successfully", module_instance_name);
    return module;
}

static esp_err_t ssd1306_init(module_t *module) {
    ESP_LOGI(TAG, "Initializing SSD1306 module...");
    
    // ... initialization logic ...
    
    ESP_LOGI(TAG, "SSD1306 module initialized successfully");
    return ESP_OK;
}
```

### 2. Service Registration Logging

```c
// Service registration
esp_err_t operation_result = fmw_service_register(module_instance_name, "display_api", &ssd1306_service_api);
if (operation_result != ESP_OK) {
    ESP_LOGE(TAG, "Failed to register service '%s': %s", module_instance_name, esp_err_to_name(operation_result));
    return operation_result;
}
ESP_LOGI(TAG, "Service registered: %s (type: display_api)", module_instance_name);

// Service usage
service_handle_t service_handle = fmw_service_get(module_instance_name);
if (!service_handle) {
    ESP_LOGW(TAG, "Service '%s' not found", module_instance_name);
    return ESP_ERR_NOT_FOUND;
}
ESP_LOGD(TAG, "Using service: %s", module_instance_name);
```

### 3. Configuration Parsing Logging

```c
static esp_err_t parse_ssd1306_config(const cJSON *config, ssd1306_private_data_t *ssd1306_data) {
    ESP_LOGD(TAG, "Parsing SSD1306 configuration...");
    
    cJSON *width = cJSON_GetObjectItem(config, "width");
    if (cJSON_IsNumber(width)) {
        ssd1306_data->display_width = width->valueint;
        ESP_LOGD(TAG, "Display width: %d", ssd1306_data->display_width);
    } else {
        ssd1306_data->display_width = DEFAULT_WIDTH;
        ESP_LOGW(TAG, "Width not specified, using default: %d", DEFAULT_WIDTH);
    }
    
    ESP_LOGI(TAG, "Configuration parsed - display: %dx%d", ssd1306_data->display_width, ssd1306_data->display_height);
    return ESP_OK;
}
```

### 4. Hardware Operation Logging

```c
static esp_err_t ssd1306_clear_display(ssd1306_private_data_t *ssd1306_data) {
    ESP_LOGD(TAG, "Clearing display buffer...");
    
    memset(ssd1306_data->display_buffer, 0, ssd1306_data->buffer_size);
    
    esp_err_t operation_result = write_display_buffer(ssd1306_data);
    if (operation_result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to clear display: %s", esp_err_to_name(operation_result));
        return operation_result;
    }
    
    ESP_LOGD(TAG, "Display cleared successfully");
    return ESP_OK;
}
```

### 5. Event Handling Logging

```c
static void mqtt_handle_event(module_t *module, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Received event: id=%ld", event_id);
    
    switch (event_id) {
        case TELEMETRY_EVENT_SENSOR_DATA:
            ESP_LOGD(TAG, "Processing sensor data event");
            handle_sensor_data(module, (telemetry_data_t *)event_data);
            break;
            
        default:
            ESP_LOGV(TAG, "Unhandled event: id=%ld", event_id);
            break;
    }
}
```

### 6. Error Context Logging

```c
// Hardware errors with context
ESP_LOGE(TAG, "I2C write failed - device: 0x%02X, register: 0x%02X, length: %d, error: %s", 
         device_address, register_address, data_length, esp_err_to_name(operation_result));

// Configuration errors with context
ESP_LOGE(TAG, "Invalid configuration value - key: '%s', value: %d, valid_range: %d-%d", 
         config_key, invalid_value, min_value, max_value);

// Service errors with context
ESP_LOGE(TAG, "Service operation failed - service: '%s', operation: '%s', error: %s", 
         service_name, operation_name, esp_err_to_name(operation_result));
```

## Runtime State Logging

### 1. Module Enable/Disable

```c
static esp_err_t ssd1306_enable(module_t *module) {
    ssd1306_private_data_t *ssd1306_data = (ssd1306_private_data_t *)module->private_data;
    
    if (!ssd1306_data->task_suspended) {
        ESP_LOGW(TAG, "Module '%s' already enabled", ssd1306_data->module_instance_name);
        return ESP_OK;
    }
    
    ESP_LOGI(TAG, "Enabling module '%s'...", ssd1306_data->module_instance_name);
    
    // ... enable logic ...
    
    ESP_LOGI(TAG, "Module '%s' enabled successfully", ssd1306_data->module_instance_name);
    return ESP_OK;
}

static esp_err_t ssd1306_disable(module_t *module) {
    ssd1306_private_data_t *ssd1306_data = (ssd1306_private_data_t *)module->private_data;
    
    if (ssd1306_data->task_suspended) {
        ESP_LOGW(TAG, "Module '%s' already disabled", ssd1306_data->module_instance_name);
        return ESP_OK;
    }
    
    ESP_LOGI(TAG, "Disabling module '%s'...", ssd1306_data->module_instance_name);
    
    // ... disable logic ...
    
    ESP_LOGI(TAG, "Module '%s' disabled successfully", ssd1306_data->module_instance_name);
    return ESP_OK;
}
```

### 2. Task State Changes

```c
static void ssd1306_task(void *pvParameters) {
    ssd1306_private_data_t *ssd1306_data = (ssd1306_private_data_t *)pvParameters;
    ESP_LOGI(TAG, "Display task started for '%s'", ssd1306_data->module_instance_name);
    
    while (1) {
        if (ssd1306_data->task_suspended) {
            ESP_LOGD(TAG, "Task suspended, waiting for resume...");
            vTaskSuspend(NULL);
            ESP_LOGD(TAG, "Task resumed");
        }
        
        // ... task logic ...
    }
}
```

## Performance Logging

### 1. Operation Timing

```c
static esp_err_t perform_heavy_operation(void) {
    int64_t operation_start_time_us = esp_timer_get_time();
    
    // ... heavy operation ...
    
    int64_t operation_end_time_us = esp_timer_get_time();
    int64_t operation_duration_us = operation_end_time_us - operation_start_time_us;
    
    ESP_LOGD(TAG, "Heavy operation completed in %lld μs", operation_duration_us);
    
    if (operation_duration_us > PERFORMANCE_WARNING_THRESHOLD_US) {
        ESP_LOGW(TAG, "Operation took longer than expected: %lld μs (threshold: %d μs)", 
                 operation_duration_us, PERFORMANCE_WARNING_THRESHOLD_US);
    }
    
    return ESP_OK;
}
```

### 2. Memory Usage

```c
static void log_memory_usage(const char *operation_name) {
    size_t free_heap_bytes = esp_get_free_heap_size();
    size_t min_free_heap_bytes = esp_get_minimum_free_heap_size();
    
    ESP_LOGD(TAG, "Memory after %s - free: %d bytes, min_free: %d bytes", 
             operation_name, free_heap_bytes, min_free_heap_bytes);
    
    if (free_heap_bytes < MEMORY_WARNING_THRESHOLD) {
        ESP_LOGW(TAG, "Low memory warning - free: %d bytes (threshold: %d bytes)", 
                 free_heap_bytes, MEMORY_WARNING_THRESHOLD);
    }
}
```

## Log Message Format Standards

### 1. Consistent Formatting

```c
ESP_LOGI(TAG, "Module created: name='%s', type='%s'", module_instance_name, module_type);
ESP_LOGE(TAG, "Operation failed: %s (code: 0x%X)", esp_err_to_name(operation_result), operation_result);
ESP_LOGD(TAG, "Processing data: size=%d, checksum=0x%08X", data_size, data_checksum);
ESP_LOGW(TAG, "Configuration missing: key='%s', using_default=%d", config_key, using_default_value);
```

### 2. Parameter Naming

```c
ESP_LOGI(TAG, "Display initialized: width=%d, height=%d, address=0x%02X", display_width, display_height, device_address);
ESP_LOGE(TAG, "Sensor read failed: pin=%d, attempts=%d, last_error=%s", sensor_pin, read_attempts, last_error_message);
ESP_LOGD(TAG, "Queue operation: action='%s', items_count=%d, max_size=%d", queue_action, queue_items_count, queue_max_size);
```

## აკრძალული პრაქტიკები

❌ **არ გავაკეთოთ:**

```c
// Don't use static const char *TAG
static const char *TAG = "SSD1306";  // Use DEFINE_COMPONENT_TAG instead

// Don't use cryptic messages
ESP_LOGI(TAG, "OK");  // Not descriptive
ESP_LOGE(TAG, "Error: %d", operation_result);  // Use esp_err_to_name()

// Don't log without context
ESP_LOGE(TAG, "Failed");  // What failed?
ESP_LOGW(TAG, "Using default");  // Default for what?
```

✅ **ყოველთვის გავაკეთოთ:**

```c
// Use DEFINE_COMPONENT_TAG
DEFINE_COMPONENT_TAG("SSD1306_MODULE");

// Use descriptive messages with context
ESP_LOGI(TAG, "Display initialization completed successfully");
ESP_LOGE(TAG, "Hardware initialization failed: %s", esp_err_to_name(operation_result));

// Provide relevant context
ESP_LOGE(TAG, "I2C write failed for display at address 0x%02X: %s", device_address, esp_err_to_name(operation_result));
ESP_LOGW(TAG, "Display width not configured, using default: %d pixels", DEFAULT_WIDTH);
```

## შეჯამება

1. **ყოველთვის** გამოვიყენოთ `DEFINE_COMPONENT_TAG("MODULE_NAME")`
2. **სწორი log level** შესაბამისი შინაარსისთვის
3. **კონტექსტი** ყველა log message-ში
4. **esp_err_to_name()** error codes-ისთვის
5. **Consistent formatting** ყველა log message-ში
6. **Performance** და **memory** logging critical operations-ისთვის