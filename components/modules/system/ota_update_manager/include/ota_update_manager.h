/**
 * @file ota_update_manager.h
 * @brief Handles secure Over-The-Air firmware updates
 * @author Synapse Framework Team
 * @version 1.0.0
 * @details Ota Update Manager მოდული რომელიც აღზევს Handles secure Over-The-Air firmware updates ფუნქციონალურობას.
 *          მოდული მუშაობს Synapse Framework-ის base_module interface-ის საფუძველზე.
 */

#ifndef OTA_UPDATE_MANAGER_H
#define OTA_UPDATE_MANAGER_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Ota Update Manager მოდულის კონფიგურაციის სტრუქტურა
 * @details შეიცავს მოდულის ყველა კონფიგურაციის პარამეტრს
 */
typedef struct {
    char instance_name[CONFIG_SYNAPSE_MODULE_NAME_MAX_LENGTH]; /**< მოდულის ინსტანციის სახელი */
    bool auto_start;                                        /**< ავტომატური გაშვება */
} ota_update_manager_config_t;

/**
 * @brief ქმნის ახალ ota_update_manager მოდულის ინსტანციას
 * 
 * @param[in] config მოდულის კონფიგურაცია (JSON ობიექტი) ან NULL default-ებისთვის
 * @return მაჩვენებელი შექმნილ მოდულზე ან NULL შეცდომისას
 * 
 * @note შექმნილი მოდული უნდა დარეგისტრირდეს system_manager-ში
 * @warning მეხსიერება უნდა გათავისუფლდეს module->base.destroy() ფუნქციით
 */
module_t *ota_update_manager_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // OTA_UPDATE_MANAGER_H
