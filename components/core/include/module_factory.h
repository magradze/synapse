/**
 * @file module_factory.h
 * @brief მოდულების შემქმნელი (Factory) კომპონენტის Public API.
 * @version 1.0
 * @date 2025-06-25
 * @author Giorgi Magradze
 * @details ეს კომპონენტი პასუხისმგებელია მოდულების დინამიურ შექმნაზე მათი
 *          ტიპის მიხედვით. ის იყენებს ავტომატურად გენერირებულ კოდს, რათა
 *          დააკავშიროს მოდულის ტიპი მის შემქმნელ ფუნქციასთან.
 */
#ifndef FMW_MODULE_FACTORY_H
#define FMW_MODULE_FACTORY_H

#include "base_module.h"
#include "cJSON.h"

// Forward declaration of the factory function type that's defined in generated_module_factory.h
typedef module_t *(*module_create_fn_t)(const cJSON *config);

/**
 * @brief მოდულის შემქმნელი (factory) ფუნქციის ტიპი.
 * @details ყველა მოდულმა უნდა უზრუნველყოს ამ ტიპის ფუნქცია, რომელიც პასუხისმგებელია
 *          მოდულის ობიექტის შექმნაზე და მისთვის საჭირო მეხსიერების გამოყოფაზე.
 * @param[in] config მაჩვენებელი cJSON ობიექტზე, რომელიც შეიცავს ამ მოდულის
 *                   კონფიგურაციას `system_config.json`-დან.
 * @return module_t* წარმატების შემთხვევაში აბრუნებს ახლად შექმნილი მოდულის
 *                   მაჩვენებელს, შეცდომის შემთხვევაში კი NULL-ს.
 * @note This typedef is defined in generated_module_factory.h
 */

/**
 * @brief ქმნის მოდულის ახალ ინსტანციას მისი ტიპისა და კონფიგურაციის მიხედვით.
 * @details ეს ფუნქცია იღებს მოდულის ტიპს, პოულობს შესაბამის შემქმნელ ფუნქციას
 *          და იძახებს მას კონფიგურაციის გადაცემით.
 * @param[in] module_type მოსაძებნი მოდულის ტიპი (მაგ: "wifi_module").
 * @param[in] config მაჩვენებელი cJSON ობიექტზე, რომელიც გადაეცემა მოდულის
 *                   შემქმნელ ფუნქციას.
 * @return module_t* ახლად შექმნილი მოდულის მაჩვენებელი, ან NULL, თუ
 *                   ტიპი ვერ მოიძებნა ან შექმნა ვერ მოხერხდა.
 */
module_t *fmw_module_factory_create(const char *module_type, const cJSON *config);

#endif // FMW_MODULE_FACTORY_H