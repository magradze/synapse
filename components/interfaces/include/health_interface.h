/**
 * @file health_interface.h
 * @brief Health Monitor მოდულის საჯარო სერვისის API-ს "კონტრაქტი".
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-07-01
 * @details ეს ჰედერი განსაზღვრავს Health Monitor სერვისის აბსტრაქტულ
 *          ინტერფეისს (API სტრუქტურას). ნებისმიერმა მოდულმა, რომელსაც სურს
 *          Health Monitor-თან ურთიერთქმედება Service Locator-ის მეშვეობით,
 *          უნდა დააინქლუდოს ეს ფაილი. ეს უზრუნველყოფს მომხმარებელი მოდულის
 *          დამოუკიდებლობას health_monitor-ის კონკრეტული იმპლემენტაციისგან.
 */

#ifndef SYNAPSE_HEALTH_INTERFACE_H
#define SYNAPSE_HEALTH_INTERFACE_H

#include "esp_err.h"
#include "cJSON.h" // რადგან get_system_health_report იყენებს cJSON-ს

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Custom health check ფუნქციის ტიპი.
 * @details სხვა მოდულებს შეუძლიათ ამ ტიპის ფუნქციის იმპლემენტაცია და დარეგისტრირება
 *          Health Monitor-ში, რათა ის პერიოდულად გამოიძახოს.
 * @param context მომხმარებლის მიერ რეგისტრაციისას გადაცემული კონტექსტი.
 * @return ESP_OK თუ შემოწმება წარმატებულია, სხვა შემთხვევაში შეცდომის კოდი.
 */
typedef esp_err_t (*health_check_fn_t)(void *context);

/**
 * @brief Health Monitor-ის საჯარო Service API სტრუქტურა.
 * @details ეს არის "კონტრაქტი", რომელსაც health_monitor მოდული ახდენს
 *          იმპლემენტაციას, ხოლო სხვა მოდულები იყენებენ.
 */
typedef struct
{
    /**
     * @brief აბრუნებს სისტემის ჯანმრთელობის სრულ რეპორტს JSON ფორმატში.
     * @param[out] report მაჩვენებელი cJSON ობიექტზე, რომელშიც ჩაიწერება რეპორტი.
     *                  გამომძახებელმა უნდა გაათავისუფლოს ეს ობიექტი cJSON_Delete-ით.
     * @return ESP_OK წარმატების შემთხვევაში.
     */
    esp_err_t (*get_system_health_report)(cJSON **report);

    /**
     * @brief არეგისტრირებს ახალ, მომხმარებლის მიერ განსაზღვრულ შემოწმებას.
     * @param[in] check_name შემოწმების უნიკალური სახელი.
     * @param[in] check_fn გამოსაძახებელი ფუნქცია.
     * @param[in] context კონტექსტი, რომელიც გადაეცემა check_fn-ს.
     * @return ESP_OK წარმატების შემთხვევაში.
     */
    esp_err_t (*register_custom_check)(const char *check_name, health_check_fn_t check_fn, void *context);

    /**
     * @brief შლის რეგისტრირებულ შემოწმებას.
     * @param[in] check_name წასაშლელი შემოწმების სახელი.
     * @return ESP_OK წარმატების შემთხვევაში.
     */
    esp_err_t (*unregister_custom_check)(const char *check_name);

} health_api_t;


#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_HEALTH_INTERFACE_H