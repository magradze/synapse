# Synapse Local Control Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის ადგილობრივი კონტროლის (Local Control, Web UI, REST API) მოდულის სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## Local Control Module API

### module_t *local_ctrl_module_create(const cJSON *config);
- ქმნის ახალი Local Control მოდულის ინსტანციას.

### esp_err_t local_ctrl_module_start(module_t *module);
- იწყებს ადგილობრივი კონტროლის სერვისს (მაგ: Web UI, REST API).

### esp_err_t local_ctrl_module_stop(module_t *module);
- აჩერებს ადგილობრივი კონტროლის სერვისს.

### esp_err_t local_ctrl_module_register_endpoint(const char *uri, http_method_t method, local_ctrl_handler_t handler);
- რეგისტრირებს ახალ REST/Web UI endpoint-ს.

---

## Event Handling API

### static void local_ctrl_handle_event(module_t *module, int32_t event_id, void *event_data);
- ამუშავებს Local Control მოდულის მიერ მიღებულ მოვლენებს (მაგ: user action, config update)

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**Web UI სერვისის დაწყება:**
```c
local_ctrl_module_start(module);
```

**REST Endpoint-ის რეგისტრაცია:**
```c
local_ctrl_module_register_endpoint("/api/relay/on", HTTP_POST, relay_on_handler);
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

