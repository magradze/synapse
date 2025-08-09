# 🩺 მოდული: Health Monitor

**🗂️ კატეგორია:** `system`  
**✒️ ავტორი:** Synapse Framework Team  
**🔖 ვერსია:** 2.0.0

## 1. 📜 მიმოხილვა

`Health Monitor` მოდული არის Synapse Framework-ის **სისტემის მდგრადობის მცველი**. მისი მთავარი ამოცანაა, ფონურ რეჟიმში, რეალურ დროში აკონტროლოს სისტემის კრიტიკული პარამეტრები, აღმოაჩინოს პოტენციური პრობლემები (როგორიცაა მეხსიერების გაჟონვა ან ტასკების სტეკის გადავსება) მათ კრიტიკულ ფაზაში გადასვლამდე და შეატყობინოს სისტემის სხვა ნაწილებს მოსალოდნელი საფრთხის შესახებ.

ეს მოდული არის პრევენციული დიაგნოსტიკის ფუნდამენტური ინსტრუმენტი, რომელიც მნიშვნელოვნად ზრდის მოწყობილობის საიმედოობას გრძელვადიან პერსპექტივაში.

## 2. ❗ წინაპირობები და `menuconfig`

ამ მოდულის სრული ფუნქციონირებისთვის, განსაკუთრებით ტასკების სტატუსის მონიტორინგისთვის, აუცილებელია ESP-IDF-ის კონფიგურაციაში (`menuconfig`) შემდეგი ოფციების ჩართვა:

1. გაუშვით `idf.py menuconfig`.
2. გადადით `Component config` ---> `FreeRTOS` --->
3. ჩართეთ (`[y]`) შემდეგი ოფცია:
    * `Enable generation of run time stats`

**გაფრთხილება:** ამ ოფციის ჩართვის გარეშე, `uxTaskGetSystemState` ფუნქცია არ იქნება ხელმისაწვდომი და პროექტის კომპილაცია დასრულდება `undefined reference` შეცდომით.

## 3. 🏛️ არქიტექტურული პრინციპები

* **რესურსების ოპტიმიზაცია:** მოდული **არ ქმნის საკუთარ ტასკს**. ამის ნაცვლად, ის თავის პერიოდულ შემოწმების ლოგიკას არეგისტრირებს `core`-ის **`Task Pool Manager`**-ში, როგორც "დავალება" (`Job`). ეს მიდგომა ზოგავს ~3KB ოპერატიულ მეხსიერებას და ამცირებს სისტემის საერთო დატვირთვას.
* **ზღვრული მნიშვნელობები (Thresholds):** ყველა მონიტორინგის ქვეშ მყოფ პარამეტრს აქვს კონფიგურირებადი ზღვრული მნიშვნელობა.
* **ივენთებზე დაფუძნებული რეაგირება:** პრობლემის აღმოჩენისას, მოდული აქვეყნებს `SYSTEM_HEALTH_ALERT` ივენთს `Event Bus`-ზე, რაზეც სხვა მოდულებს (მაგ., `alarms_manager`) შეუძლიათ რეაგირება.
* **გაფართოებადობა:** სხვა მოდულებს შეუძლიათ, დაარეგისტრირონ საკუთარი "ჯანმრთელობის შემოწმების" ფუნქციები `Service API`-ს მეშვეობით.
* **კონტექსტზე-დამოკიდებულება:** მოდული "ჭკვიანია" და შეუძლია, დროებით გათიშოს გარკვეული შემოწმებები (მაგ., მეხსიერების კონტროლი `provisioning`-ის დროს), რათა თავიდან აიცილოს ცრუ განგაშები.

## 4. ⚙️ კონფიგურაცია

მოდულის კონფიგურაცია ხდება მისი `config.json` ფაილის საშუალებით.

**კონფიგურაციის მაგალითი:**

```json
[
    {
        "type": "health_monitor",
        "enabled": true,
        "config": {
            "instance_name": "health_monitor",
            "check_interval_sec": 60,
            "thresholds": {
                "min_free_heap_kb": 20,
                "min_task_stack_hwm_bytes": 256
            }
        }
    }
]
```

**კონფიგურაციის პარამეტრები:**

| პარამეტრი | ტიპი | აღწერა | სავალდებულო | Default |
|:---|:---|:---|:---:|:---|
| `instance_name` | სტრიქონი | მოდულის ინსტანციის უნიკალური სახელი. | ✅ | - |
| `check_interval_sec` | რიცხვი | შემოწმების ინტერვალი წამებში. | ❌ | `60` |
| `thresholds` | ობიექტი | ზღვრული მნიშვნელობების კონტეინერი. | ❌ | - |
| `thresholds.min_free_heap_kb` | რიცხვი | თავისუფალი მეხსიერების მინიმალური ზღვარი კილობაიტებში. | ❌ | `20` |
| `thresholds.min_task_stack_hwm_bytes` | რიცხვი | ტასკის სტეკის დარჩენილი თავისუფალი ადგილის მინიმალური ზღვარი ბაიტებში. | ❌ | `256` |

## 5. 🔌 Service API (`health_api_t`)

სხვა მოდულებს შეუძლიათ მიიღონ წვდომა `Health Monitor`-ის API-ზე `Service Locator`-ის მეშვეობით. სერვისის ტიპი: `SYNAPSE_SERVICE_TYPE_HEALTH_API`.

**API ფუნქციები:**

* **`esp_err_t get_system_health_report(cJSON **report);`**  
    აბრუნებს სისტემის ჯანმრთელობის სრულ რეპორტს cJSON ობიექტის სახით. რეპორტი შეიცავს ინფორმაციას მეხსიერების, uptime-ისა და ყველა აქტიური ტასკის შესახებ. **მნიშვნელოვანია:** გამომძახებელმა უნდა გაათავისუფლოს დაბრუნებული `cJSON` ობიექტი `cJSON_Delete()`-ის გამოყენებით.

* **`esp_err_t register_custom_check(const char* check_name, health_check_fn_t check_fn, void* context);`**  
    არეგისტრირებს ახალ, მომხმარებლის მიერ განსაზღვრულ შემოწმების ფუნქციას.

* **`esp_err_t unregister_custom_check(const char* check_name);`**  
    შლის რეგისტრირებულ შემოწმებას.

## 6. 📢 ივენთები

### გამოქვეყნებული ივენთები

* **`SYSTEM_HEALTH_ALERT`**
  * **როდის ქვეყნდება:** როდესაც რომელიმე მონიტორინგის ქვეშ მყოფი პარამეტრი გადასცდება დაწესებულ ზღვარს.
  * **Payload (cJSON სტრიქონი):**
    * **მეხსიერების პრობლემა:** `{"alert_type":"LOW_HEAP_MEMORY", "value_kb":18}`
    * **სტეკის პრობლემა:** `{"alert_type":"LOW_STACK_SPACE", "task_name":"some_task", "remaining_bytes":200}`
    * **Custom შემოწმების პრობლემა:** `{"alert_type":"CUSTOM_CHECK_FAILED", "check_name":"my_db_check"}`

### გამოწერილი ივენთები

* **`PROV_STARTED`**: ამ ივენთის მიღებისას, მოდული დროებით თიშავს მეხსიერების შემოწმებას.
* **`PROV_ENDED`**: ამ ივენთის მიღებისას, მოდული ანახლებს მეხსიერების შემოწმებას.

## 7. 💡 გამოყენების მაგალითი

`ota_update_manager`-ს სურს, განახლების დაწყებამდე შეამოწმოს, არის თუ არა სისტემაში საკმარისი რესურსი.

```c
#include "synapse.h"
#include "health_interface.h"

void start_ota_if_healthy() {
    // ვიყენებთ lookup_by_type-ს, რათა თავიდან ავიცილოთ hardcoded სახელი
    service_handle_t handle = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_HEALTH_API);
    if (!handle) {
        ESP_LOGW(TAG, "Health Monitor service not found. Skipping health check.");
        // ... განაგრძე OTA ...
        return;
    }

    health_api_t *health_api = (health_api_t *)handle;
    cJSON *report = NULL;
    
    if (health_api->get_system_health_report(&report) == ESP_OK && report) {
        const cJSON *heap = cJSON_GetObjectItem(report, "min_free_heap_bytes");
        
        // შევამოწმოთ, თუ თავისუფალი მეხსიერება 80KB-ზე ნაკლებია
        if (cJSON_IsNumber(heap) && heap->valueint < 80000) { 
            ESP_LOGE(TAG, "Not enough free memory for OTA update. Aborting.");
            cJSON_Delete(report);
            return;
        }
        cJSON_Delete(report);
    }

    // ... სისტემა ჯანმრთელია, ვიწყებთ OTA-ს ...
}
```
