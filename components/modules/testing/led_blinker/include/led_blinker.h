/**
 * @file led_blinker.h
 * @brief GPIO2-ზე LED-ის მარტივი ციმციმის მოდული
 * @author Synapse Framework Team
 * @version 1.0.0
 * @details Led_Blinker მოდული რომელიც აღზევს GPIO2-ზე LED-ის მარტივი ციმციმის მოდული ფუნქციონალურობას.
 *          მოდული მუშაობს Synapse Framework-ის base_module interface-ის საფუძველზე.
 */

#ifndef LED_BLINKER_H
#define LED_BLINKER_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Led_Blinker მოდულის კონფიგურაციის სტრუქტურა
 * @details შეიცავს მოდულის ყველა კონფიგურაციის პარამეტრს
 */
typedef struct {
    char instance_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];  /**< მოდულის ინსტანციის სახელი */
    bool auto_start;                                        /**< ავტომატური გაშვება */
    // TODO: დაამატეთ მოდულის სპეციფიკური კონფიგურაციის ფილდები
} led_blinker_config_t;

/**
 * @brief ქმნის ახალ led_blinker მოდულის ინსტანციას
 * 
 * @param[in] config მოდულის კონფიგურაცია (JSON ობიექტი) ან NULL default-ებისთვის
 * @return მაჩვენებელი შექმნილ მოდულზე ან NULL შეცდომისას
 * 
 * @note შექმნილი მოდული უნდა დარეგისტრირდეს system_manager-ში
 * @warning მეხსიერება უნდა გათავისუფლდეს module->base.destroy() ფუნქციით
 */
module_t *led_blinker_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // LED_BLINKER_H
