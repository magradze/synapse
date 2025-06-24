# Synapse Health Monitor Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის Health Monitor მოდულის სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## Health Monitor Module API

### module_t *health_monitor_module_create(const cJSON *config);
- ქმნის ახალი Health Monitor მოდულის ინსტანციას.

### esp_err_t health_monitor_module_start(module_t *module);
- იწყებს სისტემის ჯანმრთელობის მონიტორინგს.

### esp_err_t health_monitor_module_stop(module_t *module);
- აჩერებს მონიტორინგს.

### esp_err_t health_monitor_module_get_status(module_t *module, health_status_t *status);
- აბრუნებს მიმდინარე ჯანმრთელობის სტატუსს.

### esp_err_t health_monitor_module_register_check(module_t *module, health_check_fn_t check_fn);
- რეგისტრირებს დამატებით ჯანმრთელობის შემოწმების ფუნქციას.

---

## Event Handling API

### static void health_monitor_handle_event(module_t *module, int32_t event_id, void *event_data);
- ამუშავებს Health Monitor მოდულის მიერ მიღებულ მოვლენებს (მაგ: alert, notify)

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**მონიტორინგის დაწყება:**
```c
health_monitor_module_start(module);
```

**ჯანმრთელობის სტატუსის მიღება:**
```c
health_status_t status;
health_monitor_module_get_status(module, &status);
```

**დამატებითი შემოწმების რეგისტრაცია:**
```c
health_monitor_module_register_check(module, my_custom_check_fn);
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

