# 💾 Synapse Storage Manager API Reference

## 🎯 მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის ცენტრალიზებული **Storage Manager** სერვისის საჯარო API-ს. ეს API უზრუნველყოფს ერთიან ინტერფეისს სხვადასხვა ფიზიკურ საცავთან (NVS, SPIFFS, SD Card) სამუშაოდ, რაც მოდულებს საშუალებას აძლევს, შეინახონ და წაიკითხონ მონაცემები შენახვის კონკრეტული ტექნოლოგიის ცოდნის გარეშე.

სერვისის მისაღებად გამოიყენეთ `synapse_service_get("main_storage")`.

---

## ⚙️ API-ს სტრუქტურა (`storage_api_t`)

ყველა ფუნქცია ხელმისაწვდომია `storage_api_t` სტრუქტურის მაჩვენებლის საშუალებით.

### 🔑 Key-Value ოპერაციები

ეს ფუნქციები იდეალურია მცირე ზომის კონფიგურაციული მონაცემების შესანახად.

- **`esp_err_t set_string(const char* owner, const char* key, const char* value);`**  
    ინახავს სტრიქონს. `owner` პარამეტრი გამოიყენება როგორც namespace (NVS-თვის) ან დირექტორია (SPIFFS/SD-სთვის).

- **`esp_err_t get_string(const char* owner, const char* key, char* buffer, size_t* buffer_size);`**  
    კითხულობს შენახულ სტრიქონს.

- **`esp_err_t set_i32(const char* owner, const char* key, int32_t value);`**  
    ინახავს 32-ბიტიან მთელ რიცხვს.

- **`esp_err_t get_i32(const char* owner, const char* key, int32_t* out_value);`**  
    კითხულობს შენახულ 32-ბიტიან რიცხვს.

- **`esp_err_t set_bool(const char* owner, const char* key, bool value);`**  
    ინახავს ლოგიკურ მნიშვნელობას (`true`/`false`).

- **`esp_err_t get_bool(const char* owner, const char* key, bool* out_value);`**  
    კითხულობს შენახულ ლოგიკურ მნიშვნელობას.

- **`esp_err_t erase_key(const char* owner, const char* key);`**  
    შლის მითითებულ key-value წყვილს.

---

### 🗂️ ფაილური ოპერაციები

ეს ფუნქციები ხელმისაწვდომია მხოლოდ მაშინ, როდესაც `Kconfig`-ში არჩეულია **SPIFFS** ან **SD Card** backend-ი. NVS backend-ის შემთხვევაში, ისინი დააბრუნებენ `ESP_ERR_NOT_SUPPORTED` შეცდომას.

- **`esp_err_t write_file(const char* file_path, const void* data, size_t size);`**  
    წერს მონაცემებს ფაილში მითითებულ მისამართზე (მაგ., `/spiffs/logs/today.log`). თუ ფაილი არსებობს, გადააწერს.

- **`esp_err_t read_file(const char* file_path, void* buffer, size_t* buffer_size);`**  
    კითხულობს ფაილის სრულ შიგთავსს.

- **`esp_err_t delete_file(const char* file_path);`**  
    შლის მითითებულ ფაილს.

- **`bool file_exists(const char* file_path);`**  
    ამოწმებს, არსებობს თუ არა ფაილი მითითებულ მისამართზე.

---

## 💡 გამოყენების მაგალითი

```c
#include "service_locator.h"
#include "storage_interface.h"

void example_storage_usage() {
    // 1. სერვისის მიღება
    storage_api_t *storage = (storage_api_t *)synapse_service_get("main_storage");
    if (!storage) {
        ESP_LOGE(TAG, "Storage service is not available!");
        return;
    }

    // 2. Key-Value ოპერაცია
    storage->set_i32("my_app", "boot_count", 42);

    // 3. ფაილური ოპერაცია (იმუშავებს მხოლოდ SPIFFS/SD-ზე)
    const char* message = "Hello, Synapse!";
    esp_err_t err = storage->write_file("/spiffs/welcome.txt", message, strlen(message));
    if (err == ESP_ERR_NOT_SUPPORTED) {
        ESP_LOGW(TAG, "File operations are not supported by the current storage backend (NVS).");
    }
}
```

---

დამატებითი დეტალებისთვის იხილეთ [storage_manager-ის README](../modules/storage/storage_manager/README.md) და [storage_interface.h](../interfaces/include/storage_interface.h).
