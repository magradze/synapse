/**
 * @file event_bus.h
 * @brief Event Bus კომპონენტის Public API.
 * @version 2.0
 * @date 2025-06-27
 * @author Giorgi Magradze
 * @details ეს კომპონენტი უზრუნველყოფს ასინქრონული შეტყობინებების (events) მართვის
 *          მექანიზმს სტრიქონზე დაფუძნებული ივენთების სახელების გამოყენებით.
 *          მოდულებს შეუძლიათ გამოიწერონ კონკრეტული ივენთები და მიიღონ
 *          შეტყობინება, როდესაც რომელიმე სხვა მოდული გამოაქვეყნებს ამ ივენთს.
 *          ეს ხელს უწყობს მოდულების მაქსიმალურ იზოლაციასა და დინამიურ გაფართოებას.
 */

#ifndef FMW_EVENT_BUS_H
#define FMW_EVENT_BUS_H

#include <stddef.h>
#include "esp_err.h"

// Forward declarations to avoid circular dependencies
struct module_t;
struct event_data_wrapper_t;

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
esp_err_t fmw_event_bus_init(void);

/**
 * @brief აქვეყნებს ივენთს Event Bus-ზე ასინქრონულად.
 *
 * @details ეს ფუნქცია ამატებს ივენთს და მის თანდართულ მონაცემებს (თუ არსებობს)
 *          Event Bus-ის რიგში. ფონური ტასკი შემდგომში ამ ივენთს მიაწვდის
 *          ყველა იმ მოდულს, რომელსაც გამოწერილი აქვს ეს event_name.
 *
 * @param[in] event_name ივენთის უნიკალური სახელი (სტრიქონი).
 * @param[in] data_wrapper მაჩვენებელი "შეფუთულ" მონაცემებზე. თუ ივენთს მონაცემები
 *                         არ სჭირდება, ეს პარამეტრი უნდა იყოს NULL.
 *
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ ივენთი წარმატებით დაემატა რიგში.
 * @retval ESP_ERR_NO_MEM თუ მეხსიერება ვერ გამოიყო ივენთის სახელის კოპირებისთვის.
 * @retval ESP_FAIL თუ რიგი სავსეა.
 */
esp_err_t fmw_event_bus_post(const char *event_name, struct event_data_wrapper_t *data_wrapper);

/**
 * @brief არეგისტრირებს მოდულს, როგორც კონკრეტული ივენთის გამომწერს.
 *
 * @details როდესაც მითითებული `event_name`-ის მქონე ივენთი გამოქვეყნდება,
 *          Event Bus გამოიძახებს ამ `module`-ის `handle_event` ფუნქციას.
 *
 * @param[in] event_name ივენთის უნიკალური სახელი (სტრიქონი), რომელზეც გამოწერა ხდება.
 * @param[in] module მაჩვენებელი მოდულზე, რომელიც ამ ივენთს გამოიწერს.
 *
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ გამოწერა წარმატებით შესრულდა (ან მოდული უკვე გამოწერილი იყო).
 * @retval ESP_ERR_NO_MEM თუ მეხსიერება არასაკმარისია ახალი გამომწერის დასამატებლად.
 * @retval ESP_ERR_INVALID_ARG თუ `module` ან `event_name` არასწორია.
 */
esp_err_t fmw_event_bus_subscribe(const char *event_name, struct module_t *module);

/**
 * @brief აუქმებს მოდულის გამოწერას კონკრეტულ ივენთზე. ★★★ (ახალი) ★★★
 *
 * @details ეს ფუნქცია უნდა გამოიძახოს მოდულის `deinit` ფუნქციამ, რათა უსაფრთხოდ
 *          მოიხსნას გამოწერა და თავიდან ავიცილოთ შეცდომები, რომლებიც დაკავშირებულია
 *          განადგურებული მოდულის გამოძახების მცდელობასთან.
 *
 * @param[in] event_name ივენთის უნიკალური სახელი (სტრიქონი).
 * @param[in] module მაჩვენებელი მოდულზე, რომელიც აუქმებს გამოწერას.
 *
 * @return esp_err_t
 * @retval ESP_OK თუ გამოწერა წარმატებით გაუქმდა.
 * @retval ESP_ERR_NOT_FOUND თუ მოდული არ იყო გამოწერილი ამ ივენთზე.
 * @retval ESP_ERR_INVALID_ARG თუ პარამეტრები არასწორია.
 */
esp_err_t fmw_event_bus_unsubscribe(const char *event_name, struct module_t *module);

#endif // FMW_EVENT_BUS_H