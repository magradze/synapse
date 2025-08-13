# 🚌 მოდული: `i2c_bus`

**🗂️ კატეგორია:** `drivers`  
**✒️ ავტორი:** Giorgi Magradze & Synapse AI Expert  
**🔖 ვერსია:** 2.2.0

## 1. 📜 მიმოხილვა

`i2c_bus` არის Synapse Framework-ის **ფუნდამენტური აპარატურული დრაივერის მოდული**. ის უზრუნველყოფს ცენტრალიზებულ, უსაფრთხო და ნაკად-დაცულ (thread-safe) ინტერფეისს ESP32-ის I2C (Inter-Integrated Circuit) პერიფერიასთან სამუშაოდ.

მისი მთავარი მიზანია, მოახდინოს I2C ავტობუსის, როგორც **გაზიარებული რესურსის**, მართვის აბსტრაგირება. იმის ნაცვლად, რომ თითოეულმა სენსორმა თუ დისპლეიმ თავად მოახდინოს I2C დრაივერის ინიციალიზაცია, ისინი ითხოვენ წვდომას ამ ცენტრალიზებული სერვისისგან. ეს გამორიცხავს კონფლიქტებს, ამარტივებს კოდს და უზრუნველყოფს სისტემის სტაბილურობას.

## 2. 🏛️ არქიტექტურული პრინციპები

- **სუფთა სერვისის მომწოდებელი (Pure Service Provider):** ⚙️ მოდული არის სუფთა "სერვისის მომწოდებელი". ის არეგისტრირებს `i2c_bus_api_t` სერვისს `Service Locator`-ში და არ არის დამოკიდებული სხვა აპლიკაციის დონის მოდულებზე.
- **რესურსების უსაფრთხოება (Resource Safety):** 🛡️ `init` ფაზაში, მოდული იყენებს `Resource Manager`-ს, რათა ექსკლუზიურად დაიკავოს (დალოქოს) როგორც I2C პორტი, ისე კონფიგურაციაში მითითებული SDA და SCL GPIO პინები.
- **ნაკად-უსაფრთხოება (Thread-Safety):** 🔒 თითოეული I2C ოპერაცია (`write`, `read`, `scan`) დაცულია `Mutex`-ით, რაც უზრუნველყოფს I2C ავტობუსის უსაფრთხო გამოყენებას მრავალტასკიან გარემოში.
- **თანამედროვე დრაივერი (Modern Driver):** 🚀 მოდული იყენებს ESP-IDF v5.x-ის თანამედროვე, `i2c_master.h`-ზე დაფუძნებულ დრაივერს.
- **გამართული სიცოცხლის ციკლი (Proper Lifecycle):** 🔄 `init` ფაზაში ხდება მხოლოდ რესურსების მომზადება. აქტიური ოპერაციები, როგორიცაა ავტობუსის სკანირება, სრულდება `start` ფაზაში, რაც გამორიცხავს `race condition`-ებს სისტემის ჩატვირთვისას.

## 3. ⚙️ კონფიგურაცია (`config.json`)

მოდულის კონფიგურაცია ხდება მისი `config.json` ფაილის საშუალებით. შესაძლებელია სისტემაში რამდენიმე, სხვადასხვა I2C ავტობუსის ერთდროულად კონფიგურირება.

**კონფიგურაციის მაგალითი:**

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
      "clk_speed_hz": 100000,
      "scan_on_init": true
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
| `clk_speed_hz` | რიცხვი | I2C საათის სიხშირე ჰერცებში (მაგ., `100000` Standard Mode-სთვის). | ✅ |
| `scan_on_init` | ლოგიკური | თუ `true`, მოდული `start` ფაზაში ავტომატურად დაასკანერებს ავტობუსს. | ❌ |

## 4. 🔌 Service API (`i2c_bus_api_t`)

სხვა მოდულებს შეუძლიათ მიიღონ წვდომა ამ სერვისზე `Dependency Injection`-ის ან `Service Locator`-ის მეშვეობით. სერვისის ტიპი: **`SYNAPSE_SERVICE_TYPE_I2C_BUS_API`**.

**API ფუნქციები:**

- **`esp_err_t write(context, device_address, write_data, data_size);`**  
  ➡️ წერს მონაცემებს I2C მოწყობილობაში.
- **`esp_err_t read(context, device_address, read_data, data_size);`**  
  ⬅️ კითხულობს მონაცემებს I2C მოწყობილობიდან.
- **`esp_err_t write_read(context, device_address, write_data, write_size, read_data, read_size);`**  
  🔄 ასრულებს კომბინირებულ write-read ტრანზაქციას.
- **`void scan(context);`**  
  📡 ასკანერებს ავტობუსს და ლოგში ბეჭდავს ყველა აღმოჩენილი მოწყობილობის მისამართს.
- **`i2c_port_t get_port(context);`**  
  ℹ️ აბრუნებს ამ ავტობუსის პორტის ნომერს.

## 5. 💡 გამოყენების მაგალითი

წარმოვიდგინოთ, `ssd1306_driver` მოდულს სურს, გამოიყენოს I2C ავტობუსი.

**1. `ssd1306_driver/config.json`-ში დამოკიდებულების აღწერა:**

```json
{
    "type": "ssd1306_driver",
    "config": {
        "instance_name": "main_ssd1306_driver",
        "i2c_bus_service": "main_i2c_bus",
        "i2c_address": 60
    }
}
```

**2. `ssd1306_driver.c`-ში `dependency_map`-ის შექმნა:**

```c
// private_data სტრუქტურა
typedef struct {
    i2c_bus_handle_t *i2c_bus; // ინექცირებული handle-ის შესანახი ველი
    // ...
} ssd1306_private_data_t;

// დამოკიდებულების რუკა
static const module_dependency_t s_dependencies[] = {
    {"i2c_bus_service", offsetof(ssd1306_private_data_t, i2c_bus)},
    {NULL, 0}
};

// _create ფუნქციაში
module->dependency_map = s_dependencies;
```

**3. `ssd1306_driver.c`-ში სერვისის გამოყენება:**

```c
static esp_err_t ssd1306_init(module_t *self) {
    ssd1306_private_data_t *private_data = (ssd1306_private_data_t *)self->private_data;

    // 1. 🤝 ვამოწმებთ, რომ სერვისის handle-ი წარმატებით იქნა ინექცირებული
    if (!private_data->i2c_bus) {
        ESP_LOGE(TAG, "Dependency injection failed: i2c_bus service is NULL!");
        return ESP_ERR_INVALID_STATE;
    }

    // 2. 📡 ვასკანერებთ ავტობუსს (არასავალდებულო, დიაგნოსტიკისთვის)
    private_data->i2c_bus->api->scan(private_data->i2c_bus->context);
    
    return ESP_OK;
}

// 3. ✍️ მონაცემების ჩაწერა
static void send_display_command(ssd1306_private_data_t *private_data, uint8_t cmd) {
    if (private_data->i2c_bus) {
        uint8_t buffer[] = {0x00, cmd};
        private_data->i2c_bus->api->write(
            private_data->i2c_bus->context, // გადავცემთ კონტექსტს
            private_data->i2c_address,
            buffer,
            sizeof(buffer)
        );
    }
}
```

## 6. ❗ მნიშვნელოვანი შენიშვნები

- **დამოკიდებულება `CMakeLists.txt`-ში:** ნებისმიერმა მოდულმა, რომელიც იყენებს `i2c_bus`-ის სერვისს, თავის `CMakeLists.txt` ფაილში, `REQUIRES` სექციაში უნდა დაამატოს `i2c_bus_interface`, რათა მიიღოს წვდომა `i2c_bus_interface.h`-ზე.
- **რესურსების გათავისუფლება:** `deinit` ფუნქცია ავტომატურად ათავისუფლებს დაკავებულ GPIO პინებსა და I2C პორტს.
