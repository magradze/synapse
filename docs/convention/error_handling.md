# Error Handling-ის წესები

## ძირითადი პრინციპები

### 1. ESP Error Codes გამოყენება

- **ყოველთვის** გამოვიყენოთ `esp_err_t` return type
- **ყოველთვის** დავაბრუნოთ `ESP_OK` წარმატების შემთხვევაში
- **ყოველთვის** გამოვიყენოთ სტანდარტული ESP error codes

### 2. Comprehensive Error Checking

- **ყველა** function call-ი უნდა იყოს checked
- **ყველა** pointer parameter უნდა იყოს validated
- **ყველა** resource allocation უნდა იყოს checked

## Standard Error Codes

### Framework-Specific Errors

```c
// components/core/include/framework_errors.h
#define ESP_ERR_SYNAPSE_BASE                    0x8000
#define ESP_ERR_SYNAPSE_MODULE_NOT_FOUND       (ESP_ERR_SYNAPSE_BASE + 1)
#define ESP_ERR_SYNAPSE_SERVICE_NOT_REGISTERED (ESP_ERR_SYNAPSE_BASE + 2)
#define ESP_ERR_SYNAPSE_INVALID_MODULE_STATE   (ESP_ERR_SYNAPSE_BASE + 3)
#define ESP_ERR_SYNAPSE_CONFIG_PARSE_FAILED    (ESP_ERR_SYNAPSE_BASE + 4)
#define ESP_ERR_SYNAPSE_RESOURCE_BUSY          (ESP_ERR_SYNAPSE_BASE + 5)
```

### Common ESP Error Codes

```c
ESP_OK                    // Success
ESP_ERR_INVALID_ARG       // Invalid argument
ESP_ERR_INVALID_STATE     // Invalid state
ESP_ERR_NOT_FOUND         // Resource not found
ESP_ERR_NO_MEM           // Out of memory
ESP_ERR_TIMEOUT          // Operation timeout
ESP_ERR_NOT_SUPPORTED    // Operation not supported
```

## Error Handling Patterns

### 1. Parameter Validation

```c
esp_err_t ssd1306_api_write_text(const char *text, uint8_t line) {
    // Validate parameters first
    if (!text) {
        ESP_LOGE(TAG, "Text parameter is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    
    if (line >= MAX_LINES) {
        ESP_LOGE(TAG, "Invalid line number: %d (max: %d)", line, MAX_LINES - 1);
        return ESP_ERR_INVALID_ARG;
    }
    
    if (!global_ssd1306_instance) {
        ESP_LOGE(TAG, "Module not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    // Continue with implementation...
    return ESP_OK;
}
```

### 2. Resource Allocation Checking

```c
static esp_err_t create_module_resources(module_private_data_t *module_private_data) {
    if (!module_private_data) {
        ESP_LOGE(TAG, "Private data pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    module_private_data->command_queue = xQueueCreate(COMMAND_QUEUE_SIZE, sizeof(command_t));
    if (!module_private_data->command_queue) {
        ESP_LOGE(TAG, "Failed to create command queue");
        return ESP_ERR_NO_MEM;
    }
    module_private_data->state_mutex = xSemaphoreCreateMutex();
    if (!module_private_data->state_mutex) {
        ESP_LOGE(TAG, "Failed to create state mutex");
        vQueueDelete(module_private_data->command_queue);
        return ESP_ERR_NO_MEM;
    }
    BaseType_t ret = xTaskCreate(module_task, "module_task", STACK_SIZE, 
                                 module_private_data, TASK_PRIORITY, &module_private_data->task_handle);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create task");
        vSemaphoreDelete(module_private_data->state_mutex);
        vQueueDelete(module_private_data->command_queue);
        return ESP_ERR_NO_MEM;
    }
    ESP_LOGI(TAG, "Module resources created successfully");
    return ESP_OK;
}
```

### 3. Service Locator Error Handling

```c
static esp_err_t get_i2c_service(i2c_bus_api_t **api) {
    if (!api) {
        ESP_LOGE(TAG, "API pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    
    // Get service from Service Locator
    service_handle_t service = synapse_service_get("i2c_bus");
    if (!service) {
        ESP_LOGE(TAG, "I2C bus service not found");
        return ESP_ERR_SYNAPSE_SERVICE_NOT_REGISTERED;
    }
    
    // Verify service type
    const char *service_type = synapse_service_get_type("i2c_bus");
    if (!service_type || strcmp(service_type, "i2c_bus_api") != 0) {
        ESP_LOGE(TAG, "Invalid service type for I2C bus: %s", 
                 service_type ? service_type : "NULL");
        return ESP_ERR_SYNAPSE_INVALID_MODULE_STATE;
    }
    
    *api = (i2c_bus_api_t *)service;
    return ESP_OK;
}
```

### 4. Configuration Parsing Error Handling

```c
static esp_err_t parse_display_config(const cJSON *config, display_config_t *display_config) {
    if (!config || !display_config) {
        ESP_LOGE(TAG, "Invalid parameters for config parsing");
        return ESP_ERR_INVALID_ARG;
    }
    
    // Parse width with validation
    cJSON *width_json = cJSON_GetObjectItem(config, "width");
    if (!cJSON_IsNumber(width_json)) {
        ESP_LOGW(TAG, "Width not found in config, using default: %d", DEFAULT_WIDTH);
        display_config->width = DEFAULT_WIDTH;
    } else {
        int width = width_json->valueint;
        if (width <= 0 || width > MAX_DISPLAY_WIDTH) {
            ESP_LOGE(TAG, "Invalid display width: %d (must be 1-%d)", width, MAX_DISPLAY_WIDTH);
            return ESP_ERR_INVALID_ARG;
        }
        display_config->width = width;
    }
    
    // Parse height with validation
    cJSON *height_json = cJSON_GetObjectItem(config, "height");
    if (!cJSON_IsNumber(height_json)) {
        ESP_LOGW(TAG, "Height not found in config, using default: %d", DEFAULT_HEIGHT);
        display_config->height = DEFAULT_HEIGHT;
    } else {
        int height = height_json->valueint;
        if (height <= 0 || height > MAX_DISPLAY_HEIGHT) {
            ESP_LOGE(TAG, "Invalid display height: %d (must be 1-%d)", height, MAX_DISPLAY_HEIGHT);
            return ESP_ERR_INVALID_ARG;
        }
        display_config->height = height;
    }
    
    ESP_LOGI(TAG, "Display config parsed: %dx%d", display_config->width, display_config->height);
    return ESP_OK;
}
```

### 5. Hardware Operation Error Handling

```c
static esp_err_t write_display_data(ssd1306_private_data_t *ssd1306_data, const uint8_t *data, size_t len) {
    if (!ssd1306_data || !data || len == 0) {
        ESP_LOGE(TAG, "Invalid parameters for display write");
        return ESP_ERR_INVALID_ARG;
    }
    i2c_bus_api_t *i2c_api;
    esp_err_t ret = get_i2c_service(&i2c_api);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get I2C service: %s", esp_err_to_name(ret));
        return ret;
    }
    int retry_count = 0;
    while (retry_count < MAX_RETRIES) {
        ret = i2c_api->write(ssd1306_data->i2c_handle, ssd1306_data->i2c_address, data, len);
        if (ret == ESP_OK) {
            break;
        }
        ESP_LOGW(TAG, "I2C write failed (attempt %d/%d): %s", 
                 retry_count + 1, MAX_RETRIES, esp_err_to_name(ret));
        retry_count++;
        if (retry_count < MAX_RETRIES) {
            vTaskDelay(pdMS_TO_TICKS(RETRY_DELAY_MS));
        }
    }
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C write failed after %d attempts: %s", 
                 MAX_RETRIES, esp_err_to_name(ret));
        return ret;
    }
    return ESP_OK;
}
```

## Cleanup and Resource Management

### 1. Proper Cleanup on Errors

```c
static esp_err_t module_init_with_cleanup(module_t *module) {
    if (!module) {
        return ESP_ERR_INVALID_ARG;
    }
    module_private_data_t *module_private_data = (module_private_data_t *)module->private_data;
    esp_err_t ret;
    ret = init_hardware(module_private_data);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Hardware initialization failed: %s", esp_err_to_name(ret));
        goto cleanup_and_exit;
    }
    ret = create_resources(module_private_data);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Resource creation failed: %s", esp_err_to_name(ret));
        goto cleanup_hardware;
    }
    ret = register_service(module_private_data);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Service registration failed: %s", esp_err_to_name(ret));
        goto cleanup_resources;
    }
    ESP_LOGI(TAG, "Module initialized successfully");
    return ESP_OK;
cleanup_resources:
    cleanup_resources(module_private_data);
cleanup_hardware:
    cleanup_hardware(module_private_data);
cleanup_and_exit:
    return ret;
}
```

### 2. Resource Cleanup Functions

```c
static void cleanup_module_resources(module_private_data_t *module_private_data) {
    if (!module_private_data) {
        return;
    }
    if (module_private_data->task_handle) {
        vTaskDelete(module_private_data->task_handle);
        module_private_data->task_handle = NULL;
        ESP_LOGI(TAG, "Task deleted");
    }
    if (module_private_data->command_queue) {
        vQueueDelete(module_private_data->command_queue);
        module_private_data->command_queue = NULL;
        ESP_LOGI(TAG, "Command queue deleted");
    }
    if (module_private_data->state_mutex) {
        vSemaphoreDelete(module_private_data->state_mutex);
        module_private_data->state_mutex = NULL;
        ESP_LOGI(TAG, "State mutex deleted");
    }
    if (module_private_data->i2c_handle) {
        i2c_bus_api_t *i2c_api;
        if (get_i2c_service(&i2c_api) == ESP_OK) {
            i2c_api->release_handle(module_private_data->i2c_handle);
        }
        module_private_data->i2c_handle = NULL;
        ESP_LOGI(TAG, "I2C handle released");
    }
}
```

## Logging Standards

### 1. Error Levels

```c
ESP_LOGE(TAG, "Critical error occurred: %s", esp_err_to_name(ret));  // Errors
ESP_LOGW(TAG, "Warning: using default value");                       // Warnings  
ESP_LOGI(TAG, "Module initialized successfully");                    // Info
ESP_LOGD(TAG, "Debug: processing command %d", cmd_id);              // Debug
ESP_LOGV(TAG, "Verbose: internal state changed");                   // Verbose
```

### 2. Error Context Information

```c
ESP_LOGE(TAG, "Failed to create module '%s': %s", 
         instance_name, esp_err_to_name(ret));

ESP_LOGE(TAG, "I2C write failed - addr: 0x%02X, len: %d, error: %s", 
         i2c_address, data_len, esp_err_to_name(ret));

ESP_LOGE(TAG, "Configuration parsing failed for key '%s': %s", 
         config_key, esp_err_to_name(ret));
```

## Error Recovery Strategies

### 1. Retry Logic

```c
static esp_err_t operation_with_retry(operation_params_t *params) {
    esp_err_t ret;
    int attempts = 0;
    
    do {
        ret = perform_operation(params);
        if (ret == ESP_OK) {
            break;
        }
        
        attempts++;
        ESP_LOGW(TAG, "Operation failed (attempt %d/%d): %s", 
                 attempts, MAX_RETRY_ATTEMPTS, esp_err_to_name(ret));
        
        if (attempts < MAX_RETRY_ATTEMPTS) {
            vTaskDelay(pdMS_TO_TICKS(RETRY_DELAY_MS));
        }
    } while (attempts < MAX_RETRY_ATTEMPTS);
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Operation failed after %d attempts", MAX_RETRY_ATTEMPTS);
    }
    
    return ret;
}
```

### 2. Graceful Degradation

```c
static esp_err_t initialize_display_with_fallback(ssd1306_private_data_t *ssd1306_data) {
    esp_err_t ret;
    ret = init_display_primary_mode(ssd1306_data);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Display initialized in primary mode");
        ssd1306_data->display_mode = DISPLAY_MODE_PRIMARY;
        return ESP_OK;
    }
    ESP_LOGW(TAG, "Primary mode failed: %s, trying fallback mode", esp_err_to_name(ret));
    ret = init_display_fallback_mode(ssd1306_data);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Display initialized in fallback mode");
        ssd1306_data->display_mode = DISPLAY_MODE_FALLBACK;
        return ESP_OK;
    }
    ESP_LOGE(TAG, "Both primary and fallback initialization failed");
    ssd1306_data->display_mode = DISPLAY_MODE_DISABLED;
    return ret;
}
```

## შეჯამება

1. **ყოველთვის** გამოვიყენოთ `esp_err_t` return codes
2. **ყოველთვის** დავაბრუნოთ `ESP_OK` წარმატების შემთხვევაში
3. **ყველა** parameter უნდა იყოს validated
4. **ყველა** resource allocation უნდა იყოს checked
5. **ყოველთვის** გავაკეთოთ proper cleanup on errors
6. **ყოველთვის** გამოვიყენოთ descriptive error logging
7. **Implement** retry logic და graceful degradation where appropriate