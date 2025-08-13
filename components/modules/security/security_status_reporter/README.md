# 🛡️ მოდული: `security_status_reporter`

**🗂️ კატეგორია:** `security`  
**✒️ ავტორი:** Synapse Framework Team  
**🔖 ვერსია:** 1.0.0

## 1. 📜 მიმოხილვა

`security_status_reporter` არის Synapse Framework-ის **ფუნდამენტური უსაფრთხოების მოდული**. მისი მთავარი ამოცანაა, უზრუნველყოს მარტივი და ცენტრალიზებული წვდომა მოწყობილობის აპარატურული უსაფრთხოების კრიტიკულ სტატუსებზე: **Secure Boot**-ის და **Flash Encryption**-ის მდგომარეობაზე.

ეს მოდული სისტემის გაშვებისას ერთხელ კითხულობს ამ სტატუსებს, ინახავს (აკეშირებს) მათ და შემდეგ, `Service Locator`-ისა და `Command Router`-ის მეშვეობით, აწვდის ამ ინფორმაციას როგორც სისტემის სხვა კომპონენტებს, ისე საბოლოო მომხმარებელს. ეს კრიტიკულად მნიშვნელოვანია იმის დასადასტურებლად, რომ მოწყობილობა მუშაობს დაცულ, production-ready რეჟიმში.

## 2. 🏛️ არქიტექტურული პრინციპები

- **Service-driven:** მოდული არის სუფთა "სერვისის მომწოდებელი". ის არეგისტრირებს `security_status_api_t` სერვისს `Service Locator`-ში, რათა სხვა მოდულებმა (მაგ., `selftest_manager`) შეძლონ უსაფრთხოების სტატუსის პროგრამულად შემოწმება.
- **ადრეული ინიციალიზაცია (`init_level: 22`):** როგორც ფუნდამენტური სერვისი, ის ინიციალიზდება სისტემის გაშვების ძალიან ადრეულ ეტაპზე, რათა მისი ინფორმაცია ხელმისაწვდომი იყოს თითქმის ყველა სხვა მოდულისთვის.
- **მონაცემთა კეშირება:** სტატუსები იკითხება მხოლოდ ერთხელ, `init` ფაზაში. ეს ეფექტურია, რადგან ეს პარამეტრები runtime-ში არ იცვლება და მათი წაკითხვა eFuse-ებიდან შედარებით ნელი ოპერაციაა.
- **CLI და MQTT ინტეგრაცია:** მოდული არეგისტრირებს `security status` ბრძანებას. ამ ბრძანების შესრულებისას, ის არა მხოლოდ ბეჭდავს ინფორმაციას კონსოლში, არამედ აქვეყნებს `SYNAPSE_EVENT_SECURITY_STATUS_READY` ივენთს, რაც `mqtt_manager`-ს საშუალებას აძლევს, ეს ინფორმაცია MQTT-ით გაავრცელოს.

## 3. ⚙️ კონფიგურაცია

ამ მოდულს არ გააჩნია რთული, მომხმარებლის მიერ კონფიგურირებადი პარამეტრები. ერთადერთი პარამეტრი არის `instance_name`.

**`system_config.json`-ში ჩაწერის მაგალითი:**

```json
{
    "type": "security_status_reporter",
    "enabled": true,
    "config": {
        "instance_name": "main_security_status"
    }
}
```

## 4. 🔌 Service API (`security_status_api_t`)

სხვა მოდულებს შეუძლიათ მიიღონ წვდომა ამ სერვისზე `Service Locator`-ის მეშვეობით. სერვისის ტიპი: **`SYNAPSE_SERVICE_TYPE_SECURITY_API`**.

**API ფუნქციები:**

- **`bool is_secure_boot_enabled(void);`**  
    🔐 აბრუნებს `true`-ს, თუ მოწყობილობაზე ჩართულია Secure Boot.
- **`bool is_flash_encryption_enabled(void);`**  
    🔑 აბრუნებს `true`-ს, თუ მოწყობილობის flash მეხსიერება დაშიფრულია.

## 5. ⌨️ CLI ბრძანებები (Command Line Interface)

| ბრძანება | არგუმენტები | აღწერა |
| :--- | :--- | :--- |
| `security status` | - | ℹ️ გამოაქვს ინფორმაცია Secure Boot-ისა და Flash Encryption-ის სტატუსის შესახებ. |

**ბრძანების მაგალითი და შედეგი:**

```bash
SYNAPSE> security status
----------------------------------
  Device Security Status
----------------------------------
  Secure Boot       : DISABLED
  Flash Encryption  : DISABLED
----------------------------------
WARNING: Device is not in a fully secure state.
```

## 6. 💡 გამოყენება და უსაფრთხოების ჩართვა

ეს მოდული გაძლევთ საშუალებას, გადაამოწმოთ, არის თუ არა თქვენი მოწყობილობა სრულად დაცული. იმისთვის, რომ სტატუსი `ENABLED` გახდეს, საჭიროა ESP-IDF-ის ინსტრუმენტების გამოყენება.

**❓ როგორ ჩავრთო Secure Boot და Flash Encryption?**

ეს პროცესები, როგორც წესი, სრულდება პროექტის განვითარების ბოლო ეტაპზე, როდესაც ემზადებით წარმოებისთვის. **ყურადღება: ეს ოპერაციები შეუქცევადია!**

1. **Secure Boot-ის ჩართვა:**
    - **ნაბიჯი 1:** დააგენერირეთ ხელმოწერის გასაღები: `espsecure.py generate_secure_boot_key my_secure_boot_key.bin`
    - **ნაბიჯი 2:** ჩართეთ Secure Boot `menuconfig`-ში: `Security features` -> `Enable hardware secure boot in DEVELOPMENT/PRODUCTION mode`.
    - **ნაბიჯი 3:** ჩაწერეთ გასაღები eFuse-ში: `espefuse.py --port /dev/ttyUSB0 burn_key secure_boot my_secure_boot_key.bin`
    - **ნაბიჯი 4:** ააგეთ და ჩაწერეთ ხელმოწერილი firmware. ამის შემდეგ, მოწყობილობა გაუშვებს მხოლოდ თქვენი გასაღებით ხელმოწერილ კოდს.

2. **Flash Encryption-ის ჩართვა:**
    - **ნაბიჯი 1:** ჩართეთ Flash Encryption `menuconfig`-ში: `Security features` -> `Enable flash encryption on boot`. აირჩიეთ რეჟიმი (მაგ., `DEVELOPMENT`).
    - **ნაბიჯი 2:** ააგეთ და ჩაწერეთ firmware. **პირველივე ჩატვირთვისას**, ESP32 დააგენერირებს დაშიფვრის გასაღებს, ჩაწერს მას eFuse-ში და დაშიფრავს flash მეხსიერების შიგთავსს (bootloader, partition table, app). **ეს პროცესი ერთჯერადია და შეუქცევადი!**

**მას შემდეგ, რაც ამ ნაბიჯებს შეასრულებთ, `security status` ბრძანება დააბრუნებს:**

  ```bash
    Secure Boot       : ENABLED
    Flash Encryption  : ENABLED
  ```

## 7. 🤝 ინტეგრაცია `self_test_manager`-თან

ამ მოდულის მთავარი მომხმარებელი არის `self_test_manager`. ტესტირებისას, ის იძახებს ამ სერვისს და ამოწმებს, რომ მოწყობილობა წარმოების რეჟიმში მუშაობს დაცულად.

```c
// In self_test_manager.c
static void run_security_check(self_test_private_data_t *private_data) {
    service_handle_t sec_handle = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_SECURITY_API);
    if (!sec_handle) {
        add_report_line(private_data, "Security Check", TEST_RESULT_SKIPPED, "Service not found");
        return;
    }
    security_status_api_t *sec_api = (security_status_api_t *)sec_handle;
    
    bool sb_ok = sec_api->is_secure_boot_enabled();
    bool fe_ok = sec_api->is_flash_encryption_enabled();

    // For production, both should be true.
    if (sb_ok && fe_ok) {
        add_report_line(private_data, "Security Check", TEST_RESULT_PASS, "Secure Boot & Flash Encryption Enabled");
    } else {
        add_report_line(private_data, "Security Check", TEST_RESULT_FAIL, "Device is not fully secured!");
    }
}
```
