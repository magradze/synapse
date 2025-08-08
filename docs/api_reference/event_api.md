# Synapse Event Bus API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის Event Bus-ისა და custom event-ების დეტალურ API-ს, naming და structure კონვენციების დაცვით.

---

## Event Bus API

### `esp_err_t synapse_event_bus_init(void)`

- ინიციალიზაციას უკეთებს Event Bus-ს. ქმნის ივენთების რიგს (queue) და იწყებს დამმუშავებელ ტასკს. უნდა გამოიძახოს მხოლოდ სისტემის ბირთვმა ერთხელ.

### `esp_err_t synapse_event_bus_post(core_framework_event_id_t event_id, event_data_wrapper_t *data_wrapper)`

- ასინქრონულად აქვეყნებს მოვლენას ყველა გამოწერილ მოდულზე.
- **არგუმენტები:**
  - `event_id`: მოვლენის უნიკალური იდენტიფიკატორი (`enum`).
  - `data_wrapper`: მაჩვენებელი ივენთის მონაცემების "გარსზე" (`wrapper`). თუ მონაცემები არ არის საჭირო, გადაეცემა `NULL`.

### `esp_err_t synapse_event_bus_subscribe(core_framework_event_id_t event_id, struct module_t *module)`

- **(განახლებული)** აძლევს მოდულს შესაძლებლობას, მიიღოს კონკრეტული მოვლენა. Event Bus-ი ავტომატურად გამოიძახებს ამ მოდულის `handle_event` ფუნქციას.
- **არგუმენტები:**
  - `event_id`: მოვლენის ID, რომელზეც ხდება გამოწერა.
  - `module`: მაჩვენებელი გამომწერ მოდულზე (`self`).

### `esp_err_t synapse_event_bus_unsubscribe(core_framework_event_id_t event_id, struct module_t *module)`

- **(ახალი)** აუქმებს მოდულის გამოწერას კონკრეტულ მოვლენაზე.
- **მნიშვნელოვანია:** ეს ფუნქცია აუცილებლად უნდა გამოიძახოს მოდულის `deinit` ფუნქციამ ყველა იმ ივენთისთვის, რომელზეც ის იყო გამოწერილი, რათა თავიდან ავიცილოთ Use-After-Free შეცდომები.
- **არგუმენტები:**
  - `event_id`: მოვლენის ID.
  - `module`: მაჩვენებელი მოდულზე, რომელიც აუქმებს გამოწერას.

---

## ივენთის მონაცემების მართვა (Reference Counting)

**მნიშვნელოვანია:** როდესაც ივენთს გადასცემთ მონაცემებს (`data_wrapper`), Synapse Framework იყენებს **reference counting** მექანიზმს მეხსიერების უსაფრთხოდ სამართავად.

- `event_data_wrapper_t *synapse_event_data_create(void *data, size_t size, event_data_free_fn_t free_fn);`
  - ქმნის მონაცემების "გარსს". `free_fn` არის callback, რომელიც გამოიძახება, როცა მონაცემები აღარავის სჭირდება.
- `void synapse_event_data_acquire(event_data_wrapper_t *wrapper);`
  - ზრდის მითითებების მრიცხველს.
- `void synapse_event_data_release(event_data_wrapper_t *wrapper);`
  - ამცირებს მითითებების მრიცხველს. როცა მრიცხველი 0 გახდება, გამოიძახება `free_fn` და მეხსიერება თავისუფლდება.

---

## Event Handler-ის მაგალითი

```c
// ეს არის მოდულის შიდა ფუნქცია, რომელსაც Event Bus-ი იძახებს.
// მესამე პარამეტრი არის მაჩვენებელი მონაცემთა wrapper-ზე.
static void my_module_handle_event(module_t *self, core_framework_event_id_t event_id, event_data_wrapper_t *wrapper) {
    
    if (event_id == TELEMETRY_EVENT_SENSOR_DATA) {
        // დარწმუნდით, რომ wrapper და მისი მონაცემები არ არის NULL
        if (wrapper && wrapper->data) {
            // უსაფრთხოდ მივიღოთ წვდომა მონაცემებზე
            telemetry_data_t *data = (telemetry_data_t *)wrapper->data;
            ESP_LOGI(TAG, "მივიღე ტემპერატურა: %.2f", data->temperature);
        }
    }

    // ★★★ მნიშვნელოვანია: Event Bus-ი გადასცემს wrapper-ის მფლობელობას (ownership) ★★★
    // ★★★ ამ ფუნქციას. ამიტომ, დამუშავების შემდეგ აუცილებელია synapse_event_data_release-ის გამოძახება, ★★★
    // ★★★ რათა თავიდან ავიცილოთ მეხსიერების გაჟონვა. ★★★
    if (wrapper) {
        synapse_event_data_release(wrapper);
    }
}
```

---

## Custom Event-ების აღწერა

- ყველა custom event-ს უნდა ჰქონდეს უნიკალური იდენტიფიკატორი (მაგ: #define MY_EVENT_ID 0x1001)
- რეკომენდებულია event-ისთვის შესაბამისი მონაცემების სტრუქტურის აღწერა

### მაგალითი Custom Event-ის აღწერისა

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
