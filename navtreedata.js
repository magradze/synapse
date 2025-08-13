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
    [ "Logger Module", "md_components_2modules_2utilities_2logger_2README.html", [
      [ "Overview", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md1", null ],
      [ "Features", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md2", null ],
      [ "Purpose", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md3", null ],
      [ "Kconfig Configuration", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md4", [
        [ "Main Configuration Options", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md5", null ],
        [ "Compilation Control", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md6", null ]
      ] ],
      [ "Runtime Configuration", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md7", [
        [ "Enable Logger Module", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md8", null ],
        [ "Disable Logger Module", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md9", null ]
      ] ],
      [ "Runtime Control", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md10", [
        [ "Command Format", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md11", null ],
        [ "Supported Commands", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md12", [
          [ "Disable Logger", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md13", null ],
          [ "Enable Logger", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md14", null ]
        ] ],
        [ "Behavior", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md15", null ]
      ] ],
      [ "Implementation Details", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md16", [
        [ "Module Structure", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md17", null ],
        [ "Event Subscription", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md18", null ],
        [ "Output Format", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md19", null ]
      ] ],
      [ "Development Notes", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md20", null ],
      [ "API Reference", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md21", [
        [ "Functions", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md22", [
          [ "<tt>logger_module_create(const cJSON *config)</tt>", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md23", null ]
        ] ],
        [ "ბრძანებები", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md24", null ],
        [ "ქცევა", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md25", null ],
        [ "მაგალითი", "md_components_2modules_2utilities_2logger_2README.html#autotoc_md26", null ]
      ] ]
    ] ],
    [ "Synapse Actuators Module API Reference", "md_docs_2api__reference_2actuators__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2actuators__api.html#autotoc_md28", null ],
      [ "Relay Module API", "md_docs_2api__reference_2actuators__api.html#autotoc_md30", [
        [ "module_t <em>relay_module_create(const cJSON *config);", "md_docs_2api__reference_2actuators__api.html#autotoc_md31", null ],
        [ "esp_err_t relay_module_enable(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md32", null ],
        [ "esp_err_t relay_module_disable(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md33", null ],
        [ "esp_err_t relay_module_toggle(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md34", null ],
        [ "esp_err_t relay_module_set_state(module_t *module, bool state);", "md_docs_2api__reference_2actuators__api.html#autotoc_md35", null ],
        [ "bool relay_module_get_state(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md36", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2actuators__api.html#autotoc_md38", [
        [ "static void relay_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2actuators__api.html#autotoc_md39", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2actuators__api.html#autotoc_md41", [
        [ "static esp_err_t parse_relay_config(const cJSON *config, relay_private_data_t *relay_data);", "md_docs_2api__reference_2actuators__api.html#autotoc_md42", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2actuators__api.html#autotoc_md44", [
        [ "static module_status_t relay_get_status(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md45", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2actuators__api.html#autotoc_md47", null ],
      [ "მაგალითები", "md_docs_2api__reference_2actuators__api.html#autotoc_md49", null ]
    ] ],
    [ "Synapse API Reference Index", "md_docs_2api__reference_2api__index.html", [
      [ "Core API", "md_docs_2api__reference_2api__index.html#autotoc_md53", null ],
      [ "Module API", "md_docs_2api__reference_2api__index.html#autotoc_md54", null ],
      [ "System & Extensions", "md_docs_2api__reference_2api__index.html#autotoc_md55", null ]
    ] ],
    [ "Synapse Assertions & Guards API Reference", "md_docs_2api__reference_2assert__and__guards__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md58", null ],
      [ "1. Assertion API (<tt>synapse_assert.h</tt>)", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md60", [
        [ "<tt>SYNAPSE_ASSERT(condition, format, ...)</tt>", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md61", null ]
      ] ],
      [ "2. Guard Clause API (<tt>synapse_utils.h</tt>)", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md63", [
        [ "<tt>SYNAPSE_GUARD(condition, tag, ret_val, format, ...)</tt>", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md64", null ],
        [ "<tt>SYNAPSE_GUARD_VOID(condition, tag, format, ...)</tt>", "md_docs_2api__reference_2assert__and__guards__api.html#autotoc_md65", null ]
      ] ]
    ] ],
    [ "Synapse Command Router API Reference", "md_docs_2api__reference_2command__router__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2command__router__api.html#autotoc_md67", null ],
      [ "⚙️ API სტრუქტურა (<tt>cmd_router_api_t</tt>)", "md_docs_2api__reference_2command__router__api.html#autotoc_md69", null ],
      [ "📚 API ფუნქციები", "md_docs_2api__reference_2command__router__api.html#autotoc_md71", [
        [ "<tt>esp_err_t register_command(const cmd_t *command)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md72", null ],
        [ "<tt>esp_err_t unregister_command(const char *command_name)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md74", null ],
        [ "<tt>bool is_command_registered(const char *command_name)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md76", null ]
      ] ],
      [ "📖 დამხმარე სტრუქტურა (<tt>cmd_t</tt>)", "md_docs_2api__reference_2command__router__api.html#autotoc_md78", null ],
      [ "💡 გამოყენების პატერნი: კონფლიქტისგან დაცული რეგისტრაცია", "md_docs_2api__reference_2command__router__api.html#autotoc_md80", null ]
    ] ],
    [ "📡 Synapse Communication Module API Reference", "md_docs_2api__reference_2communication__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2communication__api.html#autotoc_md82", null ],
      [ "📶 WiFi Manager Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md84", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md85", null ],
        [ "💡 WiFi API-ს გამოყენების მაგალითი", "md_docs_2api__reference_2communication__api.html#autotoc_md86", null ]
      ] ],
      [ "☁️ MQTT Client Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md88", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md89", null ],
        [ "💡 MQTT API-ს გამოყენების მაგალითი", "md_docs_2api__reference_2communication__api.html#autotoc_md90", null ]
      ] ],
      [ "⚡ ESP-NOW Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md92", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md93", null ]
      ] ],
      [ "📜 Naming & Structure კონვენციები", "md_docs_2api__reference_2communication__api.html#autotoc_md95", null ]
    ] ],
    [ "Synapse Configuration Manager API Reference", "md_docs_2api__reference_2configuration__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2configuration__api.html#autotoc_md98", null ],
      [ "🔑 გასაღების ფორმატი (Dot-Notation)", "md_docs_2api__reference_2configuration__api.html#autotoc_md100", null ],
      [ "⚙️ ძირითადი API ფუნქციები", "md_docs_2api__reference_2configuration__api.html#autotoc_md102", [
        [ "<tt>esp_err_t synapse_config_get_string(const char *key, char *buffer, size_t buffer_size)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md103", null ],
        [ "<tt>esp_err_t synapse_config_get_int(const char *key, int *out_value)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md104", null ],
        [ "<tt>esp_err_t synapse_config_get_bool(const char *key, bool *out_value)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md105", null ],
        [ "<tt>esp_err_t synapse_config_save(void)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md106", null ]
      ] ],
      [ "🚀 გამოყენების მაგალითები", "md_docs_2api__reference_2configuration__api.html#autotoc_md108", [
        [ "გლობალური პარამეტრის წაკითხვა", "md_docs_2api__reference_2configuration__api.html#autotoc_md109", null ],
        [ "მოდულის პარამეტრის წაკითხვა", "md_docs_2api__reference_2configuration__api.html#autotoc_md110", null ]
      ] ]
    ] ],
    [ "Synapse Core API Reference", "md_docs_2api__reference_2core__api.html", [
      [ "⚙️ Service Locator API (v2.0 - State-Aware)", "md_docs_2api__reference_2core__api.html#autotoc_md112", [
        [ "<tt>esp_err_t synapse_service_register_with_status(const char *service_name, synapse_service_type_t service_type, service_handle_t service_handle, service_status_t initial_status);</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md113", null ],
        [ "<tt>esp_err_t synapse_service_set_status(const char *service_name, service_status_t new_status);</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md114", null ],
        [ "<tt>esp_err_t synapse_service_get_status(const char *service_name, service_status_t *out_status);</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md115", null ],
        [ "<tt>service_handle_t synapse_service_get(const char *service_name);</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md116", null ],
        [ "<tt>esp_err_t synapse_service_unregister(const char *service_name);</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md117", null ],
        [ "<tt>__attribute__((deprecated))</tt> <tt>esp_err_t synapse_service_register(...)</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md118", null ]
      ] ],
      [ "Event Bus API", "md_docs_2api__reference_2core__api.html#autotoc_md120", [
        [ "<tt>synapse_event_bus_post</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md121", null ],
        [ "<tt>synapse_event_bus_subscribe</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md122", null ],
        [ "<tt>synapse_event_bus_unsubscribe</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md123", null ]
      ] ],
      [ "Module Register API", "md_docs_2api__reference_2core__api.html#autotoc_md125", [
        [ "synapse_module_register", "md_docs_2api__reference_2core__api.html#autotoc_md126", null ],
        [ "synapse_module_find_by_name", "md_docs_2api__reference_2core__api.html#autotoc_md127", null ]
      ] ],
      [ "Module Factory API", "md_docs_2api__reference_2core__api.html#autotoc_md129", [
        [ "synapse_module_factory_create", "md_docs_2api__reference_2core__api.html#autotoc_md130", null ]
      ] ],
      [ "Configuration Manager API", "md_docs_2api__reference_2core__api.html#autotoc_md132", [
        [ "synapse_config_get_string", "md_docs_2api__reference_2core__api.html#autotoc_md133", null ],
        [ "synapse_config_get_int", "md_docs_2api__reference_2core__api.html#autotoc_md134", null ],
        [ "synapse_config_get_module_config", "md_docs_2api__reference_2core__api.html#autotoc_md135", null ]
      ] ],
      [ "Logging API", "md_docs_2api__reference_2core__api.html#autotoc_md137", [
        [ "DEFINE_COMPONENT_TAG", "md_docs_2api__reference_2core__api.html#autotoc_md138", null ],
        [ "ESP_LOGE, ESP_LOGW, ESP_LOGI, ESP_LOGD, ESP_LOGV", "md_docs_2api__reference_2core__api.html#autotoc_md139", null ]
      ] ]
    ] ],
    [ "Synapse Custom API Reference", "md_docs_2api__reference_2custom__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2custom__api.html#autotoc_md142", null ],
      [ "Custom Module API", "md_docs_2api__reference_2custom__api.html#autotoc_md144", [
        [ "module_t *custom_module_create(const cJSON *config);", "md_docs_2api__reference_2custom__api.html#autotoc_md145", null ],
        [ "esp_err_t custom_module_do_action(module_t *module, const char *action, const void *params);", "md_docs_2api__reference_2custom__api.html#autotoc_md146", null ],
        [ "esp_err_t custom_module_get_status(module_t *module, custom_status_t *status);", "md_docs_2api__reference_2custom__api.html#autotoc_md147", null ]
      ] ],
      [ "Custom Event API", "md_docs_2api__reference_2custom__api.html#autotoc_md149", [
        [ "Custom Event-ის იდენტიფიკატორი", "md_docs_2api__reference_2custom__api.html#autotoc_md150", null ],
        [ "Custom Event-ის მონაცემების სტრუქტურა", "md_docs_2api__reference_2custom__api.html#autotoc_md151", null ],
        [ "static void custom_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2custom__api.html#autotoc_md152", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2custom__api.html#autotoc_md154", null ],
      [ "მაგალითები", "md_docs_2api__reference_2custom__api.html#autotoc_md156", null ]
    ] ],
    [ "Synapse Device Identity Service API Reference", "md_docs_2api__reference_2device__identity__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2device__identity__api.html#autotoc_md159", null ],
      [ "Device Identity Service API", "md_docs_2api__reference_2device__identity__api.html#autotoc_md161", [
        [ "<tt>const char* get_device_id(void);</tt>", "md_docs_2api__reference_2device__identity__api.html#autotoc_md162", null ],
        [ "<tt>const char* get_firmware_version(void);</tt>", "md_docs_2api__reference_2device__identity__api.html#autotoc_md163", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2device__identity__api.html#autotoc_md165", null ],
      [ "მაგალითები", "md_docs_2api__reference_2device__identity__api.html#autotoc_md167", null ]
    ] ],
    [ "Synapse Display Module API Reference", "md_docs_2api__reference_2display__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2display__api.html#autotoc_md170", null ],
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2display__api.html#autotoc_md172", [
        [ "module_t <em>ssd1306_module_create(const cJSON *config);", "md_docs_2api__reference_2display__api.html#autotoc_md173", null ],
        [ "static esp_err_t ssd1306_init(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md174", null ],
        [ "static esp_err_t ssd1306_enable(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md175", null ],
        [ "static esp_err_t ssd1306_disable(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md176", null ],
        [ "static esp_err_t ssd1306_deinit(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md177", null ]
      ] ],
      [ "Service API (Service Locator-ისთვის)", "md_docs_2api__reference_2display__api.html#autotoc_md179", [
        [ "esp_err_t ssd1306_api_enable(void);", "md_docs_2api__reference_2display__api.html#autotoc_md180", null ],
        [ "esp_err_t ssd1306_api_disable(void);", "md_docs_2api__reference_2display__api.html#autotoc_md181", null ],
        [ "esp_err_t ssd1306_api_clear(void);", "md_docs_2api__reference_2display__api.html#autotoc_md182", null ],
        [ "esp_err_t ssd1306_api_write_text(const char *text, uint8_t line);", "md_docs_2api__reference_2display__api.html#autotoc_md183", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2display__api.html#autotoc_md185", [
        [ "static void ssd1306_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2display__api.html#autotoc_md186", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2display__api.html#autotoc_md188", [
        [ "static esp_err_t parse_ssd1306_config(const cJSON *config, ssd1306_private_data_t *ssd1306_data);", "md_docs_2api__reference_2display__api.html#autotoc_md189", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2display__api.html#autotoc_md191", [
        [ "static module_status_t ssd1306_get_status(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md192", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2display__api.html#autotoc_md194", null ],
      [ "მაგალითები", "md_docs_2api__reference_2display__api.html#autotoc_md196", null ]
    ] ],
    [ "Synapse Drivers Module API Reference", "md_docs_2api__reference_2drivers__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2drivers__api.html#autotoc_md199", null ],
      [ "I2C Bus Driver API", "md_docs_2api__reference_2drivers__api.html#autotoc_md201", [
        [ "module_t *i2c_bus_module_create(const cJSON *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md202", null ],
        [ "esp_err_t i2c_bus_init(i2c_bus_handle_t *i2c_bus_handle, const i2c_bus_config_t *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md203", null ],
        [ "esp_err_t i2c_bus_read(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, uint8_t *data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md204", null ],
        [ "esp_err_t i2c_bus_write(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, const uint8_t *data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md205", null ],
        [ "esp_err_t i2c_bus_scan(i2c_bus_handle_t *i2c_bus_handle);", "md_docs_2api__reference_2drivers__api.html#autotoc_md206", null ]
      ] ],
      [ "SPI Bus Driver API", "md_docs_2api__reference_2drivers__api.html#autotoc_md208", [
        [ "module_t *spi_bus_module_create(const cJSON *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md209", null ],
        [ "esp_err_t spi_bus_init(spi_bus_handle_t *spi_bus_handle, const spi_bus_config_t *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md210", null ],
        [ "esp_err_t spi_bus_transfer(spi_bus_handle_t *spi_bus_handle, const uint8_t *tx_data, uint8_t *rx_data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md211", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2drivers__api.html#autotoc_md213", null ],
      [ "მაგალითები", "md_docs_2api__reference_2drivers__api.html#autotoc_md215", null ]
    ] ],
    [ "Synapse Event Bus API Reference", "md_docs_2api__reference_2event__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2event__api.html#autotoc_md218", null ],
      [ "Event Bus API", "md_docs_2api__reference_2event__api.html#autotoc_md220", [
        [ "<tt>esp_err_t synapse_event_bus_init(void)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md221", null ],
        [ "<tt>esp_err_t synapse_event_bus_post(core_framework_event_id_t event_id, event_data_wrapper_t *data_wrapper)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md222", null ],
        [ "<tt>esp_err_t synapse_event_bus_subscribe(core_framework_event_id_t event_id, struct module_t *module)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md223", null ],
        [ "<tt>esp_err_t synapse_event_bus_unsubscribe(core_framework_event_id_t event_id, struct module_t *module)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md224", null ]
      ] ],
      [ "ივენთის მონაცემების მართვა (Reference Counting)", "md_docs_2api__reference_2event__api.html#autotoc_md226", null ],
      [ "Event Handler-ის მაგალითი", "md_docs_2api__reference_2event__api.html#autotoc_md228", null ],
      [ "Custom Event-ების აღწერა", "md_docs_2api__reference_2event__api.html#autotoc_md230", [
        [ "მაგალითი Custom Event-ის აღწერისა", "md_docs_2api__reference_2event__api.html#autotoc_md231", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2event__api.html#autotoc_md233", null ],
      [ "მაგალითები", "md_docs_2api__reference_2event__api.html#autotoc_md235", null ]
    ] ],
    [ "API რეფერენსი: Event Payloads (event_payloads.h)", "md_docs_2api__reference_2event__payloads__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md238", null ],
      [ "Payload სტრუქტურები", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md239", [
        [ "synapse_telemetry_payload_t", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md240", null ],
        [ "synapse_config_updated_payload_t", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md241", null ]
      ] ],
      [ "მეხსიერების მართვის ფუნქციები", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md242", [
        [ "synapse_payload_common_free", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md243", null ],
        [ "synapse_telemetry_payload_free", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md244", null ]
      ] ]
    ] ],
    [ "Synapse Health Monitor Module API Reference", "md_docs_2api__reference_2health__monitor__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md246", null ],
      [ "Health Monitor Module API", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md248", [
        [ "module_t *health_monitor_module_create(const cJSON *config);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md249", null ],
        [ "esp_err_t health_monitor_module_start(module_t *module);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md250", null ],
        [ "esp_err_t health_monitor_module_stop(module_t *module);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md251", null ],
        [ "esp_err_t health_monitor_module_get_status(module_t *module, health_status_t *status);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md252", null ],
        [ "esp_err_t health_monitor_module_register_check(module_t *module, health_check_fn_t check_fn);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md253", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md255", [
        [ "static void health_monitor_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md256", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md258", null ],
      [ "მაგალითები", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md260", null ]
    ] ],
    [ "Synapse Local Control Module API Reference", "md_docs_2api__reference_2local__ctrl__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md263", null ],
      [ "Local Control Module API", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md265", [
        [ "module_t *local_ctrl_module_create(const cJSON *config);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md266", null ],
        [ "esp_err_t local_ctrl_module_start(module_t *module);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md267", null ],
        [ "esp_err_t local_ctrl_module_stop(module_t *module);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md268", null ],
        [ "esp_err_t local_ctrl_module_register_endpoint(const char *uri, http_method_t method, local_ctrl_handler_t handler);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md269", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md271", [
        [ "static void local_ctrl_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md272", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md274", null ],
      [ "მაგალითები", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md276", null ]
    ] ],
    [ "Synapse Module API Reference", "md_docs_2api__reference_2module__api.html", [
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2module__api.html#autotoc_md279", [
        [ "<tt>module_t *{module_name}_create(const cJSON *config);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md280", null ],
        [ "<tt>static esp_err_t {module_name}_init(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md281", null ],
        [ "<tt>static esp_err_t {module_name}_start(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md282", null ],
        [ "<tt>static void {module_name}_deinit(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md283", null ]
      ] ],
      [ "Service API (Service Locator-ისთვის)", "md_docs_2api__reference_2module__api.html#autotoc_md285", [
        [ "<tt>esp_err_t {module_name}_api_{action}(...);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md286", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2module__api.html#autotoc_md288", [
        [ "<tt>static void {module_name}_handle_event(module_t *self, const char *event_name, void *event_data);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md289", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2module__api.html#autotoc_md291", [
        [ "<tt>static esp_err_t parse_{module_name}_config(const cJSON *config_node, {module_name}_private_data_t *p_data);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md292", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2module__api.html#autotoc_md294", [
        [ "<tt>static module_status_t {module_name}_get_status(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md295", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2module__api.html#autotoc_md297", [
        [ "<tt>static void {module_name}_debug_print(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md298", null ]
      ] ]
    ] ],
    [ "Synapse MQTT Manager Service API Reference", "md_docs_2api__reference_2mqtt__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2mqtt__api.html#autotoc_md301", null ],
      [ "MQTT Manager API", "md_docs_2api__reference_2mqtt__api.html#autotoc_md303", [
        [ "სამომავლო გაფართოება", "md_docs_2api__reference_2mqtt__api.html#autotoc_md304", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2mqtt__api.html#autotoc_md306", null ],
      [ "მაგალითები", "md_docs_2api__reference_2mqtt__api.html#autotoc_md308", null ]
    ] ],
    [ "Synapse OTA (Over-the-Air) Module API Reference", "md_docs_2api__reference_2ota__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2ota__api.html#autotoc_md311", null ],
      [ "OTA Module API", "md_docs_2api__reference_2ota__api.html#autotoc_md313", [
        [ "module_t *ota_module_create(const cJSON *config);", "md_docs_2api__reference_2ota__api.html#autotoc_md314", null ],
        [ "esp_err_t ota_module_start_update(const char *url);", "md_docs_2api__reference_2ota__api.html#autotoc_md315", null ],
        [ "esp_err_t ota_module_get_status(ota_status_t *status);", "md_docs_2api__reference_2ota__api.html#autotoc_md316", null ],
        [ "esp_err_t ota_module_abort(void);", "md_docs_2api__reference_2ota__api.html#autotoc_md317", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2ota__api.html#autotoc_md319", [
        [ "static void ota_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2ota__api.html#autotoc_md320", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2ota__api.html#autotoc_md322", null ],
      [ "მაგალითები", "md_docs_2api__reference_2ota__api.html#autotoc_md324", null ]
    ] ],
    [ "Synapse Promise Manager API Reference", "md_docs_2api__reference_2promise__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2promise__api.html#autotoc_md327", null ],
      [ "1. Consumer Types (მომხმარებლისთვის)", "md_docs_2api__reference_2promise__api.html#autotoc_md329", [
        [ "<tt>promise_then_cb</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md330", null ],
        [ "<tt>promise_catch_cb</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md331", null ]
      ] ],
      [ "2. Provider API (სერვისის მომწოდებლისთვის)", "md_docs_2api__reference_2promise__api.html#autotoc_md333", [
        [ "<tt>promise_handle_t synapse_promise_create(promise_then_cb then_cb, promise_catch_cb catch_cb, void* user_context);</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md334", null ],
        [ "<tt>esp_err_t synapse_promise_resolve(promise_handle_t handle, void* result_data, void (*free_fn)(void*));</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md335", null ],
        [ "<tt>esp_err_t synapse_promise_reject(promise_handle_t handle, void* error_data, void (*free_fn)(void*));</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md336", null ]
      ] ],
      [ "💡 გამოყენების მაგალითი", "md_docs_2api__reference_2promise__api.html#autotoc_md338", null ]
    ] ],
    [ "📲 Synapse Provisioning Module API Reference", "md_docs_2api__reference_2provisioning__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2provisioning__api.html#autotoc_md340", null ],
      [ "🔵 BLE Provisioning Service API (იმპლემენტირებულია)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md342", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2provisioning__api.html#autotoc_md343", null ]
      ] ],
      [ "📡 SoftAP Provisioning Module API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md345", [
        [ "module_t *softap_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md346", null ],
        [ "esp_err_t softap_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md347", null ],
        [ "esp_err_t softap_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md348", null ],
        [ "esp_err_t softap_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md349", null ],
        [ "esp_err_t softap_prov_module_get_status(module_t *module, softap_prov_status_t*status)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md350", null ]
      ] ],
      [ "🔳 QR Code Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md352", [
        [ "esp_err_t qr_code_prov_generate(const char *ssid, const char*password, char *qr_code_buffer, size_t buffer_size)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md353", null ],
        [ "esp_err_t qr_code_prov_display(const char *qr_code_buffer)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md354", null ]
      ] ],
      [ "🔌 Serial Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md356", [
        [ "module_t *serial_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md357", null ],
        [ "esp_err_t serial_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md358", null ],
        [ "esp_err_t serial_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md359", null ],
        [ "esp_err_t serial_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md360", null ]
      ] ],
      [ "☁️ Cloud Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md362", [
        [ "module_t *cloud_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md363", null ],
        [ "esp_err_t cloud_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md364", null ],
        [ "esp_err_t cloud_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md365", null ],
        [ "esp_err_t cloud_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md366", null ]
      ] ],
      [ "NFC Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md368", [
        [ "module_t *nfc_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md369", null ],
        [ "esp_err_t nfc_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md370", null ],
        [ "esp_err_t nfc_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md371", null ],
        [ "esp_err_t nfc_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md372", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2provisioning__api.html#autotoc_md374", null ],
      [ "მაგალითები", "md_docs_2api__reference_2provisioning__api.html#autotoc_md376", null ]
    ] ],
    [ "🔌 Relay Actuator API Reference", "md_docs_2api__reference_2relay__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2relay__api.html#autotoc_md379", null ],
      [ "1. ⌨️ CLI (Command Line Interface)", "md_docs_2api__reference_2relay__api.html#autotoc_md381", null ],
      [ "2. 📡 MQTT ინტერფეისი", "md_docs_2api__reference_2relay__api.html#autotoc_md383", [
        [ "ბრძანების გაგზავნა", "md_docs_2api__reference_2relay__api.html#autotoc_md384", null ],
        [ "სტატუსის მიღება", "md_docs_2api__reference_2relay__api.html#autotoc_md385", null ]
      ] ],
      [ "3. 📢 ივენთები (Events)", "md_docs_2api__reference_2relay__api.html#autotoc_md387", null ]
    ] ],
    [ "Synapse Resource Manager API Reference", "md_docs_2api__reference_2resource__manager__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md390", null ],
      [ "სამართავი რესურსების ტიპები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md392", null ],
      [ "API ფუნქციები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md394", [
        [ "<tt>esp_err_t synapse_resource_manager_init(void)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md395", null ],
        [ "<tt>esp_err_t synapse_resource_lock(synapse_resource_type_t type, uint8_t resource_id, const char *owner)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md397", null ],
        [ "<tt>esp_err_t synapse_resource_release(synapse_resource_type_t type, uint8_t resource_id, const char *owner)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md399", null ],
        [ "<tt>bool synapse_resource_is_locked(synapse_resource_type_t type, uint8_t resource_id)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md401", null ],
        [ "<tt>const char *synapse_resource_get_owner(synapse_resource_type_t type, uint8_t resource_id)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md403", null ]
      ] ],
      [ "გამოყენების მაგალითი", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md405", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md407", null ]
    ] ],
    [ "🛡️ Security Status API Reference", "md_docs_2api__reference_2security__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2security__api.html#autotoc_md410", null ],
      [ "1. 🔌 Service API (<tt>security_status_api_t</tt>)", "md_docs_2api__reference_2security__api.html#autotoc_md412", [
        [ "API ფუნქციები", "md_docs_2api__reference_2security__api.html#autotoc_md413", null ]
      ] ],
      [ "2. ⌨️ CLI (Command Line Interface)", "md_docs_2api__reference_2security__api.html#autotoc_md415", null ]
    ] ],
    [ "Synapse Sensor Module API Reference", "md_docs_2api__reference_2sensor__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2sensor__api.html#autotoc_md417", null ],
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2sensor__api.html#autotoc_md419", [
        [ "module_t <em>dht22_module_create(const cJSON *config);", "md_docs_2api__reference_2sensor__api.html#autotoc_md420", null ],
        [ "static esp_err_t dht22_init(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md421", null ],
        [ "static esp_err_t dht22_enable(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md422", null ],
        [ "static esp_err_t dht22_disable(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md423", null ],
        [ "static esp_err_t dht22_deinit(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md424", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2sensor__api.html#autotoc_md426", [
        [ "static void dht22_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2sensor__api.html#autotoc_md427", null ],
        [ "static esp_err_t publish_sensor_data(dht22_private_data_t *dht22_data, float temperature, float humidity);", "md_docs_2api__reference_2sensor__api.html#autotoc_md428", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2sensor__api.html#autotoc_md430", [
        [ "static esp_err_t parse_dht22_config(const cJSON *config, dht22_private_data_t *dht22_data);", "md_docs_2api__reference_2sensor__api.html#autotoc_md431", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2sensor__api.html#autotoc_md433", [
        [ "static module_status_t dht22_get_status(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md434", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2sensor__api.html#autotoc_md436", null ],
      [ "მაგალითები", "md_docs_2api__reference_2sensor__api.html#autotoc_md438", null ]
    ] ],
    [ "💾 Synapse Storage Manager API Reference", "md_docs_2api__reference_2storage__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2storage__api.html#autotoc_md441", null ],
      [ "⚙️ API-ს სტრუქტურა (<tt>storage_api_t</tt>)", "md_docs_2api__reference_2storage__api.html#autotoc_md443", [
        [ "🔑 Key-Value ოპერაციები", "md_docs_2api__reference_2storage__api.html#autotoc_md444", null ],
        [ "🗂️ ფაილური ოპერაციები", "md_docs_2api__reference_2storage__api.html#autotoc_md446", null ]
      ] ],
      [ "💡 გამოყენების მაგალითი", "md_docs_2api__reference_2storage__api.html#autotoc_md448", null ]
    ] ],
    [ "Synapse System Module API Reference", "md_docs_2api__reference_2system__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2system__api.html#autotoc_md451", null ],
      [ "OTA Module API", "md_docs_2api__reference_2system__api.html#autotoc_md453", [
        [ "module_t *ota_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md454", null ],
        [ "esp_err_t ota_module_start_update(const char *url);", "md_docs_2api__reference_2system__api.html#autotoc_md455", null ],
        [ "esp_err_t ota_module_get_status(ota_status_t *status);", "md_docs_2api__reference_2system__api.html#autotoc_md456", null ]
      ] ],
      [ "Version Module API", "md_docs_2api__reference_2system__api.html#autotoc_md458", [
        [ "module_t *version_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md459", null ],
        [ "esp_err_t version_module_get_firmware_version(char *version_str, size_t size);", "md_docs_2api__reference_2system__api.html#autotoc_md460", null ]
      ] ],
      [ "Watchdog Module API", "md_docs_2api__reference_2system__api.html#autotoc_md462", [
        [ "module_t *watchdog_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md463", null ],
        [ "esp_err_t watchdog_module_feed(void);", "md_docs_2api__reference_2system__api.html#autotoc_md464", null ],
        [ "esp_err_t watchdog_module_set_timeout(uint32_t timeout_ms);", "md_docs_2api__reference_2system__api.html#autotoc_md465", null ]
      ] ],
      [ "Diagnostics Module API", "md_docs_2api__reference_2system__api.html#autotoc_md467", [
        [ "module_t *diagnostics_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md468", null ],
        [ "esp_err_t diagnostics_module_run_tests(void);", "md_docs_2api__reference_2system__api.html#autotoc_md469", null ],
        [ "esp_err_t diagnostics_module_get_report(char *report_buffer, size_t buffer_size);", "md_docs_2api__reference_2system__api.html#autotoc_md470", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2system__api.html#autotoc_md472", null ],
      [ "მაგალითები", "md_docs_2api__reference_2system__api.html#autotoc_md474", null ]
    ] ],
    [ "Synapse Task Pool Manager API Reference", "md_docs_2api__reference_2task__pool__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2task__pool__api.html#autotoc_md477", null ],
      [ "API-ს ტიპები და სტრუქტურები", "md_docs_2api__reference_2task__pool__api.html#autotoc_md479", [
        [ "<tt>synapse_job_handle_t</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md480", null ],
        [ "<tt>synapse_job_cb</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md481", null ]
      ] ],
      [ "API-ს ფუნქციები", "md_docs_2api__reference_2task__pool__api.html#autotoc_md483", [
        [ "<tt>synapse_job_handle_t synapse_task_pool_schedule_job(synapse_job_cb job_function, void* user_context, uint32_t interval_ms, bool is_periodic);</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md484", null ],
        [ "<tt>esp_err_t synapse_task_pool_cancel_job(synapse_job_handle_t handle);</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md485", null ]
      ] ],
      [ "💡 გამოყენების მაგალითი", "md_docs_2api__reference_2task__pool__api.html#autotoc_md487", null ]
    ] ],
    [ "Synapse Time Sync Module API Reference", "md_docs_2api__reference_2time__sync__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2time__sync__api.html#autotoc_md489", null ],
      [ "Time Sync Module API", "md_docs_2api__reference_2time__sync__api.html#autotoc_md491", [
        [ "module_t *time_sync_module_create(const cJSON *config);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md492", null ],
        [ "esp_err_t time_sync_module_start(module_t *module);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md493", null ],
        [ "esp_err_t time_sync_module_stop(module_t *module);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md494", null ],
        [ "esp_err_t time_sync_module_set_server(module_t *module, const char *server_url);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md495", null ],
        [ "esp_err_t time_sync_module_get_time(time_t *current_time);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md496", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2time__sync__api.html#autotoc_md498", [
        [ "static void time_sync_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md499", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2time__sync__api.html#autotoc_md501", null ],
      [ "მაგალითები", "md_docs_2api__reference_2time__sync__api.html#autotoc_md503", null ]
    ] ],
    [ "Synapse Utility Module API Reference", "md_docs_2api__reference_2utility__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2utility__api.html#autotoc_md506", null ],
      [ "Logger Module API", "md_docs_2api__reference_2utility__api.html#autotoc_md508", [
        [ "module_t *logger_module_create(const cJSON *config);", "md_docs_2api__reference_2utility__api.html#autotoc_md509", null ],
        [ "esp_err_t logger_module_log_info(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md510", null ],
        [ "esp_err_t logger_module_log_warning(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md511", null ],
        [ "esp_err_t logger_module_log_error(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md512", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2utility__api.html#autotoc_md514", null ],
      [ "მაგალითები", "md_docs_2api__reference_2utility__api.html#autotoc_md516", null ]
    ] ],
    [ "Synapse Utils API Reference", "md_docs_2api__reference_2utils__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2utils__api.html#autotoc_md519", null ],
      [ "1. უსაფრთხო სტრიქონების მართვა", "md_docs_2api__reference_2utils__api.html#autotoc_md521", [
        [ "<tt>char* synapse_safe_strncpy(char *dest, const char *src, size_t size);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md522", null ]
      ] ],
      [ "2. კონფიგურაციის პარსინგის დამხმარე ფუნქციები", "md_docs_2api__reference_2utils__api.html#autotoc_md524", [
        [ "<tt>bool synapse_config_get_string_from_node(const char* tag, const cJSON *json_node, const char *key, char *dest, size_t dest_size);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md525", null ],
        [ "<tt>bool synapse_config_get_int_from_node(const char* tag, const cJSON *json_node, const char *key, int *out_value);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md526", null ],
        [ "<tt>bool synapse_config_get_bool_from_node(const char* tag, const cJSON *json_node, const char *key, bool *out_value);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md527", null ]
      ] ],
      [ "3. Guard Clause & Validation Helpers", "md_docs_2api__reference_2utils__api.html#autotoc_md528", [
        [ "<tt>SYNAPSE_GUARD(condition, tag, ret_val, format, ...)</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md529", null ],
        [ "<tt>SYNAPSE_GUARD_VOID(condition, tag, format, ...)</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md530", null ]
      ] ]
    ] ],
    [ "Synapse Version Module API Reference", "md_docs_2api__reference_2version__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2version__api.html#autotoc_md532", null ],
      [ "Version Module API", "md_docs_2api__reference_2version__api.html#autotoc_md534", [
        [ "module_t *version_module_create(const cJSON *config);", "md_docs_2api__reference_2version__api.html#autotoc_md535", null ],
        [ "esp_err_t version_module_get_firmware_version(char *version_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md536", null ],
        [ "esp_err_t version_module_get_build_date(char *date_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md537", null ],
        [ "esp_err_t version_module_get_build_time(char *time_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md538", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2version__api.html#autotoc_md540", null ],
      [ "მაგალითები", "md_docs_2api__reference_2version__api.html#autotoc_md542", null ]
    ] ],
    [ "Synapse WiFi Manager Service API Reference", "md_docs_2api__reference_2wifi__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2wifi__api.html#autotoc_md545", null ],
      [ "WiFi Manager API", "md_docs_2api__reference_2wifi__api.html#autotoc_md547", [
        [ "სამომავლო გაფართოება", "md_docs_2api__reference_2wifi__api.html#autotoc_md548", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2wifi__api.html#autotoc_md550", null ],
      [ "მაგალითები", "md_docs_2api__reference_2wifi__api.html#autotoc_md552", null ]
    ] ],
    [ "CHANGELOG: led_blinker ინიციალიზაციის პრობლემის დიაგნოსტიკა და გამოსწორება", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html", [
      [ "პრობლემა", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md555", null ],
      [ "დიაგნოსტიკა", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md556", null ],
      [ "მიზეზი", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md557", null ],
      [ "გამოსავალი", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md558", null ],
      [ "ცვლილებები კოდში", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md559", null ],
      [ "რეკომენდაცია", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md560", null ]
    ] ],
    [ "ცვლილებების ჟურნალი — 2025-06-24", "md_docs_2changelog_2CHANGELOG__2025-06-24.html", [
      [ "დამატებული და განახლებული ფაილები", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md563", [
        [ "[core/CMakeLists.txt]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md564", null ],
        [ "[core/collect_modules.cmake]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md565", null ],
        [ "[core/include/system_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md566", null ],
        [ "[interfaces/include/base_module.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md567", null ],
        [ "[core/include/service_locator.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md568", null ],
        [ "[core/include/module_registry.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md569", null ],
        [ "[core/include/event_bus.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md570", null ],
        [ "[core/include/config_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md571", null ],
        [ "[core/include/system_event_ids.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md572", null ],
        [ "[core/include/resource_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md573", null ],
        [ "[core/include/framework_config.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md574", null ],
        [ "[interfaces/include/logging.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md575", null ],
        [ "[interfaces/include/service_types.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md576", null ],
        [ "[interfaces/include/telemetry_events.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md577", null ],
        [ "[README.md]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md578", null ],
        [ "[.gitignore]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md579", null ]
      ] ],
      [ "ძირითადი ცვლილებები", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md581", null ]
    ] ],
    [ "CHANGELOG - 2025-06-26", "md_docs_2changelog_2CHANGELOG__2025-06-26.html", [
      [ "[Major] Module System Automation & Refactor", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md584", [
        [ "Added", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md585", null ],
        [ "Changed", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md586", null ],
        [ "Fixed", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md587", null ],
        [ "Detailed File Changes", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md588", null ],
        [ "Why/How", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md589", null ],
        [ "Testing", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md590", null ]
      ] ]
    ] ],
    [ "Changelog", "md_docs_2changelog_2CHANGELOG__2025-06-28.html", [
      [ "[2.0.0] - 2025-06-28", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md592", [
        [ "დამატებულია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md593", null ],
        [ "შეცვლილია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md594", null ],
        [ "ცნობილი პრობლემები", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md595", null ]
      ] ]
    ] ],
    [ "CHANGELOG", "md_docs_2changelog_2v10.html", [
      [ "[10.3.1] - 2025-09-17", "md_docs_2changelog_2v10.html#autotoc_md597", [
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v10.html#autotoc_md598", null ]
      ] ],
      [ "[10.3.0] - 2025-09-15", "md_docs_2changelog_2v10.html#autotoc_md600", [
        [ "✨ დამატებული ფუნქციონალი და DX (Added Features & Developer Experience)", "md_docs_2changelog_2v10.html#autotoc_md601", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v10.html#autotoc_md602", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v10.html#autotoc_md603", null ]
      ] ],
      [ "[10.2.0] - 2025-09-14", "md_docs_2changelog_2v10.html#autotoc_md605", [
        [ "✨ დამატებული ფუნქციონალი და DX (Added Features & Developer Experience)", "md_docs_2changelog_2v10.html#autotoc_md606", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v10.html#autotoc_md607", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v10.html#autotoc_md608", null ]
      ] ],
      [ "[10.1.0] - 2025-09-13", "md_docs_2changelog_2v10.html#autotoc_md610", [
        [ "✨ დამატებული ფუნქციონალი და DX (Added Features & Developer Experience)", "md_docs_2changelog_2v10.html#autotoc_md611", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v10.html#autotoc_md612", null ]
      ] ],
      [ "[10.0.0] - 2025-09-12", "md_docs_2changelog_2v10.html#autotoc_md614", [
        [ "💥 არქიტექტურული რევოლუცია (Breaking Architectural Changes)", "md_docs_2changelog_2v10.html#autotoc_md615", null ],
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v10.html#autotoc_md616", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed & Improved)", "md_docs_2changelog_2v10.html#autotoc_md617", null ],
        [ "🗑️ მოძველებული (Deprecated)", "md_docs_2changelog_2v10.html#autotoc_md618", null ]
      ] ]
    ] ],
    [ "ცვლილებების ისტორია (Changelog)", "md_docs_2changelog_2v5.html", [
      [ "[5.8.0] - 2025-07-31", "md_docs_2changelog_2v5.html#autotoc_md621", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md622", null ],
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md623", null ]
      ] ],
      [ "[5.7.0] - 2025-07-30", "md_docs_2changelog_2v5.html#autotoc_md625", [
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md626", null ]
      ] ],
      [ "[5.6.0] - 2025-08-16", "md_docs_2changelog_2v5.html#autotoc_md627", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md628", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md629", null ]
      ] ],
      [ "[5.5.1] - 2025-08-16", "md_docs_2changelog_2v5.html#autotoc_md630", [
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md631", null ]
      ] ],
      [ "[5.5.0] - 2025-08-15", "md_docs_2changelog_2v5.html#autotoc_md633", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md634", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md635", null ],
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md636", null ]
      ] ],
      [ "[5.4.0] - 2025-08-14", "md_docs_2changelog_2v5.html#autotoc_md638", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md639", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md640", null ],
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md641", null ]
      ] ],
      [ "[5.3.0] - 2025-07-29", "md_docs_2changelog_2v5.html#autotoc_md642", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md643", null ],
        [ "🔄 Changed (შეცვლილი და გაუმჯობესებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md644", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md645", null ]
      ] ],
      [ "[5.2.0] - 2025-07-28", "md_docs_2changelog_2v5.html#autotoc_md647", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md648", null ],
        [ "🔄 Changed (შეცვლილი და გაუმჯობესებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md649", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md650", null ]
      ] ],
      [ "[5.1.0] - 2025-07-28", "md_docs_2changelog_2v5.html#autotoc_md652", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md653", null ],
        [ "🔄 Changed (შეცვლილი და გაუმჯობესებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md654", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md655", null ]
      ] ],
      [ "[5.0.1] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md657", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md658", null ],
        [ "🔄 Changed (შეცვლილი ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md659", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md660", null ]
      ] ],
      [ "[5.0.0] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md661", [
        [ "💥 Breaking Changes", "md_docs_2changelog_2v5.html#autotoc_md662", null ],
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md663", null ],
        [ "🔄 Changed (შეცვლილი ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md664", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md665", null ]
      ] ],
      [ "[4.2.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md666", [
        [ "✨ დამატებულია (Added)", "md_docs_2changelog_2v5.html#autotoc_md667", null ],
        [ "🔄 შეცვლილია (Changed)", "md_docs_2changelog_2v5.html#autotoc_md668", null ],
        [ "🐞 გასწორებულია (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md669", null ]
      ] ],
      [ "[v4.1.0] - 2025-07-19", "md_docs_2changelog_2v5.html#autotoc_md670", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md671", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md672", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md673", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md674", null ]
      ] ],
      [ "[v4.0.0] - 2025-07-19", "md_docs_2changelog_2v5.html#autotoc_md675", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md676", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md677", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md678", null ]
      ] ],
      [ "[v3.9.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md679", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md680", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md681", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md682", null ]
      ] ],
      [ "[v3.8.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md683", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md684", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md685", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md686", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md687", null ]
      ] ],
      [ "[v3.7.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md688", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md689", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md690", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md691", null ]
      ] ],
      [ "[v3.6.0] - 2025-07-17", "md_docs_2changelog_2v5.html#autotoc_md692", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md693", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md694", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md695", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md696", null ]
      ] ],
      [ "[v3.5.0] - 2025-07-16", "md_docs_2changelog_2v5.html#autotoc_md697", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md698", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md699", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md700", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md701", null ]
      ] ],
      [ "[v3.4.0] - 2025-07-13", "md_docs_2changelog_2v5.html#autotoc_md702", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md703", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md704", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md705", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md706", null ]
      ] ],
      [ "[v3.3.0] - 2025-07-10", "md_docs_2changelog_2v5.html#autotoc_md707", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md708", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md709", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md710", null ]
      ] ],
      [ "[v3.2.1] - 2025-07-08", "md_docs_2changelog_2v5.html#autotoc_md711", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md712", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md713", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md714", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md715", null ]
      ] ],
      [ "[v3.2.0] - 2025-07-08", "md_docs_2changelog_2v5.html#autotoc_md716", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md717", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md718", null ]
      ] ],
      [ "[v3.1.0] - 2025-07-06", "md_docs_2changelog_2v5.html#autotoc_md719", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md720", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md721", null ]
      ] ],
      [ "[v3.0.0] - 2025-07-05", "md_docs_2changelog_2v5.html#autotoc_md722", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md723", null ],
        [ "♻️ რეფაქტორინგი და გაუმჯობესებები (Refactoring & Improvements)", "md_docs_2changelog_2v5.html#autotoc_md724", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md725", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md726", null ]
      ] ],
      [ "[2.8.0] - 2025-07-05", "md_docs_2changelog_2v5.html#autotoc_md727", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md728", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md729", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md730", null ]
      ] ],
      [ "[2.7.0] - 2025-07-05", "md_docs_2changelog_2v5.html#autotoc_md731", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md732", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md733", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md734", null ]
      ] ],
      [ "[2.6.0] - 2025-07-04", "md_docs_2changelog_2v5.html#autotoc_md735", [
        [ "✨ დამატებულია", "md_docs_2changelog_2v5.html#autotoc_md736", null ],
        [ "🛠️ შეცვლილია", "md_docs_2changelog_2v5.html#autotoc_md737", null ],
        [ "🐛 გასწორებულია", "md_docs_2changelog_2v5.html#autotoc_md738", null ]
      ] ],
      [ "[v2.5.0] - 2025-07-02", "md_docs_2changelog_2v5.html#autotoc_md739", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md740", null ]
      ] ],
      [ "[v2.4.0] - 2025-07-02", "md_docs_2changelog_2v5.html#autotoc_md741", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md742", null ],
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md743", null ]
      ] ],
      [ "[v2.3.0] - 2025-07-02", "md_docs_2changelog_2v5.html#autotoc_md745", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md746", null ],
        [ "🏗️ ინფრასტრუქტურა", "md_docs_2changelog_2v5.html#autotoc_md747", null ]
      ] ],
      [ "[v2.2.0] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md748", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md749", null ],
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md750", null ]
      ] ],
      [ "[v2.1.1] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md751", [
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md752", null ],
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md753", null ]
      ] ],
      [ "[v2.1.0] - 2025-06-30", "md_docs_2changelog_2v5.html#autotoc_md755", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md756", null ],
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md757", null ],
        [ "♻️ რეფაქტორინგი", "md_docs_2changelog_2v5.html#autotoc_md758", null ]
      ] ]
    ] ],
    [ "CHANGELOG", "md_docs_2changelog_2v6.html", [
      [ "[6.2.0] - 2025-08-24", "md_docs_2changelog_2v6.html#autotoc_md760", [
        [ "✨ დამატებული", "md_docs_2changelog_2v6.html#autotoc_md761", null ],
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v6.html#autotoc_md762", null ]
      ] ],
      [ "[6.1.0] - 2025-08-22", "md_docs_2changelog_2v6.html#autotoc_md764", null ],
      [ "✨ დამატებული", "md_docs_2changelog_2v6.html#autotoc_md765", [
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v6.html#autotoc_md766", null ],
        [ "📄 დოკუმენტაცია", "md_docs_2changelog_2v6.html#autotoc_md767", null ]
      ] ],
      [ "[6.0.0] - 2025-08-21", "md_docs_2changelog_2v6.html#autotoc_md769", [
        [ "✨ დამატებული", "md_docs_2changelog_2v6.html#autotoc_md770", null ],
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v6.html#autotoc_md771", null ],
        [ "📄 დოკუმენტაცია", "md_docs_2changelog_2v6.html#autotoc_md772", null ]
      ] ]
    ] ],
    [ "[7.2.0] - 2025-08-28", "md_docs_2changelog_2v7.html", [
      [ "[7.1.0] - 2025-08-27", "md_docs_2changelog_2v7.html#autotoc_md780", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v7.html#autotoc_md775", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v7.html#autotoc_md776", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v7.html#autotoc_md777", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v7.html#autotoc_md778", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v7.html#autotoc_md781", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v7.html#autotoc_md782", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v7.html#autotoc_md783", null ]
      ] ],
      [ "[7.0.0] - 2025-08-26", "md_docs_2changelog_2v7.html#autotoc_md785", [
        [ "💥 არქიტექტურული ცვლილებები (Breaking Changes & Refactoring)", "md_docs_2changelog_2v7.html#autotoc_md786", null ],
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v7.html#autotoc_md787", [
          [ "ძველი ვერსიების ისტორია", "md_docs_2changelog_2v7.html#autotoc_md789", null ]
        ] ]
      ] ]
    ] ],
    [ "[8.1.0] - 2025-08-30", "md_docs_2changelog_2v8.html", [
      [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v8.html#autotoc_md791", [
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v8.html#autotoc_md792", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v8.html#autotoc_md793", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v8.html#autotoc_md794", null ]
      ] ],
      [ "[8.0.0] - 2025-08-07", "md_docs_2changelog_2v8.html#autotoc_md796", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v8.html#autotoc_md797", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v8.html#autotoc_md798", null ]
      ] ]
    ] ],
    [ "CHANGELOG", "md_docs_2changelog_2v9.html", [
      [ "[9.5.3] - 2025-09-11", "md_docs_2changelog_2v9.html#autotoc_md801", [
        [ "✨ არქიტექტურული გაუმჯობესებები და DX (Architectural & Developer Experience Improvements)", "md_docs_2changelog_2v9.html#autotoc_md802", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v9.html#autotoc_md803", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md804", null ]
      ] ],
      [ "[9.5.2] - 2025-08-09", "md_docs_2changelog_2v9.html#autotoc_md806", [
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v9.html#autotoc_md807", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md808", null ]
      ] ],
      [ "[9.5.0] - 2025-08-09", "md_docs_2changelog_2v9.html#autotoc_md810", [
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v9.html#autotoc_md811", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md812", null ]
      ] ],
      [ "[9.4.0] - 2025-08-08", "md_docs_2changelog_2v9.html#autotoc_md814", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v9.html#autotoc_md815", null ]
      ] ],
      [ "[9.3.0] - 2025-08-08", "md_docs_2changelog_2v9.html#autotoc_md817", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v9.html#autotoc_md818", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v9.html#autotoc_md819", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v9.html#autotoc_md820", null ]
      ] ],
      [ "[9.2.0] - 2025-08-08", "md_docs_2changelog_2v9.html#autotoc_md822", [
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v9.html#autotoc_md823", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md824", null ]
      ] ],
      [ "[9.1.0] - 2025-08-08", "md_docs_2changelog_2v9.html#autotoc_md826", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v9.html#autotoc_md827", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md828", null ]
      ] ],
      [ "[9.0.0] - 2025-08-07", "md_docs_2changelog_2v9.html#autotoc_md830", [
        [ "💥 არქიტექტურული ცვლილებები (Breaking Changes & Refactoring)", "md_docs_2changelog_2v9.html#autotoc_md831", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v9.html#autotoc_md832", null ]
      ] ]
    ] ],
    [ "Synapse Branching Strategy", "md_docs_2contributing_2branching__strategy.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2branching__strategy.html#autotoc_md836", null ],
      [ "ძირითადი ბრენჩები", "md_docs_2contributing_2branching__strategy.html#autotoc_md838", null ],
      [ "ბრენჩის სახელდების წესები", "md_docs_2contributing_2branching__strategy.html#autotoc_md840", null ],
      [ "სამუშაო პროცესი", "md_docs_2contributing_2branching__strategy.html#autotoc_md842", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2contributing_2branching__strategy.html#autotoc_md844", null ],
      [ "რესურსები", "md_docs_2contributing_2branching__strategy.html#autotoc_md846", null ]
    ] ],
    [ "ქცევის კოდექსი (Code of Conduct)", "md_docs_2contributing_2code__of__conduct.html", [
      [ "მიზანი", "md_docs_2contributing_2code__of__conduct.html#autotoc_md850", null ],
      [ "ძირითადი პრინციპები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md852", null ],
      [ "დაუშვებელი ქცევა", "md_docs_2contributing_2code__of__conduct.html#autotoc_md854", null ],
      [ "პასუხისმგებლობები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md856", null ],
      [ "დარღვევის შემთხვევაში მოქმედება", "md_docs_2contributing_2code__of__conduct.html#autotoc_md858", null ],
      [ "დამატებითი რესურსები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md860", null ]
    ] ],
    [ "Synapse Commit Message Guidelines", "md_docs_2contributing_2commit__message__guidelines.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md864", null ],
      [ "ძირითადი სტრუქტურა", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md866", null ],
      [ "სათაურის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md868", null ],
      [ "დეტალური აღწერის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md870", null ],
      [ "Footer-ის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md872", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md874", null ],
      [ "რესურსები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md876", null ]
    ] ],
    [ "კონტრიბუციის სახელმძღვანელო (Contributing Guide)", "md_docs_2contributing_2contributing__guide.html", [
      [ "მიზანი", "md_docs_2contributing_2contributing__guide.html#autotoc_md879", null ],
      [ "1. კონტრიბუციის პროცესი", "md_docs_2contributing_2contributing__guide.html#autotoc_md881", null ],
      [ "2. კოდის სტანდარტები", "md_docs_2contributing_2contributing__guide.html#autotoc_md882", null ],
      [ "3. დოკუმენტაციის მოთხოვნები", "md_docs_2contributing_2contributing__guide.html#autotoc_md883", null ],
      [ "4. Pull Request-ის მიმოხილვა", "md_docs_2contributing_2contributing__guide.html#autotoc_md884", null ],
      [ "5. Best Practices", "md_docs_2contributing_2contributing__guide.html#autotoc_md885", null ],
      [ "6. აკრძალული პრაქტიკები", "md_docs_2contributing_2contributing__guide.html#autotoc_md886", null ]
    ] ],
    [ "Synapse Contributing Reference Index", "md_docs_2contributing_2contributing__index.html", [
      [ "ძირითადი სახელმძღვანელოები", "md_docs_2contributing_2contributing__index.html#autotoc_md890", null ],
      [ "შაბლონები", "md_docs_2contributing_2contributing__index.html#autotoc_md891", null ],
      [ "ინსტრუმენტები", "md_docs_2contributing_2contributing__index.html#autotoc_md892", null ]
    ] ],
    [ "კონვენციების ავტომატური შემმოწმებელი (<tt>check_conventions.py</tt>)", "md_docs_2contributing_2convention__linter__guide.html", [
      [ "1. შესავალი", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md894", null ],
      [ "2. რას ამოწმებს სკრიპტი?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md895", null ],
      [ "3. როგორ გამოვიყენოთ ლოკალურად?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md900", null ],
      [ "4. ინტეგრაცია GitHub Actions-თან (ავტომატური შემოწმება)", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md902", null ],
      [ "5. როგორ გამოვასწოროთ გავრცელებული შეცდომები?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md903", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (FAQ)", "md_docs_2contributing_2faq.html", [
      [ "1. როგორ დავიწყო კონტრიბუცია Synapse Framework-ში?", "md_docs_2contributing_2faq.html#autotoc_md906", null ],
      [ "2. რა სტანდარტებს და კონვენციებს უნდა დავიცვა?", "md_docs_2contributing_2faq.html#autotoc_md907", null ],
      [ "3. როგორ შევქმნა Pull Request?", "md_docs_2contributing_2faq.html#autotoc_md908", null ],
      [ "4. როგორ მოვამზადო ლოკალური გარემო?", "md_docs_2contributing_2faq.html#autotoc_md909", null ],
      [ "5. როგორ გავუშვა ტესტები?", "md_docs_2contributing_2faq.html#autotoc_md910", null ],
      [ "6. როგორ მოვაგვარო გარემოს ან ბილდის პრობლემები?", "md_docs_2contributing_2faq.html#autotoc_md911", null ],
      [ "7. სად ვიპოვო დოკუმენტაციის სტრუქტურის და სტანდარტების შესახებ ინფორმაცია?", "md_docs_2contributing_2faq.html#autotoc_md912", null ],
      [ "8. როგორ დავამატო ან განვაახლო დოკუმენტაცია?", "md_docs_2contributing_2faq.html#autotoc_md913", null ],
      [ "9. როგორ დავიცვა ქცევის კოდექსი?", "md_docs_2contributing_2faq.html#autotoc_md914", null ],
      [ "10. ვის მივმართო დამატებითი კითხვების ან პრობლემების შემთხვევაში?", "md_docs_2contributing_2faq.html#autotoc_md915", null ]
    ] ],
    [ "Synapse Issue Template", "md_docs_2contributing_2issue__template.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2issue__template.html#autotoc_md919", null ],
      [ "ძირითადი ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md921", null ],
      [ "დეტალური აღწერა", "md_docs_2contributing_2issue__template.html#autotoc_md923", null ],
      [ "გამეორების ნაბიჯები (თუ ეს ბაგია)", "md_docs_2contributing_2issue__template.html#autotoc_md925", null ],
      [ "მოსალოდნელი შედეგი", "md_docs_2contributing_2issue__template.html#autotoc_md927", null ],
      [ "ფაქტობრივი შედეგი", "md_docs_2contributing_2issue__template.html#autotoc_md929", null ],
      [ "გარემოს ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md931", null ],
      [ "დამატებითი ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md933", null ],
      [ "რესურსები", "md_docs_2contributing_2issue__template.html#autotoc_md935", null ]
    ] ],
    [ "Synapse Local Setup Guide", "md_docs_2contributing_2local__setup.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2local__setup.html#autotoc_md939", null ],
      [ "სისტემური მოთხოვნები", "md_docs_2contributing_2local__setup.html#autotoc_md941", null ],
      [ "ESP-IDF-ის ინსტალაცია", "md_docs_2contributing_2local__setup.html#autotoc_md943", null ],
      [ "პროექტის კლონირება", "md_docs_2contributing_2local__setup.html#autotoc_md945", null ],
      [ "sdkconfig ფაილის კონფიგურაცია", "md_docs_2contributing_2local__setup.html#autotoc_md947", null ],
      [ "პროექტის აგება და გაშვება", "md_docs_2contributing_2local__setup.html#autotoc_md949", null ],
      [ "ტესტირება", "md_docs_2contributing_2local__setup.html#autotoc_md951", null ],
      [ "პრობლემების შემთხვევაში", "md_docs_2contributing_2local__setup.html#autotoc_md953", null ],
      [ "რესურსები", "md_docs_2contributing_2local__setup.html#autotoc_md955", null ]
    ] ],
    [ "მოდულების მართვა Git Submodule-ის საშუალებით", "md_docs_2contributing_2managing__modules.html", [
      [ "1. 🏛️ არქიტექტურული პრინციპი", "md_docs_2contributing_2managing__modules.html#autotoc_md958", null ],
      [ "2. 🚀 ახალი მოდულის დამატება", "md_docs_2contributing_2managing__modules.html#autotoc_md959", [
        [ "ეტაპი 1: მოდულის რეპოზიტორიის მომზადება", "md_docs_2contributing_2managing__modules.html#autotoc_md960", null ],
        [ "ეტაპი 2: Submodule-ის დამატება მთავარ პროექტში", "md_docs_2contributing_2managing__modules.html#autotoc_md961", null ],
        [ "⚙️ Build სისტემასთან ინტეგრაცია", "md_docs_2contributing_2managing__modules.html#autotoc_md962", null ]
      ] ],
      [ "3. 🔄 პროექტის კლონირება Submodule-ებთან ერთად", "md_docs_2contributing_2managing__modules.html#autotoc_md963", null ],
      [ "4. 📈 Submodule-ის განახლება", "md_docs_2contributing_2managing__modules.html#autotoc_md964", null ],
      [ "5. 💡 დამატებითი რჩევები", "md_docs_2contributing_2managing__modules.html#autotoc_md965", null ]
    ] ],
    [ "Synapse Pull Request Template", "md_docs_2contributing_2pull__request__template.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2pull__request__template.html#autotoc_md968", null ],
      [ "ძირითადი ინფორმაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md970", null ],
      [ "ცვლილებების დეტალური აღწერა", "md_docs_2contributing_2pull__request__template.html#autotoc_md972", null ],
      [ "ტესტირება და ვალიდაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md974", null ],
      [ "მიმოხილვის რეკომენდაციები", "md_docs_2contributing_2pull__request__template.html#autotoc_md976", null ],
      [ "დამატებითი ინფორმაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md978", null ],
      [ "რესურსები", "md_docs_2contributing_2pull__request__template.html#autotoc_md980", null ]
    ] ],
    [ "Synapse Review Guidelines", "md_docs_2contributing_2review__guidelines.html", [
      [ "მიმოხილვის მიზანი", "md_docs_2contributing_2review__guidelines.html#autotoc_md984", null ],
      [ "მიმოხილვის პროცესი", "md_docs_2contributing_2review__guidelines.html#autotoc_md986", null ],
      [ "მიმოხილვის კრიტერიუმები", "md_docs_2contributing_2review__guidelines.html#autotoc_md988", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2contributing_2review__guidelines.html#autotoc_md990", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2contributing_2review__guidelines.html#autotoc_md992", null ],
      [ "რესურსები", "md_docs_2contributing_2review__guidelines.html#autotoc_md994", null ]
    ] ],
    [ "Assertions და Guard Clauses კონვენცია", "md_docs_2convention_2assertion__and__guards.html", [
      [ "1. 🎯 მიზანი", "md_docs_2convention_2assertion__and__guards.html#autotoc_md997", null ],
      [ "2. 🤔 ფუნდამენტური განსხვავება: Assertion vs. Error Handling", "md_docs_2convention_2assertion__and__guards.html#autotoc_md998", null ],
      [ "3. <tt>SYNAPSE_ASSERT</tt> მაკრო", "md_docs_2convention_2assertion__and__guards.html#autotoc_md999", [
        [ "3.1. როდის გამოვიყენოთ?", "md_docs_2convention_2assertion__and__guards.html#autotoc_md1000", null ],
        [ "3.2. გამოყენების მაგალითები", "md_docs_2convention_2assertion__and__guards.html#autotoc_md1001", null ],
        [ "3.3. ქცევა <tt>release</tt> რეჟიმში", "md_docs_2convention_2assertion__and__guards.html#autotoc_md1002", null ]
      ] ],
      [ "4. <tt>SYNAPSE_GUARD</tt> მაკროები", "md_docs_2convention_2assertion__and__guards.html#autotoc_md1003", [
        [ "4.1. როდის გამოვიყენოთ?", "md_docs_2convention_2assertion__and__guards.html#autotoc_md1004", null ],
        [ "4.2. გამოყენების მაგალითები", "md_docs_2convention_2assertion__and__guards.html#autotoc_md1005", null ],
        [ "4.3. როდის <strong>არ</strong> გამოვიყენოთ <tt>SYNAPSE_GUARD</tt>?", "md_docs_2convention_2assertion__and__guards.html#autotoc_md1006", null ]
      ] ]
    ] ],
    [ "9. Doxygen კომენტარების სტანდარტი (v1.0)", "md_docs_2convention_2coding__doxygen__standard.html", [
      [ "1. შესავალი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1008", null ],
      [ "2. ფაილის სათაურის (Header) კომენტარი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1009", [
        [ "2.1. სტრუქტურა", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1010", null ],
        [ "2.2. მაგალითი (<tt>i2c_bus_module.h</tt>)", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1011", null ]
      ] ],
      [ "3. ფუნქციის კომენტარი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1012", [
        [ "3.1. სტრუქტურა", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1013", null ],
        [ "3.2. მაგალითი (<tt>service_locator.h</tt>)", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1014", null ]
      ] ],
      [ "4. სტრუქტურების და ტიპების კომენტარები", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1015", [
        [ "4.1. სტრუქტურის მაგალითი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1016", null ]
      ] ],
      [ "5. Doxygen ტეგების გამოყენების წესები", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1017", null ]
    ] ],
    [ "კომუნიკაციის პატერნების წესები (v2.0)", "md_docs_2convention_2communication__patterns.html", [
      [ "ძირითადი პრინციპი: მაქსიმალური იზოლაცია და პროგნოზირებადი გაშვება", "md_docs_2convention_2communication__patterns.html#autotoc_md1019", null ],
      [ "ოთხი ძირითადი პატერნი", "md_docs_2convention_2communication__patterns.html#autotoc_md1020", [
        [ "1. Service Locator Pattern (State-Aware)", "md_docs_2convention_2communication__patterns.html#autotoc_md1021", null ],
        [ "2. Event Bus Pattern", "md_docs_2convention_2communication__patterns.html#autotoc_md1022", null ],
        [ "3. Command Router Pattern", "md_docs_2convention_2communication__patterns.html#autotoc_md1023", null ],
        [ "4. Promise Pattern (ახალი)", "md_docs_2convention_2communication__patterns.html#autotoc_md1024", null ]
      ] ],
      [ "კომუნიკაციის ტიპები", "md_docs_2convention_2communication__patterns.html#autotoc_md1025", [
        [ "Direct API Communication (Service Locator)", "md_docs_2convention_2communication__patterns.html#autotoc_md1026", null ],
        [ "Broadcast Communication (Event Bus)", "md_docs_2convention_2communication__patterns.html#autotoc_md1027", null ]
      ] ],
      [ "აკრძალული პატერნები", "md_docs_2convention_2communication__patterns.html#autotoc_md1028", [
        [ "❌ პირდაპირი დამოკიდებულებები", "md_docs_2convention_2communication__patterns.html#autotoc_md1029", null ],
        [ "❌ პატერნების არასწორი შერევა", "md_docs_2convention_2communication__patterns.html#autotoc_md1030", null ],
        [ "❌ System Manager MQTT Handling", "md_docs_2convention_2communication__patterns.html#autotoc_md1031", null ]
      ] ],
      [ "სწორი იმპლემენტაცია", "md_docs_2convention_2communication__patterns.html#autotoc_md1032", [
        [ "Service Locator-ის გამოყენება (ახალი პატერნი)", "md_docs_2convention_2communication__patterns.html#autotoc_md1033", [
          [ "1. Service Registration (<tt>_create</tt> ფუნქციაში)", "md_docs_2convention_2communication__patterns.html#autotoc_md1034", null ],
          [ "2. Service Usage (სხვა მოდულში)", "md_docs_2convention_2communication__patterns.html#autotoc_md1035", null ]
        ] ],
        [ "Event Bus-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md1036", [
          [ "1. Event Publishing (Sensor Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md1037", null ],
          [ "2. Event Subscription (MQTT Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md1038", null ]
        ] ]
      ] ],
      [ "დინამიური MQTT თემები <tt>{module_name}</tt> Placeholder-ით", "md_docs_2convention_2communication__patterns.html#autotoc_md1039", [
        [ "პატერნის აღწერა", "md_docs_2convention_2communication__patterns.html#autotoc_md1040", null ]
      ] ],
      [ "პატერნის არჩევის გზამკვლევი", "md_docs_2convention_2communication__patterns.html#autotoc_md1041", [
        [ "Service Locator-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md1042", null ],
        [ "Event Bus-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md1043", null ]
      ] ],
      [ "Architecture Diagram", "md_docs_2convention_2communication__patterns.html#autotoc_md1044", null ],
      [ "შეჯამება", "md_docs_2convention_2communication__patterns.html#autotoc_md1045", null ]
    ] ],
    [ "კონფიგურაციის მართვის კონვენცია", "md_docs_2convention_2configuration__management.html", [
      [ "1. 📜 ძირითადი პრინციპი", "md_docs_2convention_2configuration__management.html#autotoc_md1047", null ],
      [ "2. 📂 კონფიგურაციის წყაროები და იერარქია", "md_docs_2convention_2configuration__management.html#autotoc_md1048", null ],
      [ "3. 🏗️ კონფიგურაციის სტრუქტურა", "md_docs_2convention_2configuration__management.html#autotoc_md1049", null ],
      [ "4. 📝 კონფიგურაციის პარსინგის პატერნი", "md_docs_2convention_2configuration__management.html#autotoc_md1050", [
        [ "<tt>parse_config</tt> ფუნქციის 3-ნაბიჯიანი ლოგიკა", "md_docs_2convention_2configuration__management.html#autotoc_md1051", null ],
        [ "<tt>parse_config</tt> ფუნქციის მაგალითი", "md_docs_2convention_2configuration__management.html#autotoc_md1052", null ]
      ] ],
      [ "5. 🌐 კონფიგურაციაზე წვდომა სხვა მოდულებიდან", "md_docs_2convention_2configuration__management.html#autotoc_md1053", null ],
      [ "6. ❌ აკრძალული პრაქტიკები", "md_docs_2convention_2configuration__management.html#autotoc_md1054", null ],
      [ "7. ✅ საუკეთესო პრაქტიკები", "md_docs_2convention_2configuration__management.html#autotoc_md1055", null ]
    ] ],
    [ "Synapse Convention Reference Index", "md_docs_2convention_2convention__index.html", [
      [ "Coding & Documentation", "md_docs_2convention_2convention__index.html#autotoc_md1058", null ],
      [ "Naming & Structure", "md_docs_2convention_2convention__index.html#autotoc_md1059", null ],
      [ "Patterns & Management", "md_docs_2convention_2convention__index.html#autotoc_md1060", null ]
    ] ],
    [ "Error Handling-ის წესები", "md_docs_2convention_2error__handling.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2error__handling.html#autotoc_md1063", [
        [ "1. ESP Error Codes გამოყენება", "md_docs_2convention_2error__handling.html#autotoc_md1064", null ],
        [ "2. Comprehensive Error Checking", "md_docs_2convention_2error__handling.html#autotoc_md1065", null ]
      ] ],
      [ "Standard Error Codes", "md_docs_2convention_2error__handling.html#autotoc_md1066", [
        [ "Framework-Specific Errors", "md_docs_2convention_2error__handling.html#autotoc_md1067", null ],
        [ "Common ESP Error Codes", "md_docs_2convention_2error__handling.html#autotoc_md1068", null ]
      ] ],
      [ "Error Handling Patterns", "md_docs_2convention_2error__handling.html#autotoc_md1069", [
        [ "1. Parameter Validation", "md_docs_2convention_2error__handling.html#autotoc_md1070", null ],
        [ "2. Resource Allocation Checking", "md_docs_2convention_2error__handling.html#autotoc_md1071", null ],
        [ "3. Service Locator Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md1072", null ],
        [ "4. Configuration Parsing Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md1073", null ],
        [ "5. Hardware Operation Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md1074", null ]
      ] ],
      [ "Cleanup and Resource Management", "md_docs_2convention_2error__handling.html#autotoc_md1075", [
        [ "1. Proper Cleanup on Errors", "md_docs_2convention_2error__handling.html#autotoc_md1076", null ],
        [ "2. Resource Cleanup Functions", "md_docs_2convention_2error__handling.html#autotoc_md1077", null ]
      ] ],
      [ "Logging Standards", "md_docs_2convention_2error__handling.html#autotoc_md1078", [
        [ "1. Error Levels", "md_docs_2convention_2error__handling.html#autotoc_md1079", null ],
        [ "2. Error Context Information", "md_docs_2convention_2error__handling.html#autotoc_md1080", null ]
      ] ],
      [ "Error Recovery Strategies", "md_docs_2convention_2error__handling.html#autotoc_md1081", [
        [ "1. Retry Logic", "md_docs_2convention_2error__handling.html#autotoc_md1082", null ],
        [ "2. Graceful Degradation", "md_docs_2convention_2error__handling.html#autotoc_md1083", null ]
      ] ],
      [ "შეჯამება", "md_docs_2convention_2error__handling.html#autotoc_md1084", null ]
    ] ],
    [ "ფუნქციების დასახელების წესები", "md_docs_2convention_2function__naming.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2function__naming.html#autotoc_md1086", [
        [ "1. სტრუქტურა", "md_docs_2convention_2function__naming.html#autotoc_md1087", null ],
        [ "2. Scope პრეფიქსები", "md_docs_2convention_2function__naming.html#autotoc_md1088", [
          [ "Public API ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1089", null ],
          [ "Static/Private ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1090", null ],
          [ "Framework API ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1091", null ]
        ] ]
      ] ],
      [ "მოდულის ტიპების მიხედვით", "md_docs_2convention_2function__naming.html#autotoc_md1092", [
        [ "1. Service Locator", "md_docs_2convention_2function__naming.html#autotoc_md1093", null ],
        [ "2. Event Bus", "md_docs_2convention_2function__naming.html#autotoc_md1094", null ],
        [ "3. Display Modules", "md_docs_2convention_2function__naming.html#autotoc_md1095", null ],
        [ "4. Communication Modules", "md_docs_2convention_2function__naming.html#autotoc_md1096", null ],
        [ "5. Sensor Modules", "md_docs_2convention_2function__naming.html#autotoc_md1097", null ]
      ] ],
      [ "სპეციალური კონვენციები", "md_docs_2convention_2function__naming.html#autotoc_md1098", [
        [ "1. Module Lifecycle ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1099", null ],
        [ "2. Runtime Management ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1100", null ],
        [ "3. Event Handler ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1101", null ],
        [ "4. Task ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1102", null ]
      ] ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2function__naming.html#autotoc_md1103", null ],
      [ "მაგალითი სრული მოდული", "md_docs_2convention_2function__naming.html#autotoc_md1104", null ]
    ] ],
    [ "🚀 მოდულების ინიციალიზაციის დონეები (Initialization Levels)", "md_docs_2convention_2initialization__levels.html", [
      [ "🎯 მიზანი", "md_docs_2convention_2initialization__levels.html#autotoc_md1106", null ],
      [ "⚙️ როგორ მუშაობს?", "md_docs_2convention_2initialization__levels.html#autotoc_md1107", null ],
      [ "📜 ოქროს წესი", "md_docs_2convention_2initialization__levels.html#autotoc_md1108", [
        [ "💡 პრაქტიკული მაგალითი: <tt>wifi_manager</tt> და <tt>command_router</tt>", "md_docs_2convention_2initialization__levels.html#autotoc_md1109", null ]
      ] ],
      [ "📊 ინიციალიზაციის დონეების რეკომენდებული დიაპაზონები", "md_docs_2convention_2initialization__levels.html#autotoc_md1110", [
        [ "📦 <strong>დონე 1: ფუნდამენტური დრაივერები და რესურსები (0-19)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1112", null ],
        [ "🛠️ <strong>დონე 2: საბაზისო სისტემური სერვისები (20-39)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1114", null ],
        [ "📡 <strong>დონე 3: ქსელი, კონტროლი და კომუნიკაცია (40-59)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1116", null ],
        [ "💡 <strong>დონე 4: აპლიკაციის ლოგიკა და პერიფერია (60-79)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1118", null ],
        [ "🧩 <strong>დონე 5: მაღალი დონის კომპოზიტური მოდულები (80-100)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1120", null ]
      ] ],
      [ "📝 როგორ განვსაზღვროთ <tt>init_level</tt> ახალი მოდულისთვის?", "md_docs_2convention_2initialization__levels.html#autotoc_md1122", null ]
    ] ],
    [ "ლოგირების ფერთა სქემის კონვენცია", "md_docs_2convention_2logging__colors.html", [
      [ "1. 🎯 მიზანი", "md_docs_2convention_2logging__colors.html#autotoc_md1124", null ],
      [ "2. ⚙️ ტექნიკური იმპლემენტაცია", "md_docs_2convention_2logging__colors.html#autotoc_md1125", null ],
      [ "3. 🎨 ფერთა სქემა", "md_docs_2convention_2logging__colors.html#autotoc_md1126", [
        [ "**სრული ფერების სია (<tt>logging.h</tt>-დან)**", "md_docs_2convention_2logging__colors.html#autotoc_md1128", null ]
      ] ]
    ] ],
    [ "Logging-ის სტანდარტები", "md_docs_2convention_2logging__standards.html", [
      [ "Component Tag Definition", "md_docs_2convention_2logging__standards.html#autotoc_md1130", [
        [ "სტანდარტული ფორმატი", "md_docs_2convention_2logging__standards.html#autotoc_md1131", null ],
        [ "მაგალითები", "md_docs_2convention_2logging__standards.html#autotoc_md1132", null ]
      ] ],
      [ "Log Levels", "md_docs_2convention_2logging__standards.html#autotoc_md1133", [
        [ "1. ESP_LOGE - Error Level", "md_docs_2convention_2logging__standards.html#autotoc_md1134", null ],
        [ "2. ESP_LOGW - Warning Level", "md_docs_2convention_2logging__standards.html#autotoc_md1135", null ],
        [ "3. ESP_LOGI - Info Level", "md_docs_2convention_2logging__standards.html#autotoc_md1136", null ],
        [ "4. ESP_LOGD - Debug Level", "md_docs_2convention_2logging__standards.html#autotoc_md1137", null ],
        [ "5. ESP_LOGV - Verbose Level", "md_docs_2convention_2logging__standards.html#autotoc_md1138", null ]
      ] ],
      [ "Logging Patterns", "md_docs_2convention_2logging__standards.html#autotoc_md1139", [
        [ "1. Module Lifecycle Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1140", null ],
        [ "2. Service Registration Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1141", null ],
        [ "3. Configuration Parsing Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1142", null ],
        [ "4. Hardware Operation Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1143", null ],
        [ "5. Event Handling Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1144", null ],
        [ "6. Error Context Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1145", null ]
      ] ],
      [ "Runtime State Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1146", [
        [ "1. Module Enable/Disable", "md_docs_2convention_2logging__standards.html#autotoc_md1147", null ],
        [ "2. Task State Changes", "md_docs_2convention_2logging__standards.html#autotoc_md1148", null ]
      ] ],
      [ "Performance Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1149", [
        [ "1. Operation Timing", "md_docs_2convention_2logging__standards.html#autotoc_md1150", null ],
        [ "2. Memory Usage", "md_docs_2convention_2logging__standards.html#autotoc_md1151", null ]
      ] ],
      [ "Log Message Format Standards", "md_docs_2convention_2logging__standards.html#autotoc_md1152", [
        [ "1. Consistent Formatting", "md_docs_2convention_2logging__standards.html#autotoc_md1153", null ],
        [ "2. Parameter Naming", "md_docs_2convention_2logging__standards.html#autotoc_md1154", null ]
      ] ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2logging__standards.html#autotoc_md1155", null ],
      [ "შეჯამება", "md_docs_2convention_2logging__standards.html#autotoc_md1156", null ]
    ] ],
    [ "მოდულის სტრუქტურის კონვენცია", "md_docs_2convention_2module__structure.html", [
      [ "1. 📜 მიმოხილვა", "md_docs_2convention_2module__structure.html#autotoc_md1158", null ],
      [ "2. 📂 საქაღალდის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1159", [
        [ "2.1. სტანდარტული მოდულის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1160", null ],
        [ "2.2. კატეგორიები", "md_docs_2convention_2module__structure.html#autotoc_md1161", null ]
      ] ],
      [ "3. 📄 ფაილების დანიშნულება და შინაარსი", "md_docs_2convention_2module__structure.html#autotoc_md1162", [
        [ "3.1. <tt>src/</tt> დირექტორიის დანაწევრება", "md_docs_2convention_2module__structure.html#autotoc_md1163", null ],
        [ "3.2. კონფიგურაციის ფაილები", "md_docs_2convention_2module__structure.html#autotoc_md1164", null ],
        [ "3.3. დოკუმენტაცია", "md_docs_2convention_2module__structure.html#autotoc_md1165", null ],
        [ "3.4. კოდის სტრუქტურა (<tt>_create</tt> ფუნქცია)", "md_docs_2convention_2module__structure.html#autotoc_md1166", null ]
      ] ],
      [ "4. <tt>CMakeLists.txt</tt>-ის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1167", null ],
      [ "5. ❌ აკრძალული პრაქტიკები", "md_docs_2convention_2module__structure.html#autotoc_md1168", null ],
      [ "6. ✅ საუკეთესო პრაქტიკები", "md_docs_2convention_2module__structure.html#autotoc_md1169", null ]
    ] ],
    [ "asynchronous ოპერაციების მართვის პატერნი (Promise Pattern)", "md_docs_2convention_2promise__pattern.html", [
      [ "1. 🎯 მიზანი", "md_docs_2convention_2promise__pattern.html#autotoc_md1171", null ],
      [ "2. 🤔 პრობლემა: რატომ გვჭირდება Promise-ები?", "md_docs_2convention_2promise__pattern.html#autotoc_md1172", null ],
      [ "3. 💡 გადაწყვეტა: Promise პატერნი", "md_docs_2convention_2promise__pattern.html#autotoc_md1173", null ],
      [ "4. ⚙️ როგორ მუშაობს იმპლემენტაცია?", "md_docs_2convention_2promise__pattern.html#autotoc_md1174", null ],
      [ "5. 📝 გამოყენების პატერნი", "md_docs_2convention_2promise__pattern.html#autotoc_md1175", [
        [ "5.1. სერვისის მომწოდებლის მხარე (Provider - მაგ., <tt>wifi_manager</tt>)", "md_docs_2convention_2promise__pattern.html#autotoc_md1176", null ],
        [ "5.2. მომხმარებლის მხარე (Consumer - მაგ., <tt>ui_manager</tt>)", "md_docs_2convention_2promise__pattern.html#autotoc_md1177", null ]
      ] ],
      [ "6. ✅ როდის გამოვიყენოთ Promise და როდის Event Bus?", "md_docs_2convention_2promise__pattern.html#autotoc_md1178", null ]
    ] ],
    [ "რესურსების ოპტიმიზაციის პატერნი (Shared Task Pool)", "md_docs_2convention_2task__pool__pattern.html", [
      [ "1. 🎯 მიზანი", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1180", null ],
      [ "2. 🤔 პრობლემა: \"ერთი მოდული = ერთი ტასკი\" ანტი-პატერნი", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1181", null ],
      [ "3. 💡 გადაწყვეტა: <tt>Task Pool Manager</tt>", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1182", null ],
      [ "4. 📝 გამოყენების პატერნი", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1183", [
        [ "4.1. მოდულის <tt>private_data</tt> სტრუქტურა", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1184", null ],
        [ "4.2. სამუშაო ფუნქციის (<tt>Job</tt>) იმპლემენტაცია", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1185", null ],
        [ "4.3. <tt>_start</tt> და <tt>_deinit</tt> ფუნქციები", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1186", null ]
      ] ],
      [ "5. ✅ როდის გამოვიყენოთ <tt>Task Pool</tt> და როდის საკუთარი ტასკი?", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1187", null ]
    ] ],
    [ "ცვლადების დასახელების წესები", "md_docs_2convention_2variable__naming.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2variable__naming.html#autotoc_md1189", [
        [ "1. დესკრიპტიული სახელები", "md_docs_2convention_2variable__naming.html#autotoc_md1190", null ],
        [ "2. აკრძალული პრეფიქსები", "md_docs_2convention_2variable__naming.html#autotoc_md1191", null ],
        [ "3. სწორი მიდგომა", "md_docs_2convention_2variable__naming.html#autotoc_md1192", [
          [ "Service Locator", "md_docs_2convention_2variable__naming.html#autotoc_md1193", null ],
          [ "Event Bus", "md_docs_2convention_2variable__naming.html#autotoc_md1194", null ],
          [ "Module Registry", "md_docs_2convention_2variable__naming.html#autotoc_md1195", null ],
          [ "Display Modules", "md_docs_2convention_2variable__naming.html#autotoc_md1196", null ]
        ] ],
        [ "4. კონვენციები", "md_docs_2convention_2variable__naming.html#autotoc_md1197", [
          [ "Global ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1198", null ],
          [ "Static ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1199", null ],
          [ "Mutex/Semaphore ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1200", null ],
          [ "Queue ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1201", null ],
          [ "API Structure ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1202", null ]
        ] ]
      ] ],
      [ "მაგალითები", "md_docs_2convention_2variable__naming.html#autotoc_md1203", [
        [ "Config Manager", "md_docs_2convention_2variable__naming.html#autotoc_md1204", null ],
        [ "I2C Bus Module", "md_docs_2convention_2variable__naming.html#autotoc_md1205", null ],
        [ "MQTT Module", "md_docs_2convention_2variable__naming.html#autotoc_md1206", null ]
      ] ],
      [ "Private Data Pointer Naming", "md_docs_2convention_2variable__naming.html#autotoc_md1207", [
        [ "❌ არასწორი", "md_docs_2convention_2variable__naming.html#autotoc_md1208", null ],
        [ "✅ სწორი", "md_docs_2convention_2variable__naming.html#autotoc_md1209", null ]
      ] ],
      [ "შეჯამება", "md_docs_2convention_2variable__naming.html#autotoc_md1210", null ]
    ] ],
    [ "Synapse ESP Framework — არქიტექტურის მიმოხილვა", "md_docs_2design__decisions_2architecture__overview.html", [
      [ "მიზანი", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1212", null ],
      [ "ძირითადი არქიტექტურული პრინციპები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1213", null ],
      [ "არქიტექტურული ფენები და ძირითადი კომპონენტები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1214", null ],
      [ "მოდულებს შორის კომუნიკაციის პატერნები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1215", null ],
      [ "სიცოცხლის ციკლი", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1216", null ],
      [ "საქაღალდეებისა და ფაილების სტრუქტურა", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1217", null ],
      [ "არქიტექტურული დიაგრამა (მოკლე)", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1218", null ],
      [ "სტანდარტები და დოკუმენტაცია", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1219", null ],
      [ "დასკვნა", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1220", null ]
    ] ],
    [ "კომუნიკაციის არქიტექტურული არჩევანი (Communication Choices)", "md_docs_2design__decisions_2communication__choices.html", [
      [ "მიზანი", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1223", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1225", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1227", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1229", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1231", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1233", [
        [ "Service Locator", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1234", null ],
        [ "Event Bus", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1235", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1237", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1239", null ],
      [ "Event Bus Pattern", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1241", [
        [ "არქიტექტურული ცვლილება: Enum-დან სტრიქონებზე გადასვლა", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1242", null ],
        [ "ფრეიმვორქის API-სთან წვდომა: Facade Pattern (<tt>synapse.h</tt>)", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1243", null ]
      ] ]
    ] ],
    [ "Core კომპონენტები (Core Components)", "md_docs_2design__decisions_2core__components.html", [
      [ "მიზანი", "md_docs_2design__decisions_2core__components.html#autotoc_md1247", null ],
      [ "ძირითადი კომპონენტები", "md_docs_2design__decisions_2core__components.html#autotoc_md1249", [
        [ "1. Service Locator", "md_docs_2design__decisions_2core__components.html#autotoc_md1250", null ],
        [ "2. Event Bus", "md_docs_2design__decisions_2core__components.html#autotoc_md1251", null ],
        [ "3. Module Register", "md_docs_2design__decisions_2core__components.html#autotoc_md1252", null ],
        [ "4. Module Factory", "md_docs_2design__decisions_2core__components.html#autotoc_md1253", null ],
        [ "5. Configuration Manager", "md_docs_2design__decisions_2core__components.html#autotoc_md1254", null ],
        [ "6. Logging", "md_docs_2design__decisions_2core__components.html#autotoc_md1255", null ],
        [ "7. Promise Manager", "md_docs_2design__decisions_2core__components.html#autotoc_md1256", null ]
      ] ],
      [ "ურთიერთქმედება მოდულებთან", "md_docs_2design__decisions_2core__components.html#autotoc_md1258", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2core__components.html#autotoc_md1260", null ]
    ] ],
    [ "არქიტექტურული გადაწყვეტილების ჩანაწერის შაბლონი (Decision Record Template)", "md_docs_2design__decisions_2decision__record__template.html", [
      [ "გადაწყვეტილების ID", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1264", null ],
      [ "თარიღი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1265", null ],
      [ "სტატუსი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1266", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1267", null ],
      [ "გადაწყვეტილება", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1268", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1269", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1270", null ],
      [ "განხორციელების ნაბიჯები", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1271", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1272", null ]
    ] ],
    [ "არქიტექტურული გადაწყვეტილება: დამოკიდებულებების ინექცია (Dependency Injection)", "md_docs_2design__decisions_2dependency__injection.html", [
      [ "1. კონტექსტი", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1275", null ],
      [ "2. გადაწყვეტილება", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1276", null ],
      [ "3. არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1277", null ],
      [ "4. შედეგები და გავლენა", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1278", [
        [ "4.1. კონვენცია მოდულის დეველოპერებისთვის", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1279", null ],
        [ "4.2. გამოყენების მაგალითი", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1280", null ]
      ] ],
      [ "5. დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1281", null ]
    ] ],
    [ "Synapse Design Decisions", "md_docs_2design__decisions_2design__decisions__index.html", [
      [ "ძირითადი დოკუმენტები", "md_docs_2design__decisions_2design__decisions__index.html#autotoc_md1284", null ]
    ] ],
    [ "შეცდომების დამუშავების არქიტექტურული გადაწყვეტილებები (Error Handling Decisions)", "md_docs_2design__decisions_2error__handling__decisions.html", [
      [ "მიზანი", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1288", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1290", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1292", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1294", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1296", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1298", [
        [ "პარამეტრების ვალიდაცია", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1299", null ],
        [ "რესურსების გამოყოფის შემოწმება", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1300", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1302", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1304", null ]
    ] ],
    [ "გაფართოებადობისა და მოდულარობის არქიტექტურული გადაწყვეტილებები (Extensibility & Modularity)", "md_docs_2design__decisions_2extensibility__and__modularity.html", [
      [ "მიზანი", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1308", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1310", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1312", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1314", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1316", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1318", [
        [ "ახალი მოდულის დამატება", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1319", null ],
        [ "Public API-ის გამოყენება", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1320", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1322", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1324", null ]
    ] ],
    [ "მიგრაციისა და თავსებადობის არქიტექტურული გადაწყვეტილებები (Migration & Compatibility)", "md_docs_2design__decisions_2migration__and__compatibility.html", [
      [ "მიზანი", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1328", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1330", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1332", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1334", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1336", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1338", [
        [ "სქემის ვერსიონირება", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1339", null ],
        [ "Deprecated API-ის მონიშვნა", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1340", null ],
        [ "მიგრაციის სკრიპტის გამოყენება", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1341", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1343", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1345", null ]
    ] ],
    [ "წარმადობისა და რესურსების არქიტექტურული კომპრომისები (Performance Trade-offs)", "md_docs_2design__decisions_2performance__tradeoffs.html", [
      [ "მიზანი", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1349", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1351", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1353", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1355", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1357", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1359", [
        [ "მოდულის გამორთვა ბილდიდან", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1360", null ],
        [ "Static allocation", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1361", null ],
        [ "Lazy initialization", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1362", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1364", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1366", null ]
    ] ],
    [ "უსაფრთხოების არქიტექტურული გადაწყვეტილებები (Security Considerations)", "md_docs_2design__decisions_2security__considerations.html", [
      [ "მიზანი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1370", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1372", null ],
      [ "არჩეული გადაწყვეტილებები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1374", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1376", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1378", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1380", [
        [ "TLS/SSL ჩართვა MQTT-ში", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1381", null ],
        [ "OTA განახლების სიგნირება", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1382", null ],
        [ "ლოგირების კონტროლი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1383", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1385", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1387", null ]
    ] ],
    [ "მონაცემთა შენახვის არქიტექტურული არჩევანი (Storage Strategy)", "md_docs_2design__decisions_2storage__strategy.html", [
      [ "მიზანი", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1391", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1393", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1395", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1397", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1399", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1401", [
        [ "NVS-ში სტრიქონის შენახვა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1402", null ],
        [ "NVS-დან სტრიქონის წაკითხვა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1403", null ],
        [ "Flash-ში მონაცემების შენახვა (OTA)", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1404", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1406", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1408", null ]
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
      [ "მიზანი", "md_docs_2glossary_2glossary__api.html#autotoc_md1412", null ]
    ] ],
    [ "არქიტექტურული ტერმინები (Glossary — Architecture)", "md_docs_2glossary_2glossary__architecture.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__architecture.html#autotoc_md1417", null ]
    ] ],
    [ "კონფიგურაციისა და შენახვის ტერმინები (Glossary — Configuration & Storage)", "md_docs_2glossary_2glossary__config.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__config.html#autotoc_md1422", null ]
    ] ],
    [ "Glossary (ტერმინები)", "md_docs_2glossary_2glossary__index.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__index.html#autotoc_md1427", null ],
      [ "თემატური ტერმინების დოკუმენტები", "md_docs_2glossary_2glossary__index.html#autotoc_md1429", null ],
      [ "გამოყენების წესი", "md_docs_2glossary_2glossary__index.html#autotoc_md1431", null ]
    ] ],
    [ "უსაფრთხოების და წარმადობის ტერმინები (Glossary — Security & Performance)", "md_docs_2glossary_2glossary__security.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__security.html#autotoc_md1435", null ]
    ] ],
    [ "Cloud ინტეგრაციის სახელმძღვანელო (Integration — Cloud)", "md_docs_2integration_2integration__cloud.html", [
      [ "მიზანი", "md_docs_2integration_2integration__cloud.html#autotoc_md1440", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__cloud.html#autotoc_md1442", null ],
      [ "Best Practices", "md_docs_2integration_2integration__cloud.html#autotoc_md1444", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__cloud.html#autotoc_md1446", null ]
    ] ],
    [ "Integration Index (ინტეგრაციის სახელმძღვანელოების ინდექსი)", "md_docs_2integration_2integration__index.html", [
      [ "მიზანი", "md_docs_2integration_2integration__index.html#autotoc_md1450", null ],
      [ "თემატური ინტეგრაციის სახელმძღვანელოები", "md_docs_2integration_2integration__index.html#autotoc_md1452", null ],
      [ "გამოყენების წესი", "md_docs_2integration_2integration__index.html#autotoc_md1454", null ]
    ] ],
    [ "MQTT ინტეგრაციის სახელმძღვანელო (Integration — MQTT)", "md_docs_2integration_2integration__mqtt.html", [
      [ "მიზანი", "md_docs_2integration_2integration__mqtt.html#autotoc_md1458", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__mqtt.html#autotoc_md1460", null ],
      [ "Best Practices", "md_docs_2integration_2integration__mqtt.html#autotoc_md1462", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__mqtt.html#autotoc_md1464", null ]
    ] ],
    [ "REST API ინტეგრაციის სახელმძღვანელო (Integration — REST API)", "md_docs_2integration_2integration__rest.html", [
      [ "მიზანი", "md_docs_2integration_2integration__rest.html#autotoc_md1468", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__rest.html#autotoc_md1470", null ],
      [ "Best Practices", "md_docs_2integration_2integration__rest.html#autotoc_md1472", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__rest.html#autotoc_md1474", null ]
    ] ],
    [ "WebSocket ინტეგრაციის სახელმძღვანელო (Integration — WebSocket)", "md_docs_2integration_2integration__websocket.html", [
      [ "მიზანი", "md_docs_2integration_2integration__websocket.html#autotoc_md1478", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__websocket.html#autotoc_md1480", null ],
      [ "Best Practices", "md_docs_2integration_2integration__websocket.html#autotoc_md1482", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__websocket.html#autotoc_md1484", null ]
    ] ],
    [ "✅ Synapse Framework — Documentation & Module Design Checklist", "md_docs_2internal_2synapse__checklist.html", [
      [ "1. API Naming & Structure", "md_docs_2internal_2synapse__checklist.html#autotoc_md1487", null ],
      [ "2. Module Implementation", "md_docs_2internal_2synapse__checklist.html#autotoc_md1488", null ],
      [ "3. Communication Integration (MQTT/WiFi/ESP-NOW)", "md_docs_2internal_2synapse__checklist.html#autotoc_md1489", null ],
      [ "4. Core Interaction", "md_docs_2internal_2synapse__checklist.html#autotoc_md1490", null ],
      [ "5. Documentation", "md_docs_2internal_2synapse__checklist.html#autotoc_md1491", null ],
      [ "6. Quality & Security", "md_docs_2internal_2synapse__checklist.html#autotoc_md1492", null ],
      [ "7. Testability", "md_docs_2internal_2synapse__checklist.html#autotoc_md1493", null ]
    ] ],
    [ "ბენჩმარკების ჩატარების სახელმძღვანელო (Performance Benchmarks Guide)", "md_docs_2performance_2performance__benchmarks.html", [
      [ "მიზანი", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1496", null ],
      [ "ბენჩმარკის ჩატარების ძირითადი ნაბიჯები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1498", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1500", [
        [ "სენსორის კითხვა", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1501", null ],
        [ "MQTT publish latency", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1502", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1504", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1506", null ]
    ] ],
    [ "საუკეთესო პრაქტიკები წარმადობისთვის (Performance Best Practices)", "md_docs_2performance_2performance__best__practices.html", [
      [ "მიზანი", "md_docs_2performance_2performance__best__practices.html#autotoc_md1510", null ],
      [ "ძირითადი რეკომენდაციები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1512", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1514", null ],
      [ "Best Practices", "md_docs_2performance_2performance__best__practices.html#autotoc_md1516", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1518", null ]
    ] ],
    [ "ეკრანის მოდულების ოპტიმიზაცია (Performance — Display Modules)", "md_docs_2performance_2performance__display.html", [
      [ "მიზანი", "md_docs_2performance_2performance__display.html#autotoc_md1522", null ],
      [ "ოპტიმიზაციის რეკომენდაციები", "md_docs_2performance_2performance__display.html#autotoc_md1524", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__display.html#autotoc_md1526", [
        [ "Refresh interval-ის ოპტიმიზაცია", "md_docs_2performance_2performance__display.html#autotoc_md1527", null ],
        [ "Conditional redraw", "md_docs_2performance_2performance__display.html#autotoc_md1528", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__display.html#autotoc_md1530", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__display.html#autotoc_md1532", null ]
    ] ],
    [ "Performance (წარმადობის სახელმძღვანელო)", "md_docs_2performance_2performance__index.html", [
      [ "მიზანი", "md_docs_2performance_2performance__index.html#autotoc_md1536", null ],
      [ "თემატური performance სახელმძღვანელოები", "md_docs_2performance_2performance__index.html#autotoc_md1538", null ],
      [ "გამოყენების წესი", "md_docs_2performance_2performance__index.html#autotoc_md1540", null ]
    ] ],
    [ "ოპტიმიზაციის სახელმძღვანელოები (Performance Optimization Guide)", "md_docs_2performance_2performance__optimization.html", [
      [ "მიზანი", "md_docs_2performance_2performance__optimization.html#autotoc_md1544", null ],
      [ "კოდის ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1546", null ],
      [ "კონფიგურაციის ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1548", null ],
      [ "მოდულების ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1550", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__optimization.html#autotoc_md1552", [
        [ "Static allocation", "md_docs_2performance_2performance__optimization.html#autotoc_md1553", null ],
        [ "Lazy initialization", "md_docs_2performance_2performance__optimization.html#autotoc_md1554", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__optimization.html#autotoc_md1556", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__optimization.html#autotoc_md1558", null ]
    ] ],
    [ "რესურსების გამოყენების სახელმძღვანელო (Performance Resource Usage Guide)", "md_docs_2performance_2performance__resource__usage.html", [
      [ "მიზანი", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1562", null ],
      [ "RAM-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1564", null ],
      [ "Flash-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1565", null ],
      [ "CPU-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1566", null ],
      [ "Stack-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1567", null ],
      [ "მონიტორინგის მაგალითები", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1569", [
        [ "RAM", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1570", null ],
        [ "Stack", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1571", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1573", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1575", null ]
    ] ],
    [ "უსაფრთხოების საუკეთესო პრაქტიკები (Security Best Practices)", "md_docs_2security_2security__best__practices.html", [
      [ "მიზანი", "md_docs_2security_2security__best__practices.html#autotoc_md1579", null ],
      [ "ძირითადი რეკომენდაციები", "md_docs_2security_2security__best__practices.html#autotoc_md1581", null ],
      [ "მაგალითები", "md_docs_2security_2security__best__practices.html#autotoc_md1583", [
        [ "TLS/SSL ჩართვა MQTT-ში", "md_docs_2security_2security__best__practices.html#autotoc_md1584", null ],
        [ "OTA განახლების სიგნირება", "md_docs_2security_2security__best__practices.html#autotoc_md1585", null ],
        [ "ლოგირების კონტროლი", "md_docs_2security_2security__best__practices.html#autotoc_md1586", null ]
      ] ],
      [ "Best Practices", "md_docs_2security_2security__best__practices.html#autotoc_md1588", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__best__practices.html#autotoc_md1590", null ]
    ] ],
    [ "უსაფრთხოების შემოწმების სია (Security Checklist)", "md_docs_2security_2security__checklist.html", [
      [ "მიზანი", "md_docs_2security_2security__checklist.html#autotoc_md1594", null ],
      [ "Release Checklist", "md_docs_2security_2security__checklist.html#autotoc_md1596", null ],
      [ "Code Review Checklist", "md_docs_2security_2security__checklist.html#autotoc_md1597", null ],
      [ "Best Practices", "md_docs_2security_2security__checklist.html#autotoc_md1599", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__checklist.html#autotoc_md1601", null ]
    ] ],
    [ "უსაფრთხოების სახელმძღვანელოები (Security Guidelines)", "md_docs_2security_2security__guidelines.html", [
      [ "მიზანი", "md_docs_2security_2security__guidelines.html#autotoc_md1605", null ],
      [ "TLS/SSL", "md_docs_2security_2security__guidelines.html#autotoc_md1607", null ],
      [ "Authentication & Authorization", "md_docs_2security_2security__guidelines.html#autotoc_md1608", null ],
      [ "Input Validation", "md_docs_2security_2security__guidelines.html#autotoc_md1609", null ],
      [ "Credentials Management", "md_docs_2security_2security__guidelines.html#autotoc_md1610", null ],
      [ "OTA და Firmware Security", "md_docs_2security_2security__guidelines.html#autotoc_md1611", null ],
      [ "Logging", "md_docs_2security_2security__guidelines.html#autotoc_md1612", null ],
      [ "Best Practices", "md_docs_2security_2security__guidelines.html#autotoc_md1614", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__guidelines.html#autotoc_md1616", null ]
    ] ],
    [ "Security Index (უსაფრთხოების სახელმძღვანელოების ინდექსი)", "md_docs_2security_2security__index.html", [
      [ "მიზანი", "md_docs_2security_2security__index.html#autotoc_md1620", null ],
      [ "თემატური უსაფრთხოების სახელმძღვანელოები", "md_docs_2security_2security__index.html#autotoc_md1622", null ],
      [ "გამოყენების წესი", "md_docs_2security_2security__index.html#autotoc_md1624", null ]
    ] ],
    [ "Vulnerability Disclosure Policy (უსაფრთხოების ხარვეზების აღმოჩენისა და შეტყობინების პროცედურა)", "md_docs_2security_2security__vulnerability__disclosure.html", [
      [ "მიზანი", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1628", null ],
      [ "ხარვეზის აღმოჩენა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1630", null ],
      [ "შეტყობინების პროცედურა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1631", null ],
      [ "რეაგირება", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1632", null ],
      [ "პასუხისმგებლობა და ეთიკა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1633", null ],
      [ "საკონტაქტო ინფორმაცია", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1635", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1637", null ]
    ] ],
    [ "კომუნიკაციის პატერნები (Communication Patterns)", "md_docs_2structure_2communication__patterns.html", [
      [ "მიზანი", "md_docs_2structure_2communication__patterns.html#autotoc_md1640", null ],
      [ "ძირითადი პატერნები", "md_docs_2structure_2communication__patterns.html#autotoc_md1641", [
        [ "1. Service Locator Pattern", "md_docs_2structure_2communication__patterns.html#autotoc_md1642", null ],
        [ "2. Event Bus Pattern", "md_docs_2structure_2communication__patterns.html#autotoc_md1643", null ]
      ] ],
      [ "სწორი გამოყენების წესები", "md_docs_2structure_2communication__patterns.html#autotoc_md1644", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2structure_2communication__patterns.html#autotoc_md1645", null ],
      [ "მაგალითები", "md_docs_2structure_2communication__patterns.html#autotoc_md1646", [
        [ "Service Locator", "md_docs_2structure_2communication__patterns.html#autotoc_md1647", null ],
        [ "Event Bus", "md_docs_2structure_2communication__patterns.html#autotoc_md1648", null ]
      ] ]
    ] ],
    [ "კონფიგურაციის მართვის პროცესი (Configuration Flow)", "md_docs_2structure_2configuration__flow.html", [
      [ "მიზანი", "md_docs_2structure_2configuration__flow.html#autotoc_md1651", null ],
      [ "კონფიგურაციის წყაროები", "md_docs_2structure_2configuration__flow.html#autotoc_md1652", null ],
      [ "კონფიგურაციის სტრუქტურა", "md_docs_2structure_2configuration__flow.html#autotoc_md1653", null ],
      [ "კონფიგურაციის დამუშავების ეტაპები", "md_docs_2structure_2configuration__flow.html#autotoc_md1654", null ],
      [ "კონფიგურაციის წვდომის მაგალითები", "md_docs_2structure_2configuration__flow.html#autotoc_md1655", null ],
      [ "ვალიდაცია და Default მნიშვნელობები", "md_docs_2structure_2configuration__flow.html#autotoc_md1656", null ],
      [ "Runtime Reconfiguration", "md_docs_2structure_2configuration__flow.html#autotoc_md1657", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2configuration__flow.html#autotoc_md1658", null ]
    ] ],
    [ "Core კომპონენტები (Core Components)", "md_docs_2structure_2core__components.html", [
      [ "მიზანი", "md_docs_2structure_2core__components.html#autotoc_md1662", null ],
      [ "ძირითადი კომპონენტები", "md_docs_2structure_2core__components.html#autotoc_md1664", [
        [ "1. Service Locator", "md_docs_2structure_2core__components.html#autotoc_md1665", null ],
        [ "2. Event Bus", "md_docs_2structure_2core__components.html#autotoc_md1666", null ],
        [ "3. Module Register", "md_docs_2structure_2core__components.html#autotoc_md1667", null ],
        [ "4. Module Factory", "md_docs_2structure_2core__components.html#autotoc_md1668", null ],
        [ "5. Configuration Manager", "md_docs_2structure_2core__components.html#autotoc_md1669", null ],
        [ "6. Logging", "md_docs_2structure_2core__components.html#autotoc_md1670", null ]
      ] ],
      [ "ფასადის ინტერფეისი (<tt>synapse.h</tt>)", "md_docs_2structure_2core__components.html#autotoc_md1672", null ],
      [ "ურთიერთქმედება მოდულებთან", "md_docs_2structure_2core__components.html#autotoc_md1674", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2core__components.html#autotoc_md1676", null ]
    ] ],
    [ "საქაღალდეებისა და ფაილების სტრუქტურა (Directory Structure)", "md_docs_2structure_2directory__structure.html", [
      [ "ძირითადი სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1679", null ],
      [ "ძირითადი საქაღალდეების აღწერა", "md_docs_2structure_2directory__structure.html#autotoc_md1680", null ],
      [ "<tt>modules</tt> საქაღალდის სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1681", null ],
      [ "თითოეული მოდულის სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1682", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2directory__structure.html#autotoc_md1683", null ]
    ] ],
    [ "შეცდომების დამუშავება და ლოგირების სტანდარტები (Error Handling & Logging)", "md_docs_2structure_2error__handling__and__logging.html", [
      [ "მიზანი", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1686", null ],
      [ "შეცდომების დამუშავების პრინციპები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1687", null ],
      [ "შეცდომების კოდები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1688", null ],
      [ "შეცდომების დამუშავების მაგალითები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1689", null ],
      [ "ლოგირების სტანდარტები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1690", null ],
      [ "ლოგირების მაგალითები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1691", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1692", null ]
    ] ],
    [ "ფრეიმვორქის გაფართოება და Best Practices (Extending the Framework)", "md_docs_2structure_2extending__framework.html", [
      [ "მიზანი", "md_docs_2structure_2extending__framework.html#autotoc_md1695", null ],
      [ "ახალი მოდულის დამატების პროცესი", "md_docs_2structure_2extending__framework.html#autotoc_md1696", null ],
      [ "Best Practices", "md_docs_2structure_2extending__framework.html#autotoc_md1697", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2structure_2extending__framework.html#autotoc_md1698", null ],
      [ "მაგალითი ახალი მოდულის დამატების", "md_docs_2structure_2extending__framework.html#autotoc_md1699", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (FAQ)", "md_docs_2structure_2faq.html", [
      [ "1. როგორ დავამატო ახალი მოდული?", "md_docs_2structure_2faq.html#autotoc_md1702", null ],
      [ "2. შეიძლება თუ არა ერთი და იმავე ტიპის რამდენიმე მოდულის დამატება?", "md_docs_2structure_2faq.html#autotoc_md1703", null ],
      [ "3. როგორ ხდება მოდულებს შორის კომუნიკაცია?", "md_docs_2structure_2faq.html#autotoc_md1704", null ],
      [ "4. როგორ მივიღო ან შევცვალო მოდულის კონფიგურაცია?", "md_docs_2structure_2faq.html#autotoc_md1705", null ],
      [ "5. როგორ დავამატო ახალი გლობალური პარამეტრი?", "md_docs_2structure_2faq.html#autotoc_md1706", null ],
      [ "6. როგორ დავიცვა naming და structure სტანდარტები?", "md_docs_2structure_2faq.html#autotoc_md1707", null ],
      [ "7. როგორ ხდება შეცდომების დამუშავება და ლოგირება?", "md_docs_2structure_2faq.html#autotoc_md1708", null ],
      [ "8. როგორ დავამატო ახალი კატეგორია მოდულებისთვის?", "md_docs_2structure_2faq.html#autotoc_md1709", null ],
      [ "9. როგორ დავამატო დოკუმენტაცია ან განვაახლო არსებული?", "md_docs_2structure_2faq.html#autotoc_md1710", null ],
      [ "10. როგორ დავიწყო პროექტის გამოყენება?", "md_docs_2structure_2faq.html#autotoc_md1711", null ]
    ] ],
    [ "მოდულის სიცოცხლის ციკლი და ინიციალიზაცია (v2.0)", "md_docs_2structure_2lifecycle__and__init.html", [
      [ "მიზანი", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1714", null ],
      [ "სიცოცხლის ციკლის ეტაპები და სერვისის სტატუსები", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1715", null ],
      [ "სიცოცხლის ციკლის დიაგრამა (განახლებული)", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1716", null ],
      [ "მაგალითი მოდულის სიცოცხლის ციკლის მართვის", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1717", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1718", null ]
    ] ],
    [ "მოდულების არქიტექტურა (Module Architecture)", "md_docs_2structure_2module__architecture.html", [
      [ "ძირითადი პრინციპები", "md_docs_2structure_2module__architecture.html#autotoc_md1721", null ],
      [ "მოდულის ტიპები", "md_docs_2structure_2module__architecture.html#autotoc_md1722", null ],
      [ "მოდულების ურთიერთქმედება", "md_docs_2structure_2module__architecture.html#autotoc_md1723", null ],
      [ "მოდულის სიცოცხლის ციკლი (Lifecycle)", "md_docs_2structure_2module__architecture.html#autotoc_md1724", null ],
      [ "მოდულის სტრუქტურის მაგალითი", "md_docs_2structure_2module__architecture.html#autotoc_md1725", [
        [ "<tt>_create</tt> ფუნქციის კონტრაქტი", "md_docs_2structure_2module__architecture.html#autotoc_md1726", null ]
      ] ],
      [ "მოდულის გაფართოება/დამატება", "md_docs_2structure_2module__architecture.html#autotoc_md1727", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2module__architecture.html#autotoc_md1728", null ]
    ] ],
    [ "🚀 <tt>create_module.py</tt>: Synapse მოდულის გენერატორი (v3.1)", "md_docs_2tools_2create__module.html", [
      [ "1. 📋 რა არის <tt>create_module.py</tt>?", "md_docs_2tools_2create__module.html#autotoc_md1739", null ],
      [ "2. 🎯 რას აკეთებს?", "md_docs_2tools_2create__module.html#autotoc_md1740", [
        [ "✅ გენერირებული ფაილები (10+ ფაილი)", "md_docs_2tools_2create__module.html#autotoc_md1741", null ],
        [ "✅ ავტომატური ინტეგრაცია", "md_docs_2tools_2create__module.html#autotoc_md1742", null ]
      ] ],
      [ "3. 🛠️ გამოყენების რეჟიმები", "md_docs_2tools_2create__module.html#autotoc_md1743", [
        [ "3.1. ინტერაქტიული რეჟიმი (რეკომენდებული)", "md_docs_2tools_2create__module.html#autotoc_md1744", null ],
        [ "3.2. ბრძანების ხაზის რეჟიმი (ავტომატიზაციისთვის)", "md_docs_2tools_2create__module.html#autotoc_md1745", null ]
      ] ],
      [ "4. 🏛️ არქეტიპები (Archetypes)", "md_docs_2tools_2create__module.html#autotoc_md1746", null ],
      [ "5. 🔄 სრული სამუშაო პროცესი (Workflow)", "md_docs_2tools_2create__module.html#autotoc_md1747", null ],
      [ "6. 🚨 მნიშვნელოვანი შენიშვნები", "md_docs_2tools_2create__module.html#autotoc_md1748", null ],
      [ "🎯 შეჯამება", "md_docs_2tools_2create__module.html#autotoc_md1750", null ]
    ] ],
    [ "📝 <tt>JSON</tt> ვალიდატორი (<tt>validate_jsons.py</tt>)", "md_docs_2tools_2json__validator.html", [
      [ "1. 🎯 დანიშნულება", "md_docs_2tools_2json__validator.html#autotoc_md1752", null ],
      [ "2. 🏛️ არქიტექტურა", "md_docs_2tools_2json__validator.html#autotoc_md1753", null ],
      [ "3. 🛠️ გამოყენება", "md_docs_2tools_2json__validator.html#autotoc_md1754", null ],
      [ "4. ✍️ როგორ დავამატოთ ვალიდაცია ახალ მოდულს?", "md_docs_2tools_2json__validator.html#autotoc_md1755", null ]
    ] ],
    [ "Tools & Scripts", "md_docs_2tools_2tools__index.html", null ],
    [ "Advanced Diagnostics (გაფართოებული დიაგნოსტიკა)", "md_docs_2troubleshooting_2advanced__diagnostics.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1758", null ],
      [ "1. Stack Overflow და Task Watchdog", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1760", null ],
      [ "2. Heap Corruption და Memory Leak", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1761", null ],
      [ "3. Core Dumps და Crash Reporting", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1762", null ],
      [ "4. Custom Health Checks", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1763", null ],
      [ "5. Peripheral Diagnostics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1764", null ],
      [ "6. Network Diagnostics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1765", null ],
      [ "7. Performance Metrics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1766", null ]
    ] ],
    [ "გავრცელებული პრობლემები და გადაჭრის გზები (Common Issues & Solutions)", "md_docs_2troubleshooting_2common__issues.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1769", null ],
      [ "1. ბილდის შეცდომები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1771", [
        [ "პრობლემა: მოდული არ იბილდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1772", null ],
        [ "პრობლემა: include file not found", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1773", null ]
      ] ],
      [ "2. კონფიგურაციის პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1775", [
        [ "პრობლემა: მოდული არ ინიციალიზდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1776", null ],
        [ "პრობლემა: default მნიშვნელობები არ მუშაობს", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1777", null ]
      ] ],
      [ "3. კომუნიკაციის პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1779", [
        [ "პრობლემა: Event Bus-ზე მოვლენები არ ვრცელდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1780", null ],
        [ "პრობლემა: Service Locator-ით API არ იძებნება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1781", null ]
      ] ],
      [ "4. ლოგირების და შეცდომების დიაგნოსტიკა", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1783", [
        [ "პრობლემა: ლოგები არ ჩანს", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1784", null ],
        [ "პრობლემა: შეცდომის კოდი არ იკითხება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1785", null ]
      ] ],
      [ "5. Runtime პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1787", [
        [ "პრობლემა: მოდული არ რეაგირებს enable/disable-ზე", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1788", null ],
        [ "პრობლემა: რესურსების გაჟონვა (memory leak)", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1789", null ],
        [ "პრობლემა: სისტემის კრახი <tt>Stack Overflow in task Tmr Svc</tt> შეცდომით", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1791", null ],
        [ "პრობლემა: სისტემის კრახი <tt>LoadProhibited</tt> ან <tt>assert failed: heap_caps_free</tt>", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1792", null ]
      ] ],
      [ "6. დამატებითი რჩევები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1794", null ]
    ] ],
    [ "Debugging Guide (დიბაგინგის სახელმძღვანელო)", "md_docs_2troubleshooting_2debugging__guide.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1797", null ],
      [ "1. ლოგირების ჩართვა და გამოყენება", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1799", null ],
      [ "2. Debug Build-ის კონფიგურაცია", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1800", null ],
      [ "3. GDB და Breakpoint-ების გამოყენება", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1801", null ],
      [ "4. Task და Stack Usage მონიტორინგი", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1802", null ],
      [ "5. Heap და Memory Usage", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1803", null ],
      [ "6. Event და Service Debugging", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1804", null ],
      [ "7. Assertion-ები და Error Checking", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1805", null ],
      [ "8. დამატებითი ინსტრუმენტები", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1806", null ]
    ] ],
    [ "პრობლემა: როგორ გადავცეთ კონტექსტი სერვისის API-ს გამოძახებისას?", "md_docs_2troubleshooting_2dependency__injection__and__context.html", [
      [ "1. ❓ პრობლემის აღწერა", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1809", null ],
      [ "2. ❌ არასწორი (მაგრამ მაცდური) გადაწყვეტები", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1810", null ],
      [ "3. ✅ სწორი არქიტექტურული პატერნი: ორმაგი ინექცია", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1811", [
        [ "ეტაპი 1: <tt>private_data</tt> სტრუქტურის გაფართოება", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1812", null ],
        [ "ეტაპი 2: <tt>dependency_map</tt>-ის განსაზღვრა", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1813", null ],
        [ "ეტაპი 3: <tt>config.json</tt>-ის კონფიგურაცია", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1814", null ],
        [ "ეტაპი 4: გამოყენება", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1815", null ]
      ] ],
      [ "4. 💬 დასკვნა", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1816", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (Troubleshooting FAQ)", "md_docs_2troubleshooting_2faq.html", [
      [ "1. რატომ არ იბილდება ჩემი მოდული?", "md_docs_2troubleshooting_2faq.html#autotoc_md1818", null ],
      [ "2. რატომ არ ჩანს ლოგები?", "md_docs_2troubleshooting_2faq.html#autotoc_md1819", null ],
      [ "3. რატომ ვერ პოულობს Service Locator-ი ჩემს სერვისს?", "md_docs_2troubleshooting_2faq.html#autotoc_md1820", null ],
      [ "4. რატომ არ ვრცელდება Event Bus-ზე მოვლენები?", "md_docs_2troubleshooting_2faq.html#autotoc_md1821", null ],
      [ "5. რატომ არ ინიციალიზდება მოდული?", "md_docs_2troubleshooting_2faq.html#autotoc_md1822", null ],
      [ "6. როგორ გავიგო რომელი მოდული იწვევს პრობლემას?", "md_docs_2troubleshooting_2faq.html#autotoc_md1823", null ],
      [ "7. როგორ გამოვასწორო რესურსების გაჟონვა (memory leak)?", "md_docs_2troubleshooting_2faq.html#autotoc_md1824", null ],
      [ "8. როგორ დავადგინო კონფიგურაციის შეცდომა?", "md_docs_2troubleshooting_2faq.html#autotoc_md1825", null ],
      [ "9. როგორ დავამატო ახალი კითხვა ამ FAQ-ში?", "md_docs_2troubleshooting_2faq.html#autotoc_md1826", null ]
    ] ],
    [ "Advanced Usage (გაფართოებული გამოყენება)", "md_docs_2tutorials_2advanced__usage.html", [
      [ "მიზანი", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1829", null ],
      [ "1. მრავალმოდულიანი კონფიგურაცია", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1830", null ],
      [ "2. მოდულის დინამიური ჩართვა/გამორთვა", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1831", null ],
      [ "3. Runtime Reconfiguration", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1832", null ],
      [ "4. Custom Event-ების გამოყენება", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1833", null ],
      [ "5. ასინქრონული ოპერაციების მართვა (Promise Pattern)", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1834", [
        [ "5.1. სერვისის მომწოდებლის API (<tt>wifi_manager</tt>)", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1835", null ],
        [ "5.2. მომხმარებლის ლოგიკა (<tt>ui_manager</tt>)", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1836", null ]
      ] ],
      [ "6. მოდულის სტატუსის მონიტორინგი", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1837", null ],
      [ "7. ოპტიმიზაცია და რესურსების მართვა", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1838", null ],
      [ "8. Best Practices", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1839", null ]
    ] ],
    [ "პირველი საკუთარი მოდულის შექმნა (v2.0)", "md_docs_2tutorials_2create__first__module.html", [
      [ "მიზანი", "md_docs_2tutorials_2create__first__module.html#autotoc_md1842", null ],
      [ "1. საჭიროების განსაზღვრა", "md_docs_2tutorials_2create__first__module.html#autotoc_md1843", null ],
      [ "2. მოდულის ჩონჩხის გენერაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1844", null ],
      [ "3. კონფიგურაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1845", null ],
      [ "4. კოდის იმპლემენტაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1846", null ],
      [ "5. ტესტირება", "md_docs_2tutorials_2create__first__module.html#autotoc_md1849", null ]
    ] ],
    [ "სწრაფი სტარტი (Getting Started)", "md_docs_2tutorials_2getting__started.html", [
      [ "მიზანი", "md_docs_2tutorials_2getting__started.html#autotoc_md1852", null ],
      [ "1. გარემოს მომზადება", "md_docs_2tutorials_2getting__started.html#autotoc_md1853", null ],
      [ "2. პროექტის სტრუქტურის გაცნობა", "md_docs_2tutorials_2getting__started.html#autotoc_md1854", null ],
      [ "3. პირველი მოდულის ჩართვა", "md_docs_2tutorials_2getting__started.html#autotoc_md1855", null ],
      [ "4. ბილდი და ფლეში", "md_docs_2tutorials_2getting__started.html#autotoc_md1856", null ],
      [ "5. ლოგების ნახვა და ტესტირება", "md_docs_2tutorials_2getting__started.html#autotoc_md1857", null ],
      [ "6. დამატებითი რესურსები", "md_docs_2tutorials_2getting__started.html#autotoc_md1858", null ]
    ] ],
    [ "ინტეგრაციის მაგალითი (Integration Example)", "md_docs_2tutorials_2integration__example.html", [
      [ "მიზანი", "md_docs_2tutorials_2integration__example.html#autotoc_md1861", null ],
      [ "1. ინტეგრაციის საჭიროების განსაზღვრა", "md_docs_2tutorials_2integration__example.html#autotoc_md1862", null ],
      [ "2. საჭირო მოდულების ჩართვა და კონფიგურაცია", "md_docs_2tutorials_2integration__example.html#autotoc_md1863", null ],
      [ "3. MQTT Integration (Publish/Subscribe)", "md_docs_2tutorials_2integration__example.html#autotoc_md1864", null ],
      [ "4. REST API Integration", "md_docs_2tutorials_2integration__example.html#autotoc_md1865", null ],
      [ "5. WebSocket Integration", "md_docs_2tutorials_2integration__example.html#autotoc_md1866", null ],
      [ "6. Best Practices", "md_docs_2tutorials_2integration__example.html#autotoc_md1867", null ],
      [ "7. Troubleshooting", "md_docs_2tutorials_2integration__example.html#autotoc_md1868", null ]
    ] ],
    [ "მოდულის გაფართოება ან ფუნქციონალის დამატება (Module Extension & Feature Addition)", "md_docs_2tutorials_2module__extension.html", [
      [ "მიზანი", "md_docs_2tutorials_2module__extension.html#autotoc_md1871", null ],
      [ "1. საჭიროების განსაზღვრა", "md_docs_2tutorials_2module__extension.html#autotoc_md1872", null ],
      [ "2. სტრუქტურის დაცვა", "md_docs_2tutorials_2module__extension.html#autotoc_md1873", null ],
      [ "3. ახალი API ან ფუნქციის დამატება", "md_docs_2tutorials_2module__extension.html#autotoc_md1874", null ],
      [ "4. კონფიგურაციის გაფართოება", "md_docs_2tutorials_2module__extension.html#autotoc_md1875", null ],
      [ "5. კომუნიკაციის გაფართოება", "md_docs_2tutorials_2module__extension.html#autotoc_md1876", null ],
      [ "6. ტესტირება და ლოგირება", "md_docs_2tutorials_2module__extension.html#autotoc_md1877", null ],
      [ "7. დოკუმენტაციის განახლება", "md_docs_2tutorials_2module__extension.html#autotoc_md1878", null ],
      [ "Best Practices", "md_docs_2tutorials_2module__extension.html#autotoc_md1879", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2tutorials_2module__extension.html#autotoc_md1880", null ]
    ] ],
    [ "სისტემის კონფიგურაცია (System Configuration)", "md_docs_2tutorials_2system__configuration.html", [
      [ "🎯 მიზანი", "md_docs_2tutorials_2system__configuration.html#autotoc_md1883", null ],
      [ "1. კონფიგურაციის ფაილების როლები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1885", null ],
      [ "2. <tt>config.json</tt>-ის სტრუქტურა და მაგალითები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1887", null ],
      [ "3. მოდულის კონფიგურაციის ობიექტის აღწერა", "md_docs_2tutorials_2system__configuration.html#autotoc_md1889", null ],
      [ "4. პარამეტრებზე წვდომა კოდიდან (Dot-Notation)", "md_docs_2tutorials_2system__configuration.html#autotoc_md1891", null ],
      [ "5. კონფიგურაციის ვალიდაცია და Default-ები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1893", null ],
      [ "6. საუკეთესო პრაქტიკები (Best Practices)", "md_docs_2tutorials_2system__configuration.html#autotoc_md1895", null ]
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
"md_docs_2api__reference_2core__api.html#autotoc_md112",
"md_docs_2api__reference_2system__api.html#autotoc_md463",
"md_docs_2changelog_2v5.html#autotoc_md755",
"md_docs_2convention_2function__naming.html#autotoc_md1090",
"md_docs_2glossary_2glossary__index.html#autotoc_md1429",
"md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1803",
"structcmd__router__api__t.html#ae1dda5d57f2108d77a1fc6f314a2058f",
"system__manager_8h.html#a11fa45878fd178d237cc75f901069c66"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';