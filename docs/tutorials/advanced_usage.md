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

სისტემური ივენთების გარდა, შეგიძლიათ შექმნათ და გამოიყენოთ საკუთარი, აპლიკაციისთვის სპეციფიური ივენთები.

**ნაბიჯი 1: ივენთის სახელის განსაზღვრა**

შექმენით უნიკალური სტრიქონი თქვენი ივენთისთვის. რეკომენდებულია მისი `#define`-ით განსაზღვრა ცალკე ჰედერ ფაილში.

```c
// file: my_app_events.h
#define MY_COOL_EVENT "MyCoolEvent"
#define ANOTHER_EVENT "DeviceStateChanged"
```

**ნაბიჯი 2: ივენთის გამოქვეყნება (Posting)**

გამოიყენეთ `fmw_event_bus_post` ფუნქცია თქვენი ივენთის გასაგზავნად.

```c
#include "my_app_events.h"
#include "event_bus.h"

void do_something_and_notify() {
    // ... რაიმე ლოგიკა ...
    ESP_LOGI(TAG, "Posting my cool event!");
    // მეორე პარამეტრი (payload) არის NULL, რადგან მონაცემებს არ ვაგზავნით.
    fmw_event_bus_post(MY_COOL_EVENT, NULL);
}
```

**ნაბიჯი 3: ივენთის გამოწერა და დამუშავება**

სხვა მოდულს შეუძლია გამოიწეროს ეს ივენთი და დაამუშავოს ის `handle_event` ფუნქციაში, როგორც ეს "პირველი მოდულის შექმნის" გაკვეთილშია ნაჩვენები.

## 5. ასინქრონული ოპერაციების მართვა (Promise Pattern)

`Event Bus` იდეალურია "Broadcast" ტიპის შეტყობინებებისთვის, მაგრამ როდესაც ერთ მოდულს სჭირდება მეორისგან კონკრეტული პასუხის მიღება, `Promise` პატერნი ბევრად სუფთა და ლოგიკურ ალტერნატივას გვთავაზობს.

**სცენარი:** `ui_manager`-ს სურს, პერიოდულად განაახლოს WiFi-ს სტატუსის იკონკა. ამისთვის, მან `wifi_manager`-ს უნდა გაუგზავნოს მოთხოვნა და მიიღოს პასუხი.

### 5.1. სერვისის მომწოდებლის API (`wifi_manager`)

`wifi_manager` თავის `wifi_interface.h`-ში აცხადებს ასინქრონულ ფუნქციას:

```c
// wifi_interface.h
esp_err_t (*get_status_async)(void* context, promise_then_cb then_cb, promise_catch_cb catch_cb, void* user_context);
```

ეს ფუნქცია არ აბრუნებს შედეგს პირდაპირ. ამის ნაცვლად, ის იღებს `callback` ფუნქციებს, რომლებსაც გამოიძახებს ოპერაციის დასრულების შემდეგ.

### 5.2. მომხმარებლის ლოგიკა (`ui_manager`)

`ui_manager` ახორციელებს შემდეგ ნაბიჯებს:

**ნაბიჯი 1: `callback` ფუნქციების შექმნა**

`ui_manager` ქმნის ორ ფუნქციას: ერთს წარმატების შემთხვევისთვის, მეორეს — შეცდომისთვის.

```c
// ui_events.c
static void on_wifi_status_received(void* result_data, void* user_context) {
    module_t* self = (module_t*)user_context;
    ui_manager_private_data_t* private_data = (ui_manager_private_data_t*)self->private_data;
    char* json_string = (char*)result_data;

    // ... აქ ხდება JSON-ის პარსინგი და private_data-ს ველების განახლება ...
    
    // ვაგზავნით შიდა ივენთს UI-ს გადასახატად
    fmw_event_bus_post("UI_HOME_UPDATE", NULL);
}

static void on_wifi_status_failed(void* error_data, void* user_context) {
    ESP_LOGE(TAG, "Promise rejected: Failed to get WiFi status.");
}
```

**ნაბიჯი 2: ასინქრონული API-ს გამოძახება**

პერიოდული ტაიმერის ივენთის მიღებისას, `ui_manager` იძახებს `wifi_manager`-ის API-ს და გადასცემს თავის `callback`-ებს.

```c
// ui_events.c -> ui_events_handle()
else if (strcmp(event_name, WIFI_STATUS_TIMER_EVENT) == 0)
{
    if (private_data->wifi_service && private_data->wifi_module_handle)
    {
        private_data->wifi_service->get_status_async(
            private_data->wifi_module_handle, // კონტექსტი
            on_wifi_status_received,          // then_cb
            on_wifi_status_failed,            // catch_cb
            self                              // user_context
        );
    }
}
```

**შედეგი:**
ამ მიდგომით, `ui_manager`-ის კოდი, რომელიც WiFi სტატუსს ეხება, ლოგიკურად ერთ ადგილასაა თავმოყრილი. მოთხოვნა და პასუხის დამუშავება ერთმანეთთან პირდაპირ არის დაკავშირებული, რაც კოდს ბევრად უფრო წაკითხვადს და ადვილად სამართავს ხდის.

## 6. მოდულის სტატუსის მონიტორინგი

- გამოიყენეთ get_status API მოდულის მდგომარეობის მისაღებად
- მაგალითი:

  ```c
  module_status_t status = ssd1306_get_status(module);
  if (status == MODULE_STATUS_ENABLED) {
      // ...
  }
  ```

## 7. ოპტიმიზაცია და რესურსების მართვა

- გამოიყენეთ ლოგირება და performance monitoring
- ყურადღება მიაქციეთ მეხსიერების გამოყენებას და task-ების რაოდენობას

## 8. Best Practices

- გამოიყენეთ მხოლოდ public API-ები
- დაიცავით naming და structure კონვენციები
- ჩაატარეთ ტესტირება რთულ სცენარებზე
- გამოიყენეთ დეტალური ლოგირება

---

თუ გაქვთ კითხვები ან გჭირდებათ დახმარება, იხილეთ [faq.md](../structure/faq.md) ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს.
