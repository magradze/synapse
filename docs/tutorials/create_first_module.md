# პირველი საკუთარი მოდულის შექმნა (Create Your First Module)

## მიზანი

ეს სახელმძღვანელო გაჩვენებთ, როგორ შექმნათ ახალი მოდული Synapse ESP Framework-ში naming, structure და communication კონვენციების დაცვით.

## 1. საჭიროების განსაზღვრა

- განსაზღვრეთ, რა ფუნქციონალი გჭირდებათ (მაგ: ახალი სენსორი, აქტუატორი, კომუნიკაცია)
- აირჩიეთ შესაბამისი კატეგორია (მაგ: sensors, actuators, communications)

## 2. საქაღალდის და ფაილების შექმნა

- შექმენით ახალი საქაღალდე: `components/modules/{category}/{new_module}/`
- დაამატეთ შემდეგი ფაილები:
  - `CMakeLists.txt`
  - `module.json`
  - `README.md`
  - `include/{new_module}.h`
  - `src/{new_module}.c`

## 3. Header და Source ფაილების შაბლონები

**Header (`include/{new_module}.h`):**

```c
#ifndef {MODULE_NAME}_H
#define {MODULE_NAME}_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

typedef struct {
    esp_err_t (*enable)(void);
    esp_err_t (*disable)(void);
    // საჭიროების მიხედვით სხვა API ფუნქციები
}} {module_name}_api_t;

module_t *{module_name}_create(const cJSON *config);
esp_err_t {module_name}_api_enable(void);
esp_err_t {module_name}_api_disable(void);

#endif // {MODULE_NAME}_H
```

**Source (`src/{new_module}.c`):**

```c
#include "{module_name}.h"
#include "service_locator.h"
#include "event_bus.h"
#include "config_manager.h"
#include "logging.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

DEFINE_COMPONENT_TAG("{MODULE_NAME}");

static module_t *global_{module_name}_instance = NULL;
static {module_name}_api_t {module_name}_service_api = {
    .enable = {module_name}_api_enable,
    .disable = {module_name}_api_disable,
    // საჭიროების მიხედვით სხვა ფუნქციები
};

typedef struct {
    char module_instance_name[32];
    // კონფიგურაციის პარამეტრები
    // runtime ცვლადები
    // hardware handles
}} {module_name}_private_data_t;

static esp_err_t {module_name}_deinit(module_t *self) {
    ESP_LOGI(TAG, "Deinitializing {module_name} module...");

    // ★★★ აუცილებელი ნაბიჯი: გააუქმეთ ყველა გამოწერა! ★★★
    // მაგალითად: fmw_event_bus_unsubscribe(SOME_EVENT_ID, self);

    // ...გაათავისუფლეთ სხვა რესურსები...
    
    free(self); // გაათავისუფლეთ თავად მოდულის ობიექტი
    return ESP_OK;
}

// ...function declarations, implementation...
```

## 4. Naming და Structure კონვენციების გამოყენება

- გამოიყენეთ [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md)
- დაიცავით [module_structure.md](../convention/module_structure.md)
- ყველა ცვლადი და ფუნქცია უნდა იყოს დესკრიპტიური და კონტექსტური

## 5. მოდულის რეგისტრაცია და ჩართვა

- დაამატეთ მოდული `system_config.json`-ში:

  ```json
  {
      "type": "{module_name}",
      "enabled": true,
      "config": {
          "instance_name": "{instance_name}",
          // სხვა საჭირო პარამეტრები
      }
  }
  ```

- ჩართეთ მოდული Kconfig-ში

## 6. ტესტირება და ლოგირება

- გამოიყენეთ ESP-IDF-ის build და flash ბრძანებები
- დააკვირდით ლოგებს (ESP_LOGI, ESP_LOGE და სხვ.)
- დარწმუნდით, რომ ყველა შეცდომა და მნიშვნელოვანი მოვლენა ლოგში ჩანს

## 7. დამატებითი რესურსები

- [module_structure.md](../convention/module_structure.md)
- [configuration_flow.md](../structure/configuration_flow.md)
- [error_handling_and_logging.md](../structure/error_handling_and_logging.md)

---

თუ შეგექმნათ პრობლემა, იხილეთ [troubleshooting](../troubleshooting/) ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს.
