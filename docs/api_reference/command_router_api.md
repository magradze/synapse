# Synapse Command Router API Reference

## 🎯 მიზანი

ეს დოკუმენტი აღწერს `Command Router` მოდულის საჯარო Service API-ს, რომელიც ხელმისაწვდომია სხვა მოდულებისთვის `Service Locator`-ის მეშვეობით. ეს API საშუალებას აძლევს ნებისმიერ მოდულს, დაარეგისტრიროს და მართოს საკუთარი ბრძანებები ცენტრალიზებულად.

**სერვისის სახელი:** `main_cmd_router` (ან როგორც მითითებულია `system_config.json`-ში)  
**სერვისის ტიპი:** `FMW_SERVICE_TYPE_CMD_ROUTER_API`  
**ინტერფეისის ჰედერი:** `cmd_router_interface.h`

---

## ⚙️ API სტრუქტურა (`cmd_router_api_t`)

ეს არის სტრუქტურა, რომელიც შეიცავს ფუნქციის მაჩვენებლებს `Command Router`-ის სამართავად.

```c
typedef struct
{
    esp_err_t (*register_command)(const cmd_t *command);
    esp_err_t (*unregister_command)(const char *command_name);
} cmd_router_api_t;
```

---

## 📚 API ფუნქციები

### esp_err_t register_command(const cmd_t *command)

არეგისტრირებს ახალ ბრძანებას სისტემის `Command Router`-ში. რეგისტრაციის შემდეგ, ბრძანება ხელმისაწვდომი ხდება ყველა წყაროდან (Serial, Event Bus და ა.შ.).

- **@param[in] `command`**: მაჩვენებელი `const cmd_t` სტრუქტურაზე, რომელიც სრულად აღწერს ბრძანებას.
  - **მნიშვნელოვანია:** ამ სტრუქტურისთვის გამოყოფილი მეხსიერება უნდა დარჩეს ვალიდური მთელი პროგრამის მუშაობის განმავლობაში. რეკომენდებულია მისი `static` ცვლადად გამოცხადება.

- **@return**
  - `ESP_OK`: ბრძანება წარმატებით დარეგისტრირდა.
  - `ESP_ERR_INVALID_ARG`: თუ `command` მაჩვენებელი ან მისი აუცილებელი ველები (`command`, `handler`) არის `NULL`.
  - `ESP_ERR_NO_MEM`: თუ რეგისტრირებული ბრძანებების სია სავსეა (`CONFIG_COMMAND_ROUTER_MAX_COMMANDS`).
  - `ESP_ERR_INVALID_STATE`: თუ ამ სახელის მქონე ბრძანება უკვე არსებობს.

---

### esp_err_t unregister_command(const char *command_name)

შლის (აუქმებს რეგისტრაციას) ადრე დარეგისტრირებულ ბრძანებას.

- **@param[in] `command_name`**: იმ ბრძანების სახელი (სტრიქონი), რომლის რეგისტრაციაც უნდა გაუქმდეს.

- **@return**
  - `ESP_OK`: ბრძანება წარმატებით წაიშალა.
  - `ESP_ERR_NOT_FOUND`: თუ მითითებული სახელის მქონე ბრძანება ვერ მოიძებნა.
  - `ESP_ERR_INVALID_ARG`: თუ `command_name` არის `NULL`.

**@note** ეს ფუნქცია აუცილებლად უნდა გამოიძახოს მოდულმა თავის `deinit` ფუნქციაში, რათა სისტემაში არ დარჩეს "მოჩვენება" ბრძანებები.

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

## 💡 გამოყენების მაგალითი

`relay_module`-ის `init` ფუნქციაში ბრძანების რეგისტრაცია:

```c
// In relay_module.c

// 1. Handler-ის შექმნა
static esp_err_t relay_cmd_handler(int argc, char **argv, void *context) {
    // ... handler-ის ლოგიკა ...
    return ESP_OK;
}

// 2. init ფუნქციაში რეგისტრაცია
static esp_err_t relay_module_init(module_t *self) {
    // ...
    
    // ბრძანების სტრუქტურის მომზადება
    static cmd_t relay_command;
    relay_command = (cmd_t){
        .command = "relay",
        .help = "Controls a relay actuator.",
        .usage = "relay <instance_name> <on|off>",
        .min_args = 3,
        .max_args = 3,
        .handler = relay_cmd_handler,
        .context = self // გადავეცით module_t* self, როგორც კონტექსტი
    };

    // Command Router სერვისის მოძიება და ბრძანების რეგისტრაცია
    service_handle_t handle = fmw_service_get("main_cmd_router");
    if (handle) {
        cmd_router_api_t *cmd_api = (cmd_router_api_t *)handle;
        esp_err_t err = cmd_api->register_command(&relay_command);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to register 'relay' command: %s", esp_err_to_name(err));
        }
    } else {
        ESP_LOGW(TAG, "Command Router service not found. Cannot register commands.");
    }

    // ...
    return ESP_OK;
}
```
