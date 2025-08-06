/**
 * @file event_data_wrapper.h
 * @brief Event Bus-ისთვის მონაცემთა შეფუთვის (Wrapper) და Reference Counting-ის API.
 * @version 2.0
 * @date 2025-06-25
 * @author Giorgi Magradze
 * @details ეს კომპონენტი უზრუნველყოფს მექანიზმს, რომლის საშუალებითაც შესაძლებელია
 *          დინამიურად გამოყოფილი მეხსიერების უსაფრთხოდ გადაცემა Event Bus-ის
 *          საშუალებით მრავალი მიმღებისთვის. ის იყენებს Reference Counting-ის
 *          პატერნს, რათა მეხსიერება ავტომატურად განთავისუფლდეს მხოლოდ მაშინ,
 *          როდესაც მასზე ბოლო მომხმარებელი დაასრულებს მუშაობას.
 */

#ifndef FMW_EVENT_DATA_WRAPPER_H
#define FMW_EVENT_DATA_WRAPPER_H

#include "esp_err.h"
#include <stdint.h>
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/**
 * @brief ზოგადი "შეფუთვა" (wrapper) ნებისმიერი ივენთის მონაცემებისთვის.
 *
 * @details ეს სტრუქტურა ამატებს Reference Counting მექანიზმს
 *          ნებისმიერი ტიპის დინამიურად შექმნილ მონაცემებზე (payload),
 *          რომლებიც Event Bus-ით გადაიცემა.
 */
typedef struct event_data_wrapper_t
{
    int32_t ref_count;                      /**< @brief მრიცხველი, რომელიც აჩვენებს, რამდენი მომხმარებელი იყენებს ამ მონაცემს. */
    SemaphoreHandle_t mutex;                /**< @brief Mutex, რომელიც უზრუნველყოფს ref_count-ის ატომურ (thread-safe) ცვლილებას. */
    void *payload;                          /**< @brief მაჩვენებელი რეალურ მონაცემებზე (მაგ., telemetry_data_t*). */
    void (*free_payload_fn)(void *payload); /**< @brief ფუნქციის მაჩვენებელი, რომელიც გამოიძახება payload-ის მეხსიერების გასათავისუფლებლად. */
} event_data_wrapper_t;

/**
 * @brief ქმნის ახალ "შეფუთულ" მონაცემთა ობიექტს (wrapper) და ამყარებს საწყის მფლობელობას.
 *
 * @details ეს ფუნქცია იღებს დინამიურად შექმნილ მონაცემებს (payload) და "ფუთავს" მას
 *          wrapper-ში, ამატებს რა Reference Counting-ის მექანიზმს. შექმნისას
 *          `ref_count` ინიციალიზდება 1-ით, რაც ნიშნავს, რომ გამომძახებელს
 *          ეკუთვნის ეს ობიექტი და პასუხისმგებელია მის განთავისუფლებაზე
 *          `fmw_event_data_release`-ის გამოძახებით.
 *
 * @param[in] payload მაჩვენებელი რეალურ მონაცემებზე, რომლებიც უნდა "შეიფუთოს".
 * @param[in] free_fn ფუნქცია, რომელმაც იცის, როგორ გაათავისუფლოს payload-ის მეხსიერება.
 *                    თუ ეს პარამეტრი NULL-ია, payload-ის გასათავისუფლებლად გამოიყენება
 *                    სტანდარტული `free()` ფუნქცია.
 * @param[out] wrapper_out მაჩვენებელი, რომელშიც ჩაიწერება ახლად შექმნილი wrapper ობიექტის
 *                         მაჩვენებელი.
 *
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ wrapper ობიექტი წარმატებით შეიქმნა.
 * @retval ESP_ERR_INVALID_ARG თუ `payload` ან `wrapper_out` არის NULL.
 * @retval ESP_ERR_NO_MEM თუ მეხსიერების გამოყოფა ვერ მოხერხდა.
 */
esp_err_t fmw_event_data_wrap(const void *payload, void (*free_fn)(void *payload), event_data_wrapper_t **wrapper_out);

/**
 * @brief ზრდის wrapper-ის მფლობელების (references) რაოდენობას ერთით.
 *
 * @details ამ ფუნქციას, როგორც წესი, იძახებს Event Bus-ი, როდესაც ივენთს გადასცემს
 *          ახალ გამომწერ მოდულს. თითოეული `acquire` გამოძახება უნდა დაწყვილდეს
 *          `release` გამოძახებასთან.
 *
 * @param[in] wrapper მაჩვენებელი wrapper ობიექტზე.
 * @return esp_err_t
 * @retval ESP_OK თუ ოპერაცია წარმატებით შესრულდა.
 * @retval ESP_ERR_INVALID_ARG თუ `wrapper` არის NULL.
 */
esp_err_t fmw_event_data_acquire(event_data_wrapper_t* wrapper);

/**
 * @brief ამცირებს wrapper-ის მფლობელების (references) რაოდენობას ერთით.
 *
 * @details ამ ფუნქციას იძახებენ მომხმარებლები (მოდულები), როდესაც ისინი
 *          ასრულებენ მონაცემებზე მუშაობას. თუ `ref_count` ამ გამოძახების
 *          შემდეგ 0 ხდება, ეს ფუნქცია ავტომატურად ათავისუფლებს ყველა
 *          დაკავშირებულ მეხსიერებას (თავად payload-ს და wrapper-ს).
 *
 * @param[in] wrapper მაჩვენებელი wrapper ობიექტზე.
 * @return esp_err_t
 * @retval ESP_OK თუ ოპერაცია წარმატებით შესრულდა.
 * @retval ESP_ERR_INVALID_ARG თუ `wrapper` არის NULL.
 */
esp_err_t fmw_event_data_release(event_data_wrapper_t* wrapper);

#endif // FMW_EVENT_DATA_WRAPPER_H