# Synapse Device Identity Service API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის `Device Identity Service`-ის სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## Device Identity Service API

### `const char* get_device_id(void);`

- **აღწერა:** აბრუნებს მოწყობილობის უნიკალურ ID-ს, რომელიც გენერირებულია MAC მისამართისგან.
- **აბრუნებს:** `const char*` მაჩვენებელს null-ით დაბოლოებულ სტრიქონზე (მაგ., `"5C013B7246E4"`).
- **@note:** დაბრუნებული სტრიქონის მეხსიერების მართვაზე პასუხისმგებელია თავად სერვისი. გამომძახებელმა **არ უნდა** სცადოს მისი გათავისუფლება (`free`).

### `const char* get_firmware_version(void);`

- **აღწერა:** აბრუნებს firmware-ის მიმდინარე ვერსიას, რომელიც აღებულია პროექტის აღწერიდან.
- **აბრუნებს:** `const char*` მაჩვენებელს null-ით დაბოლოებულ სტრიქონზე (მაგ., `"v3.2.1"`).
- **@note:** დაბრუნებული სტრიქონის მეხსიერების მართვაზე პასუხისმგებელია თავად სერვისი.

---

## Naming & Structure

- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**მოწყობილობის ID-ის მიღება:**

```c
service_handle_t id_service = fmw_service_get("main_identity_service");
if (id_service) {
    device_identity_api_t *id_api = (device_identity_api_t *)id_service;
    const char* device_id = id_api->get_device_id();
    ESP_LOGI(TAG, "My unique device ID is: %s", device_id);
}
```

**Firmware ვერსიის მიღება:**

```c
service_handle_t id_service = fmw_service_get("main_identity_service");
if (id_service) {
    device_identity_api_t *id_api = (device_identity_api_t *)id_service;
    const char* fw_version = id_api->get_firmware_version();
    ESP_LOGI(TAG, "Current firmware version: %s", fw_version);
}
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure.md](structure.md) დოკუმენტები.
