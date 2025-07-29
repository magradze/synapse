# 📲 მოდული `ble_provisioning`

## 1. 📜 მიმოხილვა

**მოდულის სახელი:** `ble_provisioning`  
**კატეგორია:** `provisioning`  
**ვერსია:** 2.4.0 (განახლებულია)  
**ავტორი:** Synapse Framework Team

`ble_provisioning` მოდული უზრუნველყოფს მოწყობილობის პირველადი კონფიგურაციის (provisioning) პროცესს **Bluetooth Low Energy (BLE)**-ის საშუალებით. ის იყენებს ESP-IDF-ის სტანდარტულ `wifi_provisioning` მენეჯერს, რათა მოწყობილობამ შეძლოს უსაფრთხოდ მიიღოს WiFi ქსელის მონაცემები (SSID და პაროლი) მობილური აპლიკაციიდან (მაგ., Espressif-ის **ESP BLE Prov**).

მთავარი ცვლილებაა BLE სარეკლამო სახელის **დინამიური გენერაცია**, რომელიც იყენებს მოწყობილობის უნიკალურ ID-ს `device_identity_service`-იდან, რაც უზრუნველყოფს თითოეული მოწყობილობის უნიკალურ სახელს ქსელში.

## 2. 🏗️ არქიტექტურული პრინციპები

- **სერვისზე-ორიენტირებული (Service-driven):** მოდული არეგისტრირებს `ble_prov_api_t` სერვისს `Service Locator`-ში. ეს საშუალებას აძლევს სხვა მოდულებს, შეამოწმონ, მიმდინარეობს თუ არა provisioning-ის პროცესი, ან დაიწყონ ის იძულებით.
- **ივენთებზე-დაფუძნებული (Event-driven):** მოდული აქტიურად ურთიერთქმედებს `Event Bus`-თან:
  - **აქვეყნებს** ივენთებს, როგორიცაა `PROV_STARTED`, `PROV_ENDED` და `PROV_CREDENTIALS_RECEIVED`, რათა სისტემის სხვა ნაწილებმა იცოდნენ მისი მდგომარეობის შესახებ.
  - **გამოიწერს** ივენთებს, როგორიცაა `WIFI_EVENT_IP_ASSIGNED` (რათა ავტომატურად დაასრულოს პროცესი) და `PROV_START_REQUESTED` (რათა პროცესი დაიწყოს გარე მოთხოვნით).
- **მაღალი ინიციალიზაციის დონე (`init_level: 90`):** როგორც სერვისის **მომხმარებელი (Consumer)**, რომელიც დამოკიდებულია `device_identity_service`-ზე (`init_level: 20`), `ble_provisioning` მოდულის ინიციალიზაცია ხდება სისტემის გაშვების გვიან ეტაპზე. ეს უზრუნველყოფს, რომ ყველა საჭირო სერვისი გარანტირებულად ხელმისაწვდომია მისი მუშაობის დაწყებამდე.
- **დამოკიდებულება სერვისზე (Service Dependency):** მოდული პირდაპირ დამოკიდებულია `device_identity_service`-ზე, რათა მიიღოს მოწყობილობის უნიკალური ID. ეს არის ფრეიმვორქის "მომწოდებელი-მომხმარებლის" პატერნის კლასიკური მაგალითი.

## 3. ⚙️ კონფიგურაცია

მოდულის ძირითადი პარამეტრები იმართება `Kconfig`-ის (`idf.py menuconfig`) საშუალებით.

**`Kconfig` პარამეტრები:**

`Component config` -> `Synapse Framework` -> `Modules` -> `BLE Provisioning` მენიუში შეგიძლიათ დააკონფიგურიროთ:

- **`BLE Device Name`:** **სარეკლამო სახელის პრეფიქსი (Prefix)**, რომელსაც დაემატება მოწყობილობის უნიკალური ID. საბოლოო სახელი იქნება ფორმატით: `Prefix-DeviceID` (მაგ., `Synapse-5C013B4A9954`).
- **`Proof-of-Possession (PoP) String`:** საიდუმლო სტრიქონი, რომელიც გამოიყენება უსაფრთხოების `Security 1` რეჟიმში.
- **`Security Version`:** უსაფრთხოების პროტოკოლის ვერსია (0 - დაუშიფრავი, 1 - დაშიფრული PoP-ით).

**`config.json` მაგალითი:**

`config.json`-ის საშუალებით შესაძლებელია `Kconfig`-ის პარამეტრების გადაფარვა. **ყურადღება:** `device_name` პარამეტრი აღარ გამოიყენება, რადგან სახელი დინამიურად გენერირდება.

```json
[
  {
    "type": "ble_provisioning",
    "enabled": true,
    "config": {
      "instance_name": "main_ble_provisioning",
      "pop": "secret123",
      "security_version": 1
    }
  }
]
```

## 4. 🛠️ Service API (`ble_prov_api_t`)

მოდული არეგისტრირებს სერვისს, რომლის მიღებაც შესაძლებელია `fmw_service_get`-ის გამოძახებით, `config.json`-ში მითითებული `instance_name`-ის გამოყენებით (მაგ., `"main_ble_provisioning"`).

**API ფუნქციები:**

- **`bool is_provisioning_active(void);`**  
    ❓ ამოწმებს, არის თუ არა provisioning-ის პროცესი ამჟამად აქტიური.
- **`esp_err_t start_provisioning(void);`**  
    🚀 იწყებს provisioning-ის პროცესს პროგრამულად.

## 5. 📢 ივენთები (Events)

(ეს სექცია უცვლელია)

### გამოქვეყნებული ივენთები

| ივენთის სახელი | მონაცემები (Payload) | აღწერა |
|:---|:---|:---|
| `PROV_STARTED` | `NULL` | BLE provisioning-ის პროცესი დაიწყო. |
| `PROV_CREDENTIALS_RECEIVED` | `char*` (JSON) | მიღებულია WiFi-ს მონაცემები. |
| `PROV_ENDED` | `NULL` | Provisioning-ის პროცესი დასრულდა. |

### გამოწერილი ივენთები

| ივენთის სახელი | რეაქცია |
|:---|:---|
| `WIFI_EVENT_IP_ASSIGNED` | ასრულებს პროცესს, რადგან კავშირი წარმატებულია. |
| `PROV_START_REQUESTED` | იწყებს provisioning-ის პროცესს. |

## 6. 💡 გამოყენების მაგალითი

(ეს სექცია უცვლელია)

`rgb_led_indicator` მოდული ამოწმებს, არის თუ არა provisioning აქტიური, რათa შესაბამისი ინდიკაცია აანთოს.

**`rgb_led_indicator.c`-ში:**

```c
#include "service_locator.h"
#include "ble_prov_interface.h"

static esp_err_t rgb_led_indicator_start(module_t *self) {
    // ...
    service_handle_t prov_handle = fmw_service_get("main_ble_prov");
    if (prov_handle) {
        ble_prov_api_t *prov_api = (ble_prov_api_t *)prov_handle;
        if (prov_api->is_provisioning_active()) {
            // ...
        }
    }
    // ...
    return ESP_OK;
}
```

## 7. ❗ მნიშვნელოვანი შენიშვნები

- **დამოკიდებულება:** ამ მოდულის გამართული მუშაობისთვის, სისტემაში აუცილებლად უნდა იყოს ჩართული და კონფიგურირებული `device_identity_service` მოდული.
- **უსაფრთხოება:** `Security 1` რეჟიმის გამოყენება (რომელიც default-ია) მკაცრად რეკომენდებულია რეალურ პროდუქტებში.
