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
    [ "📺 მოდული: <tt>ssd1306_oled_display</tt>", "md_components_2modules_2displays_2ssd1306__oled__display_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2displays_2ssd1306__oled__display_2README.html#autotoc_md39", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2displays_2ssd1306__oled__display_2README.html#autotoc_md40", [
        [ "**3. ⚙️ კონფიგურაცია (<tt>config.json</tt> და <tt>Kconfig</tt>)**", "md_components_2modules_2displays_2ssd1306__oled__display_2README.html#autotoc_md41", null ]
      ] ],
      [ "4. 🔌 Service API (<tt>ssd1306_api_t</tt>)", "md_components_2modules_2displays_2ssd1306__oled__display_2README.html#autotoc_md42", null ],
      [ "5. 🚀 მუშაობის ლოგიკა", "md_components_2modules_2displays_2ssd1306__oled__display_2README.html#autotoc_md43", null ],
      [ "6. 💡 გამოყენების მაგალითი (<tt>selftest_manager</tt>-ში)", "md_components_2modules_2displays_2ssd1306__oled__display_2README.html#autotoc_md44", null ],
      [ "7. 🚀 სამომავლო გაფართოების იდეები", "md_components_2modules_2displays_2ssd1306__oled__display_2README.html#autotoc_md45", null ]
    ] ],
    [ "🚌 მოდული: <tt>i2c_bus</tt>", "md_components_2modules_2drivers_2i2c__bus_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md47", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md48", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md49", null ],
      [ "4. 🔌 Service API (<tt>i2c_bus_api_t</tt>)", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md50", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md51", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md52", null ]
    ] ],
    [ "📲 მოდული <tt>ble_provisioning</tt>", "md_components_2modules_2provisioning_2ble__provisioning_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md54", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md55", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md56", null ],
      [ "4. 🛠️ Service API (<tt>ble_prov_api_t</tt>)", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md57", null ],
      [ "5. 📢 ივენთები (Events)", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md58", [
        [ "გამოქვეყნებული ივენთები", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md59", null ],
        [ "გამოწერილი ივენთები", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md60", null ]
      ] ],
      [ "6. 💡 გამოყენების მაგალითი", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md61", null ],
      [ "7. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2provisioning_2ble__provisioning_2README.html#autotoc_md62", null ]
    ] ],
    [ "🛡️ მოდული: <tt>security_status_reporter</tt>", "md_components_2modules_2security_2security__status__reporter_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md64", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md65", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md66", null ],
      [ "4. 🔌 Service API (<tt>security_status_api_t</tt>)", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md67", null ],
      [ "5. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md68", null ],
      [ "6. 💡 გამოყენება და უსაფრთხოების ჩართვა", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md69", null ],
      [ "7. 🤝 ინტეგრაცია <tt>self_test_manager</tt>-თან", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md70", null ]
    ] ],
    [ "🌡️ მოდული: <tt>sensor_aggregator</tt>", "md_components_2modules_2sensors_2sensor__aggregator_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md72", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md73", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>system_config.json</tt>)", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md74", [
        [ "3.1. ძირითადი კონფიგურაციის პარამეტრები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md75", null ],
        [ "3.2. <tt>sensors_to_aggregate</tt> ობიექტის პარამეტრები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md76", [
          [ "**<tt>aggregation_mode</tt>-ის შესაძლო მნიშვნელობები:**", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md77", null ]
        ] ]
      ] ],
      [ "4. 📢 ივენთების მოდელი", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md78", null ],
      [ "5. 💡 გამოყენების მაგალითი და ტესტირება", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md79", null ],
      [ "6. 🚀 სამომავლო განვითარების პერსპექტივები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md80", null ]
    ] ],
    [ "💾 მოდული <tt>storage_manager</tt>", "md_components_2modules_2storage_2storage__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md82", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md83", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md84", null ],
      [ "4. 🛠️ Service API (<tt>storage_api_t</tt>)", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md85", [
        [ "Key-Value ოპერაციები", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md86", null ],
        [ "ფაილური ოპერაციები (მხარდაჭერილია SPIFFS და SD Card backend-ებზე)", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md87", null ]
      ] ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md88", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md89", null ]
    ] ],
    [ "🚨 მოდული: System Alarms Manager", "md_components_2modules_2system_2alarms__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md91", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md92", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md93", [
        [ "3.1. განგაშის წესის პარამეტრები (<tt>alarms</tt> ობიექტი)", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md94", null ],
        [ "3.2. მოქმედებების პარამეტრები (<tt>actions</tt> ობიექტი)", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md95", null ]
      ] ],
      [ "4. 🔌 Service API", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md96", null ],
      [ "5. 📢 ივენთების დამუშავება", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md97", null ],
      [ "6. 💡 გამოყენების სცენარები", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md98", null ],
      [ "7. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md99", null ]
    ] ],
    [ "🛰️ მოდული: Connectivity Watchdog", "md_components_2modules_2system_2connectivity__watchdog_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md101", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md102", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>system_config.json</tt>)", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md103", [
        [ "3.1. კონფიგურაციის სრული მაგალითი", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md104", null ],
        [ "3.2. <tt>layers</tt> ობიექტის პარამეტრები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md105", null ],
        [ "3.3. <tt>actions</tt> ობიექტის პარამეტრები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md106", null ],
        [ "3.4. მხარდაჭერილი <tt>type</tt>-ები და <tt>do</tt> მოქმედებები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md107", null ]
      ] ],
      [ "4. 🔌 Service API", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md108", null ],
      [ "5. 📢 ივენთების უნივერსალური მოდელი", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md109", [
        [ "5.1. MQTT მოდულის ინტეგრაცია", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md110", null ],
        [ "5.2. WebSocket მოდულის (ჰიპოთეტური) ინტეგრაცია", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md111", null ],
        [ "5.3. PING შემოწმების ინტეგრაცია", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md112", null ]
      ] ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md113", null ]
    ] ],
    [ "🆔 მოდული: <tt>device_identity_service</tt>", "md_components_2modules_2system_2device__identity__service_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md115", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md116", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md117", null ],
      [ "4. 🔌 Service API (<tt>device_identity_api_t</tt>)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md118", null ],
      [ "5. 📢 ივენთები (Events)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md119", [
        [ "გამოქვეყნებული ივენთები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md120", null ],
        [ "გამოწერილი ივენთები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md121", null ]
      ] ],
      [ "6. 📡 MQTT ინტერფეისი", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md122", null ],
      [ "7. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md123", null ],
      [ "8. 💡 გამოყენების მაგალითი (მოთხოვნა-პასუხი)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md124", null ],
      [ "9. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md125", null ]
    ] ],
    [ "მოდული: Health Monitor", "md_components_2modules_2system_2health__monitor_2README.html", [
      [ "1. მიმოხილვა", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md127", null ],
      [ "2. წინაპირობები და <tt>menuconfig</tt>", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md128", null ],
      [ "3. არქიტექტურული პრინციპები", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md129", null ],
      [ "4. კონფიგურაცია", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md130", null ],
      [ "5. Service API (<tt>health_api_t</tt>)", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md131", null ],
      [ "6. გამოქვეყნებული ივენთები", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md132", null ],
      [ "7. 💡 გამოყენების მაგალითი", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md133", null ]
    ] ],
    [ "მოდული: OTA Update Manager", "md_components_2modules_2system_2ota__update__manager_2README.html", [
      [ "1. მიმოხილვა", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md135", null ],
      [ "2. წინაპირობები და <tt>menuconfig</tt>", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md136", null ],
      [ "3. არქიტექტურული პრინციპები", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md137", null ],
      [ "4. კონფიგურაცია", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md138", null ],
      [ "5. Service API (<tt>ota_api_t</tt>)", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md139", null ],
      [ "6. გამოქვეყნებული ივენთები", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md140", null ],
      [ "7. 💡 გამოყენების მაგალითი", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md141", [
        [ "8. განახლების ვალიდაცია და Rollback", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md142", null ]
      ] ]
    ] ],
    [ "🐕 მოდული: <tt>watchdog_manager</tt>", "md_components_2modules_2system_2watchdog__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md144", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md145", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md146", null ],
      [ "4. 🔌 Service API (<tt>watchdog_api_t</tt>)", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md147", null ],
      [ "5. 📢 გამოქვეყნებული ივენთები", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md148", null ],
      [ "6. 💡 გამოყენების მაგალითი", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md149", null ],
      [ "7. ❗ წინაპირობები და <tt>menuconfig</tt>", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md150", null ]
    ] ],
    [ "🧪 მოდული: <tt>sensor_simulator</tt>", "md_components_2modules_2testing_2sensor__simulator_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md152", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md153", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md154", null ],
      [ "4. 📢 გამოქვეყნებული ივენთები", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md155", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md156", null ]
    ] ],
    [ "🧪 მოდული <tt>timer_test</tt>", "md_components_2modules_2testing_2timer__test__module_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md158", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md159", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md160", null ],
      [ "4. 🚀 მუშაობის ლოგიკა", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md161", null ],
      [ "5. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md162", null ]
    ] ],
    [ "⚙️ მოდული: Command Router & System Shell", "md_components_2modules_2utilities_2command__router_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md164", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md165", null ],
      [ "3. 🛠️ კონფიგურაცია", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md166", null ],
      [ "4. 🔌 Service API (<tt>cmd_router_api_t</tt>)", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md167", null ],
      [ "5. 🚀 ჩაშენებული ბრძანებები", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md168", null ],
      [ "6. 💡 გამოყენების მაგალითი", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md169", null ],
      [ "7. 🩺 პრობლემების მოგვარება (Troubleshooting)", "md_components_2modules_2utilities_2command__router_2README.html#autotoc_md170", null ]
    ] ],
    [ "Logger Module", "md_components_2modules_2utilities_2logger__module_2README.html", [
      [ "Overview", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md172", null ],
      [ "Features", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md173", null ],
      [ "Purpose", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md174", null ],
      [ "Kconfig Configuration", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md175", [
        [ "Main Configuration Options", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md176", null ],
        [ "Compilation Control", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md177", null ]
      ] ],
      [ "Runtime Configuration", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md178", [
        [ "Enable Logger Module", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md179", null ],
        [ "Disable Logger Module", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md180", null ]
      ] ],
      [ "Runtime Control", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md181", [
        [ "Command Format", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md182", null ],
        [ "Supported Commands", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md183", [
          [ "Disable Logger", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md184", null ],
          [ "Enable Logger", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md185", null ]
        ] ],
        [ "Behavior", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md186", null ]
      ] ],
      [ "Implementation Details", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md187", [
        [ "Module Structure", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md188", null ],
        [ "Event Subscription", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md189", null ],
        [ "Output Format", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md190", null ]
      ] ],
      [ "Development Notes", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md191", null ],
      [ "API Reference", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md192", [
        [ "Functions", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md193", [
          [ "<tt>logger_module_create(const cJSON *config)</tt>", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md194", null ]
        ] ],
        [ "ბრძანებები", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md195", null ],
        [ "ქცევა", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md196", null ],
        [ "მაგალითი", "md_components_2modules_2utilities_2logger__module_2README.html#autotoc_md197", null ]
      ] ]
    ] ],
    [ "💡 მოდული <tt>rgb_led_indicator</tt>", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md199", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md200", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md201", null ],
      [ "4. 🛠️ Service API (<tt>rgb_led_api_t</tt>)", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md202", null ],
      [ "5. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md203", null ],
      [ "6. 📢 ივენთების დამუშავება", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md204", null ],
      [ "7. ტექნიკური იმპლემენტაციის დეტალები", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md205", null ],
      [ "8. 💡 გამოყენების მაგალითი", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md206", null ],
      [ "9. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2utilities_2rgb__led__indicator_2README.html#autotoc_md207", null ]
    ] ],
    [ "⏱️ მოდული <tt>system_timer</tt>", "md_components_2modules_2utilities_2system__timer_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md209", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md210", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md211", null ],
      [ "4. 🛠️ Service API (<tt>timer_api_t</tt>)", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md212", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md213", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2utilities_2system__timer_2README.html#autotoc_md214", null ]
    ] ],
    [ "Synapse Actuators Module API Reference", "md_docs_2api__reference_2actuators__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2actuators__api.html#autotoc_md216", null ],
      [ "Relay Module API", "md_docs_2api__reference_2actuators__api.html#autotoc_md218", [
        [ "module_t <em>relay_module_create(const cJSON *config);", "md_docs_2api__reference_2actuators__api.html#autotoc_md219", null ],
        [ "esp_err_t relay_module_enable(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md220", null ],
        [ "esp_err_t relay_module_disable(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md221", null ],
        [ "esp_err_t relay_module_toggle(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md222", null ],
        [ "esp_err_t relay_module_set_state(module_t *module, bool state);", "md_docs_2api__reference_2actuators__api.html#autotoc_md223", null ],
        [ "bool relay_module_get_state(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md224", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2actuators__api.html#autotoc_md226", [
        [ "static void relay_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2actuators__api.html#autotoc_md227", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2actuators__api.html#autotoc_md229", [
        [ "static esp_err_t parse_relay_config(const cJSON *config, relay_private_data_t *relay_data);", "md_docs_2api__reference_2actuators__api.html#autotoc_md230", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2actuators__api.html#autotoc_md232", [
        [ "static module_status_t relay_get_status(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md233", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2actuators__api.html#autotoc_md235", null ],
      [ "მაგალითები", "md_docs_2api__reference_2actuators__api.html#autotoc_md237", null ]
    ] ],
    [ "Synapse API Reference Index", "md_docs_2api__reference_2api__index.html", [
      [ "Core API", "md_docs_2api__reference_2api__index.html#autotoc_md241", null ],
      [ "Module API", "md_docs_2api__reference_2api__index.html#autotoc_md242", null ],
      [ "System & Extensions", "md_docs_2api__reference_2api__index.html#autotoc_md243", null ]
    ] ],
    [ "Synapse Command Router API Reference", "md_docs_2api__reference_2command__router__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2command__router__api.html#autotoc_md246", null ],
      [ "⚙️ API სტრუქტურა (<tt>cmd_router_api_t</tt>)", "md_docs_2api__reference_2command__router__api.html#autotoc_md248", null ],
      [ "📚 API ფუნქციები", "md_docs_2api__reference_2command__router__api.html#autotoc_md250", [
        [ "<tt>esp_err_t register_command(const cmd_t *command)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md251", null ],
        [ "<tt>esp_err_t unregister_command(const char *command_name)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md253", null ],
        [ "<tt>bool is_command_registered(const char *command_name)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md255", null ]
      ] ],
      [ "📖 დამხმარე სტრუქტურა (<tt>cmd_t</tt>)", "md_docs_2api__reference_2command__router__api.html#autotoc_md257", null ],
      [ "💡 გამოყენების პატერნი: კონფლიქტისგან დაცული რეგისტრაცია", "md_docs_2api__reference_2command__router__api.html#autotoc_md259", null ]
    ] ],
    [ "📡 Synapse Communication Module API Reference", "md_docs_2api__reference_2communication__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2communication__api.html#autotoc_md261", null ],
      [ "📶 WiFi Manager Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md263", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md264", null ],
        [ "💡 WiFi API-ს გამოყენების მაგალითი", "md_docs_2api__reference_2communication__api.html#autotoc_md265", null ]
      ] ],
      [ "☁️ MQTT Client Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md267", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md268", null ],
        [ "💡 MQTT API-ს გამოყენების მაგალითი", "md_docs_2api__reference_2communication__api.html#autotoc_md269", null ]
      ] ],
      [ "⚡ ESP-NOW Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md271", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md272", null ]
      ] ],
      [ "📜 Naming & Structure კონვენციები", "md_docs_2api__reference_2communication__api.html#autotoc_md274", null ]
    ] ],
    [ "Synapse Configuration Manager API Reference", "md_docs_2api__reference_2configuration__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2configuration__api.html#autotoc_md277", null ],
      [ "🔑 გასაღების ფორმატი (Dot-Notation)", "md_docs_2api__reference_2configuration__api.html#autotoc_md279", null ],
      [ "⚙️ ძირითადი API ფუნქციები", "md_docs_2api__reference_2configuration__api.html#autotoc_md281", [
        [ "<tt>esp_err_t fmw_config_get_string(const char *key, char *buffer, size_t buffer_size)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md282", null ],
        [ "<tt>esp_err_t fmw_config_get_int(const char *key, int *out_value)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md283", null ],
        [ "<tt>esp_err_t fmw_config_get_bool(const char *key, bool *out_value)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md284", null ],
        [ "<tt>esp_err_t fmw_config_save(void)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md285", null ]
      ] ],
      [ "🚀 გამოყენების მაგალითები", "md_docs_2api__reference_2configuration__api.html#autotoc_md287", [
        [ "გლობალური პარამეტრის წაკითხვა", "md_docs_2api__reference_2configuration__api.html#autotoc_md288", null ],
        [ "მოდულის პარამეტრის წაკითხვა", "md_docs_2api__reference_2configuration__api.html#autotoc_md289", null ]
      ] ]
    ] ],
    [ "Synapse Core API Reference", "md_docs_2api__reference_2core__api.html", [
      [ "Service Locator API", "md_docs_2api__reference_2core__api.html#autotoc_md291", [
        [ "fmw_service_register", "md_docs_2api__reference_2core__api.html#autotoc_md292", null ],
        [ "fmw_service_unregister", "md_docs_2api__reference_2core__api.html#autotoc_md293", null ],
        [ "fmw_service_get", "md_docs_2api__reference_2core__api.html#autotoc_md294", null ],
        [ "fmw_service_get_type", "md_docs_2api__reference_2core__api.html#autotoc_md295", null ],
        [ "fmw_service_lookup_by_type", "md_docs_2api__reference_2core__api.html#autotoc_md296", null ]
      ] ],
      [ "Event Bus API", "md_docs_2api__reference_2core__api.html#autotoc_md298", [
        [ "<tt>fmw_event_bus_post</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md299", null ],
        [ "<tt>fmw_event_bus_subscribe</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md300", null ],
        [ "<tt>fmw_event_bus_unsubscribe</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md301", null ]
      ] ],
      [ "Module Register API", "md_docs_2api__reference_2core__api.html#autotoc_md303", [
        [ "fmw_module_register", "md_docs_2api__reference_2core__api.html#autotoc_md304", null ],
        [ "fmw_module_find_by_name", "md_docs_2api__reference_2core__api.html#autotoc_md305", null ]
      ] ],
      [ "Module Factory API", "md_docs_2api__reference_2core__api.html#autotoc_md307", [
        [ "fmw_module_factory_create", "md_docs_2api__reference_2core__api.html#autotoc_md308", null ]
      ] ],
      [ "Configuration Manager API", "md_docs_2api__reference_2core__api.html#autotoc_md310", [
        [ "fmw_config_get_string", "md_docs_2api__reference_2core__api.html#autotoc_md311", null ],
        [ "fmw_config_get_int", "md_docs_2api__reference_2core__api.html#autotoc_md312", null ],
        [ "fmw_config_get_module_config", "md_docs_2api__reference_2core__api.html#autotoc_md313", null ]
      ] ],
      [ "Logging API", "md_docs_2api__reference_2core__api.html#autotoc_md315", [
        [ "DEFINE_COMPONENT_TAG", "md_docs_2api__reference_2core__api.html#autotoc_md316", null ],
        [ "ESP_LOGE, ESP_LOGW, ESP_LOGI, ESP_LOGD, ESP_LOGV", "md_docs_2api__reference_2core__api.html#autotoc_md317", null ]
      ] ]
    ] ],
    [ "Synapse Custom API Reference", "md_docs_2api__reference_2custom__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2custom__api.html#autotoc_md320", null ],
      [ "Custom Module API", "md_docs_2api__reference_2custom__api.html#autotoc_md322", [
        [ "module_t *custom_module_create(const cJSON *config);", "md_docs_2api__reference_2custom__api.html#autotoc_md323", null ],
        [ "esp_err_t custom_module_do_action(module_t *module, const char *action, const void *params);", "md_docs_2api__reference_2custom__api.html#autotoc_md324", null ],
        [ "esp_err_t custom_module_get_status(module_t *module, custom_status_t *status);", "md_docs_2api__reference_2custom__api.html#autotoc_md325", null ]
      ] ],
      [ "Custom Event API", "md_docs_2api__reference_2custom__api.html#autotoc_md327", [
        [ "Custom Event-ის იდენტიფიკატორი", "md_docs_2api__reference_2custom__api.html#autotoc_md328", null ],
        [ "Custom Event-ის მონაცემების სტრუქტურა", "md_docs_2api__reference_2custom__api.html#autotoc_md329", null ],
        [ "static void custom_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2custom__api.html#autotoc_md330", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2custom__api.html#autotoc_md332", null ],
      [ "მაგალითები", "md_docs_2api__reference_2custom__api.html#autotoc_md334", null ]
    ] ],
    [ "Synapse Device Identity Service API Reference", "md_docs_2api__reference_2device__identity__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2device__identity__api.html#autotoc_md337", null ],
      [ "Device Identity Service API", "md_docs_2api__reference_2device__identity__api.html#autotoc_md339", [
        [ "<tt>const char* get_device_id(void);</tt>", "md_docs_2api__reference_2device__identity__api.html#autotoc_md340", null ],
        [ "<tt>const char* get_firmware_version(void);</tt>", "md_docs_2api__reference_2device__identity__api.html#autotoc_md341", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2device__identity__api.html#autotoc_md343", null ],
      [ "მაგალითები", "md_docs_2api__reference_2device__identity__api.html#autotoc_md345", null ]
    ] ],
    [ "Synapse Display Module API Reference", "md_docs_2api__reference_2display__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2display__api.html#autotoc_md348", null ],
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2display__api.html#autotoc_md350", [
        [ "module_t <em>ssd1306_module_create(const cJSON *config);", "md_docs_2api__reference_2display__api.html#autotoc_md351", null ],
        [ "static esp_err_t ssd1306_init(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md352", null ],
        [ "static esp_err_t ssd1306_enable(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md353", null ],
        [ "static esp_err_t ssd1306_disable(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md354", null ],
        [ "static esp_err_t ssd1306_deinit(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md355", null ]
      ] ],
      [ "Service API (Service Locator-ისთვის)", "md_docs_2api__reference_2display__api.html#autotoc_md357", [
        [ "esp_err_t ssd1306_api_enable(void);", "md_docs_2api__reference_2display__api.html#autotoc_md358", null ],
        [ "esp_err_t ssd1306_api_disable(void);", "md_docs_2api__reference_2display__api.html#autotoc_md359", null ],
        [ "esp_err_t ssd1306_api_clear(void);", "md_docs_2api__reference_2display__api.html#autotoc_md360", null ],
        [ "esp_err_t ssd1306_api_write_text(const char *text, uint8_t line);", "md_docs_2api__reference_2display__api.html#autotoc_md361", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2display__api.html#autotoc_md363", [
        [ "static void ssd1306_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2display__api.html#autotoc_md364", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2display__api.html#autotoc_md366", [
        [ "static esp_err_t parse_ssd1306_config(const cJSON *config, ssd1306_private_data_t *ssd1306_data);", "md_docs_2api__reference_2display__api.html#autotoc_md367", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2display__api.html#autotoc_md369", [
        [ "static module_status_t ssd1306_get_status(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md370", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2display__api.html#autotoc_md372", null ],
      [ "მაგალითები", "md_docs_2api__reference_2display__api.html#autotoc_md374", null ]
    ] ],
    [ "Synapse Drivers Module API Reference", "md_docs_2api__reference_2drivers__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2drivers__api.html#autotoc_md377", null ],
      [ "I2C Bus Driver API", "md_docs_2api__reference_2drivers__api.html#autotoc_md379", [
        [ "module_t *i2c_bus_module_create(const cJSON *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md380", null ],
        [ "esp_err_t i2c_bus_init(i2c_bus_handle_t *i2c_bus_handle, const i2c_bus_config_t *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md381", null ],
        [ "esp_err_t i2c_bus_read(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, uint8_t *data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md382", null ],
        [ "esp_err_t i2c_bus_write(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, const uint8_t *data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md383", null ],
        [ "esp_err_t i2c_bus_scan(i2c_bus_handle_t *i2c_bus_handle);", "md_docs_2api__reference_2drivers__api.html#autotoc_md384", null ]
      ] ],
      [ "SPI Bus Driver API", "md_docs_2api__reference_2drivers__api.html#autotoc_md386", [
        [ "module_t *spi_bus_module_create(const cJSON *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md387", null ],
        [ "esp_err_t spi_bus_init(spi_bus_handle_t *spi_bus_handle, const spi_bus_config_t *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md388", null ],
        [ "esp_err_t spi_bus_transfer(spi_bus_handle_t *spi_bus_handle, const uint8_t *tx_data, uint8_t *rx_data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md389", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2drivers__api.html#autotoc_md391", null ],
      [ "მაგალითები", "md_docs_2api__reference_2drivers__api.html#autotoc_md393", null ]
    ] ],
    [ "Synapse Event Bus API Reference", "md_docs_2api__reference_2event__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2event__api.html#autotoc_md396", null ],
      [ "Event Bus API", "md_docs_2api__reference_2event__api.html#autotoc_md398", [
        [ "<tt>esp_err_t fmw_event_bus_init(void)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md399", null ],
        [ "<tt>esp_err_t fmw_event_bus_post(core_framework_event_id_t event_id, event_data_wrapper_t *data_wrapper)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md400", null ],
        [ "<tt>esp_err_t fmw_event_bus_subscribe(core_framework_event_id_t event_id, struct module_t *module)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md401", null ],
        [ "<tt>esp_err_t fmw_event_bus_unsubscribe(core_framework_event_id_t event_id, struct module_t *module)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md402", null ]
      ] ],
      [ "ივენთის მონაცემების მართვა (Reference Counting)", "md_docs_2api__reference_2event__api.html#autotoc_md404", null ],
      [ "Event Handler-ის მაგალითი", "md_docs_2api__reference_2event__api.html#autotoc_md406", null ],
      [ "Custom Event-ების აღწერა", "md_docs_2api__reference_2event__api.html#autotoc_md408", [
        [ "მაგალითი Custom Event-ის აღწერისა", "md_docs_2api__reference_2event__api.html#autotoc_md409", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2event__api.html#autotoc_md411", null ],
      [ "მაგალითები", "md_docs_2api__reference_2event__api.html#autotoc_md413", null ]
    ] ],
    [ "API რეფერენსი: Event Payloads (event_payloads.h)", "md_docs_2api__reference_2event__payloads__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md416", null ],
      [ "Payload სტრუქტურები", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md417", [
        [ "fmw_telemetry_payload_t", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md418", null ],
        [ "fmw_config_updated_payload_t", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md419", null ]
      ] ],
      [ "მეხსიერების მართვის ფუნქციები", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md420", [
        [ "fmw_payload_common_free", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md421", null ],
        [ "fmw_telemetry_payload_free", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md422", null ]
      ] ]
    ] ],
    [ "Synapse Health Monitor Module API Reference", "md_docs_2api__reference_2health__monitor__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md424", null ],
      [ "Health Monitor Module API", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md426", [
        [ "module_t *health_monitor_module_create(const cJSON *config);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md427", null ],
        [ "esp_err_t health_monitor_module_start(module_t *module);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md428", null ],
        [ "esp_err_t health_monitor_module_stop(module_t *module);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md429", null ],
        [ "esp_err_t health_monitor_module_get_status(module_t *module, health_status_t *status);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md430", null ],
        [ "esp_err_t health_monitor_module_register_check(module_t *module, health_check_fn_t check_fn);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md431", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md433", [
        [ "static void health_monitor_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md434", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md436", null ],
      [ "მაგალითები", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md438", null ]
    ] ],
    [ "Synapse Local Control Module API Reference", "md_docs_2api__reference_2local__ctrl__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md441", null ],
      [ "Local Control Module API", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md443", [
        [ "module_t *local_ctrl_module_create(const cJSON *config);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md444", null ],
        [ "esp_err_t local_ctrl_module_start(module_t *module);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md445", null ],
        [ "esp_err_t local_ctrl_module_stop(module_t *module);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md446", null ],
        [ "esp_err_t local_ctrl_module_register_endpoint(const char *uri, http_method_t method, local_ctrl_handler_t handler);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md447", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md449", [
        [ "static void local_ctrl_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md450", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md452", null ],
      [ "მაგალითები", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md454", null ]
    ] ],
    [ "Synapse Module API Reference", "md_docs_2api__reference_2module__api.html", [
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2module__api.html#autotoc_md457", [
        [ "<tt>module_t *{module_name}_create(const cJSON *config);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md458", null ],
        [ "<tt>static esp_err_t {module_name}_init(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md459", null ],
        [ "<tt>static esp_err_t {module_name}_start(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md460", null ],
        [ "<tt>static void {module_name}_deinit(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md461", null ]
      ] ],
      [ "Service API (Service Locator-ისთვის)", "md_docs_2api__reference_2module__api.html#autotoc_md463", [
        [ "<tt>esp_err_t {module_name}_api_{action}(...);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md464", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2module__api.html#autotoc_md466", [
        [ "<tt>static void {module_name}_handle_event(module_t *self, const char *event_name, void *event_data);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md467", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2module__api.html#autotoc_md469", [
        [ "<tt>static esp_err_t parse_{module_name}_config(const cJSON *config_node, {module_name}_private_data_t *p_data);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md470", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2module__api.html#autotoc_md472", [
        [ "<tt>static module_status_t {module_name}_get_status(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md473", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2module__api.html#autotoc_md475", null ]
    ] ],
    [ "Synapse MQTT Manager Service API Reference", "md_docs_2api__reference_2mqtt__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2mqtt__api.html#autotoc_md478", null ],
      [ "MQTT Manager API", "md_docs_2api__reference_2mqtt__api.html#autotoc_md480", [
        [ "სამომავლო გაფართოება", "md_docs_2api__reference_2mqtt__api.html#autotoc_md481", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2mqtt__api.html#autotoc_md483", null ],
      [ "მაგალითები", "md_docs_2api__reference_2mqtt__api.html#autotoc_md485", null ]
    ] ],
    [ "Synapse OTA (Over-the-Air) Module API Reference", "md_docs_2api__reference_2ota__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2ota__api.html#autotoc_md488", null ],
      [ "OTA Module API", "md_docs_2api__reference_2ota__api.html#autotoc_md490", [
        [ "module_t *ota_module_create(const cJSON *config);", "md_docs_2api__reference_2ota__api.html#autotoc_md491", null ],
        [ "esp_err_t ota_module_start_update(const char *url);", "md_docs_2api__reference_2ota__api.html#autotoc_md492", null ],
        [ "esp_err_t ota_module_get_status(ota_status_t *status);", "md_docs_2api__reference_2ota__api.html#autotoc_md493", null ],
        [ "esp_err_t ota_module_abort(void);", "md_docs_2api__reference_2ota__api.html#autotoc_md494", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2ota__api.html#autotoc_md496", [
        [ "static void ota_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2ota__api.html#autotoc_md497", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2ota__api.html#autotoc_md499", null ],
      [ "მაგალითები", "md_docs_2api__reference_2ota__api.html#autotoc_md501", null ]
    ] ],
    [ "📲 Synapse Provisioning Module API Reference", "md_docs_2api__reference_2provisioning__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2provisioning__api.html#autotoc_md504", null ],
      [ "🔵 BLE Provisioning Service API (იმპლემენტირებულია)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md506", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2provisioning__api.html#autotoc_md507", null ]
      ] ],
      [ "📡 SoftAP Provisioning Module API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md509", [
        [ "module_t *softap_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md510", null ],
        [ "esp_err_t softap_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md511", null ],
        [ "esp_err_t softap_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md512", null ],
        [ "esp_err_t softap_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md513", null ],
        [ "esp_err_t softap_prov_module_get_status(module_t *module, softap_prov_status_t*status)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md514", null ]
      ] ],
      [ "🔳 QR Code Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md516", [
        [ "esp_err_t qr_code_prov_generate(const char *ssid, const char*password, char *qr_code_buffer, size_t buffer_size)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md517", null ],
        [ "esp_err_t qr_code_prov_display(const char *qr_code_buffer)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md518", null ]
      ] ],
      [ "🔌 Serial Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md520", [
        [ "module_t *serial_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md521", null ],
        [ "esp_err_t serial_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md522", null ],
        [ "esp_err_t serial_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md523", null ],
        [ "esp_err_t serial_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md524", null ]
      ] ],
      [ "☁️ Cloud Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md526", [
        [ "module_t *cloud_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md527", null ],
        [ "esp_err_t cloud_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md528", null ],
        [ "esp_err_t cloud_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md529", null ],
        [ "esp_err_t cloud_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md530", null ]
      ] ],
      [ "NFC Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md532", [
        [ "module_t *nfc_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md533", null ],
        [ "esp_err_t nfc_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md534", null ],
        [ "esp_err_t nfc_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md535", null ],
        [ "esp_err_t nfc_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md536", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2provisioning__api.html#autotoc_md538", null ],
      [ "მაგალითები", "md_docs_2api__reference_2provisioning__api.html#autotoc_md540", null ]
    ] ],
    [ "🔌 Relay Actuator API Reference", "md_docs_2api__reference_2relay__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2relay__api.html#autotoc_md543", null ],
      [ "1. ⌨️ CLI (Command Line Interface)", "md_docs_2api__reference_2relay__api.html#autotoc_md545", null ],
      [ "2. 📡 MQTT ინტერფეისი", "md_docs_2api__reference_2relay__api.html#autotoc_md547", [
        [ "ბრძანების გაგზავნა", "md_docs_2api__reference_2relay__api.html#autotoc_md548", null ],
        [ "სტატუსის მიღება", "md_docs_2api__reference_2relay__api.html#autotoc_md549", null ]
      ] ],
      [ "3. 📢 ივენთები (Events)", "md_docs_2api__reference_2relay__api.html#autotoc_md551", null ]
    ] ],
    [ "Synapse Resource Manager API Reference", "md_docs_2api__reference_2resource__manager__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md554", null ],
      [ "სამართავი რესურსების ტიპები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md556", null ],
      [ "API ფუნქციები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md558", [
        [ "<tt>esp_err_t fmw_resource_manager_init(void)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md559", null ],
        [ "<tt>esp_err_t fmw_resource_lock(fmw_resource_type_t type, uint8_t resource_id, const char *owner)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md561", null ],
        [ "<tt>esp_err_t fmw_resource_release(fmw_resource_type_t type, uint8_t resource_id, const char *owner)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md563", null ],
        [ "<tt>bool fmw_resource_is_locked(fmw_resource_type_t type, uint8_t resource_id)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md565", null ],
        [ "<tt>const char *fmw_resource_get_owner(fmw_resource_type_t type, uint8_t resource_id)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md567", null ]
      ] ],
      [ "გამოყენების მაგალითი", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md569", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md571", null ]
    ] ],
    [ "🛡️ Security Status API Reference", "md_docs_2api__reference_2security__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2security__api.html#autotoc_md574", null ],
      [ "1. 🔌 Service API (<tt>security_status_api_t</tt>)", "md_docs_2api__reference_2security__api.html#autotoc_md576", [
        [ "API ფუნქციები", "md_docs_2api__reference_2security__api.html#autotoc_md577", null ]
      ] ],
      [ "2. ⌨️ CLI (Command Line Interface)", "md_docs_2api__reference_2security__api.html#autotoc_md579", null ]
    ] ],
    [ "Synapse Sensor Module API Reference", "md_docs_2api__reference_2sensor__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2sensor__api.html#autotoc_md581", null ],
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2sensor__api.html#autotoc_md583", [
        [ "module_t <em>dht22_module_create(const cJSON *config);", "md_docs_2api__reference_2sensor__api.html#autotoc_md584", null ],
        [ "static esp_err_t dht22_init(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md585", null ],
        [ "static esp_err_t dht22_enable(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md586", null ],
        [ "static esp_err_t dht22_disable(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md587", null ],
        [ "static esp_err_t dht22_deinit(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md588", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2sensor__api.html#autotoc_md590", [
        [ "static void dht22_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2sensor__api.html#autotoc_md591", null ],
        [ "static esp_err_t publish_sensor_data(dht22_private_data_t *dht22_data, float temperature, float humidity);", "md_docs_2api__reference_2sensor__api.html#autotoc_md592", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2sensor__api.html#autotoc_md594", [
        [ "static esp_err_t parse_dht22_config(const cJSON *config, dht22_private_data_t *dht22_data);", "md_docs_2api__reference_2sensor__api.html#autotoc_md595", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2sensor__api.html#autotoc_md597", [
        [ "static module_status_t dht22_get_status(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md598", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2sensor__api.html#autotoc_md600", null ],
      [ "მაგალითები", "md_docs_2api__reference_2sensor__api.html#autotoc_md602", null ]
    ] ],
    [ "💾 Synapse Storage Manager API Reference", "md_docs_2api__reference_2storage__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2storage__api.html#autotoc_md605", null ],
      [ "⚙️ API-ს სტრუქტურა (<tt>storage_api_t</tt>)", "md_docs_2api__reference_2storage__api.html#autotoc_md607", [
        [ "🔑 Key-Value ოპერაციები", "md_docs_2api__reference_2storage__api.html#autotoc_md608", null ],
        [ "🗂️ ფაილური ოპერაციები", "md_docs_2api__reference_2storage__api.html#autotoc_md610", null ]
      ] ],
      [ "💡 გამოყენების მაგალითი", "md_docs_2api__reference_2storage__api.html#autotoc_md612", null ]
    ] ],
    [ "Synapse System Module API Reference", "md_docs_2api__reference_2system__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2system__api.html#autotoc_md615", null ],
      [ "OTA Module API", "md_docs_2api__reference_2system__api.html#autotoc_md617", [
        [ "module_t *ota_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md618", null ],
        [ "esp_err_t ota_module_start_update(const char *url);", "md_docs_2api__reference_2system__api.html#autotoc_md619", null ],
        [ "esp_err_t ota_module_get_status(ota_status_t *status);", "md_docs_2api__reference_2system__api.html#autotoc_md620", null ]
      ] ],
      [ "Version Module API", "md_docs_2api__reference_2system__api.html#autotoc_md622", [
        [ "module_t *version_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md623", null ],
        [ "esp_err_t version_module_get_firmware_version(char *version_str, size_t size);", "md_docs_2api__reference_2system__api.html#autotoc_md624", null ]
      ] ],
      [ "Watchdog Module API", "md_docs_2api__reference_2system__api.html#autotoc_md626", [
        [ "module_t *watchdog_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md627", null ],
        [ "esp_err_t watchdog_module_feed(void);", "md_docs_2api__reference_2system__api.html#autotoc_md628", null ],
        [ "esp_err_t watchdog_module_set_timeout(uint32_t timeout_ms);", "md_docs_2api__reference_2system__api.html#autotoc_md629", null ]
      ] ],
      [ "Diagnostics Module API", "md_docs_2api__reference_2system__api.html#autotoc_md631", [
        [ "module_t *diagnostics_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md632", null ],
        [ "esp_err_t diagnostics_module_run_tests(void);", "md_docs_2api__reference_2system__api.html#autotoc_md633", null ],
        [ "esp_err_t diagnostics_module_get_report(char *report_buffer, size_t buffer_size);", "md_docs_2api__reference_2system__api.html#autotoc_md634", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2system__api.html#autotoc_md636", null ],
      [ "მაგალითები", "md_docs_2api__reference_2system__api.html#autotoc_md638", null ]
    ] ],
    [ "Synapse Time Sync Module API Reference", "md_docs_2api__reference_2time__sync__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2time__sync__api.html#autotoc_md641", null ],
      [ "Time Sync Module API", "md_docs_2api__reference_2time__sync__api.html#autotoc_md643", [
        [ "module_t *time_sync_module_create(const cJSON *config);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md644", null ],
        [ "esp_err_t time_sync_module_start(module_t *module);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md645", null ],
        [ "esp_err_t time_sync_module_stop(module_t *module);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md646", null ],
        [ "esp_err_t time_sync_module_set_server(module_t *module, const char *server_url);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md647", null ],
        [ "esp_err_t time_sync_module_get_time(time_t *current_time);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md648", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2time__sync__api.html#autotoc_md650", [
        [ "static void time_sync_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md651", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2time__sync__api.html#autotoc_md653", null ],
      [ "მაგალითები", "md_docs_2api__reference_2time__sync__api.html#autotoc_md655", null ]
    ] ],
    [ "Synapse Utility Module API Reference", "md_docs_2api__reference_2utility__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2utility__api.html#autotoc_md658", null ],
      [ "Logger Module API", "md_docs_2api__reference_2utility__api.html#autotoc_md660", [
        [ "module_t *logger_module_create(const cJSON *config);", "md_docs_2api__reference_2utility__api.html#autotoc_md661", null ],
        [ "esp_err_t logger_module_log_info(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md662", null ],
        [ "esp_err_t logger_module_log_warning(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md663", null ],
        [ "esp_err_t logger_module_log_error(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md664", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2utility__api.html#autotoc_md666", null ],
      [ "მაგალითები", "md_docs_2api__reference_2utility__api.html#autotoc_md668", null ]
    ] ],
    [ "Synapse Version Module API Reference", "md_docs_2api__reference_2version__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2version__api.html#autotoc_md671", null ],
      [ "Version Module API", "md_docs_2api__reference_2version__api.html#autotoc_md673", [
        [ "module_t *version_module_create(const cJSON *config);", "md_docs_2api__reference_2version__api.html#autotoc_md674", null ],
        [ "esp_err_t version_module_get_firmware_version(char *version_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md675", null ],
        [ "esp_err_t version_module_get_build_date(char *date_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md676", null ],
        [ "esp_err_t version_module_get_build_time(char *time_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md677", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2version__api.html#autotoc_md679", null ],
      [ "მაგალითები", "md_docs_2api__reference_2version__api.html#autotoc_md681", null ]
    ] ],
    [ "Synapse WiFi Manager Service API Reference", "md_docs_2api__reference_2wifi__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2wifi__api.html#autotoc_md684", null ],
      [ "WiFi Manager API", "md_docs_2api__reference_2wifi__api.html#autotoc_md686", [
        [ "სამომავლო გაფართოება", "md_docs_2api__reference_2wifi__api.html#autotoc_md687", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2wifi__api.html#autotoc_md689", null ],
      [ "მაგალითები", "md_docs_2api__reference_2wifi__api.html#autotoc_md691", null ]
    ] ],
    [ "CHANGELOG: led_blinker ინიციალიზაციის პრობლემის დიაგნოსტიკა და გამოსწორება", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html", [
      [ "პრობლემა", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md694", null ],
      [ "დიაგნოსტიკა", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md695", null ],
      [ "მიზეზი", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md696", null ],
      [ "გამოსავალი", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md697", null ],
      [ "ცვლილებები კოდში", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md698", null ],
      [ "რეკომენდაცია", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md699", null ]
    ] ],
    [ "ცვლილებების ჟურნალი — 2025-06-24", "md_docs_2changelog_2CHANGELOG__2025-06-24.html", [
      [ "დამატებული და განახლებული ფაილები", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md702", [
        [ "[core/CMakeLists.txt]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md703", null ],
        [ "[core/collect_modules.cmake]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md704", null ],
        [ "[core/include/system_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md705", null ],
        [ "[interfaces/include/base_module.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md706", null ],
        [ "[core/include/service_locator.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md707", null ],
        [ "[core/include/module_registry.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md708", null ],
        [ "[core/include/event_bus.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md709", null ],
        [ "[core/include/config_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md710", null ],
        [ "[core/include/system_event_ids.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md711", null ],
        [ "[core/include/resource_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md712", null ],
        [ "[core/include/framework_config.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md713", null ],
        [ "[interfaces/include/logging.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md714", null ],
        [ "[interfaces/include/service_types.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md715", null ],
        [ "[interfaces/include/telemetry_events.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md716", null ],
        [ "[README.md]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md717", null ],
        [ "[.gitignore]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md718", null ]
      ] ],
      [ "ძირითადი ცვლილებები", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md720", null ]
    ] ],
    [ "CHANGELOG - 2025-06-26", "md_docs_2changelog_2CHANGELOG__2025-06-26.html", [
      [ "[Major] Module System Automation & Refactor", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md723", [
        [ "Added", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md724", null ],
        [ "Changed", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md725", null ],
        [ "Fixed", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md726", null ],
        [ "Detailed File Changes", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md727", null ],
        [ "Why/How", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md728", null ],
        [ "Testing", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md729", null ]
      ] ]
    ] ],
    [ "Changelog", "md_docs_2changelog_2CHANGELOG__2025-06-28.html", [
      [ "[2.0.0] - 2025-06-28", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md731", [
        [ "დამატებულია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md732", null ],
        [ "შეცვლილია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md733", null ],
        [ "გაუქმებულია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md734", null ],
        [ "წაშლილია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md735", null ],
        [ "გასწორებულია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md736", null ],
        [ "უსაფრთხოება", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md737", null ],
        [ "ცნობილი პრობლემები", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md738", null ]
      ] ]
    ] ],
    [ "Synapse Branching Strategy", "md_docs_2contributing_2branching__strategy.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2branching__strategy.html#autotoc_md741", null ],
      [ "ძირითადი ბრენჩები", "md_docs_2contributing_2branching__strategy.html#autotoc_md743", null ],
      [ "ბრენჩის სახელდების წესები", "md_docs_2contributing_2branching__strategy.html#autotoc_md745", null ],
      [ "სამუშაო პროცესი", "md_docs_2contributing_2branching__strategy.html#autotoc_md747", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2contributing_2branching__strategy.html#autotoc_md749", null ],
      [ "რესურსები", "md_docs_2contributing_2branching__strategy.html#autotoc_md751", null ]
    ] ],
    [ "ქცევის კოდექსი (Code of Conduct)", "md_docs_2contributing_2code__of__conduct.html", [
      [ "მიზანი", "md_docs_2contributing_2code__of__conduct.html#autotoc_md755", null ],
      [ "ძირითადი პრინციპები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md757", null ],
      [ "დაუშვებელი ქცევა", "md_docs_2contributing_2code__of__conduct.html#autotoc_md759", null ],
      [ "პასუხისმგებლობები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md761", null ],
      [ "დარღვევის შემთხვევაში მოქმედება", "md_docs_2contributing_2code__of__conduct.html#autotoc_md763", null ],
      [ "დამატებითი რესურსები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md765", null ]
    ] ],
    [ "Synapse Commit Message Guidelines", "md_docs_2contributing_2commit__message__guidelines.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md769", null ],
      [ "ძირითადი სტრუქტურა", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md771", null ],
      [ "სათაურის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md773", null ],
      [ "დეტალური აღწერის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md775", null ],
      [ "Footer-ის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md777", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md779", null ],
      [ "რესურსები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md781", null ]
    ] ],
    [ "კონტრიბუციის სახელმძღვანელო (Contributing Guide)", "md_docs_2contributing_2contributing__guide.html", [
      [ "მიზანი", "md_docs_2contributing_2contributing__guide.html#autotoc_md784", null ],
      [ "1. კონტრიბუციის პროცესი", "md_docs_2contributing_2contributing__guide.html#autotoc_md786", null ],
      [ "2. კოდის სტანდარტები", "md_docs_2contributing_2contributing__guide.html#autotoc_md787", null ],
      [ "3. დოკუმენტაციის მოთხოვნები", "md_docs_2contributing_2contributing__guide.html#autotoc_md788", null ],
      [ "4. Pull Request-ის მიმოხილვა", "md_docs_2contributing_2contributing__guide.html#autotoc_md789", null ],
      [ "5. Best Practices", "md_docs_2contributing_2contributing__guide.html#autotoc_md790", null ],
      [ "6. აკრძალული პრაქტიკები", "md_docs_2contributing_2contributing__guide.html#autotoc_md791", null ]
    ] ],
    [ "Synapse Contributing Reference Index", "md_docs_2contributing_2contributing__index.html", [
      [ "ძირითადი სახელმძღვანელოები", "md_docs_2contributing_2contributing__index.html#autotoc_md795", null ],
      [ "შაბლონები", "md_docs_2contributing_2contributing__index.html#autotoc_md796", null ]
    ] ],
    [ "კონვენციების ავტომატური შემმოწმებელი (<tt>check_conventions.py</tt>)", "md_docs_2contributing_2convention__linter__guide.html", [
      [ "1. შესავალი", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md799", null ],
      [ "2. რას ამოწმებს სკრიპტი?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md800", null ],
      [ "3. როგორ გამოვიყენოთ ლოკალურად?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md805", null ],
      [ "4. ინტეგრაცია GitHub Actions-თან (ავტომატური შემოწმება)", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md807", null ],
      [ "5. როგორ გამოვასწოროთ გავრცელებული შეცდომები?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md808", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (FAQ)", "md_docs_2contributing_2faq.html", [
      [ "1. როგორ დავიწყო კონტრიბუცია Synapse Framework-ში?", "md_docs_2contributing_2faq.html#autotoc_md811", null ],
      [ "2. რა სტანდარტებს და კონვენციებს უნდა დავიცვა?", "md_docs_2contributing_2faq.html#autotoc_md812", null ],
      [ "3. როგორ შევქმნა Pull Request?", "md_docs_2contributing_2faq.html#autotoc_md813", null ],
      [ "4. როგორ მოვამზადო ლოკალური გარემო?", "md_docs_2contributing_2faq.html#autotoc_md814", null ],
      [ "5. როგორ გავუშვა ტესტები?", "md_docs_2contributing_2faq.html#autotoc_md815", null ],
      [ "6. როგორ მოვაგვარო გარემოს ან ბილდის პრობლემები?", "md_docs_2contributing_2faq.html#autotoc_md816", null ],
      [ "7. სად ვიპოვო დოკუმენტაციის სტრუქტურის და სტანდარტების შესახებ ინფორმაცია?", "md_docs_2contributing_2faq.html#autotoc_md817", null ],
      [ "8. როგორ დავამატო ან განვაახლო დოკუმენტაცია?", "md_docs_2contributing_2faq.html#autotoc_md818", null ],
      [ "9. როგორ დავიცვა ქცევის კოდექსი?", "md_docs_2contributing_2faq.html#autotoc_md819", null ],
      [ "10. ვის მივმართო დამატებითი კითხვების ან პრობლემების შემთხვევაში?", "md_docs_2contributing_2faq.html#autotoc_md820", null ]
    ] ],
    [ "Synapse Issue Template", "md_docs_2contributing_2issue__template.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2issue__template.html#autotoc_md824", null ],
      [ "ძირითადი ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md826", null ],
      [ "დეტალური აღწერა", "md_docs_2contributing_2issue__template.html#autotoc_md828", null ],
      [ "გამეორების ნაბიჯები (თუ ეს ბაგია)", "md_docs_2contributing_2issue__template.html#autotoc_md830", null ],
      [ "მოსალოდნელი შედეგი", "md_docs_2contributing_2issue__template.html#autotoc_md832", null ],
      [ "ფაქტობრივი შედეგი", "md_docs_2contributing_2issue__template.html#autotoc_md834", null ],
      [ "გარემოს ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md836", null ],
      [ "დამატებითი ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md838", null ],
      [ "რესურსები", "md_docs_2contributing_2issue__template.html#autotoc_md840", null ]
    ] ],
    [ "Synapse Local Setup Guide", "md_docs_2contributing_2local__setup.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2local__setup.html#autotoc_md844", null ],
      [ "სისტემური მოთხოვნები", "md_docs_2contributing_2local__setup.html#autotoc_md846", null ],
      [ "ESP-IDF-ის ინსტალაცია", "md_docs_2contributing_2local__setup.html#autotoc_md848", null ],
      [ "პროექტის კლონირება", "md_docs_2contributing_2local__setup.html#autotoc_md850", null ],
      [ "sdkconfig ფაილის კონფიგურაცია", "md_docs_2contributing_2local__setup.html#autotoc_md852", null ],
      [ "პროექტის აგება და გაშვება", "md_docs_2contributing_2local__setup.html#autotoc_md854", null ],
      [ "ტესტირება", "md_docs_2contributing_2local__setup.html#autotoc_md856", null ],
      [ "პრობლემების შემთხვევაში", "md_docs_2contributing_2local__setup.html#autotoc_md858", null ],
      [ "რესურსები", "md_docs_2contributing_2local__setup.html#autotoc_md860", null ]
    ] ],
    [ "Synapse Pull Request Template", "md_docs_2contributing_2pull__request__template.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2pull__request__template.html#autotoc_md864", null ],
      [ "ძირითადი ინფორმაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md866", null ],
      [ "ცვლილებების დეტალური აღწერა", "md_docs_2contributing_2pull__request__template.html#autotoc_md868", null ],
      [ "ტესტირება და ვალიდაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md870", null ],
      [ "მიმოხილვის რეკომენდაციები", "md_docs_2contributing_2pull__request__template.html#autotoc_md872", null ],
      [ "დამატებითი ინფორმაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md874", null ],
      [ "რესურსები", "md_docs_2contributing_2pull__request__template.html#autotoc_md876", null ]
    ] ],
    [ "Synapse Review Guidelines", "md_docs_2contributing_2review__guidelines.html", [
      [ "მიმოხილვის მიზანი", "md_docs_2contributing_2review__guidelines.html#autotoc_md880", null ],
      [ "მიმოხილვის პროცესი", "md_docs_2contributing_2review__guidelines.html#autotoc_md882", null ],
      [ "მიმოხილვის კრიტერიუმები", "md_docs_2contributing_2review__guidelines.html#autotoc_md884", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2contributing_2review__guidelines.html#autotoc_md886", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2contributing_2review__guidelines.html#autotoc_md888", null ],
      [ "რესურსები", "md_docs_2contributing_2review__guidelines.html#autotoc_md890", null ]
    ] ],
    [ "9. Doxygen კომენტარების სტანდარტი (v1.0)", "md_docs_2convention_2coding__doxygen__standard.html", [
      [ "1. შესავალი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md893", null ],
      [ "2. ფაილის სათაურის (Header) კომენტარი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md894", [
        [ "2.1. სტრუქტურა", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md895", null ],
        [ "2.2. მაგალითი (<tt>i2c_bus_module.h</tt>)", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md896", null ]
      ] ],
      [ "3. ფუნქციის კომენტარი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md897", [
        [ "3.1. სტრუქტურა", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md898", null ],
        [ "3.2. მაგალითი (<tt>service_locator.h</tt>)", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md899", null ]
      ] ],
      [ "4. სტრუქტურების და ტიპების კომენტარები", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md900", [
        [ "4.1. სტრუქტურის მაგალითი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md901", null ]
      ] ],
      [ "5. Doxygen ტეგების გამოყენების წესები", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md902", null ]
    ] ],
    [ "კომუნიკაციის პატერნების წესები", "md_docs_2convention_2communication__patterns.html", [
      [ "ძირითადი პრინციპი: მაქსიმალური იზოლაცია", "md_docs_2convention_2communication__patterns.html#autotoc_md904", null ],
      [ "სამი ძირითადი პატერნი", "md_docs_2convention_2communication__patterns.html#autotoc_md905", [
        [ "1. Service Locator Pattern", "md_docs_2convention_2communication__patterns.html#autotoc_md906", null ],
        [ "2. Event Bus Pattern", "md_docs_2convention_2communication__patterns.html#autotoc_md907", null ],
        [ "3. Command Router Pattern (ახალი სექცია)", "md_docs_2convention_2communication__patterns.html#autotoc_md908", null ]
      ] ],
      [ "კომუნიკაციის ტიპები", "md_docs_2convention_2communication__patterns.html#autotoc_md909", [
        [ "Direct API Communication (Service Locator)", "md_docs_2convention_2communication__patterns.html#autotoc_md910", null ],
        [ "Broadcast Communication (Event Bus)", "md_docs_2convention_2communication__patterns.html#autotoc_md911", null ]
      ] ],
      [ "აკრძალული პატერნები", "md_docs_2convention_2communication__patterns.html#autotoc_md912", [
        [ "❌ პირდაპირი დამოკიდებულებები", "md_docs_2convention_2communication__patterns.html#autotoc_md913", null ],
        [ "❌ პატერნების არასწორი შერევა", "md_docs_2convention_2communication__patterns.html#autotoc_md914", null ],
        [ "❌ System Manager MQTT Handling", "md_docs_2convention_2communication__patterns.html#autotoc_md915", null ]
      ] ],
      [ "სწორი იმპლემენტაცია", "md_docs_2convention_2communication__patterns.html#autotoc_md916", [
        [ "Service Locator-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md917", [
          [ "1. Service Registration (Display Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md918", null ],
          [ "2. Service Usage (MQTT Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md919", null ]
        ] ],
        [ "Event Bus-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md920", [
          [ "1. Event Publishing (Sensor Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md921", null ],
          [ "2. Event Subscription (MQTT Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md922", null ]
        ] ]
      ] ],
      [ "დინამიური MQTT თემები <tt>{module_name}</tt> Placeholder-ით", "md_docs_2convention_2communication__patterns.html#autotoc_md923", [
        [ "პატერნის აღწერა", "md_docs_2convention_2communication__patterns.html#autotoc_md924", null ]
      ] ],
      [ "პატერნის არჩევის გზამკვლევი", "md_docs_2convention_2communication__patterns.html#autotoc_md925", [
        [ "Service Locator-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md926", null ],
        [ "Event Bus-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md927", null ]
      ] ],
      [ "Architecture Diagram", "md_docs_2convention_2communication__patterns.html#autotoc_md928", null ],
      [ "შეჯამება", "md_docs_2convention_2communication__patterns.html#autotoc_md929", null ]
    ] ],
    [ "კონფიგურაციის მართვის წესები", "md_docs_2convention_2configuration__management.html", [
      [ "ძირითადი პრინციპი", "md_docs_2convention_2configuration__management.html#autotoc_md931", null ],
      [ "კონფიგურაციის სტრუქტურა და წყაროები", "md_docs_2convention_2configuration__management.html#autotoc_md932", null ],
      [ "კონფიგურაციის წვდომის პატერნები", "md_docs_2convention_2configuration__management.html#autotoc_md933", [
        [ "<tt>fmw_config_get_*</tt> ფუნქციების გამოყენება", "md_docs_2convention_2configuration__management.html#autotoc_md934", null ]
      ] ],
      [ "კონფიგურაციის პარსინგი მოდულის შიგნით", "md_docs_2convention_2configuration__management.html#autotoc_md935", [
        [ "<tt>parse_config</tt> ფუნქცია", "md_docs_2convention_2configuration__management.html#autotoc_md936", null ]
      ] ],
      [ "Default მნიშვნელობები და ვალიდაცია", "md_docs_2convention_2configuration__management.html#autotoc_md937", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2configuration__management.html#autotoc_md938", null ],
      [ "შეჯამება", "md_docs_2convention_2configuration__management.html#autotoc_md939", null ]
    ] ],
    [ "Synapse Convention Reference Index", "md_docs_2convention_2convention__index.html", [
      [ "Coding & Documentation", "md_docs_2convention_2convention__index.html#autotoc_md942", null ],
      [ "Naming & Structure", "md_docs_2convention_2convention__index.html#autotoc_md943", null ],
      [ "Patterns & Management", "md_docs_2convention_2convention__index.html#autotoc_md944", null ]
    ] ],
    [ "Error Handling-ის წესები", "md_docs_2convention_2error__handling.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2error__handling.html#autotoc_md947", [
        [ "1. ESP Error Codes გამოყენება", "md_docs_2convention_2error__handling.html#autotoc_md948", null ],
        [ "2. Comprehensive Error Checking", "md_docs_2convention_2error__handling.html#autotoc_md949", null ]
      ] ],
      [ "Standard Error Codes", "md_docs_2convention_2error__handling.html#autotoc_md950", [
        [ "Framework-Specific Errors", "md_docs_2convention_2error__handling.html#autotoc_md951", null ],
        [ "Common ESP Error Codes", "md_docs_2convention_2error__handling.html#autotoc_md952", null ]
      ] ],
      [ "Error Handling Patterns", "md_docs_2convention_2error__handling.html#autotoc_md953", [
        [ "1. Parameter Validation", "md_docs_2convention_2error__handling.html#autotoc_md954", null ],
        [ "2. Resource Allocation Checking", "md_docs_2convention_2error__handling.html#autotoc_md955", null ],
        [ "3. Service Locator Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md956", null ],
        [ "4. Configuration Parsing Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md957", null ],
        [ "5. Hardware Operation Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md958", null ]
      ] ],
      [ "Cleanup and Resource Management", "md_docs_2convention_2error__handling.html#autotoc_md959", [
        [ "1. Proper Cleanup on Errors", "md_docs_2convention_2error__handling.html#autotoc_md960", null ],
        [ "2. Resource Cleanup Functions", "md_docs_2convention_2error__handling.html#autotoc_md961", null ]
      ] ],
      [ "Logging Standards", "md_docs_2convention_2error__handling.html#autotoc_md962", [
        [ "1. Error Levels", "md_docs_2convention_2error__handling.html#autotoc_md963", null ],
        [ "2. Error Context Information", "md_docs_2convention_2error__handling.html#autotoc_md964", null ]
      ] ],
      [ "Error Recovery Strategies", "md_docs_2convention_2error__handling.html#autotoc_md965", [
        [ "1. Retry Logic", "md_docs_2convention_2error__handling.html#autotoc_md966", null ],
        [ "2. Graceful Degradation", "md_docs_2convention_2error__handling.html#autotoc_md967", null ]
      ] ],
      [ "შეჯამება", "md_docs_2convention_2error__handling.html#autotoc_md968", null ]
    ] ],
    [ "ფუნქციების დასახელების წესები", "md_docs_2convention_2function__naming.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2function__naming.html#autotoc_md970", [
        [ "1. სტრუქტურა", "md_docs_2convention_2function__naming.html#autotoc_md971", null ],
        [ "2. Scope პრეფიქსები", "md_docs_2convention_2function__naming.html#autotoc_md972", [
          [ "Public API ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md973", null ],
          [ "Static/Private ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md974", null ],
          [ "Framework API ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md975", null ]
        ] ]
      ] ],
      [ "მოდულის ტიპების მიხედვით", "md_docs_2convention_2function__naming.html#autotoc_md976", [
        [ "1. Service Locator", "md_docs_2convention_2function__naming.html#autotoc_md977", null ],
        [ "2. Event Bus", "md_docs_2convention_2function__naming.html#autotoc_md978", null ],
        [ "3. Display Modules", "md_docs_2convention_2function__naming.html#autotoc_md979", null ],
        [ "4. Communication Modules", "md_docs_2convention_2function__naming.html#autotoc_md980", null ],
        [ "5. Sensor Modules", "md_docs_2convention_2function__naming.html#autotoc_md981", null ]
      ] ],
      [ "სპეციალური კონვენციები", "md_docs_2convention_2function__naming.html#autotoc_md982", [
        [ "1. Module Lifecycle ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md983", null ],
        [ "2. Runtime Management ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md984", null ],
        [ "3. Event Handler ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md985", null ],
        [ "4. Task ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md986", null ]
      ] ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2function__naming.html#autotoc_md987", null ],
      [ "მაგალითი სრული მოდული", "md_docs_2convention_2function__naming.html#autotoc_md988", null ]
    ] ],
    [ "🚀 მოდულების ინიციალიზაციის დონეები (Initialization Levels)", "md_docs_2convention_2initialization__levels.html", [
      [ "🎯 მიზანი", "md_docs_2convention_2initialization__levels.html#autotoc_md990", null ],
      [ "⚙️ როგორ მუშაობს?", "md_docs_2convention_2initialization__levels.html#autotoc_md991", null ],
      [ "📜 ოქროს წესი", "md_docs_2convention_2initialization__levels.html#autotoc_md992", [
        [ "💡 პრაქტიკული მაგალითი: <tt>wifi_manager</tt> და <tt>command_router</tt>", "md_docs_2convention_2initialization__levels.html#autotoc_md993", null ]
      ] ],
      [ "📊 ინიციალიზაციის დონეების რეკომენდებული დიაპაზონები", "md_docs_2convention_2initialization__levels.html#autotoc_md994", [
        [ "📦 <strong>დონე 1: ფუნდამენტური დრაივერები და რესურსები (0-19)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md996", null ],
        [ "🛠️ <strong>დონე 2: საბაზისო სისტემური სერვისები (20-39)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md998", null ],
        [ "📡 <strong>დონე 3: ქსელი, კონტროლი და კომუნიკაცია (40-59)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1000", null ],
        [ "💡 <strong>დონე 4: აპლიკაციის ლოგიკა და პერიფერია (60-79)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1002", null ],
        [ "🧩 <strong>დონე 5: მაღალი დონის კომპოზიტური მოდულები (80-100)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1004", null ]
      ] ],
      [ "📝 როგორ განვსაზღვროთ <tt>init_level</tt> ახალი მოდულისთვის?", "md_docs_2convention_2initialization__levels.html#autotoc_md1006", null ]
    ] ],
    [ "Logging-ის სტანდარტები", "md_docs_2convention_2logging__standards.html", [
      [ "Component Tag Definition", "md_docs_2convention_2logging__standards.html#autotoc_md1008", [
        [ "სტანდარტული ფორმატი", "md_docs_2convention_2logging__standards.html#autotoc_md1009", null ],
        [ "მაგალითები", "md_docs_2convention_2logging__standards.html#autotoc_md1010", null ]
      ] ],
      [ "Log Levels", "md_docs_2convention_2logging__standards.html#autotoc_md1011", [
        [ "1. ESP_LOGE - Error Level", "md_docs_2convention_2logging__standards.html#autotoc_md1012", null ],
        [ "2. ESP_LOGW - Warning Level", "md_docs_2convention_2logging__standards.html#autotoc_md1013", null ],
        [ "3. ESP_LOGI - Info Level", "md_docs_2convention_2logging__standards.html#autotoc_md1014", null ],
        [ "4. ESP_LOGD - Debug Level", "md_docs_2convention_2logging__standards.html#autotoc_md1015", null ],
        [ "5. ESP_LOGV - Verbose Level", "md_docs_2convention_2logging__standards.html#autotoc_md1016", null ]
      ] ],
      [ "Logging Patterns", "md_docs_2convention_2logging__standards.html#autotoc_md1017", [
        [ "1. Module Lifecycle Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1018", null ],
        [ "2. Service Registration Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1019", null ],
        [ "3. Configuration Parsing Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1020", null ],
        [ "4. Hardware Operation Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1021", null ],
        [ "5. Event Handling Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1022", null ],
        [ "6. Error Context Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1023", null ]
      ] ],
      [ "Runtime State Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1024", [
        [ "1. Module Enable/Disable", "md_docs_2convention_2logging__standards.html#autotoc_md1025", null ],
        [ "2. Task State Changes", "md_docs_2convention_2logging__standards.html#autotoc_md1026", null ]
      ] ],
      [ "Performance Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1027", [
        [ "1. Operation Timing", "md_docs_2convention_2logging__standards.html#autotoc_md1028", null ],
        [ "2. Memory Usage", "md_docs_2convention_2logging__standards.html#autotoc_md1029", null ]
      ] ],
      [ "Log Message Format Standards", "md_docs_2convention_2logging__standards.html#autotoc_md1030", [
        [ "1. Consistent Formatting", "md_docs_2convention_2logging__standards.html#autotoc_md1031", null ],
        [ "2. Parameter Naming", "md_docs_2convention_2logging__standards.html#autotoc_md1032", null ]
      ] ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2logging__standards.html#autotoc_md1033", null ],
      [ "შეჯამება", "md_docs_2convention_2logging__standards.html#autotoc_md1034", null ]
    ] ],
    [ "მოდულის სტრუქტურის წესები", "md_docs_2convention_2module__structure.html", [
      [ "საქაღალდის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1036", [
        [ "სტანდარტული მოდულის layout", "md_docs_2convention_2module__structure.html#autotoc_md1037", null ],
        [ "კატეგორიები", "md_docs_2convention_2module__structure.html#autotoc_md1038", null ]
      ] ],
      [ "ფაილის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1039", [
        [ "1. <tt>config.json</tt> (ახალი სექცია)", "md_docs_2convention_2module__structure.html#autotoc_md1040", null ],
        [ "2. <tt>module.json</tt>", "md_docs_2convention_2module__structure.html#autotoc_md1041", null ],
        [ "3. <tt>src/{module_name}.c</tt> (განახლებული შაბლონი)", "md_docs_2convention_2module__structure.html#autotoc_md1042", null ]
      ] ],
      [ "<tt>CMakeLists.txt</tt> სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1043", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2module__structure.html#autotoc_md1044", null ]
    ] ],
    [ "ცვლადების დასახელების წესები", "md_docs_2convention_2variable__naming.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2variable__naming.html#autotoc_md1047", [
        [ "1. დესკრიპტიული სახელები", "md_docs_2convention_2variable__naming.html#autotoc_md1048", null ],
        [ "2. აკრძალული პრეფიქსები", "md_docs_2convention_2variable__naming.html#autotoc_md1049", null ],
        [ "3. სწორი მიდგომა", "md_docs_2convention_2variable__naming.html#autotoc_md1050", [
          [ "Service Locator", "md_docs_2convention_2variable__naming.html#autotoc_md1051", null ],
          [ "Event Bus", "md_docs_2convention_2variable__naming.html#autotoc_md1052", null ],
          [ "Module Registry", "md_docs_2convention_2variable__naming.html#autotoc_md1053", null ],
          [ "Display Modules", "md_docs_2convention_2variable__naming.html#autotoc_md1054", null ]
        ] ],
        [ "4. კონვენციები", "md_docs_2convention_2variable__naming.html#autotoc_md1055", [
          [ "Global ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1056", null ],
          [ "Static ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1057", null ],
          [ "Mutex/Semaphore ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1058", null ],
          [ "Queue ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1059", null ],
          [ "API Structure ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1060", null ]
        ] ]
      ] ],
      [ "მაგალითები", "md_docs_2convention_2variable__naming.html#autotoc_md1061", [
        [ "Config Manager", "md_docs_2convention_2variable__naming.html#autotoc_md1062", null ],
        [ "I2C Bus Module", "md_docs_2convention_2variable__naming.html#autotoc_md1063", null ],
        [ "MQTT Module", "md_docs_2convention_2variable__naming.html#autotoc_md1064", null ]
      ] ],
      [ "Private Data Pointer Naming", "md_docs_2convention_2variable__naming.html#autotoc_md1065", [
        [ "❌ არასწორი", "md_docs_2convention_2variable__naming.html#autotoc_md1066", null ],
        [ "✅ სწორი", "md_docs_2convention_2variable__naming.html#autotoc_md1067", null ]
      ] ],
      [ "შეჯამება", "md_docs_2convention_2variable__naming.html#autotoc_md1068", null ]
    ] ],
    [ "🚀 CREATE_MODULE.PY - ახალი მოდულის ავტომატური გენერატორი", "md_docs_2CREATE__MODULE__DOCUMENTATION.html", [
      [ "📋 რა არის create_module.py?", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1070", null ],
      [ "🎯 რას აკეთებს?", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1071", [
        [ "✅ შექმნილი ფაილები (5 ფაილი):", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1072", null ],
        [ "✅ ავტომატური ინტეგრაცია:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1073", null ]
      ] ],
      [ "🛠️ გამოყენება", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1074", [
        [ "1. ძირითადი სინტაქსი:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1075", null ],
        [ "2. მაგალითები:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1076", [
          [ "📡 WiFi მოდული:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1077", null ],
          [ "🌡️ ტემპერატურის სენსორი:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1078", null ],
          [ "📺 დისპლეის დრაივერი:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1079", null ],
          [ "💾 მეხსიერების მენეჯერი:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1080", null ]
        ] ],
        [ "3. პარამეტრები:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1081", null ],
        [ "4. ხელმისაწვდომი ტიპები:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1082", null ]
      ] ],
      [ "🏗️ არქიტექტურა და ლოგიკა", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1083", [
        [ "1. 🧠 კონდიციური კომპილაცია (SMART SYSTEM):", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1084", null ],
        [ "2. 📁 შექმნილი სტრუქტურა:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1085", null ],
        [ "3. 🔗 Base Module Interface:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1086", null ]
      ] ],
      [ "🔄 სრული Workflow", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1087", [
        [ "1. მოდულის შექმნა:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1088", null ],
        [ "2. სისტემის reconfigure:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1089", null ],
        [ "3. კონფიგურაცია:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1090", null ],
        [ "4. Build:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1091", null ]
      ] ],
      [ "🎛️ კონფიგურაციის პარამეტრები", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1092", null ],
      [ "🔍 ხელით მორგება", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1093", [
        [ "1. კონფიგურაციის გაფართოება:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1094", null ],
        [ "2. Private დატის გაფართოება:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1095", null ],
        [ "3. ივენთების დამუშავება:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1096", null ]
      ] ],
      [ "🚨 მნიშვნელოვანი შენიშვნები", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1097", [
        [ "✅ რაც უნდა გახსოვდეთ:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1098", null ],
        [ "❌ ჩვეული შეცდომები:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1099", null ]
      ] ],
      [ "🧪 ტესტირების მეთოდები", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1100", [
        [ "1. რომ დარწმუნდეთ მოდული სწორად იქმნება:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1101", null ],
        [ "2. რომ დარწმუნდეთ სისტემა აღმოაჩენს:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1102", null ],
        [ "3. რომ დარწმუნდეთ კონფიგურაცია მუშაობს:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1103", null ],
        [ "4. რომ დარწმუნდეთ build მუშაობს:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1104", null ]
      ] ],
      [ "📊 განსხვავება სხვა სისტემებისგან", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1105", null ],
      [ "🎓 Best Practices", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1106", [
        [ "1. სახელების კონვენცია:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1107", null ],
        [ "2. განვითარების workflow:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1108", null ],
        [ "3. კოდის ხარისხი:", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1109", null ]
      ] ],
      [ "🎯 შეჯამება", "md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1111", null ]
    ] ],
    [ "Synapse ESP Framework — არქიტექტურის მიმოხილვა", "md_docs_2design__decisions_2architecture__overview.html", [
      [ "მიზანი", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1113", null ],
      [ "ძირითადი არქიტექტურული პრინციპები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1114", null ],
      [ "არქიტექტურული ფენები და ძირითადი კომპონენტები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1115", null ],
      [ "მოდულებს შორის კომუნიკაციის პატერნები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1116", null ],
      [ "სიცოცხლის ციკლი", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1117", null ],
      [ "საქაღალდეებისა და ფაილების სტრუქტურა", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1118", null ],
      [ "არქიტექტურული დიაგრამა (მოკლე)", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1119", null ],
      [ "სტანდარტები და დოკუმენტაცია", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1120", null ],
      [ "დასკვნა", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1121", null ]
    ] ],
    [ "კომუნიკაციის არქიტექტურული არჩევანი (Communication Choices)", "md_docs_2design__decisions_2communication__choices.html", [
      [ "მიზანი", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1124", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1126", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1128", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1130", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1132", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1134", [
        [ "Service Locator", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1135", null ],
        [ "Event Bus", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1136", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1138", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1140", null ],
      [ "Event Bus Pattern", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1142", [
        [ "არქიტექტურული ცვლილება: Enum-დან სტრიქონებზე გადასვლა", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1143", null ]
      ] ]
    ] ],
    [ "Core კომპონენტები (Core Components)", "md_docs_2design__decisions_2core__components.html", [
      [ "მიზანი", "md_docs_2design__decisions_2core__components.html#autotoc_md1147", null ],
      [ "ძირითადი კომპონენტები", "md_docs_2design__decisions_2core__components.html#autotoc_md1149", [
        [ "1. Service Locator", "md_docs_2design__decisions_2core__components.html#autotoc_md1150", null ],
        [ "2. Event Bus", "md_docs_2design__decisions_2core__components.html#autotoc_md1151", null ],
        [ "3. Module Register", "md_docs_2design__decisions_2core__components.html#autotoc_md1152", null ],
        [ "4. Module Factory", "md_docs_2design__decisions_2core__components.html#autotoc_md1153", null ],
        [ "5. Configuration Manager", "md_docs_2design__decisions_2core__components.html#autotoc_md1154", null ],
        [ "6. Logging", "md_docs_2design__decisions_2core__components.html#autotoc_md1155", null ]
      ] ],
      [ "ურთიერთქმედება მოდულებთან", "md_docs_2design__decisions_2core__components.html#autotoc_md1157", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2core__components.html#autotoc_md1159", null ]
    ] ],
    [ "არქიტექტურული გადაწყვეტილების ჩანაწერის შაბლონი (Decision Record Template)", "md_docs_2design__decisions_2decision__record__template.html", [
      [ "გადაწყვეტილების ID", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1163", null ],
      [ "თარიღი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1164", null ],
      [ "სტატუსი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1165", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1166", null ],
      [ "გადაწყვეტილება", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1167", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1168", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1169", null ],
      [ "განხორციელების ნაბიჯები", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1170", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1171", null ]
    ] ],
    [ "Synapse Design Decisions", "md_docs_2design__decisions_2design__decisions__index.html", [
      [ "ძირითადი დოკუმენტები", "md_docs_2design__decisions_2design__decisions__index.html#autotoc_md1175", null ]
    ] ],
    [ "შეცდომების დამუშავების არქიტექტურული გადაწყვეტილებები (Error Handling Decisions)", "md_docs_2design__decisions_2error__handling__decisions.html", [
      [ "მიზანი", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1179", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1181", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1183", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1185", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1187", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1189", [
        [ "პარამეტრების ვალიდაცია", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1190", null ],
        [ "რესურსების გამოყოფის შემოწმება", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1191", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1193", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1195", null ]
    ] ],
    [ "გაფართოებადობისა და მოდულარობის არქიტექტურული გადაწყვეტილებები (Extensibility & Modularity)", "md_docs_2design__decisions_2extensibility__and__modularity.html", [
      [ "მიზანი", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1199", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1201", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1203", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1205", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1207", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1209", [
        [ "ახალი მოდულის დამატება", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1210", null ],
        [ "Public API-ის გამოყენება", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1211", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1213", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1215", null ]
    ] ],
    [ "მიგრაციისა და თავსებადობის არქიტექტურული გადაწყვეტილებები (Migration & Compatibility)", "md_docs_2design__decisions_2migration__and__compatibility.html", [
      [ "მიზანი", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1219", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1221", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1223", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1225", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1227", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1229", [
        [ "სქემის ვერსიონირება", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1230", null ],
        [ "Deprecated API-ის მონიშვნა", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1231", null ],
        [ "მიგრაციის სკრიპტის გამოყენება", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1232", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1234", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1236", null ]
    ] ],
    [ "წარმადობისა და რესურსების არქიტექტურული კომპრომისები (Performance Trade-offs)", "md_docs_2design__decisions_2performance__tradeoffs.html", [
      [ "მიზანი", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1240", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1242", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1244", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1246", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1248", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1250", [
        [ "მოდულის გამორთვა ბილდიდან", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1251", null ],
        [ "Static allocation", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1252", null ],
        [ "Lazy initialization", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1253", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1255", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1257", null ]
    ] ],
    [ "უსაფრთხოების არქიტექტურული გადაწყვეტილებები (Security Considerations)", "md_docs_2design__decisions_2security__considerations.html", [
      [ "მიზანი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1261", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1263", null ],
      [ "არჩეული გადაწყვეტილებები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1265", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1267", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1269", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1271", [
        [ "TLS/SSL ჩართვა MQTT-ში", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1272", null ],
        [ "OTA განახლების სიგნირება", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1273", null ],
        [ "ლოგირების კონტროლი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1274", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1276", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1278", null ]
    ] ],
    [ "მონაცემთა შენახვის არქიტექტურული არჩევანი (Storage Strategy)", "md_docs_2design__decisions_2storage__strategy.html", [
      [ "მიზანი", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1282", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1284", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1286", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1288", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1290", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1292", [
        [ "NVS-ში სტრიქონის შენახვა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1293", null ],
        [ "NVS-დან სტრიქონის წაკითხვა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1294", null ],
        [ "Flash-ში მონაცემების შენახვა (OTA)", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1295", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1297", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1299", null ]
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
      [ "მიზანი", "md_docs_2glossary_2glossary__api.html#autotoc_md1303", null ]
    ] ],
    [ "არქიტექტურული ტერმინები (Glossary — Architecture)", "md_docs_2glossary_2glossary__architecture.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__architecture.html#autotoc_md1308", null ]
    ] ],
    [ "კონფიგურაციისა და შენახვის ტერმინები (Glossary — Configuration & Storage)", "md_docs_2glossary_2glossary__config.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__config.html#autotoc_md1313", null ]
    ] ],
    [ "Glossary (ტერმინები)", "md_docs_2glossary_2glossary__index.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__index.html#autotoc_md1318", null ],
      [ "თემატური ტერმინების დოკუმენტები", "md_docs_2glossary_2glossary__index.html#autotoc_md1320", null ],
      [ "გამოყენების წესი", "md_docs_2glossary_2glossary__index.html#autotoc_md1322", null ]
    ] ],
    [ "უსაფრთხოების და წარმადობის ტერმინები (Glossary — Security & Performance)", "md_docs_2glossary_2glossary__security.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__security.html#autotoc_md1326", null ]
    ] ],
    [ "Cloud ინტეგრაციის სახელმძღვანელო (Integration — Cloud)", "md_docs_2integration_2integration__cloud.html", [
      [ "მიზანი", "md_docs_2integration_2integration__cloud.html#autotoc_md1331", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__cloud.html#autotoc_md1333", null ],
      [ "Best Practices", "md_docs_2integration_2integration__cloud.html#autotoc_md1335", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__cloud.html#autotoc_md1337", null ]
    ] ],
    [ "Integration Index (ინტეგრაციის სახელმძღვანელოების ინდექსი)", "md_docs_2integration_2integration__index.html", [
      [ "მიზანი", "md_docs_2integration_2integration__index.html#autotoc_md1341", null ],
      [ "თემატური ინტეგრაციის სახელმძღვანელოები", "md_docs_2integration_2integration__index.html#autotoc_md1343", null ],
      [ "გამოყენების წესი", "md_docs_2integration_2integration__index.html#autotoc_md1345", null ]
    ] ],
    [ "MQTT ინტეგრაციის სახელმძღვანელო (Integration — MQTT)", "md_docs_2integration_2integration__mqtt.html", [
      [ "მიზანი", "md_docs_2integration_2integration__mqtt.html#autotoc_md1349", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__mqtt.html#autotoc_md1351", null ],
      [ "Best Practices", "md_docs_2integration_2integration__mqtt.html#autotoc_md1353", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__mqtt.html#autotoc_md1355", null ]
    ] ],
    [ "REST API ინტეგრაციის სახელმძღვანელო (Integration — REST API)", "md_docs_2integration_2integration__rest.html", [
      [ "მიზანი", "md_docs_2integration_2integration__rest.html#autotoc_md1359", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__rest.html#autotoc_md1361", null ],
      [ "Best Practices", "md_docs_2integration_2integration__rest.html#autotoc_md1363", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__rest.html#autotoc_md1365", null ]
    ] ],
    [ "WebSocket ინტეგრაციის სახელმძღვანელო (Integration — WebSocket)", "md_docs_2integration_2integration__websocket.html", [
      [ "მიზანი", "md_docs_2integration_2integration__websocket.html#autotoc_md1369", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__websocket.html#autotoc_md1371", null ],
      [ "Best Practices", "md_docs_2integration_2integration__websocket.html#autotoc_md1373", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__websocket.html#autotoc_md1375", null ]
    ] ],
    [ "✅ Synapse Framework — Documentation & Module Design Checklist", "md_docs_2internal_2synapse__checklist.html", [
      [ "1. API Naming & Structure", "md_docs_2internal_2synapse__checklist.html#autotoc_md1378", null ],
      [ "2. Module Implementation", "md_docs_2internal_2synapse__checklist.html#autotoc_md1379", null ],
      [ "3. Communication Integration (MQTT/WiFi/ESP-NOW)", "md_docs_2internal_2synapse__checklist.html#autotoc_md1380", null ],
      [ "4. Core Interaction", "md_docs_2internal_2synapse__checklist.html#autotoc_md1381", null ],
      [ "5. Documentation", "md_docs_2internal_2synapse__checklist.html#autotoc_md1382", null ],
      [ "6. Quality & Security", "md_docs_2internal_2synapse__checklist.html#autotoc_md1383", null ],
      [ "7. Testability", "md_docs_2internal_2synapse__checklist.html#autotoc_md1384", null ]
    ] ],
    [ "ბენჩმარკების ჩატარების სახელმძღვანელო (Performance Benchmarks Guide)", "md_docs_2performance_2performance__benchmarks.html", [
      [ "მიზანი", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1387", null ],
      [ "ბენჩმარკის ჩატარების ძირითადი ნაბიჯები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1389", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1391", [
        [ "სენსორის კითხვა", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1392", null ],
        [ "MQTT publish latency", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1393", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1395", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1397", null ]
    ] ],
    [ "საუკეთესო პრაქტიკები წარმადობისთვის (Performance Best Practices)", "md_docs_2performance_2performance__best__practices.html", [
      [ "მიზანი", "md_docs_2performance_2performance__best__practices.html#autotoc_md1401", null ],
      [ "ძირითადი რეკომენდაციები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1403", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1405", null ],
      [ "Best Practices", "md_docs_2performance_2performance__best__practices.html#autotoc_md1407", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1409", null ]
    ] ],
    [ "ეკრანის მოდულების ოპტიმიზაცია (Performance — Display Modules)", "md_docs_2performance_2performance__display.html", [
      [ "მიზანი", "md_docs_2performance_2performance__display.html#autotoc_md1413", null ],
      [ "ოპტიმიზაციის რეკომენდაციები", "md_docs_2performance_2performance__display.html#autotoc_md1415", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__display.html#autotoc_md1417", [
        [ "Refresh interval-ის ოპტიმიზაცია", "md_docs_2performance_2performance__display.html#autotoc_md1418", null ],
        [ "Conditional redraw", "md_docs_2performance_2performance__display.html#autotoc_md1419", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__display.html#autotoc_md1421", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__display.html#autotoc_md1423", null ]
    ] ],
    [ "Performance (წარმადობის სახელმძღვანელო)", "md_docs_2performance_2performance__index.html", [
      [ "მიზანი", "md_docs_2performance_2performance__index.html#autotoc_md1427", null ],
      [ "თემატური performance სახელმძღვანელოები", "md_docs_2performance_2performance__index.html#autotoc_md1429", null ],
      [ "გამოყენების წესი", "md_docs_2performance_2performance__index.html#autotoc_md1431", null ]
    ] ],
    [ "ოპტიმიზაციის სახელმძღვანელოები (Performance Optimization Guide)", "md_docs_2performance_2performance__optimization.html", [
      [ "მიზანი", "md_docs_2performance_2performance__optimization.html#autotoc_md1435", null ],
      [ "კოდის ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1437", null ],
      [ "კონფიგურაციის ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1439", null ],
      [ "მოდულების ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1441", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__optimization.html#autotoc_md1443", [
        [ "Static allocation", "md_docs_2performance_2performance__optimization.html#autotoc_md1444", null ],
        [ "Lazy initialization", "md_docs_2performance_2performance__optimization.html#autotoc_md1445", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__optimization.html#autotoc_md1447", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__optimization.html#autotoc_md1449", null ]
    ] ],
    [ "რესურსების გამოყენების სახელმძღვანელო (Performance Resource Usage Guide)", "md_docs_2performance_2performance__resource__usage.html", [
      [ "მიზანი", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1453", null ],
      [ "RAM-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1455", null ],
      [ "Flash-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1456", null ],
      [ "CPU-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1457", null ],
      [ "Stack-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1458", null ],
      [ "მონიტორინგის მაგალითები", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1460", [
        [ "RAM", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1461", null ],
        [ "Stack", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1462", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1464", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1466", null ]
    ] ],
    [ "🚀 <tt>create_module.py</tt>: Synapse მოდულის გენერატორი (v3.0)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html", [
      [ "1. 📋 რა არის <tt>create_module.py</tt>?", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1469", null ],
      [ "2. 🎯 რას აკეთებს?", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1470", [
        [ "✅ გენერირებული ფაილები (7 ფაილი)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1471", null ],
        [ "✅ ავტომატური ინტეგრაცია", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1472", null ]
      ] ],
      [ "3. 🛠️ გამოყენების რეჟიმები", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1473", [
        [ "3.1. ინტერაქტიული რეჟიმი (რეკომენდებული)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1474", null ],
        [ "3.2. ბრძანების ხაზის რეჟიმი (ავტომატიზაციისთვის)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1475", null ]
      ] ],
      [ "4. 🏛️ არქეტიპები (Archetypes)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1476", null ],
      [ "5. 🔄 სრული სამუშაო პროცესი (Workflow)", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1477", null ],
      [ "6. 🚨 მნიშვნელოვანი შენიშვნები", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1478", null ],
      [ "🎯 შეჯამება", "md_docs_2scripts_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1480", null ]
    ] ],
    [ "უსაფრთხოების საუკეთესო პრაქტიკები (Security Best Practices)", "md_docs_2security_2security__best__practices.html", [
      [ "მიზანი", "md_docs_2security_2security__best__practices.html#autotoc_md1483", null ],
      [ "ძირითადი რეკომენდაციები", "md_docs_2security_2security__best__practices.html#autotoc_md1485", null ],
      [ "მაგალითები", "md_docs_2security_2security__best__practices.html#autotoc_md1487", [
        [ "TLS/SSL ჩართვა MQTT-ში", "md_docs_2security_2security__best__practices.html#autotoc_md1488", null ],
        [ "OTA განახლების სიგნირება", "md_docs_2security_2security__best__practices.html#autotoc_md1489", null ],
        [ "ლოგირების კონტროლი", "md_docs_2security_2security__best__practices.html#autotoc_md1490", null ]
      ] ],
      [ "Best Practices", "md_docs_2security_2security__best__practices.html#autotoc_md1492", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__best__practices.html#autotoc_md1494", null ]
    ] ],
    [ "უსაფრთხოების შემოწმების სია (Security Checklist)", "md_docs_2security_2security__checklist.html", [
      [ "მიზანი", "md_docs_2security_2security__checklist.html#autotoc_md1498", null ],
      [ "Release Checklist", "md_docs_2security_2security__checklist.html#autotoc_md1500", null ],
      [ "Code Review Checklist", "md_docs_2security_2security__checklist.html#autotoc_md1501", null ],
      [ "Best Practices", "md_docs_2security_2security__checklist.html#autotoc_md1503", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__checklist.html#autotoc_md1505", null ]
    ] ],
    [ "უსაფრთხოების სახელმძღვანელოები (Security Guidelines)", "md_docs_2security_2security__guidelines.html", [
      [ "მიზანი", "md_docs_2security_2security__guidelines.html#autotoc_md1509", null ],
      [ "TLS/SSL", "md_docs_2security_2security__guidelines.html#autotoc_md1511", null ],
      [ "Authentication & Authorization", "md_docs_2security_2security__guidelines.html#autotoc_md1512", null ],
      [ "Input Validation", "md_docs_2security_2security__guidelines.html#autotoc_md1513", null ],
      [ "Credentials Management", "md_docs_2security_2security__guidelines.html#autotoc_md1514", null ],
      [ "OTA და Firmware Security", "md_docs_2security_2security__guidelines.html#autotoc_md1515", null ],
      [ "Logging", "md_docs_2security_2security__guidelines.html#autotoc_md1516", null ],
      [ "Best Practices", "md_docs_2security_2security__guidelines.html#autotoc_md1518", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__guidelines.html#autotoc_md1520", null ]
    ] ],
    [ "Security Index (უსაფრთხოების სახელმძღვანელოების ინდექსი)", "md_docs_2security_2security__index.html", [
      [ "მიზანი", "md_docs_2security_2security__index.html#autotoc_md1524", null ],
      [ "თემატური უსაფრთხოების სახელმძღვანელოები", "md_docs_2security_2security__index.html#autotoc_md1526", null ],
      [ "გამოყენების წესი", "md_docs_2security_2security__index.html#autotoc_md1528", null ]
    ] ],
    [ "Vulnerability Disclosure Policy (უსაფრთხოების ხარვეზების აღმოჩენისა და შეტყობინების პროცედურა)", "md_docs_2security_2security__vulnerability__disclosure.html", [
      [ "მიზანი", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1532", null ],
      [ "ხარვეზის აღმოჩენა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1534", null ],
      [ "შეტყობინების პროცედურა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1535", null ],
      [ "რეაგირება", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1536", null ],
      [ "პასუხისმგებლობა და ეთიკა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1537", null ],
      [ "საკონტაქტო ინფორმაცია", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1539", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1541", null ]
    ] ],
    [ "კომუნიკაციის პატერნები (Communication Patterns)", "md_docs_2structure_2communication__patterns.html", [
      [ "მიზანი", "md_docs_2structure_2communication__patterns.html#autotoc_md1544", null ],
      [ "ძირითადი პატერნები", "md_docs_2structure_2communication__patterns.html#autotoc_md1545", [
        [ "1. Service Locator Pattern", "md_docs_2structure_2communication__patterns.html#autotoc_md1546", null ],
        [ "2. Event Bus Pattern", "md_docs_2structure_2communication__patterns.html#autotoc_md1547", null ]
      ] ],
      [ "სწორი გამოყენების წესები", "md_docs_2structure_2communication__patterns.html#autotoc_md1548", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2structure_2communication__patterns.html#autotoc_md1549", null ],
      [ "მაგალითები", "md_docs_2structure_2communication__patterns.html#autotoc_md1550", [
        [ "Service Locator", "md_docs_2structure_2communication__patterns.html#autotoc_md1551", null ],
        [ "Event Bus", "md_docs_2structure_2communication__patterns.html#autotoc_md1552", null ]
      ] ]
    ] ],
    [ "კონფიგურაციის მართვის პროცესი (Configuration Flow)", "md_docs_2structure_2configuration__flow.html", [
      [ "მიზანი", "md_docs_2structure_2configuration__flow.html#autotoc_md1555", null ],
      [ "კონფიგურაციის წყაროები", "md_docs_2structure_2configuration__flow.html#autotoc_md1556", null ],
      [ "კონფიგურაციის სტრუქტურა", "md_docs_2structure_2configuration__flow.html#autotoc_md1557", null ],
      [ "კონფიგურაციის დამუშავების ეტაპები", "md_docs_2structure_2configuration__flow.html#autotoc_md1558", null ],
      [ "კონფიგურაციის წვდომის მაგალითები", "md_docs_2structure_2configuration__flow.html#autotoc_md1559", null ],
      [ "ვალიდაცია და Default მნიშვნელობები", "md_docs_2structure_2configuration__flow.html#autotoc_md1560", null ],
      [ "Runtime Reconfiguration", "md_docs_2structure_2configuration__flow.html#autotoc_md1561", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2configuration__flow.html#autotoc_md1562", null ]
    ] ],
    [ "Core კომპონენტები (Core Components)", "md_docs_2structure_2core__components.html", [
      [ "დანიშნულება", "md_docs_2structure_2core__components.html#autotoc_md1565", null ],
      [ "ძირითადი კომპონენტები", "md_docs_2structure_2core__components.html#autotoc_md1566", [
        [ "1. Service Locator", "md_docs_2structure_2core__components.html#autotoc_md1567", null ],
        [ "2. Event Bus", "md_docs_2structure_2core__components.html#autotoc_md1568", null ],
        [ "3. Module Register", "md_docs_2structure_2core__components.html#autotoc_md1569", null ],
        [ "4. Module Factory (ყოფილი Fabrique)", "md_docs_2structure_2core__components.html#autotoc_md1570", null ],
        [ "5. Configuration Manager", "md_docs_2structure_2core__components.html#autotoc_md1571", null ],
        [ "6. Logging", "md_docs_2structure_2core__components.html#autotoc_md1572", null ]
      ] ],
      [ "ურთიერთქმედება მოდულებთან", "md_docs_2structure_2core__components.html#autotoc_md1573", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2core__components.html#autotoc_md1574", null ]
    ] ],
    [ "საქაღალდეებისა და ფაილების სტრუქტურა (Directory Structure)", "md_docs_2structure_2directory__structure.html", [
      [ "ძირითადი სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1577", null ],
      [ "ძირითადი საქაღალდეების აღწერა", "md_docs_2structure_2directory__structure.html#autotoc_md1578", null ],
      [ "<tt>modules</tt> საქაღალდის სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1579", null ],
      [ "თითოეული მოდულის სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1580", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2directory__structure.html#autotoc_md1581", null ]
    ] ],
    [ "შეცდომების დამუშავება და ლოგირების სტანდარტები (Error Handling & Logging)", "md_docs_2structure_2error__handling__and__logging.html", [
      [ "მიზანი", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1584", null ],
      [ "შეცდომების დამუშავების პრინციპები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1585", null ],
      [ "შეცდომების კოდები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1586", null ],
      [ "შეცდომების დამუშავების მაგალითები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1587", null ],
      [ "ლოგირების სტანდარტები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1588", null ],
      [ "ლოგირების მაგალითები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1589", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1590", null ]
    ] ],
    [ "ფრეიმვორქის გაფართოება და Best Practices (Extending the Framework)", "md_docs_2structure_2extending__framework.html", [
      [ "მიზანი", "md_docs_2structure_2extending__framework.html#autotoc_md1593", null ],
      [ "ახალი მოდულის დამატების პროცესი", "md_docs_2structure_2extending__framework.html#autotoc_md1594", null ],
      [ "Best Practices", "md_docs_2structure_2extending__framework.html#autotoc_md1595", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2structure_2extending__framework.html#autotoc_md1596", null ],
      [ "მაგალითი ახალი მოდულის დამატების", "md_docs_2structure_2extending__framework.html#autotoc_md1597", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (FAQ)", "md_docs_2structure_2faq.html", [
      [ "1. როგორ დავამატო ახალი მოდული?", "md_docs_2structure_2faq.html#autotoc_md1600", null ],
      [ "2. შეიძლება თუ არა ერთი და იმავე ტიპის რამდენიმე მოდულის დამატება?", "md_docs_2structure_2faq.html#autotoc_md1601", null ],
      [ "3. როგორ ხდება მოდულებს შორის კომუნიკაცია?", "md_docs_2structure_2faq.html#autotoc_md1602", null ],
      [ "4. როგორ მივიღო ან შევცვალო მოდულის კონფიგურაცია?", "md_docs_2structure_2faq.html#autotoc_md1603", null ],
      [ "5. როგორ დავამატო ახალი გლობალური პარამეტრი?", "md_docs_2structure_2faq.html#autotoc_md1604", null ],
      [ "6. როგორ დავიცვა naming და structure სტანდარტები?", "md_docs_2structure_2faq.html#autotoc_md1605", null ],
      [ "7. როგორ ხდება შეცდომების დამუშავება და ლოგირება?", "md_docs_2structure_2faq.html#autotoc_md1606", null ],
      [ "8. როგორ დავამატო ახალი კატეგორია მოდულებისთვის?", "md_docs_2structure_2faq.html#autotoc_md1607", null ],
      [ "9. როგორ დავამატო დოკუმენტაცია ან განვაახლო არსებული?", "md_docs_2structure_2faq.html#autotoc_md1608", null ],
      [ "10. როგორ დავიწყო პროექტის გამოყენება?", "md_docs_2structure_2faq.html#autotoc_md1609", null ]
    ] ],
    [ "მოდულის სიცოცხლის ციკლი და ინიციალიზაცია (Lifecycle & Initialization)", "md_docs_2structure_2lifecycle__and__init.html", [
      [ "მიზანი", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1612", null ],
      [ "სიცოცხლის ციკლის ეტაპები", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1613", null ],
      [ "დეტალური აღწერა", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1614", [
        [ "1. Create", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1615", null ],
        [ "2. Init", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1616", null ],
        [ "3. Enable/Disable", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1617", null ],
        [ "4. Deinit", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1618", null ]
      ] ],
      [ "სიცოცხლის ციკლის დიაგრამა", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1619", null ],
      [ "მაგალითი მოდულის სიცოცხლის ციკლის მართვის", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1620", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1621", null ]
    ] ],
    [ "მოდულების არქიტექტურა (Module Architecture)", "md_docs_2structure_2module__architecture.html", [
      [ "ძირითადი პრინციპები", "md_docs_2structure_2module__architecture.html#autotoc_md1624", null ],
      [ "მოდულის ტიპები", "md_docs_2structure_2module__architecture.html#autotoc_md1625", null ],
      [ "მოდულების ურთიერთქმედება", "md_docs_2structure_2module__architecture.html#autotoc_md1626", null ],
      [ "მოდულის სიცოცხლის ციკლი (Lifecycle)", "md_docs_2structure_2module__architecture.html#autotoc_md1627", null ],
      [ "მოდულის სტრუქტურის მაგალითი", "md_docs_2structure_2module__architecture.html#autotoc_md1628", [
        [ "<tt>_create</tt> ფუნქციის კონტრაქტი", "md_docs_2structure_2module__architecture.html#autotoc_md1629", null ]
      ] ],
      [ "მოდულის გაფართოება/დამატება", "md_docs_2structure_2module__architecture.html#autotoc_md1630", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2module__architecture.html#autotoc_md1631", null ]
    ] ],
    [ "Advanced Diagnostics (გაფართოებული დიაგნოსტიკა)", "md_docs_2troubleshooting_2advanced__diagnostics.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1642", null ],
      [ "1. Stack Overflow და Task Watchdog", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1644", null ],
      [ "2. Heap Corruption და Memory Leak", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1645", null ],
      [ "3. Core Dumps და Crash Reporting", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1646", null ],
      [ "4. Custom Health Checks", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1647", null ],
      [ "5. Peripheral Diagnostics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1648", null ],
      [ "6. Network Diagnostics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1649", null ],
      [ "7. Performance Metrics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1650", null ]
    ] ],
    [ "გავრცელებული პრობლემები და გადაჭრის გზები (Common Issues & Solutions)", "md_docs_2troubleshooting_2common__issues.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1653", null ],
      [ "1. ბილდის შეცდომები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1655", [
        [ "პრობლემა: მოდული არ იბილდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1656", null ],
        [ "პრობლემა: include file not found", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1657", null ]
      ] ],
      [ "2. კონფიგურაციის პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1659", [
        [ "პრობლემა: მოდული არ ინიციალიზდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1660", null ],
        [ "პრობლემა: default მნიშვნელობები არ მუშაობს", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1661", null ]
      ] ],
      [ "3. კომუნიკაციის პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1663", [
        [ "პრობლემა: Event Bus-ზე მოვლენები არ ვრცელდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1664", null ],
        [ "პრობლემა: Service Locator-ით API არ იძებნება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1665", null ]
      ] ],
      [ "4. ლოგირების და შეცდომების დიაგნოსტიკა", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1667", [
        [ "პრობლემა: ლოგები არ ჩანს", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1668", null ],
        [ "პრობლემა: შეცდომის კოდი არ იკითხება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1669", null ]
      ] ],
      [ "5. Runtime პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1671", [
        [ "პრობლემა: მოდული არ რეაგირებს enable/disable-ზე", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1672", null ],
        [ "პრობლემა: რესურსების გაჟონვა (memory leak)", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1673", null ]
      ] ],
      [ "6. დამატებითი რჩევები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1675", null ]
    ] ],
    [ "Debugging Guide (დიბაგინგის სახელმძღვანელო)", "md_docs_2troubleshooting_2debugging__guide.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1678", null ],
      [ "1. ლოგირების ჩართვა და გამოყენება", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1680", null ],
      [ "2. Debug Build-ის კონფიგურაცია", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1681", null ],
      [ "3. GDB და Breakpoint-ების გამოყენება", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1682", null ],
      [ "4. Task და Stack Usage მონიტორინგი", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1683", null ],
      [ "5. Heap და Memory Usage", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1684", null ],
      [ "6. Event და Service Debugging", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1685", null ],
      [ "7. Assertion-ები და Error Checking", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1686", null ],
      [ "8. დამატებითი ინსტრუმენტები", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1687", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (Troubleshooting FAQ)", "md_docs_2troubleshooting_2faq.html", [
      [ "1. რატომ არ იბილდება ჩემი მოდული?", "md_docs_2troubleshooting_2faq.html#autotoc_md1690", null ],
      [ "2. რატომ არ ჩანს ლოგები?", "md_docs_2troubleshooting_2faq.html#autotoc_md1691", null ],
      [ "3. რატომ ვერ პოულობს Service Locator-ი ჩემს სერვისს?", "md_docs_2troubleshooting_2faq.html#autotoc_md1692", null ],
      [ "4. რატომ არ ვრცელდება Event Bus-ზე მოვლენები?", "md_docs_2troubleshooting_2faq.html#autotoc_md1693", null ],
      [ "5. რატომ არ ინიციალიზდება მოდული?", "md_docs_2troubleshooting_2faq.html#autotoc_md1694", null ],
      [ "6. როგორ გავიგო რომელი მოდული იწვევს პრობლემას?", "md_docs_2troubleshooting_2faq.html#autotoc_md1695", null ],
      [ "7. როგორ გამოვასწორო რესურსების გაჟონვა (memory leak)?", "md_docs_2troubleshooting_2faq.html#autotoc_md1696", null ],
      [ "8. როგორ დავადგინო კონფიგურაციის შეცდომა?", "md_docs_2troubleshooting_2faq.html#autotoc_md1697", null ],
      [ "9. როგორ დავამატო ახალი კითხვა ამ FAQ-ში?", "md_docs_2troubleshooting_2faq.html#autotoc_md1698", null ]
    ] ],
    [ "Advanced Usage (გაფართოებული გამოყენება)", "md_docs_2tutorials_2advanced__usage.html", [
      [ "მიზანი", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1701", null ],
      [ "1. მრავალმოდულიანი კონფიგურაცია", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1702", null ],
      [ "2. მოდულის დინამიური ჩართვა/გამორთვა", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1703", null ],
      [ "3. Runtime Reconfiguration", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1704", null ],
      [ "4. Custom Event-ების გამოყენება", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1705", null ],
      [ "5. მოდულის სტატუსის მონიტორინგი", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1706", null ],
      [ "6. ოპტიმიზაცია და რესურსების მართვა", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1707", null ],
      [ "7. Best Practices", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1708", null ]
    ] ],
    [ "პირველი საკუთარი მოდულის შექმნა (Create Your First Module)", "md_docs_2tutorials_2create__first__module.html", [
      [ "მიზანი", "md_docs_2tutorials_2create__first__module.html#autotoc_md1711", null ],
      [ "1. საჭიროების განსაზღვრა", "md_docs_2tutorials_2create__first__module.html#autotoc_md1712", null ],
      [ "2. მოდულის ჩონჩხის გენერაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1713", null ],
      [ "3. კონფიგურაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1714", null ],
      [ "4. კოდის იმპლემენტაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1715", null ],
      [ "5. ტესტირება", "md_docs_2tutorials_2create__first__module.html#autotoc_md1716", null ]
    ] ],
    [ "სწრაფი სტარტი (Getting Started)", "md_docs_2tutorials_2getting__started.html", [
      [ "მიზანი", "md_docs_2tutorials_2getting__started.html#autotoc_md1719", null ],
      [ "1. გარემოს მომზადება", "md_docs_2tutorials_2getting__started.html#autotoc_md1720", null ],
      [ "2. პროექტის სტრუქტურის გაცნობა", "md_docs_2tutorials_2getting__started.html#autotoc_md1721", null ],
      [ "3. პირველი მოდულის ჩართვა", "md_docs_2tutorials_2getting__started.html#autotoc_md1722", null ],
      [ "4. ბილდი და ფლეში", "md_docs_2tutorials_2getting__started.html#autotoc_md1723", null ],
      [ "5. ლოგების ნახვა და ტესტირება", "md_docs_2tutorials_2getting__started.html#autotoc_md1724", null ],
      [ "6. დამატებითი რესურსები", "md_docs_2tutorials_2getting__started.html#autotoc_md1725", null ]
    ] ],
    [ "ინტეგრაციის მაგალითი (Integration Example)", "md_docs_2tutorials_2integration__example.html", [
      [ "მიზანი", "md_docs_2tutorials_2integration__example.html#autotoc_md1728", null ],
      [ "1. ინტეგრაციის საჭიროების განსაზღვრა", "md_docs_2tutorials_2integration__example.html#autotoc_md1729", null ],
      [ "2. საჭირო მოდულების ჩართვა და კონფიგურაცია", "md_docs_2tutorials_2integration__example.html#autotoc_md1730", null ],
      [ "3. MQTT Integration (Publish/Subscribe)", "md_docs_2tutorials_2integration__example.html#autotoc_md1731", null ],
      [ "4. REST API Integration", "md_docs_2tutorials_2integration__example.html#autotoc_md1732", null ],
      [ "5. WebSocket Integration", "md_docs_2tutorials_2integration__example.html#autotoc_md1733", null ],
      [ "6. Best Practices", "md_docs_2tutorials_2integration__example.html#autotoc_md1734", null ],
      [ "7. Troubleshooting", "md_docs_2tutorials_2integration__example.html#autotoc_md1735", null ]
    ] ],
    [ "მოდულის გაფართოება ან ფუნქციონალის დამატება (Module Extension & Feature Addition)", "md_docs_2tutorials_2module__extension.html", [
      [ "მიზანი", "md_docs_2tutorials_2module__extension.html#autotoc_md1738", null ],
      [ "1. საჭიროების განსაზღვრა", "md_docs_2tutorials_2module__extension.html#autotoc_md1739", null ],
      [ "2. სტრუქტურის დაცვა", "md_docs_2tutorials_2module__extension.html#autotoc_md1740", null ],
      [ "3. ახალი API ან ფუნქციის დამატება", "md_docs_2tutorials_2module__extension.html#autotoc_md1741", null ],
      [ "4. კონფიგურაციის გაფართოება", "md_docs_2tutorials_2module__extension.html#autotoc_md1742", null ],
      [ "5. კომუნიკაციის გაფართოება", "md_docs_2tutorials_2module__extension.html#autotoc_md1743", null ],
      [ "6. ტესტირება და ლოგირება", "md_docs_2tutorials_2module__extension.html#autotoc_md1744", null ],
      [ "7. დოკუმენტაციის განახლება", "md_docs_2tutorials_2module__extension.html#autotoc_md1745", null ],
      [ "Best Practices", "md_docs_2tutorials_2module__extension.html#autotoc_md1746", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2tutorials_2module__extension.html#autotoc_md1747", null ]
    ] ],
    [ "სისტემის კონფიგურაცია (System Configuration)", "md_docs_2tutorials_2system__configuration.html", [
      [ "🎯 მიზანი", "md_docs_2tutorials_2system__configuration.html#autotoc_md1750", null ],
      [ "1. კონფიგურაციის ფაილების როლები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1752", null ],
      [ "2. <tt>config.json</tt>-ის სტრუქტურა და მაგალითები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1754", null ],
      [ "3. მოდულის კონფიგურაციის ობიექტის აღწერა", "md_docs_2tutorials_2system__configuration.html#autotoc_md1756", null ],
      [ "4. პარამეტრებზე წვდომა კოდიდან (Dot-Notation)", "md_docs_2tutorials_2system__configuration.html#autotoc_md1758", null ],
      [ "5. კონფიგურაციის ვალიდაცია და Default-ები", "md_docs_2tutorials_2system__configuration.html#autotoc_md1760", null ],
      [ "6. საუკეთესო პრაქტიკები (Best Practices)", "md_docs_2tutorials_2system__configuration.html#autotoc_md1762", null ]
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
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", null ],
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
"index.html",
"md_docs_2CREATE__MODULE__DOCUMENTATION.html#autotoc_md1070",
"md_docs_2api__reference_2provisioning__api.html#autotoc_md509",
"md_docs_2contributing_2review__guidelines.html#autotoc_md882",
"md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1232",
"md_docs_2structure_2lifecycle__and__init.html#autotoc_md1619",
"service__types_8h.html#a2147d7b6aec6913e52ceb12330fc219ca22ee9ed28d32a68ad180c3626a3f1003",
"structsensor__simulator__private__data__t.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';