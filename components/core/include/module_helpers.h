/**
 * @file module_helpers.h
 * @brief მოდულებისთვის განკუთვნილი ზოგადი დამხმარე (Helper) ფუნქციები.
 * @version 1.0
 * @date 2025-06-26
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს მოდულების სიცოცხლის ციკლის სამართავად
 *          განკუთვნილ ზოგად იმპლემენტაციებს. ის მიზნად ისახავს, შეამციროს
 *          კოდის დუბლირება იმ მოდულებში, რომლებსაც აქვთ სტანდარტული
 *          `enable`, `disable` ან `get_status` ლოგიკა.
 */
#ifndef SYNAPSE_MODULE_HELPERS_H
#define SYNAPSE_MODULE_HELPERS_H

#include "base_module.h"

/**
 * @brief ზოგადი `enable` ფუნქციის იმპლემენტაცია.
 * @details ამოწმებს, ხომ არ არის მოდული უკვე ჩართული. თუ არა, ცვლის მის
 *          სტატუსს `MODULE_STATUS_RUNNING`-ზე. ეს ფუნქცია განკუთვნილია
 *          იმ მოდულებისთვის, რომლებსაც არ სჭირდებათ სპეციალური ლოგიკა
 *          ჩართვისას (მაგ. ტასკის განახლება).
 * @param[in] module სამიზნე მოდულის მაჩვენებელი.
 * @return esp_err_t
 * @retval ESP_OK თუ სტატუსი წარმატებით შეიცვალა.
 * @retval ESP_ERR_INVALID_STATE თუ მოდული უკვე ჩართულია.
 * @retval ESP_ERR_INVALID_ARG თუ `module` არის NULL.
 */
esp_err_t synapse_module_helper_simple_enable(struct module_t *module);

/**
 * @brief ზოგადი `disable` ფუნქციის იმპლემენტაცია.
 * @details ამოწმებს, ხომ არ არის მოდული უკვე გამორთული. თუ არა, ცვლის მის
 *          სტატუსს `MODULE_STATUS_DISABLED`-ზე. განკუთვნილია
 *          მოდულებისთვის, რომლებსაც არ სჭირდებათ სპეციალური ლოგიკა გამორთვისას.
 * @param[in] module სამიზნე მოდულის მაჩვენებელი.
 * @return esp_err_t
 * @retval ESP_OK თუ სტატუსი წარმატებით შეიცვალა.
 * @retval ESP_ERR_INVALID_STATE თუ მოდული უკვე გამორთულია.
 * @retval ESP_ERR_INVALID_ARG თუ `module` არის NULL.
 */
esp_err_t synapse_module_helper_simple_disable(struct module_t *module);

/**
 * @brief ზოგადი `get_status` ფუნქციის იმპლემენტაცია.
 * @details უბრალოდ აბრუნებს მოდულის `status` ველის მიმდინარე მნიშვნელობას.
 *          განკუთვნილია მოდულებისთვის, რომლებსაც არ სჭირდებათ სტატუსის
 *          დინამიური გამოთვლა.
 * @param[in] module სამიზნე მოდულის მაჩვენებელი.
 * @return module_status_t მოდულის მიმდინარე სტატუსი.
 */
module_status_t synapse_module_helper_simple_get_status(struct module_t *module);

#endif // SYNAPSE_MODULE_HELPERS_H