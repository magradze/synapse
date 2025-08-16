#ifndef MQTT_TOPICS_GENERATED_H
#define MQTT_TOPICS_GENERATED_H

/**
 * @file mqtt_topics_generated.h
 * @brief MindLab ESP Framework - ავტომატურად გენერირებული MQTT თემების განსაზღვრებები
 *
 * ⚠️  გაფრთხილება: ეს ფაილი გენერირდება ავტომატურად! ხელით არ შეცვალოთ!
 *
 * ეს ფაილი ავტომატურად გენერირდება build პროცესში module.json ფაილებიდან.
 * ყველა MQTT თემის განსაზღვრება აგროვდება ინდივიდუალური მოდულებიდან და
 * ცენტრალიზებულად ინახება აქ, რათა უზრუნველყოფილ იქნას ერთიანი გამოყენება მთელ ფრეიმვორქში.
 *
 * ახალი თემის დასამატებლად ან შესაცვლელად:
 * 1. შეცვალეთ თქვენი მოდულის module.json ფაილის "mqtt_topics" სექცია
 * 2. გაუშვით build - ეს ფაილი ავტომატურად განახლდება
 *
 * ფორმატი: mindlab/v1/{device_id}/{message_type}/{module_name}/{action}
 *
 * @note გენერირების თარიღი: 2025-08-13 13:08:10 UTC
 * @note თემების რაოდენობა: 0
 * @note დამუშავებული მოდულები: 
 * @version 1.0
 * @author MindLab Auto-Generation System
 */

#ifdef __cplusplus
extern "C"
{
#endif

  // =============================================================================
  // ავტომატურად გენერირებული MQTT თემების განსაზღვრებები
  // =============================================================================

  /**
   * @brief ავტომატურად გენერირებული MQTT თემების კონსტანტები
   * @details ქვემოთ მოცემული თემები ავტომატურად შეგროვდა module.json ფაილებიდან:
   *
  
   */

  

// =============================================================================
// დამხმარე მაკროები (თავსებადი ხელით შექმნილ mqtt_topics.h-თან)
// =============================================================================

/**
 * @brief სრულყოფილი MQTT თემის აგება მოწყობილობის ID-ით
 * @param buffer გამოსატანი ბუფერი თემისთვის
 * @param buffer_size ბუფერის ზომა
 * @param topic_template თემის შაბლონი %s placeholder-ით device_id-სთვის
 * @param device_id ჩასასმელი მოწყობილობის ID
 * @return ჩაწერილი სიმბოლოების რაოდენობა (null-terminator-ის გარეშე)
 */
#define MQTT_BUILD_TOPIC(buffer, buffer_size, topic_template, device_id) \
  snprintf(buffer, buffer_size, topic_template, device_id)

/**
 * @brief Base MQTT topic format
 * @details Format: synapse/devices/{device_id}
 */
#define MQTT_TOPIC_BASE "synapse/devices/%s"
// =============================================================================
// გენერირების ინფორმაცია
// =============================================================================

/**
 * @brief ავტომატური გენერაციის მეტამონაცემები
 */
#define MQTT_TOPICS_GENERATION_TIMESTAMP "2025-08-13 13:08:10 UTC"
#define MQTT_TOPICS_TOTAL_COUNT 0
#define MQTT_TOPICS_GENERATED_FROM_MODULES ""

#ifdef __cplusplus
}
#endif

#endif // MQTT_TOPICS_GENERATED_H