# MQTT ინტეგრაციის სახელმძღვანელო (Integration — MQTT)

---

## მიზანი

ეს დოკუმენტი წარმოადგენს Synapse ESP Framework-ში MQTT ბროკერთან ინტეგრაციის სახელმძღვანელოს naming/structure კონვენციების დაცვით.

---

## ძირითადი ნაბიჯები

1. **MQTT მოდულის ჩართვა და კონფიგურაცია**
   - დაამატეთ mqtt_module system_config.json-ში:
     ```json
     {
         "type": "mqtt_module",
         "enabled": true,
         "config": {
             "instance_name": "main_broker",
             "broker_uri": "mqtt://broker.synapse.local:1883",
             "base_topic": "/synapse/device",
             "username": "user",
             "password": "pass"
         }
     }
     ```
   - ჩართეთ მოდული Kconfig-ში

2. **Publish/Subscribe API-ის გამოყენება**
   - Publish:
     ```c
     mqtt_module_publish("/synapse/device/relay1/state", "{\"state\":\"on\"}");
     ```
   - Subscribe:
     ```c
     esp_err_t mqtt_handle_command(const char *topic, const char *payload) {
         // ... process incoming command ...
         return ESP_OK;
     }
     mqtt_module_subscribe("/synapse/device/relay1/cmd", mqtt_handle_command);
     ```

3. **უსაფრთხოების რეკომენდაციები**
   - გამოიყენეთ TLS/SSL (mqtts://) პროტოკოლი
   - credentials შეინახეთ მხოლოდ კონფიგურაციაში (NVS)
   - არ ჩაწეროთ პაროლები ლოგებში

4. **Debug და ტესტირება**
   - გამოიყენეთ ESP_LOGI/ESP_LOGE MQTT მოდულში
   - დააკვირდით MQTT კავშირის სტატუსს და შეცდომებს

---

## Best Practices

- გამოიყენეთ მხოლოდ public API-ები MQTT მოდულთან მუშაობისას
- დაიცავით naming და structure კონვენციები
- ჩაატარეთ ტესტირება სხვადასხვა ბროკერზე
- გამოიყენეთ ლოგირება და დეტალური კონტექსტი

---

## დაკავშირებული დოკუმენტები

- [integration_index.md](integration_index.md)
- [api_reference/communication_api.md](../api_reference/communication_api.md)
- [configuration_management.md](../convention/configuration_management.md)

---

_დამატებითი კითხვებისთვის იხილეთ შესაბამისი integration დოკუმენტი ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს._

