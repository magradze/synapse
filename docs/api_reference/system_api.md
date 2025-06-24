# Synapse System Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის სისტემური მოდულების (OTA, Version, Watchdog, Diagnostics) სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## OTA Module API

### module_t *ota_module_create(const cJSON *config);
- ქმნის ახალი OTA მოდულის ინსტანციას.

### esp_err_t ota_module_start_update(const char *url);
- იწყებს OTA განახლებას მითითებული URL-დან.

### esp_err_t ota_module_get_status(ota_status_t *status);
- აბრუნებს მიმდინარე OTA პროცესის სტატუსს.

---

## Version Module API

### module_t *version_module_create(const cJSON *config);
- ქმნის ახალი Version მოდულის ინსტანციას.

### esp_err_t version_module_get_firmware_version(char *version_str, size_t size);
- აბრუნებს მიმდინარე firmware-ის ვერსიას.

---

## Watchdog Module API

### module_t *watchdog_module_create(const cJSON *config);
- ქმნის ახალი Watchdog მოდულის ინსტანციას.

### esp_err_t watchdog_module_feed(void);
- აძლევს Watchdog-ს სიგნალს (feed).

### esp_err_t watchdog_module_set_timeout(uint32_t timeout_ms);
- აყენებს Watchdog-ის timeout-ს მილიწამებში.

---

## Diagnostics Module API

### module_t *diagnostics_module_create(const cJSON *config);
- ქმნის ახალი Diagnostics მოდულის ინსტანციას.

### esp_err_t diagnostics_module_run_tests(void);
- ასრულებს სისტემურ დიაგნოსტიკურ ტესტებს.

### esp_err_t diagnostics_module_get_report(char *report_buffer, size_t buffer_size);
- აბრუნებს ტესტების შედეგების რეპორტს.

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

**Firmware ვერსიის მიღება:**
```c
char version[32];
version_module_get_firmware_version(version, sizeof(version));
```

**Watchdog feed:**
```c
watchdog_module_feed();
```

**Diagnostics ტესტების გაშვება:**
```c
diagnostics_module_run_tests();
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

