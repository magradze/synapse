# კონფიგურაციის მართვის წესები

## ძირითადი პრინციპი

**(განახლებულია)**
**ყველა runtime კონფიგურაციული პარამეტრი უნდა იყოს მოდულის საკუთარ `config.json` ფაილში. კოდში hardcoded მნიშვნელობები დაუშვებელია.**

## კონფიგურაციის სტრუქტურა და წყაროები

**(სრულად განახლებულია)**

Synapse Framework v5.0.0-დან იყენებს **დეცენტრალიზებულ** კონფიგურაციის მოდელს:

1. **`configs/system_config.json` (ბაზისური):**
    * **როლი:** განსაზღვრავს მხოლოდ გლობალურ პარამეტრებს.
    * **სტრუქტურა:**

        ```json
        {
            "firmware": {
                "version": "5.0.0",
                "description": "Decentralized configuration model."
            },
            "global_config": {
                "device.id.prefix": "SYNAPSE"
            },
            "modules": [] // ეს მასივი ყოველთვის ცარიელია ამ ფაილში
        }
        ```

2. **`{module_name}/config.json` (მოდულის):**
    * **როლი:** ეს არის თითოეული მოდულის **default runtime კონფიგურაცია**.
    * **სტრუქტურა:** შეიცავს JSON ობიექტს ან ობიექტების მასივს, რომელიც სრულად აღწერს მოდულის ინსტანცი(ებ)ს.

    **მაგალითი (`relay_actuator/config.json`):**

    ```json
    [
      {
        "type": "relay_actuator",
        "enabled": true,
        "config": {
            "instance_name": "main_light",
            "pin": 23,
            "active_level": 0
        }
      },
      {
        "type": "relay_actuator",
        "enabled": true,
        "config": {
            "instance_name": "water_pump",
            "pin": 22,
            "active_level": 0
        }
      }
    ]
    ```

    **მაგალითი (`wifi_manager/config.json`):**

    ```json
    [
        {
            "type": "wifi_manager",
            "enabled": true,
            "config": {
                "instance_name": "main_wifi"
            }
        }
    ]
    ```

## კონფიგურაციის წვდომის პატერნები

**(განახლებულია)**
მიუხედავად იმისა, რომ კონფიგურაცია ფიზიკურად დაყოფილია, `Config Manager` runtime-ში აწყობს ერთიან, ლოგიკურ სტრუქტურას. ამიტომ, პარამეტრებზე წვდომის API უცვლელი რჩება.

### `synapse_config_get_*` ფუნქციების გამოყენება

წვდომა ხდება **წერტილით გამოყოფილი გასაღებით (dot-notation)**:

1. **გლობალური პარამეტრისთვის:** `"global_config.parameter_name"`
2. **მოდულის პარამეტრისთვის:** `"instance_name.parameter_name"`

**მაგალითი:**

```c
// გლობალური პარამეტრის წაკითხვა
char device_prefix[32];
synapse_config_get_string("global_config.device.id.prefix", device_prefix, sizeof(device_prefix));

// `main_light` ინსტანციის `pin` პარამეტრის წაკითხვა
int relay_pin;
synapse_config_get_int("main_light.pin", &relay_pin);
```

## კონფიგურაციის პარსინგი მოდულის შიგნით

თითოეული მოდულის `_create` ფუნქცია იღებს თავისი კონფიგურაციის სრულ JSON ობიექტს (`{"type": ..., "enabled": ..., "config": ...}`).

### `parse_config` ფუნქცია

რეკომენდებულია, თითოეულ მოდულს ჰქონდეს `static esp_err_t parse_config(const cJSON *config_node, private_data_t *p_data)` ფუნქცია.

```c
// relay_actuator.c-ში
static esp_err_t parse_config(const cJSON *config_node, relay_private_data_t *p_data) {
    if (!config_node) return ESP_ERR_INVALID_ARG;

    // Kconfig-იდან ვიღებთ default მნიშვნელობას
    p_data->gpio_pin = CONFIG_RELAY_ACTUATOR_DEFAULT_PIN;
    
    const cJSON *pin_node = cJSON_GetObjectItem(config_node, "pin");
    if (cJSON_IsNumber(pin_node)) {
        p_data->gpio_pin = pin_node->valueint;
    } else {
        ESP_LOGE(TAG, "'pin' is a required parameter!");
        return ESP_ERR_INVALID_ARG; // pin სავალდებულოა
    }
    
    // ... ვპარსავთ სხვა პარამეტრებს ...
    
    return ESP_OK;
}
```

## Default მნიშვნელობები და ვალიდაცია

* **"სიმართლის წყარო" default-ებისთვის არის `Kconfig`**. მოდულმა `parse_config` ფუნქციაში ჯერ უნდა მიანიჭოს `Kconfig`-იდან წამოღებული მნიშვნელობა და შემდეგ სცადოს მისი გადაფარვა `config.json`-იდან.
* **ვალიდაცია:** `_create` ფუნქცია პასუხისმგებელია, შეამოწმოს ყველა სავალდებულო პარამეტრის არსებობა და დააბრუნოს შეცდომა, თუ რომელიმე აკლია.

## აკრძალული პრაქტიკები

❌ **არასდროს:**

* კოდში hardcoded მნიშვნელობების გამოყენება.
* `system_config.json`-ში `modules` მასივის ხელით შევსება.
* `module.json`-ში runtime პარამეტრების განსაზღვრა.

✅ **ყოველთვის:**

* გამოიყენეთ `synapse_config_get_*` API-ები სხვა მოდულების პარამეტრებზე წვდომისთვის.
* განსაზღვრეთ მოდულის default კონფიგურაცია მის საკუთარ `config.json` ფაილში.
* გამოიყენეთ `Kconfig` build-დროის default-ებისთვის.

## შეჯამება

1. **კონფიგურაცია დეცენტრალიზებულია:** თითოეული მოდული პასუხისმგებელია თავის default კონფიგურაციაზე.
2. **`Config Manager` აერთიანებს მათ:** runtime-ში იქმნება ერთიანი, ლოგიკური კონფიგურაციის ხე.
3. **წვდომის API უცვლელია:** `synapse_config_get_*` ფუნქციები მუშაობს ისე, როგორც ადრე.
4. **`Kconfig` არის default-ების წყარო:** `config.json`-მა შეიძლება გადააფაროს `Kconfig`-ის მნიშვნელობები.
