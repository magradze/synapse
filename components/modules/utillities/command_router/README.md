# ⚙️ მოდული: Command Router & System Shell

**🗂️ კატეგორია:** `utilities`  
**✒️ ავტორი:** Synapse Framework Team  
**🔖 ვერსია:** 1.3.1

## 1. 📜 მიმოხილვა

`Command Router` მოდული არის Synapse Framework-ის **მართვის ცენტრალური კვანძი**. ის უზრუნველყოფს ერთიან, გაფართოებად და უსაფრთხო მექანიზმს სისტემური და მოდულების ბრძანებების მისაღებად, გასაპარსად და შესასრულებლად.

მისი მთავარი მიზანია, სრულად გამიჯნოს ბრძანების **წყარო** (მაგ., 💻 Serial Console, ☁️ MQTT, 🌐 HTTP, 🔵 BLE) ბრძანების **შემსრულებლისგან** (მაგ., `relay_module`, `ota_manager`). ეს ქმნის უკიდურესად მოქნილ არქიტექტურას, სადაც ნებისმიერი მოდული შეიძლება იმართოს ნებისმიერი საკომუნიკაციო არხიდან, კოდის დუბლირების გარეშე.

## 2. 🏛️ არქიტექტურული პრინციპები

- **🤝 ბრძანებების რეგისტრაციის პატერნი:** მოდულები არ ახდენენ ბრძანებების იმპლემენტაციას თავად. ამის ნაცვლად, ისინი არეგისტრირებენ თავიანთ ბრძანებებს და შესაბამის `callback` ფუნქციებს `Command Router`-ში მისი Service API-ს მეშვეობით.
- **🧩 აბსტრაქცია და დელეგირება:** `Command Router` იყენებს ESP-IDF-ის სტანდარტულ `esp_console` კომპონენტს, რათა საიმედოდ დაამუშაოს შემომავალი ტექსტური ბრძანებები, დაანაწილოს ისინი არგუმენტებად (`argc`, `argv` სტილში) და გამოიძახოს სწორი `callback` ფუნქცია.
- **📡 მრავალარხიანობა (Multi-Source Input):** მოდული შექმნილია ისე, რომ ერთდროულად იღებდეს ბრძანებებს რამდენიმე წყაროდან:
    1. **💻 Serial Console (CLI):** ინტერაქტიული, ადამიანისთვის გასაგები ბრძანებების ხაზი, რომელიც იდეალურია გამართვისა და პირდაპირი კონტროლისთვის. ის ინტეგრირებულია სისტემის ლოგირებასთან და არ იწვევს კონფლიქტებს.
    2. **📨 Event Bus:** გამოიწერს `SYNAPSE_EVENT_EXECUTE_COMMAND_STRING` ივენთს, რაც საშუალებას აძლევს სხვა მოდულებს (მაგ., `mqtt_client`), პროგრამულად გაუშვან ბრძანებები.
- **📖 ჩაშენებული დახმარების სისტემა:** `help` ბრძანება ავტომატურად აჩვენებს ყველა რეგისტრირებულ ბრძანებას, მათ აღწერას და გამოყენების ფორმატს.

## 3. 🛠️ კონფიგურაცია

მოდულის კონფიგურაცია ხდება `Kconfig` (`idf.py menuconfig`) და `system_config.json` ფაილების საშუალებით.

- **📝 კონფიგურაციის მაგალითი (`system_config.json`):**

    ```json
    {
      "type": "command_router",
      "enabled": true,
      "config": {
        "instance_name": "main_cmd_router",
        "enable_serial_shell": true,
        "serial_shell_prompt": "SYNAPSE> "
      }
    }
    ```

- **⚙️ კონფიგურაციის პარამეტრები:**

| პარამეტრი              | ტიპი    | აღწერა                                                       | სავალდებულო | Default (`Kconfig`) |
| :---------------------- | :------ | :----------------------------------------------------------- | :----------: | :------------------ |
| `instance_name`         | სტრიქონი | მოდულის ინსტანციის უნიკალური სახელი.                         |      ❌      | `main_cmd_router`   |
| `enable_serial_shell`   | boolean | რთავს/თიშავს ინტერაქტიულ ბრძანებების ხაზს სერიულ პორტზე.      |      ❌      | `true`              |
| `serial_shell_prompt`   | სტრიქონი | სერიული კონსოლის prompt-ის ტექსტი.                           |      ❌      | `"SYNAPSE> "`       |

## 4. 🔌 Service API (`cmd_router_api_t`)

სხვა მოდულებს შეუძლიათ დაარეგისტრირონ თავიანთი ბრძანებები `Service Locator`-ის მეშვეობით. სერვისის ტიპი: **`SYNAPSE_SERVICE_TYPE_CMD_ROUTER_API`**. ინტერფეისი აღწერილია `components/interfaces/include/cmd_router_interface.h`-ში.

- **API ფუნქციები:**
  - `esp_err_t register_command(const cmd_t *command);`
    - არეგისტრირებს ახალ ბრძანებას.
  - `esp_err_t unregister_command(const char *command_name);`
    - აუქმებს ბრძანების რეგისტრაციას. **აუცილებლად უნდა გამოიძახოს მოდულმა თავის `deinit` ფუნქციაში.**

- **`cmd_t` სტრუქტურა:**

    ```c
    typedef struct {
        const char *command;      // ბრძანების სახელი, მაგ., "relay"
        const char *help;         // დახმარების ტექსტი, მაგ., "Controls a relay actuator."
        const char *usage;        // გამოყენების ფორმატი, მაგ., "relay <name> <on|off>"
        int min_args;             // არგუმენტების მინიმალური რაოდენობა (ბრძანების ჩათვლით)
        int max_args;             // არგუმენტების მაქსიმალური რაოდენობა (ბრძანების ჩათვლით)
        esp_err_t (*handler)(int argc, char **argv, void *context);
        void *context;            // კონტექსტი, რომელიც გადაეცემა handler-ს (ხშირად, module_t* self)
    } cmd_t;
    ```

## 5. 🚀 ჩაშენებული ბრძანებები

მოდულს მოყვება რამდენიმე ჩაშენებული, სისტემური ბრძანება:

- **`help`**: აჩვენებს ყველა რეგისტრირებულ ბრძანებას.
- **`modules [--info <instance_name>]`**:
  - არგუმენტების გარეშე, ბეჭდავს სისტემაში არსებული ყველა მოდულის სიას და მათ სტატუსს.
  - `--info` არგუმენტით, ბეჭდავს კონკრეტული მოდულის დეტალურ, დიაგნოსტიკურ ინფორმაციას (თუ მოდული ამ ფუნქციონალს უჭერს მხარს).
- **`reboot`**: ასრულებს მოწყობილობის პროგრამულ გადატვირთვას.

## 6. 💡 გამოყენების მაგალითი

წარმოვიდგინოთ, `relay_module` არეგისტრირებს თავის ბრძანებას.

- **1. ბრძანების რეგისტრაცია (`relay_module.c`-ში):**

    ```c
    // relay_module_init ფუნქციაში
    
    // 1. ვქმნით handler ფუნქციას
    static esp_err_t relay_cmd_handler(int argc, char **argv, void *context) {
        module_t *relay_module_instance = (module_t*)context;
        
        if (argc != 3) {
            printf("Error: Invalid arguments.\nUsage: relay <instance_name> <on|off|toggle>\n");
            return ESP_ERR_INVALID_ARG;
        }
        
        const char* target_instance = argv[1];
        const char* action = argv[2];

        // ... იპოვეთ რელეს API Service Locator-ით target_instance-ის მიხედვით ...
        // ... და გამოიძახეთ შესაბამისი მოქმედება (set_state, toggle) ...
        
        printf("Relay '%s' action '%s' executed.\n", target_instance, action);
        return ESP_OK;
    }
    
    // 2. ვამზადებთ ბრძანების აღწერას
    // მნიშვნელოვანია: ეს სტრუქტურა უნდა იყოს static, რათა მისი მისამართი ვალიდური დარჩეს
    static cmd_t relay_command;
    relay_command = (cmd_t){
        .command = "relay",
        .help = "Controls a relay actuator.",
        .usage = "relay <instance_name> <on|off|toggle>",
        .min_args = 3,
        .max_args = 3,
        .handler = relay_cmd_handler,
        .context = self // გადავეცით module_t* self, როგორც კონტექსტი
    };
    
    // 3. ვიღებთ Command Router-ის API-ს და ვარეგისტრირებთ ბრძანებას
    service_handle_t handle = synapse_service_get("main_cmd_router");
    if (handle) {
        cmd_router_api_t *cmd_api = (cmd_router_api_t *)handle;
        cmd_api->register_command(&relay_command);
    }
    
    // 4. არ დაგავიწყდეთ დერეგისტრაცია relay_module_deinit ფუნქციაში:
    // if (handle) {
    //     cmd_router_api_t *cmd_api = (cmd_router_api_t *)handle;
    //     cmd_api->unregister_command("relay");
    // }
    ```

- **2. ბრძანების შესრულება:**
  - **💻 სერიული პორტიდან:**

      ```bash
      SYNAPSE> relay main_light on
      ```

  - **☁️ MQTT-დან (მაგალითი):**

      ```c
      // mqtt_client.c-ში
      
      // ვქმნით payload-ს Command Router-ისთვის
      synapse_command_payload_t* payload = malloc(sizeof(synapse_command_payload_t));
      strncpy(payload->command_string, "relay main_light on", sizeof(payload->command_string) - 1);
      strncpy(payload->source, "mqtt", sizeof(payload->source) - 1);
      
      // ვაქვეყნებთ ივენთს
      event_data_wrapper_t *wrapper;
      synapse_event_data_wrap(payload, free, &wrapper); // ვიყენებთ სტანდარტულ free-ს
      synapse_event_bus_post(SYNAPSE_EVENT_EXECUTE_COMMAND_STRING, wrapper);
      synapse_event_data_release(wrapper);
      ```

## 7. 🩺 პრობლემების მოგვარება (Troubleshooting)

- **პრობლემა:** კონსოლში იბეჭდება "ნაგავი" ან `ESP_ERR_NO_MEM` შეცდომები.
  - **მიზეზი:** UART პორტის კონფლიქტი ლოგირებასა და კონსოლს შორის.
  - **გადაწყვეტა:** დარწმუნდით, რომ `command_router.c`-ში `serial_shell_task` იყენებს `esp_console_init` და `esp_console_run` ფუნქციებს, როგორც ეს საბოლოო ვერსიაშია. შეამოწმეთ Baud Rate-ის პარამეტრები `menuconfig`-ში და თქვენს სერიულ მონიტორში.
- **პრობლემა:** ბრძანება არ სრულდება, `Command not found`.
  - **მიზეზი:** ბრძანება არ არის რეგისტრირებული, ან რეგისტრაცია ვერ მოხერხდა.
  - **გადაწყვეტა:** შეამოწმეთ ლოგები `CMD_ROUTER: Command registered: '...'` შეტყობინებაზე. დარწმუნდით, რომ `synapse_service_get` წარმატებით აბრუნებს `cmd_router`-ის API-ს.
