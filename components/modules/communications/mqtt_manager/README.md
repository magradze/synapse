# ☁️ მოდული `mqtt_manager`

## 1. 📜 მიმოხილვა

**მოდულის სახელი:** `mqtt_manager`  
**კატეგორია:** `communication`  
**ვერსია:** 1.2.0  
**ავტორი:** Synapse Framework Team

`mqtt_manager` მოდული არის Synapse Framework-ის მთავარი **საკომუნიკაციო ხიდი** გარე სამყაროსთან MQTT პროტოკოლის საშუალებით. ის არ არის უბრალოდ MQTT კლიენტი; ის არის **ინტელექტუალური შუამავალი**, რომელიც სრულად ინტეგრირებულია ფრეიმვორქის იზოლირებულ, ივენთებზე დაფუძნებულ არქიტექტურასთან.

მისი მთავარი ამოცანებია:

- **🌐 საიმედო კავშირის მართვა:** WiFi კავშირის დამყარების შემდეგ, ავტომატურად უკავშირდება MQTT ბროკერს და უზრუნველყოფს კავშირის სტაბილურობას `Last Will and Testament` (LWT) მექანიზმის გამოყენებით.
- **🆔 უნიკალური იდენტიფიკაცია:** იყენებს `Device Identity Service`-ს, რათა შექმნას უნიკალური `ClientID` და თემების (topics) იერარქია თითოეული მოწყობილობისთვის.
- **➡️ ბრძანებების გადამისამართება (Command Forwarding):** იღებს დისტანციურ ბრძანებებს სპეციალური MQTT თემიდან და "თარგმნის" მათ ფრეიმვორქის შიდა `FMW_EVENT_EXECUTE_COMMAND_STRING` ივენთად, ისე რომ არ "იცის" ამ ბრძანებების შინაარსი.
- **⬅️ სტატუსების გამოქვეყნება (Status Publishing):** უსმენს სისტემის შიდა, კონკრეტულ სტატუსის ივენთებს (მაგ., `FMW_EVENT_WIFI_STATUS_READY`) და აქვეყნებს მათ შესაბამის, ავტომატურად გენერირებულ MQTT თემებზე.

## 2. 🏛️ არქიტექტურული პრინციპები

ეს მოდული არის ფრეიმვორქის ძირითადი ფილოსოფიის იდეალური განსახიერება.

- **სრული დეკაპლინგი (Total Decoupling):** `mqtt_manager`-მა **არაფერი იცის** `relay_module`-ის, `wifi_manager`-ის ან ნებისმიერი სხვა ფუნქციონალური მოდულის შიდა ლოგიკის შესახებ. ის მხოლოდ იღებს და აგზავნის სტანდარტიზებულ შეტყობინებებს.
- **სერვისების მომხმარებელი (Service Consumer):** მოდული არის სხვა ფუნდამენტური სერვისების აქტიური მომხმარებელი:
  - `Device Identity Service`: უნიკალური ID-სა და თემების მისაღებად.
- **ივენთებზე დაფუძნებული ლოგიკა:** მოდულის თითქმის ყველა მოქმედება არის რეაქცია `Event Bus`-ზე მომხდარ ივენთზე. ის არ იღებს გადაწყვეტილებებს, არამედ რეაგირებს.
- **"ივენთი-თემის რუკა" (Event-to-Topic Map):** მოდული იყენებს შიდა "რუკას", რათა იცოდეს, რომელი შიდა ივენთი რომელ გარე MQTT თემაზე უნდა გამოაქვეყნოს. ეს ხდის სტატუსების გამოქვეყნების ლოგიკას ცენტრალიზებულს და ადვილად გაფართოებადს.

## 3. ⚙️ კონფიგურაცია

მოდულის სრული კონფიგურაცია ხდება `system_config.json` ფაილში.

**კონფიგურაციის მაგალითი:**

```json
{
    "type": "mqtt_manager",
    "enabled": true,
    "config": {
        "instance_name": "main_mqtt_broker",
        "broker_uri": "mqtts://your-broker.com:8883",
        "username": "your-username",
        "password": "your-password",
        "base_topic_prefix": "synapse/devices",
        "lwt_topic": "lwt",
        "lwt_message": "{\"status\":\"offline\"}",
        "lwt_qos": 1,
        "lwt_retain": true
    }
}
```

**კონფიგურაციის პარამეტრები:**

| პარამეტრი | ტიპი | აღწერა | სავალდებულო |
| :--- | :--- | :--- | :---: |
| `instance_name` | სტრიქონი | მოდულის ინსტანციის უნიკალური სახელი. | ✅ |
| `broker_uri` | სტრიქონი | MQTT ბროკერის სრული მისამართი (მაგ., `mqtt://` ან `mqtts://`). | ✅ |
| `username` | სტრიქონი | მომხმარებლის სახელი ავთენტიფიკაციისთვის. | ❌ |
| `password` | სტრიქონი | პაროლი ავთენტიფიკაციისთვის. | ❌ |
| `base_topic_prefix` | სტრიქონი | თემების საბაზისო პრეფიქსი. | ❌ (Default: `synapse/devices`) |
| `lwt_topic` | სტრიქონი | Last Will and Testament (LWT) თემის სახელი. | ❌ (Default: `lwt`) |
| `lwt_message` | სტრიქონი | LWT შეტყობინება, რომელიც გაიგზავნება კავშირის გაწყვეტისას. | ❌ (Default: `{"status":"offline"}`) |
| `lwt_qos` | რიცხვი | LWT შეტყობინების QoS დონე. | ❌ (Default: `1`) |
| `lwt_retain` | boolean | LWT შეტყობინების `retain` ფლაგი. | ❌ (Default: `true`) |

## 4. 📡 MQTT თემების სტრუქტურა (Topic Structure)

მოდული იყენებს სტრუქტურირებულ, იერარქიულ თემებს, რომლებიც **ავტომატურად გენერირდება** build-ის დროს `module.json` ფაილებში აღწერილი `mqtt_interface` სექციების მიხედვით.

**საბაზისო სტრუქტურა:** `synapse/devices/{device_id}/{sub_topic}`

**მაგალითი (`device_id` = `5C013B7246E4`):**

- **➡️ ბრძანებების მისაღებად (Inbound):**
  - **თემა:** `synapse/devices/5C013B7246E4/cmd/in`
  - **აღწერა:** ეს არის ერთადერთი თემა, რომელსაც `mqtt_manager` გამოიწერს ბრძანებების მისაღებად.
  - **Payload:** ბრძანების სტრიქონი, მაგ., `"led set 255 0 0"`.

- **⬅️ სტატუსების გამოსაქვეყნებლად (Outbound):**
  - **თემა:** `synapse/devices/5C013B7246E4/state/wifi/status`
  - **აღწერა:** ამ თემაზე ქვეყნდება WiFi-ს სტატუსი, როდესაც `wifi_manager` აქვეყნებს `FMW_EVENT_WIFI_STATUS_READY` ივენთს.
  - **Payload:** `{"connection_status": "Connected", "ssid": "MyHomeWiFi", ...}`
  - **თემა:** `synapse/devices/5C013B7246E4/state/device/info`
  - **აღწერა:** ამ თემაზე ქვეყნდება მოწყობილობის ინფორმაცია, როდესაც `device_identity_service` აქვეყნებს `FMW_EVENT_DEVICE_INFO_READY` ივენთს.
  - **Payload:** `{"device_id": "5C013B7246E4", ...}`

- **🔴 LWT (Last Will and Testament):**
  - **თემა:** `synapse/devices/5C013B7246E4/lwt`
  - **Payload:** `{"status": "offline"}`

## 5. 🧠 იმპლემენტაციის ლოგიკა

### 5.1. `init` ფაზა

1. **გამოიწერეთ `WIFI_EVENT_IP_ASSIGNED` ივენთზე:** ეს არის სიგნალი, რომ მზად ვართ MQTT კავშირისთვის.
2. **ავტომატურად გამოიწერეთ ყველა სტატუსის ივენთზე:** `init` ფუნქცია გადის `event_topic_map` მასივს და გამოიწერს ყველა იქ აღწერილ ივენთს (მაგ., `FMW_EVENT_WIFI_STATUS_READY`, `FMW_EVENT_DEVICE_INFO_READY`).

### 5.2. `handle_event` ფუნქცია

ეს ფუნქცია არის მოდულის "გული".

- **თუ ივენთი არის `WIFI_EVENT_IP_ASSIGNED`:**
  - იწყებს MQTT კლიენტის ინიციალიზაციისა და კავშირის დამყარების პროცესს.
- **თუ ივენთი არის `event_topic_map`-ში აღწერილი რომელიმე სხვა ივენთი:**
    1. ამოწმებს, არის თუ არა MQTT კლიენტი დაკავშირებული.
    2. პოულობს შესაბამის MQTT თემის შაბლონს "რუკიდან".
    3. `MQTT_BUILD_TOPIC` მაკროსის გამოყენებით აგებს სრულ თემას `device_id`-ის ჩასმით.
    4. აქვეყნებს ივენთის `payload`-ს (რომელიც JSON სტრიქონია) ამ თემაზე.

### 5.3. `mqtt_event_handler_cb` (ESP-IDF-ის callback)

- **`MQTT_EVENT_CONNECTED`:**
  - `MQTT_BUILD_TOPIC`-ის გამოყენებით აგებს ბრძანებების თემას (`.../cmd/in`).
  - იძახებს `esp_mqtt_client_subscribe`-ს ამ თემაზე.
- **`MQTT_EVENT_DATA` (ბრძანება მოვიდა):**
  - ეს არის **CLI Command Forwarder**-ის ლოგიკა.
  - იღებს მიღებულ `payload`-ს (ბრძანების სტრიქონს).
  - ფუთავს მას `fmw_command_payload_t`-ში.
  - აქვეყნებს `FMW_EVENT_EXECUTE_COMMAND_STRING` ივენთს.

## 6. 💡 გამოყენების მაგალითი (მოთხოვნა-პასუხის ციკლი)

1. **💻 მომხმარებელი (Postman/MQTT კლიენტი):**
    - **გამოიწერს:** `synapse/devices/+/state/#` (რომ მიიღოს ყველა სტატუსი).
    - **აქვეყნებს:**
        - **Topic:** `synapse/devices/5C013B7246E4/cmd/in`
        - **Payload:** `"wifi status"`

2. **⚙️ მოწყობილობა (Synapse Framework):**
    - `mqtt_manager` იღებს ბრძანებას და აგზავნის ივენთს `Command Router`-ში.
    - `command_router` იძახებს `wifi_manager`-ის `cmd_handler`-ს.
    - `wifi_manager` აგროვებს სტატუსს, ქმნის JSON-ს და აგზავნის `FMW_EVENT_WIFI_STATUS_READY` ივენთს.
    - `mqtt_manager` იღებს `FMW_EVENT_WIFI_STATUS_READY` ივენთს.

3. **💻 მომხმარებელი (Postman/MQTT კლიენტი):**
    - **იღებს შეტყობინებას:**
        - **Topic:** `synapse/devices/5C013B7246E4/state/wifi/status`
        - **Payload:** `{"connection_status":"Connected", ...}`

## 7. ❗ მნიშვნელოვანი შენიშვნები

- **უსაფრთხოება:** რეალურ პროდუქტში, ყოველთვის გამოიყენეთ `mqtts://` (TLS) და ძლიერი `username`/`password`.
- **`init_level`:** ამ მოდულის `init_level` (`45`) კრიტიკულია. ის უნდა იყოს `wifi_manager`-ის (`40`) და `device_identity_service`-ის (`20`) შემდეგ, რათა მათი სერვისები ხელმისაწვდომი იყოს.
