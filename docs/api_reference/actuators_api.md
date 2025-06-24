# Synapse Actuators Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის აქტუატორების (actuators) მოდულების სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## Relay Module API

### module_t *relay_module_create(const cJSON *config);
- ქმნის ახალი რელე მოდულის ინსტანციას გადაცემული კონფიგურაციით.
- არგუმენტები: მოდულის კონფიგურაცია (cJSON ობიექტი)
- აბრუნებს: module_t* წარმატების შემთხვევაში, NULL შეცდომისას.

### esp_err_t relay_module_enable(module_t *module);
- აქტივაციას უკეთებს რელეს (enable logic)

### esp_err_t relay_module_disable(module_t *module);
- დეაქტივაციას უკეთებს რელეს (disable logic)

### esp_err_t relay_module_toggle(module_t *module);
- ცვლის რელეს მდგომარეობას (on/off)

### esp_err_t relay_module_set_state(module_t *module, bool state);
- აყენებს რელეს მდგომარეობას (on/off)

### bool relay_module_get_state(module_t *module);
- აბრუნებს რელეს მიმდინარე მდგომარეობას

---

## Event Handling API

### static void relay_handle_event(module_t *module, int32_t event_id, void *event_data);
- ამუშავებს მიღებულ მოვლენებს Event Bus-დან (მაგ: command, notify)

---

## Configuration Parsing API

### static esp_err_t parse_relay_config(const cJSON *config, relay_private_data_t *relay_data);
- კითხულობს და ამოწმებს რელეს კონფიგურაციას

---

## Status & Monitoring API

### static module_status_t relay_get_status(module_t *module);
- აბრუნებს რელეს მიმდინარე სტატუსს

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**რელეს ჩართვა:**
```c
relay_module_enable(module);
```

**რელეს გამორთვა:**
```c
relay_module_disable(module);
```

**რელეს მდგომარეობის შეცვლა:**
```c
relay_module_toggle(module);
relay_module_set_state(module, true); // ჩართვა
relay_module_set_state(module, false); // გამორთვა
```

**მდგომარეობის წაკითხვა:**
```c
bool state = relay_module_get_state(module);
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

