/**
 * @file wifi_manager.h
 * @brief WiFi connection management
 * @author Synapse Framework Team
 * @version 1.0.0
 * @details Wifi_Manager მოდული რომელიც აღზევს WiFi connection management ფუნქციონალურობას.
 *          მოდული მუშაობს Synapse Framework-ის base_module interface-ის საფუძველზე.
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Wifi_Manager მოდულის კონფიგურაციის სტრუქტურა
 * @details შეიცავს მოდულის ყველა კონფიგურაციის პარამეტრს
 */
typedef struct {
    char instance_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];  /**< მოდულის ინსტანციის სახელი */
    bool auto_start;                                        /**< ავტომატური გაშვება */
    // TODO: დაამატეთ მოდულის სპეციფიკური კონფიგურაციის ფილდები
} wifi_manager_config_t;

/**
 * @brief ქმნის ახალ wifi_manager მოდულის ინსტანციას
 * 
 * @param[in] config მოდულის კონფიგურაცია (JSON ობიექტი) ან NULL default-ებისთვის
 * @return მაჩვენებელი შექმნილ მოდულზე ან NULL შეცდომისას
 * 
 * @note შექმნილი მოდული უნდა დარეგისტრირდეს system_manager-ში
 * @warning მეხსიერება უნდა გათავისუფლდეს module->base.destroy() ფუნქციით
 */
module_t *wifi_manager_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // WIFI_MANAGER_H
