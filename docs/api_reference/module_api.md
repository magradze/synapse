# Synapse Module API Reference

## Module Creation & Lifecycle

### module_t *{module_name}_module_create(const cJSON *config);
- ქმნის ახალი მოდულის ინსტანციას გადაცემული კონფიგურაციით.
- არგუმენტები: მოდულის კონფიგურაცია (cJSON ობიექტი)
- აბრუნებს: module_t* წარმატების შემთხვევაში, NULL შეცდომისას.

### static esp_err_t {module_name}_init(module_t *module);
- ახორციელებს მოდულის ინიციალიზაციას (hardware, პარამეტრები და ა.შ.)

### static esp_err_t {module_name}_enable(module_t *module);
- აქტივაციას უკეთებს მოდულს (enable logic)

### static esp_err_t {module_name}_disable(module_t *module);
- დეაქტივაციას უკეთებს მოდულს (disable logic)

### static esp_err_t {module_name}_deinit(module_t *module);
- ასუფთავებს ყველა გამოყოფილ რესურსს

---

## Service API (Service Locator-ისთვის)

### esp_err_t {module_name}_api_enable(void);
- Service Locator-ით გამოძახებული enable ფუნქცია

### esp_err_t {module_name}_api_disable(void);
- Service Locator-ით გამოძახებული disable ფუნქცია

### esp_err_t {module_name}_api_clear(void);
- Service Locator-ით გამოძახებული clear ფუნქცია (თუ საჭიროა)

### esp_err_t {module_name}_api_write_text(const char *text, uint8_t line);
- Service Locator-ით გამოძახებული ტექსტის ჩაწერა (მაგალითად, ეკრანზე)

---

## Event Handling API

### static void {module_name}_handle_event(module_t *module, int32_t event_id, void *event_data);
- ამუშავებს მიღებულ მოვლენებს Event Bus-დან

---

## Configuration Parsing API

### static esp_err_t parse_{module_name}_config(const cJSON *config, {module_name}_private_data_t *module_private_data);
- კითხულობს და ამოწმებს მოდულის კონფიგურაციას

---

## Status & Monitoring API

### static module_status_t {module_name}_get_status(module_t *module);
- აბრუნებს მოდულის მიმდინარე სტატუსს

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md) და [structure] დოკუმენტები.

