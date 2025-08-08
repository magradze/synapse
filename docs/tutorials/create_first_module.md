# პირველი საკუთარი მოდულის შექმნა (Create Your First Module)

## მიზანი

ეს სახელმძღვანელო გაჩვენებთ, როგორ შექმნათ ახალი, სრულფასოვანი მოდული Synapse ESP Framework-ში. ჩვენ გამოვიყენებთ `create_module.py` სკრიპტს, რათა ავტომატურად დავაგენერიროთ მოდულის ჩონჩხი და შემდეგ შევავსოთ ის საბაზისო ლოგიკით, მათ შორის, როგორ დავამატოთ CLI ბრძანება უსაფრთხოდ, კონფლიქტების გარეშე.

## 1. საჭიროების განსაზღვრა

- **ამოცანა:** შევქმნათ მარტივი "მისალმების" მოდული, სახელად `greeter_module`.
- **ფუნქციონალი:**
    1. `start` ფაზაში, `System Timer` სერვისის გამოყენებით, დაგეგმოს პერიოდული ივენთი `GREETING_TIMER_TICK`.
    2. როდესაც მიიღებს ამ ივენთს, `Event Bus`-ზე გამოაქვეყნოს ახალი, საკუთარი ივენთი `GREETING_MESSAGE_READY` `payload`-ით, რომელიც შეიცავს მისალმების ტექსტს.
    3. დაარეგისტრიროს `greet` CLI ბრძანება, რომელიც დაბეჭდავს კონკრეტული ინსტანციის მისალმების ტექსტს.
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
    - **აირჩიეთ მოდულის არქეტიპი:** `Event & Command` (რადგან ჩვენი მოდული ივენთებსაც გამოაქვეყნებს და ბრძანებასაც დაამუშავებს).
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

ახლა სრულად ჩავანაცვლოთ `greeter_module.c` ფაილის შიგთავსი. სკრიპტმა დააგენერირა ჩონჩხი, მაგრამ ჩვენ მას სრულად შევავსებთ ჩვენი ლოგიკით.

**`src/greeter_module.c` (საბოლოო ვერსია):**

```c
/**
 * @file greeter_module.c
 * @brief A simple module that periodically sends greetings and provides a CLI command.
 * @author (Your Name)
 */
#include "synapse.h" // ერთიანი ჰედერი ფრეიმვორქისთვის
#include "greeter_module.h"
#include "timer_interface.h"
#include "cmd_router_interface.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("GREETER_MODULE");

// Event names used by this module
#define GREETING_TIMER_TICK "GREETING_TIMER_TICK"
#define GREETING_MESSAGE_READY "GREETING_MESSAGE_READY"

// Private Data Structure
typedef struct {
    char instance_name[32];
    char greeting_text[64];
    synapse_timer_handle_t greeting_timer;
} greeter_private_data_t;

// Forward Declarations
static esp_err_t greeter_module_init(module_t *self);
static esp_err_t greeter_module_start(module_t *self);
static void greeter_module_deinit(module_t *self);
static void greeter_module_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t greeter_cmd_handler(int argc, char **argv, void *context);

// Create Function
module_t *greeter_module_create(const cJSON *config) {
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    greeter_private_data_t *private_data = (greeter_private_data_t *)calloc(1, sizeof(greeter_private_data_t));
    if (!module || !private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        if (config) cJSON_Delete((cJSON*)config);
        return NULL;
    }

    module->private_data = private_data;
    module->current_config = (cJSON*)config;

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
    synapse_event_bus_subscribe(GREETING_TIMER_TICK, self);
    synapse_event_bus_subscribe(SYNAPSE_EVENT_SYSTEM_START_COMPLETE, self);
    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

// Start Function
static esp_err_t greeter_module_start(module_t *self) {
    ESP_LOGI(TAG, "'%s' starting...", self->name);
    greeter_private_data_t *p_data = (greeter_private_data_t *)self->private_data;

    service_handle_t timer_service = synapse_service_get("main_timer_service");
    if (timer_service) {
        timer_api_t *timer_api = (timer_api_t *)timer_service;
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
        char* message_payload = strdup(p_data->greeting_text);
        event_data_wrapper_t *wrapper;
        if (synapse_event_data_wrap(message_payload, free, &wrapper) == ESP_OK) {
            synapse_event_bus_post(GREETING_MESSAGE_READY, wrapper);
            synapse_event_data_release(wrapper);
        }
    } else if (strcmp(event_name, SYNAPSE_EVENT_SYSTEM_START_COMPLETE) == 0) {
        service_handle_t cmd_router = synapse_service_get("main_cmd_router");
        if (cmd_router) {
            cmd_router_api_t *cmd_api = (cmd_router_api_t *)cmd_router;
            if (!cmd_api->is_command_registered("greet")) {
                ESP_LOGI(TAG, "Module '%s' is registering the generic 'greet' command.", self->name);
                static cmd_t greet_cmd = {
                    .command = "greet",
                    .help = "Prints a greeting message from a specific instance.",
                    .usage = "greet <instance_name>",
                    .min_args = 2,
                    .max_args = 2,
                    .handler = greeter_cmd_handler,
                    .context = NULL
                };
                cmd_api->register_command(&greet_cmd);
            }
        }
    }

    if (event_data) {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// Deinit Function
static void greeter_module_deinit(module_t *self) {
    if (!self) return;
    greeter_private_data_t *p_data = (greeter_private_data_t *)self->private_data;

    service_handle_t timer_service = synapse_service_get("main_timer_service");
    if (timer_service && p_data->greeting_timer) {
        ((timer_api_t *)timer_service)->cancel_event(p_data->greeting_timer);
    }

    synapse_event_bus_unsubscribe(GREETING_TIMER_TICK, self);
    synapse_event_bus_unsubscribe(SYNAPSE_EVENT_SYSTEM_START_COMPLETE, self);

    // Unregister the command (optional, but good practice)
    service_handle_t cmd_router = synapse_service_get("main_cmd_router");
    if (cmd_router) {
        ((cmd_router_api_t *)cmd_router)->unregister_command("greet");
    }

    if (self->current_config) cJSON_Delete(self->current_config);
    if (self->private_data) free(self->private_data);
    free(self);
}

// Command Handler
static esp_err_t greeter_cmd_handler(int argc, char **argv, void *context) {
    const char* instance_name = argv[1];
    
    module_t* target_module = synapse_module_registry_find_by_name(instance_name);
    
    if (target_module) {
        // Check if the found module is actually a greeter_module.
        // This is an advanced check, not strictly necessary for this example.
        // We can assume it is for now.
        greeter_private_data_t* p_data = (greeter_private_data_t*)target_module->private_data;
        printf("Message from '%s': %s\n", instance_name, p_data->greeting_text);
        return ESP_OK;
    } else {
        printf("Error: Greeter module with name '%s' not found.\n", instance_name);
        return ESP_ERR_NOT_FOUND;
    }
}
```

## 5. ტესტირება

1. **ააწყვეთ და ჩაწერეთ firmware:**

    ```bash
    idf.py build flash monitor
    ```

2. **დააკვირდით ლოგებს:** თქვენ უნდა დაინახოთ, რომ `greeter_module` იქმნება, იწყებს მუშაობას და ყოველ 5 წამში ერთხელ ბეჭდავს `Posting greeting message.` შეტყობინებას.
3. **გამოიყენეთ CLI:** ტერმინალში აკრიფეთ `greet main_greeter`. ეკრანზე უნდა დაიბეჭდოს: `Message from 'main_greeter': Hello from Synapse!`.

---

თუ შეგექმნათ პრობლემა, იხილეთ [troubleshooting](../troubleshooting/) ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს.
