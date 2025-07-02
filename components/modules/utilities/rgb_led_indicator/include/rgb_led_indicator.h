/**
 * @file rgb_led_indicator.h
 * @brief System LED indicator
 * @author Synapse Framework Team
 * @version 1.0.0
 * @details Rgb_Led_Indicator მოდული რომელიც აღზევს System LED indicator ფუნქციონალურობას.
 *          მოდული მუშაობს Synapse Framework-ის base_module interface-ის საფუძველზე.
 */

#ifndef RGB_LED_INDICATOR_H
#define RGB_LED_INDICATOR_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ქმნის ახალ rgb_led_indicator მოდულის ინსტანციას
 * 
 * @param[in] config მოდულის კონფიგურაცია (JSON ობიექტი) ან NULL default-ებისთვის
 * @return მაჩვენებელი შექმნილ მოდულზე ან NULL შეცდომისას
 * 
 * @note შექმნილი მოდული უნდა დარეგისტრირდეს system_manager-ში
 * @warning მეხსიერება უნდა გათავისუფლდეს module->base.destroy() ფუნქციით
 */
module_t *rgb_led_indicator_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // RGB_LED_INDICATOR_H
