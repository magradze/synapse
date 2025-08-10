# პირველი საკუთარი მოდულის შექმნა (v2.0)

## მიზანი

ეს სახელმძღვანელო გაჩვენებთ, როგორ შექმნათ ახალი, სრულფასოვანი მოდული Synapse ESP Framework-ში. ჩვენ გამოვიყენებთ `create_module.py` სკრიპტს, რათა ავტომატურად დავაგენერიროთ მოდულის ჩონჩხი და შემდეგ შევავსოთ ის საბაზისო ლოგიკით. ეს გაკვეთილი ასევე მოიცავს სერვისის რეგისტრაციის ახალ, საუკეთესო პრაქტიკას.

## 1. საჭიროების განსაზღვრა

- **ამოცანა:** შევქმნათ მარტივი "მისალმების" მოდული, სახელად `greeter_module`, რომელიც ამჯერად იქნება Service Provider-იც.
- **ფუნქციონალი:**
    1. `start` ფაზაში, `System Timer` სერვისის გამოყენებით, დაგეგმოს პერიოდული ივენთი `GREETING_TIMER_TICK`.
    2. როდესაც მიიღებს ამ ივენთს, `Event Bus`-ზე გამოაქვეყნოს ახალი, საკუთარი ივენთი `GREETING_MESSAGE_READY` `payload`-ით.
    3. დაარეგისტრიროს `greet` CLI ბრძანება.
    4. **(ახალი)** დაარეგისტრიროს საკუთარი Service API, რომელსაც ექნება ერთი ფუნქცია: `get_greeting_text()`.
- **კატეგორია:** `utilities`

## 2. მოდულის ჩონჩხის გენერაცია

ხელით ფაილების შექმნის ნაცვლად, ჩვენ გამოვიყენებთ `create_module.py` სკრიპტს.

1. გახსენით ტერმინალი პროექტის root დირექტორიაში.
2. გაუშვით სკრიპტი ინტერაქტიულ რეჟიმში:

    ```bash
    python3 tools/create_module.py
    ```

3. უპასუხეთ კითხვებს შემდეგნაირად:
    - **შეიყვანეთ მოდულის სრული სახელი:** `Greeter Module`
    - **აირჩიეთ კატეგორია:** `utilities`
    - **აირჩიეთ მოდულის არქეტიპი:** `Event & Service & Command` (რადგან ჩვენი მოდული სამივე ფუნქციონალს მოიცავს).
    - დანარჩენ კითხვებს შეგიძლიათ დაეთანხმოთ (Enter).

## 3. კონფიგურაცია

1. **Runtime კონფიგურაცია:** გახსენით ახლად შექმნილი `greeter_module/config.json` და შეავსეთ ის:

    ```json
    [
      {
        "type": "greeter_module",
        "enabled": true,
        "config": {
          "instance_name": "main_greeter",
          "greeting_text": "Hello from your first module!"
        }
      }
    ]
    ```

2. **Build-დროის კონფიგურაცია:** გაუშვით `idf.py menuconfig`, გადადით `Component config` -> `Synapse Framework` -> `Modules` -> `Greeter Module` და დარწმუნდით, რომ მოდული ჩართულია (`[*] Enable Greeter Module`).

## 4. კოდის იმპლემენტაცია

ახლა შევავსოთ გენერირებული ფაილები ჩვენი ლოგიკით.

#### **4.1. ინტერფეისის შექმნა (`greeter_interface.h`)**

რადგან ჩვენი მოდული Service Provider-ია, მას სჭირდება საჯარო "კონტრაქტი". შევქმნათ ახალი ფაილი: `components/interfaces/include/greeter_interface.h`

```c
#ifndef GREETER_INTERFACE_H
#define GREETER_INTERFACE_H

#include "synapse.h"

typedef struct {
    esp_err_t (*get_greeting_text)(void *context, const char **out_text);
} greeter_api_t;

#endif // GREETER_INTERFACE_H
```

#### **4.2. მოდულის იმპლემენტაცია (`greeter_module.c`)**

განვაახლოთ `greeter_module.c` ფაილის შიგთავსი.

```c
/**
 * @file greeter_module.c
 * @brief A simple module that provides a greeting service, posts events, and has a CLI command.
 */
#include "synapse.h"
#include "greeter_module.h"
#include "greeter_interface.h" // <<< ახალი ინტერფეისი
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("GREETER_MODULE", LOG_COLOR_GREEN);

// Event names
#define GREETING_TIMER_TICK "GREETING_TIMER_TICK"
#define GREETING_MESSAGE_READY "GREETING_MESSAGE_READY"

// Private Data
typedef struct {
    char instance_name[32];
    char greeting_text[64];
    synapse_job_handle_t greeting_job; // <<< ვიყენებთ Task Pool-ს
} greeter_private_data_t;

// Forward Declarations
static esp_err_t greeter_module_init(module_t *self);
static esp_err_t greeter_module_start(module_t *self);
static void greeter_module_deinit(module_t *self);
static void greeter_module_handle_event(module_t *self, const char *event_name, void *event_data);
static void greeting_job_callback(void *context); // <<< ახალი job callback
// API Function
static esp_err_t greeter_api_get_text(void *context, const char **out_text);

// Service API table
static greeter_api_t s_greeter_api = {
    .get_greeting_text = greeter_api_get_text
};

// Create Function
module_t *greeter_module_create(const cJSON *config) {
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    greeter_private_data_t *private_data = (greeter_private_data_t *)calloc(1, sizeof(greeter_private_data_t));
    if (!module || !private_data) { /* ... error handling ... */ return NULL; }

    module->private_data = private_data;
    module->current_config = cJSON_Duplicate(config, true);
    if (!module->current_config) { /* ... error handling ... */ return NULL; }

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    // Using synapse_utils for safe parsing
    synapse_config_get_string_from_node(TAG, config_node, "instance_name", private_data->instance_name, sizeof(private_data->instance_name));
    synapse_config_get_string_from_node(TAG, config_node, "greeting_text", private_data->greeting_text, sizeof(private_data->greeting_text));
    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);

    module->base.init = greeter_module_init;
    module->base.start = greeter_module_start;
    module->base.deinit = greeter_module_deinit;
    module->base.handle_event = greeter_module_handle_event;
    
    // Register the service in the create phase
    esp_err_t ret = synapse_service_register_with_status(
        module->name, SYNAPSE_SERVICE_TYPE_CUSTOM_API, &s_greeter_api, SERVICE_STATUS_REGISTERED);
    if (ret != ESP_OK) {
        greeter_module_deinit(module);
        return NULL;
    }

    return module;
}

// API Implementation
static esp_err_t greeter_api_get_text(void *context, const char **out_text) {
    module_t *self = (module_t *)context;
    greeter_private_data_t *p_data = (greeter_private_data_t *)self->private_data;
    *out_text = p_data->greeting_text;
    return ESP_OK;
}

// Job Callback
static void greeting_job_callback(void *context) {
    module_t *self = (module_t *)context;
    greeter_private_data_t *p_data = (greeter_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Posting greeting message from '%s'", self->name);
    
    // Post event with payload
    char *payload = strdup(p_data->greeting_text);
    event_data_wrapper_t *wrapper;
    if (synapse_event_data_wrap(payload, free, &wrapper) == ESP_OK) {
        synapse_event_bus_post(GREETING_MESSAGE_READY, wrapper);
        synapse_event_data_release(wrapper);
    }
}

// Init Function
static esp_err_t greeter_module_init(module_t *self) {
    ESP_LOGI(TAG, "'%s' initializing...", self->name);
    synapse_event_bus_subscribe(SYNAPSE_EVENT_SYSTEM_START_COMPLETE, self);
    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

// Start Function
static esp_err_t greeter_module_start(module_t *self) {
    ESP_LOGI(TAG, "'%s' starting...", self->name);
    greeter_private_data_t *p_data = (greeter_private_data_t *)self->private_data;
    
    // Schedule a job using the Task Pool Manager
    p_data->greeting_job = synapse_task_pool_schedule_job(greeting_job_callback, self, 5000, true);
    if (!p_data->greeting_job) {
        ESP_LOGE(TAG, "Failed to schedule greeting job!");
        return ESP_FAIL;
    }
    
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

// Event Handler for CLI registration
static void greeter_module_handle_event(module_t *self, const char *event_name, void *event_data) {
    if (strcmp(event_name, SYNAPSE_EVENT_SYSTEM_START_COMPLETE) == 0) {
        // Your CLI registration logic from the old file goes here
        // ...
    }
    if (event_data) {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// Deinit Function
static void greeter_module_deinit(module_t *self) {
    if (!self) return;
    greeter_private_data_t *p_data = (greeter_private_data_t *)self->private_data;

    if (p_data && p_data->greeting_job) {
        synapse_task_pool_cancel_job(p_data->greeting_job);
    }
    
    synapse_event_bus_unsubscribe(SYNAPSE_EVENT_SYSTEM_START_COMPLETE, self);
    synapse_service_unregister(self->name);
    // ... unregister command ...

    if (self->current_config) cJSON_Delete(self->current_config);
    free(self->private_data);
}

// CLI Command handler code from your old file...
```

## 5. ტესტირება

1. **ააწყვეთ და ჩაწერეთ firmware:** `idf.py build flash monitor`
2. **დააკვირდით ლოგებს:** ყოველ 5 წამში უნდა დაინახოთ `Posting greeting message...`.
3. **გამოიყენეთ CLI:** `greet main_greeter` ბრძანებამ უნდა იმუშაოს.
4. **(ახალი)** სხვა მოდულიდან (მაგალითად, სატესტო მოდულიდან) შეგიძლიათ გამოიძახოთ `synapse_service_get("main_greeter")` და შემდეგ მისი `get_greeting_text` API ფუნქცია.

---

თუ შეგექმნათ პრობლემა, იხილეთ [troubleshooting](../troubleshooting/) დოკუმენტაცია.
