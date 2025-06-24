# Synapse Event Bus API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის Event Bus-ისა და custom event-ების დეტალურ API-ს, naming და structure კონვენციების დაცვით.

---

## Event Bus API

### esp_err_t event_bus_init(void);
- ინიციალიზაციას უკეთებს Event Bus-ს.

### esp_err_t event_bus_post(int32_t event_id, void *event_data);
- ავრცელებს მოვლენას ყველა გამოწერილ მოდულზე.
- არგუმენტები: event_id (უნიკალური იდენტიფიკატორი), event_data (მონაცემები)

### esp_err_t event_bus_subscribe(int32_t event_id, event_handler_t handler);
- აძლევს მოდულს შესაძლებლობას მიიღოს კონკრეტული მოვლენა.
- არგუმენტები: event_id, handler (callback ფუნქცია)

### esp_err_t event_bus_unsubscribe(int32_t event_id, event_handler_t handler);
- წაშლის გამოწერას კონკრეტულ event-ზე.

---

## Custom Event-ების აღწერა

- ყველა custom event-ს უნდა ჰქონდეს უნიკალური იდენტიფიკატორი (მაგ: #define MY_EVENT_ID 0x1001)
- რეკომენდებულია event-ისთვის შესაბამისი მონაცემების სტრუქტურის აღწერა

### მაგალითი Custom Event-ის აღწერისა:
```c
#define TELEMETRY_EVENT_SENSOR_DATA  0x2001

typedef struct {
    char instance_name[32];
    float temperature;
    float humidity;
    int64_t timestamp;
} telemetry_data_t;
```

---

## Event Handler-ის მაგალითი

```c
static void mqtt_handle_event(module_t *module, int32_t event_id, void *event_data) {
    if (event_id == TELEMETRY_EVENT_SENSOR_DATA) {
        telemetry_data_t *data = (telemetry_data_t *)event_data;
        // ... process data ...
    }
}
```

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**Event-ის გამოქვეყნება:**
```c
event_bus_post(TELEMETRY_EVENT_SENSOR_DATA, &sensor_telemetry_data);
```

**Event-ზე გამოწერა:**
```c
event_bus_subscribe(TELEMETRY_EVENT_SENSOR_DATA, mqtt_handle_event);
```

**Custom Event-ის აღწერა და გამოყენება:**
```c
#define CUSTOM_EVENT_ID 0x3001

typedef struct {
    int value;
    char description[64];
} custom_event_data_t;

custom_event_data_t my_event = { .value = 42, .description = "Test" };
event_bus_post(CUSTOM_EVENT_ID, &my_event);
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

