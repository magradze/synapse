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
 * @brief rgb_led_indicator მოდულის Service API.
 * @details ეს სტრუქტურა გადაეცემა Service Locator-ს და საშუალებას აძლევს
 *          სხვა მოდულებს, მართონ LED ინდიკატორი.
 */
typedef struct {
    esp_err_t (*set_color)(uint8_t r, uint8_t g, uint8_t b);
    esp_err_t (*turn_off)();
    esp_err_t (*start_blink)(uint8_t r, uint8_t g, uint8_t b, uint32_t interval_ms);
    esp_err_t (*start_pulse)(uint8_t r, uint8_t g, uint8_t b, uint32_t period_ms);
    esp_err_t (*release_control)();
} rgb_led_api_t;

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
