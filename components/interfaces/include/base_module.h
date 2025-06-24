/**
 * @file base_module.h
 * @brief საბაზისო მოდულის ინტერფეისის და სტატუსის ტიპების აღწერა
 * @version 1.0
 * @date 2025-06-24
 * @author Giorgi Magradze
 * @details ეს ფაილი განსაზღვრავს მოდულის სტატუსის ენუმერაციას, საბაზისო ოპერაციების function pointer-ებს,
 *          საბაზისო მოდულის ინტერფეისის სტრუქტურას და მოდულის მთავარ სტრუქტურას. უზრუნველყოფს მოდულების
 *          ერთიან კონტრაქტს სისტემაში რეგისტრაციის, ინიციალიზაციის, გაშვების, გაჩერების, ივენთების დამუშავების,
 *          enable/disable/reconfigure და სტატუსის მიღებისათვის.
 */
#ifndef BASE_MODULE_H
#define BASE_MODULE_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "esp_err.h"
#include "cJSON.h"

/**
 * @brief წინასწარი გამოცხადება ციკლური დამოკიდებულებების თავიდან ასაცილებლად
 */
struct module_t;

/**
 * @brief მოდულის სტატუსის ენუმერაცია
 *
 * აღწერს მოდულის შესაძლო მდგომარეობებს მისი სიცოცხლის ციკლის განმავლობაში
 */
typedef enum
{
    MODULE_STATUS_DISABLED = 0, /**< მოდული გამორთულია */
    MODULE_STATUS_ENABLED,      /**< მოდული ჩართულია, მაგრამ არ მუშაობს */
    MODULE_STATUS_INITIALIZED,  /**< მოდული ინიციალიზებულია */
    MODULE_STATUS_RUNNING,      /**< მოდული მუშაობს */
    MODULE_STATUS_STOPPED,      /**< მოდული გაჩერებულია */
    MODULE_STATUS_ERROR,        /**< მოდულში დაფიქსირდა შეცდომა */
    MODULE_STATUS_UNKNOWN       /**< მოდულის სტატუსი უცნობია */
} module_status_t;

/**
 * @brief ფუნქციის მაჩვენებლის ტიპი მოდულის ინიციალიზაციისთვის
 * @param module მოდულის ინსტანციის მაჩვენებელი
 * @return ESP_OK წარმატების შემთხვევაში, შეცდომის კოდი წარუმატებლობისას
 */
typedef esp_err_t (*module_init_t)(struct module_t* module);

/**
 * @brief ფუნქციის მაჩვენებლის ტიპი მოდულის გაშვებისთვის
 * @param module მოდულის ინსტანციის მაჩვენებელი
 * @return ESP_OK წარმატების შემთხვევაში, შეცდომის კოდი წარუმატებლობისას
 */
typedef esp_err_t (*module_start_t)(struct module_t* module);

/**
 * @brief ფუნქციის მაჩვენებლის ტიპი მოდულის გაჩერებისთვის
 * @param module მოდულის ინსტანციის მაჩვენებელი
 * @return ESP_OK წარმატების შემთხვევაში, შეცდომის კოდი წარუმატებლობისას
 */
typedef esp_err_t (*module_stop_t)(struct module_t* module);

/**
 * @brief ფუნქციის მაჩვენებლის ტიპი მოდულის ივენთის დამუშავებისთვის
 * @param module მოდულის ინსტანციის მაჩვენებელი
 * @param event_id ივენთის იდენტიფიკატორი
 * @param event_data ივენთის სპეციფიური მონაცემები
 */
typedef void (*module_event_handler_t)(struct module_t *module, int32_t event_id, void *event_data);

/**
 * @brief ფუნქციის მაჩვენებლის ტიპი მოდულის ჩასართავად
 * @param module მოდულის ინსტანციის მაჩვენებელი
 * @return ESP_OK წარმატების შემთხვევაში, შეცდომის კოდი წარუმატებლობისას
 */
typedef esp_err_t (*module_enable_t)(struct module_t *module);

/**
 * @brief ფუნქციის მაჩვენებლის ტიპი მოდულის გამოსართავად
 * @param module მოდულის ინსტანციის მაჩვენებელი
 * @return ESP_OK წარმატების შემთხვევაში, შეცდომის კოდი წარუმატებლობისას
 */
typedef esp_err_t (*module_disable_t)(struct module_t *module);

/**
 * @brief ფუნქციის მაჩვენებლის ტიპი მოდულის რეკონფიგურაციისთვის
 * @param module მოდულის ინსტანციის მაჩვენებელი
 * @param new_config ახალი კონფიგურაციის JSON ობიექტი
 * @return ESP_OK წარმატების შემთხვევაში, შეცდომის კოდი წარუმატებლობისას
 */
typedef esp_err_t (*module_reconfigure_t)(struct module_t *module, const cJSON *new_config);

/**
 * @brief ფუნქციის მაჩვენებლის ტიპი მოდულის სტატუსის მისაღებად
 * @param module მოდულის ინსტანციის მაჩვენებელი
 * @return მიმდინარე მოდულის სტატუსი
 */
typedef module_status_t (*module_get_status_t)(struct module_t *module);

/**
 * @brief საბაზისო მოდულის ინტერფეისის სტრუქტურა
 *
 * შეიცავს ყველა საბაზისო ოპერაციის ფუნქციის მაჩვენებლებს
 */
typedef struct {
    module_init_t init;                  /**< მოდულის ინიციალიზაციის ფუნქცია */
    module_start_t start;                /**< მოდულის გაშვების ფუნქცია */
    module_stop_t stop;                  /**< მოდულის გაჩერების ფუნქცია */
    module_event_handler_t handle_event; /**< მოდულის ივენთის დამმუშავებელი ფუნქცია */

    module_enable_t enable;           /**< მოდულის ჩართვის ფუნქცია */
    module_disable_t disable;         /**< მოდულის გამორთვის ფუნქცია */
    module_reconfigure_t reconfigure; /**< მოდულის რეკონფიგურაციის ფუნქცია */
    module_get_status_t get_status;   /**< მოდულის სტატუსის წამკითხველი ფუნქცია */
} base_module_interface_t;

/**
 * @brief მთავარი მოდულის სტრუქტურა
 *
 * წარმოადგენს მოდულის ინსტანციას ყველა ასოცირებულ მონაცემთან და ინტერფეისთან ერთად
 */
typedef struct module_t {
    char name[32];                /**< მოდულის სახელი */
    char type[32];                /**< მოდულის ტიპი */
    int8_t init_level;            /**< ინიციალიზაციის დონე */
    void *private_data;           /**< მოდულისთვის სპეციფიური მონაცემები */
    base_module_interface_t base; /**< საბაზისო მოდულის ინტერფეისი */
    void *specific_interface;     /**< მოდულისთვის სპეციფიური ინტერფეისი */

    module_status_t status; /**< მიმდინარე სტატუსი */
    bool runtime_enabled;   /**< უჭერს თუ არა მოდული runtime enable/disable-ს */
    cJSON *current_config;  /**< მიმდინარე კონფიგურაცია */
} module_t;

/**
 * @brief მოდულის შექმნის ფუნქციის მაჩვენებლის ტიპი
 *
 * გამოიყენება მოდულების რეესტრში, რათა ყველა create ფუნქციას ჰქონდეს ერთიანი სიგნატურა
 * @param config მოდულის კონფიგურაციის JSON ობიექტი
 * @return შექმნილი მოდულის მაჩვენებელი, NULL წარუმატებლობისას
 */
typedef module_t *(*module_create_fn_t)(const cJSON *config);

/**
 * @brief ამოწმებს უჭერს თუ არა მოდული runtime ოპერაციებს
 * @param module მოდულის ინსტანციის მაჩვენებელი
 * @return true თუ runtime ჩართულია, false წინააღმდეგ შემთხვევაში
 */
#define MODULE_RUNTIME_ENABLED(module) ((module) && (module)->runtime_enabled)

/**
 * @brief ამოწმებს მუშაობს თუ არა მოდული ამჟამად
 * @param module მოდულის ინსტანციის მაჩვენებელი
 * @return true თუ მუშაობს, false წინააღმდეგ შემთხვევაში
 */
#define MODULE_STATUS_IS_RUNNING(module) ((module) && (module)->status == MODULE_STATUS_RUNNING)

/**
 * @brief ამოწმებს ჩართულია თუ არა მოდული ან უფრო მაღალ სტატუსშია
 * @param module მოდულის ინსტანციის მაჩვენებელი
 * @return true თუ ჩართულია ან უფრო მაღალ სტატუსშია, false წინააღმდეგ შემთხვევაში
 */
#define MODULE_STATUS_IS_ENABLED(module) ((module) && (module)->status >= MODULE_STATUS_ENABLED)

#endif // BASE_MODULE_H