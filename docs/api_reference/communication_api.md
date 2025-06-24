# Synapse Communication Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის კომუნიკაციის (communication) მოდულების სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## MQTT Module API

### module_t *mqtt_module_create(const cJSON *config);
- ქმნის ახალი MQTT მოდულის ინსტანციას გადაცემული კონფიგურაციით.
- არგუმენტები: მოდულის კონფიგურაცია (cJSON ობიექტი)
- აბრუნებს: module_t* წარმატების შემთხვევაში, NULL შეცდომისას.

### esp_err_t mqtt_module_connect(const char *broker_url);
- აკავშირებს MQTT მოდულს ბროკერთან

### esp_err_t mqtt_module_publish(const char *topic, const char *message);
- აქვეყნებს შეტყობინებას მითითებულ topic-ზე

### esp_err_t mqtt_module_subscribe(const char *topic, mqtt_message_handler_t handler);
- იწერს მითითებულ topic-ზე და ამუშავებს მიღებულ შეტყობინებებს handler-ით

---

## WiFi Module API

### module_t *wifi_module_create(const cJSON *config);
- ქმნის ახალი WiFi მოდულის ინსტანციას გადაცემული კონფიგურაციით.

### esp_err_t wifi_module_connect(const char *ssid, const char *password);
- აკავშირებს WiFi მოდულს მითითებულ ქსელთან

### esp_err_t wifi_module_disconnect(void);
- წყვეტს WiFi კავშირს

---

## ESP-NOW Module API

### module_t *espnow_module_create(const cJSON *config);
- ქმნის ახალი ESP-NOW მოდულის ინსტანციას გადაცემული კონფიგურაციით.

### esp_err_t espnow_module_init(const cJSON *config);
- ინიციალიზაციას უკეთებს ESP-NOW მოდულს.

### esp_err_t espnow_module_send(const uint8_t *peer_mac, const uint8_t *data, size_t data_length);
- აგზავნის მონაცემებს მითითებულ peer MAC მისამართზე.

### esp_err_t espnow_module_register_receive_handler(espnow_receive_handler_t handler);
- რეგისტრირებს მიღებული მონაცემების დამმუშავებელ handler-ს.

---

## Event Handling API

### static void mqtt_handle_event(module_t *module, int32_t event_id, void *event_data);
- ამუშავებს MQTT მოდულის მიერ მიღებულ მოვლენებს

### static void wifi_handle_event(module_t *module, int32_t event_id, void *event_data);
- ამუშავებს WiFi მოდულის მიერ მიღებულ მოვლენებს

---

## Configuration Parsing API

### static esp_err_t parse_mqtt_config(const cJSON *config, mqtt_private_data_t *mqtt_data);
- კითხულობს და ამოწმებს MQTT მოდულის კონფიგურაციას

### static esp_err_t parse_wifi_config(const cJSON *config, wifi_private_data_t *wifi_data);
- კითხულობს და ამოწმებს WiFi მოდულის კონფიგურაციას

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**MQTT Publish:**
```c
mqtt_module_publish("/synapse/device/relay1/state", "{\"state\":\"on\"}");
```

**WiFi Connect:**
```c
wifi_module_connect("MyWiFi", "password123");
```

**ESP-NOW მონაცემის გაგზავნა:**
```c
uint8_t peer_mac[6] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC};
uint8_t payload[] = "Hello, Synapse!";
espnow_module_send(peer_mac, payload, sizeof(payload));
```

**ESP-NOW მიღებული მონაცემის დამუშავება:**
```c
void my_espnow_receive_handler(const uint8_t *src_mac, const uint8_t *data, size_t data_length) {
    // ... process received data ...
}
espnow_module_register_receive_handler(my_espnow_receive_handler);
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.
