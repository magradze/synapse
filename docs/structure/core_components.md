# Core კომპონენტები (Core Components)

---

## მიზანი

ეს დოკუმენტი აღწერს Synapse ESP Framework-ის ბირთვის (core) ძირითად კომპონენტებს, მათ დანიშნულებას, მთავარ ფუნქციებსა და გამოყენების მაგალითებს naming/structure კონვენციების დაცვით.

---

## ძირითადი კომპონენტები

### 1. Service Locator

- **როლი:** უზრუნველყოფს მოდულებს შორის API-ს მოძიებასა და გამოძახებას იზოლაციის დაცვით.
- **მთავარი ფუნქციები:**
  - სერვისის რეგისტრაცია (`synapse_service_register`)
  - სერვისის მოძიება (`synapse_service_get`)
  - სერვისის ტიპის დადგენა (`synapse_service_get_type`)
- **მაგალითი:**

  ```c
  service_handle_t display_service_handle = synapse_service_get("main_display");
  if (display_service_handle) {
      ssd1306_api_t *ssd1306_service_api = (ssd1306_api_t *)display_service_handle;
      ssd1306_service_api->enable();
  }
  ```

### 2. Event Bus

- **როლი:** უზრუნველყოფს broadcast/notification ტიპის კომუნიკაციას მოდულებს შორის.
- **მთავარი ფუნქციები:**
  - მოვლენის გამოქვეყნება (`synapse_event_bus_post`)
  - მოვლენის გამოწერა (`synapse_event_bus_subscribe`)
- **მაგალი-თი:**

  ```c
  synapse_event_bus_post(TELEMETRY_EVENT_SENSOR_DATA, &sensor_telemetry_data_wrapper);
  synapse_event_bus_subscribe(TELEMETRY_EVENT_SENSOR_DATA, mqtt_module_instance);
  ```

### 3. Module Register

- **როლი:** უზრუნველყოფს მოდულების რეგისტრაციას, ინიციალიზაციას და მართვას.
- **მთავარი ფუნქციები:**
  - მოდულის რეგისტრაცია
  - მოდულის ძიება სახელით
  - მოდულის სტატუსის მიღება

### 4. Module Factory

- **როლი:** უზრუნველყოფს მოდულების დინამიურ შექმნას და ინიციალიზაციას კონფიგურაციის მიხედვით.
- **მთავარი ფუნქციები:**
  - მოდულის ობიექტის შექმნა კონფიგურაციიდან
  - მოდულის ტიპის მიხედვით შესაბამისი create ფუნქციის გამოძახება

### 5. Configuration Manager

- **როლი:** უზრუნველყოფს სისტემის და მოდულების კონფიგურაციის სტანდარტიზებულ მართვას.
- **მთავარი ფუნქციები:**
  - კონფიგურაციის წაკითხვა (`synapse_config_get_*` ფუნქციები)
  - კონფიგურაციის ვალიდაცია და განახლება
- **მაგალითი:**

  ```c
  char broker_url[128];
  synapse_config_get_string("main_broker.broker_uri", broker_url, sizeof(broker_url));
  ```

### 6. Logging

- **როლი:** უზრუნველყოფს სტანდარტიზებულ და კონტექსტურ ლოგირებას ყველა კომპონენტსა და მოდულში.
- **მთავარი ფუნქციები:**
  - კომპონენტის ტეგის განსაზღვრა (`DEFINE_COMPONENT_TAG`)
  - სხვადასხვა log level-ის გამოყენება (`ESP_LOGE`, `ESP_LOGW`, `ESP_LOGI`, `ESP_LOGD`, `ESP_LOGV`)
- **მაგალითი:**

  ```c
  DEFINE_COMPONENT_TAG("MQTT_MODULE");
  ESP_LOGI(TAG, "MQTT connected to broker: %s", broker_url);
  ```

---

## ფასადის ინტერფეისი (`synapse.h`)

`core` კომპონენტის ყველა ზემოთ ჩამოთვლილი ნაწილი და მათი საჯარო API გაერთიანებულია ერთ, ცენტრალურ ჰედერ ფაილში — `synapse.h`. ეს "ფასადის" პატერნი უზრუნველყოფს, რომ მოდულებს ჰქონდეთ ერთი, სტაბილური და მარტივი წვდომის წერტილი ფრეიმვორქის ბირთვის ფუნქციონალთან.

---

## ურთიერთქმედება მოდულებთან

- ყველა მოდული `core`-ის კომპონენტებს იყენებს ერთიანი `synapse.h` ჰედერის და მისი public API-ების მეშვეობით.
- პირდაპირი დამოკიდებულება მოდულებს შორის აკრძალულია.
- `core` უზრუნველყოფს მოდულების რეგისტრაციას, ინიციალიზაციას, კონფიგურაციას და ლოგირებას.

---

## აკრძალული და რეკომენდებული პრაქტიკები

❌ **არასდროს:**

- პირდაპირი `#include` სხვა მოდულის header-ების.
- `core`-ის კომპონენტების შიდა სტრუქტურებზე წვდომა.
- არასტანდარტული ლოგირების ან კონფიგურაციის გამოყენება.

✅ **ყოველთვის:**

- გამოიყენეთ მხოლოდ `#include "synapse.h"` `core`-ის ფუნქციონალზე წვდომისთვის.
- დაიცავით naming და structure კონვენციები.
- გამოიყენეთ სტანდარტული ლოგირება და კონფიგურაცია.

---

_დამატებითი კითხვებისთვის იხილეთ [architecture_overview.md](../design_decisions/architecture_overview.md), [module_structure.md](../convention/module_structure.md), [faq.md](../structure/faq.md) ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს._
