# 🚌 მოდული: `spi_bus`

**🗂️ კატეგორია:** `drivers`  
**✒️ ავტორი:** Synapse Framework Team  
**🔖 ვერსია:** 1.1.0

## 1. 📜 მიმოხილვა

`spi_bus` არის Synapse Framework-ის **ფუნდამენტური აპარატურული დრაივერის მოდული**. ის უზრუნველყოფს ცენტრალიზებულ, უსაფრთხო და ნაკად-დაცულ (thread-safe) ინტერფეისს ESP32-ის SPI (Serial Peripheral Interface) პერიფერიასთან სამუშაოდ.

მისი მთავარი მიზანია, მოახდინოს SPI ავტობუსის, როგორც **გაზიარებული რესურსის**, მართვის აბსტრაგირება. იმის ნაცვლად, რომ თითოეულმა SPI მოწყობილობამ (მაგ., ეკრანი, SD ბარათი, shift register) თავად მოახდინოს SPI დრაივერის ინიციალიზაცია, ისინი ითხოვენ წვდომას ამ ცენტრალიზებული სერვისისგან. ეს გამორიცხავს კონფლიქტებს, ამარტივებს კოდს და უზრუნველყოფს სისტემის სტაბილურობას.

## 2. 🏛️ არქიტექტურული პრინციპები

- **სერვისის მომწოდებელი (Pure Service Provider):** ⚙️ მოდული არის სუფთა "სერვისის მომწოდებელი". ის არეგისტრირებს `spi_bus_api_t` სერვისს `Service Locator`-ში და არ არის დამოკიდებული სხვა აპლიკაციის დონის მოდულებზე.
- **რესურსების უსაფრთხოება (Resource Safety):** 🛡️ `init` ფაზაში, მოდული იყენებს `Resource Manager`-ს, რათა ექსკლუზიურად დაიკავოს (დალოქოს) როგორც SPI ჰოსტი, ისე კონფიგურაციაში მითითებული MISO, MOSI და SCLK GPIO პინები. ეს გარანტიას იძლევა, რომ სხვა მოდული შემთხვევით ვერ გამოიყენებს ამ რესურსებს.
- **ნაკად-უსაფრთხოება (Thread-Safety):** 🔒 თითოეული SPI ოპერაცია (`add_device`, `remove_device`, `transmit`) დაცულია `Mutex`-ით. ეს უზრუნველყოფს, რომ მრავალტასკიან გარემოშიც კი, SPI ავტობუსზე ერთდროულად მხოლოდ ერთი მოწყობილობის დამატება/მოშორება ან ტრანზაქცია შესრულდება.
- **მოწყობილობების მართვა:** 🚌 მოდული მართავს ერთ SPI ავტობუსზე მიერთებულ მრავალ მოწყობილობას. თითოეული მოწყობილობა ემატება საკუთარი კონფიგურაციით (`spi_device_interface_config_t`) და იღებს უნიკალურ `handle`-ს, რომელიც გამოიყენება მასთან კომუნიკაციისთვის.
- **ადრეული ინიციალიზაცია (Early Initialization):** 📉 `init_level: 10` უზრუნველყოფს, რომ SPI ავტობუსი ინიციალიზდება სისტემის გაშვების ძალიან ადრეულ ეტაპზე, რათა ის ხელმისაწვდომი იყოს ყველა მასზე დამოკიდებული მოდულისთვის.

## 3. ⚙️ კონფიგურაცია (`config.json`)

მოდულის კონფიგურაცია ხდება მისი `config.json` ფაილის საშუალებით. შესაძლებელია სისტემაში რამდენიმე, სხვადასხვა SPI ავტობუსის ერთდროულად კონფიგურირება.

**კონფიგურაციის მაგალითი:**

```json
[
  {
    "type": "spi_bus",
    "enabled": true,
    "config": {
      "instance_name": "main_spi_bus",
      "spi_host": 2,
      "miso_pin": 12,
      "mosi_pin": 13,
      "sclk_pin": 14,
      "dma_chan": 1
    }
  }
]
```

**კონფიგურაციის პარამეტრები:**

| პარამეტრი | ტიპი | აღწერა | სავალდებულო |
| :--- | :--- | :--- | :---: |
| `instance_name` | სტრიქონი | SPI ავტობუსის უნიკალური სახელი. ამ სახელით ხდება სერვისის რეგისტრაცია. | ✅ |
| `spi_host` | რიცხვი | გამოსაყენებელი SPI ჰოსტის ID. ESP32-სთვის, როგორც წესი, `1` (VSPI) ან `2` (HSPI). | ✅ |
| `miso_pin` | რიცხვი | MISO ხაზისთვის განკუთვნილი GPIO პინის ნომერი. | ✅ |
| `mosi_pin` | რიცხვი | MOSI ხაზისთვის განკუთვნილი GPIO პინის ნომერი. | ✅ |
| `sclk_pin` | რიცხვი | SCLK (საათის) ხაზისთვის განკუთვნილი GPIO პინის ნომერი. | ✅ |
| `dma_chan` | რიცხვი | გამოსაყენებელი DMA არხი. `0` ნიშნავს DMA-ს გარეშე. | ✅ |

## 4. 🔌 Service API (`spi_bus_api_t`)

სხვა მოდულებს შეუძლიათ მიიღონ წვდომა ამ სერვისზე `Service Locator`-ის მეშვეობით. სერვისის ტიპი: **`FMW_SERVICE_TYPE_SPI_BUS_API`**.

**API ფუნქციები:**

- **`esp_err_t add_device(context, dev_config, out_handle);`**  
  ➕ ამატებს ახალ მოწყობილობას SPI ავტობუსზე. `dev_config` განსაზღვრავს მოწყობილობის პარამეტრებს (სიჩქარე, CS პინი და ა.შ.). წარმატების შემთხვევაში, `out_handle`-ში წერს მოწყობილობის უნიკალურ handle-ს.

- **`esp_err_t remove_device(context, handle);`**  
  ➖ შლის მოწყობილობას ავტობუსიდან.

- **`esp_err_t transmit(context, handle, transaction);`**  
  🔄 ასრულებს სრულ SPI ტრანზაქციას მითითებულ მოწყობილობასთან.

## 5. 💡 გამოყენების მაგალითი

`sn74hc595n_writer` მოდული იყენებს `spi_bus` სერვისს.

**`sn74hc595n_writer.c`-ში, `init` ფუნქციაში:**

```c
#include "service_locator.h"
#include "spi_bus_interface.h"

// ...

static esp_err_t sn74hc595n_writer_init(module_t *self) {
    sn74hc595n_private_data_t *private_data = (sn74hc595n_private_data_t *)self->private_data;

    // 1. 🤝 ვიღებთ SPI სერვისის handle-ს Service Locator-იდან
    private_data->spi_bus_handle = (spi_bus_handle_t*)fmw_service_get(private_data->spi_bus_service_name);
    if (!private_data->spi_bus_handle) {
        ESP_LOGE(TAG, "SPI bus service '%s' not found!", private_data->spi_bus_service_name);
        return ESP_ERR_NOT_FOUND;
    }

    // 2. ✍️ ვამატებთ ჩვენს shift register-ს, როგორც მოწყობილობას SPI ავტობუსზე
    spi_device_interface_config_t dev_cfg = {
        .mode = 0,
        .clock_speed_hz = 10 * 1000 * 1000, // 10 MHz
        .spics_io_num = -1, // CS პინს ვმართავთ ხელით (latch_pin)
        .queue_size = 1,
    };
    esp_err_t ret = private_data->spi_bus_handle->api->add_device(
        private_data->spi_bus_handle->context, 
        &dev_cfg, 
        &private_data->spi_device_handle
    );
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add SPI device: %s", esp_err_to_name(ret));
        return ret;
    }

    // ... (დანარჩენი ინიციალიზაციის ლოგიკა) ...
    return ESP_OK;
}
```

## 6. ❗ მნიშვნელოვანი შენიშვნები

- **დამოკიდებულება `CMakeLists.txt`-ში:** ნებისმიერმა მოდულმა, რომელიც იყენებს `spi_bus`-ის სერვისს, თავის `CMakeLists.txt` ფაილში, `REQUIRES` სექციაში უნდა დაამატოს `interfaces` (და არა `spi_bus`), რათა მიიღოს წვდომა `spi_bus_interface.h`-ზე.
- **CS პინის მართვა:** ბევრი მოწყობილობა მოითხოვს Chip Select (CS) პინის ხელით მართვას. ამ შემთხვევაში, `spi_device_interface_config_t`-ში `spics_io_num` უნდა იყოს `-1`, და მომხმარებელმა მოდულმა თავად უნდა მართოს CS პინი ტრანზაქციამდე და მის შემდეგ.
