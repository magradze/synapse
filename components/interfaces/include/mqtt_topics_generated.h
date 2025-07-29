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
 * @note გენერირების თარიღი: 2025-07-29 13:58:13 UTC
 * @note თემების რაოდენობა: 9
 * @note დამუშავებული მოდულები: button_input, connectivity_watchdog, device_identity_service, i2c_bus, mqtt_manager, relay_actuator, security_status_reporter, self_test_manager, sensor_aggregator, ssd1306_driver, time_of_flight_sensor, time_sync, ui_manager, wifi_manager
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
   * - MQTT_TOPIC_PUB_DEVICE_IDENTITY_SERVICE_INFO: .../state/device/info
 * - MQTT_TOPIC_PUB_SECURITY_STATUS_REPORTER_SECURITY_STATUS: .../state/security/status
 * - MQTT_TOPIC_SUB_SECURITY_STATUS_REPORTER_SECURITY_STATUS_CONTROL: .../control/security/status
 * - MQTT_TOPIC_PUB_SELF_TEST_MANAGER_SELFTEST_REPORT: .../data/selftest/report
 * - MQTT_TOPIC_SUB_MQTT_MANAGER_COMMAND_IN: .../cmd/in
 * - MQTT_TOPIC_PUB_WIFI_MANAGER_STATUS: .../state/wifi/status
 * - MQTT_TOPIC_PUB_WIFI_MANAGER_SCAN_RESULT: .../data/wifi/scan_result
 * - MQTT_TOPIC_PUB_SENSOR_AGGREGATOR_AGGREGATED_REPORT: .../data/sensors/aggregated
 * - MQTT_TOPIC_PUB_RELAY_ACTUATOR_STATE_CHANGED: .../state/relay/{module_name}/status
   */

  #define MQTT_TOPIC_PUB_DEVICE_IDENTITY_SERVICE_INFO "synapse/devices/%s/state/device/info"
#define MQTT_TOPIC_PUB_SECURITY_STATUS_REPORTER_SECURITY_STATUS "synapse/devices/%s/state/security/status"
#define MQTT_TOPIC_SUB_SECURITY_STATUS_REPORTER_SECURITY_STATUS_CONTROL "synapse/devices/%s/control/security/status"
#define MQTT_TOPIC_PUB_SELF_TEST_MANAGER_SELFTEST_REPORT "synapse/devices/%s/data/selftest/report"
#define MQTT_TOPIC_SUB_MQTT_MANAGER_COMMAND_IN "synapse/devices/%s/cmd/in"
#define MQTT_TOPIC_PUB_WIFI_MANAGER_STATUS "synapse/devices/%s/state/wifi/status"
#define MQTT_TOPIC_PUB_WIFI_MANAGER_SCAN_RESULT "synapse/devices/%s/data/wifi/scan_result"
#define MQTT_TOPIC_PUB_SENSOR_AGGREGATOR_AGGREGATED_REPORT "synapse/devices/%s/data/sensors/aggregated"
#define MQTT_TOPIC_PUB_RELAY_ACTUATOR_STATE_CHANGED "synapse/devices/%s/state/relay/{module_name}/status"

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
#define MQTT_TOPICS_GENERATION_TIMESTAMP "2025-07-29 13:58:13 UTC"
#define MQTT_TOPICS_TOTAL_COUNT 9
#define MQTT_TOPICS_GENERATED_FROM_MODULES "button_input, connectivity_watchdog, device_identity_service, i2c_bus, mqtt_manager, relay_actuator, security_status_reporter, self_test_manager, sensor_aggregator, ssd1306_driver, time_of_flight_sensor, time_sync, ui_manager, wifi_manager"

#ifdef __cplusplus
}
#endif

#endif // MQTT_TOPICS_GENERATED_H