/**
 * @file service_types.h
 * @brief Synapse ESP Framework - ცენტრალიზებული სერვისის ტიპების განსაზღვრებები.
 * @version 2.0
 * @date 2025-06-26
 * @author Giorgi Magradze
 * @details ეს ფაილი განსაზღვრავს ყველა სერვისის ტიპს `enum`-ის სახით, რაც
 *          უზრუნველყოფს compile-time ტიპების შემოწმებას და გამორიცხავს "magic string"-ებს.
 *          Service Locator-ში რეგისტრაციისას, მოდულებმა უნდა გამოიყენონ მხოლოდ აქ განსაზღვრული ტიპები.
 *          ასევე, მოცემულია დამხმარე ფუნქცია `enum`-ის სტრიქონად გადასაყვანად (დიბაგინგისთვის).
 */
#ifndef FMW_SERVICE_TYPES_H
#define FMW_SERVICE_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @enum fmw_service_type_t
   * @brief სერვისის ტიპების ცენტრალიზებული ჩამონათვალი.
   */
  typedef enum
  {
    SERVICE_TYPE_UNKNOWN = 0,

    // --- Display Services ---
    SERVICE_TYPE_DISPLAY_API,

    // --- Communication Bus Services ---
    SERVICE_TYPE_I2C_BUS_API,
    SERVICE_TYPE_SPI_BUS_API,
    SERVICE_TYPE_UART_API,

    // --- Network Services ---
    SERVICE_TYPE_WIFI_API,
    SERVICE_TYPE_MQTT_API,
    SERVICE_TYPE_HTTP_API,
    SERVICE_TYPE_BLE_API,

    // --- Storage Services ---
    SERVICE_TYPE_FLASH_API,
    SERVICE_TYPE_NVRAM_API,
    SERVICE_TYPE_SD_CARD_API,

    // --- Sensor Services ---
    SERVICE_TYPE_TEMPERATURE_API,
    SERVICE_TYPE_HUMIDITY_API,
    SERVICE_TYPE_PRESSURE_API,
    SERVICE_TYPE_MOTION_API,
    SERVICE_TYPE_DHT22_API,

    // --- Actuator Services ---
    SERVICE_TYPE_RELAY_API,
    SERVICE_TYPE_PWM_API,
    SERVICE_TYPE_SERVO_API,
    SERVICE_TYPE_MOTOR_API,

    // --- System Services ---
    SERVICE_TYPE_LOGGER_API,
    SERVICE_TYPE_CONFIG_API,
    SERVICE_TYPE_TIMER_API,
    SERVICE_TYPE_WATCHDOG_API,
    SERVICE_TYPE_OTA_API,
    SERVICE_TYPE_VERSION_API,
    SERVICE_TYPE_HEALTH_API,

    // --- Security Services ---
    SERVICE_TYPE_CRYPTO_API,
    SERVICE_TYPE_AUTH_API,
    SERVICE_TYPE_TLS_API,

    // --- Audio Services ---
    SERVICE_TYPE_AUDIO_API,
    SERVICE_TYPE_SPEAKER_API,
    SERVICE_TYPE_MICROPHONE_API,

    // --- Power Management Services ---
    SERVICE_TYPE_POWER_API,
    SERVICE_TYPE_BATTERY_API,
    SERVICE_TYPE_CHARGER_API,

    // --- Generic Services ---
    SERVICE_TYPE_GENERIC_API,
    SERVICE_TYPE_CUSTOM_API,

    // ეს ელემენტი ყოველთვის უნდა იყოს ბოლო, ვალიდაციისთვის
    SERVICE_TYPE_MAX
  } fmw_service_type_t;

  /**
   * @brief გადაყავს სერვისის ტიპის enum-ი მის სტრიქონულ წარმოდგენაზე.
   *
   * @details სასარგებლოა ლოგირებისა და დიაგნოსტიკისთვის, რათა ეკრანზე
   *          გამოჩნდეს გასაგები სტრიქონი და არა უბრალოდ რიცხვი.
   * @param[in] type სერვისის ტიპის `enum` მნიშვნელობა.
   * @return const char* სერვისის ტიპის სტრიქონული სახელი, ან "UNKNOWN_SERVICE_TYPE" თუ ტიპი არასწორია.
   */
  static inline const char *fmw_service_type_to_string(fmw_service_type_t type)
  {
    switch (type)
    {
    // Display Services
    case SERVICE_TYPE_DISPLAY_API:
      return "display_api";

    // Communication Bus Services
    case SERVICE_TYPE_I2C_BUS_API:
      return "i2c_bus_api";
    case SERVICE_TYPE_SPI_BUS_API:
      return "spi_bus_api";
    case SERVICE_TYPE_UART_API:
      return "uart_api";

    // Network Services
    case SERVICE_TYPE_WIFI_API:
      return "wifi_api";
    case SERVICE_TYPE_MQTT_API:
      return "mqtt_api";
    case SERVICE_TYPE_HTTP_API:
      return "http_api";
    case SERVICE_TYPE_BLE_API:
      return "ble_api";

    // Storage Services
    case SERVICE_TYPE_FLASH_API:
      return "flash_api";
    case SERVICE_TYPE_NVRAM_API:
      return "nvram_api";
    case SERVICE_TYPE_SD_CARD_API:
      return "sd_card_api";

    // Sensor Services
    case SERVICE_TYPE_TEMPERATURE_API:
      return "temperature_api";
    case SERVICE_TYPE_HUMIDITY_API:
      return "humidity_api";
    case SERVICE_TYPE_PRESSURE_API:
      return "pressure_api";
    case SERVICE_TYPE_MOTION_API:
      return "motion_api";
    case SERVICE_TYPE_DHT22_API:
      return "dht22_api";

    // Actuator Services
    case SERVICE_TYPE_RELAY_API:
      return "relay_api";
    case SERVICE_TYPE_PWM_API:
      return "pwm_api";
    case SERVICE_TYPE_SERVO_API:
      return "servo_api";
    case SERVICE_TYPE_MOTOR_API:
      return "motor_api";

    // System Services
    case SERVICE_TYPE_LOGGER_API:
      return "logger_api";
    case SERVICE_TYPE_CONFIG_API:
      return "config_api";
    case SERVICE_TYPE_TIMER_API:
      return "timer_api";
    case SERVICE_TYPE_WATCHDOG_API:
      return "watchdog_api";
    case SERVICE_TYPE_OTA_API:
      return "ota_api";
    case SERVICE_TYPE_VERSION_API:
      return "version_api";
    case SERVICE_TYPE_HEALTH_API:
      return "health_api";

    // Security Services
    case SERVICE_TYPE_CRYPTO_API:
      return "crypto_api";
    case SERVICE_TYPE_AUTH_API:
      return "auth_api";
    case SERVICE_TYPE_TLS_API:
      return "tls_api";

    // Audio Services
    case SERVICE_TYPE_AUDIO_API:
      return "audio_api";
    case SERVICE_TYPE_SPEAKER_API:
      return "speaker_api";
    case SERVICE_TYPE_MICROPHONE_API:
      return "microphone_api";

    // Power Management Services
    case SERVICE_TYPE_POWER_API:
      return "power_api";
    case SERVICE_TYPE_BATTERY_API:
      return "battery_api";
    case SERVICE_TYPE_CHARGER_API:
      return "charger_api";

    // Generic Services
    case SERVICE_TYPE_GENERIC_API:
      return "generic_api";
    case SERVICE_TYPE_CUSTOM_API:
      return "custom_api";

    // Default and Unknown cases
    case SERVICE_TYPE_UNKNOWN:
    case SERVICE_TYPE_MAX:
    default:
      return "UNKNOWN_SERVICE_TYPE";
    }
  }

/**
 * @brief ამოწმებს, არის თუ არა სერვისის ტიპი ვალიდური.
 * @param type შესამოწმებელი სერვისის ტიპი.
 * @return true თუ ტიპი ვალიდურია, false წინააღმდეგ შემთხვევაში.
 */
#define IS_VALID_SERVICE_TYPE(type) ((type) > SERVICE_TYPE_UNKNOWN && (type) < SERVICE_TYPE_MAX)

#ifdef __cplusplus
}
#endif

#endif // FMW_SERVICE_TYPES_H