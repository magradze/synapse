/**
 * @file config_manager.h
 * @brief კონფიგურაციის მენეჯერის API.
 * @version 2.0
 * @date 2025-06-25
 * @author Giorgi Magradze
 * @details ეს კომპონენტი პასუხისმგებელია `system_config.json`-ის ჩატვირთვაზე,
 *          ვალიდაციასა და მასზე წვდომის უზრუნველყოფაზე. ის უზრუნველყოფს
 *          ერთიან "სიმართლის წყაროს" (Single Source of Truth) მთელი სისტემისთვის
 *          და მართავს კონფიგურაციის შენახვას NVS-ში.
 */
#ifndef SYNAPSE_CONFIG_MANAGER_H
#define SYNAPSE_CONFIG_MANAGER_H

#include "esp_err.h"
#include "cJSON.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <stdbool.h>

/**
 * @brief ახდენს კონფიგურაციის მენეჯერის ინიციალიზაციას.
 *
 * @details ეს ფუნქცია კითხულობს კონფიგურაციას NVS-დან. თუ NVS-ში კონფიგურაცია არ არსებობს,
 *          ის ტვირთავს `system_config.json`-ის default ვერსიას ფაილური სისტემიდან.
 *          ეს ფუნქცია უნდა გამოიძახოს `system_manager`-მა ინიციალიზაციისას.
 *
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ კონფიგურაცია წარმატებით ჩაიტვირთა.
 * @retval ESP_FAIL თუ ჩატვირთვა ვერ მოხერხდა.
 */
esp_err_t synapse_config_manager_init(void);

/**
 * @brief ინახავს მიმდინარე კონფიგურაციას NVS მეხსიერებაში.
 *
 * @details ეს ფუნქცია უნდა გამოიძახოთ მას შემდეგ, რაც კონფიგურაციაში ცვლილებები შევა,
 *          რათა ისინი შენარჩუნდეს მოწყობილობის გადატვირთვის შემდეგაც.
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 */
esp_err_t synapse_config_save(void);

/**
 * @brief აბრუნებს მოდულის კონფიგურაციის cJSON ობიექტს.
 *
 * @param[in] module_name მოდულის უნიკალური სახელი (instance_name).
 * @return const cJSON* მოდულის კონფიგურაციის მაჩვენებელი (მხოლოდ წასაკითხად).
 * @retval NULL თუ მითითებული სახელით მოდული ვერ მოიძებნა.
 */
const cJSON *synapse_config_get_module_config(const char *module_name);

/**
 * @brief აბრუნებს გლობალური კონფიგურაციის cJSON ობიექტს.
 * @return const cJSON* გლობალური კონფიგურაციის მაჩვენებელი (მხოლოდ წასაკითხად).
 */
const cJSON *synapse_config_get_global_config(void);

/**
 * @brief აბრუნებს მთლიანი კონფიგურაციის cJSON ობიექტს.
 * @note გამოიყენეთ სიფრთხილით. სასურველია გამოიყენოთ უფრო სპეციფიური get ფუნქციები.
 * @return const cJSON* მთლიანი კონფიგურაციის root ობიექტის მაჩვენებელი.
 */
const cJSON *synapse_config_get_root(void);

// --- დამხმარე (Helper) ფუნქციები კონკრეტული ტიპების წასაკითხად ---

/**
 * @brief კითხულობს სტრიქონის ტიპის პარამეტრს მოდულის კონფიგურაციიდან.
 *
 * @param[in] module_name მოდულის სახელი.
 * @param[in] key პარამეტრის გასაღები.
 * @param[out] buffer ბუფერი, სადაც ჩაიწერება მნიშვნელობა.
 * @param[in] buffer_size ბუფერის ზომა.
 * @return esp_err_t
 * @retval ESP_OK თუ პარამეტრი წარმატებით იქნა წაკითხული.
 * @retval ESP_ERR_NOT_FOUND თუ მოდული ან გასაღები ვერ მოიძებნა.
 */
esp_err_t synapse_config_get_module_string(const char *module_name, const char *key, char *buffer, size_t buffer_size);

/**
 * @brief კითხულობს მთელი რიცხვის ტიპის პარამეტრს მოდულის კონფიგურაციიდან.
 *
 * @param[in] module_name მოდულის სახელი.
 * @param[in] key პარამეტრის გასაღები.
 * @param[out] out_value მაჩვენებელი, სადაც ჩაიწერება მნიშვნელობა.
 * @return esp_err_t
 * @retval ESP_OK თუ პარამეტრი წარმატებით იქნა წაკითხული.
 */
esp_err_t synapse_config_get_module_int(const char *module_name, const char *key, int *out_value);

/**
 * @brief კითხულობს ლოგიკური ტიპის პარამეტრს მოდულის კონფიგურაციიდან.
 *
 * @param[in] module_name მოდულის სახელი.
 * @param[in] key პარამეტრის გასაღები.
 * @param[out] out_value მაჩვენებელი, სადაც ჩაიწერება მნიშვნელობა.
 * @return esp_err_t
 * @retval ESP_OK თუ პარამეტრი წარმატებით იქნა წაკითხული.
 */
esp_err_t synapse_config_get_module_bool(const char *module_name, const char *key, bool *out_value);

#endif // SYNAPSE_CONFIG_MANAGER_H