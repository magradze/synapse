/**
 * @file event_payloads.h
 * @brief ტელემეტრიისა და სისტემური ივენთების მონაცემთა სტრუქტურები.
 *
 * @details ეს ფაილი განსაზღვრავს მონაცემთა სტრუქტურებს (payloads), რომლებიც გამოიყენება
 *          Synapse ESP Framework-ის Event Bus-ში. ის ასევე შეიცავს დამხმარე
 *          ფუნქციებს ამ სტრუქტურების მეხსიერების უსაფრთხო მართვისთვის.
 *
 * @version 2.0
 * @date 2025-06-26
 * @author Giorgi Magradze
 */

#ifndef FMW_EVENT_PAYLOADS_H
#define FMW_EVENT_PAYLOADS_H

#include "framework_config.h" // Kconfig-ის პარამეტრებისთვის
#include <sdkconfig.h>
#include <stdlib.h> // for free(), malloc()
#include <string.h> // for strncpy()
#include "cJSON.h"
// =========================================================================
//                      Payload სტრუქტურები
// =========================================================================

/**
 * @struct fmw_telemetry_payload_t
 * @brief ზოგადი სტრუქტურა ტელემეტრიული მონაცემების გადასაცემად.
 */
typedef struct {
    char module_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH]; /**< @brief მოდულის სახელი, რომელიც აქვეყნებს მონაცემებს. */
    char *json_data;                                     /**< @brief მონაცემები JSON სტრიქონის სახით (დინამიურად გამოყოფილი). */
} fmw_telemetry_payload_t;

/**
 * @struct fmw_config_updated_payload_t
 * @brief განსაზღვრავს მონაცემთა სტრუქტურას FRAMEWORK_EVENT_CONFIG_UPDATED ივენთისთვის.
 */
typedef struct
{
    char updated_key[CONFIG_FMW_NVS_KEY_MAX_LENGTH]; /**< @brief განახლებული კონფიგურაციის გასაღები. */
} fmw_config_updated_payload_t;

/**
 * @struct fmw_health_pong_payload_t
 * @brief განსაზღვრავს მონაცემთა სტრუქტურას FRAMEWORK_EVENT_HEALTH_PONG ივენთისთვის.
 */
typedef struct
{
    char module_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH]; /**< @brief მოდულის სახელი, რომელიც პასუხობს "ping"-ს. */
} fmw_health_pong_payload_t;

/**
 * @struct fmw_module_control_payload_t
 * @brief ზოგადი სტრუქტურა მოდულის მართვის ივენთებისთვის (enable/disable).
 */
typedef struct
{
    char module_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH]; /**< @brief სამიზნე მოდულის სახელი. */
} fmw_module_control_payload_t;

/**
 * @struct fmw_command_payload_t
 * @brief Defines the data structure for the FMW_EVENT_EXECUTE_COMMAND_STRING event.
 */

typedef struct
{
    /**
     * @brief The full command string to be executed (e.g., "relay main_light on").
     */
    char command_string[CONFIG_COMMAND_ROUTER_MAX_CMD_LEN];

    /**
     * @brief A string identifying the source of the command (e.g., "mqtt", "serial", "http").
     */
    char source[16];

} fmw_command_payload_t;

/**
 * @struct fmw_connectivity_payload_t
 * @brief Defines the data structure for connectivity-related events.
 * @details Used with FMW_EVENT_CONNECTIVITY_ESTABLISHED and FMW_EVENT_CONNECTIVITY_LOST.
 */
typedef struct
{
    /**
     * @brief The name of the check that changed state (e.g., "MQTT_Heartbeat", "PING_8.8.8.8").
     *        This name must match a check name defined in the Connectivity Watchdog's config.
     */
    char check_name[32];
} fmw_connectivity_payload_t;

// =========================================================================
//                      Cleanup (მეხსიერების გასუფთავების) ფუნქციები
// =========================================================================

/**
 * @brief ზოგადი cleanup ფუნქცია მარტივი, `malloc`-ით შექმნილი payload-ებისთვის.
 * @details ეს ფუნქცია გადაეცემა `fmw_event_data_wrap`-ს იმ payload-ებისთვის,
 *          რომლებსაც არ სჭირდებათ სპეციალური გასუფთავების ლოგიკა.
 * @param data void მაჩვენებელი გასათავისუფლებელ ობიექტზე.
 */
static inline void fmw_payload_common_free(void *data)
{
    if (data)
    {
        free(data);
    }
}

/**
 * @brief ათავისუფლებს ტელემეტრიის პეილოდს.
 * @param data პეილოდის მაჩვენებელი.
 */
void fmw_telemetry_payload_free(void *data);

#endif // FMW_EVENT_PAYLOADS_H