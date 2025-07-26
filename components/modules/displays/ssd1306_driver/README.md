# 📺 მოდული: `ssd1306_driver`

**🗂️ კატეგორია:** `displays`
**✒️ ავტორი:** Synapse Framework Team
**🔖 ვერსია:** 1.2.0

## 1. 📜 მიმოხილვა

`ssd1306_driver` არის Synapse Framework-ის დაბალი დონის დრაივერის მოდული, რომელიც უზრუნველყოფს SSD1306-ზე დაფუძნებული მონოქრომული OLED ეკრანების მართვას I2C ინტერფეისით.

მისი მთავარი ამოცანაა, მოახდინოს აპარატურასთან პირდაპირი კომუნიკაციის აბსტრაგირება და მიაწოდოს უნივერსალური, `display_driver_api_t` ინტერფეისი მაღალი დონის მოდულებს (როგორიცაა `ui_manager`). ამ მოდულმა არაფერი "იცის" მენიუების, სტატუს ბარების ან სხვა UI ელემენტების შესახებ; ის მხოლოდ ასრულებს გრაფიკულ პრიმიტივებთან დაკავშირებულ ბრძანებებს.

## 2. 🏛️ არქიტექტურული პრინციპები

- **Service Provider:** მოდული არის სუფთა "სერვისის მომწოდებელი". ის არეგისტრირებს `display_driver_api_t` სერვისს `Service Locator`-ში, `FMW_SERVICE_TYPE_DISPLAY_API` ტიპით.
- **ბუფერზე დაფუძნებული რენდერინგი:** ყველა ხატვის ოპერაცია სრულდება შიდა მეხსიერების ბუფერში. ცვლილებები ფიზიკურ ეკრანზე გადადის მხოლოდ `update_screen()` ფუნქციის გამოძახების შემდეგ, რაც უზრუნველყოფს ეკრანის გლუვ, "აციმციმების" გარეშე განახლებას.
- **დამოკიდებულება I2C სერვისზე:** მოდული სრულად ეყრდნობა `i2c_bus` სერვისს I2C კომუნიკაციისთვის, რაც უზრუნველყოფს რესურსების უსაფრთხო და კონტროლირებად გამოყენებას.
- **ადრეული ინიციალიზაცია (`init_level: 15`):** როგორც ფუნდამენტური დრაივერი, ის ინიციალიზდება სისტემის გაშვების ადრეულ ეტაპზე, `i2c_bus` მოდულის (`init_level: 10`) შემდეგ, რათა მზად იყოს მაღალი დონის UI მოდულების მომსახურებისთვის.

## 3. ⚙️ კონფიგურაცია

მოდული კონფიგურირდება მისი `config.json` ფაილის საშუალებით.

| პარამეტრი | ტიპი | აღწერა | სავალდებულო |
| :--- | :--- | :--- | :---: |
| `instance_name` | სტრიქონი | დრაივერის ამ ინსტანციის უნიკალური სახელი. | ✅ |
| `i2c_bus_service` | სტრიქონი | იმ `i2c_bus` სერვისის `instance_name`, რომელზეც ეკრანია მიერთებული. | ✅ |
| `i2c_addr` | სტრიქონი | ეკრანის 7-ბიტიანი I2C მისამართი (ჰექსადეციმალური ფორმატით). | ✅ |
| `width` | რიცხვი | ეკრანის სიგანე პიქსელებში. | ❌ (Default: 128) |
| `height` | რიცხვი | ეკრანის სიმაღლე პიქსელებში. | ❌ (Default: 64) |
| `reset_pin` | რიცხვი | RESET პინის GPIO ნომერი. თუ არ გამოიყენება, მიუთითეთ `-1`. | ❌ (Default: -1) |

## 4. 🔌 Service API

მოდული ახდენს `display_driver_api_t` ინტერფეისის იმპლემენტაციას. დეტალური აღწერისთვის იხილეთ `components/interfaces/include/display_driver_interface.h`.

**მხარდაჭერილი ფუნქციები:**

- `get_info`
- `get_small_font_metrics`
- `draw_pixel`
- `fill_rect`
- `draw_formatted_text` (იყენებს ჩაშენებულ, **6x8 პიქსელის** ზომის ფიქსირებული სიგანის ფონტს)
- `update_screen`
- `clear`
- `set_power`
- `set_contrast`

**არამხარდაჭერილი ფუნქციები:**

- `set_rotation` (აბრუნებს `ESP_ERR_NOT_SUPPORTED`)

## 5. 💡 გამოყენების მაგალითი

`ui_manager` მოდული იღებს ამ დრაივერის სერვისს და იყენებს მას UI-ს დასახატად.

```c
// In ui_manager.c

// 1. Get the service handle
display_driver_handle_t *display_handle = fmw_service_get("main_ssd1306_driver");
if (!display_handle) {
    ESP_LOGE(TAG, "Display driver not found!");
    return;
}

// 2. Use the API
const display_driver_api_t *display_api = display_handle->api;
void *context = display_handle->context;

display_api->clear(context);
display_api->draw_formatted_text(context, 10, 10, 1, "Hello, Synapse!");
display_api->fill_rect(context, 10, 20, 50, 10, 1);
display_api->update_screen(context);
```
