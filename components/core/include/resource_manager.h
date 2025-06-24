/**
 * @file resource_manager.h
 * @brief რესურსების (GPIO და სხვა) მართვის API
 * @version 1.0
 * @date 2025-06-24
 * @author Giorgi Magradze
 * @details ეს კომპონენტი უზრუნველყოფს სისტემაში საერთო რესურსების (მაგ: GPIO პინების) უსაფრთხო და ექსკლუზიურ გამოყენებას.
 *          თითოეულ მოდულს შეუძლია "დალოქოს" რესურსი, რათა თავიდან აიცილოს კონფლიქტი სხვა მოდულებთან. რესურსის გათავისუფლება
 *          აუცილებელია მოდულის გაჩერების ან გამორთვისას. ეს ხელს უწყობს სისტემის სტაბილურობას და მოდულებს შორის იზოლაციას.
 */
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "esp_err.h"
#include "driver/gpio.h"

/**
 * @brief რესურს მენეჯერის ინიციალიზაცია
 *
 * ასუფთავებს ყველა დაკავებული რესურსის სიას. უნდა გამოიძახოს System Manager-მა ერთხელ სისტემის გაშვებისას.
 *
 * @return esp_err_t
 *         - ESP_OK: რესურს მენეჯერი წარმატებით ინიციალიზდა
 *         - ESP_ERR_NO_MEM: მეხსიერების გამოყოფა ვერ მოხერხდა
 */
esp_err_t fmw_resource_manager_init(void);

/**
 * @brief GPIO პინის "დალოქვა" (დაკავება) კონკრეტული მოდულისთვის
 *
 * სანამ მოდული გამოიყენებს GPIO პინს, მან უნდა "დალოქოს" იგი, რათა სხვა მოდულმა ვერ გამოიყენოს იგივე პინი.
 *
 * @param[in] pin დასალოქი GPIO პინის ნომერი
 * @param[in] owner მოდულის სახელი, რომელიც იკავებს პინს (დაბგისთვის სასარგებლოა)
 *
 * @return esp_err_t
 *         - ESP_OK: პინი წარმატებით დაილოქა
 *         - ESP_ERR_INVALID_ARG: არასწორი პინის ნომერი
 *         - ESP_ERR_INVALID_STATE: პინი უკვე დაკავებულია სხვა მოდულის მიერ
 *         - ESP_ERR_NO_MEM: მეხსიერების გამოყოფა ვერ მოხერხდა
 */
esp_err_t fmw_resource_lock_gpio(gpio_num_t pin, const char* owner);

/**
 * @brief დაკავებული GPIO პინის გათავისუფლება
 *
 * როდესაც მოდული ასრულებს მუშაობას პინზე (მაგ: stop ფუნქციაში), მან უნდა გაათავისუფლოს რესურსი.
 *
 * @param[in] pin გასათავისუფლებელი GPIO პინის ნომერი
 * @param[in] owner მოდულის სახელი, რომელიც ათავისუფლებს პინს
 *
 * @return esp_err_t
 *         - ESP_OK: პინი წარმატებით გათავისუფლდა
 *         - ESP_ERR_INVALID_ARG: არასწორი პინის ნომერი
 *         - ESP_ERR_NOT_FOUND: პინი დაკავებული არ იყო
 *         - ESP_ERR_INVALID_STATE: გათავისუფლება სცადა არა მფლობელმა მოდულმა
 */
esp_err_t fmw_resource_release_gpio(gpio_num_t pin, const char* owner);

/**
 * @brief ამოწმებს დაკავებულია თუ არა GPIO პინი
 *
 * @param[in] pin შესამოწმებელი GPIO პინის ნომერი
 * @return true თუ პინი დაკავებულია
 * @return false თუ პინი თავისუფალია
 */
bool fmw_resource_is_gpio_locked(gpio_num_t pin);

#endif /* RESOURCE_MANAGER_H */