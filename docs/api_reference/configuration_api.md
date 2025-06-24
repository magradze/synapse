# Synapse Configuration Manager API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის კონფიგურაციის მენეჯერის (Configuration Manager) API-ს, naming და structure კონვენციების დაცვით.

---

## ძირითადი API ფუნქციები

### esp_err_t fmw_config_get_string(const char *key, char *value, size_t size);
- კითხულობს სტრიქონის ტიპის პარამეტრს კონფიგურაციიდან.

### esp_err_t fmw_config_get_int(const char *key, int *value);
- კითხულობს მთელი რიცხვის ტიპის პარამეტრს.

### esp_err_t fmw_config_get_bool(const char *key, bool *value);
- კითხულობს ლოგიკური ტიპის პარამეტრს.

### const cJSON *fmw_config_get_module_config(const char *module_instance_name);
- აბრუნებს მოდულის სრულ კონფიგურაციას instance_name-ით.

### esp_err_t fmw_config_set_string(const char *key, const char *value);
- ცვლის ან ამატებს სტრიქონის ტიპის პარამეტრს.

### esp_err_t fmw_config_set_int(const char *key, int value);
- ცვლის ან ამატებს მთელი რიცხვის ტიპის პარამეტრს.

### esp_err_t fmw_config_set_bool(const char *key, bool value);
- ცვლის ან ამატებს ლოგიკური ტიპის პარამეტრს.

### esp_err_t fmw_config_save(void);
- ინახავს ყველა ცვლილებას მუდმივ მეხსიერებაში.

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**სტრიქონის წაკითხვა:**
```c
char broker_url[128];
fmw_config_get_string("mqtt_module.config.broker_uri", broker_url, sizeof(broker_url));
```

**მთელი რიცხვის წაკითხვა:**
```c
int update_interval;
fmw_config_get_int("dht22_module.config.update_interval_sec", &update_interval);
```

**პარამეტრის შეცვლა და შენახვა:**
```c
fmw_config_set_string("relay_module.config.instance_name", "relay2");
fmw_config_save();
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

