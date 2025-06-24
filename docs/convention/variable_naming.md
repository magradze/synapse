# ცვლადების დასახელების წესები

## ძირითადი პრინციპები

### 1. დესკრიპტიული სახელები

- ცვლადის სახელი უნდა ასახავდეს მის **ნამდვილ დანიშნულებას**
- კონტექსტი უნდა იყოს ნათელი სახელიდან
- თავიდან ავირიდოთ ზოგადი პრეფიქსები

### 2. აკრძალული პრეფიქსები

❌ **არ გამოვიყენოთ:**

- `g_` (global)
- `s_` (static)
- `m_` (member)
- `p_` (pointer)
- `p` (როგორც private data pointer - არასაკმარისად დესკრიპტიული)

### 3. სწორი მიდგომა

#### Service Locator

```c
// ❌ არასწორი
static service_entry_t *service_registry_head;
static SemaphoreHandle_t service_registry_mutex;

// ✅ სწორი
static service_entry_t *service_locator_registry_head;
static SemaphoreHandle_t service_locator_mutex;
```

#### Event Bus

```c
// ❌ არასწორი
static QueueHandle_t g_event_queue;
static SemaphoreHandle_t s_event_mutex;

// ✅ სწორი
static QueueHandle_t event_bus_message_queue;
static SemaphoreHandle_t event_bus_mutex;
```

#### Module Registry

```c
// ❌ არასწორი
static module_t *registered_modules[MAX_MODULES];
static uint8_t module_count;

// ✅ სწორი
static module_t *module_registry_list[MAX_MODULES];
static uint8_t registered_modules_count;
```

#### Display Modules

```c
// ❌ არასწორი
static module_t *g_ssd1306_module;
static ssd1306_api_t g_ssd1306_api;

// ✅ სწორი
static module_t *global_ssd1306_instance;
static ssd1306_api_t ssd1306_service_api;
```

### 4. კონვენციები

#### Global ცვლადები

- `global_` პრეფიქსი მხოლოდ მაშინ როცა ნამდვილად საჭიროა
- უმჯობესია კონკრეტული კონტექსტი: `ssd1306_service_api`

#### Static ცვლადები

- მოდულის სახელი + ფუნქცია: `event_bus_message_queue`
- კომპონენტის სახელი + როლი: `service_locator_mutex`

#### Mutex/Semaphore ცვლადები

- `_mutex` სუფიქსი
- კონტექსტი წინ: `config_storage_mutex`

#### Queue ცვლადები

- `_queue` სუფიქსი
- მიზანი წინ: `event_bus_message_queue`

#### API Structure ცვლადები

- `_service_api` სუფიქსი
- მოდულის სახელი წინ: `lcd1602_service_api`

## მაგალითები

### Config Manager

```c
static SemaphoreHandle_t config_storage_mutex;
static config_data_t stored_config_data;
```

### I2C Bus Module

```c
static i2c_bus_handle_t i2c_bus_service_handle;
static SemaphoreHandle_t i2c_bus_access_mutex;
```

### MQTT Module

```c
static mqtt_private_data_t mqtt_connection_data;
static TaskHandle_t mqtt_processing_task;
```

## Private Data Pointer Naming

### ❌ არასწორი

```c
static esp_err_t ssd1306_init(module_t *module) {
    ssd1306_private_data_t *p = (ssd1306_private_data_t *)module->private_data;
    // რა არის p? გამოცნობა საჭიროა! 🤔
}
```

### ✅ სწორი

```c
static esp_err_t ssd1306_init(module_t *module) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)module->private_data;
    // ან
    ssd1306_private_data_t *ssd1306_data = (ssd1306_private_data_t *)module->private_data;
    // ან
    ssd1306_private_data_t *module_data = (ssd1306_private_data_t *)module->private_data;
    
    // ნათელია რა არის! ✨
}
```

## შეჯამება

**მთავარი წესი:** ცვლადის სახელმა უნდა უპასუხოს კითხვას - "რა არის ეს და რისთვის გამოიყენება?"

**არ გვიყვარს თვალების დათხრა და გამოცნობა! გვიყვარს სიცხადე! 😊**

**კარგი სახელი = კონტექსტი + ფუნქცია + ტიპი**