/**
 * @file module_registry.h
 * @brief მოდულების რეგისტრაციისა და მოძიების API.
 * @version 2.0
 * @date 2025-06-26
 * @author Giorgi Magradze
 * @details ეს კომპონენტი პასუხისმგებელია სისტემის ყველა მოდულის აღრიცხვასა და
 *          მათზე წვდომის უზრუნველყოფაზე. ის იყენებს Module Factory-ს, რათა
 *          `system_config.json`-ის მიხედვით შექმნას და დაარეგისტრიროს
 *          ყველა ჩართული მოდული.
 */
#ifndef FMW_MODULE_REGISTRY_H
#define FMW_MODULE_REGISTRY_H

#include "esp_err.h"
#include "base_module.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/**
 * @brief ინიციალიზებს მოდულების რეესტრს.
 * @details ეს ფუნქცია უნდა გამოიძახოს System Manager-მა ერთხელ, სისტემის
 *          ინიციალიზაციისას. ის კითხულობს კონფიგურაციას, ქმნის ყველა მოდულს
 *          Module Factory-ის მეშვეობით და არეგისტრირებს მათ შიდა სიაში.
 * @return esp_err_t ოპერაციის სტატუსის კოდი.
 * @retval ESP_OK თუ რეესტრი წარმატებით ინიციალიზდა.
 * @retval ESP_FAIL თუ რომელიმე მოდულის შექმნა ან რეგისტრაცია ვერ მოხერხდა.
 */
esp_err_t fmw_module_registry_init(void);

/**
 * @brief აბრუნებს ყველა დარეგისტრირებულ მოდულს.
 *
 * @details ეს ფუნქცია გამოიყენება System Manager-ის მიერ, რათა მიიღოს
 *          მოდულების სრული სია და მართოს მათი სიცოცხლის ციკლი (init, start).
 *
 * @note დაბრუნებული მასივი და მისი შიგთავსი არის მხოლოდ წასაკითხად (read-only).
 *       გამომძახებელმა არ უნდა გაათავისუფლოს მეხსიერება.
 *
 * @param[out] modules მაჩვენებელი, რომელშიც ჩაიწერება `const module_t*`-ების მასივის მაჩვენებელი.
 * @param[out] count მაჩვენებელი, რომელშიც ჩაიწერება დარეგისტრირებული მოდულების რაოდენობა.
 *
 * @return esp_err_t
 * @retval ESP_OK წარმატების შემთხვევაში.
 * @retval ESP_ERR_INVALID_ARG თუ `modules` ან `count` არის NULL.
 */
esp_err_t fmw_module_registry_get_all(const module_t ***modules, uint8_t *count);

/**
 * @brief პოულობს დარეგისტრირებულ მოდულს უნიკალური instance სახელით.
 *
 * @details ეს ფუნქცია გამოიყენება System Manager-ის მიერ, რათა იპოვოს კონკრეტული
 *          მოდული და გამოიძახოს მისი runtime მართვის ფუნქციები (`enable`, `disable` და ა.შ.).
 *
 * @param[in] name მოსაძებნი მოდულის instance სახელი (მაგ: "main_broker").
 * @return module_t* ნაპოვნი მოდულის მაჩვენებელი, ან NULL თუ ვერ მოიძებნა.
 * @warning დაბრუნებული მაჩვენებლის შეცვლის უფლება აქვს მხოლოდ System Manager-ს.
 */
module_t *fmw_module_registry_find_by_name(const char *name);

#endif // FMW_MODULE_REGISTRY_H