/**
 * @file sensor_simulator.h
 * @brief Simulates sensor data for testing purposes.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @details Sensor Simulator მოდული რომელიც აღზევს Simulates sensor data for testing purposes. ფუნქციონალურობას.
 *          მოდული მუშაობს Synapse Framework-ის base_module interface-ის საფუძველზე.
 */

#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sensor Simulator მოდულის კონფიგურაციის სტრუქტურა
 * @details შეიცავს მოდულის ყველა კონფიგურაციის პარამეტრს
 */
typedef struct {
    char instance_name[CONFIG_SYNAPSE_MODULE_NAME_MAX_LENGTH]; /**< მოდულის ინსტანციის სახელი */
    bool auto_start;                                        /**< ავტომატური გაშვება */
    // TODO: დაამატეთ მოდულის სპეციფიკური კონფიგურაციის ფილდები
} sensor_simulator_config_t;

/**
 * @brief ქმნის ახალ sensor_simulator მოდულის ინსტანციას
 * 
 * @param[in] config მოდულის კონფიგურაცია (JSON ობიექტი) ან NULL default-ებისთვის
 * @return მაჩვენებელი შექმნილ მოდულზე ან NULL შეცდომისას
 * 
 * @note შექმნილი მოდული უნდა დარეგისტრირდეს system_manager-ში
 * @warning მეხსიერება უნდა გათავისუფლდეს module->base.destroy() ფუნქციით
 */
module_t *sensor_simulator_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // SENSOR_SIMULATOR_H
