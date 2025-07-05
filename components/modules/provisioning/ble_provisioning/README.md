# 📲 მოდული `ble_provisioning`

## 1. 📜 მიმოხილვა

**მოდულის სახელი:** `ble_provisioning`  
**კატეგორია:** `provisioning`  
**ვერსია:** 2.3.1  
**ავტორი:** Synapse Framework Team

`ble_provisioning` მოდული უზრუნველყოფს მოწყობილობის პირველადი კონფიგურაციის (provisioning) პროცესს **Bluetooth Low Energy (BLE)**-ის საშუალებით. ის იყენებს ESP-IDF-ის სტანდარტულ `wifi_provisioning` მენეჯერს, რათა მოწყობილობამ შეძლოს უსაფრთხოდ მიიღოს WiFi ქსელის მონაცემები (SSID და პაროლი) მობილური აპლიკაციიდან (მაგ., Espressif-ის **ESP BLE Prov**).

მოდული ავტომატურად იწყებს მუშაობას, თუ მოწყობილობა არ არის კონფიგურირებული, და ასევე, მისი გაშვება შესაძლებელია პროგრამულად, სხვა მოდულების მიერ.

## 2. 🏗️ არქიტექტურული პრინციპები

-   **სერვისზე-ორიენტირებული (Service-driven):** მოდული არეგისტრირებს `ble_prov_api_t` სერვისს `Service Locator`-ში. ეს საშუალებას აძლევს სხვა მოდულებს, შეამოწმონ, მიმდინარეობს თუ არა provisioning-ის პროცესი, ან დაიწყონ ის იძულებით.
-   **ივენთებზე-დაფუძნებული (Event-driven):** მოდული აქტიურად ურთიერთქმედებს `Event Bus`-თან:
    -   **აქვეყნებს** ივენთებს, როგორიცაა `PROV_STARTED`, `PROV_ENDED` და `PROV_CREDENTIALS_RECEIVED`, რათა სისტემის სხვა ნაწილებმა იცოდნენ მისი მდგომარეობის შესახებ.
    -   **გამოიწერს** ივენთებს, როგორიცაა `WIFI_EVENT_IP_ASSIGNED` (რათა ავტომატურად დაასრულოს პროცესი) და `PROV_START_REQUESTED` (რათა პროცესი დაიწყოს გარე მოთხოვნით).
-   **დაბალი ინიციალიზაციის დონე (`init_level: 2`):** როგორც ერთ-ერთი ფუნდამენტური სერვისი, ის ინიციალიზდება ძალიან ადრე, რათა სხვა მოდულებმა (მაგ., `rgb_led_indicator`) შეძლონ მისი სტატუსის შემოწმება თავიანთ `start` ფაზაში.
-   **მდგომარეობის მართვა:** მოდული ინახავს თავის შიდა მდგომარეობას (`is_active` ფლაგი), რათა თავიდან აიცილოს provisioning-ის პროცესის განმეორებითი ან არასაჭირო გაშვება.

## 3. ⚙️ კონფიგურაცია

მოდულის ძირითადი პარამეტრები იმართება `Kconfig`-ის (`idf.py menuconfig`) საშუალებით, რაც უზრუნველყოფს compile-time ოპტიმიზაციას.

**`Kconfig` პარამეტრები:**

`Component config` -> `Synapse Framework` -> `Modules` -> `BLE Provisioning` მენიუში შეგიძლიათ დააკონფიგურიროთ:
-   **`Provisioning BLE Device Name`:** სახელი, რომელიც გამოჩნდება BLE სკანირებისას (მაგ., `Synapse-Prov`).
-   **`Proof-of-Possession (PoP) String`:** საიდუმლო სტრიქონი, რომელიც გამოიყენება უსაფრთხოების `Security 1` რეჟიმში. ის უნდა ემთხვეოდეს მობილურ აპლიკაციაში მითითებულ PoP-ს.
-   **`Security Version`:** უსაფრთხოების პროტოკოლის ვერსია (0 - დაუშიფრავი, 1 - დაშიფრული PoP-ით).

**`system_config.json` მაგალითი:**

`system_config.json`-ის საშუალებით შესაძლებელია `Kconfig`-ის პარამეტრების გადაფარვა.

```json
{
    "type": "ble_provisioning",
    "enabled": true,
    "config": {
        "instance_name": "main_ble_prov",
        "device_name": "MyCustomDevice-123",
        "pop": "secret123",
        "security_version": 1
    }
}
```

## 4. 🛠️ Service API (`ble_prov_api_t`)

მოდული არეგისტრირებს სერვისს, რომლის მიღებაც შესაძლებელია `fmw_service_get("main_ble_prov")` გამოძახებით.

**API ფუნქციები:**

-   **`bool is_provisioning_active(void);`**  
    ❓ ამოწმებს, არის თუ არა provisioning-ის პროცესი ამჟამად აქტიური. აბრუნებს `true`-ს, თუ მოწყობილობა ელოდება კლიენტის დაკავშირებას.

-   **`esp_err_t start_provisioning(void);`**  
    🚀 იწყებს provisioning-ის პროცესს პროგრამულად. სასარგებლოა, როდესაც გსურთ პროცესის ხელახლა გაშვება, მაგალითად, ღილაკზე დაჭერით ან CLI ბრძანებით.

## 5. 📢 ივენთები (Events)

### გამოქვეყნებული ივენთები

| ივენთის სახელი | მონაცემები (Payload) | აღწერა |
|:---|:---|:---|
| `PROV_STARTED` | `NULL` | BLE provisioning-ის პროცესი დაიწყო და მოწყობილობა ხილვადია. |
| `PROV_CREDENTIALS_RECEIVED` | `char*` (JSON) | მიღებულია WiFi-ს მონაცემები. Payload არის JSON სტრიქონი: `{"ssid":"...", "password":"..."}`. |
| `PROV_ENDED` | `NULL` | Provisioning-ის პროცესი დასრულდა (წარმატებით ან შეცდომით). |

### გამოწერილი ივენთები

| ივენთის სახელი | რეაქცია |
|:---|:---|
| `WIFI_EVENT_IP_ASSIGNED` | თუ provisioning აქტიურია, ასრულებს პროცესს, რადგან კავშირი წარმატებულია. |
| `PROV_START_REQUESTED` | იწყებს provisioning-ის პროცესს. |

## 6. 💡 გამოყენების მაგალითი

`rgb_led_indicator` მოდული ამოწმებს, არის თუ არა provisioning აქტიური, რათა შესაბამისი ინდიკაცია აანთოს.

**`rgb_led_indicator.c`-ში:**

```c
#include "service_locator.h"
#include "ble_prov_interface.h"

static esp_err_t rgb_led_indicator_start(module_t *self) {
    // ... (ტასკის შექმნა და სხვა ლოგიკა) ...

    // 1. 🚦 ვიღებთ Provisioning სერვისს
    service_handle_t prov_handle = fmw_service_get("main_ble_prov");
    if (prov_handle) {
        ble_prov_api_t *prov_api = (ble_prov_api_t *)prov_handle;

        // 2. ❓ ვამოწმებთ სტატუსს
        if (prov_api->is_provisioning_active()) {
            ESP_LOGI(TAG, "Provisioning is active. Setting LED to pulsing yellow.");
            // 3. 💛 ვიწყებთ ყვითელი ფერის პულსაციას
            led_command_t cmd = {LED_MODE_PULSE, 255, 165, 0, 3000, false};
            send_command_to_task(self, &cmd);
        }
    }
    
    // ...
    return ESP_OK;
}
```

## 7. ❗ მნიშვნელოვანი შენიშვნები

-   **დამოკიდებულება `CMakeLists.txt`-ში:** ნებისმიერმა მოდულმა, რომელიც იყენებს `ble_provisioning`-ის სერვისს, თავის `CMakeLists.txt` ფაილში, `REQUIRES` სექციაში უნდა დაამატოს `interfaces` (და არა `ble_provisioning`, რადგან ინტერფეისი ცენტრალიზებულია).
-   **უსაფრთხოება:** `Security 1` რეჟიმის გამოყენება (რომელიც default-ია) მკაცრად რეკომენდებულია რეალურ პროდუქტებში. დარწმუნდით, რომ `PoP` სტრიქონი უსაფრთხო და უნიკალურია.