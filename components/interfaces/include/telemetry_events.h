/**
 * @file telemetry_events.h
 * @brief ტელემეტრიისა და სისტემური ივენთების მონაცემთა სტრუქტურები და შესაბამისი cleanup ფუნქციები.
 *
 * ეს ფაილი განსაზღვრავს სტრუქტურებსა და დამხმარე ფუნქციებს Synapse ESP Framework-ის Event Bus-ისთვის,
 * მათ შორის: ტელემეტრიის, კონფიგურაციის განახლების, მოდულის ჩართვა/გამორთვისა და ჯანმრთელობის მონიტორინგის ივენთებისთვის.
 *
 * @version 1.0
 * @date 2025-06-24
 * @author Giorgi Magradze
 */

#ifndef TELEMETRY_EVENTS_H
#define TELEMETRY_EVENTS_H

#include <stddef.h>
#include <stdlib.h> // for free()
#include <string.h> // for strncpy

/**
 * @def FRAMEWORK_CONFIG_KEY_MAX_LEN
 * @brief განსაზღვრავს კონფიგურაციის გასაღების მაქსიმალურ სიგრძეს NVS-ში (+1 null-ბაიტისთვის).
 *
 * @details ეს მნიშვნელობა უნდა ემთხვეოდეს NVS-ის მიერ დაშვებულ მაქსიმუმს (15 სიმბოლო).
 *          მას აქ ვსაზღვრავთ, რათა 'interfaces' კომპონენტი არ გახდეს დამოკიდებული 'nvs_flash'-ზე.
 */
#define FRAMEWORK_CONFIG_KEY_MAX_LEN (15 + 1)

/**
 * @struct telemetry_data_t
 * @brief ზოგადი სტრუქტურა ტელემეტრიული მონაცემების გადასაცემად.
 *
 * @details ეს სტრუქტურა შეიცავს JSON ფორმატის სტრიქონს, რაც საშუალებას გვაძლევს,
 *          ნებისმიერი ტიპის და სტრუქტურის მონაცემი გადავცეთ.
 *
 * @var telemetry_data_t::module_name
 *      რომელი მოდული აქვეყნებს ტელემეტრიულ მონაცემებს
 * @var telemetry_data_t::json_data
 *      მონაცემები JSON სტრიქონის სახით
 */
typedef struct {
    char module_name[32]; /**< რომელი მოდული აქვეყნებს */
    char *json_data;      /**< მონაცემები JSON სტრიქონის სახით */
} telemetry_data_t;

/**
 * @struct config_updated_payload_t
 * @brief განსაზღვრავს მონაცემთა სტრუქტურას FRAMEWORK_EVENT_CONFIG_UPDATED ივენთისთვის.
 *
 * @details ეს payload-ი იგზავნება `config_handler_module`-ის მიერ, როდესაც კონფიგურაციის
 *          მნიშვნელობა წარმატებით განახლდა NVS-ში (Non-Volatile Storage).
 *          ის საშუალებას აძლევს სხვა მოდულებს მოისმინონ კონკრეტული კონფიგურაციის ცვლილებები
 *          და დინამიურად შეცვალონ მათი ქცევა სისტემის ხელახლა ჩართვის გარეშე.
 *
 *          მაგალითად, სენსორის მოდული შეიძლება მოისმინოს ეს ივენთი, რათა განახლდეს
 *          მისი მონაცემთა გამოკითხვის ინტერვალი ფრენტზე.
 *
 * @var config_updated_payload_t::updated_key
 *      NVS-ის გასაღები კონფიგურაციის პარამეტრისა, რომელიც ახლახან განახლდა.
 *      ზომა დაფუძნებულია NVS_KEY_NAME_MAX_SIZE-ზე, რათა ვარაუდოთ, რომ არასოდეს გადაივსება.
 */
typedef struct
{
    char updated_key[FRAMEWORK_CONFIG_KEY_MAX_LEN]; /**< განახლებული გასაღები */
} config_updated_payload_t;

/**
 * @brief Cleanup ფუნქცია config_updated_payload_t-სთვის.
 *
 * @details ეს ფუნქცია გადაეცემა `fmw_event_data_wrap`-ს `config_updated_payload_t`
 *          ობიექტის wrapper-ის შექმნისას, რათა Event Bus-მა შეძლოს მეხსიერების
 *          უსაფრთხოდ გათავისუფლება.
 *
 * @param data void მაჩვენებელი გასათავისუფლებელ `config_updated_payload_t` ობიექტზე.
 */
static inline void free_config_updated_payload(void *data)
{
    if (data)
    {
        free(data);
    }
}

/**
 * @brief ათავისუფლებს telemetry_data_t სტრუქტურის მიერ დაკავებულ მეხსიერებას.
 *
 * @details ეს ფუნქცია უნდა გამოიძახოს იმ მომხმარებელმა, რომელიც ბოლოს იყენებს
 *          telemetry_data_t ობიექტს. ის ჯერ ათავისუფლებს შიდა json_data სტრიქონს,
 *          შემდეგ კი თავად სტრუქტურის კონტეინერს.
 *
 * @param payload მაჩვენებელი გასათავისუფლებელ telemetry_data_t ობიექტზე.
 */
void free_telemetry_payload(void *payload);

/**
 * @brief Cleanup ფუნქცია telemetry_data_t-სთვის.
 *
 * @details ათავისუფლებს დინამიურად გამოყოფილ json_data სტრიქონს და თავად სტრუქტურას.
 *
 * @param data void მაჩვენებელი გასათავისუფლებელ `telemetry_data_t` ობიექტზე.
 */
static inline void free_telemetry_data(void *data)
{
    if (!data)
    {
        return;
    }

    telemetry_data_t *telemetry = (telemetry_data_t *)data;
    if (telemetry->json_data)
    {
        free(telemetry->json_data);
    }
    free(telemetry);
}

/**
 * @struct health_pong_payload_t
 * @brief განსაზღვრავს მონაცემთა სტრუქტურას FRAMEWORK_EVENT_HEALTH_PONG ივენთისთვის.
 *
 * @details ეს payload-ი იგზავნება მოდულის მიერ, რათა `health_monitor`-ს
 *          შეატყობინოს თავისი "ჯანმრთელი" მდგომარეობის შესახებ.
 *
 * @var health_pong_payload_t::module_name
 *      მოდულის ინსტანციის სახელი, რომელიც პასუხობს
 */
typedef struct
{
    char module_name[32]; /**< მოდულის ინსტანციის სახელი, რომელიც პასუხობს. */
} health_pong_payload_t;

/**
 * @brief Cleanup ფუნქცია health_pong_payload_t-სთვის.
 *
 * @details ეს ფუნქცია გადაეცემა `fmw_event_data_wrap`-ს `HEALTH_PONG`
 *          ივენთის შექმნისას, რათა Event Bus-მა შეძლოს მეხსიერების
 *          უსაფრთხოდ გათავისუფლება.
 *
 * @param data void მაჩვენებელი გასათავისუფლებელ `health_pong_payload_t` ობიექტზე.
 */
static inline void free_health_pong_payload(void *data)
{
    if (data)
    {
        free(data);
    }
}

/**
 * @struct module_disable_payload_t
 * @brief განსაზღვრავს მონაცემთა სტრუქტურას `FRAMEWORK_EVENT_MODULE_DISABLE_REQUESTED` ივენთისთვის.
 *
 * @details ეს payload-ი იგზავნება, როდესაც მოდული უნდა გაითიშოს runtime-ზე.
 *          მოდული უნდა შეწყვიტოს მუშაობა და განათავისუფლოს ყველა რესურსი.
 */
typedef struct
{
    char module_name[32]; /**< გასათიში მოდულის ინსტანციის სახელი. */
} module_disable_payload_t;

/**
 * @brief Cleanup ფუნქცია module_disable_payload_t-სთვის.
 */
static inline void free_module_disable_payload(void *data)
{
    if (data)
    {
        free(data);
    }
}

/**
 * @struct module_enable_payload_t
 * @brief განსაზღვრავს მონაცემთა სტრუქტურას `FRAMEWORK_EVENT_MODULE_ENABLE_REQUESTED` ივენთისთვის.
 *
 * @details ეს payload-ი იგზავნება, როდესაც მოდული უნდა ჩაირთოს runtime-ზე.
 *          მოდული უნდა ხელახლა ინიციალიზდეს და დაიწყოს მუშაობა.
 */
typedef struct
{
    char module_name[32]; /**< ჩასართი მოდულის ინსტანციის სახელი. */
} module_enable_payload_t;

/**
 * @brief Cleanup ფუნქცია module_enable_payload_t-სთვის.
 */
static inline void free_module_enable_payload(void *data)
{
    if (data)
    {
        free(data);
    }
}

#endif // TELEMETRY_EVENTS_H