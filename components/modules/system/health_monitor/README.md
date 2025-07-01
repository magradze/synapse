# მოდული: Health Monitor

## 1. მიმოხილვა

**მოდულის სახელი:** `health_monitor`  
**კატეგორია:** `system`  
**ვერსია:** 1.0.0  
**ავტორი:** Synapse Framework Team

`Health Monitor` მოდული არის Synapse Framework-ის სასიცოცხლოდ მნიშვნელოვანი კომპონენტი, რომელიც უზრუნველყოფს სისტემის მდგრადობის, საიმედოობისა და თვითდიაგნოსტიკის მექანიზმებს რეალურ დროში. ის მუდმივად აკვირდება სისტემის კრიტიკულ პარამეტრებს, როგორიცაა მეხსიერების გამოყენება და ტასკების მდგომარეობა, აღმოაჩენს ანომალიებს და ახდენს შესაბამის რეაგირებას ივენთების გამოქვეყნებით.

## 2. წინაპირობები და `menuconfig`

ამ მოდულის სრული ფუნქციონირებისთვის, განსაკუთრებით ტასკების სტატუსის მონიტორინგისთვის, აუცილებელია ESP-IDF-ის კონფიგურაციაში (`menuconfig`) შემდეგი ოფციების ჩართვა:

1. გაუშვით `idf.py menuconfig`.
2. გადადით `Component config` ---> `FreeRTOS` ---> `Kernel`.
3. ჩართეთ (`[y]`) შემდეგი ორი ოფცია:
    * `Enable FreeRTOS-specific stats formatting`
    * `Enable generation of run time stats`

**გაფრთხილება:** ამ ოფციების ჩართვის გარეშე, `uxTaskGetSystemState` ფუნქცია არ იქნება ხელმისაწვდომი და პროექტის კომპილაცია დასრულდება `undefined reference` შეცდომით.

## 3. არქიტექტურული პრინციპები

* **პერიოდული მონიტორინგი:** მოდულს აქვს საკუთარი FreeRTOS ტასკი, რომელიც კონფიგურირებადი ინტერვალით ამოწმებს სისტემის "პულსს".
* **ზღვრული მნიშვნელობები (Thresholds):** ყველა მონიტორინგის ქვეშ მყოფ პარამეტრს აქვს კონფიგურირებადი ზღვრული მნიშვნელობა.
* **ივენთებზე დაფუძნებული რეაგირება:** პრობლემის აღმოჩენისას, მოდული აქვეყნებს `SYSTEM_HEALTH_ALERT` ივენთს Event Bus-ზე.
* **გაფართოებადობა:** სხვა მოდულებს შეუძლიათ დაარეგისტრირონ საკუთარი "ჯანმრთელობის შემოწმების" ფუნქციები.
* **Service-driven:** მოდული აწვდის `health_api_t` სერვისს `Service Locator`-ის მეშვეობით.

## 4. კონფიგურაცია

მოდულის კონფიგურაცია ხდება `system_config.json` ფაილის საშუალებით.

**კონფიგურაციის მაგალითი:**

```json
{
    "type": "health_monitor",
    "enabled": true,
    "config": {
        "instance_name": "main_health_monitor",
        "check_interval_sec": 60,
        "thresholds": {
            "min_free_heap_kb": 40,
            "task_stack_watermark_percent": 90
        }
    }
}
```

**კონფიგურაციის პარამეტრები:**

| პარამეტრი | ტიპი | აღწერა | სავალდებულო | Default |
|:---|:---|:---|:---:|:---|
| `instance_name` | სტრიქონი | მოდულის ინსტანციის უნიკალური სახელი. | ❌ | `main_health_monitor`|
| `check_interval_sec` | რიცხვი | შემოწმების ინტერვალი წამებში. | ❌ | `30` |
| `thresholds` | ობიექტი | ზღვრული მნიშვნელობების კონტეინერი. | ❌ | - |
| `thresholds.min_free_heap_kb` | რიცხვი | თავისუფალი მეხსიერების მინიმალური ზღვარი კილობაიტებში. | ❌ | `50` |
| `thresholds.task_stack_watermark_percent` | რიცხვი | ტასკის სტეკის გამოყენების მაქსიმალური ზღვარი პროცენტებში. | ❌ | `85` |

## 5. Service API (`health_api_t`)

სხვა მოდულებს შეუძლიათ მიიღონ წვდომა `Health Monitor`-ის API-ზე `Service Locator`-ის მეშვეობით. სერვისის ტიპი: `FMW_SERVICE_TYPE_HEALTH_API`.

**API ფუნქციები:**

* **`esp_err_t get_system_health_report(cJSON **report);`**  
  აბრუნებს სისტემის ჯანმრთელობის სრულ რეპორტს cJSON ობიექტის სახით. რეპორტი შეიცავს ინფორმაციას მეხსიერების, uptime-ისა და ყველა აქტიური ტასკის შესახებ. **მნიშვნელოვანია:** გამომძახებელმა უნდა გაათავისუფლოს დაბრუნებული `cJSON` ობიექტი `cJSON_Delete()`-ის გამოყენებით.

* **`esp_err_t register_custom_check(const char* check_name, health_check_fn_t check_fn, void* context);`**  
  არეგისტრირებს ახალ, მომხმარებლის მიერ განსაზღვრულ შემოწმების ფუნქციას. ეს საშუალებას აძლევს სხვა მოდულებს, ჩართონ საკუთარი ლოგიკა `Health Monitor`-ის პერიოდულ შემოწმებებში.

* **`esp_err_t unregister_custom_check(const char* check_name);`**  
  შლის რეგისტრირებულ შემოწმებას. (ამჟამად არ არის იმპლემენტირებული).

## 6. გამოქვეყნებული ივენთები

* **`SYSTEM_HEALTH_ALERT`**
  * **როდის ქვეყნდება:** როდესაც რომელიმე მონიტორინგის ქვეშ მყოფი პარამეტრი გადასცდება დაწესებულ ზღვარს.
  * **Payload (cJSON სტრიქონი):**
    * **მეხსიერების პრობლემა:** `{"alert_type":"LOW_HEAP_MEMORY", "value_kb":35}`
    * **სტეკის პრობლემა:** `{"alert_type":"HIGH_STACK_USAGE", "task_name":"some_task", "usage_percent":92}`
    * **Custom შემოწმების პრობლემა:** `{"alert_type":"CUSTOM_CHECK_FAILED", "check_name":"my_db_check"}`

## 7. 💡 გამოყენების მაგალითი

`ota_update_module`-ს სურს, განახლების დაწყებამდე შეამოწმოს, არის თუ არა სისტემაში საკმარისი რესურსი.

```c
#include "service_locator.h"
#include "health_interface.h" // პირობითი ინტერფეისის ფაილი

void start_ota_if_healthy() {
    service_handle_t handle = fmw_service_get("main_health_monitor");
    if (!handle) {
        ESP_LOGW(TAG, "Health Monitor service not found. Skipping health check.");
        // ... განაგრძე OTA ...
        return;
    }

    health_api_t *health_api = (health_api_t *)handle;
    cJSON *report = NULL;
    
    if (health_api->get_system_health_report(&report) == ESP_OK && report) {
        const cJSON *heap = cJSON_GetObjectItem(report, "min_free_heap_bytes");
        if (cJSON_IsNumber(heap) && heap->valueint < 80000) { // თუ 80KB-ზე ნაკლებია
            ESP_LOGE(TAG, "Not enough memory for OTA update. Aborting.");
            cJSON_Delete(report);
            return;
        }
        cJSON_Delete(report);
    }

    // ... სისტემა ჯანმრთელია, ვიწყებთ OTA-ს ...
}
```
