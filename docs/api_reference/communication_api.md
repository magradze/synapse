# 📡 Synapse Communication Module API Reference

## 🎯 მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის საკომუნიკაციო მოდულების მიერ მოწოდებულ **სტანდარტულ სერვისის API-ებს**. ეს API-ები ხელმისაწვდომია `Service Locator`-ის მეშვეობით და საშუალებას აძლევს სხვა მოდულებს, გამოიყენონ ქსელური ფუნქციონალი (WiFi, MQTT, ESP-NOW) იზოლირებულად და უსაფრთხოდ.

---

## 📶 WiFi Manager Service API

**სერვისის ტიპი:** `SYNAPSE_SERVICE_TYPE_WIFI_API`  
**ინტერფეისის სტრუქტურა:** `wifi_api_t` (განსაზღვრულია `wifi_interface.h`-ში)

### ძირითადი ფუნქციები

- **`esp_err_t connect(const char *ssid, const char *password);`**  
    აკავშირებს მოწყობილობას მითითებულ WiFi ქსელთან. თუ მონაცემები სწორია, ისინი ასევე ინახება `Storage Manager`-ის მეშვეობით.

- **`esp_err_t disconnect(void);`**  
    წყვეტს მიმდინარე WiFi კავშირს.

- **`bool is_connected(void);`**  
    აბრუნებს `true`-ს, თუ მოწყობილობა დაკავშირებულია ქსელთან და აქვს IP მისამართი.

- **`esp_err_t get_ip_info(esp_netif_ip_info_t *ip_info);`**  
    აბრუნებს მიმდინარე IP კონფიგურაციას.

### 💡 WiFi API-ს გამოყენების მაგალითი

```c
#include "service_locator.h"
#include "wifi_interface.h" // უნდა შეიქმნას interfaces დირექტორიაში

void check_wifi_status() {
    service_handle_t wifi_handle = synapse_service_get("main_wifi");
    if (wifi_handle) {
        wifi_api_t *wifi_api = (wifi_api_t *)wifi_handle;
        
        if (wifi_api->is_connected()) {
            ESP_LOGI(TAG, "WiFi is connected.");
        } else {
            ESP_LOGW(TAG, "WiFi is disconnected.");
        }
    }
}
```

---

## ☁️ MQTT Client Service API

**სერვისის ტიპი:** `SYNAPSE_SERVICE_TYPE_MQTT_API`  
**ინტერფეისის სტრუქტურა:** `mqtt_api_t` (განსაზღვრულია `mqtt_interface.h`-ში)

### ძირითადი ფუნქციები

- **`esp_err_t connect(void);`**  
    იწყებს კავშირს კონფიგურაციაში მითითებულ MQTT ბროკერთან.

- **`esp_err_t publish(const char *topic, const char *data, int qos, bool retain);`**  
    აქვეყნებს შეტყობინებას მითითებულ თემაზე (topic).

- **`esp_err_t subscribe(const char *topic, int qos);`**  
    იწერს მითითებულ თემას. მიღებული შეტყობინებები დამუშავდება მოდულის შიდა ლოგიკით.

- **`bool is_connected(void);`**  
    აბრუნებს `true`-ს, თუ MQTT კლიენტი დაკავშირებულია ბროკერთან.

### 💡 MQTT API-ს გამოყენების მაგალითი

```c
#include "service_locator.h"
#include "mqtt_interface.h"

void publish_sensor_data(float temperature) {
    service_handle_t mqtt_handle = synapse_service_get("main_mqtt_broker");
    if (mqtt_handle) {
        mqtt_api_t *mqtt_api = (mqtt_api_t *)mqtt_handle;
        
        if (mqtt_api->is_connected()) {
            char payload[32];
            snprintf(payload, sizeof(payload), "{\"temp\": %.2f}", temperature);
            mqtt_api->publish("/sensors/temperature", payload, 1, false);
        }
    }
}
```

---

## ⚡ ESP-NOW Service API

**სერვისის ტიპი:** `SYNAPSE_SERVICE_TYPE_ESPNOW_API` (უნდა დაემატოს `service_types.h`-ში)  
**ინტერფეისის სტრუქტურა:** `espnow_api_t` (განსაზღვრულია `espnow_interface.h`-ში)

### ძირითადი ფუნქციები

- **`esp_err_t add_peer(const uint8_t *mac_addr, uint8_t channel);`**  
    ამატებს ახალ ESP-NOW მოწყობილობას (peer) სანდო სიაში.

- **`esp_err_t send_data(const uint8_t *mac_addr, const uint8_t *data, size_t len);`**  
    აგზავნის მონაცემებს მითითებულ MAC მისამართზე.

- **`esp_err_t register_receive_callback(espnow_recv_cb_t callback);`**  
    არეგისტრირებს callback ფუნქციას, რომელიც გამოიძახება მონაცემების მიღებისას.

---

## 📜 Naming & Structure კონვენციები

- ყველა საკომუნიკაციო სერვისის API სტრუქტურა უნდა იყოს განსაზღვრული `components/interfaces/include` დირექტორიაში შესაბამის `*_interface.h` ფაილში.
- ფუნქციების სახელები უნდა მიჰყვებოდეს [function_naming.md](../convention/function_naming.md) წესებს.

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md) და შესაბამისი მოდულების `README.md` ფაილები.
