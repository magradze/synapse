# 🛰️ მოდული: Connectivity Watchdog

**🗂️ კატეგორია:** `system`  
**✒️ ავტორი:** Synapse Framework Team  
**🔖 ვერსია:** 2.1.0

## 1. 📜 მიმოხილვა

`Connectivity Watchdog` არის Synapse Framework-ის **სისტემის მდგრადობისა და თვითაღდგენის მცველი**. მისი ფუნდამენტური ამოცანაა, უზრუნველყოს, რომ მოწყობილობა არასდროს დარჩეს "ცოცხალი, მაგრამ იზოლირებული" მდგომარეობაში. ის მუდმივად აკვირდება კრიტიკულ საკომუნიკაციო არხებს და პრობლემის აღმოჩენისას, ავტომატურად იწყებს მრავალსაფეხურიან, კონფიგურირებად აღდგენის პროცესს.

ეს მოდული სცილდება უბრალო "პინგს". ის არის **უნივერსალური, მრავალშრიანი მონიტორინგის სისტემა**, რომელიც:

- **ამოწმებს** როგორც პასიურ "heartbeat" სიგნალებს (მაგ., MQTT-დან), ისე აქტიურად ამოწმებს ქსელის სიჯანსაღეს (მაგ., PING-ით).
- **რეაგირებს** პრობლემებზე ეტაპობრივად — მარტივი გაფრთხილებიდან დაწყებული, სისტემის სრული გადატვირთვით დამთავრებული.
- **არის უნივერსალური** და მზად არის ნებისმიერ საკომუნიკაციო პროტოკოლთან (WebSocket, CoAP, HTTP) ინტეგრაციისთვის.

## 2. 🏛️ არქიტექტურული პრინციპები

- **🌐 უნივერსალური "Heartbeat" პატერნი:** მოდული არ არის მიბმული კონკრეტულ პროტოკოლზე. ის უსმენს ზოგად, აბსტრაქტულ `FMW_EVENT_CONNECTIVITY_ESTABLISHED` ივენთს. ნებისმიერ საკომუნიკაციო მოდულს შეუძლია ამ ივენთის გამოქვეყნება, რითაც Watchdog-ს ამცნობს, რომ "კავშირი ცოცხალია".
- **🧅 მრავალშრიანი მონიტორინგი (Multi-Layered Monitoring):**
  - **პასიური შრე (Heartbeat):** ეყრდნობა სხვა მოდულების მიერ გამოქვეყნებულ ივენთებს.
  - **აქტიური შრე (Active Probing):** თავად იწყებს შემოწმებებს (მაგ., PING), რათა დარწმუნდეს, რომ ქსელის სრული გზა გამართულად მუშაობს.
- **⚙️ კონფიგურაციით მართული ლოგიკა:** მოდულის მთელი ქცევა — რას ამოწმებს, რა ინტერვალით, რა ითვლება შეცდომად და როგორ რეაგირებს — სრულად იმართება `system_config.json` ფაილიდან.
- **⚖️ მდგომარეობის მართვა და ესკალაცია:** მოდული ითვლის წარუმატებელ მცდელობებს და პრობლემის გამწვავებასთან ერთად, გადადის უფრო აგრესიულ აღდგენის სტრატეგიებზე, რაც იცავს სისტემას არასაჭირო გადატვირთვის ციკლებისგან.
- **🤝 სერვისების ინტეგრაცია:** ის არის სხვა სისტემური სერვისების აქტიური მომხმარებელი (`Service Consumer`), როგორიცაა `System Timer` (პერიოდული შემოწმებებისთვის) და `System Manager` (გადატვირთვისთვის).

## 3. ⚙️ კონფიგურაცია (`system_config.json`)

მოდულის სრული ქცევა იმართება `layers` მასივის საშუალებით. თითოეული ობიექტი ამ მასივში წარმოადგენს ერთ დამოუკიდებელ შემოწმებას.

### 3.1. კონფიგურაციის სრული მაგალითი

```json
{
  "type": "connectivity_watchdog",
  "enabled": true,
  "config": {
    "instance_name": "main_conn_watchdog",
    "layers": [
      {
        "type": "MQTT",
        "timeout_sec": 300,
        "actions": [
            { "on_failure_count": 1, "do": "log_warning", "message": "MQTT connection seems stale." },
            { "on_failure_count": 3, "do": "reconnect_service", "service_name": "reconnect_mqtt" },
            { "on_failure_count": 5, "do": "trigger_alarm", "alarm_name": "CriticalMQTTLost" }
        ]
      },
      {
        "type": "PING",
        "target": "8.8.8.8",
        "timeout_sec": 180,
        "actions": [
            { "on_failure_count": 2, "do": "reconnect_service", "service_name": "reconnect_wifi" },
            { "on_failure_count": 4, "do": "reboot_system", "delay_ms": 5000, "message": "Rebooting due to persistent network failure." }
        ]
      },
      {
        "type": "WEBSOCKET",
        "timeout_sec": 600,
        "actions": [
            { "on_failure_count": 2, "do": "log_warning", "message": "WebSocket connection lost." }
        ]
      }
    ]
  }
}
```

### 3.2. `layers` ობიექტის პარამეტრები

| პარამეტრი | ტიპი | აღწერა | სავალდებულო |
| :--- | :--- | :--- | :---: |
| `type` | სტრიქონი | შემოწმების ტიპი. მხარდაჭერილია: `MQTT`, `PING`, `WEBSOCKET` და ა.შ. (იხილეთ ქვემოთ). | ✅ |
| `timeout_sec` | რიცხვი | მაქსიმალური დრო წამებში ბოლო წარმატებულ "heartbeat"-სა და შემოწმებას შორის. | ✅ |
| `target` | სტრიქონი | (მხოლოდ აქტიური შემოწმებისთვის) სამიზნე მისამართი, მაგ., IP მისამართი `PING`-ისთვის. | ❌ |
| `actions` | მასივი | მოქმედებების სია, რომლებიც უნდა შესრულდეს შეცდომისას. | ✅ |

### 3.3. `actions` ობიექტის პარამეტრები

| პარამეტრი | ტიპი | აღწერა | სავალდებულო |
| :--- | :--- | :--- | :---: |
| `on_failure_count` | რიცხვი | წარუმატებელი მცდელობების რაოდენობა, რომელზეც ეს მოქმედება უნდა შესრულდეს. | ✅ |
| `do` | სტრიქონი | შესასრულებელი მოქმედების ტიპი. | ✅ |
| `message` | სტრიქონი | (არასავალდებულო) ტექსტი, რომელიც დაიბეჭდება `log_warning` ან `reboot_system` მოქმედებისას. | ❌ |
| `service_name` | სტრიქონი | (მხოლოდ `reconnect_service`-თვის) იმ სერვისის სახელი, რომლის გადატვირთვაც უნდა მოხდეს. | ❌ |
| `alarm_name` | სტრიქონი | (მხოლოდ `trigger_alarm`-თვის) `Alarms Manager`-ში გასაგზავნი განგაშის სახელი. | ❌ |
| `delay_ms` | რიცხვი | (მხოლოდ `reboot_system`-თვის) დაყოვნება გადატვირთვამდე მილიწამებში. | ❌ |

### 3.4. მხარდაჭერილი `type`-ები და `do` მოქმედებები

| `layer.type` | შიდა ლოგიკა | `check_name` (გენერირებული) |
| :--- | :--- | :--- |
| `MQTT` | პასიური (Heartbeat) | `MQTT_Heartbeat` |
| `WEBSOCKET` | პასიური (Heartbeat) | `WEBSOCKET_Heartbeat` |
| `PING` | აქტიური (Probing) | `PING_{target_ip}` |

| `action.do` | აღწერა |
| :--- | :--- |
| `log_warning` | წერს გაფრთხილებას ლოგში. |
| `reconnect_service` | აგზავნის ივენთს კონკრეტული სერვისის გადატვირთვის მოთხოვნით. |
| `trigger_alarm` | აგზავნის ივენთს `Alarms Manager`-ისთვის. |
| `reboot_system` | გადატვირთავს მოწყობილობას. |

## 4. 🔌 Service API

ეს მოდული არის სუფთა **Service Consumer** და არ აწვდის საკუთარ Service API-ს სხვა მოდულებს. მისი მართვა ხდება მხოლოდ კონფიგურაციით და ივენთებით.

## 5. 📢 ივენთების უნივერსალური მოდელი

`Connectivity Watchdog`-ის უნივერსალურობის გასაღები არის `FMW_EVENT_CONNECTIVITY_ESTABLISHED` ივენთი.

- **ივენთის სახელი:** `FMW_EVENT_CONNECTIVITY_ESTABLISHED`
- **Payload-ის ტიპი:** `fmw_connectivity_payload_t`
- **Payload-ის შიგთავსი:** `char check_name[32];`

ნებისმიერმა მოდულმა, რომელსაც სურს Watchdog-ს "უთხრას", რომ ის "ცოცხალია", უნდა გამოაქვეყნოს ეს ივენთი და `payload`-ში მიუთითოს იმ შემოწმების სახელი, რომელიც კონფიგურაციაშია განსაზღვრული.

### 5.1. MQTT მოდულის ინტეგრაცია

`mqtt_manager.c`-ში, `MQTT_EVENT_CONNECTED` ქეისში:

```c
// ...
fmw_connectivity_payload_t *payload = malloc(sizeof(fmw_connectivity_payload_t));
if (payload) {
    snprintf(payload->check_name, sizeof(payload->check_name), "MQTT_Heartbeat");
    // ... შეფუთვა და გამოქვეყნება ...
}
// ...
```

### 5.2. WebSocket მოდულის (ჰიპოთეტური) ინტეგრაცია

წარმოვიდგინოთ, გვაქვს `websocket_manager`. როდესაც ის დაამყარებს კავშირს, ისიც იგივეს გააკეთებს:

```c
// In websocket_manager.c, on successful connection:
fmw_connectivity_payload_t *payload = malloc(sizeof(fmw_connectivity_payload_t));
if (payload) {
    snprintf(payload->check_name, sizeof(payload->check_name), "WEBSOCKET_Heartbeat");
    // ... შეფუთვა და გამოქვეყნება ...
}
```

### 5.3. PING შემოწმების ინტეგრაცია

`connectivity_watchdog.c`-ის შიგნით, `ping_check_cb` ფუნქცია წარმატების შემთხვევაში აკეთებს იგივეს:

```c
// In connectivity_watchdog.c, inside ping_check_cb:
fmw_connectivity_payload_t *payload = malloc(sizeof(fmw_connectivity_payload_t));
if (payload) {
    snprintf(payload->check_name, sizeof(payload->check_name), "PING_%s", check_cfg->target);
    // ... შეფუთვა და გამოქვეყნება ...
}
```

## 6. ❗ მნიშვნელოვანი შენიშვნები

- **`init_level`:** მოდულის `init_level` (`40`) კრიტიკულია. ის უნდა გაეშვას `System Timer`-ის (`35`) შემდეგ, მაგრამ იმ მოდულებამდე, რომელთა ივენთებსაც ის უსმენს.
- **დამოკიდებულებები:** `CMakeLists.txt`-ში, `REQUIRES` სექციაში უნდა იყოს მითითებული `interfaces`, რათა მას ჰქონდეს წვდომა სხვა სერვისების API კონტრაქტებზე.
- **უსაფრთხოება:** `reboot_system` მოქმედება გამოიყენეთ სიფრთხილით. დარწმუნდით, რომ `on_failure_count` და `timeout_sec` პარამეტრები სწორად არის შერჩეული, რათა თავიდან აიცილოთ არასასურველი გადატვირთვის ციკლები.
