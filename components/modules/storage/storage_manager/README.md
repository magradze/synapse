# 💾 მოდული `storage_manager`

## 1. 📜 მიმოხილვა

**მოდულის სახელი:** `storage_manager`  
**კატეგორია:** `storage`  
**ვერსია:** 2.0.0  
**ავტორი:** Synapse Framework Team

`storage_manager` არის Synapse Framework-ის **უნივერსალური, ცენტრალიზებული მონაცემთა შენახვის სერვისი**. ის უზრუნველყოფს ერთიან, აბსტრაქტულ API-ს სხვადასხვა ფიზიკურ საცავთან (backend) სამუშაოდ, როგორიცაა:

- **NVS (Non-Volatile Storage):** ⚡️ იდეალურია მცირე ზომის, ხშირად ცვლადი key-value მონაცემებისთვის (მაგ., კონფიგურაციები, მრიცხველები).
- **SPIFFS:** 🗂️ ფაილური სისტემა შიდა flash მეხსიერებაზე, შესაფერისია ვებ-სერვერის რესურსების, ლოგების ან საშუალო ზომის ფაილების შესანახად.
- **SD Card:** 🗃️ დიდი მოცულობის ფაილური საცავი გარე SD ბარათზე, გამოიყენება ვიდეოების, სენსორების დიდი ისტორიის და სხვა მოცულობითი მონაცემებისთვის.

მოდულის მთავარი მიზანია, სრულად მოახდინოს შენახვის ტექნოლოგიის აბსტრაგირება, რათა სხვა მოდულებმა გამოიყენონ ერთიანი, მარტივი ინტერფეისი ისე, რომ არ "ფიქრობდნენ" იმაზე, თუ სად და როგორ ინახება მონაცემები ფიზიკურად.

## 2. 🏗️ არქიტექტურული პრინციპები

- **Strategy Pattern:** მოდული იყენებს Strategy დიზაინ-პატერნს. `Kconfig`-ში არჩეული backend-ის მიხედვით, ის დინამიურად ირჩევს შესაბამის "სტრატეგიას" (იმპლემენტაციას) და მის API-ს არეგისტრირებს `Service Locator`-ში.
- **სერვისზე-ორიენტირებული (Service-driven):** მოდული არის სუფთა სერვისის მომწოდებელი. ის არეგისტრირებს `storage_api_t` სერვისს და არ არის დამოკიდებული სხვა აპლიკაციის დონის მოდულებზე.
- **იზოლაცია და Namespace:**
  - **NVS Backend:** იყენებს "მფლობელის" (`owner`) პარამეტრს, როგორც NVS-ის **namespace**-ს, რაც გამორიცხავს გასაღებების კონფლიქტს.
  - **SPIFFS/SD Card Backends:** იყენებს `owner`-ს, როგორც **დირექტორიის სახელს**, რათა მოახდინოს ფაილების ლოგიკური დაჯგუფება და იზოლირება.
- **დაბალი ინიციალიზაციის დონე (`init_level: 20`):** როგორც ფუნდამენტური სერვისი, ის ინიციალიზდება სისტემის გაშვების ძალიან ადრეულ ეტაპზე.
- **ერთიანი API:** მიუხედავად არჩეული backend-ისა, სხვა მოდულები ყოველთვის იყენებენ ერთსა და იმავე `storage_api_t` ინტერფეისს, რომელიც მხარს უჭერს როგორც key-value, ისე ფაილურ ოპერაციებს.

## 3. ⚙️ კონფიგურაცია

მოდულის სრული კონფიგურაცია ხდება `Kconfig`-ის (`idf.py menuconfig`) საშუალებით, სადაც შეგიძლიათ აირჩიოთ backend-ი და დააკონფიგურიროთ მისი პარამეტრები.

**`system_config.json` მაგალითი:**

```json
{
    "type": "storage_manager",
    "enabled": true,
    "config": {
        "instance_name": "main_storage"
    }
}
```

**`Kconfig` პარამეტრები:**

`Component config` -> `Synapse Framework` -> `Modules` -> `Storage Manager` მენიუში შეგიძლიათ:

1. **აირჩიოთ Storage Backend:** NVS, SPIFFS, ან SD Card.
2. **დააკონფიგურიროთ SPIFFS:** მიუთითოთ პარტიციის სახელი, მაქსიმალური ფაილების რაოდენობა და ა.შ.
3. **დააკონფიგურიროთ SD Card:** მიუთითოთ mount point-ი და SPI პინების ნომრები (MISO, MOSI, CLK, CS).

## 4. 🛠️ Service API (`storage_api_t`)

მოდული არეგისტრირებს სერვისს ტიპით `FMW_SERVICE_TYPE_NVRAM_API`. სხვა მოდულებს შეუძლიათ ამ სერვისის მიღება `fmw_service_get("main_storage")` გამოძახებით.

### Key-Value ოპერაციები

- **`set_string(owner, key, value)` / `get_string(owner, key, ...)`**: 💾/📥 ინახავს/კითხულობს სტრიქონს.
- **`set_i32(owner, key, value)` / `get_i32(owner, key, ...)`**: 💾/📥 ინახავს/კითხულობს 32-ბიტიან რიცხვს.
- **`set_bool(owner, key, value)` / `get_bool(owner, key, ...)`**: 💾/📥 ინახავს/კითხულობს ლოგიკურ მნიშვნელობას.
- **`erase_key(owner, key)`**: 🗑️ შლის კონკრეტულ key-value წყვილს.

### ფაილური ოპერაციები (მხარდაჭერილია SPIFFS და SD Card backend-ებზე)

- **`write_file(file_path, data, size)`**: 📝 წერს მონაცემებს ფაილში. ქმნის ფაილს თუ არ არსებობს, ან გადააწერს არსებულს.
- **`read_file(file_path, buffer, buffer_size)`**: 📖 კითხულობს ფაილის სრულ შიგთავსს.
- **`delete_file(file_path)`**: 🗑️ შლის მითითებულ ფაილს.
- **`file_exists(file_path)`**: ❓ ამოწმებს, არსებობს თუ არა ფაილი.

**შენიშვნა:** თუ არჩეულია NVS backend-ი, ფაილური ოპერაციების გამოძახება დააბრუნებს `ESP_ERR_NOT_SUPPORTED` შეცდომას.

## 5. 💡 გამოყენების მაგალითი

წარმოვიდგინოთ, რომ `web_server` მოდულს სურს, შეინახოს `index.html` ფაილი, ხოლო `wifi_manager`-ს - თავისი კონფიგურაცია.

**`web_server.c`-ში (SPIFFS/SD არჩეულია):**

```c
#include "service_locator.h"
#include "storage_interface.h"

void save_homepage() {
    storage_api_t *storage = (storage_api_t *)fmw_service_get("main_storage");
    if (!storage) return;

    const char* html_content = "<h1>Welcome to Synapse!</h1>";
    // ფაილური API-ს გამოყენება
    esp_err_t err = storage->write_file("/spiffs/web/index.html", html_content, strlen(html_content));
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Homepage saved successfully!");
    }
}
```

**`wifi_manager.c`-ში (ნებისმიერი backend არჩეულია):**

```c
#include "service_locator.h"
#include "storage_interface.h"

void save_wifi_ssid(const char* ssid) {
    storage_api_t *storage = (storage_api_t *)fmw_service_get("main_storage");
    if (!storage) return;

    // Key-Value API-ს გამოყენება
    esp_err_t err = storage->set_string("wifi_manager", "ssid", ssid);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "WiFi SSID saved successfully!");
    }
}
```

ამ მაგალითში, `wifi_manager`-მა არც კი "იცის", სად ინახება მისი `ssid` - NVS-ში თუ `/spiffs/wifi_manager/ssid` ფაილში. ეს არის `Storage Manager`-ის აბსტრაქციის მთავარი ძალა.

## 6. ❗ მნიშვნელოვანი შენიშვნები

- **Backend-ის არჩევა:** `menuconfig`-ში სწორად უნდა აირჩიოთ და დააკონფიგურიროთ სასურველი backend-ი.
- **პარტიციების ცხრილი:** SPIFFS-ის გამოსაყენებლად, `partitions.csv` ფაილში აუცილებლად უნდა გქონდეთ შესაბამისი პარტიცია (მაგ., `spiffs, data, spiffs, , 1M,`).
- **დამოკიდებულება `CMakeLists.txt`-ში:** ნებისმიერმა მოდულმა, რომელიც იყენებს `Storage Manager`-ს, თავის `CMakeLists.txt` ფაილში, `REQUIRES` სექციაში უნდა დაამატოს `storage_manager`.
