# Synapse Sensor Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის სენსორების (sensor) მოდულების სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## მოდულის შექმნა და სიცოცხლის ციკლი

### module_t *dht22_module_create(const cJSON *config);
- ქმნის ახალი DHT22 სენსორის მოდულის ინსტანციას გადაცემული კონფიგურაციით.
- არგუმენტები: მოდულის კონფიგურაცია (cJSON ობიექტი)
- აბრუნებს: module_t* წარმატების შემთხვევაში, NULL შეცდომისას.

### static esp_err_t dht22_init(module_t *module);
- ახორციელებს სენსორის ინიციალიზაციას (hardware, პარამეტრები და ა.შ.)

### static esp_err_t dht22_enable(module_t *module);
- აქტივაციას უკეთებს სენსორს (enable logic)

### static esp_err_t dht22_disable(module_t *module);
- დეაქტივაციას უკეთებს სენსორს (disable logic)

### static esp_err_t dht22_deinit(module_t *module);
- ასუფთავებს ყველა გამოყოფილ რესურსს

---

## Event Handling API

### static void dht22_handle_event(module_t *module, int32_t event_id, void *event_data);
- ამუშავებს მიღებულ მოვლენებს Event Bus-დან (მაგ: refresh, notify)

### static esp_err_t publish_sensor_data(dht22_private_data_t *dht22_data, float temperature, float humidity);
- ავრცელებს სენსორის მონაცემებს Event Bus-ზე

---

## Configuration Parsing API

### static esp_err_t parse_dht22_config(const cJSON *config, dht22_private_data_t *dht22_data);
- კითხულობს და ამოწმებს სენსორის კონფიგურაციას

---

## Status & Monitoring API

### static module_status_t dht22_get_status(module_t *module);
- აბრუნებს სენსორის მიმდინარე სტატუსს

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**სენსორის ჩართვა:**
```c
dht22_enable(module);
```

**მონაცემების Event Bus-ზე გამოქვეყნება:**
```c
publish_sensor_data(dht22_data, 23.5, 65.2);
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

