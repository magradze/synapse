# Synapse Display Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის ეკრანის (display) მოდულების სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## მოდულის შექმნა და სიცოცხლის ციკლი

### module_t *ssd1306_module_create(const cJSON *config);
- ქმნის ახალი SSD1306 მოდულის ინსტანციას გადაცემული კონფიგურაციით.
- არგუმენტები: მოდულის კონფიგურაცია (cJSON ობიექტი)
- აბრუნებს: module_t* წარმატების შემთხვევაში, NULL შეცდომისას.

### static esp_err_t ssd1306_init(module_t *module);
- ახორციელებს მოდულის ინიციალიზაციას (hardware, პარამეტრები და ა.შ.)

### static esp_err_t ssd1306_enable(module_t *module);
- აქტივაციას უკეთებს მოდულს (enable logic)

### static esp_err_t ssd1306_disable(module_t *module);
- დეაქტივაციას უკეთებს მოდულს (disable logic)

### static esp_err_t ssd1306_deinit(module_t *module);
- ასუფთავებს ყველა გამოყოფილ რესურსს

---

## Service API (Service Locator-ისთვის)

### esp_err_t ssd1306_api_enable(void);
- Service Locator-ით გამოძახებული enable ფუნქცია

### esp_err_t ssd1306_api_disable(void);
- Service Locator-ით გამოძახებული disable ფუნქცია

### esp_err_t ssd1306_api_clear(void);
- Service Locator-ით გამოძახებული ეკრანის გასუფთავება

### esp_err_t ssd1306_api_write_text(const char *text, uint8_t line);
- Service Locator-ით გამოძახებული ტექსტის ჩაწერა ეკრანზე
- არგუმენტები: ტექსტი, ხაზის ნომერი

---

## Event Handling API

### static void ssd1306_handle_event(module_t *module, int32_t event_id, void *event_data);
- ამუშავებს მიღებულ მოვლენებს Event Bus-დან (მაგ: refresh, notify)

---

## Configuration Parsing API

### static esp_err_t parse_ssd1306_config(const cJSON *config, ssd1306_private_data_t *ssd1306_data);
- კითხულობს და ამოწმებს მოდულის კონფიგურაციას

---

## Status & Monitoring API

### static module_status_t ssd1306_get_status(module_t *module);
- აბრუნებს მოდულის მიმდინარე სტატუსს

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**ეკრანის ჩართვა:**
```c
ssd1306_api_enable();
```

**ტექსტის ჩაწერა:**
```c
ssd1306_api_write_text("Hello, Synapse!", 0);
```

**ეკრანის გასუფთავება:**
```c
ssd1306_api_clear();
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

