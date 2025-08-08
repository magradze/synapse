# შეცდომების დამუშავება და ლოგირების სტანდარტები (Error Handling & Logging)

## მიზანი

სისტემის საიმედოობისა და დიაგნოსტიკის გასაუმჯობესებლად აუცილებელია შეცდომების სწორი დამუშავება და სტანდარტიზებული ლოგირება ყველა მოდულსა და კომპონენტში.

## შეცდომების დამუშავების პრინციპები

- გამოიყენეთ მხოლოდ esp_err_t ტიპის დაბრუნებადი მნიშვნელობები
- ყველა ფუნქციამ უნდა დააბრუნოს ESP_OK წარმატების შემთხვევაში
- გამოიყენეთ სტანდარტული ESP-IDF და Framework-ის error code-ები
- ყველა ფუნქციაში შეამოწმეთ პარამეტრების ვალიდურობა და რესურსების გამოყოფა
- ყველა შეცდომა და მნიშვნელოვანი მოვლენა უნდა ჩაიწეროს ლოგში

## შეცდომების კოდები

- ESP_OK — წარმატება
- ESP_ERR_INVALID_ARG — არასწორი არგუმენტი
- ESP_ERR_INVALID_STATE — არასწორი მდგომარეობა
- ESP_ERR_NO_MEM — მეხსიერების ნაკლებობა
- ESP_ERR_TIMEOUT — ოპერაციის ვადა ამოიწურა
- ESP_ERR_NOT_FOUND — რესურსი ვერ მოიძებნა
- ESP_ERR_NOT_SUPPORTED — ოპერაცია არ არის მხარდაჭერილი
- ESP_ERR_SYNAPSE_* — Framework-ის სპეციფიკური შეცდომები

## შეცდომების დამუშავების მაგალითები

```c
esp_err_t ssd1306_api_write_text(const char *text, uint8_t line) {
    if (!text) {
        ESP_LOGE(TAG, "Text parameter is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    if (line >= MAX_LINES) {
        ESP_LOGE(TAG, "Invalid line number: %d (max: %d)", line, MAX_LINES - 1);
        return ESP_ERR_INVALID_ARG;
    }
    if (!global_ssd1306_instance) {
        ESP_LOGE(TAG, "Module not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    // ...
    return ESP_OK;
}
```

## ლოგირების სტანდარტები

- გამოიყენეთ DEFINE_COMPONENT_TAG("MODULE_NAME") ყველა მოდულში
- გამოიყენეთ ESP_LOGE, ESP_LOGW, ESP_LOGI, ESP_LOGD, ESP_LOGV შესაბამისი მნიშვნელობისთვის
- ყველა ლოგში მიუთითეთ კონტექსტი და დეტალები
- გამოიყენეთ დესკრიპტიული ცვლადების სახელები ლოგში
- შეცდომის კოდები ყოველთვის გამოიტანეთ esp_err_to_name(ret)-ით

## ლოგირების მაგალითები

```c
DEFINE_COMPONENT_TAG("MQTT_MODULE");
ESP_LOGI(TAG, "MQTT connected to broker: %s", broker_url);
ESP_LOGE(TAG, "Failed to connect: %s", esp_err_to_name(operation_result));
ESP_LOGW(TAG, "Configuration key '%s' not found, using default: %d", config_key, default_value);
```

## აკრძალული და რეკომენდებული პრაქტიკები

❌ **არასდროს:**
- არალოგიკური ან ზოგადი ლოგები ("Error", "OK")
- სტატიკური TAG ცვლადის გამოყენება
- შეცდომის კოდის პირდაპირი დაბეჭდვა (გამოიყენეთ esp_err_to_name(ret))
- კონტექსტის გარეშე ლოგირება

✅ **ყოველთვის:**
- გამოიყენეთ DEFINE_COMPONENT_TAG
- მიუთითეთ დეტალური კონტექსტი
- გამოიყენეთ სტანდარტული error code-ები
- ჩაწერეთ ყველა მნიშვნელოვანი შეცდომა და მოვლენა ლოგში

---

შემდეგი ნაბიჯი: დეტალურად განვიხილოთ მოდულის გაფართოების პროცესი და best practices.

