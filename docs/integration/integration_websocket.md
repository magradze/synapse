# WebSocket ინტეგრაციის სახელმძღვანელო (Integration — WebSocket)

---

## მიზანი

ეს დოკუმენტი წარმოადგენს Synapse ESP Framework-ში WebSocket სერვისებთან ინტეგრაციის სახელმძღვანელოს naming/structure კონვენციების დაცვით.

---

## ძირითადი ნაბიჯები

1. **WebSocket კლიენტის გამოყენება**
   - გამოიყენეთ ESP-IDF-ის esp_websocket_client ბიბლიოთეკა
   - კავშირის დამყარება:
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
     }
     ```

2. **უსაფრთხოების რეკომენდაციები**
   - გამოიყენეთ მხოლოდ WSS (TLS/SSL) პროტოკოლი საჭიროების შემთხვევაში
   - გამოიყენეთ ავტორიზაცია (მაგ: access token)
   - არ ჩაწეროთ სენსიტიური ინფორმაცია ლოგებში

3. **Debug და ტესტირება**
   - გამოიყენეთ ESP_LOGI/ESP_LOGE WebSocket მოდულში
   - დააკვირდით კავშირის სტატუსს და შეცდომებს

---

## Best Practices

- გამოიყენეთ მხოლოდ public API-ები WebSocket ინტეგრაციისთვის
- დაიცავით naming და structure კონვენციები
- ჩაატარეთ ტესტირება სხვადასხვა WebSocket სერვისზე
- გამოიყენეთ ლოგირება და დეტალური კონტექსტი

---

## დაკავშირებული დოკუმენტები

- [integration_index.md](integration_index.md)
- [api_reference/communication_api.md](../api_reference/communication_api.md)
- [configuration_management.md](../convention/configuration_management.md)

---

_დამატებითი კითხვებისთვის იხილეთ შესაბამისი integration დოკუმენტი ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს._

