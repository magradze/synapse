# 📶 მოდული `wifi_manager`

## 1. 📜 მიმოხილვა

**მოდულის სახელი:** `wifi_manager`  
**კატეგორია:** `communications`  
**ვერსია:** 2.1.0  
**ავტორი:** გიორგი მაგრაძე & Synapse AI Assistant

`wifi_manager` არის Synapse Framework-ის ფუნდამენტური საკომუნიკაციო მოდული, რომელიც უზრუნველყოფს WiFi კავშირის სრულფასოვან, ავტომატიზებულ მართვას. მისი მთავარი ამოცანებია:

- **კავშირის ავტომატური მართვა:** შენახული მონაცემებით (credentials) ქსელთან ავტომატური დაკავშირება მოწყობილობის ჩართვისას.
- **კავშირის აღდგენა:** კავშირის გაწყვეტის შემთხვევაში, მისი ავტომატურად აღდგენის მცდელობა ექსპონენციალური backoff სტრატეგიით.
- **Provisioning-თან ინტეგრაცია:** ახალი ქსელის მონაცემების მიღება `provisioning` მოდულებიდან და მათი უსაფრთხოდ შენახვა.
- **სისტემური ივენთების გენერაცია:** სხვა მოდულების ინფორმირება WiFi კავშირის სტატუსის ცვლილების შესახებ (`CONNECTED`, `DISCONNECTED`, `IP_ASSIGNED`).
- **CLI ინტეგრაცია:** სრული, ინტერაქტიული მართვა და დიაგნოსტიკა სერიული კონსოლიდან.

## 2. 🏗️ არქიტექტურული პრინციპები

- **Event-driven არქიტექტურა:** მოდული აქტიურად იყენებს `Event Bus`-ს. ის როგორც **აქვეყნებს** ივენთებს საკუთარი სტატუსის შესახებ, ისე **გამოიწერს** სხვა მოდულების (მაგ., `provisioning`, `system_manager`) ივენთებს.
- **აბსტრაგირებული მდგრადობა (Abstracted Persistence):** ქსელის მონაცემები (SSID და პაროლი) ინახება ცენტრალიზებული **`Storage Manager`** სერვისის მეშვეობით. ეს ნიშნავს, რომ `wifi_manager`-მა არ "იცის", მონაცემები ფიზიკურად NVS-ში ინახება თუ SPIFFS ფაილში, რაც სისტემას უკიდურესად მოქნილს ხდის.
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
|:---|:---|:---|
| `WIFI_EVENT_CONNECTED` | `NULL` | ✅ მოწყობილობა წარმატებით დაუკავშირდა WiFi ქსელს (L2 კავშირი). |
| `WIFI_EVENT_DISCONNECTED` | `NULL` | ❌ მოწყობილობამ დაკარგა კავშირი WiFi ქსელთან. |
| `WIFI_EVENT_IP_ASSIGNED` | `esp_netif_ip_info_t*` | 🌐 მოწყობილობამ მიიღო IP მისამართი DHCP სერვერისგან. |
| `WIFI_CREDENTIALS_NOT_FOUND` | `NULL` | ❓ საცავში ვერ მოიძებნა შენახული მონაცემები, საჭიროა provisioning. |

### გამოწერილი ივენთები

`wifi_manager` გამოიწერს შემდეგ ივენთებს:

| ივენთის სახელი | რეაქცია |
|:---|:---|
| `PROV_CREDENTIALS_RECEIVED` | იღებს ახალ WiFi მონაცემებს და ინახავს მათ `Storage Manager`-ის მეშვეობით. |
| `FMW_EVENT_SYSTEM_START_COMPLETE` | არეგისტრირებს თავის CLI ბრძანებებს, რადგან სისტემა სრულად გაეშვა. |

## 5. ⌨️ CLI ბრძანებები (Command Line Interface)

მოდული არეგისტრირებს `wifi` ბრძანებას, რომელიც იძლევა WiFi-ს სრული მართვისა და დიაგნოსტიკის საშუალებას.

| ბრძანება | არგუმენტები | აღწერა |
|:---|:---|:---|
| `wifi status` | - | ℹ️ გამოაქვს დეტალური ინფორმაცია: კავშირის სტატუსი, SSID, RSSI, IP, MAC და Hostname. |
| `wifi scan` | - | 📡 ასკანერებს და გამოაქვს ხელმისაწვდომი WiFi ქსელების სია. |
| `wifi connect` | `<ssid> <password>` | 🔗 ცდილობს მითითებულ ქსელთან დაკავშირებას და ინახავს მონაცემებს. |
| `wifi disconnect` | - | 🔌 წყვეტს მიმდინარე WiFi კავშირს. |
| `wifi enable` | - | ✅ რთავს `wifi_manager` მოდულს. |
| `wifi disable` | - | ❌ თიშავს `wifi_manager` მოდულს და წყვეტს კავშირს. |
| `wifi erase_creds` | - | 🗑️ შლის შენახულ ყველა WiFi მონაცემს `Storage Manager`-ის გამოყენებით. |
| `wifi set_hostname` | `<name> [--reconnect]` | 📝 ცვლის მოწყობილობის Hostname-ს. `--reconnect` არგუმენტით დაუყოვნებლივ განაახლებს კავშირს. |
| `wifi set_power_save` | `<off\|min\|max>` | 🔋 აყენებს WiFi-ს ენერგიის დაზოგვის რეჟიმს. |

## 6. 💡 გამოყენების მაგალითი

წარმოვიდგინოთ, რომ `mqtt_module`-ს სურს კავშირის დამყარება მხოლოდ მას შემდეგ, რაც მოწყობილობა მიიღებს IP მისამართს.

**`mqtt_module.c`-ში:**

```c
// 1. init ფუნქციაში, გამოვიწეროთ საჭირო ივენთზე
static esp_err_t mqtt_module_init(module_t *self) {
    // ...
    fmw_event_bus_subscribe("WIFI_EVENT_IP_ASSIGNED", self);
    // ...
    return ESP_OK;
}

// 2. დავამუშაოთ ივენთი handle_event ფუნქციაში
static void mqtt_module_handle_event(module_t *self, const char *event_name, void *event_data) {
    if (strcmp(event_name, "WIFI_EVENT_IP_ASSIGNED") == 0) {
        ESP_LOGI(TAG, "WiFi is connected and has an IP. Starting MQTT connection...");
        
        // აქ ვიწყებთ MQTT კავშირის დამყარების ლოგიკას
        start_mqtt_connection(); 
    }
    
    // არ დაგვავიწყდეს wrapper-ის გათავისუფლება
    if (event_data) {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}
```

## 7. ❗ მნიშვნელოვანი შენიშვნები

- **დამოკიდებულება `CMakeLists.txt`-ში:** `wifi_manager`-ის `CMakeLists.txt` ფაილში, `REQUIRES` სექციაში უნდა იყოს მითითებული `storage_manager`, რადგან ის იყენებს ამ სერვისს.
- **`init_level`:** ამ მოდულის `init_level` (`40`) უნდა იყოს უფრო მაღალი, ვიდრე `storage_manager`-ის (`20`), მაგრამ უფრო დაბალი, ვიდრე `command_router`-ის (`50`), რათა დაცული იყოს დამოკიდებულებების იერარქია.
