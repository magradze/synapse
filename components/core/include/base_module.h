/**
 * @file base_module.h
 * @brief საბაზისო მოდულის ინტერფეისის და სტატუსის ტიპების აღწერა.
 * @version 2.0
 * @date 2025-06-26
 * @author Giorgi Magradze
 * @details ეს ფაილი განსაზღვრავს მოდულის სტატუსის ენუმერაციას, საბაზისო ოპერაციების function pointer-ებს,
 *          საბაზისო მოდულის ინტერფეისის სტრუქტურას და მოდულის მთავარ სტრუქტურას. უზრუნველყოფს მოდულების
 *          ერთიან კონტრაქტს სისტემაში რეგისტრაციის, ინიციალიზაციის, გაშვების, გაჩერების, ივენთების დამუშავების,
 *          enable/disable/reconfigure და სტატუსის მიღებისათვის.
 */
#ifndef BASE_MODULE_H
#define BASE_MODULE_H

#include <sdkconfig.h>
#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"
#include "cJSON.h"
#include "system_event_ids.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// Forward declaration for event_data_wrapper_t to break circular dependency
struct event_data_wrapper_t;

/**
 * @enum module_status_t
 * @brief მოდულის შესაძლო სტატუსების ენუმერაცია.
 * @details განსაზღვრავს მოდულის lifecycle-ის ყველა შესაძლო მდგომარეობას,
 *          რომელიც გამოიყენება მოდულის მონიტორინგისა და მართვისთვის.
 */
typedef enum
{
    MODULE_STATUS_UNINITIALIZED = 0, /**< @brief მოდული არ არის ინიციალიზებული. */
    MODULE_STATUS_INITIALIZED,       /**< @brief მოდული ინიციალიზებულია, მაგრამ არ მუშაობს. */
    MODULE_STATUS_RUNNING,           /**< @brief მოდული მუშაობს და მზადაა ოპერაციებისთვის. */
    MODULE_STATUS_DISABLED,          /**< @brief მოდული გამორთულია და არ ასრულებს ოპერაციებს. */
    MODULE_STATUS_ERROR,             /**< @brief მოდული შეცდომის მდგომარეობაშია. */
    MODULE_STATUS_UNKNOWN            /**< @brief მოდულის სტატუსი უცნობია ან არ არის დადგენილი. */
} module_status_t;

// --- მოდულის ძირითადი ფუნქციების ტიპები ---

/**
 * @brief წინასწარი დეკლარაცია module_t სტრუქტურისთვის.
 */
typedef struct module_t module_t;

/**
 * @brief მოდულის ინიციალიზაციის ფუნქციის ტიპი.
 * @param[in] self მაჩვენებელი მოდულის ინსტანციაზე.
 * @return ოპერაციის წარმატების კოდი.
 * @retval ESP_OK ინიციალიზაცია წარმატებით დასრულდა.
 * @retval ESP_ERR_INVALID_ARG მოდულის მაჩვენებელი არავალიდურია.
 * @retval ESP_ERR_INVALID_STATE მოდული უკვე ინიციალიზებულია.
 */
typedef esp_err_t (*module_init_fn)(module_t *self);

/**
 * @brief მოდულის გაშვების ფუნქციის ტიპი.
 * @param[in] self მაჩვენებელი მოდულის ინსტანციაზე.
 * @return ოპერაციის წარმატების კოდი.
 * @retval ESP_OK მოდული წარმატებით გაშვდა.
 * @retval ESP_ERR_INVALID_ARG მოდულის მაჩვენებელი არავალიდურია.
 * @retval ESP_ERR_INVALID_STATE მოდული არ არის ინიციალიზებული.
 */
typedef esp_err_t (*module_start_fn)(module_t *self);

/**
 * @brief მოდულის runtime ჩართვის ფუნქციის ტიპი.
 * @param[in] self მაჩვენებელი მოდულის ინსტანციაზე.
 * @return ოპერაციის წარმატების კოდი.
 * @retval ESP_OK მოდული წარმატებით ჩაირთო.
 * @retval ESP_ERR_INVALID_ARG მოდულის მაჩვენებელი არავალიდურია.
 * @retval ESP_ERR_INVALID_STATE მოდული არ არის მზად ჩასართავად.
 */
typedef esp_err_t (*module_enable_fn)(module_t *self);

/**
 * @brief მოდულის runtime გამორთვის ფუნქციის ტიპი.
 * @param[in] self მაჩვენებელი მოდულის ინსტანციაზე.
 * @return ოპერაციის წარმატების კოდი.
 * @retval ESP_OK მოდული წარმატებით გამოირთო.
 * @retval ESP_ERR_INVALID_ARG მოდულის მაჩვენებელი არავალიდურია.
 */
typedef esp_err_t (*module_disable_fn)(module_t *self);

/**
 * @brief მოდულის დეინიციალიზაციის (გასუფთავების) ფუნქციის ტიპი. ★★★ (ახალი) ★★★
 * @param[in] self მაჩვენებელი გასასუფთავებელ მოდულის ინსტანციაზე.
 */
typedef void (*module_deinit_fn)(module_t *self);

/**
 * @brief მოდულის კონფიგურაციის რეალ დროში შეცვლის ფუნქციის ტიპი.
 * @param[in] self მაჩვენებელი მოდულის ინსტანციაზე.
 * @param[in] new_config ახალი კონფიგურაციის cJSON ობიექტი.
 * @return ოპერაციის წარმატების კოდი.
 * @retval ESP_OK კონფიგურაცია წარმატებით შეიცვალა.
 * @retval ESP_ERR_INVALID_ARG რომელიმე პარამეტრი არავალიდურია.
 * @retval ESP_ERR_NOT_SUPPORTED მოდული არ უჭერს კონფიგურაციის შეცვლას.
 */
typedef esp_err_t (*module_reconfigure_fn)(module_t *self, const cJSON *new_config);

/**
 * @brief მოდულის სტატუსის მიღების ფუნქციის ტიპი.
 * @param[in] self მაჩვენებელი მოდულის ინსტანციაზე.
 * @return მოდულის მიმდინარე სტატუსი.
 */
typedef module_status_t (*module_get_status_fn)(module_t *self);

/**
 * @brief მოდულის ივენთების დამუშავების ფუნქციის ტიპი. ★★★ (შესწორებულია) ★★★
 * @details ეს ფუნქცია არ აბრუნებს მნიშვნელობას, რადგან ის ასინქრონულად
 *          გამოიძახება Event Bus-ის ტასკიდან.
 * @param[in] self მაჩვენებელი მოდულის ინსტანციაზე.
 * @param[in] event_name ივენთის სახელი, რომელზეც მოხდა რეაგირება.
 * @param[in] data მაჩვენებელი ივენთის მონაცემების "გარსზე" (event_data_wrapper_t*).
 *                 მნიშვნელოვანია: ამ მონაცემების გამოყენების შემდეგ აუცილებელია
 *                 fmw_event_data_release(data)-ის გამოძახება.
 */
typedef void (*module_event_handler_fn)(module_t *self, const char *event_name, void *data);

/**
 * @struct module_t
 * @brief მოდულის ძირითადი სტრუქტურა, რომელიც განსაზღვრავს ფრეიმვორქის მოდულის კონტრაქტს.
 * @details ეს სტრუქტურა შეიცავს მოდულის ყველა აუცილებელ ინფორმაციას და ფუნქციონალურს:
 *          სახელს, სტატუსს, კონფიგურაციას, API ფუნქციებს და შიდა მონაცემებს.
 *          ყველა მოდული უნდა აღებულ იქნას ამ სტრუქტურის საფუძველზე.
 */
struct module_t
{
    char name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH]; /**< @brief მოდულის უნიკალური სახელი (instance_name). */
    uint8_t init_level;                           /**< @brief მოდულის ლეველი */
    module_status_t status;                       /**< @brief მოდულის მიმდინარე სტატუსი. */
    cJSON *current_config;                        /**< @brief მაჩვენებელი მოდულის მიმდინარე კონფიგურაციაზე. */
    SemaphoreHandle_t state_mutex;                /**< @brief mutex მოდულის მდგომარეობის დაცვისთვის. */

    /**
     * @brief მოდულის ძირითადი API ფუნქციების კოლექცია.
     * @details ყველა მოდული უნდა იმპლემენტირებდეს ამ ფუნქციებს ფრეიმვორქის
     *          სტანდარტული lifecycle-ის უზრუნველსაყოფად.
     */
    struct
    {
        module_init_fn init;                 /**< @brief მოდულის ინიციალიზაციის ფუნქცია. */
        module_start_fn start;               /**< @brief მოდულის გაშვების ფუნქცია. */
        module_deinit_fn deinit;             /**< @brief მოდულის გასუფთავების (დეინიციალიზაციის) ფუნქცია. */
        module_enable_fn enable;             /**< @brief მოდულის runtime ჩართვის ფუნქცია. */
        module_disable_fn disable;           /**< @brief მოდულის runtime გამორთვის ფუნქცია. */
        module_reconfigure_fn reconfigure;   /**< @brief მოდულის კონფიგურაციის შეცვლის ფუნქცია. */
        module_get_status_fn get_status;     /**< @brief მოდულის სტატუსის მიღების ფუნქცია. */
        module_event_handler_fn handle_event; /**< @brief მოდულის ივენთების დამუშავების ფუნქცია. */
    } base;

    void *private_data; /**< @brief მაჩვენებელი მოდულის შიდა მონაცემებზე. */
};

// --- დამხმარე მაკროები ---

/**
 * @brief ამოწმებს, უჭერს თუ არა მოდული runtime ოპერაციებს.
 * @details ამოწმებს არის თუ არა განსაზღვრული enable და disable ფუნქციები.
 *          Runtime ოპერაციები საშუალებას იძლევა მოდული ჩაირთოს/გამოირთოს
 *          სისტემის მუშაობის დროს რესტარტის გარეშე.
 * @param module მოდულის ინსტანციის მაჩვენებელი.
 * @return true თუ runtime ფუნქციები არსებობს, false წინააღმდეგ შემთხვევაში.
 * @note მაკრო ასევე ამოწმებს მოდულის მაჩვენებელს NULL-ზე.
 */
#define MODULE_SUPPORTS_RUNTIME(module) ((module) != NULL && (module)->base.enable && (module)->base.disable)

/**
 * @brief ამოწმებს, მუშაობს თუ არა მოდული ამჟამად.
 * @details ამოწმებს არის თუ არა მოდულის სტატუსი MODULE_STATUS_RUNNING.
 *          მუშაობდა მოდული ნიშნავს რომ ის მზადაა ოპერაციების შესასრულებლად.
 * @param module მოდულის ინსტანციის მაჩვენებელი.
 * @return true თუ მუშაობს, false წინააღმდეგ შემთხვევაში.
 * @note მაკრო ასევე ამოწმებს მოდულის მაჩვენებელს NULL-ზე.
 */
#define MODULE_IS_RUNNING(module) ((module) != NULL && (module)->status == MODULE_STATUS_RUNNING)

/**
 * @brief ამოწმებს, არის თუ არა მოდული ოპერაციული მდგომარეობაში.
 * @details ამოწმებს რომ მოდული არ არის შეცდომის (ERROR) მდგომარეობაში.
 *          ოპერაციული მოდული შეიძლება იყოს ინიციალიზებული ან მუშაობდეს,
 *          მაგრამ არ უნდა იყოს გაფუჭებული.
 * @param module მოდულის ინსტანციის მაჩვენებელი.
 * @return true თუ სტატუსი გამართულია, false წინააღმდეგ შემთხვევაში.
 * @note მაკრო ასევე ამოწმებს მოდულის მაჩვენებელს NULL-ზე.
 */
#define MODULE_IS_OPERATIONAL(module) ((module) != NULL && (module)->status != MODULE_STATUS_ERROR)

#endif // BASE_MODULE_H