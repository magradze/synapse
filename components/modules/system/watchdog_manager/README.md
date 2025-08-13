# 🐕 მოდული: `watchdog_manager`

**🗂️ კატეგორია:** `system`  
**✒️ ავტორი:** Synapse Framework Team  
**🔖 ვერსია:** 1.2.0

## 1. 📜 მიმოხილვა

`Watchdog Manager` მოდული არის Synapse Framework-ის **თვითაღდგენის და საიმედოობის კრიტიკული კომპონენტი**. მისი მთავარი ამოცანაა, დაიცვას სისტემა პროგრამული უზრუნველყოფის გაჭედვისგან (software freeze/hang).

ტრადიციული watchdog-ებისგან განსხვავებით, რომლებიც უბრალოდ პერიოდულად "იკვებებიან", ეს მოდული ახდენს **"ჭკვიანი" ანუ აპლიკაციაზე მორგებული (application-aware)** watchdog-ის იმპლემენტაციას. ის იყენებს ESP-IDF-ის Task Watchdog Timer-ს (TWDT), მაგრამ "აკვებებს" მას მხოლოდ იმ შემთხვევაში, თუ სისტემის **ყველა** კრიტიკულად მნიშვნელოვანი, რეგისტრირებული კომპონენტი გამართულად და დროულად ფუნქციონირებს.

## 2. 🏛️ არქიტექტურული პრინციპები

- **განაწილებული "გულისცემა" (Distributed Heartbeat):** მოდული არ ეყრდნობა ერთ ცენტრალურ ტასკს. ამის ნაცვლად, ის ელოდება "გულისცემის" სიგნალებს სხვადასხვა კრიტიკული მოდულისგან. Watchdog-ი "იკვებება" მხოლოდ მაშინ, როცა ყველა რეგისტრირებული მოდულისგან მიღებულია სიგნალი.
- **Service-driven:** მოდული აწვდის `watchdog_api_t` სერვისს, რომელიც სხვა მოდულებს საშუალებას აძლევს, დაარეგისტრირონ საკუთარი თავი, როგორც "კრიტიკული" კომპონენტი და პერიოდულად გააგზავნონ "გულისცემა".
- **ეფექტური პერიოდული შემოწმება:** ცალკე FreeRTOS ტასკის შექმნის ნაცვლად, მოდული იყენებს `System Timer Service`-ს, რათა პერიოდულად გამოიძახოს შემოწმების ლოგიკა. ეს ამცირებს სისტემის დატვირთვას და ზოგავს რესურსებს.
- **ფენებად დაყოფილი პასუხისმგებლობა:**
    1. **ESP-IDF TWDT:** დაბალი დონის ტაიმერი, რომელიც იწვევს აპარატურულ გადატვირთვას.
    2. **`watchdog_manager`:** მაღალი დონის ლოგიკა, რომელიც წყვეტს, "ვაჭამოთ" თუ არა TWDT-ს.
    3. **სხვა მოდულები:** პასუხისმგებელნი არიან, დროულად გააგზავნონ საკუთარი "გულისცემა".

## 3. ⚙️ კონფიგურაცია

მოდულის კონფიგურაცია ხდება როგორც `Kconfig`-ის, ისე `system_config.json` ფაილის საშუალებით.

**`system_config.json` მაგალითი:**

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

| პარამეტრი | ტიპი | აღწერა | Default (`Kconfig`) |
| :--- | :--- | :--- | :--- |
| `instance_name` | სტრიქონი | მოდულის ინსტანციის უნიკალური სახელი. | `main_watchdog` |
| `check_interval_ms` | რიცხვი | ინტერვალი (მილიწამებში), რა სიხშირითაც მოდული ამოწმებს მიღებულ "გულისცემებს". | `2000` |
| `heartbeat_timeout_ms` | რიცხვი | მაქსიმალური დრო (მილიწამებში), რომლის განმავლობაშიც მოდული ელოდება "გულისცემას" რეგისტრირებული კომპონენტისგან. | `5000` |

## 4. 🔌 Service API (`watchdog_api_t`)

სხვა მოდულებს შეუძლიათ მიიღონ წვდომა Watchdog მენეჯერის API-ზე `Service Locator`-ის მეშვეობით. სერვისის ტიპი: `SYNAPSE_SERVICE_TYPE_WATCHDOG_API`.

**API ფუნქციები:**

- **`esp_err_t register_heartbeat(const char *module_name);`**  
    ❤️ არეგისტრირებს მოდულს, როგორც კრიტიკულ კომპონენტს. ამის შემდეგ, `watchdog_manager` დაიწყებს ამ მოდულისგან "გულისცემის" ლოდინს.
- **`esp_err_t send_heartbeat(const char *module_name);`**  
    💓 რეგისტრირებული მოდული პერიოდულად იძახებს ამ ფუნქციას, რათა ამცნოს `watchdog_manager`-ს, რომ ის "ცოცხალია".
- **`esp_err_t unregister_heartbeat(const char *module_name);`**  
    💔 შლის მოდულს მონიტორინგის სიიდან (ამჟამად არ არის იმპლემენტირებული).

## 5. 📢 გამოქვეყნებული ივენთები

- **`SYNAPSE_EVENT_HEARTBEAT_MISSED`**
  - **როდის ქვეყნდება:** როდესაც რომელიმე რეგისტრირებული მოდული ვერ აგზავნის "გულისცემას" `heartbeat_timeout_ms`-ის განმავლობაში.
  - **Payload (`synapse_telemetry_payload_t`):** `module_name` ველი შეიცავს იმ მოდულის სახელს, რომელიც "გაიჭედა".

## 6. 💡 გამოყენების მაგალითი

წარმოვიდგინოთ, `mqtt_manager` არის კრიტიკული კომპონენტი.

**1. რეგისტრაცია (`mqtt_manager.c`-ში, `init` ფუნქციაში):**

```c
service_handle_t handle = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_WATCHDOG_API);
if (handle) {
    watchdog_api_t *wd_api = (watchdog_api_t *)handle;
    wd_api->register_heartbeat("main_mqtt_broker");
}
```

**2. "გულისცემის" გაგზავნა (`mqtt_manager.c`-ში, პერიოდულად):**

```c
// მაგალითად, MQTT_EVENT_CONNECTED-ისას ან პერიოდულად
service_handle_t handle = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_WATCHDOG_API);
if (handle) {
    watchdog_api_t *wd_api = (watchdog_api_t *)handle;
    wd_api->send_heartbeat("main_mqtt_broker");
}
```

თუ `mqtt_manager` გაიჭედება, `send_heartbeat` აღარ გამოიძახება, `watchdog_manager` ამას დააფიქსირებს, გამოაქვეყნებს `SYNAPSE_EVENT_HEARTBEAT_MISSED` ივენთს და შეწყვეტს TWDT-ს "კვებას", რაც გამოიწვევს სისტემის უსაფრთხო გადატვირთვას.

## 7. ❗ წინაპირობები და `menuconfig`

ამ მოდულის გამართული მუშაობისთვის აუცილებელია ESP-IDF-ის კონფიგურაციაში (`menuconfig`) შემდეგი ოფციის ჩართვა:

- **გზა:** `Component config` → `ESP System Settings`
- **ოფცია:** `[*] Initialize Task Watchdog Timer on startup` (`CONFIG_ESP_TASK_WDT_INIT`)
