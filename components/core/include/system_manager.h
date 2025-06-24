/**
 * @file system_manager.h
 * @brief სისტემის მენეჯერის Public API.
 * @details ეს ფაილი განსაზღვრავს ფრეიმვორქის ინიციალიზაციის, გაშვების და
 *          runtime მართვის მთავარ ფუნქციებს. System Manager მოქმედებს როგორც
 *          "დირიჟორი", რომელიც ორკესტრირებას უწევს ბირთვის კომპონენტებისა და
 *          მოდულების მუშაობას.
 * @author Giorgi Magradze
 * @version 1.0
 * @date 2025-06-24
 */
#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "esp_err.h"
#include "base_module.h"
#include "cJSON.h"

/**
 * @brief სისტემის ბირთვის ინიციალიზაცია.
 * @details ახდენს ყველა ძირითადი სერვისის (Config Manager, Event Bus, Service Locator,
 *          Resource Manager) და Module Registry-ის ინიციალიზაციას. ეს ფუნქცია უნდა
 *          გამოიძახოს `app_main`-მა ერთხელ, სისტემის გაშვებამდე.
 * @return ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ ბირთვის ყველა კომპონენტი წარმატებით ინიციალიზდა.
 * @retval ESP_FAIL თუ რომელიმე კრიტიკული კომპონენტის ინიციალიზაცია ვერ მოხერხდა.
 */
esp_err_t system_init(void);

/**
 * @brief სისტემის და ყველა დარეგისტრირებული მოდულის გაშვება.
 * @details ეს ფუნქცია იღებს მოდულების მზა, დახარისხებულ სიას Module Registry-დან
 *          და შემდეგ თანმიმდევრულად უშვებს თითოეული მოდულის `init()` და `start()`
 *          ფუნქციებს. `system_init()` უნდა იყოს გამოძახებული ამ ფუნქციამდე.
 * @return ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ ყველა მოდული წარმატებით გაეშვა.
 */
esp_err_t system_start(void);

/**
 * @brief ჩართავს მოდულს runtime-ზე.
 * @details იძახებს შესაბამისი მოდულის `enable()` ფუნქციას, თუ ის განსაზღვრულია.
 *          ოპერაცია დელეგირდება Module Registry-ზე.
 * @param[in] module_name სამიზნე მოდულის უნიკალური სახელი (instance_name).
 * @return ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ მოდული წარმატებით ჩაირთო.
 * @retval ESP_ERR_INVALID_ARG თუ `module_name` არის NULL.
 * @retval ESP_ERR_NOT_FOUND თუ მითითებული სახელით მოდული ვერ მოიძებნა.
 */
esp_err_t system_enable_module(const char *module_name);

/**
 * @brief გამორთავს მოდულს runtime-ზე.
 * @details იძახებს შესაბამისი მოდულის `disable()` ფუნქციას, თუ ის განსაზღვრულია.
 *          ოპერაცია დელეგირდება Module Registry-ზე.
 * @param[in] module_name სამიზნე მოდულის უნიკალური სახელი (instance_name).
 * @return ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ მოდული წარმატებით გამოირთო.
 * @retval ESP_ERR_INVALID_ARG თუ `module_name` არის NULL.
 * @retval ESP_ERR_NOT_FOUND თუ მითითებული სახელით მოდული ვერ მოიძებნა.
 */
esp_err_t system_disable_module(const char *module_name);

/**
 * @brief ხელახლა აკონფიგურირებს მოდულს runtime-ზე.
 * @details იძახებს შესაბამისი მოდულის `reconfigure()` ფუნქციას, თუ ის განსაზღვრულია.
 * @param[in] module_name სამიზნე მოდულის უნიკალური სახელი (instance_name).
 * @param[in] new_config ახალი კონფიგურაციის cJSON ობიექტი.
 * @return ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ მოდული წარმატებით რეკონფიგურირდა.
 * @retval ESP_ERR_INVALID_ARG თუ `module_name` ან `new_config` არის NULL.
 * @retval ESP_ERR_NOT_FOUND თუ მითითებული სახელით მოდული ვერ მოიძებნა.
 */
esp_err_t system_reconfigure_module(const char *module_name, const cJSON *new_config);

/**
 * @brief აბრუნებს მოდულის სტატუსს.
 * @param[in] module_name სამიზნე მოდულის უნიკალური სახელი (instance_name).
 * @return მოდულის მიმდინარე სტატუსი (`module_status_t`).
 * @retval MODULE_STATUS_ERROR თუ მოდული ვერ მოიძებნა.
 */
module_status_t system_get_module_status(const char *module_name);

/**
 * @brief აბრუნებს ყველა runtime ჩართული მოდულის სიას.
 * @note დაბრუნებული მასივი (`modules`) არ უნდა გათავისუფლდეს გამომძახებლის მიერ.
 * @param[out] modules მაჩვენებელი, რომელშიც ჩაიწერება მოდულების მასივის მაჩვენებელი.
 * @param[out] count მაჩვენებელი, რომელშიც ჩაიწერება მოდულების რაოდენობა.
 * @return ოპერაციის წარმატების კოდი.
 * @retval ESP_OK წარმატების შემთხვევაში.
 */
esp_err_t system_get_runtime_modules(module_t ***modules, uint8_t *count);

/**
 * @brief აბრუნებს მოდულის კონფიგურაციას.
 * @param[in] module_name სამიზნე მოდულის უნიკალური სახელი (instance_name).
 * @return cJSON ობიექტის მაჩვენებელი, ან NULL თუ მოდული ვერ მოიძებნა.
 * @warning დაბრუნებული cJSON ობიექტი არ უნდა გათავისუფლდეს გამომძახებლის მიერ.
 */
cJSON *system_get_module_config(const char *module_name);

/**
 * @brief აბრუნებს სისტემის ზოგად ინფორმაციას JSON ფორმატში.
 * @deprecated ეს ფუნქცია მოძველებულია და გადატანილია `version_module`-ში.
 *             გამოიყენეთ `fmw_version_get_system_info()` ნაცვლად.
 * @return cJSON ობიექტის მაჩვენებელი, ან NULL შეცდომისას.
 */
cJSON *system_get_system_info(void);

#endif // SYSTEM_MANAGER_H