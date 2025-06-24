# Cloud ინტეგრაციის სახელმძღვანელო (Integration — Cloud)

---

## მიზანი

ეს დოკუმენტი წარმოადგენს Synapse ESP Framework-ში Cloud სერვისებთან ინტეგრაციის სახელმძღვანელოს naming/structure კონვენციების დაცვით.

---

## ძირითადი ნაბიჯები

1. **Cloud მოდულის ან ინტეგრაციის არჩევა**
   - აირჩიეთ შესაბამისი cloud სერვისი (მაგ: AWS IoT, Azure IoT, Google Cloud IoT, custom REST/MQTT cloud)
   - ჩართეთ საჭირო მოდული ან შექმენით integration მოდული

2. **Cloud სერვისის კონფიგურაცია**
   - დაამატეთ cloud მოდული system_config.json-ში:
     ```json
     {
         "type": "cloud_module",
         "enabled": true,
         "config": {
             "instance_name": "main_cloud",
             "endpoint": "mqtts://cloud.example.com:8883",
             "auth_token": "<token>"
         }
     }
     ```
   - ჩართეთ მოდული Kconfig-ში

3. **Cloud API-ის გამოყენება**
   - მონაცემების ატვირთვა (publish):
     ```c
     cloud_module_publish("/cloud/device/telemetry", telemetry_json);
     ```
   - მონაცემების მიღება (subscribe):
     ```c
     cloud_module_subscribe("/cloud/device/command", cloud_command_handler);
     ```

4. **უსაფრთხოების რეკომენდაციები**
   - გამოიყენეთ მხოლოდ დაშიფრული კავშირი (TLS/SSL)
   - გამოიყენეთ სერტიფიკატები ან access token-ები
   - არ ჩაწეროთ credentials ლოგებში

5. **Debug და ტესტირება**
   - გამოიყენეთ ESP_LOGI/ESP_LOGE Cloud მოდულში
   - დააკვირდით კავშირის სტატუსს და შეცდომებს

---

## Best Practices

- გამოიყენეთ მხოლოდ public API-ები Cloud ინტეგრაციისთვის
- დაიცავით naming და structure კონვენციები
- ჩაატარეთ ტესტირება სხვადასხვა cloud სერვისზე
- გამოიყენეთ ლოგირება და დეტალური კონტექსტი

---

## დაკავშირებული დოკუმენტები

- [integration_index.md](integration_index.md)
- [api_reference/communication_api.md](../api_reference/communication_api.md)
- [configuration_management.md](../convention/configuration_management.md)

---

_დამატებითი კითხვებისთვის იხილეთ შესაბამისი integration დოკუმენტი ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს._

