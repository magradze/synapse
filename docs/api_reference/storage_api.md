# Synapse Storage Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის მეხსიერების (Storage) მოდულების სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## NVS (Non-Volatile Storage) Module API

### module_t *nvs_storage_module_create(const cJSON *config);
- ქმნის ახალი NVS Storage მოდულის ინსტანციას.

### esp_err_t nvs_storage_module_set_string(const char *key, const char *value);
- ინახავს სტრიქონის მნიშვნელობას NVS-ში.

### esp_err_t nvs_storage_module_get_string(const char *key, char *value, size_t size);
- კითხულობს სტრიქონის მნიშვნელობას NVS-დან.

### esp_err_t nvs_storage_module_set_int(const char *key, int value);
- ინახავს მთელი რიცხვის მნიშვნელობას NVS-ში.

### esp_err_t nvs_storage_module_get_int(const char *key, int *value);
- კითხულობს მთელი რიცხვის მნიშვნელობას NVS-დან.

### esp_err_t nvs_storage_module_erase_key(const char *key);
- შლის მითითებულ key-ს NVS-დან.

### esp_err_t nvs_storage_module_erase_all(void);
- შლის ყველა მონაცემს NVS-დან.

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**სტრიქონის შენახვა:**
```c
nvs_storage_module_set_string("device_name", "synapse_node_1");
```

**მთელი რიცხვის წაკითხვა:**
```c
int counter;
nvs_storage_module_get_int("boot_count", &counter);
```

**ყველა მონაცემის წაშლა:**
```c
nvs_storage_module_erase_all();
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

