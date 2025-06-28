# სისტემის კონფიგურაცია (System Configuration)

## 🎯 მიზანი

ეს სახელმძღვანელო დაგეხმარებათ სწორად და სტანდარტულად აღწეროთ და მართოთ სისტემის და მოდულების კონფიგურაცია Synapse Framework-ში `system_config.json` ფაილის მეშვეობით.

---

## 1. `system_config.json`-ის როლი

- **ცენტრალური კონფიგურაცია:** ეს არის პროექტის მთავარი კონფიგურაციის ფაილი, სადაც აღწერილია ყველა მოდული, მათი პარამეტრები და გლობალური კონფიგურაციები.
- **გამოიყენება:** როგორც build-ის, ასევე runtime-ის დროს, რაც უზრუნველყოფს ერთიან და სტაბილურ გარემოს.

---

## 2. სტრუქტურა და მაგალითი

```json
{
    "firmware": {
        "version": "1.0.0",
        "description": "My First Synapse Project"
    },
    "global_config": {
        "device.id.prefix": "SYNAPSE_DEVICE"
    },
    "modules": [
        {
            "type": "relay_module",
            "enabled": true,
            "config": {
                "instance_name": "relay1",
                "pin": 23
            }
        },
        {
            "type": "dht22_module",
            "enabled": true,
            "config": {
                "instance_name": "indoor_sensor",
                "pin": 4,
                "update_interval_sec": 30
            }
        }
    ]
}
```

---

## 3. მოდულის დამატება და პარამეტრების აღწერა

- თითოეული მოდული აღიწერება `modules` მასივში, როგორც JSON ობიექტი.
- **აუცილებელი ველები:**
  - `type`: მოდულის ტიპი (მაგ., "mqtt_module").
  - `enabled`: `true` ან `false`, რთავს ან თიშავს მოდულს.
  - `config`: ობიექტი, რომელიც შეიცავს მოდულის სპეციფიკურ პარამეტრებს.
  - `instance_name`: `config` ობიექტის შიგნით არსებული **უნიკალური** სახელი, რომელიც ამ მოდულის კონკრეტულ მაგალითს (instance) აღნიშნავს.

- **მაგალითი:**
  ```json
  {
      "type": "mqtt_module",
      "enabled": true,
      "config": {
          "instance_name": "main_broker",
          "broker_uri": "mqtt://192.168.1.100:1883",
          "base_topic": "/synapse/device"
      }
  }
  ```

---

## 4. პარამეტრებზე წვდომა კოდიდან (Dot-Notation)

კონფიგურაციის პარამეტრებზე წვდომა ხდება `fmw_config_get_*` API ფუნქციებით, რომლებიც იყენებენ **წერტილით გამოყოფილ გასაღებს (dot-notation)**.

- **გლობალური პარამეტრისთვის:**
  `"global_config.parameter_name"`

- **მოდულის პარამეტრისთვის:**
  `"module_instance_name.parameter_name"`

- **მაგალიტები:**
  ```c
  // გლობალური პარამეტრის წაკითხვა
  char device_prefix[32];
  esp_err_t ret = fmw_config_get_string("global_config.device.id.prefix", device_prefix, sizeof(device_prefix));
  if (ret == ESP_OK) {
      ESP_LOGI(TAG, "Device prefix: %s", device_prefix);
  } else {
      ESP_LOGE(TAG, "Failed to read device prefix, using default.");
      // შეცდომის დამუშავება, მაგალითად, default მნიშვნელობის მინიჭება
  }

  // მოდულის პარამეტრის წაკითხვა
  int update_interval;
  ret = fmw_config_get_int("indoor_sensor.update_interval_sec", &update_interval);
  if (ret == ESP_OK) {
      ESP_LOGI(TAG, "Sensor update interval: %d seconds", update_interval);
  } else {
      ESP_LOGW(TAG, "Update interval not found for 'indoor_sensor', using default.");
      // შეცდომის დამუშავება, მაგალითად, default მნიშვნელობის მინიჭება
  }
  ```

---

## 5. კონფიგურაციის ვალიდაცია

- **ავტომატური ვალიდაცია:** Framework-ი ავტომატურად ამოწმებს `system_config.json`-ის სტრუქტურას და მოდულების კონფიგურაციას გაშვებისას.
- **Default მნიშვნელობები:** თუ რომელიმე პარამეტრი არასწორია ან არ არსებობს, მოდულმა უნდა გამოიყენოს წინასწარ განსაზღვრული (default) მნიშვნელობა.
- **ლოგირება:** ყველა შეცდომა და გაფრთხილება ლოგირდება, რაც ამარტივებს პრობლემის აღმოჩენას.

---

## 6. საუკეთესო პრაქტიკები (Best Practices)

- **აღწერითი სახელები:** გამოიყენეთ მხოლოდ დესკრიპტიული და კონტექსტური სახელები პარამეტრებისთვის.
- **Hardcoded მნიშვნელობების თავიდან არიდება:** არასდროს გამოიყენოთ hardcoded მნიშვნელობები კოდში. ყველა პარამეტრი უნდა იყოს `system_config.json`-ში.
- **დოკუმენტაცია:** ყველა ახალი პარამეტრი და ცვლილება დეტალურად აღწერეთ მოდულის დოკუმენტაციაში.
- **კონვენციების დაცვა:** დაიცავით პროექტში მიღებული [კონფიგურაციის მართვის](../convention/configuration_management.md) და [მოდულის სტრუქტურის](../convention/module_structure.md) წესები.

---

თუ შეგექმნათ პრობლემა, იხილეთ [Troubleshooting](../troubleshooting/) დოკუმენტაცია.

