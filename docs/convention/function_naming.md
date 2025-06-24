# ფუნქციების დასახელების წესები

## ძირითადი პრინციპები

### 1. სტრუქტურა

```
[scope_]module_action_object()
```

### 2. Scope პრეფიქსები

#### Public API ფუნქციები

- **არ იყენებს პრეფიქსს** - პირდაპირ იწყება მოდულის სახელით

```c
esp_err_t ssd1306_api_enable(void);
esp_err_t lcd1602_api_clear(void);
esp_err_t mqtt_module_connect(const char *broker_url);
```

#### Static/Private ფუნქციები

- **არ იყენებს პრეფიქსს** - კონტექსტი ნათელია ფაილიდან

```c
static esp_err_t init_display_hardware(ssd1306_private_data_t *p);
static void handle_mqtt_event(module_t *module, int32_t event_id);
static esp_err_t parse_sensor_config(const cJSON *config);
```

#### Framework API ფუნქციები

- `fmw_` პრეფიქსი

```c
esp_err_t fmw_service_register(const char *name, const char *type, void *api);
esp_err_t fmw_config_get_string(const char *key, char *value, size_t size);
module_t *fmw_module_registry_find_by_name(const char *name);
```

## მოდულის ტიპების მიხედვით

### 1. Service Locator

```c
// Public API
esp_err_t fmw_service_locator_init(void);
esp_err_t fmw_service_register(const char *name, const char *type, void *handle);
service_handle_t fmw_service_get(const char *name);
const char *fmw_service_get_type(const char *name);

// Private functions
static service_entry_t *find_service_by_name(const char *name);
static esp_err_t validate_service_parameters(const char *name, const char *type);
```

### 2. Event Bus

```c
// Public API
esp_err_t event_bus_init(void);
esp_err_t event_bus_post(int32_t event_id, void *data);
esp_err_t event_bus_subscribe(int32_t event_id, event_handler_t handler);

// Private functions
static esp_err_t deliver_event_to_subscribers(event_message_t *msg);
static event_subscriber_t *find_subscriber_by_id(int32_t event_id);
```

### 3. Display Modules

```c
// Module creation
module_t *ssd1306_module_create(const cJSON *config);
module_t *lcd1602_module_create(const cJSON *config);

// Service API (რეგისტრირებული Service Locator-ში)
esp_err_t ssd1306_api_enable(void);
esp_err_t ssd1306_api_disable(void);
esp_err_t ssd1306_api_clear(void);
esp_err_t ssd1306_api_write_text(const char *text, uint8_t line);

// Private functions
static esp_err_t init_ssd1306_hardware(ssd1306_private_data_t *p);
static void draw_text_on_screen(ssd1306_private_data_t *p, const char *text);
static esp_err_t configure_display_parameters(ssd1306_private_data_t *p);
```

### 4. Communication Modules

```c
// MQTT Module
module_t *mqtt_module_create(const cJSON *config);
esp_err_t mqtt_module_connect(const char *broker_url);
esp_err_t mqtt_module_publish(const char *topic, const char *message);
esp_err_t mqtt_module_subscribe(const char *topic);

// Private functions
static void handle_mqtt_connection_event(module_t *module, esp_mqtt_event_handle_t event);
static esp_err_t build_full_topic(module_t *module, char *buffer, const char *sub_topic);
static esp_err_t parse_incoming_command(const char *payload, telemetry_data_t *command);
```

### 5. Sensor Modules

```c
// DHT22 Module
module_t *dht22_module_create(const cJSON *config);

// Private functions
static esp_err_t read_dht22_sensor(dht22_private_data_t *p);
static esp_err_t publish_sensor_data(dht22_private_data_t *p, float temp, float humidity);
static void sensor_reading_task(void *pvParameters);
```

## სპეციალური კონვენციები

### 1. Module Lifecycle ფუნქციები

```c
// სტანდარტული სახელები ყველა მოდულისთვის
static esp_err_t [module]_init(module_t *module);
static esp_err_t [module]_start(module_t *module);
static esp_err_t [module]_stop(module_t *module);
static esp_err_t [module]_deinit(module_t *module);

// მაგალითი
static esp_err_t ssd1306_init(module_t *module);
static esp_err_t ssd1306_start(module_t *module);
```

### 2. Runtime Management ფუნქციები

```c
static esp_err_t [module]_enable(module_t *module);
static esp_err_t [module]_disable(module_t *module);
static esp_err_t [module]_reconfigure(module_t *module, const cJSON *config);
static module_status_t [module]_get_status(module_t *module);
```

### 3. Event Handler ფუნქციები

```c
static void [module]_handle_event(module_t *module, int32_t event_id, void *event_data);

// მაგალითი
static void mqtt_handle_event(module_t *module, int32_t event_id, void *event_data);
static void ssd1306_handle_event(module_t *module, int32_t event_id, void *event_data);
```

### 4. Task ფუნქციები

```c
static void [module]_task(void *pvParameters);
static void [module]_[purpose]_task(void *pvParameters);

// მაგალითი
static void dht22_task(void *pvParameters);
static void mqtt_processing_task(void *pvParameters);
static void lcd_display_task(void *pvParameters);
```

## აკრძალული პრაქტიკები

❌ **არ გამოვიყენოთ:**

- ზოგადი სახელები: `init()`, `start()`, `process()`
- Cryptic abbreviations: `ssd_en()`, `mqtt_proc()`
- Hungarian notation: `fnInit()`, `ptrHandle()`
- Camel case public APIs: `ssd1306ApiEnable()`

✅ **ყოველთვის გამოვიყენოთ:**

- Snake case: `ssd1306_api_enable()`
- Descriptive names: `handle_mqtt_connection_event()`
- Consistent prefixes: `fmw_` for framework APIs
- Clear action verbs: `init`, `start`, `enable`, `configure`

## მაგალითი სრული მოდული

```c
// ssd1306_module.c

// === Public API ===
module_t *ssd1306_module_create(const cJSON *config);

// === Service API (for Service Locator) ===
esp_err_t ssd1306_api_enable(void);
esp_err_t ssd1306_api_disable(void);
esp_err_t ssd1306_api_clear(void);
esp_err_t ssd1306_api_write_text(const char *text, uint8_t line);

// === Module Lifecycle ===
static esp_err_t ssd1306_init(module_t *module);
static esp_err_t ssd1306_start(module_t *module);
static esp_err_t ssd1306_deinit(module_t *module);

// === Runtime Management ===
static esp_err_t ssd1306_enable(module_t *module);
static esp_err_t ssd1306_disable(module_t *module);
static module_status_t ssd1306_get_status(module_t *module);

// === Event Handling ===
static void ssd1306_handle_event(module_t *module, int32_t event_id, void *event_data);

// === Hardware Operations ===
static esp_err_t init_ssd1306_hardware(ssd1306_private_data_t *p);
static void draw_text_on_screen(ssd1306_private_data_t *p, const char *text);
static void clear_display_buffer(ssd1306_private_data_t *p);
```