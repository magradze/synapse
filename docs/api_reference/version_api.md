# Synapse Version Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის ვერსიის (Version) მოდულის სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## Version Module API

### module_t *version_module_create(const cJSON *config);
- ქმნის ახალი Version მოდულის ინსტანციას გადაცემული კონფიგურაციით.

### esp_err_t version_module_get_firmware_version(char *version_str, size_t size);
- აბრუნებს მიმდინარე firmware-ის ვერსიას.

### esp_err_t version_module_get_build_date(char *date_str, size_t size);
- აბრუნებს firmware-ის build თარიღს.

### esp_err_t version_module_get_build_time(char *time_str, size_t size);
- აბრუნებს firmware-ის build დროს.

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**Firmware ვერსიის მიღება:**
```c
char version[32];
version_module_get_firmware_version(version, sizeof(version));
```

**Build თარიღის მიღება:**
```c
char build_date[32];
version_module_get_build_date(build_date, sizeof(build_date));
```

**Build დროის მიღება:**
```c
char build_time[32];
version_module_get_build_time(build_time, sizeof(build_time));
```

---

დამატებითი დეტალებისთვის იხილეთ [system_api.md](system_api.md), [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

