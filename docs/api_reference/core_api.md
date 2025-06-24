# Synapse Core API Reference

## Service Locator API

### fmw_service_register
```c
esp_err_t fmw_service_register(const char *service_name, const char *service_type, service_handle_t service_api_handle);
```
- რეგისტრირებს ახალ სერვისს Service Locator-ში.
- არგუმენტები: უნიკალური სახელი, ტიპი, API-ის მაჩვენებელი.
- აბრუნებს: ESP_OK წარმატების შემთხვევაში, ESP_ERR_INVALID_ARG ან ESP_ERR_NO_MEM შეცდომისას.

### fmw_service_get
```c
service_handle_t fmw_service_get(const char *service_name);
```
- აბრუნებს სერვისის API-ს მაჩვენებელს სახელით.

### fmw_service_get_type
```c
const char *fmw_service_get_type(const char *service_name);
```
- აბრუნებს სერვისის ტიპს სახელით.

---

## Event Bus API

### event_bus_post
```c
esp_err_t event_bus_post(int32_t event_id, void *event_data);
```
- ავრცელებს მოვლენას ყველა გამოწერილ მოდულზე.

### event_bus_subscribe
```c
esp_err_t event_bus_subscribe(int32_t event_id, event_handler_t handler);
```
- აძლევს მოდულს შესაძლებლობას მიიღოს კონკრეტული მოვლენა.

---

## Module Register API

### fmw_module_register
```c
esp_err_t fmw_module_register(const char *module_instance_name, module_t *module);
```
- რეგისტრირებს ახალ მოდულს სისტემაში.

### fmw_module_find_by_name
```c
module_t *fmw_module_find_by_name(const char *module_instance_name);
```
- პოულობს მოდულს instance_name-ით.

---

## Module Factory API

### fmw_module_factory_create
```c
module_t *fmw_module_factory_create(const char *type, const cJSON *config);
```
- ქმნის მოდულის ახალ ინსტანციას ტიპისა და კონფიგურაციის მიხედვით.

---

## Configuration Manager API

### fmw_config_get_string
```c
esp_err_t fmw_config_get_string(const char *key, char *value, size_t size);
```
- კითხულობს სტრიქონის ტიპის პარამეტრს კონფიგურაციიდან.

### fmw_config_get_int
```c
esp_err_t fmw_config_get_int(const char *key, int *value);
```
- კითხულობს მთელი რიცხვის ტიპის პარამეტრს.

### fmw_config_get_module_config
```c
const cJSON *fmw_config_get_module_config(const char *module_instance_name);
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

დამატებითი დეტალებისთვის იხილეთ [convention] და [structure] დოკუმენტები.

