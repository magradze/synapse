# 💾 მოდული: `sn74hc595n_writer`

**🗂️ კატეგორია:** `drivers`  
**✒️ ავტორი:** Synapse Framework Team  
**🔖 ვერსია:** 1.2.0

## 1. 📜 მიმოხილვა

`sn74hc595n_writer` არის Synapse Framework-ის დაბალი დონის დრაივერის მოდული, რომელიც უზრუნველყოფს **გამომავალი (Output) პინების გაფართოებას** პოპულარული `SN74HC595N` 8-bit Serial-In, Parallel-Out Shift Register ჩიპის გამოყენებით.

მისი მთავარი ამოცანაა, მინიმალური რაოდენობის ESP32 პინების (MOSI, SCLK, Latch) გამოყენებით, მართოს 8, 16, 24 ან მეტი დამატებითი ციფრული გამომავალი პინი. ეს მოდული იდეალურია ისეთი ამოცანებისთვის, როგორიცაა მრავალი LED-ის, 7-სეგმენტიანი ინდიკატორის ან რელეს მართვა. ის მოქმედებს როგორც "ხიდი" მაღალი დონის მოდულებსა (როგორიცაა `rgb_led_indicator`) და ფიზიკურ SPI ავტობუსს შორის.

## 2. 🏛️ არქიტექტურული პრინციპები

- **სერვისის მომწოდებელი (Pure Service Provider):** ⚙️ მოდული არის სუფთა "სერვისის მომწოდებელი". ის არეგისტრირებს `sn74hc595n_writer_api_t` სერვისს `Service Locator`-ში, რაც სხვა მოდულებს აძლევს აბსტრაქტულ და მარტივ API-ს გაფართოებული პინების სამართავად.
- **სერვისის მომხმარებელი (Service Consumer):** 🛰️ მოდული არის `spi_bus` სერვისის მომხმარებელი. ის არ მართავს SPI პერიფერიას პირდაპირ, არამედ იყენებს ცენტრალიზებულ `spi_bus` სერვისს, რაც უზრუნველყოფს რესურსების კონფლიქტის გარეშე გამოყენებას.
- **რესურსების უსაფრთხოება (Resource Safety):** 🛡️ `init` ფაზაში, მოდული იყენებს `Resource Manager`-ს, რათა ექსკლუზიურად დაიკავოს (დალოქოს) Latch (ST_CP) პინი.
- **შიდა ბუფერიზაცია:** 📝 მოდული ინახავს ყველა გამომავალი პინის ლოგიკურ მდგომარეობას შიდა ბუფერში (`pin_states_buffer`). API ფუნქციები ცვლიან ამ ბუფერს და შემდეგ, `update_shift_registers` ფუნქცია აგზავნის ბუფერის სრულ შიგთავსს ჩიპ(ებ)ში ერთი SPI ტრანზაქციით.
- **მასშტაბირებადობა (Daisy-Chaining):** ⛓️ არქიტექტურა მხარს უჭერს რამდენიმე `SN74HC595N` ჩიპის ერთმანეთზე მიმდევრობით გადაბმას (daisy-chaining), რაც იძლევა პინების რაოდენობის მარტივად გაზრდის საშუალებას.

## 3. ⚙️ კონფიგურაცია (`config.json`)

მოდულის კონფიგურაცია ხდება მისი `config.json` ფაილის საშუალებით.

**კონფიგურაციის მაგალითი (ორი გადაბმული ჩიპი, 16 პინი):**

```json
[
  {
    "type": "sn74hc595n_writer",
    "enabled": true,
    "config": {
      "instance_name": "main_sr_writer",
      "spi_bus_service": "main_spi_bus",
      "latch_pin": 25,
      "num_chips": 2
    }
  }
]
```

**კონფიგურაციის პარამეტრები:**

| პარამეტრი | ტიპი | აღწერა | სავალდებულო |
| :--- | :--- | :--- | :---: |
| `instance_name` | სტრიქონი | ამ დრაივერის ინსტანციის უნიკალური სახელი. ამ სახელით ხდება სერვისის რეგისტრაცია. | ✅ |
| `spi_bus_service` | სტრიქონი | იმ `spi_bus` სერვისის `instance_name`, რომელსაც ჩიპი უკავშირდება. | ✅ |
| `latch_pin` | რიცხვი | Latch/Strobe (ST_CP) პინის GPIO ნომერი. | ✅ |
| `num_chips` | რიცხვი | ერთმანეთზე გადაბმული (daisy-chained) ჩიპების რაოდენობა. | ✅ |

## 4. 🔌 Service API (`sn74hc595n_writer_api_t`)

სხვა მოდულებს შეუძლიათ მიიღონ წვდომა ამ სერვისზე `Service Locator`-ის მეშვეობით. სერვისის ტიპი: **`SYNAPSE_SERVICE_TYPE_SN74HC595N_WRITER_API`**.

**API ფუნქციები:**

- **`esp_err_t set_pin_state(context, pin, state);`**  
  📌 რთავს ან თიშავს ერთ კონკრეტულ გაფართოებულ პინს. `pin` ნუმერაცია იწყება 0-დან.

- **`esp_err_t set_chip_pins(context, chip_index, value);`**  
   BYTE აყენებს 8-ვე პინის მდგომარეობას ერთდროულად კონკრეტული ჩიპისთვის. `value` არის 8-ბიტიანი მნიშვნელობა.

- **`esp_err_t get_pin_state(context, pin, out_state);`**  
  ❓ კითხულობს პინის ბოლო ცნობილ (დამახსოვრებულ) მდგომარეობას შიდა ბუფერიდან.

## 5. 💡 გამოყენების მაგალითი

`rgb_led_indicator` მოდული იყენებს ამ სერვისს, რათა მართოს RGB LED, რომელიც მიერთებულია გაფართოებულ პინებზე.

**`rgb_led_indicator.c`-ში, `init` და `set_led_color` ფუნქციებში:**

```c
#include "service_locator.h"
#include "sn74hc595n_writer_interface.h"

// ...

// init ფუნქციაში:
static esp_err_t rgb_led_indicator_init(module_t *self) {
    // ...
    private_data->sr_handle = (sn74hc595n_writer_handle_t *)synapse_service_get(private_data->sr_writer_service_name);
    if (!private_data->sr_handle) {
        ESP_LOGE(TAG, "Shift register writer service '%s' not found!", private_data->sr_writer_service_name);
        return ESP_ERR_NOT_FOUND;
    }
    // ...
}

// set_led_color ფუნქციაში:
static void set_led_color(rgb_led_private_data_t *private_data, uint8_t r, uint8_t g, uint8_t b) {
    // ...
    // გამოითვლება 8-ბიტიანი new_chip_state
    // ...
    private_data->sr_handle->api->set_chip_pins(private_data->sr_handle->context, 0, new_chip_state);
}
```

## 6. ❗ მნიშვნელოვანი შენიშვნები

- **დამოკიდებულება `CMakeLists.txt`-ში:** ნებისმიერმა მოდულმა, რომელიც იყენებს ამ სერვისს, თავის `CMakeLists.txt` ფაილში, `REQUIRES` სექციაში უნდა დაამატოს `interfaces`.
- **შეერთების სქემა:** `SN74HC595N`-ის გამართული მუშაობისთვის, `OE` (Output Enable) პინი უნდა იყოს მიერთებული GND-ზე, ხოლო `SRCLR` (Shift Register Clear) პინი - 3.3V-ზე.
