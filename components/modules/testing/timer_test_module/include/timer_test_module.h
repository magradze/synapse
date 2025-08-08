/**
 * @file timer_test_module.h
 * @brief A module to test the System Timer and RGB LED services.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @details Timer Test Module მოდული რომელიც აღზევს A module to test the System Timer and RGB LED services. ფუნქციონალურობას.
 *          მოდული მუშაობს Synapse Framework-ის base_module interface-ის საფუძველზე.
 */

#ifndef TIMER_TEST_MODULE_H
#define TIMER_TEST_MODULE_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Timer Test Module მოდულის კონფიგურაციის სტრუქტურა
 * @details შეიცავს მოდულის ყველა კონფიგურაციის პარამეტრს
 */
typedef struct {
    char instance_name[CONFIG_SYNAPSE_MODULE_NAME_MAX_LENGTH]; /**< მოდულის ინსტანციის სახელი */
    bool auto_start;                                        /**< ავტომატური გაშვება */
    // TODO: დაამატეთ მოდულის სპეციფიკური კონფიგურაციის ფილდები
} timer_test_module_config_t;

/**
 * @brief ქმნის ახალ timer_test_module მოდულის ინსტანციას
 * 
 * @param[in] config მოდულის კონფიგურაცია (JSON ობიექტი) ან NULL default-ებისთვის
 * @return მაჩვენებელი შექმნილ მოდულზე ან NULL შეცდომისას
 * 
 * @note შექმნილი მოდული უნდა დარეგისტრირდეს system_manager-ში
 * @warning მეხსიერება უნდა გათავისუფლდეს module->base.destroy() ფუნქციით
 */
module_t *timer_test_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // TIMER_TEST_MODULE_H
