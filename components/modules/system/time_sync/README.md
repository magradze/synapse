# ⏱️ მოდული: `time_sync`

**🗂️ კატეგორია:** `system`
**✒️ ავტორი:** Synapse Framework Team
**🔖 ვერსია:** 2.0.0

## 1. 📜 მიმოხილვა

`time_sync` არის Synapse Framework-ის **სისტემური დროის სინქრონიზაციის** ფუნდამენტური მოდული. მისი მთავარი ამოცანაა, უზრუნველყოს მოწყობილობის სისტემური საათის ავტომატური გასწორება ინტერნეტში არსებულ დროის სერვერებთან **SNTP (Simple Network Time Protocol)** პროტოკოლის გამოყენებით.

ეს მოდული გამორიცხავს გარე RTC (Real-Time Clock) აპარატურის საჭიროებას და იყენებს არსებულ WiFi კავშირს, რათა მოწყობილობის დრო ყოველთვის იყოს ზუსტი. ეს კრიტიკულად მნიშვნელოვანია ისეთი ფუნქციებისთვის, როგორიცაა ლოგების დროის ნიშნულები (timestamps), დაგეგმილი ამოცანები და, რა თქმა უნდა, დროის ჩვენება მომხმარებლის ინტერფეისზე.

## 2. 🏛️ არქიტექტურული პრინციპები

- **Service Provider:** 🕰️ მოდული არის სუფთა "სერვისის მომწოდებელი". ის არეგისტრირებს `time_sync_api_t` სერვისს `Service Locator`-ში, `FMW_SERVICE_TYPE_TIME_SYNC_API` ტიპით, რათა სხვა მოდულებმა (მაგ., `ui_manager`) შეძლონ ზუსტი დროის მიღება.
- **Event Consumer:** 🌐 მოდული არის "ივენთების მომხმარებელი". ის გამოიწერს `WIFI_EVENT_IP_ASSIGNED` ივენთს. სინქრონიზაციის პროცესი იწყება მხოლოდ მას შემდეგ, რაც WiFi კავშირი დამყარდება და მოწყობილობა მიიღებს IP მისამართს.
- **ასინქრონული ოპერაცია:** 🔄 დროის სინქრონიზაცია ხდება ასინქრონულად, ESP-IDF-ის `esp_sntp` კომპონენტის მეშვეობით. პროცესის დასრულებისას, სპეციალური `callback` ფუნქცია აფიქსირებს წარმატებას და აყენებს შიდა `is_synced` ფლაგს.
- **დროის ზონის მხარდაჭერა:** ⏰ მოდული კითხულობს `timezone` პარამეტრს თავისი `config.json` ფაილიდან და აყენებს სისტემურ `TZ` გარემოს ცვლადს. ეს უზრუნველყოფს, რომ C-ს სტანდარტული დროის ფუნქციები (`localtime_r`) ავტომატურად გარდაქმნის UTC დროს სწორ ლოკალურ დროში.

## 3. ⚙️ კონფიგურაცია

მოდულის კონფიგურაცია ხდება მისი `config.json` ფაილის საშუალებით.

| პარამეტრი | ტიპი | აღწერა | სავალდებულო |
| :--- | :--- | :--- | :---: |
| `instance_name` | სტრიქონი | მოდულის ამ ინსტანციის უნიკალური სახელი. | ✅ |
| `timezone` | სტრიქონი | დროის ზონის სტრიქონი POSIX ფორმატში. | ❌ (Default: UTC) |

**კონფიგურაციის მაგალითი (საფრანგეთისთვის):**

```json
[
  {
    "type": "time_sync",
    "enabled": true,
    "config": {
      "instance_name": "main_time_sync",
      "timezone": "CET-1CEST,M3.5.0,M10.5.0/3"
    }
  }
]
```

**საქართველოსთვის (`UTC+4`) გამოიყენეთ:** `"timezone": "<-04>4"`

## 4. 🔌 Service API (`time_sync_api_t`)

მოდული აწვდის ერთ, მარტივ API ფუნქციას.

- **`esp_err_t get_time(time_t *current_time);`**
  - **აღწერა:** აბრუნებს მიმდინარე, სინქრონიზებულ დროს.
  - **აბრუნებს:**
    - `ESP_OK`: თუ დრო წარმატებით იქნა მიღებული.
    - `ESP_ERR_INVALID_STATE`: თუ დრო ჯერ არ არის სინქრონიზებული.

## 5. 💡 გამოყენების მაგალითი

`ui_manager` მოდული იყენებს ამ სერვისს საათის გამოსატანად.

```c
// In ui_manager.c

void render_home_screen(ui_manager_private_data_t *private_data) {
    char time_str[9];
    time_t now;

    // 1. Check if service exists AND if time is successfully retrieved
    if (private_data->time_sync && private_data->time_sync->get_time(&now) == ESP_OK) {
        struct tm timeinfo;
        localtime_r(&now, &timeinfo); // Converts UTC to local time using TZ variable
        strftime(time_str, sizeof(time_str), "%H:%M", &timeinfo);
    } else {
        // Fallback if service is missing OR time is not yet synced
        strcpy(time_str, "--:--");
    }

    // 2. Render the time string
    draw_large_digits_string(private_data, ..., time_str, 1);
}
```
