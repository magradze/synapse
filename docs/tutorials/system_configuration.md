# სისტემის კონფიგურაცია (System Configuration)

## მიზანი

ეს სახელმძღვანელო დაგეხმარებათ სწორად და სტანდარტულად აღწეროთ და მართოთ სისტემის და მოდულების კონფიგურაცია Synapse ESP Framework-ში.

## 1. system_config.json-ის როლი

- მთავარი კონფიგურაციის ფაილი, სადაც აღწერილია ყველა მოდული, მათი პარამეტრები და გლობალური პარამეტრები
- გამოიყენება როგორც ბილდის, ასევე runtime-ის დროს

## 2. სტრუქტურა და მაგალითი

```json
{
    "firmware": {
        "version": "1.0.0",
        "description": "My first ESP project"
    },
    "global_config": {
        "device.id.prefix": "SYNAPSE_DEVICE_UNIQUE_ID"
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

## 3. მოდულის დამატება და პარამეტრების აღწერა

- თითოეული მოდული აღწერილია modules მასივში
- აუცილებელია უნიკალური `instance_name` და ყველა საჭირო პარამეტრი
- მაგალითი:
  ```json
  {
      "type": "mqtt_module",
      "enabled": true,
      "config": {
          "instance_name": "main_broker",
          "broker_uri": "mqtt://192.168.1.160:1883",
          "base_topic": "/synapse/device"
      }
  }
  ```

## 4. გლობალური პარამეტრების გამოყენება

- გამოიყენეთ `global_config` სექცია საერთო მნიშვნელობებისთვის
- წვდომა ხდება fmw_config_get_* API-ებით
- მაგალითი:
  ```c
  char device_prefix[32];
  fmw_config_get_string("global_config.device.id.prefix", device_prefix, sizeof(device_prefix));
  ```

## 5. კონფიგურაციის ვალიდაცია და შეცდომების თავიდან აცილება

- ყველა მოდული ვალიდაციას გადის parse_module_config ფუნქციაში
- არასწორი ან არარსებული მნიშვნელობების შემთხვევაში გამოიყენება default მნიშვნელობები
- ყველა შეცდომა ლოგირდება

## 6. Best Practices

- გამოიყენეთ მხოლოდ დესკრიპტიული და კონტექსტური სახელები
- არასდროს გამოიყენოთ hardcoded მნიშვნელობები კოდში
- ყველა ცვლილება და ახალი პარამეტრი აღწერეთ დოკუმენტაციაში
- გამოიყენეთ [configuration_management.md](../convention/configuration_management.md) და [module_structure.md](../convention/module_structure.md)

---

თუ შეგექმნათ პრობლემა, იხილეთ [troubleshooting](../troubleshooting/) ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს.

