# Synapse Command Router API Reference

## 🎯 მიზანი

ეს დოკუმენტი აღწერს `Command Router` მოდულის საჯარო Service API-ს, რომელიც ხელმისაწვდომია სხვა მოდულებისთვის `Service Locator`-ის მეშვეობით. ეს API საშუალებას აძლევს ნებისმიერ მოდულს, დაარეგისტრიროს და მართოს საკუთარი ბრძანებები ცენტრალიზებულად და **კონფლიქტების გარეშე**.

**სერვისის სახელი:** `main_cmd_router` (ან როგორც მითითებულია `config.json`-ში)
**სერვისის ტიპი:** `SYNAPSE_SERVICE_TYPE_CMD_ROUTER_API`
**ინტერფეისის ჰედერი:** `cmd_router_interface.h`

---

## ⚙️ API სტრუქტურა (`cmd_router_api_t`)

ეს არის სტრუქტურა, რომელიც შეიცავს ფუნქციის მაჩვენებლებს `Command Router`-ის სამართავად.

```c
typedef struct
{
    esp_err_t (*register_command)(const cmd_t *command);
    esp_err_t (*unregister_command)(const char *command_name);
    bool (*is_command_registered)(const char *command_name); // <--- განახლებულია
} cmd_router_api_t;
```

---

## 📚 API ფუნქციები

### `esp_err_t register_command(const cmd_t *command)`

არეგისტრირებს ახალ ბრძანებას სისტემის `Command Router`-ში. რეგისტრაციის შემდეგ, ბრძანება ხელმისაწვდომი ხდება ყველა წყაროდან (Serial, Event Bus და ა.შ.).

- **@param[in] `command`**: მაჩვენებელი `const cmd_t` სტრუქტურაზე, რომელიც სრულად აღწერს ბრძანებას.
  - **მნიშვნელოვანია:** ამ სტრუქტურისთვის გამოყოფილი მეხსიერება უნდა დარჩეს ვალიდური მთელი პროგრამის მუშაობის განმავლობაში. რეკომენდებულია მისი `static` ცვლადად გამოცხადება.
- **@return**
  - `ESP_OK`: ბრძანება წარმატებით დარეგისტრირდა.
  - `ESP_ERR_INVALID_ARG`: თუ `command` მაჩვენებელი ან მისი აუცილებელი ველები (`command`, `handler`) არის `NULL`.
  - `ESP_ERR_NO_MEM`: თუ რეგისტრირებული ბრძანებების სია სავსეა (`CONFIG_COMMAND_ROUTER_MAX_COMMANDS`).
  - `ESP_ERR_INVALID_STATE`: თუ ამ სახელის მქონე ბრძანება უკვე არსებობს.

---

### `esp_err_t unregister_command(const char *command_name)`

შლის (აუქმებს რეგისტრაციას) ადრე დარეგისტრირებულ ბრძანებას.

- **@param[in] `command_name`**: იმ ბრძანების სახელი (სტრიქონი), რომლის რეგისტრაციაც უნდა გაუქმდეს.
- **@return**
  - `ESP_OK`: ბრძანება წარმატებით წაიშალა.
  - `ESP_ERR_NOT_FOUND`: თუ მითითებული სახელის მქონე ბრძანება ვერ მოიძებნა.
  - `ESP_ERR_INVALID_ARG`: თუ `command_name` არის `NULL`.

**@note** ეს ფუნქცია აუცილებლად უნდა გამოიძახოს მოდულმა თავის `deinit` ფუნქციაში, რათა სისტემაში არ დარჩეს "მოჩვენება" ბრძანებები.

---

### `bool is_command_registered(const char *command_name)`

ამოწმებს, არის თუ არა მითითებული სახელის მქონე ბრძანება უკვე რეგისტრირებული. ეს ფუნქცია კრიტიკულად მნიშვნელოვანია იმ მოდულებისთვის, რომელთა მრავალი ინსტანციაც შეიძლება არსებობდეს სისტემაში, რათა თავიდან აიცილონ ერთი და იმავე ბრძანების განმეორებითი რეგისტრაცია.

- **@param[in] `command_name`**: შესამოწმებელი ბრძანების სახელი.
- **@return**: `true` თუ ბრძანება არსებობს, `false` წინააღმდეგ შემთხვევაში.

---

## 📖 დამხმარე სტრუქტურა (`cmd_t`)

ეს სტრუქტურა გამოიყენება `register_command` ფუნქციისთვის ბრძანების სრულად აღსაწერად.

```c
typedef struct {
    const char *command;      // ბრძანების სახელი, მაგ., "relay"
    const char *help;         // დახმარების ტექსტი, მაგ., "Controls a relay actuator."
    const char *usage;        // გამოყენების ფორმატი, მაგ., "relay <name> <on|off>"
    int min_args;             // არგუმენტების მინიმალური რაოდენობა (ბრძანების ჩათვლით)
    int max_args;             // არგუმენტების მაქსიმალური რაოდენობა (ბრძანების ჩათვლით)
    
    // ბრძანების დამმუშავებელი callback ფუნქცია
    esp_err_t (*handler)(int argc, char **argv, void *context);
    
    // კონტექსტი, რომელიც გადაეცემა handler-ს (ხშირად, module_t* self)
    void *context;            
} cmd_t;
```

---

## 💡 გამოყენების პატერნი: კონფლიქტისგან დაცული რეგისტრაცია

იმისათვის, რომ თავიდან ავიცილოთ კონფლიქტი, როდესაც სისტემაში ერთი და იმავე ტიპის მრავალი მოდული არსებობს (მაგ., რამდენიმე რელე), გამოიყენეთ **"პირობითი, ერთჯერადი რეგისტრაციის"** პატერნი.

**ლოგიკა:**

1. მოდული გამოიწერს `SYNAPSE_EVENT_SYSTEM_START_COMPLETE` ივენთზე.
2. როდესაც ივენთი მოვა, ის ამოწმებს, არის თუ არა მისი ზოგადი ბრძანება (მაგ., `"relay"`) უკვე რეგისტრირებული `is_command_registered` ფუნქციით.
3. **მხოლოდ იმ შემთხვევაში, თუ ბრძანება არ არის რეგისტრირებული**, ის არეგისტრირებს მას ზოგადი `handler` ფუნქციით.
4. შედეგად, ბრძანებას არეგისტრირებს მხოლოდ პირველი ინსტანცია, რომელიც ამ კოდს გაუშვებს.

**`relay_actuator.c`-ის მაგალითი:**

```c
// In relay_actuator.c

// 1. Handler-ის შექმნა (ის ზოგადია და არ არის დამოკიდებული კონკრეტულ ინსტანციაზე)
static esp_err_t generic_relay_cmd_handler(int argc, char **argv, void *context) {
    const char* instance_name = argv[1];
    const char* action = argv[2];
    
    // იპოვე კონკრეტული რელეს სერვისი Service Locator-ით
    service_handle_t handle = synapse_service_get(instance_name);
    if (!handle) {
        printf("Error: Relay with name '%s' not found.\n", instance_name);
        return ESP_ERR_NOT_FOUND;
    }
    
    // ... გამოიძახე სერვისის API ...
    return ESP_OK;
}

// 2. ივენთის 핸들ერში რეგისტრაცია
static void relay_module_handle_event(module_t *self, const char *event_name, void *event_data) {
    if (strcmp(event_name, SYNAPSE_EVENT_SYSTEM_START_COMPLETE) == 0) {
        
        service_handle_t cmd_router = synapse_service_get("main_cmd_router");
        if (cmd_router) {
            cmd_router_api_t *cmd_api = (cmd_router_api_t *)cmd_router;

            // 3. შეამოწმე, ხომ არ დაგასწრო ვინმემ
            if (!cmd_api->is_command_registered("relay")) {
                ESP_LOGI(TAG, "Module '%s' is registering the generic 'relay' command.", self->name);
                
                // 4. თუ არა, დაარეგისტრირე
                static cmd_t relay_cmd = {
                    .command = "relay",
                    .help = "Controls a relay actuator.",
                    .usage = "relay <instance_name> <on|off|toggle>",
                    .min_args = 3,
                    .max_args = 3,
                    .handler = generic_relay_cmd_handler,
                    .context = NULL // კონტექსტი არ გვჭირდება, რადგან handler-ი ზოგადია
                };
                cmd_api->register_command(&relay_cmd);
            }
        }
    }
    // ...
}
