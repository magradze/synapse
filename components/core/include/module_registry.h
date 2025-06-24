/**
 * @file module_registry.h
 * @brief მოდულების რეგისტრაციისა და მართვის API
 * @version 1.0
 * @date 2025-06-24
 * @author Giorgi Magradze
 * @details ეს კომპონენტი პასუხისმგებელია სისტემის ყველა მოდულის აღრიცხვასა და
 *          მათზე წვდომის უზრუნველყოფაზე. ინახავს მოდულების ცენტრალიზებულ
 *          რეესტრს და უზრუნველყოფს ფუნქციებს მათი მოძიების, ინიციალიზაციისა და
 *          runtime მართვისთვის.
 */

#ifndef MODULE_REGISTRY_H
#define MODULE_REGISTRY_H

#include "esp_err.h"
#include "base_module.h"
#include "cJSON.h"

/**
 * @brief მოდულის შემქმნელი (factory) ფუნქციის ტიპი
 * @details ყველა მოდულმა უნდა უზრუნველყოს ამ ტიპის ფუნქცია, რომელიც პასუხისმგებელია
 *          მოდულის ობიექტის შექმნაზე და მისთვის საჭირო მეხსიერების გამოყოფაზე.
 * @param[in] config მაჩვენებელი cJSON ობიექტზე, რომელიც შეიცავს ამ მოდულის
 *                   კონფიგურაციას `system_config.json`-დან.
 * @return module_t* წარმატების შემთხვევაში აბრუნებს ახლად შექმნილი მოდულის
 *                   მაჩვენებელს, შეცდომის შემთხვევაში კი NULL-ს.
 */
typedef module_t *(*module_create_fn_t)(const cJSON *config);

/**
 * @brief აბრუნებს მოდულის factory ფუნქციას მისი ტიპის მიხედვით
 * @details ეს ფუნქცია წარმოადგენს ავტომატურად გენერირებული კოდის ნაწილს,
 *          რომელიც ბილდის დროს იქმნება. იღებს მოდულის ტიპს და აბრუნებს
 *          შესაბამისი factory ფუნქციის მისამართს.
 * @param[in] module_type მოსაძებნი მოდულის ტიპი (მაგ: "wifi_module")
 * @return module_create_fn_t ფუნქციის მაჩვენებელი ან NULL თუ ვერ მოიძებნა
 */
module_create_fn_t fmw_module_factory_get(const char *module_type);

/**
 * @brief ინიციალიზებს მოდულების რეესტრს
 * @details ეს ფუნქცია უნდა გამოიძახოს system_manager-მა ერთხელ გაშვებისას.
 *          პასუხისმგებელია ყველა მოდულის შექმნაზე და რეგისტრაციაზე,
 *          რომლებიც აღწერილია `system_config.json`-ში.
 * @return esp_err_t ოპერაციის სტატუსის კოდი
 */
esp_err_t fmw_module_registry_init(void);

/**
 * @brief აბრუნებს ყველა დარეგისტრირებულ მოდულს
 * @param[out] modules მაჩვენებელი, რომელშიც ჩაიწერება მოდულების მასივის მაჩვენებელი (`module_t**`)
 * @param[out] count მაჩვენებელი, რომელშიც ჩაიწერება დარეგისტრირებული მოდულების რაოდენობა
 */
void fmw_module_registry_get_all(module_t ***modules, uint8_t *count);

/**
 * @brief პოულობს დარეგისტრირებულ მოდულს მისი ტიპის მიხედვით
 * @param[in] type მოდულის ტიპის სტრიქონი (მაგ: "mqtt_module", "dht22_module"). უნდა ემთხვეოდეს `module.json`-ში განსაზღვრულ `type`-ს
 * @return module_t* წარმატების შემთხვევაში აბრუნებს მოძებნილი მოდულის მაჩვენებელს, წინააღმდეგ შემთხვევაში NULL-ს
 */
module_t *fmw_module_registry_find_by_type(const char *type);

/**
 * @brief პოულობს დარეგისტრირებულ მოდულს უნიკალური instance სახელით
 * @param[in] name მოსაძებნი მოდულის instance სახელი (მაგ: "main_broker", "indoor_dht22")
 * @return module_t* ნაპოვნი მოდულის მაჩვენებელი ან NULL თუ ვერ მოიძებნა
 */
module_t *fmw_module_registry_find_by_name(const char *name);

// --- Runtime მართვის ფუნქციები ---

/**
 * @brief ჩართავს მოდულს runtime-ზე
 * @param[in] module_name ჩასართავი მოდულის instance სახელი
 * @return esp_err_t ოპერაციის სტატუსის კოდი
 */
esp_err_t fmw_module_registry_enable_module(const char *module_name);

/**
 * @brief გამორთავს მოდულს runtime-ზე
 * @param[in] module_name გამოსართავი მოდულის instance სახელი
 * @return esp_err_t ოპერაციის სტატუსის კოდი
 */
esp_err_t fmw_module_registry_disable_module(const char *module_name);

/**
 * @brief რეკონფიგურაციას უკეთებს მოდულს runtime-ზე
 * @param[in] module_name რეკონფიგურირებადი მოდულის instance სახელი
 * @param[in] new_config მაჩვენებელი ახალი კონფიგურაციის cJSON ობიექტზე
 * @return esp_err_t ოპერაციის სტატუსის კოდი
 */
esp_err_t fmw_module_registry_reconfigure_module(const char *module_name, const cJSON *new_config);

/**
 * @brief აბრუნებს მოდულის მიმდინარე სტატუსს
 * @param[in] module_name მოდულის instance სახელი
 * @return module_status_t მოდულის სტატუსი (MODULE_STATUS_ENABLED და სხვ.)
 */
module_status_t fmw_module_registry_get_status(const char *module_name);

/**
 * @brief აბრუნებს ყველა მოდულს, რომელიც უჭერს runtime ოპერაციებს
 * @param[out] modules მაჩვენებელი, რომელშიც ჩაიწერება runtime მოდულების მასივის მაჩვენებელი
 * @param[out] count მაჩვენებელი, რომელშიც ჩაიწერება runtime მოდულების რაოდენობა
 * @return esp_err_t ოპერაციის სტატუსის კოდი
 */
esp_err_t fmw_module_registry_get_runtime_all(module_t ***modules, uint8_t *count);

/**
 * @brief აბრუნებს მოდულის კონფიგურაციას cJSON ობიექტის სახით
 * @param[in] module_name მოდულის instance სახელი
 * @return cJSON* მოდულის კონფიგურაციის მაჩვენებელი ან NULL თუ ვერ მოიძებნა ან არ აქვს კონფიგურაცია
 */
cJSON *fmw_module_registry_get_config(const char *module_name);

#endif // MODULE_REGISTRY_H