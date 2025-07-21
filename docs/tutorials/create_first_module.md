# პირველი საკუთარი მოდულის შექმნა (Create Your First Module)

## მიზანი

ეს სახელმძღვანელო გაჩვენებთ, როგორ შექმნათ ახალი, სრულფასოვანი მოდული Synapse ESP Framework-ში. ჩვენ გამოვიყენებთ `create_module.py` სკრიპტს, რათა ავტომატურად დავაგენერიროთ მოდულის ჩონჩხი და შემდეგ შევავსოთ ის საბაზისო ლოგიკით.

## 1. საჭიროების განსაზღვრა

- **ამოცანა:** შევქმნათ მარტივი "მისალმების" მოდული, სახელად `greeter_module`.
- **ფუნქციონალი:**
    1. `start` ფაზაში, `System Timer` სერვისის გამოყენებით, დაგეგმოს პერიოდული ივენთი `GREETING_TIMER_TICK`.
    2. როდესაც მიიღებს ამ ივენთს, `Event Bus`-ზე გამოაქვეყნოს ახალი, საკუთარი ივენთი `GREETING_MESSAGE_READY` `payload`-ით, რომელიც შეიცავს მისალმების ტექსტს.
- **კატეგორია:** `utilities`

## 2. მოდულის ჩონჩხის გენერაცია

**(სრულად განახლებულია)**

ხელით ფაილების შექმნის ნაცვლად, ჩვენ გამოვიყენებთ `create_module.py` სკრიპტს.

1. გახსენით ტერმინალი პროექტის root დირექტორიაში.
2. გაუშვით სკრიპტი ინტერაქტიულ რეჟიმში:

    ```bash
    python3 scripts/create_module.py
    ```

3. უპასუხეთ კითხვებს შემდეგნაირად:
    - **შეიყვანეთ მოდულის სრული სახელი:** `Greeter Module`
    - **აირჩიეთ კატეგორია:** `utilities`
    - **აირჩიეთ მოდულის არქეტიპი:** `Event Producer` (რადგან ჩვენი მოდული ივენთებს გამოაქვეყნებს).
    - დანარჩენ კითხვებს შეგიძლიათ დაეთანხმოთ (Enter).

სკრიპტი შექმნის ყველა საჭირო ფაილს, მათ შორის `config.json`-ს, `components/modules/utilities/greeter_module/` დირექტორიაში.

## 3. კონფიგურაცია

1. **Runtime კონფიგურაცია:** გახსენით ახლად შექმნილი `greeter_module/config.json` და შეავსეთ ის:

    ```json
    [
      {
        "type": "greeter_module",
        "enabled": true,
        "config": {
          "instance_name": "main_greeter",
          "greeting_text": "Hello from Synapse!"
        }
      }
    ]
    ```

2. **Build-დროის კონფიგურაცია:** გაუშვით `idf.py menuconfig`, გადადით `Component config` -> `Synapse Framework` -> `Modules` -> `Greeter Module` და დარწმუნდით, რომ მოდული ჩართულია (`[*] Enable Greeter Module`).

## 4. კოდის იმპლემენტაცია

ახლა შევავსოთ `greeter_module.c` ფაილი. სკრიპტმა უკვე დააგენერირა ჩონჩხი, ჩვენ მხოლოდ ლოგიკა უნდა დავამატოთ.

**`src/greeter_module.c` (საბოლოო ვერსია):**

```c
/**
 * @file greeter_module.c
 * @brief A simple module that periodically sends greetings.
 * @author (Your Name)
 */
#include "greeter_module.h"
#include "logging.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "service_locator.h"
#include "timer_interface.h" // <--- დაგვჭირდება ტაიმერის ინტერფეისი
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("GREETER_MODULE");

// ჩვენი მოდულის მიერ გამოყენებული ივენთების სახელები
#define GREETING_TIMER_TICK "GREETING_TIMER_TICK"
#define GREETING_MESSAGE_READY "GREETING_MESSAGE_READY"

// Private Data Structure
typedef struct {
    char instance_name[32];
    char greeting_text[64];
    fmw_timer_handle_t greeting_timer;
} greeter_private_data_t;

// Forward Declarations
static esp_err_t greeter_module_init(module_t *self);
static esp_err_t greeter_module_start(module_t *self);
static void greeter_module_deinit(module_t *self);
static void greeter_module_handle_event(module_t *self, const char *event_name, void *event_data);

// Create Function (განახლებულია მეხსიერების მართვის წესების მიხედვით)
module_t *greeter_module_create(const cJSON *config) {
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    greeter_private_data_t *private_data = (greeter_private_data_t *)calloc(1, sizeof(greeter_private_data_t));
    if (!module || !private_data) { /* ... error handling ... */ return NULL; }

    module->private_data = private_data;
    module->current_config = (cJSON*)config; // ვიღებთ მფლობელობას

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    const cJSON *text_node = cJSON_GetObjectItem(config_node, "greeting_text");

    snprintf(module->name, sizeof(module->name), "%s", cJSON_GetStringValue(name_node));
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", cJSON_GetStringValue(name_node));
    snprintf(private_data->greeting_text, sizeof(private_data->greeting_text), "%s", cJSON_GetStringValue(text_node));

    module->base.init = greeter_module_init;
    module->base.start = greeter_module_start;
    module->base.deinit = greeter_module_deinit;
    module->base.handle_event = greeter_module_handle_event;
    
    return module;
}

// Init Function
static esp_err_t greeter_module_init(module_t *self) {
    ESP_LOGI(TAG, "'%s' initializing...", self->name);
    // გამოვიწეროთ ჩვენივე ტაიმერის ივენთი
    fmw_event_bus_subscribe(GREETING_TIMER_TICK, self);
    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

// Start Function
static esp_err_t greeter_module_start(module_t *self) {
    ESP_LOGI(TAG, "'%s' starting...", self->name);
    greeter_private_data_t *p_data = (greeter_private_data_t *)self->private_data;

    service_handle_t timer_service = fmw_service_get("main_timer_service");
    if (timer_service) {
        timer_api_t *timer_api = (timer_api_t *)timer_service;
        // დავგეგმოთ პერიოდული ივენთი ყოველ 5 წამში
        p_data->greeting_timer = timer_api->schedule_event(GREETING_TIMER_TICK, 5000, true);
    } else {
        ESP_LOGE(TAG, "System Timer service not found!");
        return ESP_FAIL;
    }
    
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

// Event Handler
static void greeter_module_handle_event(module_t *self, const char *event_name, void *event_data) {
    greeter_private_data_t *p_data = (greeter_private_data_t *)self->private_data;

    if (strcmp(event_name, GREETING_TIMER_TICK) == 0) {
        ESP_LOGI(TAG, "Timer tick received. Posting greeting message.");
        
        // მოვამზადოთ payload
        char* message_payload = strdup(p_data->greeting_text);
        event_data_wrapper_t *wrapper;

        if (fmw_event_data_wrap(message_payload, free, &wrapper) == ESP_OK) {
            fmw_event_bus_post(GREETING_MESSAGE_READY, wrapper);
            fmw_event_data_release(wrapper); // გავათავისუფლოთ ჩვენი reference
        }
    }

    if (event_data) {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// Deinit Function (განახლებულია მეხსიერების მართვის წესების მიხედვით)
static void greeter_module_deinit(module_t *self) {
    if (!self) return;
    greeter_private_data_t *p_data = (greeter_private_data_t *)self->private_data;

    // გავაუქმოთ ტაიმერი
    service_handle_t timer_service = fmw_service_get("main_timer_service");
    if (timer_service && p_data->greeting_timer) {
        ((timer_api_t *)timer_service)->cancel_event(p_data->greeting_timer);
    }

    fmw_event_bus_unsubscribe(GREETING_TIMER_TICK, self);

    if (self->current_config) cJSON_Delete(self->current_config);
    if (self->private_data) free(self->private_data);
    free(self);
}
```

## 5. ტესტირება

1. **ააწყვეთ და ჩაწერეთ firmware:**

    ```bash
    idf.py build flash monitor
    ```

2. **დააკვირდით ლოგებს:** თქვენ უნდა დაინახოთ, რომ `greeter_module` იქმნება, იწყებს მუშაობას და ყოველ 5 წამში ერთხელ ბეჭდავს `Posting greeting message.` შეტყობინებას.
3. **დამატებით:** შეგიძლიათ, `logger_module` გამოიყენოთ, რათა დაინახოთ, რომ `GREETING_MESSAGE_READY` ივენთი ნამდვილად ქვეყნდება `Event Bus`-ზე.

---

თუ შეგექმნათ პრობლემა, იხილეთ [troubleshooting](../troubleshooting/) ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს.
