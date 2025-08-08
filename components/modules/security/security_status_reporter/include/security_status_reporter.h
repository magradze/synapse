/**
 * @file security_status_reporter.h
 * @brief Reports Secure Boot and Flash Encryption status.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @details Security Status Reporter მოდული რომელიც აღზევს Reports Secure Boot and Flash Encryption status. ფუნქციონალურობას.
 *          მოდული მუშაობს Synapse Framework-ის base_module interface-ის საფუძველზე.
 */

#ifndef SECURITY_STATUS_REPORTER_H
#define SECURITY_STATUS_REPORTER_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Security Status Reporter მოდულის კონფიგურაციის სტრუქტურა
 * @details შეიცავს მოდულის ყველა კონფიგურაციის პარამეტრს
 */
typedef struct {
    char instance_name[CONFIG_SYNAPSE_MODULE_NAME_MAX_LENGTH]; /**< მოდულის ინსტანციის სახელი */
    bool auto_start;                                        /**< ავტომატური გაშვება */
    // TODO: დაამატეთ მოდულის სპეციფიკური კონფიგურაციის ფილდები
} security_status_reporter_config_t;

/**
 * @brief ქმნის ახალ security_status_reporter მოდულის ინსტანციას
 * 
 * @param[in] config მოდულის კონფიგურაცია (JSON ობიექტი) ან NULL default-ებისთვის
 * @return მაჩვენებელი შექმნილ მოდულზე ან NULL შეცდომისას
 * 
 * @note შექმნილი მოდული უნდა დარეგისტრირდეს system_manager-ში
 * @warning მეხსიერება უნდა გათავისუფლდეს module->base.destroy() ფუნქციით
 */
module_t *security_status_reporter_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // SECURITY_STATUS_REPORTER_H
