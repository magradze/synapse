# ⏱️ მოდული `system_timer`

## 1. 📜 მიმოხილვა

**მოდულის სახელი:** `system_timer`  
**კატეგორია:** `utils`  
**ვერსია:** 1.0.0  
**ავტორი:** Synapse Framework Team

`system_timer` არის Synapse Framework-ის ცენტრალიზებული, სერვისზე-დაფუძნებული მოდული, რომელიც უზრუნველყოფს დროზე-დაფუძნებული ივენთების დაგეგმვას. ის წარმოადგენს ESP-IDF-ის მაღალი რეზოლუციის **`esp_timer`** კომპონენტის მაღალი დონის აბსტრაქციას.

მოდულის მთავარი მიზანია, გაამარტივოს პერიოდული ან ერთჯერადი ამოცანების შესრულება. იმის ნაცვლად, რომ თითოეულმა მოდულმა თავად შექმნას და მართოს FreeRTOS ტაიმერები, მათ შეუძლიათ, უბრალოდ, "სთხოვონ" `system_timer`-ს, რომ নির্দিষ্ট დროის შემდეგ `Event Bus`-ზე გამოაქვეყნოს კონკრეტული ივენთი.

## 2. 🏗️ არქიტექტურული პრინციპები

- **სერვისზე-ორიენტირებული (Service-driven):** მოდული არის სუფთა სერვისის მომწოდებელი. ის არეგისტრირებს `timer_api_t` სერვისს `Service Locator`-ში და არ არის დამოკიდებული სხვა აპლიკაციის დონის მოდულებზე.
- **დეკაპლინგი (Decoupling):** ეს მოდული ხელს უწყობს მოდულებს შორის კავშირების მაქსიმალურ შემცირებას. მოდულს, რომელსაც სჭირდება პერიოდული მოქმედება, აღარ სჭირდება callback ფუნქციის შექმნა და გადაცემა. ის უბრალოდ ითხოვს ივენთის დაგეგმვას და შემდეგ თავის სტანდარტულ `handle_event` ფუნქციაში ამუშავებს მას.
- **რესურსების ეფექტურობა:** მოდული იყენებს `esp_timer`-ს, რომელიც უფრო მსუბუქი და ეფექტურია, ვიდრე FreeRTOS ტაიმერები, განსაკუთრებით დიდი რაოდენობით ტაიმერების შემთხვევაში. ის ასევე ზღუდავს ერთდროულად აქტიური ტაიმერების რაოდენობას (`CONFIG_SYSTEM_TIMER_MAX_TIMERS`), რაც იცავს სისტემას რესურსების გადატვირთვისგან.
- **საშუალო ინიციალიზაციის დონე (`init_level: 35`):** როგორც სისტემური სერვისი, ის ინიციალიზდება ბირთვის ძირითადი კომპონენტების შემდეგ, მაგრამ აპლიკაციის დონის მოდულების უმეტესობამდე, რათა მათ შეძლონ მისი გამოყენება თავიანთ `start` ფაზაში.

## 3. ⚙️ კონფიგურაცია

მოდულის კონფიგურაცია ხდება `Kconfig`-ის (`idf.py menuconfig`) საშუალებით.

**`Kconfig` პარამეტრები:**

`Component config` -> `Synapse Framework` -> `Modules` -> `System Timer Service` მენიუში შეგიძლიათ დააკონფიგურიროთ:

- **`Maximum number of concurrent timers`:** ერთდროულად აქტიური ტაიმერების მაქსიმალური რაოდენობა. თითოეული დაგეგმილი ივენთი იკავებს ერთ სლოტს.

**`system_config.json` მაგალითი:**

```json
{
    "type": "system_timer",
    "enabled": true,
    "config": {
        "instance_name": "main_timer_service"
    }
}
```

## 4. 🛠️ Service API (`timer_api_t`)

მოდული არეგისტრირებს სერვისს ტიპით `FMW_SERVICE_TYPE_TIMER_API`.

**API ფუნქციები:**

- **`fmw_timer_handle_t schedule_event(const char* event_name, uint32_t interval_ms, bool is_periodic);`**  
    🗓️ გეგმავს ივენთის გამოქვეყნებას.
  - `event_name`: ივენთის სახელი, რომელიც უნდა გამოქვეყნდეს. **ყურადღება:** ეს სტრიქონი უნდა იყოს პერმანენტული (მაგ., `#define` ან `static const char*`).
  - `interval_ms`: ინტერვალი მილიწამებში.
  - `is_periodic`: თუ `true`, ივენთი პერიოდულად გამოქვეყნდება. თუ `false`, ის ერთჯერადი იქნება.
  - **აბრუნებს:** უნიკალურ `handle`-ს, რომელიც შეიძლება გამოყენებულ იქნას ტაიმერის გასაუქმებლად, ან `NULL`-ს შეცდომის შემთხვევაში.

- **`esp_err_t cancel_event(fmw_timer_handle_t handle);`**  
    ❌ აუქმებს ადრე დაგეგმილ ივენთს მისი `handle`-ის გამოყენებით.

## 5. 💡 გამოყენების მაგალითი

წარმოვიდგინოთ, რომ `dht22_sensor` მოდულს სურს, ყოველ 60 წამში ერთხელ წაიკითხოს სენსორის მონაცემები.

**`dht22_sensor.c`-ში:**

```c
#include "service_locator.h"
#include "timer_interface.h" // ტაიმერის სერვისის ინტერფეისი

// 1. განვსაზღვროთ ივენთის სახელი, როგორც პერმანენტული სტრიქონი
#define EVT_READ_SENSOR "EVT_READ_DHT22_SENSOR"

// ...

// 2. start ფუნქციაში დავგეგმოთ ივენთი
static esp_err_t dht22_start(module_t *self) {
    // ...
    
    // ვიღებთ ტაიმერის სერვისს
    service_handle_t timer_service_handle = fmw_service_get("main_timer_service");
    if (timer_service_handle) {
        timer_api_t *timer_api = (timer_api_t *)timer_service_handle;
        
        // ვგეგმავთ პერიოდულ ივენთს ყოველ 60 წამში
        fmw_timer_handle_t sensor_timer = timer_api->schedule_event(EVT_READ_SENSOR, 60000, true);
        
        if (sensor_timer) {
            ESP_LOGI(TAG, "Sensor reading scheduled successfully.");
            // შეგვიძლია, შევინახოთ sensor_timer handle, თუ მომავალში მისი გაუქმება დაგვჭირდება
        } else {
            ESP_LOGE(TAG, "Failed to schedule sensor reading.");
        }
    } else {
        ESP_LOGE(TAG, "System Timer service not found!");
    }
    
    // ...
    return ESP_OK;
}

// 3. handle_event ფუნქციაში დავამუშაოთ ივენთი
static void dht22_handle_event(module_t *self, const char *event_name, void *event_data) {
    if (strcmp(event_name, EVT_READ_SENSOR) == 0) {
        // აქ ვწერთ სენსორის წაკითხვის და Event Bus-ზე გამოქვეყნების ლოგიკას
        ESP_LOGI(TAG, "Timer fired! Reading sensor data...");
        read_and_publish_dht22_data();
    }
    
    // არ დაგვავიწყდეს wrapper-ის გათავისუფლება (თუმცა ამ შემთხვევაში ის NULL-ია)
    if (event_data) {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}
```

## 6. ❗ მნიშვნელოვანი შენიშვნები

- **`event_name`-ის სიცოცხლის ციკლი:** `schedule_event` ფუნქციას გადაცემული `event_name` სტრიქონი არ კოპირდება. მოდული ინახავს მხოლოდ მის მაჩვენებელს. ამიტომ, ეს სტრიქონი უნდა იყოს ვალიდური ტაიმერის მთელი სიცოცხლის განმავლობაში. საუკეთესო პრაქტიკაა `#define`-ის ან `static const char*`-ის გამოყენება.
- **დამოკიდებულება `CMakeLists.txt`-ში:** ნებისმიერმა მოდულმა, რომელიც იყენებს `System Timer`-ს, თავის `CMakeLists.txt` ფაილში, `REQUIRES` სექციაში უნდა დაამატოს `system_timer`.
