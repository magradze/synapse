# მოდულების არქიტექტურა (Module Architecture)

## ძირითადი პრინციპები

- **მოდულურობა:** ყველა ფუნქციონალი წარმოდგენილია დამოუკიდებელი მოდულების სახით, რომლებიც შეიძლება ჩაერთოს ან გამოირთოს კონფიგურაციით.
- **იზოლაცია:** მოდულებს შორის პირდაპირი დამოკიდებულებები აკრძალულია. კომუნიკაცია ხდება მხოლოდ Service Locator-ის ან Event Bus-ის მეშვეობით.
- **სტანდარტიზაცია:** ყველა მოდული მიჰყვება ერთიან naming, structure და communication სტანდარტებს.
- **გაფართოებადობა:** ახალი მოდულის დამატება შესაძლებელია მინიმალური ძალისხმევით.

## მოდულის ტიპები

1. **Service-driven Module**
   - უზრუნველყოფს API-ს Service Locator-ისთვის
   - მაგ: display, actuator, driver მოდულები
2. **Event-driven Module**
   - ავრცელებს ან იღებს მოვლენებს Event Bus-ის მეშვეობით
   - მაგ: სენსორები, ლოგერები
3. **Driver Module**
   - უზრუნველყოფს hardware abstraction-ს და API-ს სხვა მოდულებისთვის
   - მაგ: i2c_bus_module, spi_bus_module
4. **Utility Module**
   - დამხმარე ფუნქციონალი, რომელიც შეიძლება იყოს როგორც service-driven, ასევე event-driven

## მოდულების ურთიერთქმედება

- **Service Locator:** გამოიყენება კონკრეტული API-ს მისაღებად და პირდაპირი ფუნქციური გამოძახებისთვის
- **Event Bus:** გამოიყენება broadcast/notification სცენარებისთვის
- **აკრძალულია:** პირდაპირი #include სხვა მოდულის header-ების

## მოდულის სიცოცხლის ციკლი (Lifecycle)

1. **Create:** მოდულის ობიექტის შექმნა (მაგ: ssd1306_module_create)
2. **Init:** ინიციალიზაცია (მაგ: static esp_err_t ssd1306_init)
3. **Enable/Disable:** ჩართვა/გამორთვა (მაგ: static esp_err_t ssd1306_enable/disable)
4. **Deinit:** რესურსების გათავისუფლება (მაგ: static esp_err_t ssd1306_deinit)

## მოდულის სტრუქტურის მაგალითი

```
components/modules/display/ssd1306_module/
├── CMakeLists.txt
├── module.json
├── README.md
├── include/
│   └── ssd1306_module.h
├── src/
│   └── ssd1306_module.c
```

### Header (include/ssd1306_module.h)

```c
#ifndef SSD1306_MODULE_H
#define SSD1306_MODULE_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

// Service API Structure
typedef struct {
    esp_err_t (*enable)(void);
    esp_err_t (*disable)(void);
    esp_err_t (*clear)(void);
    esp_err_t (*write_text)(const char *text, uint8_t line);
} ssd1306_api_t;

// Public API
module_t *ssd1306_module_create(const cJSON *config);

// Service API Functions
esp_err_t ssd1306_api_enable(void);
esp_err_t ssd1306_api_disable(void);
esp_err_t ssd1306_api_clear(void);
esp_err_t ssd1306_api_write_text(const char *text, uint8_t line);

#endif // SSD1306_MODULE_H
```

### Source (src/ssd1306_module.c)

```c
// ...existing code...
static module_t *global_ssd1306_instance = NULL;
static ssd1306_api_t ssd1306_service_api = {
    .enable = ssd1306_api_enable,
    .disable = ssd1306_api_disable,
    .clear = ssd1306_api_clear,
    .write_text = ssd1306_api_write_text
};

typedef struct {
    char module_instance_name[32];
    // კონფიგურაციის პარამეტრები
    // runtime ცვლადები
    // hardware handles
} ssd1306_private_data_t;

// ...function declarations, implementation...
```

## მოდულის გაფართოება/დამატება

1. შექმენით ახალი საქაღალდე შესაბამის კატეგორიაში (მაგ: sensors/new_sensor_module)
2. დაამატეთ CMakeLists.txt, module.json, include/, src/, README.md
3. გამოიყენეთ naming და structure კონვენციები
4. აღწერეთ API და საჭირო კონფიგურაცია
5. ჩართეთ მოდული მთავარ კონფიგურაციაში (Kconfig)

## აკრძალული და რეკომენდებული პრაქტიკები

❌ **არასდროს:**
- პირდაპირი #include სხვა მოდულის header-ების
- არასტანდარტული ცვლადებისა და ფუნქციების სახელები
- არასტანდარტული ფაილური სტრუქტურა
- კონფიგურაციის მნიშვნელობების hardcode

✅ **ყოველთვის:**
- დაიცავით naming და structure კონვენციები
- გამოიყენეთ Service Locator ან Event Bus სწორი სცენარისთვის
- აღწერეთ ყველა public API და კონფიგურაცია
- გამოიყენეთ დესკრიპტიული სახელები

---

შემდეგი ნაბიჯი: დეტალურად განვიხილოთ core კომპონენტები და მათი დანიშნულება.

