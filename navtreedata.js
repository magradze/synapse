/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Synapse ESP Framework", "index.html", [
    [ "პროექტის მიმოხილვა (Project Overview)", "index.html", "index" ],
    [ "Synapse Actuators Module API Reference", "md_docs_2api__reference_2actuators__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2actuators__api.html#autotoc_md1", null ],
      [ "Relay Module API", "md_docs_2api__reference_2actuators__api.html#autotoc_md3", [
        [ "module_t <em>relay_module_create(const cJSON *config);", "md_docs_2api__reference_2actuators__api.html#autotoc_md4", null ],
        [ "esp_err_t relay_module_enable(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md5", null ],
        [ "esp_err_t relay_module_disable(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md6", null ],
        [ "esp_err_t relay_module_toggle(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md7", null ],
        [ "esp_err_t relay_module_set_state(module_t *module, bool state);", "md_docs_2api__reference_2actuators__api.html#autotoc_md8", null ],
        [ "bool relay_module_get_state(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md9", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2actuators__api.html#autotoc_md11", [
        [ "static void relay_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2actuators__api.html#autotoc_md12", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2actuators__api.html#autotoc_md14", [
        [ "static esp_err_t parse_relay_config(const cJSON *config, relay_private_data_t *relay_data);", "md_docs_2api__reference_2actuators__api.html#autotoc_md15", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2actuators__api.html#autotoc_md17", [
        [ "static module_status_t relay_get_status(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md18", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2actuators__api.html#autotoc_md20", null ],
      [ "მაგალითები", "md_docs_2api__reference_2actuators__api.html#autotoc_md22", null ]
    ] ],
    [ "Synapse API Reference Index", "md_docs_2api__reference_2api__index.html", [
      [ "Core API", "md_docs_2api__reference_2api__index.html#autotoc_md26", null ],
      [ "Module API", "md_docs_2api__reference_2api__index.html#autotoc_md27", null ],
      [ "System & Extensions", "md_docs_2api__reference_2api__index.html#autotoc_md28", null ]
    ] ],
    [ "Synapse Assertions & Guards API Reference", "md_docs_2api__reference_2assert__and__guards__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md31", null ],
      [ "1. Assertion API (<tt>synapse_assert.h</tt>)", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md33", [
        [ "<tt>SYNAPSE_ASSERT(condition, format, ...)</tt>", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md34", null ]
      ] ],
      [ "2. Guard Clause API (<tt>synapse_utils.h</tt>)", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md36", [
        [ "<tt>SYNAPSE_GUARD(condition, tag, ret_val, format, ...)</tt>", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md37", null ],
        [ "<tt>SYNAPSE_GUARD_VOID(condition, tag, format, ...)</tt>", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md38", null ]
      ] ]
    ] ],
    [ "Synapse Command Router API Reference", "md_docs_2api__reference_2command__router__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2command__router__api.html#autotoc_md40", null ],
      [ "⚙️ API სტრუქტურა (<tt>cmd_router_api_t</tt>)", "md_docs_2api__reference_2command__router__api.html#autotoc_md42", null ],
      [ "📚 API ფუნქციები", "md_docs_2api__reference_2command__router__api.html#autotoc_md44", [
        [ "<tt>esp_err_t register_command(const cmd_t *command)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md45", null ],
        [ "<tt>esp_err_t unregister_command(const char *command_name)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md47", null ],
        [ "<tt>bool is_command_registered(const char *command_name)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md49", null ]
      ] ],
      [ "📖 დამხმარე სტრუქტურა (<tt>cmd_t</tt>)", "md_docs_2api__reference_2command__router__api.html#autotoc_md51", null ],
      [ "💡 გამოყენების პატერნი: კონფლიქტისგან დაცული რეგისტრაცია", "md_docs_2api__reference_2command__router__api.html#autotoc_md53", null ]
    ] ],
    [ "📡 Synapse Communication Module API Reference", "md_docs_2api__reference_2communication__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2communication__api.html#autotoc_md55", null ],
      [ "📶 WiFi Manager Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md57", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md58", null ],
        [ "💡 WiFi API-ს გამოყენების მაგალითი", "md_docs_2api__reference_2communication__api.html#autotoc_md59", null ]
      ] ],
      [ "☁️ MQTT Client Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md61", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md62", null ],
        [ "💡 MQTT API-ს გამოყენების მაგალითი", "md_docs_2api__reference_2communication__api.html#autotoc_md63", null ]
      ] ],
      [ "⚡ ESP-NOW Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md65", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md66", null ]
      ] ],
      [ "📜 Naming & Structure კონვენციები", "md_docs_2api__reference_2communication__api.html#autotoc_md68", null ]
    ] ],
    [ "Synapse Configuration Manager API Reference", "md_docs_2api__reference_2configuration__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2configuration__api.html#autotoc_md71", null ],
      [ "🔑 გასაღების ფორმატი (Dot-Notation)", "md_docs_2api__reference_2configuration__api.html#autotoc_md73", null ],
      [ "⚙️ ძირითადი API ფუნქციები", "md_docs_2api__reference_2configuration__api.html#autotoc_md75", [
        [ "<tt>esp_err_t synapse_config_get_string(const char *key, char *buffer, size_t buffer_size)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md76", null ],
        [ "<tt>esp_err_t synapse_config_get_int(const char *key, int *out_value)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md77", null ],
        [ "<tt>esp_err_t synapse_config_get_bool(const char *key, bool *out_value)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md78", null ],
        [ "<tt>esp_err_t synapse_config_save(void)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md79", null ]
      ] ],
      [ "🚀 გამოყენების მაგალითები", "md_docs_2api__reference_2configuration__api.html#autotoc_md81", [
        [ "გლობალური პარამეტრის წაკითხვა", "md_docs_2api__reference_2configuration__api.html#autotoc_md82", null ],
        [ "მოდულის პარამეტრის წაკითხვა", "md_docs_2api__reference_2configuration__api.html#autotoc_md83", null ]
      ] ]
    ] ],
    [ "Synapse Core API Reference", "md_docs_2api__reference_2core__api.html", [
      [ "⚙️ Service Locator API (v2.0 - State-Aware)", "md_docs_2api__reference_2core__api.html#autotoc_md85", [
        [ "<tt>esp_err_t synapse_service_register_with_status(const char *service_name, synapse_service_type_t service_type, service_handle_t service_handle, service_status_t initial_status);</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md86", null ],
        [ "<tt>esp_err_t synapse_service_set_status(const char *service_name, service_status_t new_status);</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md87", null ],
        [ "<tt>esp_err_t synapse_service_get_status(const char *service_name, service_status_t *out_status);</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md88", null ],
        [ "<tt>service_handle_t synapse_service_get(const char *service_name);</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md89", null ],
        [ "<tt>esp_err_t synapse_service_unregister(const char *service_name);</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md90", null ],
        [ "<tt>__attribute__((deprecated))</tt> <tt>esp_err_t synapse_service_register(...)</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md91", null ]
      ] ],
      [ "Event Bus API", "md_docs_2api__reference_2core__api.html#autotoc_md93", [
        [ "<tt>synapse_event_bus_post</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md94", null ],
        [ "<tt>synapse_event_bus_subscribe</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md95", null ],
        [ "<tt>synapse_event_bus_unsubscribe</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md96", null ]
      ] ],
      [ "Module Register API", "md_docs_2api__reference_2core__api.html#autotoc_md98", [
        [ "synapse_module_register", "md_docs_2api__reference_2core__api.html#autotoc_md99", null ],
        [ "synapse_module_find_by_name", "md_docs_2api__reference_2core__api.html#autotoc_md100", null ]
      ] ],
      [ "Module Factory API", "md_docs_2api__reference_2core__api.html#autotoc_md102", [
        [ "synapse_module_factory_create", "md_docs_2api__reference_2core__api.html#autotoc_md103", null ]
      ] ],
      [ "Configuration Manager API", "md_docs_2api__reference_2core__api.html#autotoc_md105", [
        [ "synapse_config_get_string", "md_docs_2api__reference_2core__api.html#autotoc_md106", null ],
        [ "synapse_config_get_int", "md_docs_2api__reference_2core__api.html#autotoc_md107", null ],
        [ "synapse_config_get_module_config", "md_docs_2api__reference_2core__api.html#autotoc_md108", null ]
      ] ],
      [ "Logging API", "md_docs_2api__reference_2core__api.html#autotoc_md110", [
        [ "DEFINE_COMPONENT_TAG", "md_docs_2api__reference_2core__api.html#autotoc_md111", null ],
        [ "ESP_LOGE, ESP_LOGW, ESP_LOGI, ESP_LOGD, ESP_LOGV", "md_docs_2api__reference_2core__api.html#autotoc_md112", null ]
      ] ]
    ] ],
    [ "Synapse Custom API Reference", "md_docs_2api__reference_2custom__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2custom__api.html#autotoc_md115", null ],
      [ "Custom Module API", "md_docs_2api__reference_2custom__api.html#autotoc_md117", [
        [ "module_t *custom_module_create(const cJSON *config);", "md_docs_2api__reference_2custom__api.html#autotoc_md118", null ],
        [ "esp_err_t custom_module_do_action(module_t *module, const char *action, const void *params);", "md_docs_2api__reference_2custom__api.html#autotoc_md119", null ],
        [ "esp_err_t custom_module_get_status(module_t *module, custom_status_t *status);", "md_docs_2api__reference_2custom__api.html#autotoc_md120", null ]
      ] ],
      [ "Custom Event API", "md_docs_2api__reference_2custom__api.html#autotoc_md122", [
        [ "Custom Event-ის იდენტიფიკატორი", "md_docs_2api__reference_2custom__api.html#autotoc_md123", null ],
        [ "Custom Event-ის მონაცემების სტრუქტურა", "md_docs_2api__reference_2custom__api.html#autotoc_md124", null ],
        [ "static void custom_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2custom__api.html#autotoc_md125", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2custom__api.html#autotoc_md127", null ],
      [ "მაგალითები", "md_docs_2api__reference_2custom__api.html#autotoc_md129", null ]
    ] ],
    [ "Synapse Device Identity Service API Reference", "md_docs_2api__reference_2device__identity__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2device__identity__api.html#autotoc_md132", null ],
      [ "Device Identity Service API", "md_docs_2api__reference_2device__identity__api.html#autotoc_md134", [
        [ "<tt>const char* get_device_id(void);</tt>", "md_docs_2api__reference_2device__identity__api.html#autotoc_md135", null ],
        [ "<tt>const char* get_firmware_version(void);</tt>", "md_docs_2api__reference_2device__identity__api.html#autotoc_md136", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2device__identity__api.html#autotoc_md138", null ],
      [ "მაგალითები", "md_docs_2api__reference_2device__identity__api.html#autotoc_md140", null ]
    ] ],
    [ "Synapse Display Module API Reference", "md_docs_2api__reference_2display__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2display__api.html#autotoc_md143", null ],
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2display__api.html#autotoc_md145", [
        [ "module_t <em>ssd1306_module_create(const cJSON *config);", "md_docs_2api__reference_2display__api.html#autotoc_md146", null ],
        [ "static esp_err_t ssd1306_init(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md147", null ],
        [ "static esp_err_t ssd1306_enable(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md148", null ],
        [ "static esp_err_t ssd1306_disable(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md149", null ],
        [ "static esp_err_t ssd1306_deinit(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md150", null ]
      ] ],
      [ "Service API (Service Locator-ისთვის)", "md_docs_2api__reference_2display__api.html#autotoc_md152", [
        [ "esp_err_t ssd1306_api_enable(void);", "md_docs_2api__reference_2display__api.html#autotoc_md153", null ],
        [ "esp_err_t ssd1306_api_disable(void);", "md_docs_2api__reference_2display__api.html#autotoc_md154", null ],
        [ "esp_err_t ssd1306_api_clear(void);", "md_docs_2api__reference_2display__api.html#autotoc_md155", null ],
        [ "esp_err_t ssd1306_api_write_text(const char *text, uint8_t line);", "md_docs_2api__reference_2display__api.html#autotoc_md156", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2display__api.html#autotoc_md158", [
        [ "static void ssd1306_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2display__api.html#autotoc_md159", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2display__api.html#autotoc_md161", [
        [ "static esp_err_t parse_ssd1306_config(const cJSON *config, ssd1306_private_data_t *ssd1306_data);", "md_docs_2api__reference_2display__api.html#autotoc_md162", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2display__api.html#autotoc_md164", [
        [ "static module_status_t ssd1306_get_status(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md165", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2display__api.html#autotoc_md167", null ],
      [ "მაგალითები", "md_docs_2api__reference_2display__api.html#autotoc_md169", null ]
    ] ],
    [ "Synapse Drivers Module API Reference", "md_docs_2api__reference_2drivers__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2drivers__api.html#autotoc_md172", null ],
      [ "I2C Bus Driver API", "md_docs_2api__reference_2drivers__api.html#autotoc_md174", [
        [ "module_t *i2c_bus_module_create(const cJSON *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md175", null ],
        [ "esp_err_t i2c_bus_init(i2c_bus_handle_t *i2c_bus_handle, const i2c_bus_config_t *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md176", null ],
        [ "esp_err_t i2c_bus_read(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, uint8_t *data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md177", null ],
        [ "esp_err_t i2c_bus_write(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, const uint8_t *data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md178", null ],
        [ "esp_err_t i2c_bus_scan(i2c_bus_handle_t *i2c_bus_handle);", "md_docs_2api__reference_2drivers__api.html#autotoc_md179", null ]
      ] ],
      [ "SPI Bus Driver API", "md_docs_2api__reference_2drivers__api.html#autotoc_md181", [
        [ "module_t *spi_bus_module_create(const cJSON *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md182", null ],
        [ "esp_err_t spi_bus_init(spi_bus_handle_t *spi_bus_handle, const spi_bus_config_t *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md183", null ],
        [ "esp_err_t spi_bus_transfer(spi_bus_handle_t *spi_bus_handle, const uint8_t *tx_data, uint8_t *rx_data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md184", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2drivers__api.html#autotoc_md186", null ],
      [ "მაგალითები", "md_docs_2api__reference_2drivers__api.html#autotoc_md188", null ]
    ] ],
    [ "Synapse Event Bus API Reference", "md_docs_2api__reference_2event__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2event__api.html#autotoc_md191", null ],
      [ "Event Bus API", "md_docs_2api__reference_2event__api.html#autotoc_md193", [
        [ "<tt>esp_err_t synapse_event_bus_init(void)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md194", null ],
        [ "<tt>esp_err_t synapse_event_bus_post(core_framework_event_id_t event_id, event_data_wrapper_t *data_wrapper)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md195", null ],
        [ "<tt>esp_err_t synapse_event_bus_subscribe(core_framework_event_id_t event_id, struct module_t *module)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md196", null ],
        [ "<tt>esp_err_t synapse_event_bus_unsubscribe(core_framework_event_id_t event_id, struct module_t *module)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md197", null ]
      ] ],
      [ "ივენთის მონაცემების მართვა (Reference Counting)", "md_docs_2api__reference_2event__api.html#autotoc_md199", null ],
      [ "Event Handler-ის მაგალითი", "md_docs_2api__reference_2event__api.html#autotoc_md201", null ],
      [ "Custom Event-ების აღწერა", "md_docs_2api__reference_2event__api.html#autotoc_md203", [
        [ "მაგალითი Custom Event-ის აღწერისა", "md_docs_2api__reference_2event__api.html#autotoc_md204", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2event__api.html#autotoc_md206", null ],
      [ "მაგალითები", "md_docs_2api__reference_2event__api.html#autotoc_md208", null ]
    ] ],
    [ "API რეფერენსი: Event Payloads (event_payloads.h)", "md_docs_2api__reference_2event__payloads__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md211", null ],
      [ "Payload სტრუქტურები", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md212", [
        [ "synapse_telemetry_payload_t", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md213", null ],
        [ "synapse_config_updated_payload_t", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md214", null ]
      ] ],
      [ "მეხსიერების მართვის ფუნქციები", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md215", [
        [ "synapse_payload_common_free", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md216", null ],
        [ "synapse_telemetry_payload_free", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md217", null ]
      ] ]
    ] ],
    [ "Synapse Health Monitor Module API Reference", "md_docs_2api__reference_2health__monitor__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md219", null ],
      [ "Health Monitor Module API", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md221", [
        [ "module_t *health_monitor_module_create(const cJSON *config);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md222", null ],
        [ "esp_err_t health_monitor_module_start(module_t *module);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md223", null ],
        [ "esp_err_t health_monitor_module_stop(module_t *module);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md224", null ],
        [ "esp_err_t health_monitor_module_get_status(module_t *module, health_status_t *status);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md225", null ],
        [ "esp_err_t health_monitor_module_register_check(module_t *module, health_check_fn_t check_fn);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md226", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md228", [
        [ "static void health_monitor_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md229", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md231", null ],
      [ "მაგალითები", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md233", null ]
    ] ],
    [ "Synapse Local Control Module API Reference", "md_docs_2api__reference_2local__ctrl__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md236", null ],
      [ "Local Control Module API", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md238", [
        [ "module_t *local_ctrl_module_create(const cJSON *config);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md239", null ],
        [ "esp_err_t local_ctrl_module_start(module_t *module);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md240", null ],
        [ "esp_err_t local_ctrl_module_stop(module_t *module);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md241", null ],
        [ "esp_err_t local_ctrl_module_register_endpoint(const char *uri, http_method_t method, local_ctrl_handler_t handler);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md242", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md244", [
        [ "static void local_ctrl_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md245", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md247", null ],
      [ "მაგალითები", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md249", null ]
    ] ],
    [ "Synapse Module API Reference", "md_docs_2api__reference_2module__api.html", [
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2module__api.html#autotoc_md252", [
        [ "<tt>module_t *{module_name}_create(const cJSON *config);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md253", null ],
        [ "<tt>static esp_err_t {module_name}_init(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md254", null ],
        [ "<tt>static esp_err_t {module_name}_start(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md255", null ],
        [ "<tt>static void {module_name}_deinit(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md256", null ]
      ] ],
      [ "Service API (Service Locator-ისთვის)", "md_docs_2api__reference_2module__api.html#autotoc_md258", [
        [ "<tt>esp_err_t {module_name}_api_{action}(...);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md259", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2module__api.html#autotoc_md261", [
        [ "<tt>static void {module_name}_handle_event(module_t *self, const char *event_name, void *event_data);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md262", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2module__api.html#autotoc_md264", [
        [ "<tt>static esp_err_t parse_{module_name}_config(const cJSON *config_node, {module_name}_private_data_t *p_data);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md265", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2module__api.html#autotoc_md267", [
        [ "<tt>static module_status_t {module_name}_get_status(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md268", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2module__api.html#autotoc_md270", [
        [ "<tt>static void {module_name}_debug_print(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md271", null ]
      ] ]
    ] ],
    [ "Synapse MQTT Manager Service API Reference", "md_docs_2api__reference_2mqtt__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2mqtt__api.html#autotoc_md274", null ],
      [ "MQTT Manager API", "md_docs_2api__reference_2mqtt__api.html#autotoc_md276", [
        [ "სამომავლო გაფართოება", "md_docs_2api__reference_2mqtt__api.html#autotoc_md277", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2mqtt__api.html#autotoc_md279", null ],
      [ "მაგალითები", "md_docs_2api__reference_2mqtt__api.html#autotoc_md281", null ]
    ] ],
    [ "Synapse OTA (Over-the-Air) Module API Reference", "md_docs_2api__reference_2ota__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2ota__api.html#autotoc_md284", null ],
      [ "OTA Module API", "md_docs_2api__reference_2ota__api.html#autotoc_md286", [
        [ "module_t *ota_module_create(const cJSON *config);", "md_docs_2api__reference_2ota__api.html#autotoc_md287", null ],
        [ "esp_err_t ota_module_start_update(const char *url);", "md_docs_2api__reference_2ota__api.html#autotoc_md288", null ],
        [ "esp_err_t ota_module_get_status(ota_status_t *status);", "md_docs_2api__reference_2ota__api.html#autotoc_md289", null ],
        [ "esp_err_t ota_module_abort(void);", "md_docs_2api__reference_2ota__api.html#autotoc_md290", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2ota__api.html#autotoc_md292", [
        [ "static void ota_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2ota__api.html#autotoc_md293", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2ota__api.html#autotoc_md295", null ],
      [ "მაგალითები", "md_docs_2api__reference_2ota__api.html#autotoc_md297", null ]
    ] ],
    [ "Synapse Promise Manager API Reference", "md_docs_2api__reference_2promise__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2promise__api.html#autotoc_md300", null ],
      [ "1. Consumer Types (მომხმარებლისთვის)", "md_docs_2api__reference_2promise__api.html#autotoc_md302", [
        [ "<tt>promise_then_cb</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md303", null ],
        [ "<tt>promise_catch_cb</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md304", null ]
      ] ],
      [ "2. Provider API (სერვისის მომწოდებლისთვის)", "md_docs_2api__reference_2promise__api.html#autotoc_md306", [
        [ "<tt>promise_handle_t synapse_promise_create(promise_then_cb then_cb, promise_catch_cb catch_cb, void* user_context);</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md307", null ],
        [ "<tt>esp_err_t synapse_promise_resolve(promise_handle_t handle, void* result_data, void (*free_fn)(void*));</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md308", null ],
        [ "<tt>esp_err_t synapse_promise_reject(promise_handle_t handle, void* error_data, void (*free_fn)(void*));</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md309", null ]
      ] ],
      [ "💡 გამოყენების მაგალითი", "md_docs_2api__reference_2promise__api.html#autotoc_md311", null ]
    ] ],
    [ "📲 Synapse Provisioning Module API Reference", "md_docs_2api__reference_2provisioning__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2provisioning__api.html#autotoc_md313", null ],
      [ "🔵 BLE Provisioning Service API (იმპლემენტირებულია)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md315", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2provisioning__api.html#autotoc_md316", null ]
      ] ],
      [ "📡 SoftAP Provisioning Module API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md318", [
        [ "module_t *softap_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md319", null ],
        [ "esp_err_t softap_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md320", null ],
        [ "esp_err_t softap_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md321", null ],
        [ "esp_err_t softap_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md322", null ],
        [ "esp_err_t softap_prov_module_get_status(module_t *module, softap_prov_status_t*status)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md323", null ]
      ] ],
      [ "🔳 QR Code Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md325", [
        [ "esp_err_t qr_code_prov_generate(const char *ssid, const char*password, char *qr_code_buffer, size_t buffer_size)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md326", null ],
        [ "esp_err_t qr_code_prov_display(const char *qr_code_buffer)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md327", null ]
      ] ],
      [ "🔌 Serial Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md329", [
        [ "module_t *serial_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md330", null ],
        [ "esp_err_t serial_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md331", null ],
        [ "esp_err_t serial_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md332", null ],
        [ "esp_err_t serial_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md333", null ]
      ] ],
      [ "☁️ Cloud Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md335", [
        [ "module_t *cloud_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md336", null ],
        [ "esp_err_t cloud_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md337", null ],
        [ "esp_err_t cloud_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md338", null ],
        [ "esp_err_t cloud_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md339", null ]
      ] ],
      [ "NFC Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md341", [
        [ "module_t *nfc_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md342", null ],
        [ "esp_err_t nfc_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md343", null ],
        [ "esp_err_t nfc_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md344", null ],
        [ "esp_err_t nfc_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md345", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2provisioning__api.html#autotoc_md347", null ],
      [ "მაგალითები", "md_docs_2api__reference_2provisioning__api.html#autotoc_md349", null ]
    ] ],
    [ "🔌 Relay Actuator API Reference", "md_docs_2api__reference_2relay__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2relay__api.html#autotoc_md352", null ],
      [ "1. ⌨️ CLI (Command Line Interface)", "md_docs_2api__reference_2relay__api.html#autotoc_md354", null ],
      [ "2. 📡 MQTT ინტერფეისი", "md_docs_2api__reference_2relay__api.html#autotoc_md356", [
        [ "ბრძანების გაგზავნა", "md_docs_2api__reference_2relay__api.html#autotoc_md357", null ],
        [ "სტატუსის მიღება", "md_docs_2api__reference_2relay__api.html#autotoc_md358", null ]
      ] ],
      [ "3. 📢 ივენთები (Events)", "md_docs_2api__reference_2relay__api.html#autotoc_md360", null ]
    ] ],
    [ "Synapse Resource Manager API Reference", "md_docs_2api__reference_2resource__manager__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md363", null ],
      [ "სამართავი რესურსების ტიპები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md365", null ],
      [ "API ფუნქციები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md367", [
        [ "<tt>esp_err_t synapse_resource_manager_init(void)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md368", null ],
        [ "<tt>esp_err_t synapse_resource_lock(synapse_resource_type_t type, uint8_t resource_id, const char *owner)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md370", null ],
        [ "<tt>esp_err_t synapse_resource_release(synapse_resource_type_t type, uint8_t resource_id, const char *owner)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md372", null ],
        [ "<tt>bool synapse_resource_is_locked(synapse_resource_type_t type, uint8_t resource_id)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md374", null ],
        [ "<tt>const char *synapse_resource_get_owner(synapse_resource_type_t type, uint8_t resource_id)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md376", null ]
      ] ],
      [ "გამოყენების მაგალითი", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md378", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md380", null ]
    ] ],
    [ "🛡️ Security Status API Reference", "md_docs_2api__reference_2security__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2security__api.html#autotoc_md383", null ],
      [ "1. 🔌 Service API (<tt>security_status_api_t</tt>)", "md_docs_2api__reference_2security__api.html#autotoc_md385", [
        [ "API ფუნქციები", "md_docs_2api__reference_2security__api.html#autotoc_md386", null ]
      ] ],
      [ "2. ⌨️ CLI (Command Line Interface)", "md_docs_2api__reference_2security__api.html#autotoc_md388", null ]
    ] ],
    [ "Synapse Sensor Module API Reference", "md_docs_2api__reference_2sensor__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2sensor__api.html#autotoc_md390", null ],
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2sensor__api.html#autotoc_md392", [
        [ "module_t <em>dht22_module_create(const cJSON *config);", "md_docs_2api__reference_2sensor__api.html#autotoc_md393", null ],
        [ "static esp_err_t dht22_init(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md394", null ],
        [ "static esp_err_t dht22_enable(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md395", null ],
        [ "static esp_err_t dht22_disable(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md396", null ],
        [ "static esp_err_t dht22_deinit(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md397", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2sensor__api.html#autotoc_md399", [
        [ "static void dht22_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2sensor__api.html#autotoc_md400", null ],
        [ "static esp_err_t publish_sensor_data(dht22_private_data_t *dht22_data, float temperature, float humidity);", "md_docs_2api__reference_2sensor__api.html#autotoc_md401", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2sensor__api.html#autotoc_md403", [
        [ "static esp_err_t parse_dht22_config(const cJSON *config, dht22_private_data_t *dht22_data);", "md_docs_2api__reference_2sensor__api.html#autotoc_md404", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2sensor__api.html#autotoc_md406", [
        [ "static module_status_t dht22_get_status(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md407", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2sensor__api.html#autotoc_md409", null ],
      [ "მაგალითები", "md_docs_2api__reference_2sensor__api.html#autotoc_md411", null ]
    ] ],
    [ "💾 Synapse Storage Manager API Reference", "md_docs_2api__reference_2storage__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2storage__api.html#autotoc_md414", null ],
      [ "⚙️ API-ს სტრუქტურა (<tt>storage_api_t</tt>)", "md_docs_2api__reference_2storage__api.html#autotoc_md416", [
        [ "🔑 Key-Value ოპერაციები", "md_docs_2api__reference_2storage__api.html#autotoc_md417", null ],
        [ "🗂️ ფაილური ოპერაციები", "md_docs_2api__reference_2storage__api.html#autotoc_md419", null ]
      ] ],
      [ "💡 გამოყენების მაგალითი", "md_docs_2api__reference_2storage__api.html#autotoc_md421", null ]
    ] ],
    [ "Synapse System Module API Reference", "md_docs_2api__reference_2system__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2system__api.html#autotoc_md424", null ],
      [ "OTA Module API", "md_docs_2api__reference_2system__api.html#autotoc_md426", [
        [ "module_t *ota_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md427", null ],
        [ "esp_err_t ota_module_start_update(const char *url);", "md_docs_2api__reference_2system__api.html#autotoc_md428", null ],
        [ "esp_err_t ota_module_get_status(ota_status_t *status);", "md_docs_2api__reference_2system__api.html#autotoc_md429", null ]
      ] ],
      [ "Version Module API", "md_docs_2api__reference_2system__api.html#autotoc_md431", [
        [ "module_t *version_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md432", null ],
        [ "esp_err_t version_module_get_firmware_version(char *version_str, size_t size);", "md_docs_2api__reference_2system__api.html#autotoc_md433", null ]
      ] ],
      [ "Watchdog Module API", "md_docs_2api__reference_2system__api.html#autotoc_md435", [
        [ "module_t *watchdog_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md436", null ],
        [ "esp_err_t watchdog_module_feed(void);", "md_docs_2api__reference_2system__api.html#autotoc_md437", null ],
        [ "esp_err_t watchdog_module_set_timeout(uint32_t timeout_ms);", "md_docs_2api__reference_2system__api.html#autotoc_md438", null ]
      ] ],
      [ "Diagnostics Module API", "md_docs_2api__reference_2system__api.html#autotoc_md440", [
        [ "module_t *diagnostics_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md441", null ],
        [ "esp_err_t diagnostics_module_run_tests(void);", "md_docs_2api__reference_2system__api.html#autotoc_md442", null ],
        [ "esp_err_t diagnostics_module_get_report(char *report_buffer, size_t buffer_size);", "md_docs_2api__reference_2system__api.html#autotoc_md443", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2system__api.html#autotoc_md445", null ],
      [ "მაგალითები", "md_docs_2api__reference_2system__api.html#autotoc_md447", null ]
    ] ],
    [ "Synapse Task Pool Manager API Reference", "md_docs_2api__reference_2task__pool__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2task__pool__api.html#autotoc_md450", null ],
      [ "API-ს ტიპები და სტრუქტურები", "md_docs_2api__reference_2task__pool__api.html#autotoc_md452", [
        [ "<tt>synapse_job_handle_t</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md453", null ],
        [ "<tt>synapse_job_cb</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md454", null ]
      ] ],
      [ "API-ს ფუნქციები", "md_docs_2api__reference_2task__pool__api.html#autotoc_md456", [
        [ "<tt>synapse_job_handle_t synapse_task_pool_schedule_job(synapse_job_cb job_function, void* user_context, uint32_t interval_ms, bool is_periodic);</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md457", null ],
        [ "<tt>esp_err_t synapse_task_pool_cancel_job(synapse_job_handle_t handle);</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md458", null ]
      ] ],
      [ "💡 გამოყენების მაგალითი", "md_docs_2api__reference_2task__pool__api.html#autotoc_md460", null ]
    ] ],
    [ "Synapse Time Sync Module API Reference", "md_docs_2api__reference_2time__sync__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2time__sync__api.html#autotoc_md462", null ],
      [ "Time Sync Module API", "md_docs_2api__reference_2time__sync__api.html#autotoc_md464", [
        [ "module_t *time_sync_module_create(const cJSON *config);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md465", null ],
        [ "esp_err_t time_sync_module_start(module_t *module);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md466", null ],
        [ "esp_err_t time_sync_module_stop(module_t *module);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md467", null ],
        [ "esp_err_t time_sync_module_set_server(module_t *module, const char *server_url);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md468", null ],
        [ "esp_err_t time_sync_module_get_time(time_t *current_time);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md469", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2time__sync__api.html#autotoc_md471", [
        [ "static void time_sync_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md472", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2time__sync__api.html#autotoc_md474", null ],
      [ "მაგალითები", "md_docs_2api__reference_2time__sync__api.html#autotoc_md476", null ]
    ] ],
    [ "Synapse Utility Module API Reference", "md_docs_2api__reference_2utility__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2utility__api.html#autotoc_md479", null ],
      [ "Logger Module API", "md_docs_2api__reference_2utility__api.html#autotoc_md481", [
        [ "module_t *logger_module_create(const cJSON *config);", "md_docs_2api__reference_2utility__api.html#autotoc_md482", null ],
        [ "esp_err_t logger_module_log_info(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md483", null ],
        [ "esp_err_t logger_module_log_warning(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md484", null ],
        [ "esp_err_t logger_module_log_error(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md485", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2utility__api.html#autotoc_md487", null ],
      [ "მაგალითები", "md_docs_2api__reference_2utility__api.html#autotoc_md489", null ]
    ] ],
    [ "Synapse Utils API Reference", "md_docs_2api__reference_2utils__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2utils__api.html#autotoc_md492", null ],
      [ "1. უსაფრთხო სტრიქონების მართვა", "md_docs_2api__reference_2utils__api.html#autotoc_md494", [
        [ "<tt>char* synapse_safe_strncpy(char *dest, const char *src, size_t size);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md495", null ]
      ] ],
      [ "2. კონფიგურაციის პარსინგის დამხმარე ფუნქციები", "md_docs_2api__reference_2utils__api.html#autotoc_md497", [
        [ "<tt>bool synapse_config_get_string_from_node(const char* tag, const cJSON *json_node, const char *key, char *dest, size_t dest_size);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md498", null ],
        [ "<tt>bool synapse_config_get_int_from_node(const char* tag, const cJSON *json_node, const char *key, int *out_value);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md499", null ],
        [ "<tt>bool synapse_config_get_bool_from_node(const char* tag, const cJSON *json_node, const char *key, bool *out_value);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md500", null ]
      ] ],
      [ "3. Guard Clause & Validation Helpers", "md_docs_2api__reference_2utils__api.html#autotoc_md501", [
        [ "<tt>SYNAPSE_GUARD(condition, tag, ret_val, format, ...)</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md502", null ],
        [ "<tt>SYNAPSE_GUARD_VOID(condition, tag, format, ...)</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md503", null ]
      ] ]
    ] ],
    [ "Synapse Version Module API Reference", "md_docs_2api__reference_2version__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2version__api.html#autotoc_md505", null ],
      [ "Version Module API", "md_docs_2api__reference_2version__api.html#autotoc_md507", [
        [ "module_t *version_module_create(const cJSON *config);", "md_docs_2api__reference_2version__api.html#autotoc_md508", null ],
        [ "esp_err_t version_module_get_firmware_version(char *version_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md509", null ],
        [ "esp_err_t version_module_get_build_date(char *date_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md510", null ],
        [ "esp_err_t version_module_get_build_time(char *time_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md511", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2version__api.html#autotoc_md513", null ],
      [ "მაგალითები", "md_docs_2api__reference_2version__api.html#autotoc_md515", null ]
    ] ],
    [ "Synapse WiFi Manager Service API Reference", "md_docs_2api__reference_2wifi__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2wifi__api.html#autotoc_md518", null ],
      [ "WiFi Manager API", "md_docs_2api__reference_2wifi__api.html#autotoc_md520", [
        [ "სამომავლო გაფართოება", "md_docs_2api__reference_2wifi__api.html#autotoc_md521", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2wifi__api.html#autotoc_md523", null ],
      [ "მაგალითები", "md_docs_2api__reference_2wifi__api.html#autotoc_md525", null ]
    ] ],
    [ "CHANGELOG: led_blinker ინიციალიზაციის პრობლემის დიაგნოსტიკა და გამოსწორება", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html", [
      [ "პრობლემა", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md528", null ],
      [ "დიაგნოსტიკა", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md529", null ],
      [ "მიზეზი", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md530", null ],
      [ "გამოსავალი", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md531", null ],
      [ "ცვლილებები კოდში", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md532", null ],
      [ "რეკომენდაცია", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md533", null ]
    ] ],
    [ "ცვლილებების ჟურნალი — 2025-06-24", "md_docs_2changelog_2CHANGELOG__2025-06-24.html", [
      [ "დამატებული და განახლებული ფაილები", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md536", [
        [ "[core/CMakeLists.txt]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md537", null ],
        [ "[core/collect_modules.cmake]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md538", null ],
        [ "[core/include/system_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md539", null ],
        [ "[interfaces/include/base_module.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md540", null ],
        [ "[core/include/service_locator.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md541", null ],
        [ "[core/include/module_registry.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md542", null ],
        [ "[core/include/event_bus.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md543", null ],
        [ "[core/include/config_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md544", null ],
        [ "[core/include/system_event_ids.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md545", null ],
        [ "[core/include/resource_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md546", null ],
        [ "[core/include/framework_config.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md547", null ],
        [ "[interfaces/include/logging.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md548", null ],
        [ "[interfaces/include/service_types.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md549", null ],
        [ "[interfaces/include/telemetry_events.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md550", null ],
        [ "[README.md]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md551", null ],
        [ "[.gitignore]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md552", null ]
      ] ],
      [ "ძირითადი ცვლილებები", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md554", null ]
    ] ],
    [ "CHANGELOG - 2025-06-26", "md_docs_2changelog_2CHANGELOG__2025-06-26.html", [
      [ "[Major] Module System Automation & Refactor", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md557", [
        [ "Added", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md558", null ],
        [ "Changed", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md559", null ],
        [ "Fixed", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md560", null ],
        [ "Detailed File Changes", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md561", null ],
        [ "Why/How", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md562", null ],
        [ "Testing", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md563", null ]
      ] ]
    ] ],
    [ "Changelog", "md_docs_2changelog_2CHANGELOG__2025-06-28.html", [
      [ "[2.0.0] - 2025-06-28", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md565", [
        [ "დამატებულია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md566", null ],
        [ "შეცვლილია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md567", null ],
        [ "ცნობილი პრობლემები", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md568", null ]
      ] ]
    ] ],
    [ "CHANGELOG", "md_docs_2changelog_2v10.html", [
      [ "[10.3.1] - 2025-09-17", "md_docs_2changelog_2v10.html#autotoc_md570", [
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v10.html#autotoc_md571", null ]
      ] ],
      [ "[10.3.0] - 2025-09-15", "md_docs_2changelog_2v10.html#autotoc_md573", [
        [ "✨ დამატებული ფუნქციონალი და DX (Added Features & Developer Experience)", "md_docs_2changelog_2v10.html#autotoc_md574", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v10.html#autotoc_md575", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v10.html#autotoc_md576", null ]
      ] ],
      [ "[10.2.0] - 2025-09-14", "md_docs_2changelog_2v10.html#autotoc_md578", [
        [ "✨ დამატებული ფუნქციონალი და DX (Added Features & Developer Experience)", "md_docs_2changelog_2v10.html#autotoc_md579", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v10.html#autotoc_md580", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v10.html#autotoc_md581", null ]
      ] ],
      [ "[10.1.0] - 2025-09-13", "md_docs_2changelog_2v10.html#autotoc_md583", [
        [ "✨ დამატებული ფუნქციონალი და DX (Added Features & Developer Experience)", "md_docs_2changelog_2v10.html#autotoc_md584", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v10.html#autotoc_md585", null ]
      ] ],
      [ "[10.0.0] - 2025-09-12", "md_docs_2changelog_2v10.html#autotoc_md587", [
        [ "💥 არქიტექტურული რევოლუცია (Breaking Architectural Changes)", "md_docs_2changelog_2v10.html#autotoc_md588", null ],
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v10.html#autotoc_md589", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed & Improved)", "md_docs_2changelog_2v10.html#autotoc_md590", null ],
        [ "🗑️ მოძველებული (Deprecated)", "md_docs_2changelog_2v10.html#autotoc_md591", null ]
      ] ]
    ] ],
    [ "ცვლილებების ისტორია (Changelog)", "md_docs_2changelog_2v5.html", [
      [ "[5.8.0] - 2025-07-31", "md_docs_2changelog_2v5.html#autotoc_md594", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md595", null ],
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md596", null ]
      ] ],
      [ "[5.7.0] - 2025-07-30", "md_docs_2changelog_2v5.html#autotoc_md598", [
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md599", null ]
      ] ],
      [ "[5.6.0] - 2025-08-16", "md_docs_2changelog_2v5.html#autotoc_md600", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md601", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md602", null ]
      ] ],
      [ "[5.5.1] - 2025-08-16", "md_docs_2changelog_2v5.html#autotoc_md603", [
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md604", null ]
      ] ],
      [ "[5.5.0] - 2025-08-15", "md_docs_2changelog_2v5.html#autotoc_md606", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md607", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md608", null ],
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md609", null ]
      ] ],
      [ "[5.4.0] - 2025-08-14", "md_docs_2changelog_2v5.html#autotoc_md611", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md612", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md613", null ],
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md614", null ]
      ] ],
      [ "[5.3.0] - 2025-07-29", "md_docs_2changelog_2v5.html#autotoc_md615", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md616", null ],
        [ "🔄 Changed (შეცვლილი და გაუმჯობესებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md617", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md618", null ]
      ] ],
      [ "[5.2.0] - 2025-07-28", "md_docs_2changelog_2v5.html#autotoc_md620", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md621", null ],
        [ "🔄 Changed (შეცვლილი და გაუმჯობესებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md622", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md623", null ]
      ] ],
      [ "[5.1.0] - 2025-07-28", "md_docs_2changelog_2v5.html#autotoc_md625", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md626", null ],
        [ "🔄 Changed (შეცვლილი და გაუმჯობესებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md627", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md628", null ]
      ] ],
      [ "[5.0.1] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md630", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md631", null ],
        [ "🔄 Changed (შეცვლილი ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md632", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md633", null ]
      ] ],
      [ "[5.0.0] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md634", [
        [ "💥 Breaking Changes", "md_docs_2changelog_2v5.html#autotoc_md635", null ],
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md636", null ],
        [ "🔄 Changed (შეცვლილი ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md637", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md638", null ]
      ] ],
      [ "[4.2.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md639", [
        [ "✨ დამატებულია (Added)", "md_docs_2changelog_2v5.html#autotoc_md640", null ],
        [ "🔄 შეცვლილია (Changed)", "md_docs_2changelog_2v5.html#autotoc_md641", null ],
        [ "🐞 გასწორებულია (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md642", null ]
      ] ],
      [ "[v4.1.0] - 2025-07-19", "md_docs_2changelog_2v5.html#autotoc_md643", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md644", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md645", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md646", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md647", null ]
      ] ],
      [ "[v4.0.0] - 2025-07-19", "md_docs_2changelog_2v5.html#autotoc_md648", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md649", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md650", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md651", null ]
      ] ],
      [ "[v3.9.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md652", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md653", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md654", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md655", null ]
      ] ],
      [ "[v3.8.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md656", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md657", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md658", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md659", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md660", null ]
      ] ],
      [ "[v3.7.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md661", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md662", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md663", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md664", null ]
      ] ],
      [ "[v3.6.0] - 2025-07-17", "md_docs_2changelog_2v5.html#autotoc_md665", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md666", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md667", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md668", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md669", null ]
      ] ],
      [ "[v3.5.0] - 2025-07-16", "md_docs_2changelog_2v5.html#autotoc_md670", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md671", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md672", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md673", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md674", null ]
      ] ],
      [ "[v3.4.0] - 2025-07-13", "md_docs_2changelog_2v5.html#autotoc_md675", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md676", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md677", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md678", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md679", null ]
      ] ],
      [ "[v3.3.0] - 2025-07-10", "md_docs_2changelog_2v5.html#autotoc_md680", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md681", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md682", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md683", null ]
      ] ],
      [ "[v3.2.1] - 2025-07-08", "md_docs_2changelog_2v5.html#autotoc_md684", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md685", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md686", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md687", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md688", null ]
      ] ],
      [ "[v3.2.0] - 2025-07-08", "md_docs_2changelog_2v5.html#autotoc_md689", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md690", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md691", null ]
      ] ],
      [ "[v3.1.0] - 2025-07-06", "md_docs_2changelog_2v5.html#autotoc_md692", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md693", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md694", null ]
      ] ],
      [ "[v3.0.0] - 2025-07-05", "md_docs_2changelog_2v5.html#autotoc_md695", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md696", null ],
        [ "♻️ რეფაქტორინგი და გაუმჯობესებები (Refactoring & Improvements)", "md_docs_2changelog_2v5.html#autotoc_md697", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md698", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md699", null ]
      ] ],
      [ "[2.8.0] - 2025-07-05", "md_docs_2changelog_2v5.html#autotoc_md700", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md701", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md702", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md703", null ]
      ] ],
      [ "[2.7.0] - 2025-07-05", "md_docs_2changelog_2v5.html#autotoc_md704", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md705", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md706", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md707", null ]
      ] ],
      [ "[2.6.0] - 2025-07-04", "md_docs_2changelog_2v5.html#autotoc_md708", [
        [ "✨ დამატებულია", "md_docs_2changelog_2v5.html#autotoc_md709", null ],
        [ "🛠️ შეცვლილია", "md_docs_2changelog_2v5.html#autotoc_md710", null ],
        [ "🐛 გასწორებულია", "md_docs_2changelog_2v5.html#autotoc_md711", null ]
      ] ],
      [ "[v2.5.0] - 2025-07-02", "md_docs_2changelog_2v5.html#autotoc_md712", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md713", null ]
      ] ],
      [ "[v2.4.0] - 2025-07-02", "md_docs_2changelog_2v5.html#autotoc_md714", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md715", null ],
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md716", null ]
      ] ],
      [ "[v2.3.0] - 2025-07-02", "md_docs_2changelog_2v5.html#autotoc_md718", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md719", null ],
        [ "🏗️ ინფრასტრუქტურა", "md_docs_2changelog_2v5.html#autotoc_md720", null ]
      ] ],
      [ "[v2.2.0] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md721", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md722", null ],
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md723", null ]
      ] ],
      [ "[v2.1.1] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md724", [
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md725", null ],
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md726", null ]
      ] ],
      [ "[v2.1.0] - 2025-06-30", "md_docs_2changelog_2v5.html#autotoc_md728", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md729", null ],
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md730", null ],
        [ "♻️ რეფაქტორინგი", "md_docs_2changelog_2v5.html#autotoc_md731", null ]
      ] ]
    ] ],
    [ "CHANGELOG", "md_docs_2changelog_2v6.html", [
      [ "[6.2.0] - 2025-08-24", "md_docs_2changelog_2v6.html#autotoc_md733", [
        [ "✨ დამატებული", "md_docs_2changelog_2v6.html#autotoc_md734", null ],
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v6.html#autotoc_md735", null ]
      ] ],
      [ "[6.1.0] - 2025-08-22", "md_docs_2changelog_2v6.html#autotoc_md737", null ],
      [ "✨ დამატებული", "md_docs_2changelog_2v6.html#autotoc_md738", [
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v6.html#autotoc_md739", null ],
        [ "📄 დოკუმენტაცია", "md_docs_2changelog_2v6.html#autotoc_md740", null ]
      ] ],
      [ "[6.0.0] - 2025-08-21", "md_docs_2changelog_2v6.html#autotoc_md742", [
        [ "✨ დამატებული", "md_docs_2changelog_2v6.html#autotoc_md743", null ],
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v6.html#autotoc_md744", null ],
        [ "📄 დოკუმენტაცია", "md_docs_2changelog_2v6.html#autotoc_md745", null ]
      ] ]
    ] ],
    [ "[7.2.0] - 2025-08-28", "md_docs_2changelog_2v7.html", [
      [ "[7.1.0] - 2025-08-27", "md_docs_2changelog_2v7.html#autotoc_md753", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v7.html#autotoc_md748", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v7.html#autotoc_md749", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v7.html#autotoc_md750", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v7.html#autotoc_md751", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v7.html#autotoc_md754", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v7.html#autotoc_md755", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v7.html#autotoc_md756", null ]
      ] ],
      [ "[7.0.0] - 2025-08-26", "md_docs_2changelog_2v7.html#autotoc_md758", [
        [ "💥 არქიტექტურული ცვლილებები (Breaking Changes & Refactoring)", "md_docs_2changelog_2v7.html#autotoc_md759", null ],
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v7.html#autotoc_md760", [
          [ "ძველი ვერსიების ისტორია", "md_docs_2changelog_2v7.html#autotoc_md762", null ]
        ] ]
      ] ]
    ] ],
    [ "[8.1.0] - 2025-08-30", "md_docs_2changelog_2v8.html", [
      [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v8.html#autotoc_md764", [
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v8.html#autotoc_md765", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v8.html#autotoc_md766", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v8.html#autotoc_md767", null ]
      ] ],
      [ "[8.0.0] - 2025-08-07", "md_docs_2changelog_2v8.html#autotoc_md769", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v8.html#autotoc_md770", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v8.html#autotoc_md771", null ]
      ] ]
    ] ],
    [ "CHANGELOG", "md_docs_2changelog_2v9.html", [
      [ "[9.5.3] - 2025-09-11", "md_docs_2changelog_2v9.html#autotoc_md774", [
        [ "✨ არქიტექტურული გაუმჯობესებები და DX (Architectural & Developer Experience Improvements)", "md_docs_2changelog_2v9.html#autotoc_md775", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v9.html#autotoc_md776", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md777", null ]
      ] ],
      [ "[9.5.2] - 2025-08-09", "md_docs_2changelog_2v9.html#autotoc_md779", [
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v9.html#autotoc_md780", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md781", null ]
      ] ],
      [ "[9.5.0] - 2025-08-09", "md_docs_2changelog_2v9.html#autotoc_md783", [
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v9.html#autotoc_md784", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md785", null ]
      ] ],
      [ "[9.4.0] - 2025-08-08", "md_docs_2changelog_2v9.html#autotoc_md787", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v9.html#autotoc_md788", null ]
      ] ],
      [ "[9.3.0] - 2025-08-08", "md_docs_2changelog_2v9.html#autotoc_md790", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v9.html#autotoc_md791", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v9.html#autotoc_md792", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v9.html#autotoc_md793", null ]
      ] ],
      [ "[9.2.0] - 2025-08-08", "md_docs_2changelog_2v9.html#autotoc_md795", [
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v9.html#autotoc_md796", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md797", null ]
      ] ],
      [ "[9.1.0] - 2025-08-08", "md_docs_2changelog_2v9.html#autotoc_md799", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v9.html#autotoc_md800", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md801", null ]
      ] ],
      [ "[9.0.0] - 2025-08-07", "md_docs_2changelog_2v9.html#autotoc_md803", [
        [ "💥 არქიტექტურული ცვლილებები (Breaking Changes & Refactoring)", "md_docs_2changelog_2v9.html#autotoc_md804", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md805", null ]
      ] ]
    ] ],
    [ "Synapse Branching Strategy", "md_docs_2contributing_2branching__strategy.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2branching__strategy.html#autotoc_md809", null ],
      [ "ძირითადი ბრენჩები", "md_docs_2contributing_2branching__strategy.html#autotoc_md811", null ],
      [ "ბრენჩის სახელდების წესები", "md_docs_2contributing_2branching__strategy.html#autotoc_md813", null ],
      [ "სამუშაო პროცესი", "md_docs_2contributing_2branching__strategy.html#autotoc_md815", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2contributing_2branching__strategy.html#autotoc_md817", null ],
      [ "რესურსები", "md_docs_2contributing_2branching__strategy.html#autotoc_md819", null ]
    ] ],
    [ "ქცევის კოდექსი (Code of Conduct)", "md_docs_2contributing_2code__of__conduct.html", [
      [ "მიზანი", "md_docs_2contributing_2code__of__conduct.html#autotoc_md823", null ],
      [ "ძირითადი პრინციპები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md825", null ],
      [ "დაუშვებელი ქცევა", "md_docs_2contributing_2code__of__conduct.html#autotoc_md827", null ],
      [ "პასუხისმგებლობები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md829", null ],
      [ "დარღვევის შემთხვევაში მოქმედება", "md_docs_2contributing_2code__of__conduct.html#autotoc_md831", null ],
      [ "დამატებითი რესურსები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md833", null ]
    ] ],
    [ "Synapse Commit Message Guidelines", "md_docs_2contributing_2commit__message__guidelines.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md837", null ],
      [ "ძირითადი სტრუქტურა", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md839", null ],
      [ "სათაურის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md841", null ],
      [ "დეტალური აღწერის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md843", null ],
      [ "Footer-ის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md845", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md847", null ],
      [ "რესურსები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md849", null ]
    ] ],
    [ "კონტრიბუციის სახელმძღვანელო (Contributing Guide)", "md_docs_2contributing_2contributing__guide.html", [
      [ "მიზანი", "md_docs_2contributing_2contributing__guide.html#autotoc_md852", null ],
      [ "1. კონტრიბუციის პროცესი", "md_docs_2contributing_2contributing__guide.html#autotoc_md854", null ],
      [ "2. კოდის სტანდარტები", "md_docs_2contributing_2contributing__guide.html#autotoc_md855", null ],
      [ "3. დოკუმენტაციის მოთხოვნები", "md_docs_2contributing_2contributing__guide.html#autotoc_md856", null ],
      [ "4. Pull Request-ის მიმოხილვა", "md_docs_2contributing_2contributing__guide.html#autotoc_md857", null ],
      [ "5. Best Practices", "md_docs_2contributing_2contributing__guide.html#autotoc_md858", null ],
      [ "6. აკრძალული პრაქტიკები", "md_docs_2contributing_2contributing__guide.html#autotoc_md859", null ]
    ] ],
    [ "Synapse Contributing Reference Index", "md_docs_2contributing_2contributing__index.html", [
      [ "ძირითადი სახელმძღვანელოები", "md_docs_2contributing_2contributing__index.html#autotoc_md863", null ],
      [ "შაბლონები", "md_docs_2contributing_2contributing__index.html#autotoc_md864", null ],
      [ "ინსტრუმენტები", "md_docs_2contributing_2contributing__index.html#autotoc_md865", null ]
    ] ],
    [ "კონვენციების ავტომატური შემმოწმებელი (<tt>check_conventions.py</tt>)", "md_docs_2contributing_2convention__linter__guide.html", [
      [ "1. შესავალი", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md867", null ],
      [ "2. რას ამოწმებს სკრიპტი?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md868", null ],
      [ "3. როგორ გამოვიყენოთ ლოკალურად?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md873", null ],
      [ "4. ინტეგრაცია GitHub Actions-თან (ავტომატური შემოწმება)", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md875", null ],
      [ "5. როგორ გამოვასწოროთ გავრცელებული შეცდომები?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md876", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (FAQ)", "md_docs_2contributing_2faq.html", [
      [ "1. როგორ დავიწყო კონტრიბუცია Synapse Framework-ში?", "md_docs_2contributing_2faq.html#autotoc_md879", null ],
      [ "2. რა სტანდარტებს და კონვენციებს უნდა დავიცვა?", "md_docs_2contributing_2faq.html#autotoc_md880", null ],
      [ "3. როგორ შევქმნა Pull Request?", "md_docs_2contributing_2faq.html#autotoc_md881", null ],
      [ "4. როგორ მოვამზადო ლოკალური გარემო?", "md_docs_2contributing_2faq.html#autotoc_md882", null ],
      [ "5. როგორ გავუშვა ტესტები?", "md_docs_2contributing_2faq.html#autotoc_md883", null ],
      [ "6. როგორ მოვაგვარო გარემოს ან ბილდის პრობლემები?", "md_docs_2contributing_2faq.html#autotoc_md884", null ],
      [ "7. სად ვიპოვო დოკუმენტაციის სტრუქტურის და სტანდარტების შესახებ ინფორმაცია?", "md_docs_2contributing_2faq.html#autotoc_md885", null ],
      [ "8. როგორ დავამატო ან განვაახლო დოკუმენტაცია?", "md_docs_2contributing_2faq.html#autotoc_md886", null ],
      [ "9. როგორ დავიცვა ქცევის კოდექსი?", "md_docs_2contributing_2faq.html#autotoc_md887", null ],
      [ "10. ვის მივმართო დამატებითი კითხვების ან პრობლემების შემთხვევაში?", "md_docs_2contributing_2faq.html#autotoc_md888", null ]
    ] ],
    [ "Synapse Issue Template", "md_docs_2contributing_2issue__template.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2issue__template.html#autotoc_md892", null ],
      [ "ძირითადი ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md894", null ],
      [ "დეტალური აღწერა", "md_docs_2contributing_2issue__template.html#autotoc_md896", null ],
      [ "გამეორების ნაბიჯები (თუ ეს ბაგია)", "md_docs_2contributing_2issue__template.html#autotoc_md898", null ],
      [ "მოსალოდნელი შედეგი", "md_docs_2contributing_2issue__template.html#autotoc_md900", null ],
      [ "ფაქტობრივი შედეგი", "md_docs_2contributing_2issue__template.html#autotoc_md902", null ],
      [ "გარემოს ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md904", null ],
      [ "დამატებითი ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md906", null ],
      [ "რესურსები", "md_docs_2contributing_2issue__template.html#autotoc_md908", null ]
    ] ],
    [ "Synapse Local Setup Guide", "md_docs_2contributing_2local__setup.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2local__setup.html#autotoc_md912", null ],
      [ "სისტემური მოთხოვნები", "md_docs_2contributing_2local__setup.html#autotoc_md914", null ],
      [ "ESP-IDF-ის ინსტალაცია", "md_docs_2contributing_2local__setup.html#autotoc_md916", null ],
      [ "პროექტის კლონირება", "md_docs_2contributing_2local__setup.html#autotoc_md918", null ],
      [ "sdkconfig ფაილის კონფიგურაცია", "md_docs_2contributing_2local__setup.html#autotoc_md920", null ],
      [ "პროექტის აგება და გაშვება", "md_docs_2contributing_2local__setup.html#autotoc_md922", null ],
      [ "ტესტირება", "md_docs_2contributing_2local__setup.html#autotoc_md924", null ],
      [ "პრობლემების შემთხვევაში", "md_docs_2contributing_2local__setup.html#autotoc_md926", null ],
      [ "რესურსები", "md_docs_2contributing_2local__setup.html#autotoc_md928", null ]
    ] ],
    [ "მოდულების მართვა Git Submodule-ის საშუალებით", "md_docs_2contributing_2managing__modules.html", [
      [ "1. 🏛️ არქიტექტურული პრინციპი", "md_docs_2contributing_2managing__modules.html#autotoc_md931", null ],
      [ "2. 🚀 ახალი მოდულის დამატება", "md_docs_2contributing_2managing__modules.html#autotoc_md932", [
        [ "ეტაპი 1: მოდულის რეპოზიტორიის მომზადება", "md_docs_2contributing_2managing__modules.html#autotoc_md933", null ],
        [ "ეტაპი 2: Submodule-ის დამატება მთავარ პროექტში", "md_docs_2contributing_2managing__modules.html#autotoc_md934", null ],
        [ "⚙️ Build სისტემასთან ინტეგრაცია", "md_docs_2contributing_2managing__modules.html#autotoc_md935", null ]
      ] ],
      [ "3. 🔄 პროექტის კლონირება Submodule-ებთან ერთად", "md_docs_2contributing_2managing__modules.html#autotoc_md936", null ],
      [ "4. 📈 Submodule-ის განახლება", "md_docs_2contributing_2managing__modules.html#autotoc_md937", null ],
      [ "5. 💡 დამატებითი რჩევები", "md_docs_2contributing_2managing__modules.html#autotoc_md938", null ]
    ] ],
    [ "Synapse Pull Request Template", "md_docs_2contributing_2pull__request__template.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2pull__request__template.html#autotoc_md941", null ],
      [ "ძირითადი ინფორმაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md943", null ],
      [ "ცვლილებების დეტალური აღწერა", "md_docs_2contributing_2pull__request__template.html#autotoc_md945", null ],
      [ "ტესტირება და ვალიდაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md947", null ],
      [ "მიმოხილვის რეკომენდაციები", "md_docs_2contributing_2pull__request__template.html#autotoc_md949", null ],
      [ "დამატებითი ინფორმაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md951", null ],
      [ "რესურსები", "md_docs_2contributing_2pull__request__template.html#autotoc_md953", null ]
    ] ],
    [ "Synapse Review Guidelines", "md_docs_2contributing_2review__guidelines.html", [
      [ "მიმოხილვის მიზანი", "md_docs_2contributing_2review__guidelines.html#autotoc_md957", null ],
      [ "მიმოხილვის პროცესი", "md_docs_2contributing_2review__guidelines.html#autotoc_md959", null ],
      [ "მიმოხილვის კრიტერიუმები", "md_docs_2contributing_2review__guidelines.html#autotoc_md961", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2contributing_2review__guidelines.html#autotoc_md963", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2contributing_2review__guidelines.html#autotoc_md965", null ],
      [ "რესურსები", "md_docs_2contributing_2review__guidelines.html#autotoc_md967", null ]
    ] ],
    [ "Assertions და Guard Clauses კონვენცია", "md_docs_2convention_2assertion__and__guards.html", [
      [ "1. 🎯 მიზანი", "md_docs_2convention_2assertion__and__guards.html#autotoc_md970", null ],
      [ "2. 🤔 ფუნდამენტური განსხვავება: Assertion vs. Error Handling", "md_docs_2convention_2assertion__and__guards.html#autotoc_md971", null ],
      [ "3. <tt>SYNAPSE_ASSERT</tt> მაკრო", "md_docs_2convention_2assertion__and__guards.html#autotoc_md972", [
        [ "3.1. როდის გამოვიყენოთ?", "md_docs_2convention_2assertion__and__guards.html#autotoc_md973", null ],
        [ "3.2. გამოყენების მაგალითები", "md_docs_2convention_2assertion__and__guards.html#autotoc_md974", null ],
        [ "3.3. ქცევა <tt>release</tt> რეჟიმში", "md_docs_2convention_2assertion__and__guards.html#autotoc_md975", null ]
      ] ],
      [ "4. <tt>SYNAPSE_GUARD</tt> მაკროები", "md_docs_2convention_2assertion__and__guards.html#autotoc_md976", [
        [ "4.1. როდის გამოვიყენოთ?", "md_docs_2convention_2assertion__and__guards.html#autotoc_md977", null ],
        [ "4.2. გამოყენების მაგალითები", "md_docs_2convention_2assertion__and__guards.html#autotoc_md978", null ],
        [ "4.3. როდის <strong>არ</strong> გამოვიყენოთ <tt>SYNAPSE_GUARD</tt>?", "md_docs_2convention_2assertion__and__guards.html#autotoc_md979", null ]
      ] ]
    ] ],
    [ "9. Doxygen კომენტარების სტანდარტი (v1.0)", "md_docs_2convention_2coding__doxygen__standard.html", [
      [ "1. შესავალი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md981", null ],
      [ "2. ფაილის სათაურის (Header) კომენტარი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md982", [
        [ "2.1. სტრუქტურა", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md983", null ],
        [ "2.2. მაგალითი (<tt>i2c_bus_module.h</tt>)", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md984", null ]
      ] ],
      [ "3. ფუნქციის კომენტარი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md985", [
        [ "3.1. სტრუქტურა", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md986", null ],
        [ "3.2. მაგალითი (<tt>service_locator.h</tt>)", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md987", null ]
      ] ],
      [ "4. სტრუქტურების და ტიპების კომენტარები", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md988", [
        [ "4.1. სტრუქტურის მაგალითი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md989", null ]
      ] ],
      [ "5. Doxygen ტეგების გამოყენების წესები", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md990", null ]
    ] ],
    [ "კომუნიკაციის პატერნების წესები (v2.0)", "md_docs_2convention_2communication__patterns.html", [
      [ "ძირითადი პრინციპი: მაქსიმალური იზოლაცია და პროგნოზირებადი გაშვება", "md_docs_2convention_2communication__patterns.html#autotoc_md992", null ],
      [ "ოთხი ძირითადი პატერნი", "md_docs_2convention_2communication__patterns.html#autotoc_md993", [
        [ "1. Service Locator Pattern (State-Aware)", "md_docs_2convention_2communication__patterns.html#autotoc_md994", null ],
        [ "2. Event Bus Pattern", "md_docs_2convention_2communication__patterns.html#autotoc_md995", null ],
        [ "3. Command Router Pattern", "md_docs_2convention_2communication__patterns.html#autotoc_md996", null ],
        [ "4. Promise Pattern (ახალი)", "md_docs_2convention_2communication__patterns.html#autotoc_md997", null ]
      ] ],
      [ "კომუნიკაციის ტიპები", "md_docs_2convention_2communication__patterns.html#autotoc_md998", [
        [ "Direct API Communication (Service Locator)", "md_docs_2convention_2communication__patterns.html#autotoc_md999", null ],
        [ "Broadcast Communication (Event Bus)", "md_docs_2convention_2communication__patterns.html#autotoc_md1000", null ]
      ] ],
      [ "აკრძალული პატერნები", "md_docs_2convention_2communication__patterns.html#autotoc_md1001", [
        [ "❌ პირდაპირი დამოკიდებულებები", "md_docs_2convention_2communication__patterns.html#autotoc_md1002", null ],
        [ "❌ პატერნების არასწორი შერევა", "md_docs_2convention_2communication__patterns.html#autotoc_md1003", null ],
        [ "❌ System Manager MQTT Handling", "md_docs_2convention_2communication__patterns.html#autotoc_md1004", null ]
      ] ],
      [ "სწორი იმპლემენტაცია", "md_docs_2convention_2communication__patterns.html#autotoc_md1005", [
        [ "Service Locator-ის გამოყენება (ახალი პატერნი)", "md_docs_2convention_2communication__patterns.html#autotoc_md1006", [
          [ "1. Service Registration (<tt>_create</tt> ფუნქციაში)", "md_docs_2convention_2communication__patterns.html#autotoc_md1007", null ],
          [ "2. Service Usage (სხვა მოდულში)", "md_docs_2convention_2communication__patterns.html#autotoc_md1008", null ]
        ] ],
        [ "Event Bus-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md1009", [
          [ "1. Event Publishing (Sensor Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md1010", null ],
          [ "2. Event Subscription (MQTT Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md1011", null ]
        ] ]
      ] ],
      [ "დინამიური MQTT თემები <tt>{module_name}</tt> Placeholder-ით", "md_docs_2convention_2communication__patterns.html#autotoc_md1012", [
        [ "პატერნის აღწერა", "md_docs_2convention_2communication__patterns.html#autotoc_md1013", null ]
      ] ],
      [ "პატერნის არჩევის გზამკვლევი", "md_docs_2convention_2communication__patterns.html#autotoc_md1014", [
        [ "Service Locator-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md1015", null ],
        [ "Event Bus-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md1016", null ]
      ] ],
      [ "Architecture Diagram", "md_docs_2convention_2communication__patterns.html#autotoc_md1017", null ],
      [ "შეჯამება", "md_docs_2convention_2communication__patterns.html#autotoc_md1018", null ]
    ] ],
    [ "კონფიგურაციის მართვის კონვენცია", "md_docs_2convention_2configuration__management.html", [
      [ "1. 📜 ძირითადი პრინციპი", "md_docs_2convention_2configuration__management.html#autotoc_md1020", null ],
      [ "2. 📂 კონფიგურაციის წყაროები და იერარქია", "md_docs_2convention_2configuration__management.html#autotoc_md1021", null ],
      [ "3. 🏗️ კონფიგურაციის სტრუქტურა", "md_docs_2convention_2configuration__management.html#autotoc_md1022", null ],
      [ "4. 📝 კონფიგურაციის პარსინგის პატერნი", "md_docs_2convention_2configuration__management.html#autotoc_md1023", [
        [ "<tt>parse_config</tt> ფუნქციის 3-ნაბიჯიანი ლოგიკა", "md_docs_2convention_2configuration__management.html#autotoc_md1024", null ],
        [ "<tt>parse_config</tt> ფუნქციის მაგალითი", "md_docs_2convention_2configuration__management.html#autotoc_md1025", null ]
      ] ],
      [ "5. 🌐 კონფიგურაციაზე წვდომა სხვა მოდულებიდან", "md_docs_2convention_2configuration__management.html#autotoc_md1026", null ],
      [ "6. ❌ აკრძალული პრაქტიკები", "md_docs_2convention_2configuration__management.html#autotoc_md1027", null ],
      [ "7. ✅ საუკეთესო პრაქტიკები", "md_docs_2convention_2configuration__management.html#autotoc_md1028", null ]
    ] ],
    [ "Synapse Convention Reference Index", "md_docs_2convention_2convention__index.html", [
      [ "Coding & Documentation", "md_docs_2convention_2convention__index.html#autotoc_md1031", null ],
      [ "Naming & Structure", "md_docs_2convention_2convention__index.html#autotoc_md1032", null ],
      [ "Patterns & Management", "md_docs_2convention_2convention__index.html#autotoc_md1033", null ]
    ] ],
    [ "Error Handling-ის წესები", "md_docs_2convention_2error__handling.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2error__handling.html#autotoc_md1036", [
        [ "1. ESP Error Codes გამოყენება", "md_docs_2convention_2error__handling.html#autotoc_md1037", null ],
        [ "2. Comprehensive Error Checking", "md_docs_2convention_2error__handling.html#autotoc_md1038", null ]
      ] ],
      [ "Standard Error Codes", "md_docs_2convention_2error__handling.html#autotoc_md1039", [
        [ "Framework-Specific Errors", "md_docs_2convention_2error__handling.html#autotoc_md1040", null ],
        [ "Common ESP Error Codes", "md_docs_2convention_2error__handling.html#autotoc_md1041", null ]
      ] ],
      [ "Error Handling Patterns", "md_docs_2convention_2error__handling.html#autotoc_md1042", [
        [ "1. Parameter Validation", "md_docs_2convention_2error__handling.html#autotoc_md1043", null ],
        [ "2. Resource Allocation Checking", "md_docs_2convention_2error__handling.html#autotoc_md1044", null ],
        [ "3. Service Locator Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md1045", null ],
        [ "4. Configuration Parsing Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md1046", null ],
        [ "5. Hardware Operation Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md1047", null ]
      ] ],
      [ "Cleanup and Resource Management", "md_docs_2convention_2error__handling.html#autotoc_md1048", [
        [ "1. Proper Cleanup on Errors", "md_docs_2convention_2error__handling.html#autotoc_md1049", null ],
        [ "2. Resource Cleanup Functions", "md_docs_2convention_2error__handling.html#autotoc_md1050", null ]
      ] ],
      [ "Logging Standards", "md_docs_2convention_2error__handling.html#autotoc_md1051", [
        [ "1. Error Levels", "md_docs_2convention_2error__handling.html#autotoc_md1052", null ],
        [ "2. Error Context Information", "md_docs_2convention_2error__handling.html#autotoc_md1053", null ]
      ] ],
      [ "Error Recovery Strategies", "md_docs_2convention_2error__handling.html#autotoc_md1054", [
        [ "1. Retry Logic", "md_docs_2convention_2error__handling.html#autotoc_md1055", null ],
        [ "2. Graceful Degradation", "md_docs_2convention_2error__handling.html#autotoc_md1056", null ]
      ] ],
      [ "შეჯამება", "md_docs_2convention_2error__handling.html#autotoc_md1057", null ]
    ] ],
    [ "ფუნქციების დასახელების წესები", "md_docs_2convention_2function__naming.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2function__naming.html#autotoc_md1059", [
        [ "1. სტრუქტურა", "md_docs_2convention_2function__naming.html#autotoc_md1060", null ],
        [ "2. Scope პრეფიქსები", "md_docs_2convention_2function__naming.html#autotoc_md1061", [
          [ "Public API ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1062", null ],
          [ "Static/Private ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1063", null ],
          [ "Framework API ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1064", null ]
        ] ]
      ] ],
      [ "მოდულის ტიპების მიხედვით", "md_docs_2convention_2function__naming.html#autotoc_md1065", [
        [ "1. Service Locator", "md_docs_2convention_2function__naming.html#autotoc_md1066", null ],
        [ "2. Event Bus", "md_docs_2convention_2function__naming.html#autotoc_md1067", null ],
        [ "3. Display Modules", "md_docs_2convention_2function__naming.html#autotoc_md1068", null ],
        [ "4. Communication Modules", "md_docs_2convention_2function__naming.html#autotoc_md1069", null ],
        [ "5. Sensor Modules", "md_docs_2convention_2function__naming.html#autotoc_md1070", null ]
      ] ],
      [ "სპეციალური კონვენციები", "md_docs_2convention_2function__naming.html#autotoc_md1071", [
        [ "1. Module Lifecycle ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1072", null ],
        [ "2. Runtime Management ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1073", null ],
        [ "3. Event Handler ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1074", null ],
        [ "4. Task ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1075", null ]
      ] ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2function__naming.html#autotoc_md1076", null ],
      [ "მაგალითი სრული მოდული", "md_docs_2convention_2function__naming.html#autotoc_md1077", null ]
    ] ],
    [ "🚀 მოდულების ინიციალიზაციის დონეები (Initialization Levels)", "md_docs_2convention_2initialization__levels.html", [
      [ "🎯 მიზანი", "md_docs_2convention_2initialization__levels.html#autotoc_md1079", null ],
      [ "⚙️ როგორ მუშაობს?", "md_docs_2convention_2initialization__levels.html#autotoc_md1080", null ],
      [ "📜 ოქროს წესი", "md_docs_2convention_2initialization__levels.html#autotoc_md1081", [
        [ "💡 პრაქტიკული მაგალითი: <tt>wifi_manager</tt> და <tt>command_router</tt>", "md_docs_2convention_2initialization__levels.html#autotoc_md1082", null ]
      ] ],
      [ "📊 ინიციალიზაციის დონეების რეკომენდებული დიაპაზონები", "md_docs_2convention_2initialization__levels.html#autotoc_md1083", [
        [ "📦 <strong>დონე 1: ფუნდამენტური დრაივერები და რესურსები (0-19)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1085", null ],
        [ "🛠️ <strong>დონე 2: საბაზისო სისტემური სერვისები (20-39)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1087", null ],
        [ "📡 <strong>დონე 3: ქსელი, კონტროლი და კომუნიკაცია (40-59)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1089", null ],
        [ "💡 <strong>დონე 4: აპლიკაციის ლოგიკა და პერიფერია (60-79)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1091", null ],
        [ "🧩 <strong>დონე 5: მაღალი დონის კომპოზიტური მოდულები (80-100)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1093", null ]
      ] ],
      [ "📝 როგორ განვსაზღვროთ <tt>init_level</tt> ახალი მოდულისთვის?", "md_docs_2convention_2initialization__levels.html#autotoc_md1095", null ]
    ] ],
    [ "ლოგირების ფერთა სქემის კონვენცია", "md_docs_2convention_2logging__colors.html", [
      [ "1. 🎯 მიზანი", "md_docs_2convention_2logging__colors.html#autotoc_md1097", null ],
      [ "2. ⚙️ ტექნიკური იმპლემენტაცია", "md_docs_2convention_2logging__colors.html#autotoc_md1098", null ],
      [ "3. 🎨 ფერთა სქემა", "md_docs_2convention_2logging__colors.html#autotoc_md1099", [
        [ "**სრული ფერების სია (<tt>logging.h</tt>-დან)**", "md_docs_2convention_2logging__colors.html#autotoc_md1101", null ]
      ] ]
    ] ],
    [ "Logging-ის სტანდარტები", "md_docs_2convention_2logging__standards.html", [
      [ "Component Tag Definition", "md_docs_2convention_2logging__standards.html#autotoc_md1103", [
        [ "სტანდარტული ფორმატი", "md_docs_2convention_2logging__standards.html#autotoc_md1104", null ],
        [ "მაგალითები", "md_docs_2convention_2logging__standards.html#autotoc_md1105", null ]
      ] ],
      [ "Log Levels", "md_docs_2convention_2logging__standards.html#autotoc_md1106", [
        [ "1. ESP_LOGE - Error Level", "md_docs_2convention_2logging__standards.html#autotoc_md1107", null ],
        [ "2. ESP_LOGW - Warning Level", "md_docs_2convention_2logging__standards.html#autotoc_md1108", null ],
        [ "3. ESP_LOGI - Info Level", "md_docs_2convention_2logging__standards.html#autotoc_md1109", null ],
        [ "4. ESP_LOGD - Debug Level", "md_docs_2convention_2logging__standards.html#autotoc_md1110", null ],
        [ "5. ESP_LOGV - Verbose Level", "md_docs_2convention_2logging__standards.html#autotoc_md1111", null ]
      ] ],
      [ "Logging Patterns", "md_docs_2convention_2logging__standards.html#autotoc_md1112", [
        [ "1. Module Lifecycle Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1113", null ],
        [ "2. Service Registration Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1114", null ],
        [ "3. Configuration Parsing Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1115", null ],
        [ "4. Hardware Operation Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1116", null ],
        [ "5. Event Handling Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1117", null ],
        [ "6. Error Context Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1118", null ]
      ] ],
      [ "Runtime State Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1119", [
        [ "1. Module Enable/Disable", "md_docs_2convention_2logging__standards.html#autotoc_md1120", null ],
        [ "2. Task State Changes", "md_docs_2convention_2logging__standards.html#autotoc_md1121", null ]
      ] ],
      [ "Performance Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1122", [
        [ "1. Operation Timing", "md_docs_2convention_2logging__standards.html#autotoc_md1123", null ],
        [ "2. Memory Usage", "md_docs_2convention_2logging__standards.html#autotoc_md1124", null ]
      ] ],
      [ "Log Message Format Standards", "md_docs_2convention_2logging__standards.html#autotoc_md1125", [
        [ "1. Consistent Formatting", "md_docs_2convention_2logging__standards.html#autotoc_md1126", null ],
        [ "2. Parameter Naming", "md_docs_2convention_2logging__standards.html#autotoc_md1127", null ]
      ] ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2logging__standards.html#autotoc_md1128", null ],
      [ "შეჯამება", "md_docs_2convention_2logging__standards.html#autotoc_md1129", null ]
    ] ],
    [ "მოდულის სტრუქტურის კონვენცია", "md_docs_2convention_2module__structure.html", [
      [ "1. 📜 მიმოხილვა", "md_docs_2convention_2module__structure.html#autotoc_md1131", null ],
      [ "2. 📂 საქაღალდის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1132", [
        [ "2.1. სტანდარტული მოდულის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1133", null ],
        [ "2.2. კატეგორიები", "md_docs_2convention_2module__structure.html#autotoc_md1134", null ]
      ] ],
      [ "3. 📄 ფაილების დანიშნულება და შინაარსი", "md_docs_2convention_2module__structure.html#autotoc_md1135", [
        [ "3.1. <tt>src/</tt> დირექტორიის დანაწევრება", "md_docs_2convention_2module__structure.html#autotoc_md1136", null ],
        [ "3.2. კონფიგურაციის ფაილები", "md_docs_2convention_2module__structure.html#autotoc_md1137", null ],
        [ "3.3. დოკუმენტაცია", "md_docs_2convention_2module__structure.html#autotoc_md1138", null ],
        [ "3.4. კოდის სტრუქტურა (<tt>_create</tt> ფუნქცია)", "md_docs_2convention_2module__structure.html#autotoc_md1139", null ]
      ] ],
      [ "4. <tt>CMakeLists.txt</tt>-ის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1140", null ],
      [ "5. ❌ აკრძალული პრაქტიკები", "md_docs_2convention_2module__structure.html#autotoc_md1141", null ],
      [ "6. ✅ საუკეთესო პრაქტიკები", "md_docs_2convention_2module__structure.html#autotoc_md1142", null ]
    ] ],
    [ "asynchronous ოპერაციების მართვის პატერნი (Promise Pattern)", "md_docs_2convention_2promise__pattern.html", [
      [ "1. 🎯 მიზანი", "md_docs_2convention_2promise__pattern.html#autotoc_md1144", null ],
      [ "2. 🤔 პრობლემა: რატომ გვჭირდება Promise-ები?", "md_docs_2convention_2promise__pattern.html#autotoc_md1145", null ],
      [ "3. 💡 გადაწყვეტა: Promise პატერნი", "md_docs_2convention_2promise__pattern.html#autotoc_md1146", null ],
      [ "4. ⚙️ როგორ მუშაობს იმპლემენტაცია?", "md_docs_2convention_2promise__pattern.html#autotoc_md1147", null ],
      [ "5. 📝 გამოყენების პატერნი", "md_docs_2convention_2promise__pattern.html#autotoc_md1148", [
        [ "5.1. სერვისის მომწოდებლის მხარე (Provider - მაგ., <tt>wifi_manager</tt>)", "md_docs_2convention_2promise__pattern.html#autotoc_md1149", null ],
        [ "5.2. მომხმარებლის მხარე (Consumer - მაგ., <tt>ui_manager</tt>)", "md_docs_2convention_2promise__pattern.html#autotoc_md1150", null ]
      ] ],
      [ "6. ✅ როდის გამოვიყენოთ Promise და როდის Event Bus?", "md_docs_2convention_2promise__pattern.html#autotoc_md1151", null ]
    ] ],
    [ "რესურსების ოპტიმიზაციის პატერნი (Shared Task Pool)", "md_docs_2convention_2task__pool__pattern.html", [
      [ "1. 🎯 მიზანი", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1153", null ],
      [ "2. 🤔 პრობლემა: \"ერთი მოდული = ერთი ტასკი\" ანტი-პატერნი", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1154", null ],
      [ "3. 💡 გადაწყვეტა: <tt>Task Pool Manager</tt>", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1155", null ],
      [ "4. 📝 გამოყენების პატერნი", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1156", [
        [ "4.1. მოდულის <tt>private_data</tt> სტრუქტურა", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1157", null ],
        [ "4.2. სამუშაო ფუნქციის (<tt>Job</tt>) იმპლემენტაცია", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1158", null ],
        [ "4.3. <tt>_start</tt> და <tt>_deinit</tt> ფუნქციები", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1159", null ]
      ] ],
      [ "5. ✅ როდის გამოვიყენოთ <tt>Task Pool</tt> და როდის საკუთარი ტასკი?", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1160", null ]
    ] ],
    [ "ცვლადების დასახელების წესები", "md_docs_2convention_2variable__naming.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2variable__naming.html#autotoc_md1162", [
        [ "1. დესკრიპტიული სახელები", "md_docs_2convention_2variable__naming.html#autotoc_md1163", null ],
        [ "2. აკრძალული პრეფიქსები", "md_docs_2convention_2variable__naming.html#autotoc_md1164", null ],
        [ "3. სწორი მიდგომა", "md_docs_2convention_2variable__naming.html#autotoc_md1165", [
          [ "Service Locator", "md_docs_2convention_2variable__naming.html#autotoc_md1166", null ],
          [ "Event Bus", "md_docs_2convention_2variable__naming.html#autotoc_md1167", null ],
          [ "Module Registry", "md_docs_2convention_2variable__naming.html#autotoc_md1168", null ],
          [ "Display Modules", "md_docs_2convention_2variable__naming.html#autotoc_md1169", null ]
        ] ],
        [ "4. კონვენციები", "md_docs_2convention_2variable__naming.html#autotoc_md1170", [
          [ "Global ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1171", null ],
          [ "Static ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1172", null ],
          [ "Mutex/Semaphore ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1173", null ],
          [ "Queue ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1174", null ],
          [ "API Structure ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1175", null ]
        ] ]
      ] ],
      [ "მაგალითები", "md_docs_2convention_2variable__naming.html#autotoc_md1176", [
        [ "Config Manager", "md_docs_2convention_2variable__naming.html#autotoc_md1177", null ],
        [ "I2C Bus Module", "md_docs_2convention_2variable__naming.html#autotoc_md1178", null ],
        [ "MQTT Module", "md_docs_2convention_2variable__naming.html#autotoc_md1179", null ]
      ] ],
      [ "Private Data Pointer Naming", "md_docs_2convention_2variable__naming.html#autotoc_md1180", [
        [ "❌ არასწორი", "md_docs_2convention_2variable__naming.html#autotoc_md1181", null ],
        [ "✅ სწორი", "md_docs_2convention_2variable__naming.html#autotoc_md1182", null ]
      ] ],
      [ "შეჯამება", "md_docs_2convention_2variable__naming.html#autotoc_md1183", null ]
    ] ],
    [ "Synapse ESP Framework — არქიტექტურის მიმოხილვა", "md_docs_2design__decisions_2architecture__overview.html", [
      [ "მიზანი", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1185", null ],
      [ "ძირითადი არქიტექტურული პრინციპები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1186", null ],
      [ "არქიტექტურული ფენები და ძირითადი კომპონენტები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1187", null ],
      [ "მოდულებს შორის კომუნიკაციის პატერნები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1188", null ],
      [ "სიცოცხლის ციკლი", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1189", null ],
      [ "საქაღალდეებისა და ფაილების სტრუქტურა", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1190", null ],
      [ "არქიტექტურული დიაგრამა (მოკლე)", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1191", null ],
      [ "სტანდარტები და დოკუმენტაცია", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1192", null ],
      [ "დასკვნა", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1193", null ]
    ] ],
    [ "კომუნიკაციის არქიტექტურული არჩევანი (Communication Choices)", "md_docs_2design__decisions_2communication__choices.html", [
      [ "მიზანი", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1196", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1198", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1200", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1202", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1204", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1206", [
        [ "Service Locator", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1207", null ],
        [ "Event Bus", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1208", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1210", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1212", null ],
      [ "Event Bus Pattern", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1214", [
        [ "არქიტექტურული ცვლილება: Enum-დან სტრიქონებზე გადასვლა", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1215", null ],
        [ "ფრეიმვორქის API-სთან წვდომა: Facade Pattern (<tt>synapse.h</tt>)", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1216", null ]
      ] ]
    ] ],
    [ "Core კომპონენტები (Core Components)", "md_docs_2design__decisions_2core__components.html", [
      [ "მიზანი", "md_docs_2design__decisions_2core__components.html#autotoc_md1220", null ],
      [ "ძირითადი კომპონენტები", "md_docs_2design__decisions_2core__components.html#autotoc_md1222", [
        [ "1. Service Locator", "md_docs_2design__decisions_2core__components.html#autotoc_md1223", null ],
        [ "2. Event Bus", "md_docs_2design__decisions_2core__components.html#autotoc_md1224", null ],
        [ "3. Module Register", "md_docs_2design__decisions_2core__components.html#autotoc_md1225", null ],
        [ "4. Module Factory", "md_docs_2design__decisions_2core__components.html#autotoc_md1226", null ],
        [ "5. Configuration Manager", "md_docs_2design__decisions_2core__components.html#autotoc_md1227", null ],
        [ "6. Logging", "md_docs_2design__decisions_2core__components.html#autotoc_md1228", null ],
        [ "7. Promise Manager", "md_docs_2design__decisions_2core__components.html#autotoc_md1229", null ]
      ] ],
      [ "ურთიერთქმედება მოდულებთან", "md_docs_2design__decisions_2core__components.html#autotoc_md1231", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2core__components.html#autotoc_md1233", null ]
    ] ],
    [ "არქიტექტურული გადაწყვეტილების ჩანაწერის შაბლონი (Decision Record Template)", "md_docs_2design__decisions_2decision__record__template.html", [
      [ "გადაწყვეტილების ID", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1237", null ],
      [ "თარიღი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1238", null ],
      [ "სტატუსი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1239", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1240", null ],
      [ "გადაწყვეტილება", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1241", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1242", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1243", null ],
      [ "განხორციელების ნაბიჯები", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1244", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1245", null ]
    ] ],
    [ "არქიტექტურული გადაწყვეტილება: დამოკიდებულებების ინექცია (Dependency Injection)", "md_docs_2design__decisions_2dependency__injection.html", [
      [ "1. კონტექსტი", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1248", null ],
      [ "2. გადაწყვეტილება", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1249", null ],
      [ "3. არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1250", null ],
      [ "4. შედეგები და გავლენა", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1251", [
        [ "4.1. კონვენცია მოდულის დეველოპერებისთვის", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1252", null ],
        [ "4.2. გამოყენების მაგალითი", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1253", null ]
      ] ],
      [ "5. დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1254", null ]
    ] ],
    [ "Synapse Design Decisions", "md_docs_2design__decisions_2design__decisions__index.html", [
      [ "ძირითადი დოკუმენტები", "md_docs_2design__decisions_2design__decisions__index.html#autotoc_md1257", null ]
    ] ],
    [ "შეცდომების დამუშავების არქიტექტურული გადაწყვეტილებები (Error Handling Decisions)", "md_docs_2design__decisions_2error__handling__decisions.html", [
      [ "მიზანი", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1261", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1263", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1265", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1267", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1269", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1271", [
        [ "პარამეტრების ვალიდაცია", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1272", null ],
        [ "რესურსების გამოყოფის შემოწმება", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1273", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1275", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1277", null ]
    ] ],
    [ "გაფართოებადობისა და მოდულარობის არქიტექტურული გადაწყვეტილებები (Extensibility & Modularity)", "md_docs_2design__decisions_2extensibility__and__modularity.html", [
      [ "მიზანი", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1281", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1283", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1285", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1287", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1289", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1291", [
        [ "ახალი მოდულის დამატება", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1292", null ],
        [ "Public API-ის გამოყენება", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1293", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1295", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1297", null ]
    ] ],
    [ "მიგრაციისა და თავსებადობის არქიტექტურული გადაწყვეტილებები (Migration & Compatibility)", "md_docs_2design__decisions_2migration__and__compatibility.html", [
      [ "მიზანი", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1301", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1303", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1305", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1307", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1309", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1311", [
        [ "სქემის ვერსიონირება", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1312", null ],
        [ "Deprecated API-ის მონიშვნა", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1313", null ],
        [ "მიგრაციის სკრიპტის გამოყენება", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1314", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1316", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1318", null ]
    ] ],
    [ "წარმადობისა და რესურსების არქიტექტურული კომპრომისები (Performance Trade-offs)", "md_docs_2design__decisions_2performance__tradeoffs.html", [
      [ "მიზანი", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1322", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1324", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1326", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1328", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1330", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1332", [
        [ "მოდულის გამორთვა ბილდიდან", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1333", null ],
        [ "Static allocation", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1334", null ],
        [ "Lazy initialization", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1335", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1337", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1339", null ]
    ] ],
    [ "უსაფრთხოების არქიტექტურული გადაწყვეტილებები (Security Considerations)", "md_docs_2design__decisions_2security__considerations.html", [
      [ "მიზანი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1343", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1345", null ],
      [ "არჩეული გადაწყვეტილებები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1347", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1349", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1351", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1353", [
        [ "TLS/SSL ჩართვა MQTT-ში", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1354", null ],
        [ "OTA განახლების სიგნირება", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1355", null ],
        [ "ლოგირების კონტროლი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1356", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1358", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1360", null ]
    ] ],
    [ "მონაცემთა შენახვის არქიტექტურული არჩევანი (Storage Strategy)", "md_docs_2design__decisions_2storage__strategy.html", [
      [ "მიზანი", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1364", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1366", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1368", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1370", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1372", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1374", [
        [ "NVS-ში სტრიქონის შენახვა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1375", null ],
        [ "NVS-დან სტრიქონის წაკითხვა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1376", null ],
        [ "Flash-ში მონაცემების შენახვა (OTA)", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1377", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1379", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1381", null ]
    ] ],
    [ "doxygen_main_pages", "md_docs_2doxygen__main__pages.html", null ],
    [ "არქიტექტურა და სტრუქტურა", "structure_docs.html", [
      [ "პროექტის მიმოხილვა", "structure_docs.html#struct_overview_sec", null ],
      [ "მოდულების არქიტექტურა", "structure_docs.html#struct_module_arch_sec", null ],
      [ "საქაღალდეებისა და ფაილების სტრუქტურა", "structure_docs.html#struct_dir_sec", null ],
      [ "მოდულის სიცოცხლის ციკლი", "structure_docs.html#struct_lifecycle_sec", null ],
      [ "კონფიგურაციის მართვის პროცესი", "structure_docs.html#struct_config_flow_sec", null ],
      [ "კომუნიკაციის პატერნები", "structure_docs.html#struct_comm_patterns_sec", null ],
      [ "შეცდომების დამუშავება და ლოგირება", "structure_docs.html#struct_error_sec", null ],
      [ "ფრეიმვორქის გაფართოება", "structure_docs.html#struct_extending_sec", null ]
    ] ],
    [ "კონვენციები და სტანდარტები", "conventions_docs.html", [
      [ "Doxygen კომენტარების სტანდარტი", "conventions_docs.html#conv_doxygen_sec", null ],
      [ "კომუნიკაციის პატერნები", "conventions_docs.html#conv_comm_sec", null ],
      [ "კონფიგურაციის მართვა", "conventions_docs.html#conv_config_sec", null ],
      [ "შეცდომების დამუშავება", "conventions_docs.html#conv_error_sec", null ],
      [ "ფუნქციების დასახელება", "conventions_docs.html#conv_func_name_sec", null ],
      [ "ლოგირების სტანდარტები", "conventions_docs.html#conv_logging_sec", null ],
      [ "მოდულის სტრუქტურა", "conventions_docs.html#conv_module_struct_sec", null ],
      [ "ცვლადების დასახელება", "conventions_docs.html#conv_var_name_sec", null ]
    ] ],
    [ "კონტრიბუციის გზამკვლევი", "contributing_docs.html", [
      [ "კონტრიბუციის პროცესი", "contributing_docs.html#contrib_guide_sec", null ],
      [ "ქცევის კოდექსი", "contributing_docs.html#contrib_coc_sec", null ],
      [ "Commit Message-ის წესები", "contributing_docs.html#contrib_commit_sec", null ],
      [ "ბრენჩინგის სტრატეგია", "contributing_docs.html#contrib_branching_sec", null ],
      [ "კონვენციების ავტომატური შემმოწმებელი", "contributing_docs.html#contrib_linter_guide_sec", null ]
    ] ],
    [ "სახელმძღვანელოები და მაგალითები", "tutorials_docs.html", [
      [ "სწრაფი სტარტი", "tutorials_docs.html#tut_getting_started_sec", null ],
      [ "პირველი მოდულის შექმნა", "tutorials_docs.html#tut_first_module_sec", null ],
      [ "ინტეგრაციის მაგალითი", "tutorials_docs.html#tut_integration_sec", null ]
    ] ],
    [ "api", "md_docs_2glossary_2api.html", null ],
    [ "API-სა და კოდის ტერმინები (Glossary — API & Code)", "md_docs_2glossary_2glossary__api.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__api.html#autotoc_md1385", null ]
    ] ],
    [ "არქიტექტურული ტერმინები (Glossary — Architecture)", "md_docs_2glossary_2glossary__architecture.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__architecture.html#autotoc_md1390", null ]
    ] ],
    [ "კონფიგურაციისა და შენახვის ტერმინები (Glossary — Configuration & Storage)", "md_docs_2glossary_2glossary__config.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__config.html#autotoc_md1395", null ]
    ] ],
    [ "Glossary (ტერმინები)", "md_docs_2glossary_2glossary__index.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__index.html#autotoc_md1400", null ],
      [ "თემატური ტერმინების დოკუმენტები", "md_docs_2glossary_2glossary__index.html#autotoc_md1402", null ],
      [ "გამოყენების წესი", "md_docs_2glossary_2glossary__index.html#autotoc_md1404", null ]
    ] ],
    [ "უსაფრთხოების და წარმადობის ტერმინები (Glossary — Security & Performance)", "md_docs_2glossary_2glossary__security.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__security.html#autotoc_md1408", null ]
    ] ],
    [ "Cloud ინტეგრაციის სახელმძღვანელო (Integration — Cloud)", "md_docs_2integration_2integration__cloud.html", [
      [ "მიზანი", "md_docs_2integration_2integration__cloud.html#autotoc_md1413", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__cloud.html#autotoc_md1415", null ],
      [ "Best Practices", "md_docs_2integration_2integration__cloud.html#autotoc_md1417", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__cloud.html#autotoc_md1419", null ]
    ] ],
    [ "Integration Index (ინტეგრაციის სახელმძღვანელოების ინდექსი)", "md_docs_2integration_2integration__index.html", [
      [ "მიზანი", "md_docs_2integration_2integration__index.html#autotoc_md1423", null ],
      [ "თემატური ინტეგრაციის სახელმძღვანელოები", "md_docs_2integration_2integration__index.html#autotoc_md1425", null ],
      [ "გამოყენების წესი", "md_docs_2integration_2integration__index.html#autotoc_md1427", null ]
    ] ],
    [ "MQTT ინტეგრაციის სახელმძღვანელო (Integration — MQTT)", "md_docs_2integration_2integration__mqtt.html", [
      [ "მიზანი", "md_docs_2integration_2integration__mqtt.html#autotoc_md1431", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__mqtt.html#autotoc_md1433", null ],
      [ "Best Practices", "md_docs_2integration_2integration__mqtt.html#autotoc_md1435", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__mqtt.html#autotoc_md1437", null ]
    ] ],
    [ "REST API ინტეგრაციის სახელმძღვანელო (Integration — REST API)", "md_docs_2integration_2integration__rest.html", [
      [ "მიზანი", "md_docs_2integration_2integration__rest.html#autotoc_md1441", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__rest.html#autotoc_md1443", null ],
      [ "Best Practices", "md_docs_2integration_2integration__rest.html#autotoc_md1445", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__rest.html#autotoc_md1447", null ]
    ] ],
    [ "WebSocket ინტეგრაციის სახელმძღვანელო (Integration — WebSocket)", "md_docs_2integration_2integration__websocket.html", [
      [ "მიზანი", "md_docs_2integration_2integration__websocket.html#autotoc_md1451", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__websocket.html#autotoc_md1453", null ],
      [ "Best Practices", "md_docs_2integration_2integration__websocket.html#autotoc_md1455", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__websocket.html#autotoc_md1457", null ]
    ] ],
    [ "✅ Synapse Framework — Documentation & Module Design Checklist", "md_docs_2internal_2synapse__checklist.html", [
      [ "1. API Naming & Structure", "md_docs_2internal_2synapse__checklist.html#autotoc_md1460", null ],
      [ "2. Module Implementation", "md_docs_2internal_2synapse__checklist.html#autotoc_md1461", null ],
      [ "3. Communication Integration (MQTT/WiFi/ESP-NOW)", "md_docs_2internal_2synapse__checklist.html#autotoc_md1462", null ],
      [ "4. Core Interaction", "md_docs_2internal_2synapse__checklist.html#autotoc_md1463", null ],
      [ "5. Documentation", "md_docs_2internal_2synapse__checklist.html#autotoc_md1464", null ],
      [ "6. Quality & Security", "md_docs_2internal_2synapse__checklist.html#autotoc_md1465", null ],
      [ "7. Testability", "md_docs_2internal_2synapse__checklist.html#autotoc_md1466", null ]
    ] ],
    [ "ბენჩმარკების ჩატარების სახელმძღვანელო (Performance Benchmarks Guide)", "md_docs_2performance_2performance__benchmarks.html", [
      [ "მიზანი", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1469", null ],
      [ "ბენჩმარკის ჩატარების ძირითადი ნაბიჯები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1471", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1473", [
        [ "სენსორის კითხვა", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1474", null ],
        [ "MQTT publish latency", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1475", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1477", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1479", null ]
    ] ],
    [ "საუკეთესო პრაქტიკები წარმადობისთვის (Performance Best Practices)", "md_docs_2performance_2performance__best__practices.html", [
      [ "მიზანი", "md_docs_2performance_2performance__best__practices.html#autotoc_md1483", null ],
      [ "ძირითადი რეკომენდაციები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1485", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1487", null ],
      [ "Best Practices", "md_docs_2performance_2performance__best__practices.html#autotoc_md1489", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1491", null ]
    ] ],
    [ "ეკრანის მოდულების ოპტიმიზაცია (Performance — Display Modules)", "md_docs_2performance_2performance__display.html", [
      [ "მიზანი", "md_docs_2performance_2performance__display.html#autotoc_md1495", null ],
      [ "ოპტიმიზაციის რეკომენდაციები", "md_docs_2performance_2performance__display.html#autotoc_md1497", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__display.html#autotoc_md1499", [
        [ "Refresh interval-ის ოპტიმიზაცია", "md_docs_2performance_2performance__display.html#autotoc_md1500", null ],
        [ "Conditional redraw", "md_docs_2performance_2performance__display.html#autotoc_md1501", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__display.html#autotoc_md1503", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__display.html#autotoc_md1505", null ]
    ] ],
    [ "Performance (წარმადობის სახელმძღვანელო)", "md_docs_2performance_2performance__index.html", [
      [ "მიზანი", "md_docs_2performance_2performance__index.html#autotoc_md1509", null ],
      [ "თემატური performance სახელმძღვანელოები", "md_docs_2performance_2performance__index.html#autotoc_md1511", null ],
      [ "გამოყენების წესი", "md_docs_2performance_2performance__index.html#autotoc_md1513", null ]
    ] ],
    [ "ოპტიმიზაციის სახელმძღვანელოები (Performance Optimization Guide)", "md_docs_2performance_2performance__optimization.html", [
      [ "მიზანი", "md_docs_2performance_2performance__optimization.html#autotoc_md1517", null ],
      [ "კოდის ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1519", null ],
      [ "კონფიგურაციის ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1521", null ],
      [ "მოდულების ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1523", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__optimization.html#autotoc_md1525", [
        [ "Static allocation", "md_docs_2performance_2performance__optimization.html#autotoc_md1526", null ],
        [ "Lazy initialization", "md_docs_2performance_2performance__optimization.html#autotoc_md1527", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__optimization.html#autotoc_md1529", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__optimization.html#autotoc_md1531", null ]
    ] ],
    [ "რესურსების გამოყენების სახელმძღვანელო (Performance Resource Usage Guide)", "md_docs_2performance_2performance__resource__usage.html", [
      [ "მიზანი", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1535", null ],
      [ "RAM-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1537", null ],
      [ "Flash-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1538", null ],
      [ "CPU-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1539", null ],
      [ "Stack-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1540", null ],
      [ "მონიტორინგის მაგალითები", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1542", [
        [ "RAM", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1543", null ],
        [ "Stack", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1544", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1546", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1548", null ]
    ] ],
    [ "უსაფრთხოების საუკეთესო პრაქტიკები (Security Best Practices)", "md_docs_2security_2security__best__practices.html", [
      [ "მიზანი", "md_docs_2security_2security__best__practices.html#autotoc_md1552", null ],
      [ "ძირითადი რეკომენდაციები", "md_docs_2security_2security__best__practices.html#autotoc_md1554", null ],
      [ "მაგალითები", "md_docs_2security_2security__best__practices.html#autotoc_md1556", [
        [ "TLS/SSL ჩართვა MQTT-ში", "md_docs_2security_2security__best__practices.html#autotoc_md1557", null ],
        [ "OTA განახლების სიგნირება", "md_docs_2security_2security__best__practices.html#autotoc_md1558", null ],
        [ "ლოგირების კონტროლი", "md_docs_2security_2security__best__practices.html#autotoc_md1559", null ]
      ] ],
      [ "Best Practices", "md_docs_2security_2security__best__practices.html#autotoc_md1561", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__best__practices.html#autotoc_md1563", null ]
    ] ],
    [ "უსაფრთხოების შემოწმების სია (Security Checklist)", "md_docs_2security_2security__checklist.html", [
      [ "მიზანი", "md_docs_2security_2security__checklist.html#autotoc_md1567", null ],
      [ "Release Checklist", "md_docs_2security_2security__checklist.html#autotoc_md1569", null ],
      [ "Code Review Checklist", "md_docs_2security_2security__checklist.html#autotoc_md1570", null ],
      [ "Best Practices", "md_docs_2security_2security__checklist.html#autotoc_md1572", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__checklist.html#autotoc_md1574", null ]
    ] ],
    [ "უსაფრთხოების სახელმძღვანელოები (Security Guidelines)", "md_docs_2security_2security__guidelines.html", [
      [ "მიზანი", "md_docs_2security_2security__guidelines.html#autotoc_md1578", null ],
      [ "TLS/SSL", "md_docs_2security_2security__guidelines.html#autotoc_md1580", null ],
      [ "Authentication & Authorization", "md_docs_2security_2security__guidelines.html#autotoc_md1581", null ],
      [ "Input Validation", "md_docs_2security_2security__guidelines.html#autotoc_md1582", null ],
      [ "Credentials Management", "md_docs_2security_2security__guidelines.html#autotoc_md1583", null ],
      [ "OTA და Firmware Security", "md_docs_2security_2security__guidelines.html#autotoc_md1584", null ],
      [ "Logging", "md_docs_2security_2security__guidelines.html#autotoc_md1585", null ],
      [ "Best Practices", "md_docs_2security_2security__guidelines.html#autotoc_md1587", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__guidelines.html#autotoc_md1589", null ]
    ] ],
    [ "Security Index (უსაფრთხოების სახელმძღვანელოების ინდექსი)", "md_docs_2security_2security__index.html", [
      [ "მიზანი", "md_docs_2security_2security__index.html#autotoc_md1593", null ],
      [ "თემატური უსაფრთხოების სახელმძღვანელოები", "md_docs_2security_2security__index.html#autotoc_md1595", null ],
      [ "გამოყენების წესი", "md_docs_2security_2security__index.html#autotoc_md1597", null ]
    ] ],
    [ "Vulnerability Disclosure Policy (უსაფრთხოების ხარვეზების აღმოჩენისა და შეტყობინების პროცედურა)", "md_docs_2security_2security__vulnerability__disclosure.html", [
      [ "მიზანი", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1601", null ],
      [ "ხარვეზის აღმოჩენა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1603", null ],
      [ "შეტყობინების პროცედურა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1604", null ],
      [ "რეაგირება", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1605", null ],
      [ "პასუხისმგებლობა და ეთიკა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1606", null ],
      [ "საკონტაქტო ინფორმაცია", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1608", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1610", null ]
    ] ],
    [ "კომუნიკაციის პატერნები (Communication Patterns)", "md_docs_2structure_2communication__patterns.html", [
      [ "მიზანი", "md_docs_2structure_2communication__patterns.html#autotoc_md1613", null ],
      [ "ძირითადი პატერნები", "md_docs_2structure_2communication__patterns.html#autotoc_md1614", [
        [ "1. Service Locator Pattern", "md_docs_2structure_2communication__patterns.html#autotoc_md1615", null ],
        [ "2. Event Bus Pattern", "md_docs_2structure_2communication__patterns.html#autotoc_md1616", null ]
      ] ],
      [ "სწორი გამოყენების წესები", "md_docs_2structure_2communication__patterns.html#autotoc_md1617", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2structure_2communication__patterns.html#autotoc_md1618", null ],
      [ "მაგალითები", "md_docs_2structure_2communication__patterns.html#autotoc_md1619", [
        [ "Service Locator", "md_docs_2structure_2communication__patterns.html#autotoc_md1620", null ],
        [ "Event Bus", "md_docs_2structure_2communication__patterns.html#autotoc_md1621", null ]
      ] ]
    ] ],
    [ "კონფიგურაციის მართვის პროცესი (Configuration Flow)", "md_docs_2structure_2configuration__flow.html", [
      [ "მიზანი", "md_docs_2structure_2configuration__flow.html#autotoc_md1624", null ],
      [ "კონფიგურაციის წყაროები", "md_docs_2structure_2configuration__flow.html#autotoc_md1625", null ],
      [ "კონფიგურაციის სტრუქტურა", "md_docs_2structure_2configuration__flow.html#autotoc_md1626", null ],
      [ "კონფიგურაციის დამუშავების ეტაპები", "md_docs_2structure_2configuration__flow.html#autotoc_md1627", null ],
      [ "კონფიგურაციის წვდომის მაგალითები", "md_docs_2structure_2configuration__flow.html#autotoc_md1628", null ],
      [ "ვალიდაცია და Default მნიშვნელობები", "md_docs_2structure_2configuration__flow.html#autotoc_md1629", null ],
      [ "Runtime Reconfiguration", "md_docs_2structure_2configuration__flow.html#autotoc_md1630", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2configuration__flow.html#autotoc_md1631", null ]
    ] ],
    [ "Core კომპონენტები (Core Components)", "md_docs_2structure_2core__components.html", [
      [ "მიზანი", "md_docs_2structure_2core__components.html#autotoc_md1635", null ],
      [ "ძირითადი კომპონენტები", "md_docs_2structure_2core__components.html#autotoc_md1637", [
        [ "1. Service Locator", "md_docs_2structure_2core__components.html#autotoc_md1638", null ],
        [ "2. Event Bus", "md_docs_2structure_2core__components.html#autotoc_md1639", null ],
        [ "3. Module Register", "md_docs_2structure_2core__components.html#autotoc_md1640", null ],
        [ "4. Module Factory", "md_docs_2structure_2core__components.html#autotoc_md1641", null ],
        [ "5. Configuration Manager", "md_docs_2structure_2core__components.html#autotoc_md1642", null ],
        [ "6. Logging", "md_docs_2structure_2core__components.html#autotoc_md1643", null ]
      ] ],
      [ "ფასადის ინტერფეისი (<tt>synapse.h</tt>)", "md_docs_2structure_2core__components.html#autotoc_md1645", null ],
      [ "ურთიერთქმედება მოდულებთან", "md_docs_2structure_2core__components.html#autotoc_md1647", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2core__components.html#autotoc_md1649", null ]
    ] ],
    [ "საქაღალდეებისა და ფაილების სტრუქტურა (Directory Structure)", "md_docs_2structure_2directory__structure.html", [
      [ "ძირითადი სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1652", null ],
      [ "ძირითადი საქაღალდეების აღწერა", "md_docs_2structure_2directory__structure.html#autotoc_md1653", null ],
      [ "<tt>modules</tt> საქაღალდის სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1654", null ],
      [ "თითოეული მოდულის სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1655", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2directory__structure.html#autotoc_md1656", null ]
    ] ],
    [ "შეცდომების დამუშავება და ლოგირების სტანდარტები (Error Handling & Logging)", "md_docs_2structure_2error__handling__and__logging.html", [
      [ "მიზანი", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1659", null ],
      [ "შეცდომების დამუშავების პრინციპები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1660", null ],
      [ "შეცდომების კოდები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1661", null ],
      [ "შეცდომების დამუშავების მაგალითები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1662", null ],
      [ "ლოგირების სტანდარტები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1663", null ],
      [ "ლოგირების მაგალითები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1664", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1665", null ]
    ] ],
    [ "ფრეიმვორქის გაფართოება და Best Practices (Extending the Framework)", "md_docs_2structure_2extending__framework.html", [
      [ "მიზანი", "md_docs_2structure_2extending__framework.html#autotoc_md1668", null ],
      [ "ახალი მოდულის დამატების პროცესი", "md_docs_2structure_2extending__framework.html#autotoc_md1669", null ],
      [ "Best Practices", "md_docs_2structure_2extending__framework.html#autotoc_md1670", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2structure_2extending__framework.html#autotoc_md1671", null ],
      [ "მაგალითი ახალი მოდულის დამატების", "md_docs_2structure_2extending__framework.html#autotoc_md1672", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (FAQ)", "md_docs_2structure_2faq.html", [
      [ "1. როგორ დავამატო ახალი მოდული?", "md_docs_2structure_2faq.html#autotoc_md1675", null ],
      [ "2. შეიძლება თუ არა ერთი და იმავე ტიპის რამდენიმე მოდულის დამატება?", "md_docs_2structure_2faq.html#autotoc_md1676", null ],
      [ "3. როგორ ხდება მოდულებს შორის კომუნიკაცია?", "md_docs_2structure_2faq.html#autotoc_md1677", null ],
      [ "4. როგორ მივიღო ან შევცვალო მოდულის კონფიგურაცია?", "md_docs_2structure_2faq.html#autotoc_md1678", null ],
      [ "5. როგორ დავამატო ახალი გლობალური პარამეტრი?", "md_docs_2structure_2faq.html#autotoc_md1679", null ],
      [ "6. როგორ დავიცვა naming და structure სტანდარტები?", "md_docs_2structure_2faq.html#autotoc_md1680", null ],
      [ "7. როგორ ხდება შეცდომების დამუშავება და ლოგირება?", "md_docs_2structure_2faq.html#autotoc_md1681", null ],
      [ "8. როგორ დავამატო ახალი კატეგორია მოდულებისთვის?", "md_docs_2structure_2faq.html#autotoc_md1682", null ],
      [ "9. როგორ დავამატო დოკუმენტაცია ან განვაახლო არსებული?", "md_docs_2structure_2faq.html#autotoc_md1683", null ],
      [ "10. როგორ დავიწყო პროექტის გამოყენება?", "md_docs_2structure_2faq.html#autotoc_md1684", null ]
    ] ],
    [ "მოდულის სიცოცხლის ციკლი და ინიციალიზაცია (v2.0)", "md_docs_2structure_2lifecycle__and__init.html", [
      [ "მიზანი", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1687", null ],
      [ "სიცოცხლის ციკლის ეტაპები და სერვისის სტატუსები", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1688", null ],
      [ "სიცოცხლის ციკლის დიაგრამა (განახლებული)", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1689", null ],
      [ "მაგალითი მოდულის სიცოცხლის ციკლის მართვის", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1690", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1691", null ]
    ] ],
    [ "მოდულების არქიტექტურა (Module Architecture)", "md_docs_2structure_2module__architecture.html", [
      [ "ძირითადი პრინციპები", "md_docs_2structure_2module__architecture.html#autotoc_md1694", null ],
      [ "მოდულის ტიპები", "md_docs_2structure_2module__architecture.html#autotoc_md1695", null ],
      [ "მოდულების ურთიერთქმედება", "md_docs_2structure_2module__architecture.html#autotoc_md1696", null ],
      [ "მოდულის სიცოცხლის ციკლი (Lifecycle)", "md_docs_2structure_2module__architecture.html#autotoc_md1697", null ],
      [ "მოდულის სტრუქტურის მაგალითი", "md_docs_2structure_2module__architecture.html#autotoc_md1698", [
        [ "<tt>_create</tt> ფუნქციის კონტრაქტი", "md_docs_2structure_2module__architecture.html#autotoc_md1699", null ]
      ] ],
      [ "მოდულის გაფართოება/დამატება", "md_docs_2structure_2module__architecture.html#autotoc_md1700", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2module__architecture.html#autotoc_md1701", null ]
    ] ],
    [ "🚀 <tt>create_module.py</tt>: Synapse მოდულის გენერატორი (v3.1)", "md_docs_2tools_2create__module.html", [
      [ "1. 📋 რა არის <tt>create_module.py</tt>?", "md_docs_2tools_2create__module.html#autotoc_md1712", null ],
      [ "2. 🎯 რას აკეთებს?", "md_docs_2tools_2create__module.html#autotoc_md1713", [
        [ "✅ გენერირებული ფაილები (10+ ფაილი)", "md_docs_2tools_2create__module.html#autotoc_md1714", null ],
        [ "✅ ავტომატური ინტეგრაცია", "md_docs_2tools_2create__module.html#autotoc_md1715", null ]
      ] ],
      [ "3. 🛠️ გამოყენების რეჟიმები", "md_docs_2tools_2create__module.html#autotoc_md1716", [
        [ "3.1. ინტერაქტიული რეჟიმი (რეკომენდებული)", "md_docs_2tools_2create__module.html#autotoc_md1717", null ],
        [ "3.2. ბრძანების ხაზის რეჟიმი (ავტომატიზაციისთვის)", "md_docs_2tools_2create__module.html#autotoc_md1718", null ]
      ] ],
      [ "4. 🏛️ არქეტიპები (Archetypes)", "md_docs_2tools_2create__module.html#autotoc_md1719", null ],
      [ "5. 🔄 სრული სამუშაო პროცესი (Workflow)", "md_docs_2tools_2create__module.html#autotoc_md1720", null ],
      [ "6. 🚨 მნიშვნელოვანი შენიშვნები", "md_docs_2tools_2create__module.html#autotoc_md1721", null ],
      [ "🎯 შეჯამება", "md_docs_2tools_2create__module.html#autotoc_md1723", null ]
    ] ],
    [ "📝 <tt>JSON</tt> ვალიდატორი (<tt>validate_jsons.py</tt>)", "md_docs_2tools_2json__validator.html", [
      [ "1. 🎯 დანიშნულება", "md_docs_2tools_2json__validator.html#autotoc_md1725", null ],
      [ "2. 🏛️ არქიტექტურა", "md_docs_2tools_2json__validator.html#autotoc_md1726", null ],
      [ "3. 🛠️ გამოყენება", "md_docs_2tools_2json__validator.html#autotoc_md1727", null ],
      [ "4. ✍️ როგორ დავამატოთ ვალიდაცია ახალ მოდულს?", "md_docs_2tools_2json__validator.html#autotoc_md1728", null ]
    ] ],
    [ "Tools & Scripts", "md_docs_2tools_2tools__index.html", null ],
    [ "Advanced Diagnostics (გაფართოებული დიაგნოსტიკა)", "md_docs_2troubleshooting_2advanced__diagnostics.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1731", null ],
      [ "1. Stack Overflow და Task Watchdog", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1733", null ],
      [ "2. Heap Corruption და Memory Leak", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1734", null ],
      [ "3. Core Dumps და Crash Reporting", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1735", null ],
      [ "4. Custom Health Checks", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1736", null ],
      [ "5. Peripheral Diagnostics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1737", null ],
      [ "6. Network Diagnostics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1738", null ],
      [ "7. Performance Metrics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1739", null ]
    ] ],
    [ "გავრცელებული პრობლემები და გადაჭრის გზები (Common Issues & Solutions)", "md_docs_2troubleshooting_2common__issues.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1742", null ],
      [ "1. ბილდის შეცდომები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1744", [
        [ "პრობლემა: მოდული არ იბილდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1745", null ],
        [ "პრობლემა: include file not found", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1746", null ]
      ] ],
      [ "2. კონფიგურაციის პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1748", [
        [ "პრობლემა: მოდული არ ინიციალიზდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1749", null ],
        [ "პრობლემა: default მნიშვნელობები არ მუშაობს", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1750", null ]
      ] ],
      [ "3. კომუნიკაციის პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1752", [
        [ "პრობლემა: Event Bus-ზე მოვლენები არ ვრცელდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1753", null ],
        [ "პრობლემა: Service Locator-ით API არ იძებნება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1754", null ]
      ] ],
      [ "4. ლოგირების და შეცდომების დიაგნოსტიკა", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1756", [
        [ "პრობლემა: ლოგები არ ჩანს", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1757", null ],
        [ "პრობლემა: შეცდომის კოდი არ იკითხება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1758", null ]
      ] ],
      [ "5. Runtime პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1760", [
        [ "პრობლემა: მოდული არ რეაგირებს enable/disable-ზე", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1761", null ],
        [ "პრობლემა: რესურსების გაჟონვა (memory leak)", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1762", null ],
        [ "პრობლემა: სისტემის კრახი <tt>Stack Overflow in task Tmr Svc</tt> შეცდომით", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1764", null ],
        [ "პრობლემა: სისტემის კრახი <tt>LoadProhibited</tt> ან <tt>assert failed: heap_caps_free</tt>", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1765", null ]
      ] ],
      [ "6. დამატებითი რჩევები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1767", null ]
    ] ],
    [ "Debugging Guide (დიბაგინგის სახელმძღვანელო)", "md_docs_2troubleshooting_2debugging__guide.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1770", null ],
      [ "1. ლოგირების ჩართვა და გამოყენება", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1772", null ],
      [ "2. Debug Build-ის კონფიგურაცია", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1773", null ],
      [ "3. GDB და Breakpoint-ების გამოყენება", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1774", null ],
      [ "4. Task და Stack Usage მონიტორინგი", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1775", null ],
      [ "5. Heap და Memory Usage", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1776", null ],
      [ "6. Event და Service Debugging", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1777", null ],
      [ "7. Assertion-ები და Error Checking", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1778", null ],
      [ "8. დამატებითი ინსტრუმენტები", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1779", null ]
    ] ],
    [ "პრობლემა: როგორ გადავცეთ კონტექსტი სერვისის API-ს გამოძახებისას?", "md_docs_2troubleshooting_2dependency__injection__and__context.html", [
      [ "1. ❓ პრობლემის აღწერა", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1782", null ],
      [ "2. ❌ არასწორი (მაგრამ მაცდური) გადაწყვეტები", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1783", null ],
      [ "3. ✅ სწორი არქიტექტურული პატერნი: ორმაგი ინექცია", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1784", [
        [ "ეტაპი 1: <tt>private_data</tt> სტრუქტურის გაფართოება", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1785", null ],
        [ "ეტაპი 2: <tt>dependency_map</tt>-ის განსაზღვრა", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1786", null ],
        [ "ეტაპი 3: <tt>config.json</tt>-ის კონფიგურაცია", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1787", null ],
        [ "ეტაპი 4: გამოყენება", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1788", null ]
      ] ],
      [ "4. 💬 დასკვნა", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1789", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (Troubleshooting FAQ)", "md_docs_2troubleshooting_2faq.html", [
      [ "1. რატომ არ იბილდება ჩემი მოდული?", "md_docs_2troubleshooting_2faq.html#autotoc_md1791", null ],
      [ "2. რატომ არ ჩანს ლოგები?", "md_docs_2troubleshooting_2faq.html#autotoc_md1792", null ],
      [ "3. რატომ ვერ პოულობს Service Locator-ი ჩემს სერვისს?", "md_docs_2troubleshooting_2faq.html#autotoc_md1793", null ],
      [ "4. რატომ არ ვრცელდება Event Bus-ზე მოვლენები?", "md_docs_2troubleshooting_2faq.html#autotoc_md1794", null ],
      [ "5. რატომ არ ინიციალიზდება მოდული?", "md_docs_2troubleshooting_2faq.html#autotoc_md1795", null ],
      [ "6. როგორ გავიგო რომელი მოდული იწვევს პრობლემას?", "md_docs_2troubleshooting_2faq.html#autotoc_md1796", null ],
      [ "7. როგორ გამოვასწორო რესურსების გაჟონვა (memory leak)?", "md_docs_2troubleshooting_2faq.html#autotoc_md1797", null ],
      [ "8. როგორ დავადგინო კონფიგურაციის შეცდომა?", "md_docs_2troubleshooting_2faq.html#autotoc_md1798", null ],
      [ "9. როგორ დავამატო ახალი კითხვა ამ FAQ-ში?", "md_docs_2troubleshooting_2faq.html#autotoc_md1799", null ]
    ] ],
    [ "Advanced Usage (გაფართოებული გამოყენება)", "md_docs_2tutorials_2advanced__usage.html", [
      [ "მიზანი", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1802", null ],
      [ "1. მრავალმოდულიანი კონფიგურაცია", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1803", null ],
      [ "2. მოდულის დინამიური ჩართვა/გამორთვა", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1804", null ],
      [ "3. Runtime Reconfiguration", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1805", null ],
      [ "4. Custom Event-ების გამოყენება", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1806", null ],
      [ "5. ასინქრონული ოპერაციების მართვა (Promise Pattern)", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1807", [
        [ "5.1. სერვისის მომწოდებლის API (<tt>wifi_manager</tt>)", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1808", null ],
        [ "5.2. მომხმარებლის ლოგიკა (<tt>ui_manager</tt>)", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1809", null ]
      ] ],
      [ "6. მოდულის სტატუსის მონიტორინგი", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1810", null ],
      [ "7. ოპტიმიზაცია და რესურსების მართვა", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1811", null ],
      [ "8. Best Practices", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1812", null ]
    ] ],
    [ "პირველი საკუთარი მოდულის შექმნა (v2.0)", "md_docs_2tutorials_2create__first__module.html", [
      [ "მიზანი", "md_docs_2tutorials_2create__first__module.html#autotoc_md1815", null ],
      [ "1. საჭიროების განსაზღვრა", "md_docs_2tutorials_2create__first__module.html#autotoc_md1816", null ],
      [ "2. მოდულის ჩონჩხის გენერაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1817", null ],
      [ "3. კონფიგურაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1818", null ],
      [ "4. კოდის იმპლემენტაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1819", null ],
      [ "5. ტესტირება", "md_docs_2tutorials_2create__first__module.html#autotoc_md1822", null ]
    ] ],
    [ "სწრაფი სტარტი (Getting Started)", "md_docs_2tutorials_2getting__started.html", [
      [ "მიზანი", "md_docs_2tutorials_2getting__started.html#autotoc_md1825", null ],
      [ "1. გარემოს მომზადება", "md_docs_2tutorials_2getting__started.html#autotoc_md1826", null ],
      [ "2. პროექტის სტრუქტურის გაცნობა", "md_docs_2tutorials_2getting__started.html#autotoc_md1827", null ],
      [ "3. პირველი მოდულის ჩართვა", "md_docs_2tutorials_2getting__started.html#autotoc_md1828", null ],
      [ "4. ბილდი და ფლეში", "md_docs_2tutorials_2getting__started.html#autotoc_md1829", null ],
      [ "5. ლოგების ნახვა და ტესტირება", "md_docs_2tutorials_2getting__started.html#autotoc_md1830", null ],
      [ "6. დამატებითი რესურსები", "md_docs_2tutorials_2getting__started.html#autotoc_md1831", null ]
    ] ],
    [ "ინტეგრაციის მაგალითი (Integration Example)", "md_docs_2tutorials_2integration__example.html", [
      [ "მიზანი", "md_docs_2tutorials_2integration__example.html#autotoc_md1834", null ],
      [ "1. ინტეგრაციის საჭიროების განსაზღვრა", "md_docs_2tutorials_2integration__example.html#autotoc_md1835", null ],
      [ "2. საჭირო მოდულების ჩართვა და კონფიგურაცია", "md_docs_2tutorials_2integration__example.html#autotoc_md1836", null ],
      [ "3. MQTT Integration (Publish/Subscribe)", "md_docs_2tutorials_2integration__example.html#autotoc_md1837", null ],
      [ "4. REST API Integration", "md_docs_2tutorials_2integration__example.html#autotoc_md1838", null ],
      [ "5. WebSocket Integration", "md_docs_2tutorials_2integration__example.html#autotoc_md1839", null ],
      [ "6. Best Practices", "md_docs_2tutorials_2integration__example.html#autotoc_md1840", null ],
      [ "7. Troubleshooting", "md_docs_2tutorials_2integration__example.html#autotoc_md1841", null ]
    ] ],
    [ "მოდულის გაფართოება ან ფუნქციონალის დამატება (Module Extension & Feature Addition)", "md_docs_2tutorials_2module__extension.html", [
      [ "მიზანი", "md_docs_2tutorials_2module__extension.html#autotoc_md1844", null ],
      [ "1. საჭიროების განსაზღვრა", "md_docs_2tutorials_2module__extension.html#autotoc_md1845", null ],
      [ "2. სტრუქტურის დაცვა", "md_docs_2tutorials_2module__extension.html#autotoc_md1846", null ],
      [ "3. ახალი API ან ფუნქციის დამატება", "md_docs_2tutorials_2module__extension.html#autotoc_md1847", null ],
      [ "4. კონფიგურაციის გაფართოება", "md_docs_2tutorials_2module__extension.html#autotoc_md1848", null ],
      [ "5. კომუნიკაციის გაფართოება", "md_docs_2tutorials_2module__extension.html#autotoc_md1849", null ],
      [ "6. ტესტირება და ლოგირება", "md_docs_2tutorials_2module__extension.html#autotoc_md1850", null ],
      [ "7. დოკუმენტაციის განახლება", "md_docs_2tutorials_2module__extension.html#autotoc_md1851", null ],
      [ "Best Practices", "md_docs_2tutorials_2module__extension.html#autotoc_md1852", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2tutorials_2module__extension.html#autotoc_md1853", null ]
    ] ],
    [ "სისტემის კონფიგურაცია (System Configuration)", "md_docs_2tutorials_2system__configuration.html", [
      [ "🎯 მიზანი", "md_docs_2tutorials_2system__configuration.html#autotoc_md1856", null ],
      [ "1. კონფიგურაციის ფაილების როლები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1858", null ],
      [ "2. <tt>config.json</tt>-ის სტრუქტურა და მაგალითები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1860", null ],
      [ "3. მოდულის კონფიგურაციის ობიექტის აღწერა", "md_docs_2tutorials_2system__configuration.html#autotoc_md1862", null ],
      [ "4. პარამეტრებზე წვდომა კოდიდან (Dot-Notation)", "md_docs_2tutorials_2system__configuration.html#autotoc_md1864", null ],
      [ "5. კონფიგურაციის ვალიდაცია და Default-ები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1866", null ],
      [ "6. საუკეთესო პრაქტიკები (Best Practices)", "md_docs_2tutorials_2system__configuration.html#autotoc_md1868", null ]
    ] ],
    [ "Deprecated List", "deprecated.html", null ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ],
    [ "Examples", "examples.html", "examples" ]
  ] ]
];

var NAVTREEINDEX =
[
"_2home_2runner_2work_2synapse_2synapse_2components_2core_2include_2synapse_utils_8h-example.html",
"md_docs_2api__reference_2display__api.html#autotoc_md150",
"md_docs_2api__reference_2utils__api.html#autotoc_md503",
"md_docs_2changelog_2v9.html#autotoc_md788",
"md_docs_2convention_2logging__standards.html#autotoc_md1119",
"md_docs_2performance_2performance__best__practices.html#autotoc_md1491",
"md_docs_2tutorials_2getting__started.html#autotoc_md1831",
"structfont__metrics__t.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';