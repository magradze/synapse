# მოდული: Watchdog Manager

## 1. მიმოხილვა

**მოდულის სახელი:** `watchdog_manager`  
**კატეგორია:** `system`  
**ვერსია:** 1.0.0  
**ავტორი:** Synapse Framework Team

`Watchdog Manager` მოდული არის Synapse Framework-ის თვითაღდგენის და საიმედოობის კრიტიკული კომპონენტი. მისი მთავარი ამოცანაა, დაიცვას სისტემა პროგრამული უზრუნველყოფის გაჭედვისგან (software freeze/hang).

ტრადიციული watchdog-ებისგან განსხვავებით, რომლებიც უბრალოდ პერიოდულად "იკვებებიან", ეს მოდული ახდენს **"ჭკვიანი" ანუ აპლიკაციაზე მორგებული (application-aware)** watchdog-ის იმპლემენტაციას. ის გადატვირთავს ESP-IDF-ის Task Watchdog Timer-ს (TWDT) მხოლოდ იმ შემთხვევაში, თუ სისტემის **ყველა** კრიტიკულად მნიშვნელოვანი კომპონენტი გამართულად და დროულად ფუნქციონირებს.

## 2. წინაპირობები და `menuconfig`

ამ მოდულის გამართული მუშაობისთვის აუცილებელია ESP-IDF-ის კონფიგურაციაში (`menuconfig`) შემდეგი ოფციების ჩართვა:

1. **Task Watchdog Timer (TWDT):**
    * გზა: `Component config` → `ESP System Settings`
    * ოფცია: `[*] Initialize Task Watchdog Timer on startup` (`CONFIG_ESP_TASK_WDT_INIT`)
    * ოფცია: `Task Watchdog timeout period (seconds)` (`CONFIG_ESP_TASK_WDT_TIMEOUT_S`) - ეს მნიშვნელობა უნდა იყოს უფრო დიდი, ვიდრე ჩვენი მოდულის `check_interval_sec`.

## 3. არქიტექტურული პრინციპები

* **განაწილებული "გულისცემა" (Distributed Heartbeat):** მოდული არ ეყრდნობა ერთ ცენტრალურ ტასკს. ამის ნაცვლად, ის ელოდება "გულისცემის" სიგნალებს სხვადასხვა კრიტიკული მოდულისგან. Watchdog-ი "იკვებება" მხოლოდ მაშინ, როცა ყველა რეგისტრირებული მოდულისგან მიღებულია სიგნალი.
* **Service-driven:** მოდული აწვდის `watchdog_api_t` სერვისს, რომელიც სხვა მოდულებს საშუალებას აძლევს, დაარეგისტრირონ საკუთარი თავი, როგორც "კრიტიკული" კომპონენტი და პერიოდულად გააგზავნონ "გულისცემა".
* **Event-driven:** მოდული გამოიწერს `SYSTEM_HEALTH_ALERT` ივენთს. თუ `Health Monitor`-ისგან მიიღებს გამოუსწორებელი შეცდომის სიგნალს, მას შეუძლია მიზანმიმართულად შეწყვიტოს Watchdog-ის "კვება", რათა გამოიწვიოს სისტემის უსაფრთხო გადატვირთვა.
* **ფენებად დაყოფილი პასუხისმგებლობა:**
    1. **ESP-IDF TWDT:** დაბალი დონის ტაიმერი, რომელიც იწვევს აპარატურულ გადატვირთვას.
    2. **`watchdog_manager`:** მაღალი დონის ლოგიკა, რომელიც წყვეტს, "ვაჭამოთ" თუ არა TWDT-ს.
    3. **სხვა მოდულები:** პასუხისმგებელნი არიან, დროულად გააგზავნონ საკუთარი "გულისცემა".

## 4. კონფიგურაცია

მოდულის კონფიგურაცია ხდება `system_config.json` ფაილის საშუალებით.

**კონფიგურაციის მაგალითი:**

```json
{
    "type": "watchdog_manager",
    "enabled": true,
    "config": {
        "instance_name": "main_watchdog",
        "check_interval_ms": 2000,
        "heartbeat_timeout_ms": 5000
    }
}
```

**კონფიგურაციის პარამეტრები:**

| პარამეტრი | ტიპი | აღწერა | სავალდებულო | Default |
|:---|:---|:---|:---:|:---|
| `instance_name` | სტრიქონი | მოდულის ინსტანციის უნიკალური სახელი. | ❌ | `main_watchdog`|
| `check_interval_ms` | რიცხვი | ინტერვალი (მილიწამებში), რა სიხშირითაც მოდული ამოწმებს მიღებულ "გულისცემებს". | ❌ | `2000` |
| `heartbeat_timeout_ms` | რიცხვი | მაქსიმალური დრო (მილიწამებში), რომლის განმავლობაშიც მოდული ელოდება "გულისცემას" რეგისტრირებული კომპონენტისგან. | ❌ | `5000` |

## 5. Service API (`watchdog_api_t`)

სხვა მოდულებს შეუძლიათ მიიღონ წვდომა Watchdog მენეჯერის API-ზე `Service Locator`-ის მეშვეობით. სერვისის ტიპი: `FMW_SERVICE_TYPE_WATCHDOG_API`.

**API ფუნქციები:**

* **`esp_err_t register_heartbeat(const char *module_name);`**  
  არეგისტრირებს მოდულს, როგორც კრიტიკულ კომპონენტს. ამის შემდეგ, `watchdog_manager` დაიწყებს ამ მოდულისგან "გულისცემის" ლოდინს. ეს ფუნქცია უნდა გამოიძახოს კრიტიკულმა მოდულმა თავის `init` ან `start` ფუნქციაში.

* **`esp_err_t send_heartbeat(const char *module_name);`**  
  რეგისტრირებული მოდული პერიოდულად იძახებს ამ ფუნქციას, რათა ამცნოს `watchdog_manager`-ს, რომ ის "ცოცხალია" და გამართულად მუშაობს.

* **`esp_err_t unregister_heartbeat(const char *module_name);`**  
  შლის მოდულს მონიტორინგის სიიდან. უნდა გამოიძახოს მოდულის `deinit` ფუნქციამ.

## 6. გამოქვეყნებული ივენთები

* **`WATCHDOG_HEARTBEAT_MISSED`**
  * **როდის ქვეყნდება:** როდესაც რომელიმე რეგისტრირებული მოდული ვერ აგზავნის "გულისცემას" `heartbeat_timeout_ms`-ის განმავლობაში.
  * **Payload (cJSON სტრიქონი):** `{"module_name":"main_mqtt_client"}`

* **`WATCHDOG_TRIGGERED_REBOOT`**
  * **როდის ქვეყნდება:** როდესაც მოდული წყვეტს "კვებას" და ემზადება სისტემის გადასატვირთად.
  * **Payload (cJSON სტრიქონი):** `{"reason":"Heartbeat missed from main_mqtt_client"}`

## 7. 💡 გამოყენების მაგალითი

წარმოვიდგინოთ, `mqtt_client` მოდული არის კრიტიკული კომპონენტი.

**1. რეგისტრაცია (`mqtt_client.c`-ში):**

```c
// mqtt_client_init ფუნქციაში
service_handle_t handle = fmw_service_get("main_watchdog");
if (handle) {
    watchdog_api_t *wd_api = (watchdog_api_t *)handle;
    wd_api->register_heartbeat("main_mqtt_client");
}
```

**2. "გულისცემის" გაგზავნა (`mqtt_client.c`-ში):**

```c
// MQTT ტასკის მთავარ ციკლში, ყოველი წარმატებული ოპერაციის შემდეგ
void mqtt_task() {
    while(1) {
        // ... აკეთებს თავის საქმეს ...
        
        // ვაგზავნით "გულისცემას"
        service_handle_t handle = fmw_service_get("main_watchdog");
        if (handle) {
            watchdog_api_t *wd_api = (watchdog_api_t *)handle;
            wd_api->send_heartbeat("main_mqtt_client");
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

თუ `mqtt_task` გაიჭედება, `send_heartbeat` აღარ გამოიძახება, `watchdog_manager` ამას დააფიქსირებს და გადატვირთავს სისტემას.
