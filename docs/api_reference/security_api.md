# 🛡️ Security Status API Reference

## 🎯 მიზანი

ეს დოკუმენტი აღწერს `security_status_reporter` მოდულის მიერ მოწოდებულ Service API-ს.

---

## 1. 🔌 Service API (`security_status_api_t`)

სხვა მოდულებს შეუძლიათ მიიღონ წვდომა ამ სერვისზე `Service Locator`-ის მეშვეობით, `SYNAPSE_SERVICE_TYPE_SECURITY_API` ტიპის გამოყენებით.

**ინტერფეისის ჰედერი:** `security_status_interface.h`

### API ფუნქციები

-   **`bool is_secure_boot_enabled(void);`**
    -   **აღწერა:** ამოწმებს, ჩართულია თუ არა Secure Boot.
    -   **აბრუნებს:** `true` თუ ჩართულია, `false` თუ გამორთულია.

-   **`bool is_flash_encryption_enabled(void);`**
    -   **აღწერა:** ამოწმებს, დაშიფრულია თუ არა flash მეხსიერება.
    -   **აბრუნებს:** `true` თუ დაშიფრულია, `false` თუ არა.

**გამოყენების მაგალითი (`selftest_manager`-ში):**

```c
service_handle_t sec_handle = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_SECURITY_API);
if (sec_handle) {
    security_status_api_t *sec_api = (security_status_api_t *)sec_handle;
    bool sb_enabled = sec_api->is_secure_boot_enabled();
    printf("Secure Boot status: %s\n", sb_enabled ? "ENABLED" : "DISABLED");
}
```

---

## 2. ⌨️ CLI (Command Line Interface)

-   **ბრძანება:** `security status`
- **აღწერა:** გამოაქვს მოწყობილობის უსაფრთხოების სტატუსი კონსოლში და აქვეყნებს `SYNAPSE_EVENT_SECURITY_STATUS_READY` ივენთს.
