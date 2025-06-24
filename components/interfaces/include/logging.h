/**
 * @file logging.h
 * @brief Synapse ESP Framework-ის ლოგირების ინტერფეისი
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-06-24
 *
 * @details
 * ეს ფაილი განსაზღვრავს ლოგირების მაკროებს და კონსტანტებს,
 * რომლებიც გამოიყენება Synapse ESP Framework-ის ყველა მოდულში.
 * უზრუნველყოფს ერთიან ლოგირების სტილს და კომპონენტების იდენტიფიკაციას.
 *
 * @note ყველა მოდულმა უნდა გამოიყენოს DEFINE_COMPONENT_TAG მაკრო
 * @warning არ შეცვალოთ არსებული TAG კონსტანტები
 */

#ifndef LOGGING_H
#define LOGGING_H

#include "esp_log.h"

/**
 * @brief კომპონენტის TAG-ის განსაზღვრა ლოგირებისთვის
 *
 * @details ეს მაკრო ქმნის static const char* TAG ცვლადს კომპონენტის სახელით.
 * გამოყენება: DEFINE_COMPONENT_TAG("MY_COMPONENT")
 *
 * __attribute__((unused)) ამცირებს კომპაილერის გაფრთხილებებს, როდესაც TAG
 * არ გამოიყენება ზოგიერთ ფაილში, მაგრამ მაინც საშუალებას იძლევა გამოყენება
 * საჭიროების შემთხვევაში.
 *
 * @param[in] component_name კომპონენტის სახელი, რომელიც გამოჩნდება ლოგებში
 *
 * @note ეს მაკრო უნდა გამოყენდეს ყველა მოდულის .c ფაილის დასაწყისში
 * @warning component_name პარამეტრი უნდა იყოს უნიკალური მოდულისთვის
 */
#define DEFINE_COMPONENT_TAG(component_name) \
  static const char *TAG __attribute__((unused)) = component_name

/**
 * @defgroup ComponentTags კომპონენტების TAG-ები
 * @{
 */

/**
 * @brief Event Bus კომპონენტის TAG
 */
#define TAG_EVENT_BUS "EVENT_BUS"

/**
 * @brief Config Manager კომპონენტის TAG
 */
#define TAG_CONFIG_MANAGER "CONFIG_MANAGER"

/**
 * @brief Module Registry კომპონენტის TAG
 */
#define TAG_MODULE_REGISTRY "MODULE_REGISTRY"

/**
 * @brief Service Locator კომპონენტის TAG
 */
#define TAG_SERVICE_LOCATOR "SERVICE_LOCATOR"

/**
 * @brief Event Wrapper კომპონენტის TAG
 */
#define TAG_EVENT_WRAPPER "EVENT_WRAPPER"

/**
 * @brief System Manager კომპონენტის TAG
 */
#define TAG_SYSTEM_MANAGER "SYSTEM_MANAGER"

/**
 * @brief Resource Manager კომპონენტის TAG
 */
#define TAG_RESOURCE_MANAGER "RESOURCE_MANAGER"

/** @} */

#endif // LOGGING_H