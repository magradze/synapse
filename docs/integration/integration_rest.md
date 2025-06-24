# REST API ინტეგრაციის სახელმძღვანელო (Integration — REST API)

---

## მიზანი

ეს დოკუმენტი წარმოადგენს Synapse ESP Framework-ში REST API-სთან ინტეგრაციის სახელმძღვანელოს naming/structure კონვენციების დაცვით.

---

## ძირითადი ნაბიჯები

1. **HTTP კლიენტის გამოყენება**
   - გამოიყენეთ ESP-IDF-ის esp_http_client ბიბლიოთეკა
   - მაგალითი GET მოთხოვნისთვის:
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
   - მაგალითი POST მოთხოვნისთვის:
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

2. **უსაფრთხოების რეკომენდაციები**
   - გამოიყენეთ მხოლოდ HTTPS (TLS/SSL)
   - გამოიყენეთ ავტორიზაცია (მაგ: API token, OAuth)
   - არ ჩაწეროთ სენსიტიური ინფორმაცია ლოგებში

3. **Debug და ტესტირება**
   - გამოიყენეთ ESP_LOGI/ESP_LOGE REST მოდულში
   - დააკვირდით HTTP სტატუს კოდებს და შეცდომებს

---

## Best Practices

- გამოიყენეთ მხოლოდ public API-ები REST ინტეგრაციისთვის
- დაიცავით naming და structure კონვენციები
- ჩაატარეთ ტესტირება სხვადასხვა REST სერვისზე
- გამოიყენეთ ლოგირება და დეტალური კონტექსტი

---

## დაკავშირებული დოკუმენტები

- [integration_index.md](integration_index.md)
- [api_reference/communication_api.md](../api_reference/communication_api.md)
- [configuration_management.md](../convention/configuration_management.md)

---

_დამატებითი კითხვებისთვის იხილეთ შესაბამისი integration დოკუმენტი ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს._

