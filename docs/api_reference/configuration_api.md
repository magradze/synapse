# Synapse Configuration Manager API Reference

## 🎯 მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის კონფიგურაციის მენეჯერის (Configuration Manager) API-ს. ეს API უზრუნველყოფს ერთიან, უსაფრთხო და მოქნილ წვდომას სისტემის ყველა კონფიგურაციულ პარამეტრზე `system_config.json`-დან.

---

## 🔑 გასაღების ფორმატი (Dot-Notation)

ყველა `fmw_config_get_*` ფუნქცია იყენებს **წერტილით გამოყოფილ გასაღებს (dot-notation)** პარამეტრების მისაღებად. ფორმატი დამოკიდებულია იმაზე, გლობალურ პარამეტრს ითხოვთ თუ კონკრეტული მოდულისას.

- **გლობალური პარამეტრისთვის:**
  `"global_config.parameter_name"`
  *მაგალითი:* `"global_config.device.id.prefix"`

- **მოდულის პარამეტრისთვის:**
  `"module_instance_name.parameter_name"`
  *მაგალითი:* `"main_broker.broker_uri"` ან `"indoor_sensor.update_interval_sec"`

---

## ⚙️ ძირითადი API ფუნქციები

### esp_err_t fmw_config_get_string(const char *key, char*buffer, size_t buffer_size)

- კითხულობს სტრიქონის ტიპის პარამეტრს კონფიგურაციიდან მითითებული `key`-ს მიხედვით.
- **აბრუნებს:** `ESP_OK` წარმატების შემთხვევაში, `ESP_ERR_NOT_FOUND` თუ გასაღები ვერ მოიძებნა, `ESP_ERR_NVS_TYPE_MISMATCH` თუ ტიპი არასწორია.

### esp_err_t fmw_config_get_int(const char *key, int*out_value)

- კითხულობს მთელი რიცხვის (`int`) ტიპის პარამეტრს.
- **აბრუნებს:** `ESP_OK` წარმატების შემთხვევაში, ან შესაბამის შეცდომის კოდს.

### esp_err_t fmw_config_get_bool(const char *key, bool*out_value)

- კითხულობს ლოგიკური (`bool`) ტიპის პარამეტრს.
- **აბრუნებს:** `ESP_OK` წარმატების შემთხვევაში, ან შესაბამის შეცდომის კოდს.

### esp_err_t fmw_config_save(void)

- ინახავს ყველა ცვლილებას, რომელიც runtime-ში განხორციელდა, NVS მუდმივ მეხსიერებაში.

---

## 🚀 გამოყენების მაგალითები

### გლობალური პარამეტრის წაკითხვა

```c
char device_prefix[32];
esp_err_t ret = fmw_config_get_string("global_config.device.id.prefix", device_prefix, sizeof(device_prefix));
if (ret == ESP_OK) {
    ESP_LOGI(TAG, "Device prefix is: %s", device_prefix);
}
```

### მოდულის პარამეტრის წაკითხვა

```c
// დავუშვათ, ჩვენ ვიმყოფებით მოდულში, რომლის instance_name არის "main_broker"

char broker_uri[128];
esp_err_t ret = fmw_config_get_string("main_broker.broker_uri", broker_uri, sizeof(broker_uri));
if (ret != ESP_OK) {
    // Handle error, maybe use a default value
}

int update_interval;
// ვკითხულობთ სხვა მოდულის ("indoor_sensor") პარამეტრს
ret = fmw_config_get_int("indoor_sensor.update_interval_sec", &update_interval);
if (ret == ESP_OK) {
    ESP_LOGI(TAG, "Sensor update interval: %d seconds", update_interval);
}
```

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.
