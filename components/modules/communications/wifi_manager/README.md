# 📶 მოდული `wifi_manager`

## 1. 📜 მიმოხილვა

**მოდულის სახელი:** `wifi_manager`  
**კატეგორია:** `communications`  
**ვერსია:** 2.0.0  
**ავტორი:** გიორგი მაგრაძე & Synapse AI Assistant

`wifi_manager` არის Synapse Framework-ის ფუნდამენტური საკომუნიკაციო მოდული, რომელიც უზრუნველყოფს WiFi კავშირის სრულფასოვან მართვას. მისი მთავარი ამოცანებია:

- **კავშირის ავტომატური მართვა:** შენახული მონაცემებით (credentials) ქსელთან ავტომატური დაკავშირება.
- **კავშირის აღდგენა:** კავშირის გაწყვეტის შემთხვევაში, მისი ავტომატურად აღდგენის მცდელობა ექსპონენციალური backoff სტრატეგიით.
- **Provisioning-თან ინტეგრაცია:** ახალი ქსელის მონაცემების მიღება და შენახვა `provisioning` მოდულებიდან.
- **სისტემური ივენთების გენერაცია:** სხვა მოდულების ინფორმირება WiFi კავშირის სტატუსის ცვლილების შესახებ (`CONNECTED`, `DISCONNECTED`, `IP_ASSIGNED`).
- **CLI ინტეგრაცია:** სრული, ინტერაქტიული მართვა და დიაგნოსტიკა სერიული კონსოლიდან.

## 2. 🏗️ არქიტექტურული პრინციპები

- **Event-driven არქიტექტურა:** მოდული აქტიურად იყენებს `Event Bus`-ს. ის როგორც **აქვეყნებს** ივენთებს საკუთარი სტატუსის შესახებ, ისე **გამოიწერს** სხვა მოდულების (მაგ., `provisioning`, `system_manager`) ივენთებს.
- **მდგრადობა (Persistence):** ქსელის მონაცემები (SSID და პაროლი) უსაფრთხოდ ინახება **NVS (Non-Volatile Storage)** მეხსიერებაში, რაც უზრუნველყოფს, რომ მოწყობილობამ გადატვირთვის შემდეგ ავტომატურად შეძლოს ქსელთან დაკავშირება.
- **ფონური ოპერაციები:** მოდული ეყრდნობა ESP-IDF-ის `event loop` სისტემას და FreeRTOS ტაიმერებს, რათა კავშირის მართვის ყველა ოპერაცია ასინქრონულად, ფონურ რეჟიმში შესრულდეს.
- **დეკაპლინგი (Decoupling):** მოდული არ არის პირდაპირ დამოკიდებული `command_router`-ზე. ის იყენებს `FMW_EVENT_SYSTEM_START_COMPLETE` ივენთს, რათა ბრძანებების რეგისტრაცია მოახდინოს მხოლოდ მას შემდეგ, რაც დარწმუნდება, რომ `command_router`-ის სერვისი უკვე ხელმისაწვდომია.

## 3. ⚙️ კონფიგურაცია

მოდულის საბაზისო კონფიგურაცია ხდება `system_config.json` ფაილში, თუმცა მისი პარამეტრების უმეტესობა იმართება `Kconfig`-ის (`menuconfig`) საშუალებით.

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
- **NVS Namespace:** NVS-ის სახელი, სადაც ინახება მონაცემები.

## 4. 📢 ივენთები (Events)

### გამოქვეყნებული ივენთები

`wifi_manager` აქვეყნებს შემდეგ ივენთებს, რომლებზეც სხვა მოდულებს შეუძლიათ გამოწერა:

| ივენთის სახელი | მონაცემები (Payload) | აღწერა |
|:---|:---|:---|
| `WIFI_EVENT_CONNECTED` | `NULL` | მოწყობილობა წარმატებით დაუკავშირდა WiFi ქსელს. |
| `WIFI_EVENT_DISCONNECTED` | `NULL` | მოწყობილობამ დაკარგა კავშირი WiFi ქსელთან. |
| `WIFI_EVENT_IP_ASSIGNED` | `esp_netif_ip_info_t*` | მოწყობილობამ მიიღო IP მისამართი DHCP სერვერისგან. |
| `WIFI_CREDENTIALS_NOT_FOUND` | `NULL` | NVS-ში ვერ მოიძებნა შენახული მონაცემები, საჭიროა provisioning. |

### გამოწერილი ივენთები

`wifi_manager` გამოიწერს შემდეგ ივენთებს:

| ივენთის სახელი | აღწერა |
|:---|:---|
| `PROV_CREDENTIALS_RECEIVED` | მიიღო ახალი WiFi მონაცემები `provisioning` მოდულისგან. |
| `FMW_EVENT_SYSTEM_START_COMPLETE` | სისტემის გაშვება დასრულდა, შესაძლებელია CLI ბრძანებების რეგისტრაცია. |

## 5. ⌨️ CLI ბრძანებები (Command Line Interface)

მოდული არეგისტრირებს `wifi` ბრძანებას, რომელიც იძლევა WiFi-ს სრული მართვისა და დიაგნოსტიკის საშუალებას.

| ბრძანება | არგუმენტები | აღწერა |
|:---|:---|:---|
| `wifi status` | - | ℹ️ გამოაქვს დეტალური ინფორმაცია: კავშირის სტატუსი, SSID, RSSI, IP, MAC და Hostname. |
| `wifi scan` | - | 📡 ასკანერებს და გამოაქვს ხელმისაწვდომი WiFi ქსელების სია. |
| `wifi connect` | `<ssid> <password>` | 🔗 ცდილობს მითითებულ ქსელთან დაკავშირებას და ინახავს მონაცემებს NVS-ში. |
| `wifi disconnect` | - | 🔌 წყვეტს მიმდინარე WiFi კავშირს. |
| `wifi enable` | - | ✅ რთავს `wifi_manager` მოდულს. |
| `wifi disable` | - | ❌ თიშავს `wifi_manager` მოდულს და წყვეტს კავშირს. |
| `wifi erase_creds` | - | 🗑️ შლის NVS-ში შენახულ ყველა WiFi მონაცემს. |
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
