# Core კომპონენტები (Core Components)

---

## მიზანი

ეს დოკუმენტი აღწერს Synapse ESP Framework-ის ბირთვის (core) ძირითად კომპონენტებს, მათ დანიშნულებას, მთავარ ფუნქციებსა და გამოყენების მაგალითებს naming/structure კონვენციების დაცვით.

---

## ძირითადი კომპონენტები

### 1. Service Locator

- **როლი:** უზრუნველყოფს მოდულებს შორის API-ს მოძიებასა და გამოძახებას იზოლაციის დაცვით.
- **მთავარი ფუნქციები:**
  - სერვისის რეგისტრაცია (`fmw_service_register`)
  - სერვისის მოძიება (`fmw_service_get`)
  - სერვისის ტიპის დადგენა (`fmw_service_get_type`)
- **მაგალითი:**

  ```c
  service_handle_t display_service_handle = fmw_service_get("main_display");
  if (display_service_handle) {
      ssd1306_api_t *ssd1306_service_api = (ssd1306_api_t *)display_service_handle;
      ssd1306_service_api->enable();
  }
  ```

### 2. Event Bus

- **როლი:** უზრუნველყოფს broadcast/notification ტიპის კომუნიკაციას მოდულებს შორის.
- **მთავარი ფუნქციები:**
  - მოვლენის გამოქვეყნება (`event_bus_post`)
  - მოვლენის გამოწერა (`event_bus_subscribe`)
- **მაგალითი:**

  ```c
  event_bus_post(TELEMETRY_EVENT_SENSOR_DATA, &sensor_telemetry_data);
  event_bus_subscribe(TELEMETRY_EVENT_SENSOR_DATA, mqtt_handle_sensor_data);
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
  - კონფიგურაციის წაკითხვა (`fmw_config_get_*` ფუნქციები)
  - კონფიგურაციის ვალიდაცია და განახლება
- **მაგალითი:**

  ```c
  char broker_url[128];
  fmw_config_get_string("mqtt_module.config.broker_uri", broker_url, sizeof(broker_url));
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

## ურთიერთქმედება მოდულებთან

- ყველა მოდული core-ის კომპონენტებს იყენებს მხოლოდ მათი public API-ების მეშვეობით.
- პირდაპირი დამოკიდებულება მოდულებს შორის აკრძალულია.
- core უზრუნველყოფს მოდულების რეგისტრაციას, ინიციალიზაციას, კონფიგურაციას და ლოგირებას.

---

## აკრძალული და რეკომენდებული პრაქტიკები

❌ **არასდროს:**

- პირდაპირი #include სხვა მოდულის header-ების
- core-ის კომპონენტების შიდა სტრუქტურებზე წვდომა
- არასტანდარტული ლოგირების ან კონფიგურაციის გამოყენება

✅ **ყოველთვის:**

- გამოიყენეთ მხოლოდ core-ის public API-ები
- დაიცავით naming და structure კონვენციები
- გამოიყენეთ სტანდარტული ლოგირება და კონფიგურაცია

---

_დამატებითი კითხვებისთვის იხილეთ [architecture_overview.md](architecture_overview.md), [module_structure.md](../convention/module_structure.md), [faq.md](../structure/faq.md) ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს._
