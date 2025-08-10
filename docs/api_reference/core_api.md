# Synapse Core API Reference

## ⚙️ Service Locator API (v2.0 - State-Aware)

Service Locator-ი უზრუნველყოფს მოდულებს შორის API-ს მოძიებასა და გამოძახებას იზოლაციის დაცვით. v2.0-დან ის ასევე აღრიცხავს თითოეული სერვისის სიცოცხლის ციკლის სტატუსს.

### `esp_err_t synapse_service_register_with_status(const char *service_name, synapse_service_type_t service_type, service_handle_t service_handle, service_status_t initial_status);`

- **აღწერა:** არეგისტრირებს ახალ სერვისს `Service Locator`-ში მითითებული საწყისი სტატუსით. ეს არის სერვისის რეგისტრაციის **რეკომენდებული** მეთოდი.
- **გამოძახების ადგილი:** მოდულის `_create` ფუნქცია.
- **არგუმენტები:**
  - `service_name`: სერვისის უნიკალური სახელი (როგორც წესი, მოდულის `instance_name`).
  - `service_type`: სერვისის ტიპი `synapse_service_type_t` enum-იდან.
  - `service_handle`: მაჩვენებელი სერვისის API სტრუქტურაზე.
  - `initial_status`: სერვისის საწყისი სტატუსი (როგორც წესი, `SERVICE_STATUS_REGISTERED`).
- **აბრუნებს:** `ESP_OK` წარმატების შემთხვევაში, ან შეცდომის კოდს.

### `esp_err_t synapse_service_set_status(const char *service_name, service_status_t new_status);`

- **აღწერა:** ცვლის უკვე რეგისტრირებული სერვისის სტატუსს. სტატუსის ცვლილებისას, ავტომატურად აქვეყნებს `SYNAPSE_EVENT_SERVICE_STATUS_CHANGED` ივენთს `Event Bus`-ზე.
- **გამოძახების ადგილი:** როგორც წესი, გამოიძახება `System Manager`-ის მიერ მოდულის სიცოცხლის ციკლის სხვადასხვა ეტაპზე.
- **არგუმენტები:**
  - `service_name`: სერვისის სახელი.
  - `new_status`: ახალი სტატუსი `service_status_t` enum-იდან.
- **აბრუნებს:** `ESP_OK` წარმატების შემთხვევაში, `ESP_ERR_NOT_FOUND` თუ სერვისი ვერ მოიძებნა.

### `esp_err_t synapse_service_get_status(const char *service_name, service_status_t *out_status);`

- **აღწერა:** აბრუნებს სერვისის მიმდინარე სტატუსს.
- **არგუმენტები:**
  - `service_name`: სერვისის სახელი.
  - `out_status`: მაჩვენებელი, სადაც ჩაიწერება სერვისის სტატუსი.
- **აბრუნებს:** `ESP_OK` წარმატების შემთხვევაში.

### `service_handle_t synapse_service_get(const char *service_name);`

- **აღწერა:** აბრუნებს სერვისის `handle`-ს (API სტრუქტურის მაჩვენებელს) მისი სახელით.
- **მნიშვნელოვანია:** ეს ფუნქცია აბრუნებს `handle`-ს, თუ სერვისი რეგისტრირებულია, **განურჩევლად მისი სტატუსისა**. `System Manager` (`Strict Mode`-ში) უზრუნველყოფს, რომ მოდული, რომელიც ამ `handle`-ს გამოიყენებს, მხოლოდ მაშინ დაიწყებს მუშაობას, როდესაც სერვისი `ACTIVE` გახდება.

### `esp_err_t synapse_service_unregister(const char *service_name);`

- **აღწერა:** აუქმებს სერვისის რეგისტრაციას.
- **გამოძახების ადგილი:** როგორც წესი, გამოიძახება მოდულის `deinit` ფუნქციაში, თუ სერვისი დინამიურად უნდა მოიხსნას.

### `__attribute__((deprecated))` `esp_err_t synapse_service_register(...)`

- **სტატუსი:** მოძველებული (Deprecated).
- **აღწერა:** ძველი რეგისტრაციის ფუნქცია. უკუთავსებადობისთვის ის კვლავ მუშაობს და შიდა დონეზე იძახებს `synapse_service_register_with_status`-ს `SERVICE_STATUS_REGISTERED` სტატუსით. თავი აარიდეთ მის გამოყენებას ახალ კოდში.

---

## Event Bus API

### `synapse_event_bus_post`

```c
esp_err_t synapse_event_bus_post(const char *event_name, struct event_data_wrapper_t *data_wrapper);
```

- **აღწერა:** ასინქრონულად აქვეყნებს მოვლენას ყველა გამოწერილ მოდულზე.
- **`event_name`:** ივენთის უნიკალური სახელი (სტრიქონი).
- **`data_wrapper`:** მაჩვენებელი "შეფუთულ" მონაცემებზე. თუ მონაცემები არ არის საჭირო, გადაეცემა `NULL`.

### `synapse_event_bus_subscribe`

```c
esp_err_t synapse_event_bus_subscribe(const char *event_name, struct module_t *module);
```

- **აღწერა:** არეგისტრირებს მოდულს, როგორც კონკრეტული ივენთის გამომწერს.
- **`event_name`:** ივენთის სახელი, რომელზეც ხდება გამოწერა. `*` (wildcard) გამოიწერს ყველა ივენთს.
- **`module`:** მაჩვენებელი გამომწერ მოდულზე.

### `synapse_event_bus_unsubscribe`

```c
esp_err_t synapse_event_bus_unsubscribe(const char *event_name, struct module_t *module);
```

- **აღწერა:** აუქმებს მოდულის გამოწერას კონკრეტულ ივენთზე. აუცილებლად უნდა გამოიძახოს მოდულის `deinit` ფუნქციამ.
- **`event_name`:** ივენთის სახელი, რომლიდანაც გამოწერა უნდა მოიხსნას.
- **`module`:** მაჩვენებელი მოდულზე, რომელიც აუქმებს გამოწერას.

---

## Module Register API

### synapse_module_register

```c
esp_err_t synapse_module_register(const char *module_instance_name, module_t *module);
```

- რეგისტრირებს ახალ მოდულს სისტემაში.

### synapse_module_find_by_name

```c
module_t *synapse_module_find_by_name(const char *module_instance_name);
```

- პოულობს მოდულს instance_name-ით.

---

## Module Factory API

### synapse_module_factory_create

```c
module_t *synapse_module_factory_create(const char *type, const cJSON *config);
```

- ქმნის მოდულის ახალ ინსტანციას ტიპისა და კონფიგურაციის მიხედვით.

---

## Configuration Manager API

### synapse_config_get_string

```c
esp_err_t synapse_config_get_string(const char *key, char *value, size_t size);
```

- კითხულობს სტრიქონის ტიპის პარამეტრს კონფიგურაციიდან.

### synapse_config_get_int

```c
esp_err_t synapse_config_get_int(const char *key, int *value);
```

- კითხულობს მთელი რიცხვის ტიპის პარამეტრს.

### synapse_config_get_module_config

```c
const cJSON *synapse_config_get_module_config(const char *module_instance_name);
```

- აბრუნებს მოდულის სრულ კონფიგურაციას instance_name-ით.

---

## Logging API

### DEFINE_COMPONENT_TAG

```c
#define DEFINE_COMPONENT_TAG(NAME)
```

- განსაზღვრავს კომპონენტის უნიკალურ ტეგს ლოგირებისთვის.

### ESP_LOGE, ESP_LOGW, ESP_LOGI, ESP_LOGD, ESP_LOGV

- გამოიყენეთ შესაბამისი log level-ისთვის.
- მაგალითი:

  ```c
  ESP_LOGI(TAG, "Module enabled: %s", module_instance_name);
  ESP_LOGE(TAG, "Error: %s", esp_err_to_name(operation_result));
  ```

---

დამატებითი დეტალებისთვის იხილეთ [convention.md](../convention/convention.md) და [structure.md](../convention/structure.md) დოკუმენტები.
