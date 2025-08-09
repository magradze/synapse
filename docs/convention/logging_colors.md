# ლოგირების ფერთა სქემის კონვენცია

**სტატუსი:** საუკეთესო პრაქტიკა (Best Practice)
**ვერსია:** 1.0.0

## 1. 🎯 მიზანი

ეს დოკუმენტი განსაზღვრავს Synapse Framework-ში ლოგირების ტეგების (TAG) გაფერადების სტანდარტულ სქემას. ამ კონვენციის მიზანია, ლოგების ვიზუალური სტრუქტურირება, რაც მნიშვნელოვნად ამარტივებს და აჩქარებს სისტემის მუშაობის ანალიზსა და გამართვას (debugging).

ფერების ლოგიკური დაჯგუფება დეველოპერს საშუალებას აძლევს, ერთი შეხედვით გაარჩიოს, სისტემის რომელი ნაწილიდან (მაგ., `core`, `driver`, `communication`) მოდის კონკრეტული შეტყობინება.

## 2. ⚙️ ტექნიკური იმპლემენტაცია

ფერების მხარდაჭერა რეალიზებულია `logging.h` ჰედერში, `DEFINE_COMPONENT_TAG` მაკროს საშუალებით, რომელიც მეორე არგუმენტად იღებს ANSI ფერის კოდს.

**გამოყენების წესი:**

1.  **`menuconfig`-ში ფერების ჩართვა:** დარწმუნდით, რომ `Component config` -> `ESP System Settings` -> `Channel for console output`-ში ჩართულია `[*] Use ANSI escape codes to colorize console output` ოფცია.
2.  **მაკროს გამოძახება:** მოდულის `.c` ფაილში, გამოიძახეთ მაკრო შესაბამისი ფერის კონსტანტით:

    ```c
    #include "synapse.h" // მოიცავს logging.h-ს

    // მაგალითი communications მოდულისთვის
    DEFINE_COMPONENT_TAG("WIFI_MANAGER", SYNAPSE_LOG_COLOR_CYAN);
    ```

## 3. 🎨 ფერთა სქემა

ქვემოთ მოცემულია რეკომენდებული ფერები მოდულების კატეგორიების მიხედვით. გთხოვთ, დაიცვათ ეს სქემა ახალი მოდულების შექმნისას.

| კატეგორია | ფერი | ANSI კონსტანტა | რატომ ეს ფერი? | მაგალითები |
| :--- | :--- | :--- | :--- | :--- |
| **Core Infrastructure** | **ნათელი ლურჯი** | `BRIGHT_BLUE` | ასოცირდება სტაბილურობასთან, სანდოობასთან და ფუნდამენტურ ინფრასტრუქტურასთან. | `SYSTEM_MANAGER`, `EVENT_BUS`, `SERVICE_LOCATOR`, `PROMISE_MANAGER`, `TASK_POOL_MANAGER` |
| **Communications** | **ცისფერი** | `CYAN` | ასოცირდება კომუნიკაციასთან, ქსელებთან და "ჰაერთან". | `WIFI_MANAGER`, `MQTT_MANAGER`, `BLE_PROVISIONING` |
| **Hardware Drivers** | **იასამნისფერი** | `MAGENTA` | გამორჩეული, ტექნიკური ფერი, რომელიც კარგად გამოყოფს დაბალი დონის, აპარატურულ ლოგებს. | `I2C_BUS`, `SPI_BUS`, `ROTARY_ENCODER_INPUT`, `MCP23017_EXPANDER` |
| **System Services** | **ლურჯი** | `BLUE` | ლოგიკურად აგრძელებს `core`-ის სქემას, მაგრამ ნაკლებად "კაშკაშაა", რაც მათ დამხმარე როლს უსვამს ხაზს. | `SYSTEM_TIMER`, `HEALTH_MONITOR`, `CONN_WATCHDOG`, `TIME_SYNC` |
| **Utilities & App Logic** | **ყვითელი** | `YELLOW` | ყურადღების მიმქცევი ფერი, რომელიც კარგად გამოარჩევს მაღალი დონის, აპლიკაციის ლოგიკას. | `UI_MANAGER`, `CMD_ROUTER`, `LOGGER_MODULE`, `STORAGE_MANAGER`, `SENSOR_AGGR` |
| **Sensors & Actuators** | **მწვანე** | `GREEN` | ასოცირდება მოქმედებასთან, "წარმატებასთან" და ფიზიკურ გარემოსთან. | `RELAY_ACTUATOR`, `RGB_LED_INDICATOR`, `TIME_OF_FLIGHT_SENSOR` |
| **Security** | **ნათელი იასამნისფერი** | `BRIGHT_MAGENTA` | კრიტიკულად მნიშვნელოვანი, ყურადღების მიმქცევი ფერი უსაფრთხოებასთან დაკავშირებული ლოგებისთვის. | `SEC_STATUS` |

---

### **სრული ფერების სია (`logging.h`-დან)**

- `SYNAPSE_LOG_COLOR_BLACK`
- `SYNAPSE_LOG_COLOR_RED`
- `SYNAPSE_LOG_COLOR_GREEN`
- `SYNAPSE_LOG_COLOR_YELLOW`
- `SYNAPSE_LOG_COLOR_BLUE`
- `SYNAPSE_LOG_COLOR_MAGENTA`
- `SYNAPSE_LOG_COLOR_CYAN`
- `SYNAPSE_LOG_COLOR_WHITE`
- `SYNAPSE_LOG_COLOR_RESET`
- `SYNAPSE_LOG_COLOR_BRIGHT_BLUE`
- `SYNAPSE_LOG_COLOR_BRIGHT_MAGENTA`
- `SYNAPSE_LOG_COLOR_BRIGHT_CYAN`
