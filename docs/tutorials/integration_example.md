# ინტეგრაციის მაგალითი (Integration Example)

## მიზანი

ეს სახელმძღვანელო გაჩვენებთ, როგორ ინტეგრირდეთ Synapse Framework-ში სხვადასხვა გარე სისტემასთან ან სერვისთან (მაგალითად, MQTT, REST API, WebSocket) naming, structure და communication კონვენციების დაცვით.

## 1. ინტეგრაციის საჭიროების განსაზღვრა
- განსაზღვრეთ, რომელი გარე სერვისთან გსურთ დაკავშირება (მაგ: MQTT ბროკერი, REST API, WebSocket სერვერი)
- აირჩიეთ შესაბამისი მოდული ან შექმენით ახალი

## 2. საჭირო მოდულების ჩართვა და კონფიგურაცია
- ჩართეთ საჭირო მოდული system_config.json-ში
- მაგალითი MQTT-სთვის:
  ```json
  {
      "type": "mqtt_module",
      "enabled": true,
      "config": {
          "instance_name": "main_broker",
          "broker_uri": "mqtt://broker.synapse.local:1883",
          "base_topic": "/synapse/device"
      }
  }
  ```

## 3. MQTT Integration (Publish/Subscribe)

**Publish:**
```c
const char *topic = "/synapse/device/relay1/state";
const char *payload = "{\"state\":\"on\"}";
mqtt_module_publish(topic, payload);
```

**Subscribe:**
```c
esp_err_t mqtt_handle_command(const char *topic, const char *payload) {
    // ... process incoming command ...
    return ESP_OK;
}
mqtt_module_subscribe("/synapse/device/relay1/cmd", mqtt_handle_command);
```

## 4. REST API Integration

**HTTP GET:**
```c
#include "esp_http_client.h"

esp_err_t rest_api_get_example(void) {
    esp_http_client_config_t config = {
        .url = "https://api.synapse.local/v1/status"
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t ret = esp_http_client_perform(client);
    if (ret == ESP_OK) {
        int status = esp_http_client_get_status_code(client);
        // ... process response ...
    }
    esp_http_client_cleanup(client);
    return ret;
}
```

**HTTP POST:**
```c
esp_err_t rest_api_post_example(const char *json_payload) {
    esp_http_client_config_t config = {
        .url = "https://api.synapse.local/v1/data"
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, json_payload, strlen(json_payload));
    esp_err_t ret = esp_http_client_perform(client);
    // ... process response ...
    esp_http_client_cleanup(client);
    return ret;
}
```

## 5. WebSocket Integration

**WebSocket კავშირის დამყარება და შეტყობინების გაგზავნა:**
```c
#include "esp_websocket_client.h"

void websocket_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    // ... process websocket events ...
}

void websocket_example(void) {
    esp_websocket_client_config_t websocket_cfg = {
        .uri = "ws://ws.synapse.local:8080/ws"
    };
    esp_websocket_client_handle_t client = esp_websocket_client_init(&websocket_cfg);
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, NULL);
    esp_websocket_client_start(client);
    // გაგზავნეთ შეტყობინება
    const char *msg = "{\"type\":\"ping\"}";
    esp_websocket_client_send_text(client, msg, strlen(msg), portMAX_DELAY);
    // ... მიიღეთ და დაამუშავეთ შეტყობინებები ...
    // კავშირის დახურვა
    esp_websocket_client_stop(client);
    esp_websocket_client_destroy(client);
}
```

## 6. Best Practices
- გამოიყენეთ მხოლოდ public API-ები მოდულებს შორის
- დაიცავით naming და structure კონვენციები
- ყველა ინტეგრაცია აღწერეთ დოკუმენტაციაში
- გამოიყენეთ დეტალური ლოგირება და შეცდომების დამუშავება

## 7. Troubleshooting
- დარწმუნდით, რომ ყველა საჭირო მოდული ჩართულია და სწორად კონფიგურირებულია
- გამოიყენეთ ლოგები პრობლემების დიაგნოსტიკისთვის
- იხილეთ [troubleshooting](../troubleshooting/) დამატებითი რჩევებისთვის

---

თუ გაქვთ კითხვები ან გჭირდებათ დახმარება, მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს.

