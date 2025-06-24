# Synapse OTA (Over-the-Air) Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის OTA (Over-the-Air update) მოდულის სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## OTA Module API

### module_t *ota_module_create(const cJSON *config);
- ქმნის ახალი OTA მოდულის ინსტანციას გადაცემული კონფიგურაციით.

### esp_err_t ota_module_start_update(const char *url);
- იწყებს OTA განახლებას მითითებული URL-დან.
- არგუმენტები: firmware-ის ბმული (URL)

### esp_err_t ota_module_get_status(ota_status_t *status);
- აბრუნებს მიმდინარე OTA პროცესის სტატუსს.

### esp_err_t ota_module_abort(void);
- აჩერებს მიმდინარე OTA განახლებას.

---

## Event Handling API

### static void ota_handle_event(module_t *module, int32_t event_id, void *event_data);
- ამუშავებს OTA მოდულის მიერ მიღებულ მოვლენებს (მაგ: update started, update finished, error)

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**OTA განახლების დაწყება:**
```c
ota_module_start_update("https://update.synapse.local/firmware.bin");
```

**OTA სტატუსის მიღება:**
```c
ota_status_t status;
ota_module_get_status(&status);
```

**OTA განახლების შეწყვეტა:**
```c
ota_module_abort();
```

---

დამატებითი დეტალებისთვის იხილეთ [system_api.md](system_api.md), [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

