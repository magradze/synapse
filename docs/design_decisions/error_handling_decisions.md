# შეცდომების დამუშავების არქიტექტურული გადაწყვეტილებები (Error Handling Decisions)

---

## მიზანი

ეს დოკუმენტი აღწერს Synapse ESP Framework-ში შეცდომების დამუშავების არქიტექტურულ მიდგომებს, არჩეულ სტრატეგიას, ალტერნატივებს და საბოლოო გადაწყვეტილების შედეგებს naming/structure კონვენციების დაცვით.

---

## კონტექსტი

ფრეიმვორქს სჭირდება ერთიანი, პროგნოზირებადი და სტანდარტიზებული შეცდომების დამუშავების მექანიზმი, რომელიც უზრუნველყოფს როგორც სისტემური, ისე მოდულების შეცდომების მართვას, დიაგნოსტიკას და ლოგირებას.

---

## არჩეული გადაწყვეტილება

- **esp_err_t ტიპის დაბრუნებადი მნიშვნელობები:** ყველა public და private ფუნქცია აბრუნებს esp_err_t-ს.
- **სტანდარტული ESP-IDF და Framework-ის error code-ები:** გამოიყენება ESP_OK, ESP_ERR_INVALID_ARG, ESP_ERR_NO_MEM, ESP_ERR_FMW_* და სხვა.
- **პარამეტრების ვალიდაცია:** ყველა ფუნქციაში მოწმდება პარამეტრების ვალიდურობა.
- **ლოგირება:** ყველა შეცდომა და მნიშვნელოვანი მოვლენა ლოგდება შესაბამისი log level-ით (ESP_LOGE, ESP_LOGW).

---

## არგუმენტები და მოტივაცია

- ერთიანი შეცდომების დამუშავება ამარტივებს დიაგნოსტიკას და ტესტირებას
- ESP-IDF-ის სტანდარტებთან შესაბამისობა უზრუნველყოფს ინტეგრაციის სიმარტივეს
- დეტალური ლოგირება ამარტივებს პრობლემების აღმოჩენას
- ალტერნატივა (გლობალური errno, exception-style) უარყოფილია portability-სა და სტანდარტიზაციის არქონის გამო

---

## შედეგები და გავლენა

- შეცდომების მართვა ხდება პროგნოზირებადი და სტანდარტიზებული
- ყველა შეცდომა ადვილად იძებნება ლოგებში
- მოდულების ტესტირება და მხარდაჭერა მარტივდება
- შესაძლებელია შეცდომების კოდის გაფართოება Framework-ის საჭიროებებზე

---

## გამოყენების მაგალითები

### პარამეტრების ვალიდაცია
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

### რესურსების გამოყოფის შემოწმება
```c
static esp_err_t create_module_resources(module_private_data_t *module_private_data) {
    if (!module_private_data) {
        ESP_LOGE(TAG, "Private data pointer is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    module_private_data->command_queue = xQueueCreate(COMMAND_QUEUE_SIZE, sizeof(command_t));
    if (!module_private_data->command_queue) {
        ESP_LOGE(TAG, "Failed to create command queue");
        return ESP_ERR_NO_MEM;
    }
    // ...
    return ESP_OK;
}
```

---

## აკრძალული და რეკომენდებული პრაქტიკები

❌ **არასდროს:**
- გლობალური errno ან exception-style შეცდომების გამოყენება
- არალოგიკური ან ზოგადი ლოგები ("Error", "OK")
- შეცდომის კოდის პირდაპირი დაბეჭდვა (გამოიყენეთ esp_err_to_name(ret))

✅ **ყოველთვის:**
- გამოიყენეთ esp_err_t და სტანდარტული error code-ები
- ჩაწერეთ ყველა შეცდომა ლოგში დეტალური კონტექსტით
- დაიცავით naming და structure კონვენციები

---

## დაკავშირებული დოკუმენტები

- [core_components.md](core_components.md)
- [architecture_overview.md](architecture_overview.md)
- [error_handling.md](../convention/error_handling.md)
- [logging_standards.md](../convention/logging_standards.md)

---

_დამატებითი კითხვებისთვის იხილეთ შესაბამისი დოკუმენტაცია ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს._

