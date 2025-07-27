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
    [ "🔌 მოდული: <tt>relay_actuator</tt>", "md_components_2modules_2actuators_2relay__actuator_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2actuators_2relay__actuator_2README.html#autotoc_md1", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2actuators_2relay__actuator_2README.html#autotoc_md2", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2actuators_2relay__actuator_2README.html#autotoc_md3", null ],
      [ "4. 🔌 Service API (<tt>relay_api_t</tt>)", "md_components_2modules_2actuators_2relay__actuator_2README.html#autotoc_md4", null ],
      [ "5. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2actuators_2relay__actuator_2README.html#autotoc_md5", null ],
      [ "6. 📢 ივენთების მოდელი", "md_components_2modules_2actuators_2relay__actuator_2README.html#autotoc_md6", null ],
      [ "7. 📡 MQTT ინტერფეისი", "md_components_2modules_2actuators_2relay__actuator_2README.html#autotoc_md7", null ],
      [ "8. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2actuators_2relay__actuator_2README.html#autotoc_md8", null ]
    ] ],
    [ "☁️ მოდული: <tt>mqtt_manager</tt>", "md_components_2modules_2communications_2mqtt__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2communications_2mqtt__manager_2README.html#autotoc_md10", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2communications_2mqtt__manager_2README.html#autotoc_md11", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2communications_2mqtt__manager_2README.html#autotoc_md12", null ],
      [ "4. 📡 MQTT თემების სტრუქტურა (Topic Structure)", "md_components_2modules_2communications_2mqtt__manager_2README.html#autotoc_md13", null ],
      [ "5. 🧠 იმპლემენტაციის ლოგიკა", "md_components_2modules_2communications_2mqtt__manager_2README.html#autotoc_md14", [
        [ "5.1. <tt>init</tt> ფაზა", "md_components_2modules_2communications_2mqtt__manager_2README.html#autotoc_md15", null ],
        [ "5.2. <tt>handle_event</tt> ფუნქცია", "md_components_2modules_2communications_2mqtt__manager_2README.html#autotoc_md16", null ],
        [ "5.3. <tt>mqtt_event_handler_cb</tt> (ESP-IDF-ის callback)", "md_components_2modules_2communications_2mqtt__manager_2README.html#autotoc_md17", null ]
      ] ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2communications_2mqtt__manager_2README.html#autotoc_md18", null ]
    ] ],
    [ "📶 მოდული <tt>wifi_manager</tt>", "md_components_2modules_2communications_2wifi__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md20", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md21", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md22", null ],
      [ "4. 📢 ივენთები (Events)", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md23", [
        [ "გამოქვეყნებული ივენთები", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md24", null ],
        [ "გამოწერილი ივენთები", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md25", null ]
      ] ],
      [ "5. 📡 MQTT ინტერფეისი", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md26", null ],
      [ "6. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md27", null ],
      [ "7. 💡 გამოყენების მაგალითი (მოთხოვნა-პასუხი)", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md28", null ],
      [ "8. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md29", null ]
    ] ],
    [ "🩺 მოდული: <tt>self_test_manager</tt>", "md_components_2modules_2diagnostics_2self__test__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md31", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md32", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md33", null ],
      [ "4. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md34", null ],
      [ "5. 📡 MQTT ინტერფეისი", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md35", null ],
      [ "6. 💡 ახალი ტესტის დამატების მაგალითი", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md36", null ],
      [ "7. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md37", null ]
    ] ],
    [ "📺 მოდული: <tt>ssd1306_driver</tt>", "md_components_2modules_2displays_2ssd1306__driver_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2displays_2ssd1306__driver_2README.html#autotoc_md39", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2displays_2ssd1306__driver_2README.html#autotoc_md40", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2displays_2ssd1306__driver_2README.html#autotoc_md41", null ],
      [ "4. 🔌 Service API", "md_components_2modules_2displays_2ssd1306__driver_2README.html#autotoc_md42", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2displays_2ssd1306__driver_2README.html#autotoc_md43", null ]
    ] ],
    [ "🔘 მოდული: <tt>button_input</tt>", "md_components_2modules_2drivers_2button__input_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md45", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md46", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md47", null ],
      [ "4. 📢 გამოქვეყნებული ივენთი", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md48", null ],
      [ "5. 🔌 მიერთების სქემა", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md49", null ]
    ] ],
    [ "🚌 მოდული: <tt>i2c_bus</tt>", "md_components_2modules_2drivers_2i2c__bus_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md51", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md52", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md53", null ],
      [ "4. 🔌 Service API (<tt>i2c_bus_api_t</tt>)", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md54", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md55", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md56", null ]
    ] ],
    [ "📲 მოდული <tt>ble_provisioning</tt>", "md_components_2modules_2provisioning_2ble__provisioning_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md58", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md59", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md60", null ],
      [ "4. 🛠️ Service API (<tt>ble_prov_api_t</tt>)", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md61", null ],
      [ "5. 📢 ივენთები (Events)", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md62", [
        [ "გამოქვეყნებული ივენთები", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md63", null ],
        [ "გამოწერილი ივენთები", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md64", null ]
      ] ],
      [ "6. 💡 გამოყენების მაგალითი", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md65", null ],
      [ "7. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md66", null ]
    ] ],
    [ "🛡️ მოდული: <tt>security_status_reporter</tt>", "md_components_2modules_2security_2security__status__reporter_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md68", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md69", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md70", null ],
      [ "4. 🔌 Service API (<tt>security_status_api_t</tt>)", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md71", null ],
      [ "5. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md72", null ],
      [ "6. 💡 გამოყენება და უსაფრთხოების ჩართვა", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md73", null ],
      [ "7. 🤝 ინტეგრაცია <tt>self_test_manager</tt>-თან", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md74", null ]
    ] ],
    [ "🌡️ მოდული: <tt>sensor_aggregator</tt>", "md_components_2modules_2sensors_2sensor__aggregator_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md76", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md77", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>system_config.json</tt>)", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md78", [
        [ "3.1. ძირითადი კონფიგურაციის პარამეტრები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md79", null ],
        [ "3.2. <tt>sensors_to_aggregate</tt> ობიექტის პარამეტრები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md80", [
          [ "**<tt>aggregation_mode</tt>-ის შესაძლო მნიშვნელობები:**", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md81", null ]
        ] ]
      ] ],
      [ "4. 📢 ივენთების მოდელი", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md82", null ],
      [ "5. 💡 გამოყენების მაგალითი და ტესტირება", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md83", null ],
      [ "6. 🚀 სამომავლო განვითარების პერსპექტივები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md84", null ]
    ] ],
    [ "💾 მოდული <tt>storage_manager</tt>", "md_components_2modules_2storage_2storage__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md86", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md87", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md88", null ],
      [ "4. 🛠️ Service API (<tt>storage_api_t</tt>)", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md89", [
        [ "Key-Value ოპერაციები", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md90", null ],
        [ "ფაილური ოპერაციები (მხარდაჭერილია SPIFFS და SD Card backend-ებზე)", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md91", null ]
      ] ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md92", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md93", null ]
    ] ],
    [ "🚨 მოდული: System Alarms Manager", "md_components_2modules_2system_2alarms__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md95", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md96", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md97", [
        [ "3.1. განგაშის წესის პარამეტრები (<tt>alarms</tt> ობიექტი)", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md98", null ],
        [ "3.2. მოქმედებების პარამეტრები (<tt>actions</tt> ობიექტი)", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md99", null ]
      ] ],
      [ "4. 🔌 Service API", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md100", null ],
      [ "5. 📢 ივენთების დამუშავება", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md101", null ],
      [ "6. 💡 გამოყენების სცენარები", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md102", null ],
      [ "7. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md103", null ]
    ] ],
    [ "🛰️ მოდული: Connectivity Watchdog", "md_components_2modules_2system_2connectivity__watchdog_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md105", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md106", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>system_config.json</tt>)", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md107", [
        [ "3.1. კონფიგურაციის სრული მაგალითი", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md108", null ],
        [ "3.2. <tt>layers</tt> ობიექტის პარამეტრები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md109", null ],
        [ "3.3. <tt>actions</tt> ობიექტის პარამეტრები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md110", null ],
        [ "3.4. მხარდაჭერილი <tt>type</tt>-ები და <tt>do</tt> მოქმედებები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md111", null ]
      ] ],
      [ "4. 🔌 Service API", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md112", null ],
      [ "5. 📢 ივენთების უნივერსალური მოდელი", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md113", [
        [ "5.1. MQTT მოდულის ინტეგრაცია", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md114", null ],
        [ "5.2. WebSocket მოდულის (ჰიპოთეტური) ინტეგრაცია", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md115", null ],
        [ "5.3. PING შემოწმების ინტეგრაცია", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md116", null ]
      ] ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md117", null ]
    ] ],
    [ "🆔 მოდული: <tt>device_identity_service</tt>", "md_components_2modules_2system_2device__identity__service_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md119", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md120", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md121", null ],
      [ "4. 🔌 Service API (<tt>device_identity_api_t</tt>)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md122", null ],
      [ "5. 📢 ივენთები (Events)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md123", [
        [ "გამოქვეყნებული ივენთები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md124", null ],
        [ "გამოწერილი ივენთები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md125", null ]
      ] ],
      [ "6. 📡 MQTT ინტერფეისი", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md126", null ],
      [ "7. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md127", null ],
      [ "8. 💡 გამოყენების მაგალითი (მოთხოვნა-პასუხი)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md128", null ],
      [ "9. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md129", null ]
    ] ],
    [ "მოდული: Health Monitor", "md_components_2modules_2system_2health__monitor_2README.html", [
      [ "1. მიმოხილვა", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md131", null ],
      [ "2. წინაპირობები და <tt>menuconfig</tt>", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md132", null ],
      [ "3. არქიტექტურული პრინციპები", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md133", null ],
      [ "4. კონფიგურაცია", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md134", null ],
      [ "5. Service API (<tt>health_api_t</tt>)", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md135", null ],
      [ "6. გამოქვეყნებული ივენთები", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md136", null ],
      [ "7. 💡 გამოყენების მაგალითი", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md137", null ]
    ] ],
    [ "მოდული: OTA Update Manager", "md_components_2modules_2system_2ota__update__manager_2README.html", [
      [ "1. მიმოხილვა", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md139", null ],
      [ "2. წინაპირობები და <tt>menuconfig</tt>", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md140", null ],
      [ "3. არქიტექტურული პრინციპები", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md141", null ],
      [ "4. კონფიგურაცია", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md142", null ],
      [ "5. Service API (<tt>ota_api_t</tt>)", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md143", null ],
      [ "6. გამოქვეყნებული ივენთები", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md144", null ],
      [ "7. 💡 გამოყენების მაგალითი", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md145", [
        [ "8. განახლების ვალიდაცია და Rollback", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md146", null ]
      ] ]
    ] ],
    [ "⏱️ მოდული: <tt>time_sync</tt>", "md_components_2modules_2system_2time__sync_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2time__sync_2README.html#autotoc_md148", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2time__sync_2README.html#autotoc_md149", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2time__sync_2README.html#autotoc_md150", null ],
      [ "4. 🔌 Service API (<tt>time_sync_api_t</tt>)", "md_components_2modules_2system_2time__sync_2README.html#autotoc_md151", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2system_2time__sync_2README.html#autotoc_md152", null ]
    ] ],
    [ "🐕 მოდული: <tt>watchdog_manager</tt>", "md_components_2modules_2system_2watchdog__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md154", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md155", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md156", null ],
      [ "4. 🔌 Service API (<tt>watchdog_api_t</tt>)", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md157", null ],
      [ "5. 📢 გამოქვეყნებული ივენთები", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md158", null ],
      [ "6. 💡 გამოყენების მაგალითი", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md159", null ],
      [ "7. ❗ წინაპირობები და <tt>menuconfig</tt>", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md160", null ]
    ] ],
    [ "🧪 მოდული: <tt>sensor_simulator</tt>", "md_components_2modules_2testing_2sensor__simulator_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md162", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md163", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md164", null ],
      [ "4. 📢 გამოქვეყნებული ივენთები", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md165", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md166", null ]
    ] ],
    [ "🧪 მოდული <tt>timer_test</tt>", "md_components_2modules_2testing_2timer__test__module_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md168", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md169", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md170", null ],
      [ "4. 🚀 მუშაობის ლოგიკა", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md171", null ],
      [ "5. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md172", null ]
    ] ],
    [ "⚙️ მოდული: Command Router & System Shell", "md_components_2modules_2utilities_2command__router_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md174", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md175", null ],
      [ "3. 🛠️ კონფიგურაცია", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md176", null ],
      [ "4. 🔌 Service API (<tt>cmd_router_api_t</tt>)", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md177", null ],
      [ "5. 🚀 ჩაშენებული ბრძანებები", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md178", null ],
      [ "6. 💡 გამოყენების მაგალითი", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md179", null ],
      [ "7. 🩺 პრობლემების მოგვარება (Troubleshooting)", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md180", null ]
    ] ],
    [ "Logger Module", "md_components_2modules_2utilities_2logger__module_2README.html", [
      [ "Overview", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md182", null ],
      [ "Features", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md183", null ],
      [ "Purpose", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md184", null ],
      [ "Kconfig Configuration", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md185", [
        [ "Main Configuration Options", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md186", null ],
        [ "Compilation Control", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md187", null ]
      ] ],
      [ "Runtime Configuration", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md188", [
        [ "Enable Logger Module", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md189", null ],
        [ "Disable Logger Module", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md190", null ]
      ] ],
      [ "Runtime Control", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md191", [
        [ "Command Format", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md192", null ],
        [ "Supported Commands", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md193", [
          [ "Disable Logger", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md194", null ],
          [ "Enable Logger", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md195", null ]
        ] ],
        [ "Behavior", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md196", null ]
      ] ],
      [ "Implementation Details", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md197", [
        [ "Module Structure", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md198", null ],
        [ "Event Subscription", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md199", null ],
        [ "Output Format", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md200", null ]
      ] ],
      [ "Development Notes", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md201", null ],
      [ "API Reference", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md202", [
        [ "Functions", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md203", [
          [ "<tt>logger_module_create(const cJSON *config)</tt>", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md204", null ]
        ] ],
        [ "ბრძანებები", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md205", null ],
        [ "ქცევა", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md206", null ],
        [ "მაგალითი", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md207", null ]
      ] ]
    ] ],
    [ "💡 მოდული <tt>rgb_led_indicator</tt>", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md209", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md210", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md211", null ],
      [ "4. 🛠️ Service API (<tt>rgb_led_api_t</tt>)", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md212", null ],
      [ "5. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md213", null ],
      [ "6. 📢 ივენთების დამუშავება", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md214", null ],
      [ "7. ტექნიკური იმპლემენტაციის დეტალები", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md215", null ],
      [ "8. 💡 გამოყენების მაგალითი", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md216", null ],
      [ "9. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md217", null ]
    ] ],
    [ "⏱️ მოდული <tt>system_timer</tt>", "md_components_2modules_2utilities_2system__timer_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md219", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md220", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md221", null ],
      [ "4. 🛠️ Service API (<tt>timer_api_t</tt>)", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md222", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md223", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md224", null ]
    ] ],
    [ "🖥️ მოდული: <tt>ui_manager</tt>", "md_components_2modules_2utilities_2ui__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2utilities_2ui__manager_2README.html#autotoc_md226", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2utilities_2ui__manager_2README.html#autotoc_md227", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2utilities_2ui__manager_2README.html#autotoc_md228", null ],
      [ "4. 🚀 UI-ს სტრუქტურა და ნავიგაცია", "md_components_2modules_2utilities_2ui__manager_2README.html#autotoc_md229", null ],
      [ "5. 🔌 გამოყენებული სერვისები", "md_components_2modules_2utilities_2ui__manager_2README.html#autotoc_md230", null ],
      [ "6. 📢 დამუშავებული ივენთები", "md_components_2modules_2utilities_2ui__manager_2README.html#autotoc_md231", null ]
    ] ],
    [ "Synapse Actuators Module API Reference", "md_docs_2api__reference_2actuators__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2actuators__api.html#autotoc_md233", null ],
      [ "Relay Module API", "md_docs_2api__reference_2actuators__api.html#autotoc_md235", [
        [ "module_t <em>relay_module_create(const cJSON *config);", "md_docs_2api__reference_2actuators__api.html#autotoc_md236", null ],
        [ "esp_err_t relay_module_enable(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md237", null ],
        [ "esp_err_t relay_module_disable(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md238", null ],
        [ "esp_err_t relay_module_toggle(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md239", null ],
        [ "esp_err_t relay_module_set_state(module_t *module, bool state);", "md_docs_2api__reference_2actuators__api.html#autotoc_md240", null ],
        [ "bool relay_module_get_state(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md241", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2actuators__api.html#autotoc_md243", [
        [ "static void relay_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2actuators__api.html#autotoc_md244", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2actuators__api.html#autotoc_md246", [
        [ "static esp_err_t parse_relay_config(const cJSON *config, relay_private_data_t *relay_data);", "md_docs_2api__reference_2actuators__api.html#autotoc_md247", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2actuators__api.html#autotoc_md249", [
        [ "static module_status_t relay_get_status(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md250", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2actuators__api.html#autotoc_md252", null ],
      [ "მაგალითები", "md_docs_2api__reference_2actuators__api.html#autotoc_md254", null ]
    ] ],
    [ "Synapse API Reference Index", "md_docs_2api__reference_2api__index.html", [
      [ "Core API", "md_docs_2api__reference_2api__index.html#autotoc_md258", null ],
      [ "Module API", "md_docs_2api__reference_2api__index.html#autotoc_md259", null ],
      [ "System & Extensions", "md_docs_2api__reference_2api__index.html#autotoc_md260", null ]
    ] ],
    [ "Synapse Command Router API Reference", "md_docs_2api__reference_2command__router__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2command__router__api.html#autotoc_md263", null ],
      [ "⚙️ API სტრუქტურა (<tt>cmd_router_api_t</tt>)", "md_docs_2api__reference_2command__router__api.html#autotoc_md265", null ],
      [ "📚 API ფუნქციები", "md_docs_2api__reference_2command__router__api.html#autotoc_md267", [
        [ "<tt>esp_err_t register_command(const cmd_t *command)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md268", null ],
        [ "<tt>esp_err_t unregister_command(const char *command_name)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md270", null ],
        [ "<tt>bool is_command_registered(const char *command_name)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md272", null ]
      ] ],
      [ "📖 დამხმარე სტრუქტურა (<tt>cmd_t</tt>)", "md_docs_2api__reference_2command__router__api.html#autotoc_md274", null ],
      [ "💡 გამოყენების პატერნი: კონფლიქტისგან დაცული რეგისტრაცია", "md_docs_2api__reference_2command__router__api.html#autotoc_md276", null ]
    ] ],
    [ "📡 Synapse Communication Module API Reference", "md_docs_2api__reference_2communication__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2communication__api.html#autotoc_md278", null ],
      [ "📶 WiFi Manager Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md280", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md281", null ],
        [ "💡 WiFi API-ს გამოყენების მაგალითი", "md_docs_2api__reference_2communication__api.html#autotoc_md282", null ]
      ] ],
      [ "☁️ MQTT Client Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md284", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md285", null ],
        [ "💡 MQTT API-ს გამოყენების მაგალითი", "md_docs_2api__reference_2communication__api.html#autotoc_md286", null ]
      ] ],
      [ "⚡ ESP-NOW Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md288", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md289", null ]
      ] ],
      [ "📜 Naming & Structure კონვენციები", "md_docs_2api__reference_2communication__api.html#autotoc_md291", null ]
    ] ],
    [ "Synapse Configuration Manager API Reference", "md_docs_2api__reference_2configuration__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2configuration__api.html#autotoc_md294", null ],
      [ "🔑 გასაღების ფორმატი (Dot-Notation)", "md_docs_2api__reference_2configuration__api.html#autotoc_md296", null ],
      [ "⚙️ ძირითადი API ფუნქციები", "md_docs_2api__reference_2configuration__api.html#autotoc_md298", [
        [ "<tt>esp_err_t fmw_config_get_string(const char *key, char *buffer, size_t buffer_size)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md299", null ],
        [ "<tt>esp_err_t fmw_config_get_int(const char *key, int *out_value)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md300", null ],
        [ "<tt>esp_err_t fmw_config_get_bool(const char *key, bool *out_value)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md301", null ],
        [ "<tt>esp_err_t fmw_config_save(void)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md302", null ]
      ] ],
      [ "🚀 გამოყენების მაგალითები", "md_docs_2api__reference_2configuration__api.html#autotoc_md304", [
        [ "გლობალური პარამეტრის წაკითხვა", "md_docs_2api__reference_2configuration__api.html#autotoc_md305", null ],
        [ "მოდულის პარამეტრის წაკითხვა", "md_docs_2api__reference_2configuration__api.html#autotoc_md306", null ]
      ] ]
    ] ],
    [ "Synapse Core API Reference", "md_docs_2api__reference_2core__api.html", [
      [ "Service Locator API", "md_docs_2api__reference_2core__api.html#autotoc_md308", [
        [ "fmw_service_register", "md_docs_2api__reference_2core__api.html#autotoc_md309", null ],
        [ "fmw_service_unregister", "md_docs_2api__reference_2core__api.html#autotoc_md310", null ],
        [ "fmw_service_get", "md_docs_2api__reference_2core__api.html#autotoc_md311", null ],
        [ "fmw_service_get_type", "md_docs_2api__reference_2core__api.html#autotoc_md312", null ],
        [ "fmw_service_lookup_by_type", "md_docs_2api__reference_2core__api.html#autotoc_md313", null ]
      ] ],
      [ "Event Bus API", "md_docs_2api__reference_2core__api.html#autotoc_md315", [
        [ "<tt>fmw_event_bus_post</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md316", null ],
        [ "<tt>fmw_event_bus_subscribe</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md317", null ],
        [ "<tt>fmw_event_bus_unsubscribe</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md318", null ]
      ] ],
      [ "Module Register API", "md_docs_2api__reference_2core__api.html#autotoc_md320", [
        [ "fmw_module_register", "md_docs_2api__reference_2core__api.html#autotoc_md321", null ],
        [ "fmw_module_find_by_name", "md_docs_2api__reference_2core__api.html#autotoc_md322", null ]
      ] ],
      [ "Module Factory API", "md_docs_2api__reference_2core__api.html#autotoc_md324", [
        [ "fmw_module_factory_create", "md_docs_2api__reference_2core__api.html#autotoc_md325", null ]
      ] ],
      [ "Configuration Manager API", "md_docs_2api__reference_2core__api.html#autotoc_md327", [
        [ "fmw_config_get_string", "md_docs_2api__reference_2core__api.html#autotoc_md328", null ],
        [ "fmw_config_get_int", "md_docs_2api__reference_2core__api.html#autotoc_md329", null ],
        [ "fmw_config_get_module_config", "md_docs_2api__reference_2core__api.html#autotoc_md330", null ]
      ] ],
      [ "Logging API", "md_docs_2api__reference_2core__api.html#autotoc_md332", [
        [ "DEFINE_COMPONENT_TAG", "md_docs_2api__reference_2core__api.html#autotoc_md333", null ],
        [ "ESP_LOGE, ESP_LOGW, ESP_LOGI, ESP_LOGD, ESP_LOGV", "md_docs_2api__reference_2core__api.html#autotoc_md334", null ]
      ] ]
    ] ],
    [ "Synapse Custom API Reference", "md_docs_2api__reference_2custom__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2custom__api.html#autotoc_md337", null ],
      [ "Custom Module API", "md_docs_2api__reference_2custom__api.html#autotoc_md339", [
        [ "module_t *custom_module_create(const cJSON *config);", "md_docs_2api__reference_2custom__api.html#autotoc_md340", null ],
        [ "esp_err_t custom_module_do_action(module_t *module, const char *action, const void *params);", "md_docs_2api__reference_2custom__api.html#autotoc_md341", null ],
        [ "esp_err_t custom_module_get_status(module_t *module, custom_status_t *status);", "md_docs_2api__reference_2custom__api.html#autotoc_md342", null ]
      ] ],
      [ "Custom Event API", "md_docs_2api__reference_2custom__api.html#autotoc_md344", [
        [ "Custom Event-ის იდენტიფიკატორი", "md_docs_2api__reference_2custom__api.html#autotoc_md345", null ],
        [ "Custom Event-ის მონაცემების სტრუქტურა", "md_docs_2api__reference_2custom__api.html#autotoc_md346", null ],
        [ "static void custom_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2custom__api.html#autotoc_md347", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2custom__api.html#autotoc_md349", null ],
      [ "მაგალითები", "md_docs_2api__reference_2custom__api.html#autotoc_md351", null ]
    ] ],
    [ "Synapse Device Identity Service API Reference", "md_docs_2api__reference_2device__identity__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2device__identity__api.html#autotoc_md354", null ],
      [ "Device Identity Service API", "md_docs_2api__reference_2device__identity__api.html#autotoc_md356", [
        [ "<tt>const char* get_device_id(void);</tt>", "md_docs_2api__reference_2device__identity__api.html#autotoc_md357", null ],
        [ "<tt>const char* get_firmware_version(void);</tt>", "md_docs_2api__reference_2device__identity__api.html#autotoc_md358", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2device__identity__api.html#autotoc_md360", null ],
      [ "მაგალითები", "md_docs_2api__reference_2device__identity__api.html#autotoc_md362", null ]
    ] ],
    [ "Synapse Display Module API Reference", "md_docs_2api__reference_2display__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2display__api.html#autotoc_md365", null ],
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2display__api.html#autotoc_md367", [
        [ "module_t <em>ssd1306_module_create(const cJSON *config);", "md_docs_2api__reference_2display__api.html#autotoc_md368", null ],
        [ "static esp_err_t ssd1306_init(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md369", null ],
        [ "static esp_err_t ssd1306_enable(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md370", null ],
        [ "static esp_err_t ssd1306_disable(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md371", null ],
        [ "static esp_err_t ssd1306_deinit(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md372", null ]
      ] ],
      [ "Service API (Service Locator-ისთვის)", "md_docs_2api__reference_2display__api.html#autotoc_md374", [
        [ "esp_err_t ssd1306_api_enable(void);", "md_docs_2api__reference_2display__api.html#autotoc_md375", null ],
        [ "esp_err_t ssd1306_api_disable(void);", "md_docs_2api__reference_2display__api.html#autotoc_md376", null ],
        [ "esp_err_t ssd1306_api_clear(void);", "md_docs_2api__reference_2display__api.html#autotoc_md377", null ],
        [ "esp_err_t ssd1306_api_write_text(const char *text, uint8_t line);", "md_docs_2api__reference_2display__api.html#autotoc_md378", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2display__api.html#autotoc_md380", [
        [ "static void ssd1306_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2display__api.html#autotoc_md381", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2display__api.html#autotoc_md383", [
        [ "static esp_err_t parse_ssd1306_config(const cJSON *config, ssd1306_private_data_t *ssd1306_data);", "md_docs_2api__reference_2display__api.html#autotoc_md384", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2display__api.html#autotoc_md386", [
        [ "static module_status_t ssd1306_get_status(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md387", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2display__api.html#autotoc_md389", null ],
      [ "მაგალითები", "md_docs_2api__reference_2display__api.html#autotoc_md391", null ]
    ] ],
    [ "Synapse Drivers Module API Reference", "md_docs_2api__reference_2drivers__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2drivers__api.html#autotoc_md394", null ],
      [ "I2C Bus Driver API", "md_docs_2api__reference_2drivers__api.html#autotoc_md396", [
        [ "module_t *i2c_bus_module_create(const cJSON *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md397", null ],
        [ "esp_err_t i2c_bus_init(i2c_bus_handle_t *i2c_bus_handle, const i2c_bus_config_t *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md398", null ],
        [ "esp_err_t i2c_bus_read(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, uint8_t *data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md399", null ],
        [ "esp_err_t i2c_bus_write(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, const uint8_t *data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md400", null ],
        [ "esp_err_t i2c_bus_scan(i2c_bus_handle_t *i2c_bus_handle);", "md_docs_2api__reference_2drivers__api.html#autotoc_md401", null ]
      ] ],
      [ "SPI Bus Driver API", "md_docs_2api__reference_2drivers__api.html#autotoc_md403", [
        [ "module_t *spi_bus_module_create(const cJSON *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md404", null ],
        [ "esp_err_t spi_bus_init(spi_bus_handle_t *spi_bus_handle, const spi_bus_config_t *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md405", null ],
        [ "esp_err_t spi_bus_transfer(spi_bus_handle_t *spi_bus_handle, const uint8_t *tx_data, uint8_t *rx_data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md406", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2drivers__api.html#autotoc_md408", null ],
      [ "მაგალითები", "md_docs_2api__reference_2drivers__api.html#autotoc_md410", null ]
    ] ],
    [ "Synapse Event Bus API Reference", "md_docs_2api__reference_2event__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2event__api.html#autotoc_md413", null ],
      [ "Event Bus API", "md_docs_2api__reference_2event__api.html#autotoc_md415", [
        [ "<tt>esp_err_t fmw_event_bus_init(void)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md416", null ],
        [ "<tt>esp_err_t fmw_event_bus_post(core_framework_event_id_t event_id, event_data_wrapper_t *data_wrapper)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md417", null ],
        [ "<tt>esp_err_t fmw_event_bus_subscribe(core_framework_event_id_t event_id, struct module_t *module)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md418", null ],
        [ "<tt>esp_err_t fmw_event_bus_unsubscribe(core_framework_event_id_t event_id, struct module_t *module)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md419", null ]
      ] ],
      [ "ივენთის მონაცემების მართვა (Reference Counting)", "md_docs_2api__reference_2event__api.html#autotoc_md421", null ],
      [ "Event Handler-ის მაგალითი", "md_docs_2api__reference_2event__api.html#autotoc_md423", null ],
      [ "Custom Event-ების აღწერა", "md_docs_2api__reference_2event__api.html#autotoc_md425", [
        [ "მაგალითი Custom Event-ის აღწერისა", "md_docs_2api__reference_2event__api.html#autotoc_md426", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2event__api.html#autotoc_md428", null ],
      [ "მაგალითები", "md_docs_2api__reference_2event__api.html#autotoc_md430", null ]
    ] ],
    [ "API რეფერენსი: Event Payloads (event_payloads.h)", "md_docs_2api__reference_2event__payloads__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md433", null ],
      [ "Payload სტრუქტურები", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md434", [
        [ "fmw_telemetry_payload_t", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md435", null ],
        [ "fmw_config_updated_payload_t", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md436", null ]
      ] ],
      [ "მეხსიერების მართვის ფუნქციები", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md437", [
        [ "fmw_payload_common_free", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md438", null ],
        [ "fmw_telemetry_payload_free", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md439", null ]
      ] ]
    ] ],
    [ "Synapse Health Monitor Module API Reference", "md_docs_2api__reference_2health__monitor__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md441", null ],
      [ "Health Monitor Module API", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md443", [
        [ "module_t *health_monitor_module_create(const cJSON *config);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md444", null ],
        [ "esp_err_t health_monitor_module_start(module_t *module);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md445", null ],
        [ "esp_err_t health_monitor_module_stop(module_t *module);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md446", null ],
        [ "esp_err_t health_monitor_module_get_status(module_t *module, health_status_t *status);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md447", null ],
        [ "esp_err_t health_monitor_module_register_check(module_t *module, health_check_fn_t check_fn);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md448", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md450", [
        [ "static void health_monitor_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md451", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md453", null ],
      [ "მაგალითები", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md455", null ]
    ] ],
    [ "Synapse Local Control Module API Reference", "md_docs_2api__reference_2local__ctrl__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md458", null ],
      [ "Local Control Module API", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md460", [
        [ "module_t *local_ctrl_module_create(const cJSON *config);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md461", null ],
        [ "esp_err_t local_ctrl_module_start(module_t *module);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md462", null ],
        [ "esp_err_t local_ctrl_module_stop(module_t *module);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md463", null ],
        [ "esp_err_t local_ctrl_module_register_endpoint(const char *uri, http_method_t method, local_ctrl_handler_t handler);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md464", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md466", [
        [ "static void local_ctrl_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md467", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md469", null ],
      [ "მაგალითები", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md471", null ]
    ] ],
    [ "Synapse Module API Reference", "md_docs_2api__reference_2module__api.html", [
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2module__api.html#autotoc_md474", [
        [ "<tt>module_t *{module_name}_create(const cJSON *config);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md475", null ],
        [ "<tt>static esp_err_t {module_name}_init(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md476", null ],
        [ "<tt>static esp_err_t {module_name}_start(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md477", null ],
        [ "<tt>static void {module_name}_deinit(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md478", null ]
      ] ],
      [ "Service API (Service Locator-ისთვის)", "md_docs_2api__reference_2module__api.html#autotoc_md480", [
        [ "<tt>esp_err_t {module_name}_api_{action}(...);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md481", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2module__api.html#autotoc_md483", [
        [ "<tt>static void {module_name}_handle_event(module_t *self, const char *event_name, void *event_data);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md484", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2module__api.html#autotoc_md486", [
        [ "<tt>static esp_err_t parse_{module_name}_config(const cJSON *config_node, {module_name}_private_data_t *p_data);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md487", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2module__api.html#autotoc_md489", [
        [ "<tt>static module_status_t {module_name}_get_status(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md490", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2module__api.html#autotoc_md492", null ]
    ] ],
    [ "Synapse MQTT Manager Service API Reference", "md_docs_2api__reference_2mqtt__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2mqtt__api.html#autotoc_md495", null ],
      [ "MQTT Manager API", "md_docs_2api__reference_2mqtt__api.html#autotoc_md497", [
        [ "სამომავლო გაფართოება", "md_docs_2api__reference_2mqtt__api.html#autotoc_md498", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2mqtt__api.html#autotoc_md500", null ],
      [ "მაგალითები", "md_docs_2api__reference_2mqtt__api.html#autotoc_md502", null ]
    ] ],
    [ "Synapse OTA (Over-the-Air) Module API Reference", "md_docs_2api__reference_2ota__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2ota__api.html#autotoc_md505", null ],
      [ "OTA Module API", "md_docs_2api__reference_2ota__api.html#autotoc_md507", [
        [ "module_t *ota_module_create(const cJSON *config);", "md_docs_2api__reference_2ota__api.html#autotoc_md508", null ],
        [ "esp_err_t ota_module_start_update(const char *url);", "md_docs_2api__reference_2ota__api.html#autotoc_md509", null ],
        [ "esp_err_t ota_module_get_status(ota_status_t *status);", "md_docs_2api__reference_2ota__api.html#autotoc_md510", null ],
        [ "esp_err_t ota_module_abort(void);", "md_docs_2api__reference_2ota__api.html#autotoc_md511", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2ota__api.html#autotoc_md513", [
        [ "static void ota_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2ota__api.html#autotoc_md514", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2ota__api.html#autotoc_md516", null ],
      [ "მაგალითები", "md_docs_2api__reference_2ota__api.html#autotoc_md518", null ]
    ] ],
    [ "📲 Synapse Provisioning Module API Reference", "md_docs_2api__reference_2provisioning__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2provisioning__api.html#autotoc_md521", null ],
      [ "🔵 BLE Provisioning Service API (იმპლემენტირებულია)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md523", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2provisioning__api.html#autotoc_md524", null ]
      ] ],
      [ "📡 SoftAP Provisioning Module API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md526", [
        [ "module_t *softap_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md527", null ],
        [ "esp_err_t softap_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md528", null ],
        [ "esp_err_t softap_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md529", null ],
        [ "esp_err_t softap_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md530", null ],
        [ "esp_err_t softap_prov_module_get_status(module_t *module, softap_prov_status_t*status)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md531", null ]
      ] ],
      [ "🔳 QR Code Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md533", [
        [ "esp_err_t qr_code_prov_generate(const char *ssid, const char*password, char *qr_code_buffer, size_t buffer_size)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md534", null ],
        [ "esp_err_t qr_code_prov_display(const char *qr_code_buffer)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md535", null ]
      ] ],
      [ "🔌 Serial Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md537", [
        [ "module_t *serial_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md538", null ],
        [ "esp_err_t serial_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md539", null ],
        [ "esp_err_t serial_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md540", null ],
        [ "esp_err_t serial_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md541", null ]
      ] ],
      [ "☁️ Cloud Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md543", [
        [ "module_t *cloud_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md544", null ],
        [ "esp_err_t cloud_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md545", null ],
        [ "esp_err_t cloud_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md546", null ],
        [ "esp_err_t cloud_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md547", null ]
      ] ],
      [ "NFC Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md549", [
        [ "module_t *nfc_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md550", null ],
        [ "esp_err_t nfc_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md551", null ],
        [ "esp_err_t nfc_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md552", null ],
        [ "esp_err_t nfc_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md553", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2provisioning__api.html#autotoc_md555", null ],
      [ "მაგალითები", "md_docs_2api__reference_2provisioning__api.html#autotoc_md557", null ]
    ] ],
    [ "🔌 Relay Actuator API Reference", "md_docs_2api__reference_2relay__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2relay__api.html#autotoc_md560", null ],
      [ "1. ⌨️ CLI (Command Line Interface)", "md_docs_2api__reference_2relay__api.html#autotoc_md562", null ],
      [ "2. 📡 MQTT ინტერფეისი", "md_docs_2api__reference_2relay__api.html#autotoc_md564", [
        [ "ბრძანების გაგზავნა", "md_docs_2api__reference_2relay__api.html#autotoc_md565", null ],
        [ "სტატუსის მიღება", "md_docs_2api__reference_2relay__api.html#autotoc_md566", null ]
      ] ],
      [ "3. 📢 ივენთები (Events)", "md_docs_2api__reference_2relay__api.html#autotoc_md568", null ]
    ] ],
    [ "Synapse Resource Manager API Reference", "md_docs_2api__reference_2resource__manager__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md571", null ],
      [ "სამართავი რესურსების ტიპები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md573", null ],
      [ "API ფუნქციები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md575", [
        [ "<tt>esp_err_t fmw_resource_manager_init(void)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md576", null ],
        [ "<tt>esp_err_t fmw_resource_lock(fmw_resource_type_t type, uint8_t resource_id, const char *owner)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md578", null ],
        [ "<tt>esp_err_t fmw_resource_release(fmw_resource_type_t type, uint8_t resource_id, const char *owner)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md580", null ],
        [ "<tt>bool fmw_resource_is_locked(fmw_resource_type_t type, uint8_t resource_id)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md582", null ],
        [ "<tt>const char *fmw_resource_get_owner(fmw_resource_type_t type, uint8_t resource_id)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md584", null ]
      ] ],
      [ "გამოყენების მაგალითი", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md586", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md588", null ]
    ] ],
    [ "🛡️ Security Status API Reference", "md_docs_2api__reference_2security__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2security__api.html#autotoc_md591", null ],
      [ "1. 🔌 Service API (<tt>security_status_api_t</tt>)", "md_docs_2api__reference_2security__api.html#autotoc_md593", [
        [ "API ფუნქციები", "md_docs_2api__reference_2security__api.html#autotoc_md594", null ]
      ] ],
      [ "2. ⌨️ CLI (Command Line Interface)", "md_docs_2api__reference_2security__api.html#autotoc_md596", null ]
    ] ],
    [ "Synapse Sensor Module API Reference", "md_docs_2api__reference_2sensor__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2sensor__api.html#autotoc_md598", null ],
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2sensor__api.html#autotoc_md600", [
        [ "module_t <em>dht22_module_create(const cJSON *config);", "md_docs_2api__reference_2sensor__api.html#autotoc_md601", null ],
        [ "static esp_err_t dht22_init(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md602", null ],
        [ "static esp_err_t dht22_enable(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md603", null ],
        [ "static esp_err_t dht22_disable(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md604", null ],
        [ "static esp_err_t dht22_deinit(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md605", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2sensor__api.html#autotoc_md607", [
        [ "static void dht22_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2sensor__api.html#autotoc_md608", null ],
        [ "static esp_err_t publish_sensor_data(dht22_private_data_t *dht22_data, float temperature, float humidity);", "md_docs_2api__reference_2sensor__api.html#autotoc_md609", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2sensor__api.html#autotoc_md611", [
        [ "static esp_err_t parse_dht22_config(const cJSON *config, dht22_private_data_t *dht22_data);", "md_docs_2api__reference_2sensor__api.html#autotoc_md612", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2sensor__api.html#autotoc_md614", [
        [ "static module_status_t dht22_get_status(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md615", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2sensor__api.html#autotoc_md617", null ],
      [ "მაგალითები", "md_docs_2api__reference_2sensor__api.html#autotoc_md619", null ]
    ] ],
    [ "💾 Synapse Storage Manager API Reference", "md_docs_2api__reference_2storage__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2storage__api.html#autotoc_md622", null ],
      [ "⚙️ API-ს სტრუქტურა (<tt>storage_api_t</tt>)", "md_docs_2api__reference_2storage__api.html#autotoc_md624", [
        [ "🔑 Key-Value ოპერაციები", "md_docs_2api__reference_2storage__api.html#autotoc_md625", null ],
        [ "🗂️ ფაილური ოპერაციები", "md_docs_2api__reference_2storage__api.html#autotoc_md627", null ]
      ] ],
      [ "💡 გამოყენების მაგალითი", "md_docs_2api__reference_2storage__api.html#autotoc_md629", null ]
    ] ],
    [ "Synapse System Module API Reference", "md_docs_2api__reference_2system__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2system__api.html#autotoc_md632", null ],
      [ "OTA Module API", "md_docs_2api__reference_2system__api.html#autotoc_md634", [
        [ "module_t *ota_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md635", null ],
        [ "esp_err_t ota_module_start_update(const char *url);", "md_docs_2api__reference_2system__api.html#autotoc_md636", null ],
        [ "esp_err_t ota_module_get_status(ota_status_t *status);", "md_docs_2api__reference_2system__api.html#autotoc_md637", null ]
      ] ],
      [ "Version Module API", "md_docs_2api__reference_2system__api.html#autotoc_md639", [
        [ "module_t *version_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md640", null ],
        [ "esp_err_t version_module_get_firmware_version(char *version_str, size_t size);", "md_docs_2api__reference_2system__api.html#autotoc_md641", null ]
      ] ],
      [ "Watchdog Module API", "md_docs_2api__reference_2system__api.html#autotoc_md643", [
        [ "module_t *watchdog_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md644", null ],
        [ "esp_err_t watchdog_module_feed(void);", "md_docs_2api__reference_2system__api.html#autotoc_md645", null ],
        [ "esp_err_t watchdog_module_set_timeout(uint32_t timeout_ms);", "md_docs_2api__reference_2system__api.html#autotoc_md646", null ]
      ] ],
      [ "Diagnostics Module API", "md_docs_2api__reference_2system__api.html#autotoc_md648", [
        [ "module_t *diagnostics_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md649", null ],
        [ "esp_err_t diagnostics_module_run_tests(void);", "md_docs_2api__reference_2system__api.html#autotoc_md650", null ],
        [ "esp_err_t diagnostics_module_get_report(char *report_buffer, size_t buffer_size);", "md_docs_2api__reference_2system__api.html#autotoc_md651", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2system__api.html#autotoc_md653", null ],
      [ "მაგალითები", "md_docs_2api__reference_2system__api.html#autotoc_md655", null ]
    ] ],
    [ "Synapse Time Sync Module API Reference", "md_docs_2api__reference_2time__sync__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2time__sync__api.html#autotoc_md658", null ],
      [ "Time Sync Module API", "md_docs_2api__reference_2time__sync__api.html#autotoc_md660", [
        [ "module_t *time_sync_module_create(const cJSON *config);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md661", null ],
        [ "esp_err_t time_sync_module_start(module_t *module);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md662", null ],
        [ "esp_err_t time_sync_module_stop(module_t *module);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md663", null ],
        [ "esp_err_t time_sync_module_set_server(module_t *module, const char *server_url);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md664", null ],
        [ "esp_err_t time_sync_module_get_time(time_t *current_time);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md665", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2time__sync__api.html#autotoc_md667", [
        [ "static void time_sync_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md668", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2time__sync__api.html#autotoc_md670", null ],
      [ "მაგალითები", "md_docs_2api__reference_2time__sync__api.html#autotoc_md672", null ]
    ] ],
    [ "Synapse Utility Module API Reference", "md_docs_2api__reference_2utility__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2utility__api.html#autotoc_md675", null ],
      [ "Logger Module API", "md_docs_2api__reference_2utility__api.html#autotoc_md677", [
        [ "module_t *logger_module_create(const cJSON *config);", "md_docs_2api__reference_2utility__api.html#autotoc_md678", null ],
        [ "esp_err_t logger_module_log_info(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md679", null ],
        [ "esp_err_t logger_module_log_warning(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md680", null ],
        [ "esp_err_t logger_module_log_error(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md681", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2utility__api.html#autotoc_md683", null ],
      [ "მაგალითები", "md_docs_2api__reference_2utility__api.html#autotoc_md685", null ]
    ] ],
    [ "Synapse Version Module API Reference", "md_docs_2api__reference_2version__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2version__api.html#autotoc_md688", null ],
      [ "Version Module API", "md_docs_2api__reference_2version__api.html#autotoc_md690", [
        [ "module_t *version_module_create(const cJSON *config);", "md_docs_2api__reference_2version__api.html#autotoc_md691", null ],
        [ "esp_err_t version_module_get_firmware_version(char *version_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md692", null ],
        [ "esp_err_t version_module_get_build_date(char *date_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md693", null ],
        [ "esp_err_t version_module_get_build_time(char *time_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md694", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2version__api.html#autotoc_md696", null ],
      [ "მაგალითები", "md_docs_2api__reference_2version__api.html#autotoc_md698", null ]
    ] ],
    [ "Synapse WiFi Manager Service API Reference", "md_docs_2api__reference_2wifi__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2wifi__api.html#autotoc_md701", null ],
      [ "WiFi Manager API", "md_docs_2api__reference_2wifi__api.html#autotoc_md703", [
        [ "სამომავლო გაფართოება", "md_docs_2api__reference_2wifi__api.html#autotoc_md704", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2wifi__api.html#autotoc_md706", null ],
      [ "მაგალითები", "md_docs_2api__reference_2wifi__api.html#autotoc_md708", null ]
    ] ],
    [ "CHANGELOG: led_blinker ინიციალიზაციის პრობლემის დიაგნოსტიკა და გამოსწორება", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html", [
      [ "პრობლემა", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md711", null ],
      [ "დიაგნოსტიკა", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md712", null ],
      [ "მიზეზი", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md713", null ],
      [ "გამოსავალი", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md714", null ],
      [ "ცვლილებები კოდში", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md715", null ],
      [ "რეკომენდაცია", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md716", null ]
    ] ],
    [ "ცვლილებების ჟურნალი — 2025-06-24", "md_docs_2changelog_2CHANGELOG__2025-06-24.html", [
      [ "დამატებული და განახლებული ფაილები", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md719", [
        [ "[core/CMakeLists.txt]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md720", null ],
        [ "[core/collect_modules.cmake]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md721", null ],
        [ "[core/include/system_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md722", null ],
        [ "[interfaces/include/base_module.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md723", null ],
        [ "[core/include/service_locator.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md724", null ],
        [ "[core/include/module_registry.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md725", null ],
        [ "[core/include/event_bus.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md726", null ],
        [ "[core/include/config_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md727", null ],
        [ "[core/include/system_event_ids.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md728", null ],
        [ "[core/include/resource_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md729", null ],
        [ "[core/include/framework_config.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md730", null ],
        [ "[interfaces/include/logging.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md731", null ],
        [ "[interfaces/include/service_types.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md732", null ],
        [ "[interfaces/include/telemetry_events.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md733", null ],
        [ "[README.md]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md734", null ],
        [ "[.gitignore]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md735", null ]
      ] ],
      [ "ძირითადი ცვლილებები", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md737", null ]
    ] ],
    [ "CHANGELOG - 2025-06-26", "md_docs_2changelog_2CHANGELOG__2025-06-26.html", [
      [ "[Major] Module System Automation & Refactor", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md740", [
        [ "Added", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md741", null ],
        [ "Changed", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md742", null ],
        [ "Fixed", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md743", null ],
        [ "Detailed File Changes", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md744", null ],
        [ "Why/How", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md745", null ],
        [ "Testing", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md746", null ]
      ] ]
    ] ],
    [ "Changelog", "md_docs_2changelog_2CHANGELOG__2025-06-28.html", [
      [ "[2.0.0] - 2025-06-28", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md748", [
        [ "დამატებულია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md749", null ],
        [ "შეცვლილია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md750", null ],
        [ "გაუქმებულია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md751", null ],
        [ "წაშლილია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md752", null ],
        [ "გასწორებულია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md753", null ],
        [ "უსაფრთხოება", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md754", null ],
        [ "ცნობილი პრობლემები", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md755", null ]
      ] ]
    ] ],
    [ "Synapse Branching Strategy", "md_docs_2contributing_2branching__strategy.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2branching__strategy.html#autotoc_md758", null ],
      [ "ძირითადი ბრენჩები", "md_docs_2contributing_2branching__strategy.html#autotoc_md760", null ],
      [ "ბრენჩის სახელდების წესები", "md_docs_2contributing_2branching__strategy.html#autotoc_md762", null ],
      [ "სამუშაო პროცესი", "md_docs_2contributing_2branching__strategy.html#autotoc_md764", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2contributing_2branching__strategy.html#autotoc_md766", null ],
      [ "რესურსები", "md_docs_2contributing_2branching__strategy.html#autotoc_md768", null ]
    ] ],
    [ "ქცევის კოდექსი (Code of Conduct)", "md_docs_2contributing_2code__of__conduct.html", [
      [ "მიზანი", "md_docs_2contributing_2code__of__conduct.html#autotoc_md772", null ],
      [ "ძირითადი პრინციპები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md774", null ],
      [ "დაუშვებელი ქცევა", "md_docs_2contributing_2code__of__conduct.html#autotoc_md776", null ],
      [ "პასუხისმგებლობები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md778", null ],
      [ "დარღვევის შემთხვევაში მოქმედება", "md_docs_2contributing_2code__of__conduct.html#autotoc_md780", null ],
      [ "დამატებითი რესურსები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md782", null ]
    ] ],
    [ "Synapse Commit Message Guidelines", "md_docs_2contributing_2commit__message__guidelines.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md786", null ],
      [ "ძირითადი სტრუქტურა", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md788", null ],
      [ "სათაურის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md790", null ],
      [ "დეტალური აღწერის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md792", null ],
      [ "Footer-ის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md794", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md796", null ],
      [ "რესურსები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md798", null ]
    ] ],
    [ "კონტრიბუციის სახელმძღვანელო (Contributing Guide)", "md_docs_2contributing_2contributing__guide.html", [
      [ "მიზანი", "md_docs_2contributing_2contributing__guide.html#autotoc_md801", null ],
      [ "1. კონტრიბუციის პროცესი", "md_docs_2contributing_2contributing__guide.html#autotoc_md803", null ],
      [ "2. კოდის სტანდარტები", "md_docs_2contributing_2contributing__guide.html#autotoc_md804", null ],
      [ "3. დოკუმენტაციის მოთხოვნები", "md_docs_2contributing_2contributing__guide.html#autotoc_md805", null ],
      [ "4. Pull Request-ის მიმოხილვა", "md_docs_2contributing_2contributing__guide.html#autotoc_md806", null ],
      [ "5. Best Practices", "md_docs_2contributing_2contributing__guide.html#autotoc_md807", null ],
      [ "6. აკრძალული პრაქტიკები", "md_docs_2contributing_2contributing__guide.html#autotoc_md808", null ]
    ] ],
    [ "Synapse Contributing Reference Index", "md_docs_2contributing_2contributing__index.html", [
      [ "ძირითადი სახელმძღვანელოები", "md_docs_2contributing_2contributing__index.html#autotoc_md812", null ],
      [ "შაბლონები", "md_docs_2contributing_2contributing__index.html#autotoc_md813", null ]
    ] ],
    [ "კონვენციების ავტომატური შემმოწმებელი (<tt>check_conventions.py</tt>)", "md_docs_2contributing_2convention__linter__guide.html", [
      [ "1. შესავალი", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md816", null ],
      [ "2. რას ამოწმებს სკრიპტი?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md817", null ],
      [ "3. როგორ გამოვიყენოთ ლოკალურად?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md822", null ],
      [ "4. ინტეგრაცია GitHub Actions-თან (ავტომატური შემოწმება)", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md824", null ],
      [ "5. როგორ გამოვასწოროთ გავრცელებული შეცდომები?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md825", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (FAQ)", "md_docs_2contributing_2faq.html", [
      [ "1. როგორ დავიწყო კონტრიბუცია Synapse Framework-ში?", "md_docs_2contributing_2faq.html#autotoc_md828", null ],
      [ "2. რა სტანდარტებს და კონვენციებს უნდა დავიცვა?", "md_docs_2contributing_2faq.html#autotoc_md829", null ],
      [ "3. როგორ შევქმნა Pull Request?", "md_docs_2contributing_2faq.html#autotoc_md830", null ],
      [ "4. როგორ მოვამზადო ლოკალური გარემო?", "md_docs_2contributing_2faq.html#autotoc_md831", null ],
      [ "5. როგორ გავუშვა ტესტები?", "md_docs_2contributing_2faq.html#autotoc_md832", null ],
      [ "6. როგორ მოვაგვარო გარემოს ან ბილდის პრობლემები?", "md_docs_2contributing_2faq.html#autotoc_md833", null ],
      [ "7. სად ვიპოვო დოკუმენტაციის სტრუქტურის და სტანდარტების შესახებ ინფორმაცია?", "md_docs_2contributing_2faq.html#autotoc_md834", null ],
      [ "8. როგორ დავამატო ან განვაახლო დოკუმენტაცია?", "md_docs_2contributing_2faq.html#autotoc_md835", null ],
      [ "9. როგორ დავიცვა ქცევის კოდექსი?", "md_docs_2contributing_2faq.html#autotoc_md836", null ],
      [ "10. ვის მივმართო დამატებითი კითხვების ან პრობლემების შემთხვევაში?", "md_docs_2contributing_2faq.html#autotoc_md837", null ]
    ] ],
    [ "Synapse Issue Template", "md_docs_2contributing_2issue__template.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2issue__template.html#autotoc_md841", null ],
      [ "ძირითადი ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md843", null ],
      [ "დეტალური აღწერა", "md_docs_2contributing_2issue__template.html#autotoc_md845", null ],
      [ "გამეორების ნაბიჯები (თუ ეს ბაგია)", "md_docs_2contributing_2issue__template.html#autotoc_md847", null ],
      [ "მოსალოდნელი შედეგი", "md_docs_2contributing_2issue__template.html#autotoc_md849", null ],
      [ "ფაქტობრივი შედეგი", "md_docs_2contributing_2issue__template.html#autotoc_md851", null ],
      [ "გარემოს ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md853", null ],
      [ "დამატებითი ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md855", null ],
      [ "რესურსები", "md_docs_2contributing_2issue__template.html#autotoc_md857", null ]
    ] ],
    [ "Synapse Local Setup Guide", "md_docs_2contributing_2local__setup.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2local__setup.html#autotoc_md861", null ],
      [ "სისტემური მოთხოვნები", "md_docs_2contributing_2local__setup.html#autotoc_md863", null ],
      [ "ESP-IDF-ის ინსტალაცია", "md_docs_2contributing_2local__setup.html#autotoc_md865", null ],
      [ "პროექტის კლონირება", "md_docs_2contributing_2local__setup.html#autotoc_md867", null ],
      [ "sdkconfig ფაილის კონფიგურაცია", "md_docs_2contributing_2local__setup.html#autotoc_md869", null ],
      [ "პროექტის აგება და გაშვება", "md_docs_2contributing_2local__setup.html#autotoc_md871", null ],
      [ "ტესტირება", "md_docs_2contributing_2local__setup.html#autotoc_md873", null ],
      [ "პრობლემების შემთხვევაში", "md_docs_2contributing_2local__setup.html#autotoc_md875", null ],
      [ "რესურსები", "md_docs_2contributing_2local__setup.html#autotoc_md877", null ]
    ] ],
    [ "Synapse Pull Request Template", "md_docs_2contributing_2pull__request__template.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2pull__request__template.html#autotoc_md881", null ],
      [ "ძირითადი ინფორმაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md883", null ],
      [ "ცვლილებების დეტალური აღწერა", "md_docs_2contributing_2pull__request__template.html#autotoc_md885", null ],
      [ "ტესტირება და ვალიდაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md887", null ],
      [ "მიმოხილვის რეკომენდაციები", "md_docs_2contributing_2pull__request__template.html#autotoc_md889", null ],
      [ "დამატებითი ინფორმაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md891", null ],
      [ "რესურსები", "md_docs_2contributing_2pull__request__template.html#autotoc_md893", null ]
    ] ],
    [ "Synapse Review Guidelines", "md_docs_2contributing_2review__guidelines.html", [
      [ "მიმოხილვის მიზანი", "md_docs_2contributing_2review__guidelines.html#autotoc_md897", null ],
      [ "მიმოხილვის პროცესი", "md_docs_2contributing_2review__guidelines.html#autotoc_md899", null ],
      [ "მიმოხილვის კრიტერიუმები", "md_docs_2contributing_2review__guidelines.html#autotoc_md901", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2contributing_2review__guidelines.html#autotoc_md903", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2contributing_2review__guidelines.html#autotoc_md905", null ],
      [ "რესურსები", "md_docs_2contributing_2review__guidelines.html#autotoc_md907", null ]
    ] ],
    [ "9. Doxygen კომენტარების სტანდარტი (v1.0)", "md_docs_2convention_2coding__doxygen__standard.html", [
      [ "1. შესავალი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md910", null ],
      [ "2. ფაილის სათაურის (Header) კომენტარი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md911", [
        [ "2.1. სტრუქტურა", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md912", null ],
        [ "2.2. მაგალითი (<tt>i2c_bus_module.h</tt>)", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md913", null ]
      ] ],
      [ "3. ფუნქციის კომენტარი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md914", [
        [ "3.1. სტრუქტურა", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md915", null ],
        [ "3.2. მაგალითი (<tt>service_locator.h</tt>)", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md916", null ]
      ] ],
      [ "4. სტრუქტურების და ტიპების კომენტარები", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md917", [
        [ "4.1. სტრუქტურის მაგალითი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md918", null ]
      ] ],
      [ "5. Doxygen ტეგების გამოყენების წესები", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md919", null ]
    ] ],
    [ "კომუნიკაციის პატერნების წესები", "md_docs_2convention_2communication__patterns.html", [
      [ "ძირითადი პრინციპი: მაქსიმალური იზოლაცია", "md_docs_2convention_2communication__patterns.html#autotoc_md921", null ],
      [ "სამი ძირითადი პატერნი", "md_docs_2convention_2communication__patterns.html#autotoc_md922", [
        [ "1. Service Locator Pattern", "md_docs_2convention_2communication__patterns.html#autotoc_md923", null ],
        [ "2. Event Bus Pattern", "md_docs_2convention_2communication__patterns.html#autotoc_md924", null ],
        [ "3. Command Router Pattern (ახალი სექცია)", "md_docs_2convention_2communication__patterns.html#autotoc_md925", null ]
      ] ],
      [ "კომუნიკაციის ტიპები", "md_docs_2convention_2communication__patterns.html#autotoc_md926", [
        [ "Direct API Communication (Service Locator)", "md_docs_2convention_2communication__patterns.html#autotoc_md927", null ],
        [ "Broadcast Communication (Event Bus)", "md_docs_2convention_2communication__patterns.html#autotoc_md928", null ]
      ] ],
      [ "აკრძალული პატერნები", "md_docs_2convention_2communication__patterns.html#autotoc_md929", [
        [ "❌ პირდაპირი დამოკიდებულებები", "md_docs_2convention_2communication__patterns.html#autotoc_md930", null ],
        [ "❌ პატერნების არასწორი შერევა", "md_docs_2convention_2communication__patterns.html#autotoc_md931", null ],
        [ "❌ System Manager MQTT Handling", "md_docs_2convention_2communication__patterns.html#autotoc_md932", null ]
      ] ],
      [ "სწორი იმპლემენტაცია", "md_docs_2convention_2communication__patterns.html#autotoc_md933", [
        [ "Service Locator-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md934", [
          [ "1. Service Registration (Display Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md935", null ],
          [ "2. Service Usage (MQTT Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md936", null ]
        ] ],
        [ "Event Bus-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md937", [
          [ "1. Event Publishing (Sensor Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md938", null ],
          [ "2. Event Subscription (MQTT Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md939", null ]
        ] ]
      ] ],
      [ "დინამიური MQTT თემები <tt>{module_name}</tt> Placeholder-ით", "md_docs_2convention_2communication__patterns.html#autotoc_md940", [
        [ "პატერნის აღწერა", "md_docs_2convention_2communication__patterns.html#autotoc_md941", null ]
      ] ],
      [ "პატერნის არჩევის გზამკვლევი", "md_docs_2convention_2communication__patterns.html#autotoc_md942", [
        [ "Service Locator-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md943", null ],
        [ "Event Bus-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md944", null ]
      ] ],
      [ "Architecture Diagram", "md_docs_2convention_2communication__patterns.html#autotoc_md945", null ],
      [ "შეჯამება", "md_docs_2convention_2communication__patterns.html#autotoc_md946", null ]
    ] ],
    [ "კონფიგურაციის მართვის წესები", "md_docs_2convention_2configuration__management.html", [
      [ "ძირითადი პრინციპი", "md_docs_2convention_2configuration__management.html#autotoc_md948", null ],
      [ "კონფიგურაციის სტრუქტურა და წყაროები", "md_docs_2convention_2configuration__management.html#autotoc_md949", null ],
      [ "კონფიგურაციის წვდომის პატერნები", "md_docs_2convention_2configuration__management.html#autotoc_md950", [
        [ "<tt>fmw_config_get_*</tt> ფუნქციების გამოყენება", "md_docs_2convention_2configuration__management.html#autotoc_md951", null ]
      ] ],
      [ "კონფიგურაციის პარსინგი მოდულის შიგნით", "md_docs_2convention_2configuration__management.html#autotoc_md952", [
        [ "<tt>parse_config</tt> ფუნქცია", "md_docs_2convention_2configuration__management.html#autotoc_md953", null ]
      ] ],
      [ "Default მნიშვნელობები და ვალიდაცია", "md_docs_2convention_2configuration__management.html#autotoc_md954", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2configuration__management.html#autotoc_md955", null ],
      [ "შეჯამება", "md_docs_2convention_2configuration__management.html#autotoc_md956", null ]
    ] ],
    [ "Synapse Convention Reference Index", "md_docs_2convention_2convention__index.html", [
      [ "Coding & Documentation", "md_docs_2convention_2convention__index.html#autotoc_md959", null ],
      [ "Naming & Structure", "md_docs_2convention_2convention__index.html#autotoc_md960", null ],
      [ "Patterns & Management", "md_docs_2convention_2convention__index.html#autotoc_md961", null ]
    ] ],
    [ "Error Handling-ის წესები", "md_docs_2convention_2error__handling.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2error__handling.html#autotoc_md964", [
        [ "1. ESP Error Codes გამოყენება", "md_docs_2convention_2error__handling.html#autotoc_md965", null ],
        [ "2. Comprehensive Error Checking", "md_docs_2convention_2error__handling.html#autotoc_md966", null ]
      ] ],
      [ "Standard Error Codes", "md_docs_2convention_2error__handling.html#autotoc_md967", [
        [ "Framework-Specific Errors", "md_docs_2convention_2error__handling.html#autotoc_md968", null ],
        [ "Common ESP Error Codes", "md_docs_2convention_2error__handling.html#autotoc_md969", null ]
      ] ],
      [ "Error Handling Patterns", "md_docs_2convention_2error__handling.html#autotoc_md970", [
        [ "1. Parameter Validation", "md_docs_2convention_2error__handling.html#autotoc_md971", null ],
        [ "2. Resource Allocation Checking", "md_docs_2convention_2error__handling.html#autotoc_md972", null ],
        [ "3. Service Locator Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md973", null ],
        [ "4. Configuration Parsing Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md974", null ],
        [ "5. Hardware Operation Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md975", null ]
      ] ],
      [ "Cleanup and Resource Management", "md_docs_2convention_2error__handling.html#autotoc_md976", [
        [ "1. Proper Cleanup on Errors", "md_docs_2convention_2error__handling.html#autotoc_md977", null ],
        [ "2. Resource Cleanup Functions", "md_docs_2convention_2error__handling.html#autotoc_md978", null ]
      ] ],
      [ "Logging Standards", "md_docs_2convention_2error__handling.html#autotoc_md979", [
        [ "1. Error Levels", "md_docs_2convention_2error__handling.html#autotoc_md980", null ],
        [ "2. Error Context Information", "md_docs_2convention_2error__handling.html#autotoc_md981", null ]
      ] ],
      [ "Error Recovery Strategies", "md_docs_2convention_2error__handling.html#autotoc_md982", [
        [ "1. Retry Logic", "md_docs_2convention_2error__handling.html#autotoc_md983", null ],
        [ "2. Graceful Degradation", "md_docs_2convention_2error__handling.html#autotoc_md984", null ]
      ] ],
      [ "შეჯამება", "md_docs_2convention_2error__handling.html#autotoc_md985", null ]
    ] ],
    [ "ფუნქციების დასახელების წესები", "md_docs_2convention_2function__naming.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2function__naming.html#autotoc_md987", [
        [ "1. სტრუქტურა", "md_docs_2convention_2function__naming.html#autotoc_md988", null ],
        [ "2. Scope პრეფიქსები", "md_docs_2convention_2function__naming.html#autotoc_md989", [
          [ "Public API ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md990", null ],
          [ "Static/Private ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md991", null ],
          [ "Framework API ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md992", null ]
        ] ]
      ] ],
      [ "მოდულის ტიპების მიხედვით", "md_docs_2convention_2function__naming.html#autotoc_md993", [
        [ "1. Service Locator", "md_docs_2convention_2function__naming.html#autotoc_md994", null ],
        [ "2. Event Bus", "md_docs_2convention_2function__naming.html#autotoc_md995", null ],
        [ "3. Display Modules", "md_docs_2convention_2function__naming.html#autotoc_md996", null ],
        [ "4. Communication Modules", "md_docs_2convention_2function__naming.html#autotoc_md997", null ],
        [ "5. Sensor Modules", "md_docs_2convention_2function__naming.html#autotoc_md998", null ]
      ] ],
      [ "სპეციალური კონვენციები", "md_docs_2convention_2function__naming.html#autotoc_md999", [
        [ "1. Module Lifecycle ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1000", null ],
        [ "2. Runtime Management ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1001", null ],
        [ "3. Event Handler ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1002", null ],
        [ "4. Task ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1003", null ]
      ] ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2function__naming.html#autotoc_md1004", null ],
      [ "მაგალითი სრული მოდული", "md_docs_2convention_2function__naming.html#autotoc_md1005", null ]
    ] ],
    [ "🚀 მოდულების ინიციალიზაციის დონეები (Initialization Levels)", "md_docs_2convention_2initialization__levels.html", [
      [ "🎯 მიზანი", "md_docs_2convention_2initialization__levels.html#autotoc_md1007", null ],
      [ "⚙️ როგორ მუშაობს?", "md_docs_2convention_2initialization__levels.html#autotoc_md1008", null ],
      [ "📜 ოქროს წესი", "md_docs_2convention_2initialization__levels.html#autotoc_md1009", [
        [ "💡 პრაქტიკული მაგალითი: <tt>wifi_manager</tt> და <tt>command_router</tt>", "md_docs_2convention_2initialization__levels.html#autotoc_md1010", null ]
      ] ],
      [ "📊 ინიციალიზაციის დონეების რეკომენდებული დიაპაზონები", "md_docs_2convention_2initialization__levels.html#autotoc_md1011", [
        [ "📦 <strong>დონე 1: ფუნდამენტური დრაივერები და რესურსები (0-19)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1013", null ],
        [ "🛠️ <strong>დონე 2: საბაზისო სისტემური სერვისები (20-39)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1015", null ],
        [ "📡 <strong>დონე 3: ქსელი, კონტროლი და კომუნიკაცია (40-59)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1017", null ],
        [ "💡 <strong>დონე 4: აპლიკაციის ლოგიკა და პერიფერია (60-79)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1019", null ],
        [ "🧩 <strong>დონე 5: მაღალი დონის კომპოზიტური მოდულები (80-100)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1021", null ]
      ] ],
      [ "📝 როგორ განვსაზღვროთ <tt>init_level</tt> ახალი მოდულისთვის?", "md_docs_2convention_2initialization__levels.html#autotoc_md1023", null ]
    ] ],
    [ "Logging-ის სტანდარტები", "md_docs_2convention_2logging__standards.html", [
      [ "Component Tag Definition", "md_docs_2convention_2logging__standards.html#autotoc_md1025", [
        [ "სტანდარტული ფორმატი", "md_docs_2convention_2logging__standards.html#autotoc_md1026", null ],
        [ "მაგალითები", "md_docs_2convention_2logging__standards.html#autotoc_md1027", null ]
      ] ],
      [ "Log Levels", "md_docs_2convention_2logging__standards.html#autotoc_md1028", [
        [ "1. ESP_LOGE - Error Level", "md_docs_2convention_2logging__standards.html#autotoc_md1029", null ],
        [ "2. ESP_LOGW - Warning Level", "md_docs_2convention_2logging__standards.html#autotoc_md1030", null ],
        [ "3. ESP_LOGI - Info Level", "md_docs_2convention_2logging__standards.html#autotoc_md1031", null ],
        [ "4. ESP_LOGD - Debug Level", "md_docs_2convention_2logging__standards.html#autotoc_md1032", null ],
        [ "5. ESP_LOGV - Verbose Level", "md_docs_2convention_2logging__standards.html#autotoc_md1033", null ]
      ] ],
      [ "Logging Patterns", "md_docs_2convention_2logging__standards.html#autotoc_md1034", [
        [ "1. Module Lifecycle Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1035", null ],
        [ "2. Service Registration Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1036", null ],
        [ "3. Configuration Parsing Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1037", null ],
        [ "4. Hardware Operation Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1038", null ],
        [ "5. Event Handling Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1039", null ],
        [ "6. Error Context Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1040", null ]
      ] ],
      [ "Runtime State Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1041", [
        [ "1. Module Enable/Disable", "md_docs_2convention_2logging__standards.html#autotoc_md1042", null ],
        [ "2. Task State Changes", "md_docs_2convention_2logging__standards.html#autotoc_md1043", null ]
      ] ],
      [ "Performance Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1044", [
        [ "1. Operation Timing", "md_docs_2convention_2logging__standards.html#autotoc_md1045", null ],
        [ "2. Memory Usage", "md_docs_2convention_2logging__standards.html#autotoc_md1046", null ]
      ] ],
      [ "Log Message Format Standards", "md_docs_2convention_2logging__standards.html#autotoc_md1047", [
        [ "1. Consistent Formatting", "md_docs_2convention_2logging__standards.html#autotoc_md1048", null ],
        [ "2. Parameter Naming", "md_docs_2convention_2logging__standards.html#autotoc_md1049", null ]
      ] ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2logging__standards.html#autotoc_md1050", null ],
      [ "შეჯამება", "md_docs_2convention_2logging__standards.html#autotoc_md1051", null ]
    ] ],
    [ "მოდულის სტრუქტურის წესები", "md_docs_2convention_2module__structure.html", [
      [ "საქაღალდის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1053", [
        [ "სტანდარტული მოდულის layout", "md_docs_2convention_2module__structure.html#autotoc_md1054", null ],
        [ "კატეგორიები", "md_docs_2convention_2module__structure.html#autotoc_md1055", null ]
      ] ],
      [ "ფაილის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1056", [
        [ "1. <tt>config.json</tt> (ახალი სექცია)", "md_docs_2convention_2module__structure.html#autotoc_md1057", null ],
        [ "2. <tt>module.json</tt>", "md_docs_2convention_2module__structure.html#autotoc_md1058", null ],
        [ "3. <tt>src/{module_name}.c</tt> (განახლებული შაბლონი)", "md_docs_2convention_2module__structure.html#autotoc_md1059", null ]
      ] ],
      [ "<tt>CMakeLists.txt</tt> სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1060", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2module__structure.html#autotoc_md1061", null ]
    ] ],
    [ "ცვლადების დასახელების წესები", "md_docs_2convention_2variable__naming.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2variable__naming.html#autotoc_md1064", [
        [ "1. დესკრიპტიული სახელები", "md_docs_2convention_2variable__naming.html#autotoc_md1065", null ],
        [ "2. აკრძალული პრეფიქსები", "md_docs_2convention_2variable__naming.html#autotoc_md1066", null ],
        [ "3. სწორი მიდგომა", "md_docs_2convention_2variable__naming.html#autotoc_md1067", [
          [ "Service Locator", "md_docs_2convention_2variable__naming.html#autotoc_md1068", null ],
          [ "Event Bus", "md_docs_2convention_2variable__naming.html#autotoc_md1069", null ],
          [ "Module Registry", "md_docs_2convention_2variable__naming.html#autotoc_md1070", null ],
          [ "Display Modules", "md_docs_2convention_2variable__naming.html#autotoc_md1071", null ]
        ] ],
        [ "4. კონვენციები", "md_docs_2convention_2variable__naming.html#autotoc_md1072", [
          [ "Global ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1073", null ],
          [ "Static ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1074", null ],
          [ "Mutex/Semaphore ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1075", null ],
          [ "Queue ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1076", null ],
          [ "API Structure ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1077", null ]
        ] ]
      ] ],
      [ "მაგალითები", "md_docs_2convention_2variable__naming.html#autotoc_md1078", [
        [ "Config Manager", "md_docs_2convention_2variable__naming.html#autotoc_md1079", null ],
        [ "I2C Bus Module", "md_docs_2convention_2variable__naming.html#autotoc_md1080", null ],
        [ "MQTT Module", "md_docs_2convention_2variable__naming.html#autotoc_md1081", null ]
      ] ],
      [ "Private Data Pointer Naming", "md_docs_2convention_2variable__naming.html#autotoc_md1082", [
        [ "❌ არასწორი", "md_docs_2convention_2variable__naming.html#autotoc_md1083", null ],
        [ "✅ სწორი", "md_docs_2convention_2variable__naming.html#autotoc_md1084", null ]
      ] ],
      [ "შეჯამება", "md_docs_2convention_2variable__naming.html#autotoc_md1085", null ]
    ] ],
    [ "🚀 CREATE_MODULE.PY - ახალი მოდულის ავტომატური გენერატორი", "md_docs_2CREATE__MODULE__DOCUMENTATION.html", [
      [ "📋 რა არის create_module.py?", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1087", null ],
      [ "🎯 რას აკეთებს?", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1088", [
        [ "✅ შექმნილი ფაილები (5 ფაილი):", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1089", null ],
        [ "✅ ავტომატური ინტეგრაცია:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1090", null ]
      ] ],
      [ "🛠️ გამოყენება", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1091", [
        [ "1. ძირითადი სინტაქსი:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1092", null ],
        [ "2. მაგალითები:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1093", [
          [ "📡 WiFi მოდული:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1094", null ],
          [ "🌡️ ტემპერატურის სენსორი:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1095", null ],
          [ "📺 დისპლეის დრაივერი:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1096", null ],
          [ "💾 მეხსიერების მენეჯერი:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1097", null ]
        ] ],
        [ "3. პარამეტრები:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1098", null ],
        [ "4. ხელმისაწვდომი ტიპები:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1099", null ]
      ] ],
      [ "🏗️ არქიტექტურა და ლოგიკა", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1100", [
        [ "1. 🧠 კონდიციური კომპილაცია (SMART SYSTEM):", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1101", null ],
        [ "2. 📁 შექმნილი სტრუქტურა:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1102", null ],
        [ "3. 🔗 Base Module Interface:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1103", null ]
      ] ],
      [ "🔄 სრული Workflow", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1104", [
        [ "1. მოდულის შექმნა:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1105", null ],
        [ "2. სისტემის reconfigure:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1106", null ],
        [ "3. კონფიგურაცია:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1107", null ],
        [ "4. Build:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1108", null ]
      ] ],
      [ "🎛️ კონფიგურაციის პარამეტრები", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1109", null ],
      [ "🔍 ხელით მორგება", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1110", [
        [ "1. კონფიგურაციის გაფართოება:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1111", null ],
        [ "2. Private დატის გაფართოება:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1112", null ],
        [ "3. ივენთების დამუშავება:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1113", null ]
      ] ],
      [ "🚨 მნიშვნელოვანი შენიშვნები", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1114", [
        [ "✅ რაც უნდა გახსოვდეთ:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1115", null ],
        [ "❌ ჩვეული შეცდომები:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1116", null ]
      ] ],
      [ "🧪 ტესტირების მეთოდები", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1117", [
        [ "1. რომ დარწმუნდეთ მოდული სწორად იქმნება:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1118", null ],
        [ "2. რომ დარწმუნდეთ სისტემა აღმოაჩენს:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1119", null ],
        [ "3. რომ დარწმუნდეთ კონფიგურაცია მუშაობს:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1120", null ],
        [ "4. რომ დარწმუნდეთ build მუშაობს:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1121", null ]
      ] ],
      [ "📊 განსხვავება სხვა სისტემებისგან", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1122", null ],
      [ "🎓 Best Practices", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1123", [
        [ "1. სახელების კონვენცია:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1124", null ],
        [ "2. განვითარების workflow:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1125", null ],
        [ "3. კოდის ხარისხი:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1126", null ]
      ] ],
      [ "🎯 შეჯამება", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1128", null ]
    ] ],
    [ "Synapse ESP Framework — არქიტექტურის მიმოხილვა", "md_docs_2design__decisions_2architecture__overview.html", [
      [ "მიზანი", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1130", null ],
      [ "ძირითადი არქიტექტურული პრინციპები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1131", null ],
      [ "არქიტექტურული ფენები და ძირითადი კომპონენტები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1132", null ],
      [ "მოდულებს შორის კომუნიკაციის პატერნები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1133", null ],
      [ "სიცოცხლის ციკლი", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1134", null ],
      [ "საქაღალდეებისა და ფაილების სტრუქტურა", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1135", null ],
      [ "არქიტექტურული დიაგრამა (მოკლე)", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1136", null ],
      [ "სტანდარტები და დოკუმენტაცია", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1137", null ],
      [ "დასკვნა", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1138", null ]
    ] ],
    [ "კომუნიკაციის არქიტექტურული არჩევანი (Communication Choices)", "md_docs_2design__decisions_2communication__choices.html", [
      [ "მიზანი", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1141", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1143", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1145", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1147", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1149", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1151", [
        [ "Service Locator", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1152", null ],
        [ "Event Bus", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1153", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1155", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1157", null ],
      [ "Event Bus Pattern", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1159", [
        [ "არქიტექტურული ცვლილება: Enum-დან სტრიქონებზე გადასვლა", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1160", null ]
      ] ]
    ] ],
    [ "Core კომპონენტები (Core Components)", "md_docs_2design__decisions_2core__components.html", [
      [ "მიზანი", "md_docs_2design__decisions_2core__components.html#autotoc_md1164", null ],
      [ "ძირითადი კომპონენტები", "md_docs_2design__decisions_2core__components.html#autotoc_md1166", [
        [ "1. Service Locator", "md_docs_2design__decisions_2core__components.html#autotoc_md1167", null ],
        [ "2. Event Bus", "md_docs_2design__decisions_2core__components.html#autotoc_md1168", null ],
        [ "3. Module Register", "md_docs_2design__decisions_2core__components.html#autotoc_md1169", null ],
        [ "4. Module Factory", "md_docs_2design__decisions_2core__components.html#autotoc_md1170", null ],
        [ "5. Configuration Manager", "md_docs_2design__decisions_2core__components.html#autotoc_md1171", null ],
        [ "6. Logging", "md_docs_2design__decisions_2core__components.html#autotoc_md1172", null ]
      ] ],
      [ "ურთიერთქმედება მოდულებთან", "md_docs_2design__decisions_2core__components.html#autotoc_md1174", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2core__components.html#autotoc_md1176", null ]
    ] ],
    [ "არქიტექტურული გადაწყვეტილების ჩანაწერის შაბლონი (Decision Record Template)", "md_docs_2design__decisions_2decision__record__template.html", [
      [ "გადაწყვეტილების ID", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1180", null ],
      [ "თარიღი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1181", null ],
      [ "სტატუსი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1182", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1183", null ],
      [ "გადაწყვეტილება", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1184", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1185", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1186", null ],
      [ "განხორციელების ნაბიჯები", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1187", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1188", null ]
    ] ],
    [ "Synapse Design Decisions", "md_docs_2design__decisions_2design__decisions__index.html", [
      [ "ძირითადი დოკუმენტები", "md_docs_2design__decisions_2design__decisions__index.html#autotoc_md1192", null ]
    ] ],
    [ "შეცდომების დამუშავების არქიტექტურული გადაწყვეტილებები (Error Handling Decisions)", "md_docs_2design__decisions_2error__handling__decisions.html", [
      [ "მიზანი", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1196", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1198", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1200", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1202", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1204", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1206", [
        [ "პარამეტრების ვალიდაცია", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1207", null ],
        [ "რესურსების გამოყოფის შემოწმება", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1208", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1210", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1212", null ]
    ] ],
    [ "გაფართოებადობისა და მოდულარობის არქიტექტურული გადაწყვეტილებები (Extensibility & Modularity)", "md_docs_2design__decisions_2extensibility__and__modularity.html", [
      [ "მიზანი", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1216", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1218", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1220", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1222", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1224", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1226", [
        [ "ახალი მოდულის დამატება", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1227", null ],
        [ "Public API-ის გამოყენება", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1228", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1230", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1232", null ]
    ] ],
    [ "მიგრაციისა და თავსებადობის არქიტექტურული გადაწყვეტილებები (Migration & Compatibility)", "md_docs_2design__decisions_2migration__and__compatibility.html", [
      [ "მიზანი", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1236", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1238", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1240", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1242", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1244", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1246", [
        [ "სქემის ვერსიონირება", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1247", null ],
        [ "Deprecated API-ის მონიშვნა", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1248", null ],
        [ "მიგრაციის სკრიპტის გამოყენება", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1249", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1251", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1253", null ]
    ] ],
    [ "წარმადობისა და რესურსების არქიტექტურული კომპრომისები (Performance Trade-offs)", "md_docs_2design__decisions_2performance__tradeoffs.html", [
      [ "მიზანი", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1257", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1259", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1261", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1263", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1265", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1267", [
        [ "მოდულის გამორთვა ბილდიდან", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1268", null ],
        [ "Static allocation", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1269", null ],
        [ "Lazy initialization", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1270", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1272", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1274", null ]
    ] ],
    [ "უსაფრთხოების არქიტექტურული გადაწყვეტილებები (Security Considerations)", "md_docs_2design__decisions_2security__considerations.html", [
      [ "მიზანი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1278", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1280", null ],
      [ "არჩეული გადაწყვეტილებები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1282", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1284", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1286", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1288", [
        [ "TLS/SSL ჩართვა MQTT-ში", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1289", null ],
        [ "OTA განახლების სიგნირება", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1290", null ],
        [ "ლოგირების კონტროლი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1291", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1293", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1295", null ]
    ] ],
    [ "მონაცემთა შენახვის არქიტექტურული არჩევანი (Storage Strategy)", "md_docs_2design__decisions_2storage__strategy.html", [
      [ "მიზანი", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1299", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1301", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1303", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1305", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1307", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1309", [
        [ "NVS-ში სტრიქონის შენახვა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1310", null ],
        [ "NVS-დან სტრიქონის წაკითხვა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1311", null ],
        [ "Flash-ში მონაცემების შენახვა (OTA)", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1312", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1314", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1316", null ]
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
      [ "მიზანი", "md_docs_2glossary_2glossary__api.html#autotoc_md1320", null ]
    ] ],
    [ "არქიტექტურული ტერმინები (Glossary — Architecture)", "md_docs_2glossary_2glossary__architecture.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__architecture.html#autotoc_md1325", null ]
    ] ],
    [ "კონფიგურაციისა და შენახვის ტერმინები (Glossary — Configuration & Storage)", "md_docs_2glossary_2glossary__config.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__config.html#autotoc_md1330", null ]
    ] ],
    [ "Glossary (ტერმინები)", "md_docs_2glossary_2glossary__index.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__index.html#autotoc_md1335", null ],
      [ "თემატური ტერმინების დოკუმენტები", "md_docs_2glossary_2glossary__index.html#autotoc_md1337", null ],
      [ "გამოყენების წესი", "md_docs_2glossary_2glossary__index.html#autotoc_md1339", null ]
    ] ],
    [ "უსაფრთხოების და წარმადობის ტერმინები (Glossary — Security & Performance)", "md_docs_2glossary_2glossary__security.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__security.html#autotoc_md1343", null ]
    ] ],
    [ "Cloud ინტეგრაციის სახელმძღვანელო (Integration — Cloud)", "md_docs_2integration_2integration__cloud.html", [
      [ "მიზანი", "md_docs_2integration_2integration__cloud.html#autotoc_md1348", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__cloud.html#autotoc_md1350", null ],
      [ "Best Practices", "md_docs_2integration_2integration__cloud.html#autotoc_md1352", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__cloud.html#autotoc_md1354", null ]
    ] ],
    [ "Integration Index (ინტეგრაციის სახელმძღვანელოების ინდექსი)", "md_docs_2integration_2integration__index.html", [
      [ "მიზანი", "md_docs_2integration_2integration__index.html#autotoc_md1358", null ],
      [ "თემატური ინტეგრაციის სახელმძღვანელოები", "md_docs_2integration_2integration__index.html#autotoc_md1360", null ],
      [ "გამოყენების წესი", "md_docs_2integration_2integration__index.html#autotoc_md1362", null ]
    ] ],
    [ "MQTT ინტეგრაციის სახელმძღვანელო (Integration — MQTT)", "md_docs_2integration_2integration__mqtt.html", [
      [ "მიზანი", "md_docs_2integration_2integration__mqtt.html#autotoc_md1366", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__mqtt.html#autotoc_md1368", null ],
      [ "Best Practices", "md_docs_2integration_2integration__mqtt.html#autotoc_md1370", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__mqtt.html#autotoc_md1372", null ]
    ] ],
    [ "REST API ინტეგრაციის სახელმძღვანელო (Integration — REST API)", "md_docs_2integration_2integration__rest.html", [
      [ "მიზანი", "md_docs_2integration_2integration__rest.html#autotoc_md1376", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__rest.html#autotoc_md1378", null ],
      [ "Best Practices", "md_docs_2integration_2integration__rest.html#autotoc_md1380", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__rest.html#autotoc_md1382", null ]
    ] ],
    [ "WebSocket ინტეგრაციის სახელმძღვანელო (Integration — WebSocket)", "md_docs_2integration_2integration__websocket.html", [
      [ "მიზანი", "md_docs_2integration_2integration__websocket.html#autotoc_md1386", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__websocket.html#autotoc_md1388", null ],
      [ "Best Practices", "md_docs_2integration_2integration__websocket.html#autotoc_md1390", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__websocket.html#autotoc_md1392", null ]
    ] ],
    [ "✅ Synapse Framework — Documentation & Module Design Checklist", "md_docs_2internal_2synapse__checklist.html", [
      [ "1. API Naming & Structure", "md_docs_2internal_2synapse__checklist.html#autotoc_md1395", null ],
      [ "2. Module Implementation", "md_docs_2internal_2synapse__checklist.html#autotoc_md1396", null ],
      [ "3. Communication Integration (MQTT/WiFi/ESP-NOW)", "md_docs_2internal_2synapse__checklist.html#autotoc_md1397", null ],
      [ "4. Core Interaction", "md_docs_2internal_2synapse__checklist.html#autotoc_md1398", null ],
      [ "5. Documentation", "md_docs_2internal_2synapse__checklist.html#autotoc_md1399", null ],
      [ "6. Quality & Security", "md_docs_2internal_2synapse__checklist.html#autotoc_md1400", null ],
      [ "7. Testability", "md_docs_2internal_2synapse__checklist.html#autotoc_md1401", null ]
    ] ],
    [ "ბენჩმარკების ჩატარების სახელმძღვანელო (Performance Benchmarks Guide)", "md_docs_2performance_2performance__benchmarks.html", [
      [ "მიზანი", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1404", null ],
      [ "ბენჩმარკის ჩატარების ძირითადი ნაბიჯები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1406", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1408", [
        [ "სენსორის კითხვა", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1409", null ],
        [ "MQTT publish latency", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1410", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1412", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1414", null ]
    ] ],
    [ "საუკეთესო პრაქტიკები წარმადობისთვის (Performance Best Practices)", "md_docs_2performance_2performance__best__practices.html", [
      [ "მიზანი", "md_docs_2performance_2performance__best__practices.html#autotoc_md1418", null ],
      [ "ძირითადი რეკომენდაციები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1420", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1422", null ],
      [ "Best Practices", "md_docs_2performance_2performance__best__practices.html#autotoc_md1424", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1426", null ]
    ] ],
    [ "ეკრანის მოდულების ოპტიმიზაცია (Performance — Display Modules)", "md_docs_2performance_2performance__display.html", [
      [ "მიზანი", "md_docs_2performance_2performance__display.html#autotoc_md1430", null ],
      [ "ოპტიმიზაციის რეკომენდაციები", "md_docs_2performance_2performance__display.html#autotoc_md1432", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__display.html#autotoc_md1434", [
        [ "Refresh interval-ის ოპტიმიზაცია", "md_docs_2performance_2performance__display.html#autotoc_md1435", null ],
        [ "Conditional redraw", "md_docs_2performance_2performance__display.html#autotoc_md1436", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__display.html#autotoc_md1438", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__display.html#autotoc_md1440", null ]
    ] ],
    [ "Performance (წარმადობის სახელმძღვანელო)", "md_docs_2performance_2performance__index.html", [
      [ "მიზანი", "md_docs_2performance_2performance__index.html#autotoc_md1444", null ],
      [ "თემატური performance სახელმძღვანელოები", "md_docs_2performance_2performance__index.html#autotoc_md1446", null ],
      [ "გამოყენების წესი", "md_docs_2performance_2performance__index.html#autotoc_md1448", null ]
    ] ],
    [ "ოპტიმიზაციის სახელმძღვანელოები (Performance Optimization Guide)", "md_docs_2performance_2performance__optimization.html", [
      [ "მიზანი", "md_docs_2performance_2performance__optimization.html#autotoc_md1452", null ],
      [ "კოდის ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1454", null ],
      [ "კონფიგურაციის ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1456", null ],
      [ "მოდულების ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1458", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__optimization.html#autotoc_md1460", [
        [ "Static allocation", "md_docs_2performance_2performance__optimization.html#autotoc_md1461", null ],
        [ "Lazy initialization", "md_docs_2performance_2performance__optimization.html#autotoc_md1462", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__optimization.html#autotoc_md1464", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__optimization.html#autotoc_md1466", null ]
    ] ],
    [ "რესურსების გამოყენების სახელმძღვანელო (Performance Resource Usage Guide)", "md_docs_2performance_2performance__resource__usage.html", [
      [ "მიზანი", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1470", null ],
      [ "RAM-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1472", null ],
      [ "Flash-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1473", null ],
      [ "CPU-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1474", null ],
      [ "Stack-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1475", null ],
      [ "მონიტორინგის მაგალითები", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1477", [
        [ "RAM", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1478", null ],
        [ "Stack", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1479", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1481", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1483", null ]
    ] ],
    [ "🚀 <tt>create_module.py</tt>: Synapse მოდულის გენერატორი (v3.0)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html", [
      [ "1. 📋 რა არის <tt>create_module.py</tt>?", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1486", null ],
      [ "2. 🎯 რას აკეთებს?", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1487", [
        [ "✅ გენერირებული ფაილები (7 ფაილი)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1488", null ],
        [ "✅ ავტომატური ინტეგრაცია", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1489", null ]
      ] ],
      [ "3. 🛠️ გამოყენების რეჟიმები", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1490", [
        [ "3.1. ინტერაქტიული რეჟიმი (რეკომენდებული)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1491", null ],
        [ "3.2. ბრძანების ხაზის რეჟიმი (ავტომატიზაციისთვის)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1492", null ]
      ] ],
      [ "4. 🏛️ არქეტიპები (Archetypes)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1493", null ],
      [ "5. 🔄 სრული სამუშაო პროცესი (Workflow)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1494", null ],
      [ "6. 🚨 მნიშვნელოვანი შენიშვნები", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1495", null ],
      [ "🎯 შეჯამება", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1497", null ]
    ] ],
    [ "უსაფრთხოების საუკეთესო პრაქტიკები (Security Best Practices)", "md_docs_2security_2security__best__practices.html", [
      [ "მიზანი", "md_docs_2security_2security__best__practices.html#autotoc_md1500", null ],
      [ "ძირითადი რეკომენდაციები", "md_docs_2security_2security__best__practices.html#autotoc_md1502", null ],
      [ "მაგალითები", "md_docs_2security_2security__best__practices.html#autotoc_md1504", [
        [ "TLS/SSL ჩართვა MQTT-ში", "md_docs_2security_2security__best__practices.html#autotoc_md1505", null ],
        [ "OTA განახლების სიგნირება", "md_docs_2security_2security__best__practices.html#autotoc_md1506", null ],
        [ "ლოგირების კონტროლი", "md_docs_2security_2security__best__practices.html#autotoc_md1507", null ]
      ] ],
      [ "Best Practices", "md_docs_2security_2security__best__practices.html#autotoc_md1509", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__best__practices.html#autotoc_md1511", null ]
    ] ],
    [ "უსაფრთხოების შემოწმების სია (Security Checklist)", "md_docs_2security_2security__checklist.html", [
      [ "მიზანი", "md_docs_2security_2security__checklist.html#autotoc_md1515", null ],
      [ "Release Checklist", "md_docs_2security_2security__checklist.html#autotoc_md1517", null ],
      [ "Code Review Checklist", "md_docs_2security_2security__checklist.html#autotoc_md1518", null ],
      [ "Best Practices", "md_docs_2security_2security__checklist.html#autotoc_md1520", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__checklist.html#autotoc_md1522", null ]
    ] ],
    [ "უსაფრთხოების სახელმძღვანელოები (Security Guidelines)", "md_docs_2security_2security__guidelines.html", [
      [ "მიზანი", "md_docs_2security_2security__guidelines.html#autotoc_md1526", null ],
      [ "TLS/SSL", "md_docs_2security_2security__guidelines.html#autotoc_md1528", null ],
      [ "Authentication & Authorization", "md_docs_2security_2security__guidelines.html#autotoc_md1529", null ],
      [ "Input Validation", "md_docs_2security_2security__guidelines.html#autotoc_md1530", null ],
      [ "Credentials Management", "md_docs_2security_2security__guidelines.html#autotoc_md1531", null ],
      [ "OTA და Firmware Security", "md_docs_2security_2security__guidelines.html#autotoc_md1532", null ],
      [ "Logging", "md_docs_2security_2security__guidelines.html#autotoc_md1533", null ],
      [ "Best Practices", "md_docs_2security_2security__guidelines.html#autotoc_md1535", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__guidelines.html#autotoc_md1537", null ]
    ] ],
    [ "Security Index (უსაფრთხოების სახელმძღვანელოების ინდექსი)", "md_docs_2security_2security__index.html", [
      [ "მიზანი", "md_docs_2security_2security__index.html#autotoc_md1541", null ],
      [ "თემატური უსაფრთხოების სახელმძღვანელოები", "md_docs_2security_2security__index.html#autotoc_md1543", null ],
      [ "გამოყენების წესი", "md_docs_2security_2security__index.html#autotoc_md1545", null ]
    ] ],
    [ "Vulnerability Disclosure Policy (უსაფრთხოების ხარვეზების აღმოჩენისა და შეტყობინების პროცედურა)", "md_docs_2security_2security__vulnerability__disclosure.html", [
      [ "მიზანი", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1549", null ],
      [ "ხარვეზის აღმოჩენა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1551", null ],
      [ "შეტყობინების პროცედურა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1552", null ],
      [ "რეაგირება", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1553", null ],
      [ "პასუხისმგებლობა და ეთიკა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1554", null ],
      [ "საკონტაქტო ინფორმაცია", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1556", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1558", null ]
    ] ],
    [ "კომუნიკაციის პატერნები (Communication Patterns)", "md_docs_2structure_2communication__patterns.html", [
      [ "მიზანი", "md_docs_2structure_2communication__patterns.html#autotoc_md1561", null ],
      [ "ძირითადი პატერნები", "md_docs_2structure_2communication__patterns.html#autotoc_md1562", [
        [ "1. Service Locator Pattern", "md_docs_2structure_2communication__patterns.html#autotoc_md1563", null ],
        [ "2. Event Bus Pattern", "md_docs_2structure_2communication__patterns.html#autotoc_md1564", null ]
      ] ],
      [ "სწორი გამოყენების წესები", "md_docs_2structure_2communication__patterns.html#autotoc_md1565", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2structure_2communication__patterns.html#autotoc_md1566", null ],
      [ "მაგალითები", "md_docs_2structure_2communication__patterns.html#autotoc_md1567", [
        [ "Service Locator", "md_docs_2structure_2communication__patterns.html#autotoc_md1568", null ],
        [ "Event Bus", "md_docs_2structure_2communication__patterns.html#autotoc_md1569", null ]
      ] ]
    ] ],
    [ "კონფიგურაციის მართვის პროცესი (Configuration Flow)", "md_docs_2structure_2configuration__flow.html", [
      [ "მიზანი", "md_docs_2structure_2configuration__flow.html#autotoc_md1572", null ],
      [ "კონფიგურაციის წყაროები", "md_docs_2structure_2configuration__flow.html#autotoc_md1573", null ],
      [ "კონფიგურაციის სტრუქტურა", "md_docs_2structure_2configuration__flow.html#autotoc_md1574", null ],
      [ "კონფიგურაციის დამუშავების ეტაპები", "md_docs_2structure_2configuration__flow.html#autotoc_md1575", null ],
      [ "კონფიგურაციის წვდომის მაგალითები", "md_docs_2structure_2configuration__flow.html#autotoc_md1576", null ],
      [ "ვალიდაცია და Default მნიშვნელობები", "md_docs_2structure_2configuration__flow.html#autotoc_md1577", null ],
      [ "Runtime Reconfiguration", "md_docs_2structure_2configuration__flow.html#autotoc_md1578", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2configuration__flow.html#autotoc_md1579", null ]
    ] ],
    [ "Core კომპონენტები (Core Components)", "md_docs_2structure_2core__components.html", [
      [ "დანიშნულება", "md_docs_2structure_2core__components.html#autotoc_md1582", null ],
      [ "ძირითადი კომპონენტები", "md_docs_2structure_2core__components.html#autotoc_md1583", [
        [ "1. Service Locator", "md_docs_2structure_2core__components.html#autotoc_md1584", null ],
        [ "2. Event Bus", "md_docs_2structure_2core__components.html#autotoc_md1585", null ],
        [ "3. Module Register", "md_docs_2structure_2core__components.html#autotoc_md1586", null ],
        [ "4. Module Factory (ყოფილი Fabrique)", "md_docs_2structure_2core__components.html#autotoc_md1587", null ],
        [ "5. Configuration Manager", "md_docs_2structure_2core__components.html#autotoc_md1588", null ],
        [ "6. Logging", "md_docs_2structure_2core__components.html#autotoc_md1589", null ]
      ] ],
      [ "ურთიერთქმედება მოდულებთან", "md_docs_2structure_2core__components.html#autotoc_md1590", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2core__components.html#autotoc_md1591", null ]
    ] ],
    [ "საქაღალდეებისა და ფაილების სტრუქტურა (Directory Structure)", "md_docs_2structure_2directory__structure.html", [
      [ "ძირითადი სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1594", null ],
      [ "ძირითადი საქაღალდეების აღწერა", "md_docs_2structure_2directory__structure.html#autotoc_md1595", null ],
      [ "<tt>modules</tt> საქაღალდის სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1596", null ],
      [ "თითოეული მოდულის სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1597", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2directory__structure.html#autotoc_md1598", null ]
    ] ],
    [ "შეცდომების დამუშავება და ლოგირების სტანდარტები (Error Handling & Logging)", "md_docs_2structure_2error__handling__and__logging.html", [
      [ "მიზანი", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1601", null ],
      [ "შეცდომების დამუშავების პრინციპები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1602", null ],
      [ "შეცდომების კოდები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1603", null ],
      [ "შეცდომების დამუშავების მაგალითები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1604", null ],
      [ "ლოგირების სტანდარტები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1605", null ],
      [ "ლოგირების მაგალითები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1606", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1607", null ]
    ] ],
    [ "ფრეიმვორქის გაფართოება და Best Practices (Extending the Framework)", "md_docs_2structure_2extending__framework.html", [
      [ "მიზანი", "md_docs_2structure_2extending__framework.html#autotoc_md1610", null ],
      [ "ახალი მოდულის დამატების პროცესი", "md_docs_2structure_2extending__framework.html#autotoc_md1611", null ],
      [ "Best Practices", "md_docs_2structure_2extending__framework.html#autotoc_md1612", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2structure_2extending__framework.html#autotoc_md1613", null ],
      [ "მაგალითი ახალი მოდულის დამატების", "md_docs_2structure_2extending__framework.html#autotoc_md1614", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (FAQ)", "md_docs_2structure_2faq.html", [
      [ "1. როგორ დავამატო ახალი მოდული?", "md_docs_2structure_2faq.html#autotoc_md1617", null ],
      [ "2. შეიძლება თუ არა ერთი და იმავე ტიპის რამდენიმე მოდულის დამატება?", "md_docs_2structure_2faq.html#autotoc_md1618", null ],
      [ "3. როგორ ხდება მოდულებს შორის კომუნიკაცია?", "md_docs_2structure_2faq.html#autotoc_md1619", null ],
      [ "4. როგორ მივიღო ან შევცვალო მოდულის კონფიგურაცია?", "md_docs_2structure_2faq.html#autotoc_md1620", null ],
      [ "5. როგორ დავამატო ახალი გლობალური პარამეტრი?", "md_docs_2structure_2faq.html#autotoc_md1621", null ],
      [ "6. როგორ დავიცვა naming და structure სტანდარტები?", "md_docs_2structure_2faq.html#autotoc_md1622", null ],
      [ "7. როგორ ხდება შეცდომების დამუშავება და ლოგირება?", "md_docs_2structure_2faq.html#autotoc_md1623", null ],
      [ "8. როგორ დავამატო ახალი კატეგორია მოდულებისთვის?", "md_docs_2structure_2faq.html#autotoc_md1624", null ],
      [ "9. როგორ დავამატო დოკუმენტაცია ან განვაახლო არსებული?", "md_docs_2structure_2faq.html#autotoc_md1625", null ],
      [ "10. როგორ დავიწყო პროექტის გამოყენება?", "md_docs_2structure_2faq.html#autotoc_md1626", null ]
    ] ],
    [ "მოდულის სიცოცხლის ციკლი და ინიციალიზაცია (Lifecycle & Initialization)", "md_docs_2structure_2lifecycle__and__init.html", [
      [ "მიზანი", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1629", null ],
      [ "სიცოცხლის ციკლის ეტაპები", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1630", null ],
      [ "დეტალური აღწერა", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1631", [
        [ "1. Create", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1632", null ],
        [ "2. Init", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1633", null ],
        [ "3. Enable/Disable", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1634", null ],
        [ "4. Deinit", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1635", null ]
      ] ],
      [ "სიცოცხლის ციკლის დიაგრამა", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1636", null ],
      [ "მაგალითი მოდულის სიცოცხლის ციკლის მართვის", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1637", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1638", null ]
    ] ],
    [ "მოდულების არქიტექტურა (Module Architecture)", "md_docs_2structure_2module__architecture.html", [
      [ "ძირითადი პრინციპები", "md_docs_2structure_2module__architecture.html#autotoc_md1641", null ],
      [ "მოდულის ტიპები", "md_docs_2structure_2module__architecture.html#autotoc_md1642", null ],
      [ "მოდულების ურთიერთქმედება", "md_docs_2structure_2module__architecture.html#autotoc_md1643", null ],
      [ "მოდულის სიცოცხლის ციკლი (Lifecycle)", "md_docs_2structure_2module__architecture.html#autotoc_md1644", null ],
      [ "მოდულის სტრუქტურის მაგალითი", "md_docs_2structure_2module__architecture.html#autotoc_md1645", [
        [ "<tt>_create</tt> ფუნქციის კონტრაქტი", "md_docs_2structure_2module__architecture.html#autotoc_md1646", null ]
      ] ],
      [ "მოდულის გაფართოება/დამატება", "md_docs_2structure_2module__architecture.html#autotoc_md1647", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2module__architecture.html#autotoc_md1648", null ]
    ] ],
    [ "Advanced Diagnostics (გაფართოებული დიაგნოსტიკა)", "md_docs_2troubleshooting_2advanced__diagnostics.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1659", null ],
      [ "1. Stack Overflow და Task Watchdog", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1661", null ],
      [ "2. Heap Corruption და Memory Leak", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1662", null ],
      [ "3. Core Dumps და Crash Reporting", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1663", null ],
      [ "4. Custom Health Checks", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1664", null ],
      [ "5. Peripheral Diagnostics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1665", null ],
      [ "6. Network Diagnostics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1666", null ],
      [ "7. Performance Metrics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1667", null ]
    ] ],
    [ "გავრცელებული პრობლემები და გადაჭრის გზები (Common Issues & Solutions)", "md_docs_2troubleshooting_2common__issues.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1670", null ],
      [ "1. ბილდის შეცდომები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1672", [
        [ "პრობლემა: მოდული არ იბილდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1673", null ],
        [ "პრობლემა: include file not found", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1674", null ]
      ] ],
      [ "2. კონფიგურაციის პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1676", [
        [ "პრობლემა: მოდული არ ინიციალიზდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1677", null ],
        [ "პრობლემა: default მნიშვნელობები არ მუშაობს", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1678", null ]
      ] ],
      [ "3. კომუნიკაციის პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1680", [
        [ "პრობლემა: Event Bus-ზე მოვლენები არ ვრცელდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1681", null ],
        [ "პრობლემა: Service Locator-ით API არ იძებნება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1682", null ]
      ] ],
      [ "4. ლოგირების და შეცდომების დიაგნოსტიკა", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1684", [
        [ "პრობლემა: ლოგები არ ჩანს", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1685", null ],
        [ "პრობლემა: შეცდომის კოდი არ იკითხება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1686", null ]
      ] ],
      [ "5. Runtime პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1688", [
        [ "პრობლემა: მოდული არ რეაგირებს enable/disable-ზე", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1689", null ],
        [ "პრობლემა: რესურსების გაჟონვა (memory leak)", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1690", null ]
      ] ],
      [ "6. დამატებითი რჩევები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1692", null ]
    ] ],
    [ "Debugging Guide (დიბაგინგის სახელმძღვანელო)", "md_docs_2troubleshooting_2debugging__guide.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1695", null ],
      [ "1. ლოგირების ჩართვა და გამოყენება", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1697", null ],
      [ "2. Debug Build-ის კონფიგურაცია", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1698", null ],
      [ "3. GDB და Breakpoint-ების გამოყენება", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1699", null ],
      [ "4. Task და Stack Usage მონიტორინგი", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1700", null ],
      [ "5. Heap და Memory Usage", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1701", null ],
      [ "6. Event და Service Debugging", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1702", null ],
      [ "7. Assertion-ები და Error Checking", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1703", null ],
      [ "8. დამატებითი ინსტრუმენტები", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1704", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (Troubleshooting FAQ)", "md_docs_2troubleshooting_2faq.html", [
      [ "1. რატომ არ იბილდება ჩემი მოდული?", "md_docs_2troubleshooting_2faq.html#autotoc_md1707", null ],
      [ "2. რატომ არ ჩანს ლოგები?", "md_docs_2troubleshooting_2faq.html#autotoc_md1708", null ],
      [ "3. რატომ ვერ პოულობს Service Locator-ი ჩემს სერვისს?", "md_docs_2troubleshooting_2faq.html#autotoc_md1709", null ],
      [ "4. რატომ არ ვრცელდება Event Bus-ზე მოვლენები?", "md_docs_2troubleshooting_2faq.html#autotoc_md1710", null ],
      [ "5. რატომ არ ინიციალიზდება მოდული?", "md_docs_2troubleshooting_2faq.html#autotoc_md1711", null ],
      [ "6. როგორ გავიგო რომელი მოდული იწვევს პრობლემას?", "md_docs_2troubleshooting_2faq.html#autotoc_md1712", null ],
      [ "7. როგორ გამოვასწორო რესურსების გაჟონვა (memory leak)?", "md_docs_2troubleshooting_2faq.html#autotoc_md1713", null ],
      [ "8. როგორ დავადგინო კონფიგურაციის შეცდომა?", "md_docs_2troubleshooting_2faq.html#autotoc_md1714", null ],
      [ "9. როგორ დავამატო ახალი კითხვა ამ FAQ-ში?", "md_docs_2troubleshooting_2faq.html#autotoc_md1715", null ]
    ] ],
    [ "Advanced Usage (გაფართოებული გამოყენება)", "md_docs_2tutorials_2advanced__usage.html", [
      [ "მიზანი", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1718", null ],
      [ "1. მრავალმოდულიანი კონფიგურაცია", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1719", null ],
      [ "2. მოდულის დინამიური ჩართვა/გამორთვა", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1720", null ],
      [ "3. Runtime Reconfiguration", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1721", null ],
      [ "4. Custom Event-ების გამოყენება", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1722", null ],
      [ "5. მოდულის სტატუსის მონიტორინგი", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1723", null ],
      [ "6. ოპტიმიზაცია და რესურსების მართვა", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1724", null ],
      [ "7. Best Practices", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1725", null ]
    ] ],
    [ "პირველი საკუთარი მოდულის შექმნა (Create Your First Module)", "md_docs_2tutorials_2create__first__module.html", [
      [ "მიზანი", "md_docs_2tutorials_2create__first__module.html#autotoc_md1728", null ],
      [ "1. საჭიროების განსაზღვრა", "md_docs_2tutorials_2create__first__module.html#autotoc_md1729", null ],
      [ "2. მოდულის ჩონჩხის გენერაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1730", null ],
      [ "3. კონფიგურაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1731", null ],
      [ "4. კოდის იმპლემენტაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1732", null ],
      [ "5. ტესტირება", "md_docs_2tutorials_2create__first__module.html#autotoc_md1733", null ]
    ] ],
    [ "სწრაფი სტარტი (Getting Started)", "md_docs_2tutorials_2getting__started.html", [
      [ "მიზანი", "md_docs_2tutorials_2getting__started.html#autotoc_md1736", null ],
      [ "1. გარემოს მომზადება", "md_docs_2tutorials_2getting__started.html#autotoc_md1737", null ],
      [ "2. პროექტის სტრუქტურის გაცნობა", "md_docs_2tutorials_2getting__started.html#autotoc_md1738", null ],
      [ "3. პირველი მოდულის ჩართვა", "md_docs_2tutorials_2getting__started.html#autotoc_md1739", null ],
      [ "4. ბილდი და ფლეში", "md_docs_2tutorials_2getting__started.html#autotoc_md1740", null ],
      [ "5. ლოგების ნახვა და ტესტირება", "md_docs_2tutorials_2getting__started.html#autotoc_md1741", null ],
      [ "6. დამატებითი რესურსები", "md_docs_2tutorials_2getting__started.html#autotoc_md1742", null ]
    ] ],
    [ "ინტეგრაციის მაგალითი (Integration Example)", "md_docs_2tutorials_2integration__example.html", [
      [ "მიზანი", "md_docs_2tutorials_2integration__example.html#autotoc_md1745", null ],
      [ "1. ინტეგრაციის საჭიროების განსაზღვრა", "md_docs_2tutorials_2integration__example.html#autotoc_md1746", null ],
      [ "2. საჭირო მოდულების ჩართვა და კონფიგურაცია", "md_docs_2tutorials_2integration__example.html#autotoc_md1747", null ],
      [ "3. MQTT Integration (Publish/Subscribe)", "md_docs_2tutorials_2integration__example.html#autotoc_md1748", null ],
      [ "4. REST API Integration", "md_docs_2tutorials_2integration__example.html#autotoc_md1749", null ],
      [ "5. WebSocket Integration", "md_docs_2tutorials_2integration__example.html#autotoc_md1750", null ],
      [ "6. Best Practices", "md_docs_2tutorials_2integration__example.html#autotoc_md1751", null ],
      [ "7. Troubleshooting", "md_docs_2tutorials_2integration__example.html#autotoc_md1752", null ]
    ] ],
    [ "მოდულის გაფართოება ან ფუნქციონალის დამატება (Module Extension & Feature Addition)", "md_docs_2tutorials_2module__extension.html", [
      [ "მიზანი", "md_docs_2tutorials_2module__extension.html#autotoc_md1755", null ],
      [ "1. საჭიროების განსაზღვრა", "md_docs_2tutorials_2module__extension.html#autotoc_md1756", null ],
      [ "2. სტრუქტურის დაცვა", "md_docs_2tutorials_2module__extension.html#autotoc_md1757", null ],
      [ "3. ახალი API ან ფუნქციის დამატება", "md_docs_2tutorials_2module__extension.html#autotoc_md1758", null ],
      [ "4. კონფიგურაციის გაფართოება", "md_docs_2tutorials_2module__extension.html#autotoc_md1759", null ],
      [ "5. კომუნიკაციის გაფართოება", "md_docs_2tutorials_2module__extension.html#autotoc_md1760", null ],
      [ "6. ტესტირება და ლოგირება", "md_docs_2tutorials_2module__extension.html#autotoc_md1761", null ],
      [ "7. დოკუმენტაციის განახლება", "md_docs_2tutorials_2module__extension.html#autotoc_md1762", null ],
      [ "Best Practices", "md_docs_2tutorials_2module__extension.html#autotoc_md1763", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2tutorials_2module__extension.html#autotoc_md1764", null ]
    ] ],
    [ "სისტემის კონფიგურაცია (System Configuration)", "md_docs_2tutorials_2system__configuration.html", [
      [ "🎯 მიზანი", "md_docs_2tutorials_2system__configuration.html#autotoc_md1767", null ],
      [ "1. კონფიგურაციის ფაილების როლები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1769", null ],
      [ "2. <tt>config.json</tt>-ის სტრუქტურა და მაგალითები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1771", null ],
      [ "3. მოდულის კონფიგურაციის ობიექტის აღწერა", "md_docs_2tutorials_2system__configuration.html#autotoc_md1773", null ],
      [ "4. პარამეტრებზე წვდომა კოდიდან (Dot-Notation)", "md_docs_2tutorials_2system__configuration.html#autotoc_md1775", null ],
      [ "5. კონფიგურაციის ვალიდაცია და Default-ები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1777", null ],
      [ "6. საუკეთესო პრაქტიკები (Best Practices)", "md_docs_2tutorials_2system__configuration.html#autotoc_md1779", null ]
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
        [ "All", "functions.html", "functions_dup" ],
        [ "Variables", "functions_vars.html", "functions_vars" ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", null ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"alarms__manager_8c.html",
"functions_vars.html",
"md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md155",
"md_docs_2api__reference_2event__api.html#autotoc_md415",
"md_docs_2contributing_2branching__strategy.html#autotoc_md760",
"md_docs_2convention_2variable__naming.html#autotoc_md1085",
"md_docs_2security_2security__index.html#autotoc_md1545",
"resource__manager_8c.html#abb1c1f1c6aadfa6f7311de08d27fae8b",
"structevent__subscription__node__t.html",
"system__event__ids_8h.html#a14be883083731f631af5eb44a5f3a046a250d8cc23b0010dfe8fbb207f03160b3"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';