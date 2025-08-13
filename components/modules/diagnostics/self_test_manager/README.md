# 🩺 მოდული: `self_test_manager`

**🗂️ კატეგორია:** `diagnostics`  
**✒️ ავტორი:** Synapse Framework Team  
**🔖 ვერსია:** 1.2.0

## 1. 📜 მიმოხილვა

`self_test_manager` არის Synapse Framework-ის **ცენტრალიზებული თვით-დიაგნოსტიკის მოდული**. მისი მთავარი ამოცანაა, უზრუნველყოს სისტემის კომპონენტების, სერვისების და პერიფერიის ჯანმრთელობის შემოწმების ერთიანი, მოთხოვნაზე-დაფუძნებული მექანიზმი.

ეს მოდული საშუალებას გვაძლევს, ნებისმიერ დროს (მაგ., ჩართვისას, დისტანციური ბრძანებით ან წარმოების ხაზზე) გავუშვათ ტესტების სრული ან ნაწილობრივი ნაკრები და მივიღოთ დეტალური, სტრუქტურირებული რეპორტი სისტემის მდგომარეობის შესახებ. ის კრიტიკულად მნიშვნელოვანია როგორც გამართვის (debugging) პროცესისთვის, ისე მოწყობილობის გრძელვადიანი საიმედოობის უზრუნველსაყოფად.

## 2. 🏛️ არქიტექტურული პრინციპები

- **⚙️ Service Consumer:** მოდული არის სუფთა "სერვისების მომხმარებელი". ის არ აწვდის საკუთარ Service API-ს, არამედ აქტიურად იყენებს ფრეიმვორქის სხვა სერვისებს (`System Manager`, `Health Monitor`, `Storage Manager` და ა.შ.) სისტემის კომპონენტების შესამოწმებლად.
- **⌨️ CLI-ზე დაფუძნებული მართვა:** მოდულის სრული მართვა ხდება `Command Router`-ის მეშვეობით რეგისტრირებული `selftest` ბრძანებით. ეს უზრუნველყოფს მის ხელმისაწვდომობას ნებისმიერი ინტერფეისიდან (Serial, MQTT, HTTP).
- **📨 ივენთზე-დაფუძნებული პასუხი:** ტესტის რეპორტის მოთხოვნისას, მოდული, კონსოლში ბეჭდვის პარალელურად, `Event Bus`-ზე აქვეყნებს `SYNAPSE_EVENT_SELF_TEST_REPORT_READY` ივენთს, რომლის `payload` არის რეპორტი JSON ფორმატში. ეს საშუალებას აძლევს `mqtt_manager`-ს და სხვა მოდულებს, მიიღონ და გაავრცელონ ტესტის შედეგები.
- **🚀 არაბლოკირებადი შესრულება:** ტესტების სრული ნაკრები ეშვება გამოყოფილ, დროებით FreeRTOS ტასკში. ეს უზრუნველყოფს, რომ დიაგნოსტიკის პროცესი არ ბლოკავს სისტემის ძირითად ნაკადს ან `Command Router`-ის ინტერაქტიულ გარსს.
- **🔝 მაღალი ინიციალიზაციის დონე (`init_level: 80`):** როგორც თითქმის ყველა სხვა სერვისის მომხმარებელი, ის ინიციალიზდება ერთ-ერთი ბოლო, რათა დარწმუნებული იყოს, რომ ყველა შესამოწმებელი კომპონენტი უკვე ხელმისაწვდომია.

## 3. ⚙️ კონფიგურაცია

მოდულის ძირითადი პარამეტრები იმართება `Kconfig`-ის (`idf.py menuconfig`) საშუალებით.

**`system_config.json` მაგალითი:**

```json
{
    "type": "self_test_manager",
    "enabled": true,
    "config": {
        "instance_name": "selftest"
    }
}
```

**შენიშვნა:** `instance_name` უნდა იყოს საკმარისად მოკლე, რათა `storage_manager`-ის ტესტირებისას არ გადააჭარბოს SPIFFS-ის ფაილის სახელის სიგრძის ლიმიტს.

**`Kconfig` პარამეტრები:**

`Component config` -> `Synapse Framework` -> `Modules` -> `Self Test Manager` მენიუში შეგიძლიათ დააკონფიგურიროთ:

- **`Self-Test Task Stack Size`:** დროებითი ტასკის სტეკის ზომა.
- **`Maximum Lines in Test Report`:** რეპორტში ხაზების მაქსიმალური რაოდენობა.

## 4. ⌨️ CLI ბრძანებები (Command Line Interface)

მოდული არეგისტრირებს `selftest` ბრძანებას `Command Router`-ში.

| ბრძანება | არგუმენტები | აღწერა |
| :--- | :--- | :--- |
| `selftest --run` | `[all\|core\|health\|storage\|conn]` | 🚀 იწყებს ტესტების შესრულებას. არგუმენტის გარეშე (`all`) გაეშვება ყველა ტესტი. |
| `selftest --report` | - | 📊 გამოაქვს ბოლო ტესტის შედეგების რეპორტი კონსოლში **და** აქვეყნებს JSON რეპორტს `Event Bus`-ზე. |

**გამოყენების მაგალითი:**

```bash
# ყველა ტესტის გაშვება
SYNAPSE> selftest --run all

# მხოლოდ storage ქვესისტემის ტესტირება
SYNAPSE> selftest --run storage

# შედეგების ნახვა
SYNAPSE> selftest --report
```

## 5. 📡 MQTT ინტერფეისი

`selftest` ბრძანების გამოძახება შესაძლებელია MQTT-ით, `Command Router`-ის მეშვეობით.

- **ბრძანების გაგზავნა:**
  - **Topic:** `synapse/devices/{id}/cmd/in`
  - **Payload:** `"selftest --report"`
- **პასუხის მიღება (JSON):**
  - **Topic:** `synapse/devices/{id}/data/selftest/report`
  - **Payload (მაგალითი):**

    ```json
    {"last_run_ms_ago":8547,"results":[{"test":"Core Services Check","status":"PASS","details":""},{"test":"Free Heap","status":"PASS","details":"71032 bytes"},{"test":"Storage Check","status":"PASS","details":"Write/Read/Erase OK"},{"test":"main_wifi_manager","status":"PASS","details":"Running"},{"test":"main_mqtt_broker","status":"PASS","details":"Running"}]}
    ```

## 6. 💡 ახალი ტესტის დამატების მაგალითი

წარმოვიდგინოთ, გვინდა დავამატოთ ტესტი, რომელიც ამოწმებს `rgb_led_indicator`-ის მუშაობას.

**ნაბიჯი 1: შექმენით ახალი ტესტის ფუნქცია `self_test_manager.c`-ში**

```c
/**
 * @internal
 * @brief Test suite for the RGB LED Indicator.
 * @details Checks if the service is available and briefly cycles through colors.
 */
static void run_rgb_led_check(self_test_private_data_t *private_data) {
    // 1. Find the service by its type
    service_handle_t led_handle = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_RGB_LED_API);
    if (!led_handle) {
        add_report_line(private_data, "RGB LED Check", TEST_RESULT_SKIPPED, "RGB LED service not found");
        return;
    }
    rgb_led_api_t *led_api = (rgb_led_api_t *)led_handle;

    // 2. Perform a simple test: cycle through Red, Green, Blue
    led_api->set_color(255, 0, 0); // Red
    vTaskDelay(pdMS_TO_TICKS(200));
    led_api->set_color(0, 255, 0); // Green
    vTaskDelay(pdMS_TO_TICKS(200));
    led_api->set_color(0, 0, 255); // Blue
    vTaskDelay(pdMS_TO_TICKS(200));
    
    // 3. Release control to return to automatic state
    led_api->release_control();

    // 4. Add result to the report
    add_report_line(private_data, "RGB LED Check", TEST_RESULT_PASS, "Color cycle OK");
}
```

**ნაბიჯი 2: დაამატეთ ფუნქციის `Forward Declaration`**

```c
// In the Forward Declarations section
static void run_rgb_led_check(self_test_private_data_t *private_data);
```

**ნაბიჯი 3: გამოიძახეთ ახალი ტესტი `run_tests_task`-ში**

```c
// Inside run_tests_task function
// ...
if (run_all || strcmp(suite_to_run, "conn") == 0) {
    printf("Running Connectivity Check...\n");
    run_connectivity_check(private_data);
    vTaskDelay(pdMS_TO_TICKS(50));
}
// ★★★ Add the new test call here ★★★
if (run_all || strcmp(suite_to_run, "led") == 0) {
    printf("Running RGB LED Check...\n");
    run_rgb_led_check(private_data);
    vTaskDelay(pdMS_TO_TICKS(50));
}
// ...
```

**ნაბიჯი 4: განაახლეთ `usage` სტრიქონი `cmd_t`-ში**

```c
// Inside self_test_manager_handle_event
static cmd_t selftest_cmd = {
    .command = "selftest",
    .help = "Run system self-diagnostics.",
    .usage = "selftest [--run [all|core|health|storage|conn|led]] [--report]", // <-- Add 'led'
    // ...
};
```

ამ ცვლილებების შემდეგ, `selftest --run led` ბრძანება გაუშვებს მხოლოდ LED-ის ტესტს, ხოლო `selftest --run all` მასაც ჩართავს საერთო სიაში.

## 7. ❗ მნიშვნელოვანი შენიშვნები

- **დამოკიდებულებები:** `CMakeLists.txt`-ში, `REQUIRES` სექციაში უნდა იყოს მითითებული ყველა ის მოდული, რომლის Service API-საც ეს მოდული პირდაპირ იყენებს (მაგ., `command_router`, `health_monitor`).
- **არაბლოკირებადობა:** დარწმუნდით, რომ თქვენს მიერ დამატებული ტესტები არ არის ხანგრძლივი ან მბლოკავი. თუ ტესტს დიდი დრო სჭირდება, გამოიყენეთ `vTaskDelay`, რათა სხვა ტასკებსაც მიეცეთ მუშაობის საშუალება.
