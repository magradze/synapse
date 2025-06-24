# Synapse Provisioning Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის provisioning (მოწყობილობის კონფიგურაციისა და პირველადი დაყენების) მოდულების სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## BLE Provisioning Module API

### module_t *ble_prov_module_create(const cJSON *config);
- ქმნის ახალი BLE Provisioning მოდულის ინსტანციას გადაცემული კონფიგურაციით.

### esp_err_t ble_prov_module_start(module_t *module);
- იწყებს BLE provisioning პროცესს.

### esp_err_t ble_prov_module_stop(module_t *module);
- აჩერებს BLE provisioning პროცესს.

### esp_err_t ble_prov_module_set_credentials(module_t *module, const char *ssid, const char *password);
- აყენებს WiFi credentials BLE provisioning-ისთვის.

### esp_err_t ble_prov_module_get_status(module_t *module, ble_prov_status_t *status);
- აბრუნებს provisioning პროცესის მიმდინარე სტატუსს.

---

## SoftAP Provisioning Module API

### module_t *softap_prov_module_create(const cJSON *config);
- ქმნის ახალი SoftAP Provisioning მოდულის ინსტანციას გადაცემული კონფიგურაციით.

### esp_err_t softap_prov_module_start(module_t *module);
- იწყებს SoftAP provisioning პროცესს (მოწყობილობა ქმნის WiFi AP-ს).

### esp_err_t softap_prov_module_stop(module_t *module);
- აჩერებს SoftAP provisioning პროცესს.

### esp_err_t softap_prov_module_set_credentials(module_t *module, const char *ssid, const char *password);
- აყენებს WiFi credentials SoftAP provisioning-ისთვის.

### esp_err_t softap_prov_module_get_status(module_t *module, softap_prov_status_t *status);
- აბრუნებს provisioning პროცესის მიმდინარე სტატუსს.

---

## QR Code Provisioning API

### esp_err_t qr_code_prov_generate(const char *ssid, const char *password, char *qr_code_buffer, size_t buffer_size);
- ქმნის QR კოდს WiFi credentials-ით ან სხვა საჭირო მონაცემებით.

### esp_err_t qr_code_prov_display(const char *qr_code_buffer);
- აჩვენებს QR კოდს ეკრანზე ან სხვა ინტერფეისით.

---

## Serial Provisioning API

### module_t *serial_prov_module_create(const cJSON *config);
- ქმნის ახალი Serial Provisioning მოდულის ინსტანციას.

### esp_err_t serial_prov_module_start(module_t *module);
- იწყებს Serial provisioning პროცესს (UART/USB).

### esp_err_t serial_prov_module_stop(module_t *module);
- აჩერებს Serial provisioning პროცესს.

### esp_err_t serial_prov_module_set_credentials(module_t *module, const char *ssid, const char *password);
- აყენებს WiFi credentials Serial provisioning-ისთვის.

---

## Cloud Provisioning API

### module_t *cloud_prov_module_create(const cJSON *config);
- ქმნის ახალი Cloud Provisioning მოდულის ინსტანციას.

### esp_err_t cloud_prov_module_start(module_t *module);
- იწყებს provisioning პროცესს ღრუბლოვანი სერვისის მეშვეობით.

### esp_err_t cloud_prov_module_stop(module_t *module);
- აჩერებს provisioning პროცესს.

### esp_err_t cloud_prov_module_set_credentials(module_t *module, const char *ssid, const char *password);
- აყენებს WiFi credentials Cloud provisioning-ისთვის.

---

## NFC Provisioning API

### module_t *nfc_prov_module_create(const cJSON *config);
- ქმნის ახალი NFC Provisioning მოდულის ინსტანციას.

### esp_err_t nfc_prov_module_start(module_t *module);
- იწყებს NFC provisioning პროცესს.

### esp_err_t nfc_prov_module_stop(module_t *module);
- აჩერებს NFC provisioning პროცესს.

### esp_err_t nfc_prov_module_set_credentials(module_t *module, const char *ssid, const char *password);
- აყენებს WiFi credentials NFC provisioning-ისთვის.

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**BLE Provisioning პროცესის დაწყება:**
```c
ble_prov_module_start(module);
```

**WiFi Credentials-ის დაყენება:**
```c
ble_prov_module_set_credentials(module, "MyWiFi", "password123");
```

**Provisioning სტატუსის მიღება:**
```c
ble_prov_status_t status;
ble_prov_module_get_status(module, &status);
```

**SoftAP Provisioning პროცესის დაწყება:**
```c
softap_prov_module_start(module);
```

**QR კოდის გენერაცია და ჩვენება:**
```c
char qr_code_buffer[256];
qr_code_prov_generate("MyWiFi", "password123", qr_code_buffer, sizeof(qr_code_buffer));
qr_code_prov_display(qr_code_buffer);
```

**Serial Provisioning პროცესის დაწყება:**
```c
serial_prov_module_start(module);
```

**Cloud Provisioning პროცესის დაწყება:**
```c
cloud_prov_module_start(module);
```

**NFC Provisioning პროცესის დაწყება:**
```c
nfc_prov_module_start(module);
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.
