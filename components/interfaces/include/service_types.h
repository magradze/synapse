/**
 * @file service_types.h
 * @brief Synapse ESP Framework - ცენტრალიზებული სერვისის ტიპების განსაზღვრებები
 * @version 1.0
 * @date 2025-06-24
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს ყველა სერვისის ტიპის განსაზღვრებას, რომელიც გამოიყენება Service Locator-ში.
 *          ყველა მოდულმა უნდა გამოიყენოს მხოლოდ აქ განსაზღვრული სტანდარტიზებული ტიპები.
 *          ფორმატი: SERVICE_TYPE_{CATEGORY}_{API_TYPE}
 *          მაგალითები: SERVICE_TYPE_DISPLAY_API, SERVICE_TYPE_I2C_BUS_API
 *
 * @note ყველა მოდულმა უნდა გამოიყენოს ეს სტანდარტიზებული ტიპები
 * @warning არასდროს განსაზღვროთ სერვისის ტიპები ამ ფაილის გარეთ
 */
#ifndef SERVICE_TYPES_H
#define SERVICE_TYPES_H


#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>

// =============================================================================
// SERVICE TYPE STANDARD v1.0 - CENTRALIZED DEFINITIONS
// =============================================================================

// --- Display Services ---
/** @brief Display API service type for all display modules (SSD1306, LCD1602, etc.) */
#define SERVICE_TYPE_DISPLAY_API "display_api"

// --- Communication Bus Services ---
/** @brief I2C Bus API service type for I2C bus driver modules */
#define SERVICE_TYPE_I2C_BUS_API "i2c_bus_api"

/** @brief SPI Bus API service type for SPI bus driver modules */
#define SERVICE_TYPE_SPI_BUS_API "spi_bus_api"

/** @brief UART API service type for UART communication modules */
#define SERVICE_TYPE_UART_API "uart_api"

// --- Network Services ---
/** @brief WiFi API service type for WiFi connection modules */
#define SERVICE_TYPE_WIFI_API "wifi_api"

/** @brief MQTT API service type for MQTT communication modules */
#define SERVICE_TYPE_MQTT_API "mqtt_api"

/** @brief HTTP API service type for HTTP client/server modules */
#define SERVICE_TYPE_HTTP_API "http_api"

/** @brief BLE API service type for Bluetooth Low Energy modules */
#define SERVICE_TYPE_BLE_API "ble_api"

// --- Storage Services ---
/** @brief Flash API სერვისის ტიპი flash მეხსიერების წვდომის მოდულებისთვის */
#define SERVICE_TYPE_FLASH_API "flash_api"

/** @brief NVRAM API სერვისის ტიპი არავოლატური RAM მოდულებისთვის */
#define SERVICE_TYPE_NVRAM_API "nvram_api"

/** @brief SD Card API სერვისის ტიპი SD ბარათის წვდომის მოდულებისთვის */
#define SERVICE_TYPE_SD_CARD_API "sd_card_api"

// --- Sensor Services ---
/** @brief Temperature sensor API სერვისის ტიპი */
#define SERVICE_TYPE_TEMPERATURE_API "temperature_api"

/** @brief Humidity sensor API სერვისის ტიპი */
#define SERVICE_TYPE_HUMIDITY_API "humidity_api"

/** @brief Pressure sensor API სერვისის ტიპი */
#define SERVICE_TYPE_PRESSURE_API "pressure_api"

/** @brief Motion sensor API სერვისის ტიპი */
#define SERVICE_TYPE_MOTION_API "motion_api"

/** @brief DHT22 sensor API სერვისის ტიპი */
#define SERVICE_TYPE_DHT22_API "dht22_api"

// --- Actuator Services ---
/** @brief Relay control API სერვისის ტიპი */
#define SERVICE_TYPE_RELAY_API "relay_api"

/** @brief PWM control API სერვისის ტიპი */
#define SERVICE_TYPE_PWM_API "pwm_api"

/** @brief Servo motor API სერვისის ტიპი */
#define SERVICE_TYPE_SERVO_API "servo_api"

/** @brief Motor control API სერვისის ტიპი */
#define SERVICE_TYPE_MOTOR_API "motor_api"

// --- System Services ---
/** @brief Logger API სერვისის ტიპი ლოგირების მოდულებისთვის */
#define SERVICE_TYPE_LOGGER_API "logger_api"

/** @brief Configuration API სერვისის ტიპი კონფიგურაციის მართვის მოდულებისთვის */
#define SERVICE_TYPE_CONFIG_API "config_api"

/** @brief Timer API სერვისის ტიპი ტაიმერის მოდულებისთვის */
#define SERVICE_TYPE_TIMER_API "timer_api"

/** @brief Watchdog API სერვისის ტიპი watchdog მოდულებისთვის */
#define SERVICE_TYPE_WATCHDOG_API "watchdog_api"

/** @brief OTA (Over-The-Air) update API სერვისის ტიპი */
#define SERVICE_TYPE_OTA_API "ota_api"

/** @brief Version management API სერვისის ტიპი */
#define SERVICE_TYPE_VERSION_API "version_api"

/** @brief Health monitoring API სერვისის ტიპი */
#define SERVICE_TYPE_HEALTH_API "health_api"

// --- Security Services ---
/** @brief Cryptography API სერვისის ტიპი */
#define SERVICE_TYPE_CRYPTO_API "crypto_api"

/** @brief Authentication API სერვისის ტიპი */
#define SERVICE_TYPE_AUTH_API "auth_api"

/** @brief TLS/SSL API სერვისის ტიპი */
#define SERVICE_TYPE_TLS_API "tls_api"

// --- Audio Services ---
/** @brief Audio processing API service type */
#define SERVICE_TYPE_AUDIO_API "audio_api"

/** @brief Speaker control API service type */
#define SERVICE_TYPE_SPEAKER_API "speaker_api"

/** @brief Microphone API service type */
#define SERVICE_TYPE_MICROPHONE_API "microphone_api"

// --- Power Management Services ---
/** @brief Power management API service type */
#define SERVICE_TYPE_POWER_API "power_api"

/** @brief Battery monitoring API service type */
#define SERVICE_TYPE_BATTERY_API "battery_api"

/** @brief Battery charger API service type */
#define SERVICE_TYPE_CHARGER_API "charger_api"

// --- Generic Services ---
/** @brief Generic API service type for custom implementations */
#define SERVICE_TYPE_GENERIC_API "generic_api"

/** @brief Custom API service type for specialized modules */
#define SERVICE_TYPE_CUSTOM_API "custom_api"

// =============================================================================
// SERVICE TYPE VALIDATION MACROS
// =============================================================================

/**
 * @brief Validate if a service type is defined in this header
 * @param target_service_type Service type string to validate
 * @return true if service type is valid, false otherwise
 * @note This is a compile-time validation helper
 * @warning This macro performs string comparison at runtime
 */
#define IS_VALID_SERVICE_TYPE(target_service_type) (                    \
    (strcmp(target_service_type, SERVICE_TYPE_DISPLAY_API) == 0) ||     \
    (strcmp(target_service_type, SERVICE_TYPE_I2C_BUS_API) == 0) ||     \
    (strcmp(target_service_type, SERVICE_TYPE_SPI_BUS_API) == 0) ||     \
    (strcmp(target_service_type, SERVICE_TYPE_UART_API) == 0) ||        \
    (strcmp(target_service_type, SERVICE_TYPE_WIFI_API) == 0) ||        \
    (strcmp(target_service_type, SERVICE_TYPE_MQTT_API) == 0) ||        \
    (strcmp(target_service_type, SERVICE_TYPE_HTTP_API) == 0) ||        \
    (strcmp(target_service_type, SERVICE_TYPE_BLE_API) == 0) ||         \
    (strcmp(target_service_type, SERVICE_TYPE_FLASH_API) == 0) ||       \
    (strcmp(target_service_type, SERVICE_TYPE_NVRAM_API) == 0) ||       \
    (strcmp(target_service_type, SERVICE_TYPE_SD_CARD_API) == 0) ||     \
    (strcmp(target_service_type, SERVICE_TYPE_TEMPERATURE_API) == 0) || \
    (strcmp(target_service_type, SERVICE_TYPE_HUMIDITY_API) == 0) ||    \
    (strcmp(target_service_type, SERVICE_TYPE_PRESSURE_API) == 0) ||    \
    (strcmp(target_service_type, SERVICE_TYPE_MOTION_API) == 0) ||      \
    (strcmp(target_service_type, SERVICE_TYPE_DHT22_API) == 0) ||       \
    (strcmp(target_service_type, SERVICE_TYPE_RELAY_API) == 0) ||       \
    (strcmp(target_service_type, SERVICE_TYPE_PWM_API) == 0) ||         \
    (strcmp(target_service_type, SERVICE_TYPE_SERVO_API) == 0) ||       \
    (strcmp(target_service_type, SERVICE_TYPE_MOTOR_API) == 0) ||       \
    (strcmp(target_service_type, SERVICE_TYPE_LOGGER_API) == 0) ||      \
    (strcmp(target_service_type, SERVICE_TYPE_CONFIG_API) == 0) ||      \
    (strcmp(target_service_type, SERVICE_TYPE_TIMER_API) == 0) ||       \
    (strcmp(target_service_type, SERVICE_TYPE_WATCHDOG_API) == 0) ||    \
    (strcmp(target_service_type, SERVICE_TYPE_OTA_API) == 0) ||         \
    (strcmp(target_service_type, SERVICE_TYPE_VERSION_API) == 0) ||     \
    (strcmp(target_service_type, SERVICE_TYPE_HEALTH_API) == 0) ||      \
    (strcmp(target_service_type, SERVICE_TYPE_CRYPTO_API) == 0) ||      \
    (strcmp(target_service_type, SERVICE_TYPE_AUTH_API) == 0) ||        \
    (strcmp(target_service_type, SERVICE_TYPE_TLS_API) == 0) ||         \
    (strcmp(target_service_type, SERVICE_TYPE_AUDIO_API) == 0) ||       \
    (strcmp(target_service_type, SERVICE_TYPE_SPEAKER_API) == 0) ||     \
    (strcmp(target_service_type, SERVICE_TYPE_MICROPHONE_API) == 0) ||  \
    (strcmp(target_service_type, SERVICE_TYPE_POWER_API) == 0) ||       \
    (strcmp(target_service_type, SERVICE_TYPE_BATTERY_API) == 0) ||     \
    (strcmp(target_service_type, SERVICE_TYPE_CHARGER_API) == 0) ||     \
    (strcmp(target_service_type, SERVICE_TYPE_GENERIC_API) == 0) ||     \
    (strcmp(target_service_type, SERVICE_TYPE_CUSTOM_API) == 0))

// =============================================================================
// UTILITY MACROS
// =============================================================================

/**
 * @brief Get service type category from service type string
 * @param input_service_type Service type string
 * @return Category string or "unknown" if not recognized
 * @note This is a helper macro for debugging and logging
 */
#define GET_SERVICE_TYPE_CATEGORY(input_service_type) \
  _get_service_category_internal(input_service_type)

  /**
   * @brief Internal helper function for service type categorization
   * @param service_type_string Service type string to categorize
   * @return Category string
   * @note This is an internal helper, use GET_SERVICE_TYPE_CATEGORY instead
   */
  static inline const char *_get_service_category_internal(const char *service_type_string)
  {
    if (strstr(service_type_string, "display") != NULL)
      return "Display";
    if (strstr(service_type_string, "i2c") != NULL ||
        strstr(service_type_string, "spi") != NULL ||
        strstr(service_type_string, "uart") != NULL)
      return "Communication";
    if (strstr(service_type_string, "wifi") != NULL ||
        strstr(service_type_string, "mqtt") != NULL ||
        strstr(service_type_string, "http") != NULL ||
        strstr(service_type_string, "ble") != NULL)
      return "Network";
    if (strstr(service_type_string, "flash") != NULL ||
        strstr(service_type_string, "nvram") != NULL ||
        strstr(service_type_string, "sd_card") != NULL)
      return "Storage";
    if (strstr(service_type_string, "temperature") != NULL ||
        strstr(service_type_string, "humidity") != NULL ||
        strstr(service_type_string, "pressure") != NULL ||
        strstr(service_type_string, "motion") != NULL ||
        strstr(service_type_string, "dht22") != NULL)
      return "Sensor";
    if (strstr(service_type_string, "relay") != NULL ||
        strstr(service_type_string, "pwm") != NULL ||
        strstr(service_type_string, "servo") != NULL ||
        strstr(service_type_string, "motor") != NULL)
      return "Actuator";
    if (strstr(service_type_string, "logger") != NULL ||
        strstr(service_type_string, "config") != NULL ||
        strstr(service_type_string, "timer") != NULL ||
        strstr(service_type_string, "watchdog") != NULL ||
        strstr(service_type_string, "ota") != NULL ||
        strstr(service_type_string, "version") != NULL ||
        strstr(service_type_string, "health") != NULL)
      return "System";
    if (strstr(service_type_string, "crypto") != NULL ||
        strstr(service_type_string, "auth") != NULL ||
        strstr(service_type_string, "tls") != NULL)
      return "Security";
    if (strstr(service_type_string, "audio") != NULL ||
        strstr(service_type_string, "speaker") != NULL ||
        strstr(service_type_string, "microphone") != NULL)
      return "Audio";
    if (strstr(service_type_string, "power") != NULL ||
        strstr(service_type_string, "battery") != NULL ||
        strstr(service_type_string, "charger") != NULL)
      return "Power";

    return "Unknown";
  }

  // =============================================================================
  // DEPRECATED SERVICE TYPES (DO NOT USE)
  // =============================================================================

  /*
   * The following service types are deprecated and should not be used in new code:
   * - "i2c_api" (use SERVICE_TYPE_I2C_BUS_API)
   * - "display" (use SERVICE_TYPE_DISPLAY_API)
   * - "sensor" (use specific sensor type APIs like SERVICE_TYPE_TEMPERATURE_API)
   * - "actuator" (use specific actuator type APIs like SERVICE_TYPE_RELAY_API)
   * - "communication" (use specific communication APIs like SERVICE_TYPE_MQTT_API)
   */

#ifdef __cplusplus
}
#endif

#endif // SERVICE_TYPES_H