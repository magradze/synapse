/**
 * @file event_bus.h
 * @brief Event Bus კომპონენტის API
 * @version 1.0
 * @date 2025-06-24
 * @author Giorgi Magradze
 * @details ეს კომპონენტი უზრუნველყოფს ასინქრონული შეტყობინებების (events) მართვის
 *          მექანიზმს. მოდულებს შეუძლიათ გამოიწერონ კონკრეტული ტიპის ივენთები
 *          და მიიღონ შეტყობინება, როდესაც რომელიმე სხვა მოდული გამოაქვეყნებს
 *          ამ ივენთს. ეს ხელს უწყობს მოდულების ერთმანეთისგან იზოლირებას.
 */

#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <stddef.h>
#include "esp_err.h"

// Forward declaration of the module_t struct
struct module_t;

// --- Framework Contracts ---
#include "event_data_wrapper.h"
#include "system_event_ids.h"

/**
 * @brief Event Bus-ის ინიციალიზაცია და ფონური ტასკის გაშვება.
 *
 * @details ეს ფუნქცია ქმნის ივენთების რიგს (queue) და აამუშავებს ფონურ ტასკს,
 *          რომელიც პასუხისმგებელია რიგში მოხვედრილი ივენთების დამუშავებასა
 *          და შესაბამისი გამომწერების ინფორმირებაზე.
 *          უნდა გამოიძახოს `system_manager`-მა ინიციალიზაციისას.
 *
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ ინიციალიზაცია წარმატებით დასრულდა.
 * @retval ESP_FAIL თუ ტასკის ან რიგის შექმნა ვერ მოხერხდა.
 */
esp_err_t event_bus_init(void);

/**
 * @brief აქვეყნებს ივენთს Event Bus-ზე ასინქრონულად.
 *
 * @details ეს ფუნქცია ამატებს ივენთს და მის თანდართულ მონაცემებს (თუ არსებობს)
 *          Event Bus-ის რიგში. ფონური ტასკი შემდგომში ამ ივენთს მიაწვდის
 *          ყველა იმ მოდულს, რომელსაც გამოწერილი აქვს ეს event_id.
 *
 * @param[in] event_id ივენთის უნიკალური იდენტიფიკატორი.
 * @param[in] data_wrapper მაჩვენებელი "შეფუთულ" მონაცემებზე. თუ ივენთს მონაცემები
 *                         არ სჭირდება, ეს პარამეტრი უნდა იყოს NULL.
 *                         მნიშვნელოვანია: Event Bus არ არის პასუხისმგებელი ამ
 *                         მეხსიერების განთავისუფლებაზე. გამომძახებელმა ან
 *                         მიმღებმა უნდა იზრუნოს ამაზე.
 *
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ ივენთი წარმატებით დაემატა რიგში.
 * @retval ESP_FAIL თუ რიგი სავსეა ან სხვა შეცდომა მოხდა.
 */
esp_err_t event_bus_post(system_event_id_t event_id, event_data_wrapper_t *data_wrapper);

/**
 * @brief არეგისტრირებს მოდულს, როგორც კონკრეტული ივენთის გამომწერს.
 *
 * @details როდესაც მითითებული `event_id`-ის მქონე ივენთი გამოქვეყნდება,
 *          Event Bus გამოიძახებს ამ `module`-ის `on_event` ფუნქციას.
 *
 * @param[in] event_id ივენთის ID, რომელზეც გამოწერა ხდება.
 * @param[in] module მაჩვენებელი მოდულზე, რომელიც ამ ივენთს გამოიწერს.
 *
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ გამოწერა წარმატებით შესრულდა.
 * @retval ESP_ERR_NO_MEM თუ მეხსიერება არასაკმარისია ახალი გამომწერის დასამატებლად.
 * @retval ESP_ERR_INVALID_ARG თუ `module` არასწორია.
 */
esp_err_t event_bus_subscribe(system_event_id_t event_id, struct module_t *module);

#endif // EVENT_BUS_H