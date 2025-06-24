# Synapse Time Sync Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის დროის სინქრონიზაციის (Time Sync, NTP/SNTP) მოდულის სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## Time Sync Module API

### module_t *time_sync_module_create(const cJSON *config);
- ქმნის ახალი Time Sync მოდულის ინსტანციას.

### esp_err_t time_sync_module_start(module_t *module);
- იწყებს დროის სინქრონიზაციის პროცესს.

### esp_err_t time_sync_module_stop(module_t *module);
- აჩერებს დროის სინქრონიზაციას.

### esp_err_t time_sync_module_set_server(module_t *module, const char *server_url);
- აყენებს NTP/SNTP სერვერის მისამართს.

### esp_err_t time_sync_module_get_time(time_t *current_time);
- აბრუნებს სინქრონიზებულ დროს.

---

## Event Handling API

### static void time_sync_handle_event(module_t *module, int32_t event_id, void *event_data);
- ამუშავებს Time Sync მოდულის მიერ მიღებულ მოვლენებს (მაგ: sync complete, error)

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**დროის სინქრონიზაციის დაწყება:**
```c
time_sync_module_start(module);
```

**NTP სერვერის დაყენება:**
```c
time_sync_module_set_server(module, "pool.ntp.org");
```

**სინქრონიზებული დროის მიღება:**
```c
time_t now;
time_sync_module_get_time(&now);
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

