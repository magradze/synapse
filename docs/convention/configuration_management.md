# კონფიგურაციის მართვის კონვენცია

**სტატუსი:** უმკაცრესი მოთხოვნა (Mandatory)
**ვერსია:** 2.0.0

## 1. 📜 ძირითადი პრინციპი

**ყველა runtime კონფიგურაციული პარამეტრი უნდა იყოს მოდულის საკუთარ `config.json` ფაილში. კოდში `hardcoded` მნიშვნელობები დაუშვებელია.**

## 2. 📂 კონფიგურაციის წყაროები და იერარქია

Synapse Framework-ი იყენებს მრავალშრიან, დეცენტრალიზებულ კონფიგურაციის მოდელს. პარამეტრების საბოლოო მნიშვნელობა განისაზღვრება შემდეგი პრიორიტეტის მიხედვით (მაღლიდან დაბლისკენ):

1. **NVS (Non-Volatile Storage):**
    * **როლი:** მუდმივი მეხსიერება, სადაც ინახება საბოლოო, მომხმარებლის მიერ შეცვლილი ან `provisioning`-ით მიღებული კონფიგურაცია. NVS-ში შენახული კონფიგურაცია **ყველაზე მაღალი პრიორიტეტისაა** და გადააწერს ყველა დანარჩენს.

2. **`{module_name}/config.json` (Runtime Default-ები):**
    * **როლი:** ეს არის თითოეული მოდულის **default runtime კონფიგურაცია**. აქ აღწერილმა მნიშვნელობებმა შეიძლება გადააწეროს `Kconfig`-ის `default`-ები.

3. **`Kconfig` (Build-დროის Default-ები):**
    * **როლი:** ეს არის **"სიმართლის საბოლოო წყარო"** default მნიშვნელობებისთვის. ის განსაზღვრავს პარამეტრების საწყის მნიშვნელობებს, რომლებიც firmware-ში ჩაიშენება.

4. **`configs/system_config.json` (ბაზისური):**
    * **როლი:** განსაზღვრავს მხოლოდ გლობალურ პარამეტრებს (`firmware`, `global_config`) და `modules` მასივის საწყის, ცარიელ სტრუქტურას.

## 3. 🏗️ კონფიგურაციის სტრუქტურა

მიუხედავად იმისა, რომ კონფიგურაცია ფიზიკურად დაყოფილია, `Config Manager` runtime-ში აწყობს ერთიან, ლოგიკურ სტრუქტურას.

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
  }
]
```

## 4. 📝 კონფიგურაციის პარსინგის პატერნი

თითოეული მოდულის `_create` ფუნქცია იღებს თავისი კონფიგურაციის სრულ JSON ობიექტს. ამ ობიექტის დასამუშავებლად, ყველა მოდულმა უნდა გამოიყენოს სტანდარტული, 3-ნაბიჯიანი პატერნი `parse_config` ფუნქციაში.

### `parse_config` ფუნქციის 3-ნაბიჯიანი ლოგიკა

1. **ნაბიჯი 1: Default მნიშვნელობების დაყენება:** ფუნქციის დასაწყისში, `private_data` სტრუქტურის ველებს მიანიჭეთ `default` მნიშვნელობები, როგორც წესი, `Kconfig`-იდან.
2. **ნაბიჯი 2: JSON-იდან გადაწერა:** გამოიყენეთ `synapse_utils.h`-ში განსაზღვრული დამხმარე ფუნქციები (`synapse_config_get_string_from_node`, `..._get_int_...` და ა.შ.), რათა სცადოთ `default` მნიშვნელობების გადაწერა `config.json`-იდან მოსული მნიშვნელობებით.
3. **ნაბიჯი 3: საბოლოო ვალიდაცია:** ფუნქციის ბოლოს, შეამოწმეთ, რომ ყველა **სავალდებულო** პარამეტრმა მიიღო ვალიდური მნიშვნელობა. თუ რომელიმე სავალდებულო პარამეტრი არ არის განსაზღვრული, დააბრუნეთ შეცდომა.

### `parse_config` ფუნქციის მაგალითი

```c
// health_monitor.c-ში
static esp_err_t parse_config(const cJSON *config_node, health_monitor_private_data_t *private_data) {
    // ნაბიჯი 1: Default მნიშვნელობების დაყენება Kconfig-იდან
    private_data->check_interval_sec = CONFIG_HEALTH_MONITOR_DEFAULT_INTERVAL;
    private_data->min_free_heap_kb = CONFIG_HEALTH_MONITOR_DEFAULT_HEAP_KB;

    // ნაბიჯი 2: JSON-იდან გადაწერის მცდელობა synapse_utils-ის გამოყენებით
    synapse_config_get_int_from_node(TAG, config_node, "check_interval_sec", (int*)&private_data->check_interval_sec);
    
    const cJSON *thresholds = cJSON_GetObjectItem(config_node, "thresholds");
    if (thresholds) {
        synapse_config_get_int_from_node(TAG, thresholds, "min_free_heap_kb", (int*)&private_data->min_free_heap_kb);
    }

    // ნაბიჯი 3: ვალიდაცია (ამ შემთხვევაში, პარამეტრები არასავალდებულოა)
    ESP_LOGI(TAG, "Health Monitor configured: Interval=%us, Min Heap=%ukB", 
             private_data->check_interval_sec, private_data->min_free_heap_kb);

    return ESP_OK;
}
```

## 5. 🌐 კონფიგურაციაზე წვდომა სხვა მოდულებიდან

სხვა მოდულის კონფიგურაციის პარამეტრზე წვდომა უნდა მოხდეს **მხოლოდ** `Config Manager`-ის API-ს საშუალებით, **წერტილით გამოყოფილი გასაღების (dot-notation)** გამოყენებით.

* **გლობალური პარამეტრისთვის:** `"global_config.parameter_name"`
* **მოდულის პარამეტრისთვის:** `"instance_name.parameter_name"`

```c
// `main_light` ინსტანციის `pin` პარამეტრის წაკითხვა
int relay_pin;
synapse_config_get_int("main_light.pin", &relay_pin);
```

## 6. ❌ აკრძალული პრაქტიკები

* **არასდროს:**
  * კოდში `hardcoded` მნიშვნელობების გამოყენება.
  * `parse_config` ფუნქციაში `cJSON_*` ფუნქციების პირდაპირი გამოყენება (გამოიყენეთ `synapse_utils`).
  * `module.json`-ში runtime პარამეტრების განსაზღვრა.
  * `system_config.json`-ში `modules` მასივის ხელით შევსება.

## 7. ✅ საუკეთესო პრაქტიკები

* **ყოველთვის:**
  * გამოიყენეთ `synapse_utils.h`-ის დამხმარე ფუნქციები კონფიგურაციის პარსინგისთვის.
  * განსაზღვრეთ `default` მნიშვნელობები `Kconfig`-ში.
  * აღწერეთ ყველა კონფიგურაციის პარამეტრი მოდულის `README.md` ფაილში.
  * გამოიყენეთ `synapse_config_get_*` API-ები სხვა მოდულების პარამეტრებზე წვდომისთვის.
  * განსაზღვრეთ მოდულის default კონფიგურაცია მის საკუთარ `config.json` ფაილში.
