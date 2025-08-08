# Synapse Resource Manager API Reference

**ფაილი:** `resource_manager.h`  
**ვერსია:** 2.0  
**თარიღი:** 2025-06-25  
**ავტორი:** Giorgi Magradze  

---

## მიზანი

`Resource Manager` უზრუნველყოფს სისტემაში საერთო რესურსების (მაგ: GPIO, I2C, SPI და სხვა) ექსკლუზიურ და უსაფრთხო მართვას. ეს საშუალებას აძლევს მოდულებს, დაიკავონ რესურსები ისე, რომ სხვა მოდულებმა ვერ გამოიყენონ ისინი პარალელურად, რაც კრიტიკულია სტაბილური და პროგნოზირებადი სისტემური ქცევის უზრუნველსაყოფად.

---

## სამართავი რესურსების ტიპები

`synapse_resource_type_t` არის ენუმერაცია, რომელიც განსაზღვრავს მართვად რესურსებს:

| ტიპი | აღწერა | `resource_id` |
|------|--------|----------------|
| `SYNAPSE_RESOURCE_TYPE_GPIO` | GPIO პინი | `gpio_num_t` |
| `SYNAPSE_RESOURCE_TYPE_I2C_PORT` | I2C პორტი | `i2c_port_t` |
| `SYNAPSE_RESOURCE_TYPE_SPI_HOST` | SPI ჰოსტი | `spi_host_device_t` |
| `SYNAPSE_RESOURCE_TYPE_ADC_CHANNEL` | ADC არხი | `adc_channel_t` |
| `SYNAPSE_RESOURCE_TYPE_TIMER_GROUP` | ტაიმერების ჯგუფი | `timer_group_t` |
| `SYNAPSE_RESOURCE_TYPE_RMT_CHANNEL` | RMT არხი | `rmt_channel_t` |
| `SYNAPSE_RESOURCE_TYPE_UART_PORT` | UART პორტი | `uart_port_t` |

---

## API ფუნქციები

### `esp_err_t synapse_resource_manager_init(void)`

ინიციალიზაციას უკეთებს რესურს მენეჯერს. უნდა გამოიძახოს System Manager-მა ერთხელ სისტემის გაშვებისას.

| აბრუნებს | აღწერა |
|----------|--------|
| `ESP_OK` | წარმატებით ინიციალიზდა |
| `ESP_ERR_NO_MEM` | მეხსიერების გამოყოფა ვერ მოხერხდა |

---

### `esp_err_t synapse_resource_lock(synapse_resource_type_t type, uint8_t resource_id, const char *owner)`

დალოქავს კონკრეტულ რესურსს მითითებული მფლობელისთვის (მოდულისთვის).

| პარამეტრი | აღწერა |
|-----------|--------|
| `type` | რესურსის ტიპი |
| `resource_id` | რესურსის უნიკალური ID |
| `owner` | მოდულის სახელი (მაგ: `"relay_1"`) |

| აბრუნებს | აღწერა |
|----------|--------|
| `ESP_OK` | რესურსი წარმატებით დაილოქა |
| `ESP_ERR_INVALID_ARG` | არგუმენტები არავალიდურია |
| `ESP_ERR_INVALID_STATE` | რესურსი უკვე დაკავებულია |
| `ESP_ERR_NO_MEM` | მეხსიერება ვერ გამოეყო |

---

### `esp_err_t synapse_resource_release(synapse_resource_type_t type, uint8_t resource_id, const char *owner)`

ათავისუფლებს დაკავებულ რესურსს.

| პარამეტრი | აღწერა |
|-----------|--------|
| `type` | რესურსის ტიპი |
| `resource_id` | გასათავისუფლებელი რესურსი |
| `owner` | მოდულის სახელი, რომელიც ათავისუფლებს |

| აბრუნებს | აღწერა |
|----------|--------|
| `ESP_OK` | წარმატებით გათავისუფლდა |
| `ESP_ERR_INVALID_ARG` | არგუმენტები არასწორია |
| `ESP_ERR_NOT_FOUND` | რესურსი დაკავებული არ იყო |
| `ESP_ERR_INVALID_STATE` | გათავისუფლება სცადა არა მფლობელმა |

---

### `bool synapse_resource_is_locked(synapse_resource_type_t type, uint8_t resource_id)`

ამოწმებს, დაკავებულია თუ არა კონკრეტული რესურსი.

| აბრუნებს |
|----------|
| `true` – რესურსი დაკავებულია |
| `false` – რესურსი თავისუფალია |

---

### `const char *synapse_resource_get_owner(synapse_resource_type_t type, uint8_t resource_id)`

აბრუნებს რესურსის მფლობელი მოდულის სახელს. სასარგებლოა დიაგნოსტიკისთვის.

| აბრუნებს |
|----------|
| `const char *` – მფლობელი მოდული |
| `NULL` – რესურსი თავისუფალია ან არ მოიძებნა |

---

## გამოყენების მაგალითი

```c
static esp_err_t relay_module_init(module_t *module) {
    relay_private_data_t *private_data = (relay_private_data_t *)module->private_data;

    // 1. დალოქვა
    esp_err_t ret = synapse_resource_lock(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->gpio_pin, private_data->instance_name);
    if (ret != ESP_OK) {
        const char *owner = synapse_resource_get_owner(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->gpio_pin);
        ESP_LOGE(TAG, "GPIO %d lock failed. Owner: %s", private_data->gpio_pin, owner ?: "unknown");
        return ret;
    }

    // 2. გამოყენება
    gpio_set_direction(private_data->gpio_pin, GPIO_MODE_OUTPUT);
    ESP_LOGI(TAG, "GPIO %d locked and configured.", private_data->gpio_pin);
    return ESP_OK;
}

static esp_err_t relay_module_deinit(module_t *module) {
    relay_private_data_t *private_data = (relay_private_data_t *)module->private_data;
    synapse_resource_release(SYNAPSE_RESOURCE_TYPE_GPIO, private_data->gpio_pin, private_data->instance_name);
    ESP_LOGI(TAG, "GPIO %d released.", private_data->gpio_pin);
    return ESP_OK;
}
```

---

## საუკეთესო პრაქტიკები

- ✅ **Lock Before Use** – დაილოქეთ რესურსები მათი გამოყენების წინ (`init()` ფუნქციაში).
- ✅ **Release When Done** – გაათავისუფლეთ რესურსები მოდულის `deinit()` ფუნქციაში.
- ✅ **Check Return Values** – ყოველთვის შეამოწმეთ `synapse_resource_lock` შედეგი.
- ✅ **Use Descriptive Owners** – გამოიყენეთ უნიკალური `instance_name` owner-ისთვის.

---

> Synapse Resource Manager უზრუნველყოფს ერთიან, მოქნილ და უსაფრთხო რესურსების მართვას, რომელიც კრიტიკულია მოდულების ერთმანეთთან იზოლირებისთვის და სისტემის მდგრადი ფუნქციონირებისთვის.