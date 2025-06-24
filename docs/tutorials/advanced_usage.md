# Advanced Usage (გაფართოებული გამოყენება)

## მიზანი

ეს სახელმძღვანელო გაჩვენებთ, როგორ გამოიყენოთ Synapse Framework-ის გაფართოებული შესაძლებლობები, როგორ დააკონფიგურიროთ რთული სცენარები და როგორ მოახდინოთ სისტემის ოპტიმიზაცია.

## 1. მრავალმოდულიანი კონფიგურაცია
- ერთზე მეტი ერთი და იმავე ტიპის მოდულის დამატება (მაგ: რამდენიმე რელე, რამდენიმე სენსორი)
- მაგალითი:
  ```json
  {
      "type": "relay_module",
      "enabled": true,
      "config": {
          "instance_name": "relay1",
          "pin": 23
      }
  },
  {
      "type": "relay_module",
      "enabled": true,
      "config": {
          "instance_name": "relay2",
          "pin": 25
      }
  }
  ```

## 2. მოდულის დინამიური ჩართვა/გამორთვა
- მოდულის enable/disable ფუნქციების გამოყენება runtime-ზე
- მაგალითი:
  ```c
  // მოდულის ჩართვა
  ssd1306_api_enable();
  // მოდულის გამორთვა
  ssd1306_api_disable();
  ```

## 3. Runtime Reconfiguration
- მოდულის კონფიგურაციის შეცვლა მუშაობის დროს
- გამოიყენეთ module_reconfigure API
- მაგალითი:
  ```c
  cJSON *new_config = ...; // ახალი კონფიგურაცია
  module_reconfigure(module, new_config);
  ```

## 4. Custom Event-ების გამოყენება
- საკუთარი მოვლენის განსაზღვრა და გამოყენება Event Bus-ში
- მაგალითი:
  ```c
  #define CUSTOM_EVENT_ID  0x1001
  event_bus_post(CUSTOM_EVENT_ID, &custom_data);
  event_bus_subscribe(CUSTOM_EVENT_ID, custom_event_handler);
  ```

## 5. მოდულის სტატუსის მონიტორინგი
- გამოიყენეთ get_status API მოდულის მდგომარეობის მისაღებად
- მაგალითი:
  ```c
  module_status_t status = ssd1306_get_status(module);
  if (status == MODULE_STATUS_ENABLED) {
      // ...
  }
  ```

## 6. ოპტიმიზაცია და რესურსების მართვა
- გამოიყენეთ ლოგირება და performance monitoring
- ყურადღება მიაქციეთ მეხსიერების გამოყენებას და task-ების რაოდენობას

## 7. Best Practices
- გამოიყენეთ მხოლოდ public API-ები
- დაიცავით naming და structure კონვენციები
- ჩაატარეთ ტესტირება რთულ სცენარებზე
- გამოიყენეთ დეტალური ლოგირება

---

თუ გაქვთ კითხვები ან გჭირდებათ დახმარება, იხილეთ [faq.md](../structure/faq.md) ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს.

