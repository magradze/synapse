# Synapse Custom API Reference

## მიზანი

ეს დოკუმენტი განკუთვნილია მომხმარებლის მიერ დამატებული ან გაფართოებული (custom) API-ების აღწერისთვის, naming და structure კონვენციების დაცვით.

---

## Custom Module API

### module_t *custom_module_create(const cJSON *config);
- ქმნის ახალი custom მოდულის ინსტანციას.

### esp_err_t custom_module_do_action(module_t *module, const char *action, const void *params);
- ასრულებს მომხმარებლის მიერ განსაზღვრულ მოქმედებას.

### esp_err_t custom_module_get_status(module_t *module, custom_status_t *status);
- აბრუნებს custom მოდულის მიმდინარე სტატუსს.

---

## Custom Event API

### Custom Event-ის იდენტიფიკატორი
```c
#define CUSTOM_EVENT_ID 0x4001
```
- უნიკალური იდენტიფიკატორი custom event-ისთვის.

### Custom Event-ის მონაცემების სტრუქტურა
```c
typedef struct {
    int value;
    char description[64];
} custom_event_data_t;
```
- custom event-ის მონაცემების სტრუქტურა.

### static void custom_handle_event(module_t *module, int32_t event_id, void *event_data);
- ამუშავებს custom event-ს.

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**Custom მოქმედების შესრულება:**
```c
custom_module_do_action(module, "reset", NULL);
```

**Custom event-ის გამოქვეყნება:**
```c
custom_event_data_t my_event = { .value = 42, .description = "Test" };
event_bus_post(CUSTOM_EVENT_ID, &my_event);
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.
