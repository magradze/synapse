# 🚌 მოდული: `i2c_bus`

**🗂️ კატეგორია:** `drivers`  
**✒️ ავტორი:** Giorgi Magradze & Synapse AI Expert  
**🔖 ვერსია:** 2.1.0

## 1. 📜 მიმოხილვა

`i2c_bus` არის Synapse Framework-ის **ფუნდამენტური აპარატურული დრაივერის მოდული**. ის უზრუნველყოფს ცენტრალიზებულ, უსაფრთხო და ნაკად-დაცულ (thread-safe) ინტერფეისს ESP32-ის I2C (Inter-Integrated Circuit) პერიფერიასთან სამუშაოდ.

მისი მთავარი მიზანია, მოახდინოს I2C ავტობუსის, როგორც **გაზიარებული რესურსის**, მართვის აბსტრაგირება. იმის ნაცვლად, რომ თითოეულმა სენსორმა თუ დისპლეიმ თავად მოახდინოს I2C დრაივერის ინიციალიზაცია, ისინი ითხოვენ წვდომას ამ ცენტრალიზებული სერვისისგან. ეს გამორიცხავს კონფლიქტებს, ამარტივებს კოდს და უზრუნველყოფს სისტემის სტაბილურობას.

## 2. 🏛️ არქიტექტურული პრინციპები

- **純粋なサービスプロバイダー (Pure Service Provider):** ⚙️ მოდული არის სუფთა "სერვისის მომწოდებელი". ის არეგისტრირებს `i2c_bus_api_t` სერვისს `Service Locator`-ში და არ არის დამოკიდებული სხვა აპლიკაციის დონის მოდულებზე.
- **リソースの安全性 (Resource Safety):** 🛡️ `init` ფაზაში, მოდული იყენებს `Resource Manager`-ს, რათა ექსკლუზიურად დაიკავოს (დალოქოს) როგორც I2C პორტი, ისე კონფიგურაციაში მითითებული SDA და SCL GPIO პინები. ეს გარანტიას იძლევა, რომ სხვა მოდული შემთხვევით ვერ გამოიყენებს ამ რესურსებს.
- **スレッドセーフティ (Thread-Safety):** 🔒 თითოეული I2C ოპერაცია (`write`, `read`, `scan`) დაცულია `Mutex`-ით. ეს უზრუნველყოფს, რომ მრავალტასკიან გარემოშიც კი, I2C ავტობუსზე ერთდროულად მხოლოდ ერთი ტრანზაქცია შესრულდება, რაც იცავს მონაცემთა მთლიანობას.
- **最新のドライバー (Modern Driver):** 🚀 მოდული იყენებს ESP-IDF v5.x-ის თანამედროვე, `i2c_master.h`-ზე დაფუძნებულ დრაივერს, რაც უზრუნველყოფს უკეთეს წარმადობას, მოქნილობას და მომავალთან თავსებადობას.
- **早期初期化 (Early Initialization):** 📉 `init_level: 10` უზრუნველყოფს, რომ I2C ავტობუსი ინიციალიზდება სისტემის გაშვების ძალიან ადრეულ ეტაპზე, რათა ის ხელმისაწვდომი იყოს ყველა მასზე დამოკიდებული მოდულისთვის.

## 3. ⚙️ კონფიგურაცია (`config.json`)

მოდულის კონფიგურაცია ხდება მისი `config.json` ფაილის საშუალებით. შესაძლებელია სისტემაში რამდენიმე, სხვადასხვა I2C ავტობუსის ერთდროულად კონფიგურირება.

**კონფიგურაციის მაგალითი (ერთი ავტობუსი):**

```json
[
  {
    "type": "i2c_bus",
    "enabled": true,
    "config": {
      "instance_name": "main_i2c_bus",
      "port": 0,
      "sda_pin": 21,
      "scl_pin": 22,
      "clk_speed_hz": 100000
    }
  }
]
```

**კონფიგურაციის პარამეტრები:**

| პარამეტრი | ტიპი | აღწერა | სავალდებულო |
| :--- | :--- | :--- | :---: |
| `instance_name` | სტრიქონი | I2C ავტობუსის უნიკალური სახელი. ამ სახელით ხდება სერვისის რეგისტრაცია. | ✅ |
| `port` | რიცხვი | გამოსაყენებელი I2C პორტის ნომერი (`0` ან `1`). | ✅ |
| `sda_pin` | რიცხვი | SDA ხაზისთვის განკუთვნილი GPIO პინის ნომერი. | ✅ |
| `scl_pin` | რიცხვი | SCL ხაზისთვის განკუთვნილი GPIO პინის ნომერი. | ✅ |
| `clk_speed_hz` | რიცხვი | I2C საათის სიხშირე ჰერცებში (მაგ., `100000` Standard Mode-სთვის, `400000` Fast Mode-სთვის). | ✅ |

## 4. 🔌 Service API (`i2c_bus_api_t`)

სხვა მოდულებს შეუძლიათ მიიღონ წვდომა ამ სერვისზე `Service Locator`-ის მეშვეობით. სერვისის ტიპი: **`SYNAPSE_SERVICE_TYPE_I2C_BUS_API`**.

**API ფუნქციები:**

- **`esp_err_t write(context, device_address, write_data, data_size);`**  
  ➡️ წერს მონაცემებს I2C მოწყობილობაში.
- **`esp_err_t read(context, device_address, read_data, data_size);`**  
  ⬅️ კითხულობს მონაცემებს I2C მოწყობილობიდან.
- **`esp_err_t write_read(context, device_address, write_data, write_size, read_data, read_size);`**  
  🔄 ასრულებს კომბინირებულ write-read ტრანზაქციას (მაგ., რეგისტრის წასაკითხად).
- **`void scan(context);`**  
  📡 ასკანერებს ავტობუსს და ლოგში ბეჭდავს ყველა აღმოჩენილი მოწყობილობის მისამართს.
- **`i2c_port_t get_port(context);`**  
  ℹ️ აბრუნებს ამ ავტობუსის პორტის ნომერს.

## 5. 💡 გამოყენების მაგალითი

წარმოვიდგინოთ, `ssd1306_oled_display` მოდულს სურს, გამოიყენოს I2C ავტობუსი, რომლის `instance_name` არის `"main_i2c_bus"`.

**`ssd1306_oled_display.c`-ში, `init` ფუნქციაში:**

```c
#include "service_locator.h"
#include "i2c_bus_interface.h" // ★★★ მნიშვნელოვანია ინტერფეისის ჩართვა ★★★

// ...

static esp_err_t ssd1306_init(module_t *self) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;

    // 1. 🤝 ვიღებთ I2C სერვისის handle-ს Service Locator-იდან
    private_data->i2c_bus_service = synapse_service_get("main_i2c_bus");
    if (!private_data->i2c_bus_service) {
        ESP_LOGE(TAG, "I2C bus service 'main_i2c_bus' not found!");
        return ESP_ERR_NOT_FOUND;
    }

    // 2. 📡 ვასკანერებთ ავტობუსს, რომ დავრწმუნდეთ, რომ ეკრანი ჩანს
    ESP_LOGI(TAG, "Performing diagnostic scan on the acquired I2C bus...");
    private_data->i2c_bus_service->api->scan(private_data->i2c_bus_service->context);

    // ... (დანარჩენი ინიციალიზაციის ლოგიკა) ...
    return ESP_OK;
}

// ...

// 3. ✍️ მონაცემების ჩაწერა (მაგალითი)
static void send_display_command(ssd1306_private_data_t *private_data, uint8_t cmd) {
    if (private_data->i2c_bus_service) {
        uint8_t buffer[] = {0x00, cmd};
        private_data->i2c_bus_service->api->write(
            private_data->i2c_bus_service->context, // გადავცემთ კონტექსტს
            private_data->i2c_address,
            buffer,
            sizeof(buffer)
        );
    }
}
```

## 6. ❗ მნიშვნელოვანი შენიშვნები

- **დამოკიდებულება `CMakeLists.txt`-ში:** ნებისმიერმა მოდულმა, რომელიც იყენებს `i2c_bus`-ის სერვისს, თავის `CMakeLists.txt` ფაილში, `REQUIRES` სექციაში უნდა დაამატოს `interfaces` (და არა `i2c_bus`), რათა მიიღოს წვდომა `i2c_bus_interface.h`-ზე.
- **რესურსების გათავისუფლება:** `deinit` ფუნქცია ავტომატურად ათავისუფლებს დაკავებულ GPIO პინებსა და I2C პორტს, რაც უზრუნველყოფს სისტემის სისუფთავეს.
