# 📶 მოდული `wifi_manager`

## 1. 📜 მიმოხილვა

**მოდულის სახელი:** `wifi_manager`  
**კატეგორია:** `communication`  
**ვერსია:** 2.2.0  
**ავტორი:** გიორგი მაგრაძე & Synapse AI Assistant

`wifi_manager` არის Synapse Framework-ის ფუნდამენტური საკომუნიკაციო მოდული, რომელიც უზრუნველყოფს WiFi კავშირის სრულფასოვან, ავტომატიზებულ და მდგრად მართვას. მისი მთავარი ამოცანებია:

- **🔗 კავშირის ავტომატური მართვა:** შენახული მონაცემებით (credentials) ქსელთან ავტომატური დაკავშირება მოწყობილობის ჩართვისას.
- **🔄 კავშირის აღდგენა:** კავშირის გაწყვეტის შემთხვევაში, მისი ავტომატურად აღდგენის მცდელობა ექსპონენციალური backoff სტრატეგიით.
- **📲 Provisioning-თან ინტეგრაცია:** ახალი ქსელის მონაცემების მიღება `provisioning` მოდულებიდან და მათი უსაფრთხოდ შენახვა.
- **📢 სისტემური ივენთების გენერაცია:** სხვა მოდულების ინფორმირება WiFi კავშირის სტატუსის ცვლილების შესახებ (`WIFI_EVENT_CONNECTED`, `WIFI_EVENT_DISCONNECTED`, `WIFI_EVENT_IP_ASSIGNED`).
- **📡 დისტანციური პასუხები:** `wifi status` ბრძანების შესრულებისას, აქვეყნებს დეტალურ სტატუსს JSON ფორმატში `FMW_EVENT_WIFI_STATUS_READY` ივენთის სახით, რაც `mqtt_manager`-ს საშუალებას აძლევს, ეს ინფორმაცია გარე სისტემებს მიაწოდოს.
- **⌨️ CLI ინტეგრაცია:** სრული, ინტერაქტიული მართვა და დიაგნოსტიკა სერიული კონსოლიდან.

## 2. 🏛️ არქიტექტურული პრინციპები

- **Event-driven არქიტექტურა:** მოდული აქტიურად იყენებს `Event Bus`-ს. ის როგორც **აქვეყნებს** ივენთებს საკუთარი სტატუსის შესახებ, ისე **გამოიწერს** სხვა მოდულების (მაგ., `provisioning`, `system_manager`) ივენთებს.
- **აბსტრაგირებული მდგრადობა (Abstracted Persistence):** ქსელის მონაცემები (SSID და პაროლი) ინახება ცენტრალიზებული **`Storage Manager`** სერვისის მეშვეობით. ეს ნიშნავს, რომ `wifi_manager`-მა არ "იცის", მონაცემები ფიზიკურად NVS-ში ინახება თუ SPIFFS ფაილში.
- **ფონური ოპერაციები:** მოდული ეყრდნობა ESP-IDF-ის `event loop` სისტემას და FreeRTOS ტაიმერებს, რათა კავშირის მართვის ყველა ოპერაცია ასინქრონულად, ფონურ რეჟიმში შესრულდეს.
- **დაგვიანებული ინიციალიზაცია (Lazy Initialization of Dependencies):** მოდული არ არის პირდაპირ დამოკიდებული `command_router`-ზე. ის იყენებს `FMW_EVENT_SYSTEM_START_COMPLETE` ივენთს, რათა ბრძანებების რეგისტრაცია მოახდინოს მხოლოდ მას შემდეგ, რაც დარწმუნდება, რომ `command_router`-ის სერვისი უკვე ხელმისაწვდომია.

## 3. ⚙️ კონფიგურაცია

მოდულის საბაზისო კონფიგურაცია ხდება `system_config.json` ფაილში, თუმცა მისი ქცევის პარამეტრების უმეტესობა იმართება `Kconfig`-ის (`idf.py menuconfig`) საშუალებით.

**`system_config.json` მაგალითი:**

```json
{
    "type": "wifi_manager",
    "enabled": true,
    "config": {
        "instance_name": "main_wifi"
    }
}
```

**`Kconfig` პარამეტრები (`idf.py menuconfig`):**

`Component config` -> `Synapse Framework` -> `Modules` -> `WiFi Manager` მენიუში შეგიძლიათ დააკონფიგურიროთ:

- **Reconnect Attempts:** ხელახალი დაკავშირების მცდელობების მაქსიმალური რაოდენობა.
- **Reconnect Delay:** საწყისი დაყოვნება ხელახალი დაკავშირებისას.

## 4. 📢 ივენთები (Events)

### გამოქვეყნებული ივენთები

`wifi_manager` აქვეყნებს შემდეგ ივენთებს, რომლებზეც სხვა მოდულებს შეუძლიათ გამოწერა:

| ივენთის სახელი | მონაცემები (Payload) | აღწერა |
| :--- | :--- | :--- |
| `WIFI_EVENT_CONNECTED` | `NULL` | ✅ მოწყობილობა წარმატებით დაუკავშირდა WiFi ქსელს (L2 კავშირი). |
| `WIFI_EVENT_DISCONNECTED` | `NULL` | ❌ მოწყობილობამ დაკარგა კავშირი WiFi ქსელთან. |
| `WIFI_EVENT_IP_ASSIGNED` | `esp_netif_ip_info_t*` | 🌐 მოწყობილობამ მიიღო IP მისამართი DHCP სერვერისგან. |
| `WIFI_CREDENTIALS_NOT_FOUND` | `NULL` | ❓ საცავში ვერ მოიძებნა შენახული მონაცემები, საჭიროა provisioning. |
| `FMW_EVENT_WIFI_STATUS_READY` | `char*` (JSON) | 📊 `wifi status` ბრძანების პასუხი JSON ფორმატში. |

### გამოწერილი ივენთები

`wifi_manager` გამოიწერს შემდეგ ივენთებს:

| ივენთის სახელი | რეაქცია |
| :--- | :--- |
| `PROV_CREDENTIALS_RECEIVED` | იღებს ახალ WiFi მონაცემებს და ინახავს მათ `Storage Manager`-ის მეშვეობით. |
| `FMW_EVENT_SYSTEM_START_COMPLETE` | არეგისტრირებს თავის CLI ბრძანებებს, რადგან სისტემა სრულად გაეშვა. |

## 5. 📡 MQTT ინტერფეისი

ეს მოდული პირდაპირ არ უკავშირდება MQTT-ს, მაგრამ ის აწვდის ინფორმაციას `mqtt_manager`-ს `Event Bus`-ის საშუალებით.

- **გამოსაქვეყნებელი თემა:** `synapse/devices/{id}/state/wifi/status`
- **როდის ქვეყნდება:** როდესაც `mqtt_manager` მიიღებს `FMW_EVENT_WIFI_STATUS_READY` ივენთს.
- **Payload (მაგალითი):**

    ```json
    {
      "module_state": "Enabled",
      "connection_status": "Connected",
      "ssid": "MyHomeWiFi",
      "channel": 11,
      "rssi": -55,
      "ip_address": "192.168.1.123"
    }
    ```

## 6. ⌨️ CLI ბრძანებები (Command Line Interface)

მოდული არეგისტრირებს `wifi` ბრძანებას, რომელიც იძლევა WiFi-ს სრული მართვისა და დიაგნოსტიკის საშუალებას.

| ბრძანება | არგუმენტები | აღწერა |
| :--- | :--- | :--- |
| `wifi status` | - | ℹ️ გამოაქვს დეტალური ინფორმაცია კონსოლში **და** აქვეყნებს `FMW_EVENT_WIFI_STATUS_READY` ივენთს. |
| `wifi scan` | - | 📡 ასკანერებს და გამოაქვს ხელმისაწვდომი WiFi ქსელების სია. |
| `wifi connect` | `<ssid> <password>` | 🔗 ცდილობს მითითებულ ქსელთან დაკავშირებას და ინახავს მონაცემებს. |
| `wifi disconnect` | - | 🔌 წყვეტს მიმდინარე WiFi კავშირს. |
| `wifi enable` | - | ✅ რთავს `wifi_manager` მოდულს. |
| `wifi disable` | - | ❌ თიშავს `wifi_manager` მოდულს და წყვეტს კავშირს. |
| `wifi erase_creds` | - | 🗑️ შლის შენახულ ყველა WiFi მონაცემს. |
| `wifi set_hostname` | `<name> [--reconnect]` | 📝 ცვლის მოწყობილობის Hostname-ს. |
| `wifi set_power_save` | `<off\|min\|max>` | 🔋 აყენებს WiFi-ს ენერგიის დაზოგვის რეჟიმს. |

## 7. 💡 გამოყენების მაგალითი (მოთხოვნა-პასუხი)

1. **💻 მომხმარებელი (Postman/MQTT კლიენტი):**
    - **გამოიწერს:** `synapse/devices/+/state/#`
    - **აქვეყნებს:**
        - **Topic:** `synapse/devices/{id}/cmd/in`
        - **Payload:** `"wifi status"`

2. **⚙️ მოწყობილობა (Synapse Framework):**
    - `mqtt_manager` იღებს ბრძანებას და აგზავნის ივენთს `Command Router`-ში.
    - `command_router` იძახებს `wifi_manager`-ის `cmd_handler`-ს.
    - `wifi_manager` აგროვებს სტატუსს, ქმნის JSON-ს და აგზავნის `FMW_EVENT_WIFI_STATUS_READY` ივენთს.
    - `mqtt_manager` იღებს `FMW_EVENT_WIFI_STATUS_READY` ივენთს და აქვეყნებს MQTT-ში.

3. **💻 მომხმარებელი (Postman/MQTT კლიენტი):**
    - **იღებს შეტყობინებას:**
        - **Topic:** `synapse/devices/{id}/state/wifi/status`
        - **Payload:** `{"connection_status":"Connected", ...}`

## 8. ❗ მნიშვნელოვანი შენიშვნები

- **დამოკიდებულება `CMakeLists.txt`-ში:** `wifi_manager`-ის `CMakeLists.txt` ფაილში, `REQUIRES` სექციაში უნდა იყოს მითითებული `storage_manager`, რადგან ის იყენებს ამ სერვისს.
- **`init_level`:** ამ მოდულის `init_level` (`40`) უნდა იყოს უფრო მაღალი, ვიდრე `storage_manager`-ის (`20`), მაგრამ უფრო დაბალი, ვიდრე `command_router`-ის (`50`), რათა დაცული იყოს დამოკიდებულებების იერარქია.
