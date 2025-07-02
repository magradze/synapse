# მოდული: OTA Update Manager

## 1. მიმოხილვა

**მოდულის სახელი:** `ota_update_manager`  
**კატეგორია:** `system`  
**ვერსია:** 1.0.0  
**ავტორი:** Synapse Framework Team

`OTA Update Manager` მოდული უზრუნველყოფს Synapse Framework-ისთვის უსაფრთხო და საიმედო Over-the-Air (OTA) firmware განახლების მექანიზმს. ის მოქმედებს როგორც მაღალი დონის "მენეჯერი", რომელიც კოორდინაციას უწევს განახლების პროცესის ყველა ეტაპს: სისტემის რესურსების შემოწმებას, firmware-ის გადმოწერას, ვალიდაციას და მოწყობილობის გადატვირთვას.

მოდული შექმნილია ფრეიმვორქის ძირითადი პრინციპების სრული დაცვით, რაც უზრუნველყოფს მის იზოლირებულ და ივენთებზე დაფუძნებულ მუშაობას.

## 2. წინაპირობები და `menuconfig`

ამ მოდულის გამართული მუშაობისთვის აუცილებელია ESP-IDF-ის კონფიგურაციაში (`menuconfig`) შემდეგი ოფციების ჩართვა:

1. **Bootloader-ის მხარდაჭერა:**
    * გზა: `Component config` → `Bootloader`
    * ოფცია: `[*] Allow app partition length to be shrunk to fit bootloader` (თუ საჭიროა)

2. **OTA კონფიგურაცია:**
    * გზა: `Component config` → `ESP HTTPS OTA`
    * ოფცია: `[*] Allow HTTP redirection` (რეკომენდებულია)

3. **სერტიფიკატების ნაკრები (Certificate Bundle):**
    * გზა: `Component config` → `mbedTLS`
    * ოფცია: `[*] Add the certificate bundle to the firmware`
    * ეს აუცილებელია HTTPS კავშირისთვის და სერვერის სერტიფიკატის ვალიდაციისთვის.

4. **ხელმოწერის ვალიდაცია (App Signing):**
    * გზა: `Security features`
    * ოფცია: `[*] Enable hardware secure boot in DEVELOPMENT mode` ან `PRODUCTION mode`.
    * ოფცია: `[*] Enable signature verification for secure boot`.
    * ეს უზრუნველყოფს, რომ მოწყობილობამ გაუშვას მხოლოდ სანდო, ციფრულად ხელმოწერილი firmware.

## 3. არქიტექტურული პრინციპები

* **Service-driven:** მოდული აწვდის `ota_api_t` სერვისს `Service Locator`-ის მეშვეობით, რათა სხვა მოდულებმა (მაგ., `mqtt_client`, `cmd_router`) შეძლონ განახლების პროცესის ინიცირება.
* **Event-driven:** პროცესის ყველა მნიშვნელოვან ეტაპზე (დაწყება, პროგრესი, წარმატება, შეცდომა) მოდული აქვეყნებს შესაბამის ივენთებს `Event Bus`-ზე.
* **ინტეგრირებული უსაფრთხოება:** იყენებს HTTPS-ს, სერვერის სერტიფიკატის ვალიდაციას და გადმოწერილი firmware-ის ციფრული ხელმოწერის შემოწმებას.
* **სისტემის მდგრადობა:** განახლების დაწყებამდე, `Health Monitor` მოდულის მეშვეობით ამოწმებს სისტემის რესურსებს (მაგ., თავისუფალი მეხსიერება), რათა თავიდან აიცილოს განახლების პროცესში სისტემის გაჭედვა.
* **ვიზუალური უკუკავშირი:** `RGB LED Indicator` მოდულის გამოყენებით, მომხმარებელს აწვდის ვიზუალურ ინფორმაციას განახლების სტატუსის შესახებ.

## 4. კონფიგურაცია

მოდულის კონფიგურაცია ხდება `system_config.json` ფაილის საშუალებით.

**კონფიგურაციის მაგალითი:**

```json
{
    "type": "ota_update_manager",
    "enabled": true,
    "config": {
        "instance_name": "main_ota_manager",
        "server_cert_pem_start": "-----BEGIN CERTIFICATE-----\n...",
        "pre_check_min_heap_kb": 80
    }
}
```

**კონფიგურაციის პარამეტრები:**

| პარამეტრი | ტიპი | აღწერა | სავალდებულო | Default |
|:---|:---|:---|:---:|:---|
| `instance_name` | სტრიქონი | მოდულის ინსტანციის უნიკალური სახელი. | ❌ | `ota_manager`|
| `server_cert_pem_start` | სტრიქონი | OTA სერვერის root CA სერტიფიკატი PEM ფორმატში. | ❌ | ESP-IDF-ის default bundle |
| `pre_check_min_heap_kb` | რიცხვი | თავისუფალი მეხსიერების მინიმალური ზღვარი (KB), რომელიც საჭიროა განახლების დასაწყებად. | ❌ | `80` |

## 5. Service API (`ota_api_t`)

სხვა მოდულებს შეუძლიათ მიიღონ წვდომა OTA მენეჯერის API-ზე `Service Locator`-ის მეშვეობით. სერვისის ტიპი: `FMW_SERVICE_TYPE_OTA_API`.

**API ფუნქციები:**

* **`esp_err_t start_update(const char *firmware_url);`**  
  იწყებს OTA განახლების პროცესს მითითებული URL-დან. ფუნქცია არ არის მბლოკავი; ის ქმნის ფონურ ტასკს განახლების შესასრულებლად და მყისიერად აბრუნებს პასუხს. პროცესის სტატუსის მონიტორინგი შესაძლებელია ივენთების საშუალებით.

## 6. გამოქვეყნებული ივენთები

* **`OTA_STARTED`**
  * **როდის ქვეყნდება:** როდესაც განახლების პროცესი წარმატებით იწყება.
  * **Payload (cJSON სტრიქონი):** `{"url":"https://..."}`

* **`OTA_PROGRESS`**
  * **როდის ქვეყნდება:** პერიოდულად, გადმოწერის პროცესში.
  * **Payload (cJSON სტრიქონი):** `{"progress_percent": 25}`

* **`OTA_SUCCESS`**
  * **როდის ქვეყნდება:** როდესაც firmware წარმატებით გადმოიწერება, ვალიდაციას გაივლის და მზად არის ინსტალაციისთვის (გადატვირთვის შემდეგ).
  * **Payload:** `NULL`

* **`OTA_FAILED`**
  * **როდის ქვეყნდება:** თუ პროცესის ნებისმიერ ეტაპზე მოხდა შეცდომა.
  * **Payload (cJSON სტრიქონი):** `{"error_code": -1, "error_msg": "Signature validation failed"}`

## 7. 💡 გამოყენების მაგალითი

`cmd_router` მოდული არეგისტრირებს `ota_start` ბრძანებას, რომელიც იღებს URL-ს და იწყებს განახლებას.

```c
// cmd_router.c-ში რეგისტრირებული handler

esp_err_t ota_command_handler(int argc, char **argv, void *context) {
    if (argc != 2) {
        printf("Usage: ota_start <https_url>\n");
        return ESP_ERR_INVALID_ARG;
    }

    const char *url = argv[1];

    service_handle_t handle = fmw_service_get("main_ota_manager");
    if (!handle) {
        printf("OTA Manager service not available.\n");
        return ESP_FAIL;
    }

    ota_api_t *ota_api = (ota_api_t *)handle;
    
    printf("Requesting OTA update from URL: %s\n", url);
    esp_err_t ret = ota_api->start_update(url);

    if (ret == ESP_OK) {
        printf("OTA process initiated successfully.\n");
    } else {
        printf("Failed to initiate OTA process: %s\n", esp_err_to_name(ret));
    }
    
    return ret;
}
```

### 8. განახლების ვალიდაცია და Rollback

`ota_manager` მოდული სრულად იყენებს ESP-IDF-ის ჩაშენებულ Rollback მექანიზმს, რათა უზრუნველყოს განახლების პროცესის უსაფრთხოება.

* **დროებითი გაშვება:** ახალი firmware-ის ჩაწერისა და გადატვირთვის შემდეგ, სისტემა ირთვება "შემოწმების რეჟიმში".
* **ავტომატური დაბრუნება:** თუ ახალი firmware ვერ დაადასტურებს თავის გამართულ მუშაობას (მაგალითად, კრიტიკული შეცდომის ან boot-loop-ის გამო), bootloader-ი შემდეგ გადატვირთვაზე ავტომატურად დააბრუნებს სისტემას წინა, სტაბილურ ვერსიაზე.
* **განახლების დადასტურება:** `ota_manager` (ან სხვა სისტემური მოდული) პასუხისმგებელია, გამოიძახოს `esp_ota_mark_app_valid_cancel_rollback()` ფუნქცია მას შემდეგ, რაც დარწმუნდება, რომ სისტემა სრულად ფუნქციონალურია (მაგ., WiFi-სთან კავშირი დამყარდა, MQTT სერვერთან დაკავშირება წარმატებულია). ეს, როგორც წესი, ხდება `SYSTEM_START` ივენთის შემდეგ, გარკვეული პირობების შესრულებისას.
