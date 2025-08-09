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
      [ "4. 🔌 Service API (<tt>wifi_api_t</tt>)", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md23", [
        [ "<tt>Promise</tt>-ზე დაფუძნებული სტატუსის მოთხოვნა", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md24", null ]
      ] ],
      [ "5. 📢 ივენთები (Events)", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md25", [
        [ "გამოქვეყნებული ივენთები", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md26", null ]
      ] ],
      [ "6. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2communications_2wifi__manager_2README.html#autotoc_md27", null ]
    ] ],
    [ "🩺 მოდული: <tt>self_test_manager</tt>", "md_components_2modules_2diagnostics_2self__test__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md29", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md30", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md31", null ],
      [ "4. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md32", null ],
      [ "5. 📡 MQTT ინტერფეისი", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md33", null ],
      [ "6. 💡 ახალი ტესტის დამატების მაგალითი", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md34", null ],
      [ "7. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2diagnostics_2self__test__manager_2README.html#autotoc_md35", null ]
    ] ],
    [ "📺 მოდული: <tt>ssd1306_driver</tt>", "md_components_2modules_2displays_2ssd1306__driver_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2displays_2ssd1306__driver_2README.html#autotoc_md37", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2displays_2ssd1306__driver_2README.html#autotoc_md38", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2displays_2ssd1306__driver_2README.html#autotoc_md39", null ],
      [ "4. 🔌 Service API", "md_components_2modules_2displays_2ssd1306__driver_2README.html#autotoc_md40", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2displays_2ssd1306__driver_2README.html#autotoc_md41", null ]
    ] ],
    [ "🔘 მოდული: <tt>button_input</tt>", "md_components_2modules_2drivers_2button__input_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md43", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md44", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md45", [
        [ "3.1. კონფიგურაცია <tt>GPIO</tt> რეჟიმისთვის", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md46", null ],
        [ "3.2. კონფიგურაცია <tt>Expander</tt> რეჟიმისთვის", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md47", null ],
        [ "3.3. კონფიგურაციის პარამეტრები", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md48", null ]
      ] ],
      [ "4. 📢 გამოქვეყნებული ივენთი", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md49", null ],
      [ "5. 🔌 მიერთების სქემა", "md_components_2modules_2drivers_2button__input_2README.html#autotoc_md50", null ]
    ] ],
    [ "🚌 მოდული: <tt>i2c_bus</tt>", "md_components_2modules_2drivers_2i2c__bus_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md52", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md53", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md54", null ],
      [ "4. 🔌 Service API (<tt>i2c_bus_api_t</tt>)", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md55", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md56", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2drivers_2i2c__bus_2README.html#autotoc_md57", null ]
    ] ],
    [ "🔄 მოდული: <tt>mcp23017_io_expander</tt>", "md_components_2modules_2drivers_2mcp23017__io__expander_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2drivers_2mcp23017__io__expander_2README.html#autotoc_md59", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2drivers_2mcp23017__io__expander_2README.html#autotoc_md60", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2drivers_2mcp23017__io__expander_2README.html#autotoc_md61", null ],
      [ "4. 🔌 Service API (<tt>mcp23017_api_t</tt>)", "md_components_2modules_2drivers_2mcp23017__io__expander_2README.html#autotoc_md62", null ],
      [ "5. 📌 პინების ნუმერაცია და შეერთების სქემა", "md_components_2modules_2drivers_2mcp23017__io__expander_2README.html#autotoc_md63", [
        [ "5.1. ლოგიკური და ფიზიკური პინები", "md_components_2modules_2drivers_2mcp23017__io__expander_2README.html#autotoc_md64", null ],
        [ "5.2. ASCII Pinout დიაგრამა (DIP-28 პაკეტი)", "md_components_2modules_2drivers_2mcp23017__io__expander_2README.html#autotoc_md65", null ],
        [ "5.3. შეერთების სქემა", "md_components_2modules_2drivers_2mcp23017__io__expander_2README.html#autotoc_md66", null ]
      ] ],
      [ "6. 💡 გამოყენების მაგალითი", "md_components_2modules_2drivers_2mcp23017__io__expander_2README.html#autotoc_md67", null ]
    ] ],
    [ "🔘 მოდული: <tt>rotary_encoder_input</tt>", "md_components_2modules_2drivers_2rotary__encoder__input_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2drivers_2rotary__encoder__input_2README.html#autotoc_md69", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2drivers_2rotary__encoder__input_2README.html#autotoc_md70", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2drivers_2rotary__encoder__input_2README.html#autotoc_md71", [
        [ "3.1. კონფიგურაცია <tt>GPIO</tt> რეჟიმისთვის", "md_components_2modules_2drivers_2rotary__encoder__input_2README.html#autotoc_md72", null ],
        [ "3.2. კონფიგურაცია <tt>Expander</tt> რეჟიმისთვის", "md_components_2modules_2drivers_2rotary__encoder__input_2README.html#autotoc_md73", null ],
        [ "3.3. კონფიგურაციის პარამეტრები", "md_components_2modules_2drivers_2rotary__encoder__input_2README.html#autotoc_md74", null ]
      ] ],
      [ "4. 📢 გამოქვეყნებული ივენთები", "md_components_2modules_2drivers_2rotary__encoder__input_2README.html#autotoc_md75", null ],
      [ "5. 🔌 მიერთების სქემა", "md_components_2modules_2drivers_2rotary__encoder__input_2README.html#autotoc_md76", null ],
      [ "6. 💡 გამოყენების მაგალითი (<tt>ui_manager</tt>-თან)", "md_components_2modules_2drivers_2rotary__encoder__input_2README.html#autotoc_md77", null ]
    ] ],
    [ "💾 მოდული: <tt>sn74hc595n_writer</tt>", "md_components_2modules_2drivers_2sn74hc595n__writer_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2drivers_2sn74hc595n__writer_2README.html#autotoc_md79", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2drivers_2sn74hc595n__writer_2README.html#autotoc_md80", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2drivers_2sn74hc595n__writer_2README.html#autotoc_md81", null ],
      [ "4. 🔌 Service API (<tt>sn74hc595n_writer_api_t</tt>)", "md_components_2modules_2drivers_2sn74hc595n__writer_2README.html#autotoc_md82", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2drivers_2sn74hc595n__writer_2README.html#autotoc_md83", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2drivers_2sn74hc595n__writer_2README.html#autotoc_md84", null ]
    ] ],
    [ "🚌 მოდული: <tt>spi_bus</tt>", "md_components_2modules_2drivers_2spi__bus_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2drivers_2spi__bus_2README.html#autotoc_md86", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2drivers_2spi__bus_2README.html#autotoc_md87", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2drivers_2spi__bus_2README.html#autotoc_md88", null ],
      [ "4. 🔌 Service API (<tt>spi_bus_api_t</tt>)", "md_components_2modules_2drivers_2spi__bus_2README.html#autotoc_md89", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2drivers_2spi__bus_2README.html#autotoc_md90", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2drivers_2spi__bus_2README.html#autotoc_md91", null ]
    ] ],
    [ "🛡️ მოდული: <tt>security_status_reporter</tt>", "md_components_2modules_2security_2security__status__reporter_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md93", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md94", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md95", null ],
      [ "4. 🔌 Service API (<tt>security_status_api_t</tt>)", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md96", null ],
      [ "5. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md97", null ],
      [ "6. 💡 გამოყენება და უსაფრთხოების ჩართვა", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md98", null ],
      [ "7. 🤝 ინტეგრაცია <tt>self_test_manager</tt>-თან", "md_components_2modules_2security_2security__status__reporter_2README.html#autotoc_md99", null ]
    ] ],
    [ "🌡️ მოდული: <tt>sensor_aggregator</tt>", "md_components_2modules_2sensors_2sensor__aggregator_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md101", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md102", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>system_config.json</tt>)", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md103", [
        [ "3.1. ძირითადი კონფიგურაციის პარამეტრები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md104", null ],
        [ "3.2. <tt>sensors_to_aggregate</tt> ობიექტის პარამეტრები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md105", [
          [ "**<tt>aggregation_mode</tt>-ის შესაძლო მნიშვნელობები:**", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md106", null ]
        ] ]
      ] ],
      [ "4. 📢 ივენთების მოდელი", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md107", null ],
      [ "5. 💡 გამოყენების მაგალითი და ტესტირება", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md108", null ],
      [ "6. 🚀 სამომავლო განვითარების პერსპექტივები", "md_components_2modules_2sensors_2sensor__aggregator_2README.html#autotoc_md109", null ]
    ] ],
    [ "🔬 მოდული: <tt>time_of_flight_sensor</tt>", "md_components_2modules_2sensors_2time__of__flight__sensor_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2sensors_2time__of__flight__sensor_2README.html#autotoc_md111", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2sensors_2time__of__flight__sensor_2README.html#autotoc_md112", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2sensors_2time__of__flight__sensor_2README.html#autotoc_md113", null ],
      [ "4. 📢 ივენთები (Events)", "md_components_2modules_2sensors_2time__of__flight__sensor_2README.html#autotoc_md114", [
        [ "გამოქვეყნებული ივენთები", "md_components_2modules_2sensors_2time__of__flight__sensor_2README.html#autotoc_md115", null ]
      ] ],
      [ "5. 💡 გამოყენების მაგალითი (<tt>sensor_aggregator</tt>-თან ინტეგრაცია)", "md_components_2modules_2sensors_2time__of__flight__sensor_2README.html#autotoc_md116", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2sensors_2time__of__flight__sensor_2README.html#autotoc_md117", null ]
    ] ],
    [ "💾 მოდული <tt>storage_manager</tt>", "md_components_2modules_2storage_2storage__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md119", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md120", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md121", null ],
      [ "4. 🛠️ Service API (<tt>storage_api_t</tt>)", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md122", [
        [ "Key-Value ოპერაციები", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md123", null ],
        [ "ფაილური ოპერაციები (მხარდაჭერილია SPIFFS და SD Card backend-ებზე)", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md124", null ]
      ] ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md125", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2storage_2storage__manager_2README.html#autotoc_md126", null ]
    ] ],
    [ "🚨 მოდული: System Alarms Manager", "md_components_2modules_2system_2alarms__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md128", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md129", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md130", [
        [ "3.1. განგაშის წესის პარამეტრები (<tt>alarms</tt> ობიექტი)", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md131", null ],
        [ "3.2. მოქმედებების პარამეტრები (<tt>actions</tt> ობიექტი)", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md132", null ]
      ] ],
      [ "4. 🔌 Service API", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md133", null ],
      [ "5. 📢 ივენთების დამუშავება", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md134", null ],
      [ "6. 💡 გამოყენების სცენარები", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md135", null ],
      [ "7. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2system_2alarms__manager_2README.html#autotoc_md136", null ]
    ] ],
    [ "🛰️ მოდული: Connectivity Watchdog", "md_components_2modules_2system_2connectivity__watchdog_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md138", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md139", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>system_config.json</tt>)", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md140", [
        [ "3.1. კონფიგურაციის სრული მაგალითი", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md141", null ],
        [ "3.2. <tt>layers</tt> ობიექტის პარამეტრები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md142", null ],
        [ "3.3. <tt>actions</tt> ობიექტის პარამეტრები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md143", null ],
        [ "3.4. მხარდაჭერილი <tt>type</tt>-ები და <tt>do</tt> მოქმედებები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md144", null ]
      ] ],
      [ "4. 🔌 Service API", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md145", null ],
      [ "5. 📢 ივენთების უნივერსალური მოდელი", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md146", [
        [ "5.1. MQTT მოდულის ინტეგრაცია", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md147", null ],
        [ "5.2. WebSocket მოდულის (ჰიპოთეტური) ინტეგრაცია", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md148", null ],
        [ "5.3. PING შემოწმების ინტეგრაცია", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md149", null ]
      ] ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md150", null ]
    ] ],
    [ "🆔 მოდული: <tt>device_identity_service</tt>", "md_components_2modules_2system_2device__identity__service_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md152", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md153", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md154", null ],
      [ "4. 🔌 Service API (<tt>device_identity_api_t</tt>)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md155", null ],
      [ "5. 📢 ივენთები (Events)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md156", [
        [ "გამოქვეყნებული ივენთები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md157", null ],
        [ "გამოწერილი ივენთები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md158", null ]
      ] ],
      [ "6. 📡 MQTT ინტერფეისი", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md159", null ],
      [ "7. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md160", null ],
      [ "8. 💡 გამოყენების მაგალითი (მოთხოვნა-პასუხი)", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md161", null ],
      [ "9. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2system_2device__identity__service_2README.html#autotoc_md162", null ]
    ] ],
    [ "🩺 მოდული: Health Monitor", "md_components_2modules_2system_2health__monitor_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md164", null ],
      [ "2. ❗ წინაპირობები და <tt>menuconfig</tt>", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md165", null ],
      [ "3. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md166", null ],
      [ "4. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md167", null ],
      [ "5. 🔌 Service API (<tt>health_api_t</tt>)", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md168", null ],
      [ "6. 📢 ივენთები", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md169", [
        [ "გამოქვეყნებული ივენთები", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md170", null ],
        [ "გამოწერილი ივენთები", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md171", null ]
      ] ],
      [ "7. 💡 გამოყენების მაგალითი", "md_components_2modules_2system_2health__monitor_2README.html#autotoc_md172", null ]
    ] ],
    [ "მოდული: OTA Update Manager", "md_components_2modules_2system_2ota__update__manager_2README.html", [
      [ "1. მიმოხილვა", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md174", null ],
      [ "2. წინაპირობები და <tt>menuconfig</tt>", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md175", null ],
      [ "3. არქიტექტურული პრინციპები", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md176", null ],
      [ "4. კონფიგურაცია", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md177", null ],
      [ "5. Service API (<tt>ota_api_t</tt>)", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md178", null ],
      [ "6. გამოქვეყნებული ივენთები", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md179", null ],
      [ "7. 💡 გამოყენების მაგალითი", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md180", [
        [ "8. განახლების ვალიდაცია და Rollback", "md_components_2modules_2system_2ota__update__manager_2README.html#autotoc_md181", null ]
      ] ]
    ] ],
    [ "⏱️ მოდული: <tt>time_sync</tt>", "md_components_2modules_2system_2time__sync_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2time__sync_2README.html#autotoc_md183", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2time__sync_2README.html#autotoc_md184", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2time__sync_2README.html#autotoc_md185", null ],
      [ "4. 🔌 Service API (<tt>time_sync_api_t</tt>)", "md_components_2modules_2system_2time__sync_2README.html#autotoc_md186", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2system_2time__sync_2README.html#autotoc_md187", null ]
    ] ],
    [ "🐕 მოდული: <tt>watchdog_manager</tt>", "md_components_2modules_2system_2watchdog__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md189", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md190", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md191", null ],
      [ "4. 🔌 Service API (<tt>watchdog_api_t</tt>)", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md192", null ],
      [ "5. 📢 გამოქვეყნებული ივენთები", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md193", null ],
      [ "6. 💡 გამოყენების მაგალითი", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md194", null ],
      [ "7. ❗ წინაპირობები და <tt>menuconfig</tt>", "md_components_2modules_2system_2watchdog__manager_2README.html#autotoc_md195", null ]
    ] ],
    [ "🧪 მოდული: <tt>sensor_simulator</tt>", "md_components_2modules_2testing_2sensor__simulator_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md197", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md198", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md199", null ],
      [ "4. 📢 გამოქვეყნებული ივენთები", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md200", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2testing_2sensor__simulator_2README.html#autotoc_md201", null ]
    ] ],
    [ "🧪 მოდული <tt>timer_test</tt>", "md_components_2modules_2testing_2timer__test__module_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md203", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md204", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md205", null ],
      [ "4. 🚀 მუშაობის ლოგიკა", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md206", null ],
      [ "5. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2testing_2timer__test__module_2README.html#autotoc_md207", null ]
    ] ],
    [ "⚙️ მოდული: Command Router & System Shell", "md_components_2modules_2utillities_2command__router_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2utillities_2command__router_2README.html#autotoc_md209", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2utillities_2command__router_2README.html#autotoc_md210", null ],
      [ "3. 🛠️ კონფიგურაცია", "md_components_2modules_2utillities_2command__router_2README.html#autotoc_md211", null ],
      [ "4. 🔌 Service API (<tt>cmd_router_api_t</tt>)", "md_components_2modules_2utillities_2command__router_2README.html#autotoc_md212", null ],
      [ "5. 🚀 ჩაშენებული ბრძანებები", "md_components_2modules_2utillities_2command__router_2README.html#autotoc_md213", null ],
      [ "6. 💡 გამოყენების მაგალითი", "md_components_2modules_2utillities_2command__router_2README.html#autotoc_md214", null ],
      [ "7. 🩺 პრობლემების მოგვარება (Troubleshooting)", "md_components_2modules_2utillities_2command__router_2README.html#autotoc_md215", null ]
    ] ],
    [ "Logger Module", "md_components_2modules_2utillities_2logger__module_2README.html", [
      [ "Overview", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md217", null ],
      [ "Features", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md218", null ],
      [ "Purpose", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md219", null ],
      [ "Kconfig Configuration", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md220", [
        [ "Main Configuration Options", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md221", null ],
        [ "Compilation Control", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md222", null ]
      ] ],
      [ "Runtime Configuration", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md223", [
        [ "Enable Logger Module", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md224", null ],
        [ "Disable Logger Module", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md225", null ]
      ] ],
      [ "Runtime Control", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md226", [
        [ "Command Format", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md227", null ],
        [ "Supported Commands", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md228", [
          [ "Disable Logger", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md229", null ],
          [ "Enable Logger", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md230", null ]
        ] ],
        [ "Behavior", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md231", null ]
      ] ],
      [ "Implementation Details", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md232", [
        [ "Module Structure", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md233", null ],
        [ "Event Subscription", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md234", null ],
        [ "Output Format", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md235", null ]
      ] ],
      [ "Development Notes", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md236", null ],
      [ "API Reference", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md237", [
        [ "Functions", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md238", [
          [ "<tt>logger_module_create(const cJSON *config)</tt>", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md239", null ]
        ] ],
        [ "ბრძანებები", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md240", null ],
        [ "ქცევა", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md241", null ],
        [ "მაგალითი", "md_components_2modules_2utillities_2logger__module_2README.html#autotoc_md242", null ]
      ] ]
    ] ],
    [ "💡 მოდული: <tt>rgb_led_indicator</tt>", "md_components_2modules_2utillities_2rgb__led__indicator_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2utillities_2rgb__led__indicator_2README.html#autotoc_md244", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2utillities_2rgb__led__indicator_2README.html#autotoc_md245", null ],
      [ "3. ⚙️ კონფიგურაცია (<tt>config.json</tt>)", "md_components_2modules_2utillities_2rgb__led__indicator_2README.html#autotoc_md246", null ],
      [ "4. 🛠️ Service API (<tt>rgb_led_api_t</tt>)", "md_components_2modules_2utillities_2rgb__led__indicator_2README.html#autotoc_md247", null ],
      [ "5. ⌨️ CLI ბრძანებები (Command Line Interface)", "md_components_2modules_2utillities_2rgb__led__indicator_2README.html#autotoc_md248", null ],
      [ "6. 📢 ივენთების დამუშავება", "md_components_2modules_2utillities_2rgb__led__indicator_2README.html#autotoc_md249", null ],
      [ "7. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2utillities_2rgb__led__indicator_2README.html#autotoc_md250", null ]
    ] ],
    [ "⏱️ მოდული <tt>system_timer</tt>", "md_components_2modules_2utillities_2system__timer_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2utillities_2system__timer_2README.html#autotoc_md252", null ],
      [ "2. 🏗️ არქიტექტურული პრინციპები", "md_components_2modules_2utillities_2system__timer_2README.html#autotoc_md253", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2utillities_2system__timer_2README.html#autotoc_md254", null ],
      [ "4. 🛠️ Service API (<tt>timer_api_t</tt>)", "md_components_2modules_2utillities_2system__timer_2README.html#autotoc_md255", null ],
      [ "5. 💡 გამოყენების მაგალითი", "md_components_2modules_2utillities_2system__timer_2README.html#autotoc_md256", null ],
      [ "6. ❗ მნიშვნელოვანი შენიშვნები", "md_components_2modules_2utillities_2system__timer_2README.html#autotoc_md257", null ]
    ] ],
    [ "🖥️ მოდული: <tt>ui_manager</tt>", "md_components_2modules_2utillities_2ui__manager_2README.html", [
      [ "1. 📜 მიმოხილვა", "md_components_2modules_2utillities_2ui__manager_2README.html#autotoc_md259", null ],
      [ "2. 🏛️ არქიტექტურული პრინციპები", "md_components_2modules_2utillities_2ui__manager_2README.html#autotoc_md260", null ],
      [ "3. ⚙️ კონფიგურაცია", "md_components_2modules_2utillities_2ui__manager_2README.html#autotoc_md261", null ],
      [ "4. 🔌 გამოყენებული სერვისები", "md_components_2modules_2utillities_2ui__manager_2README.html#autotoc_md262", null ],
      [ "5. 📢 დამუშავებული ივენთები", "md_components_2modules_2utillities_2ui__manager_2README.html#autotoc_md263", null ]
    ] ],
    [ "Synapse Actuators Module API Reference", "md_docs_2api__reference_2actuators__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2actuators__api.html#autotoc_md265", null ],
      [ "Relay Module API", "md_docs_2api__reference_2actuators__api.html#autotoc_md267", [
        [ "module_t <em>relay_module_create(const cJSON *config);", "md_docs_2api__reference_2actuators__api.html#autotoc_md268", null ],
        [ "esp_err_t relay_module_enable(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md269", null ],
        [ "esp_err_t relay_module_disable(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md270", null ],
        [ "esp_err_t relay_module_toggle(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md271", null ],
        [ "esp_err_t relay_module_set_state(module_t *module, bool state);", "md_docs_2api__reference_2actuators__api.html#autotoc_md272", null ],
        [ "bool relay_module_get_state(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md273", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2actuators__api.html#autotoc_md275", [
        [ "static void relay_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2actuators__api.html#autotoc_md276", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2actuators__api.html#autotoc_md278", [
        [ "static esp_err_t parse_relay_config(const cJSON *config, relay_private_data_t *relay_data);", "md_docs_2api__reference_2actuators__api.html#autotoc_md279", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2actuators__api.html#autotoc_md281", [
        [ "static module_status_t relay_get_status(module_t *module);", "md_docs_2api__reference_2actuators__api.html#autotoc_md282", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2actuators__api.html#autotoc_md284", null ],
      [ "მაგალითები", "md_docs_2api__reference_2actuators__api.html#autotoc_md286", null ]
    ] ],
    [ "Synapse API Reference Index", "md_docs_2api__reference_2api__index.html", [
      [ "Core API", "md_docs_2api__reference_2api__index.html#autotoc_md290", null ],
      [ "Module API", "md_docs_2api__reference_2api__index.html#autotoc_md291", null ],
      [ "System & Extensions", "md_docs_2api__reference_2api__index.html#autotoc_md292", null ]
    ] ],
    [ "Synapse Command Router API Reference", "md_docs_2api__reference_2command__router__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2command__router__api.html#autotoc_md295", null ],
      [ "⚙️ API სტრუქტურა (<tt>cmd_router_api_t</tt>)", "md_docs_2api__reference_2command__router__api.html#autotoc_md297", null ],
      [ "📚 API ფუნქციები", "md_docs_2api__reference_2command__router__api.html#autotoc_md299", [
        [ "<tt>esp_err_t register_command(const cmd_t *command)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md300", null ],
        [ "<tt>esp_err_t unregister_command(const char *command_name)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md302", null ],
        [ "<tt>bool is_command_registered(const char *command_name)</tt>", "md_docs_2api__reference_2command__router__api.html#autotoc_md304", null ]
      ] ],
      [ "📖 დამხმარე სტრუქტურა (<tt>cmd_t</tt>)", "md_docs_2api__reference_2command__router__api.html#autotoc_md306", null ],
      [ "💡 გამოყენების პატერნი: კონფლიქტისგან დაცული რეგისტრაცია", "md_docs_2api__reference_2command__router__api.html#autotoc_md308", null ]
    ] ],
    [ "📡 Synapse Communication Module API Reference", "md_docs_2api__reference_2communication__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2communication__api.html#autotoc_md310", null ],
      [ "📶 WiFi Manager Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md312", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md313", null ],
        [ "💡 WiFi API-ს გამოყენების მაგალითი", "md_docs_2api__reference_2communication__api.html#autotoc_md314", null ]
      ] ],
      [ "☁️ MQTT Client Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md316", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md317", null ],
        [ "💡 MQTT API-ს გამოყენების მაგალითი", "md_docs_2api__reference_2communication__api.html#autotoc_md318", null ]
      ] ],
      [ "⚡ ESP-NOW Service API", "md_docs_2api__reference_2communication__api.html#autotoc_md320", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2communication__api.html#autotoc_md321", null ]
      ] ],
      [ "📜 Naming & Structure კონვენციები", "md_docs_2api__reference_2communication__api.html#autotoc_md323", null ]
    ] ],
    [ "Synapse Configuration Manager API Reference", "md_docs_2api__reference_2configuration__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2configuration__api.html#autotoc_md326", null ],
      [ "🔑 გასაღების ფორმატი (Dot-Notation)", "md_docs_2api__reference_2configuration__api.html#autotoc_md328", null ],
      [ "⚙️ ძირითადი API ფუნქციები", "md_docs_2api__reference_2configuration__api.html#autotoc_md330", [
        [ "<tt>esp_err_t synapse_config_get_string(const char *key, char *buffer, size_t buffer_size)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md331", null ],
        [ "<tt>esp_err_t synapse_config_get_int(const char *key, int *out_value)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md332", null ],
        [ "<tt>esp_err_t synapse_config_get_bool(const char *key, bool *out_value)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md333", null ],
        [ "<tt>esp_err_t synapse_config_save(void)</tt>", "md_docs_2api__reference_2configuration__api.html#autotoc_md334", null ]
      ] ],
      [ "🚀 გამოყენების მაგალითები", "md_docs_2api__reference_2configuration__api.html#autotoc_md336", [
        [ "გლობალური პარამეტრის წაკითხვა", "md_docs_2api__reference_2configuration__api.html#autotoc_md337", null ],
        [ "მოდულის პარამეტრის წაკითხვა", "md_docs_2api__reference_2configuration__api.html#autotoc_md338", null ]
      ] ]
    ] ],
    [ "Synapse Core API Reference", "md_docs_2api__reference_2core__api.html", [
      [ "Service Locator API", "md_docs_2api__reference_2core__api.html#autotoc_md340", [
        [ "synapse_service_register", "md_docs_2api__reference_2core__api.html#autotoc_md341", null ],
        [ "synapse_service_unregister", "md_docs_2api__reference_2core__api.html#autotoc_md342", null ],
        [ "synapse_service_get", "md_docs_2api__reference_2core__api.html#autotoc_md343", null ],
        [ "synapse_service_get_type", "md_docs_2api__reference_2core__api.html#autotoc_md344", null ],
        [ "synapse_service_lookup_by_type", "md_docs_2api__reference_2core__api.html#autotoc_md345", null ]
      ] ],
      [ "Event Bus API", "md_docs_2api__reference_2core__api.html#autotoc_md347", [
        [ "<tt>synapse_event_bus_post</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md348", null ],
        [ "<tt>synapse_event_bus_subscribe</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md349", null ],
        [ "<tt>synapse_event_bus_unsubscribe</tt>", "md_docs_2api__reference_2core__api.html#autotoc_md350", null ]
      ] ],
      [ "Module Register API", "md_docs_2api__reference_2core__api.html#autotoc_md352", [
        [ "synapse_module_register", "md_docs_2api__reference_2core__api.html#autotoc_md353", null ],
        [ "synapse_module_find_by_name", "md_docs_2api__reference_2core__api.html#autotoc_md354", null ]
      ] ],
      [ "Module Factory API", "md_docs_2api__reference_2core__api.html#autotoc_md356", [
        [ "synapse_module_factory_create", "md_docs_2api__reference_2core__api.html#autotoc_md357", null ]
      ] ],
      [ "Configuration Manager API", "md_docs_2api__reference_2core__api.html#autotoc_md359", [
        [ "synapse_config_get_string", "md_docs_2api__reference_2core__api.html#autotoc_md360", null ],
        [ "synapse_config_get_int", "md_docs_2api__reference_2core__api.html#autotoc_md361", null ],
        [ "synapse_config_get_module_config", "md_docs_2api__reference_2core__api.html#autotoc_md362", null ]
      ] ],
      [ "Logging API", "md_docs_2api__reference_2core__api.html#autotoc_md364", [
        [ "DEFINE_COMPONENT_TAG", "md_docs_2api__reference_2core__api.html#autotoc_md365", null ],
        [ "ESP_LOGE, ESP_LOGW, ESP_LOGI, ESP_LOGD, ESP_LOGV", "md_docs_2api__reference_2core__api.html#autotoc_md366", null ]
      ] ]
    ] ],
    [ "Synapse Custom API Reference", "md_docs_2api__reference_2custom__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2custom__api.html#autotoc_md369", null ],
      [ "Custom Module API", "md_docs_2api__reference_2custom__api.html#autotoc_md371", [
        [ "module_t *custom_module_create(const cJSON *config);", "md_docs_2api__reference_2custom__api.html#autotoc_md372", null ],
        [ "esp_err_t custom_module_do_action(module_t *module, const char *action, const void *params);", "md_docs_2api__reference_2custom__api.html#autotoc_md373", null ],
        [ "esp_err_t custom_module_get_status(module_t *module, custom_status_t *status);", "md_docs_2api__reference_2custom__api.html#autotoc_md374", null ]
      ] ],
      [ "Custom Event API", "md_docs_2api__reference_2custom__api.html#autotoc_md376", [
        [ "Custom Event-ის იდენტიფიკატორი", "md_docs_2api__reference_2custom__api.html#autotoc_md377", null ],
        [ "Custom Event-ის მონაცემების სტრუქტურა", "md_docs_2api__reference_2custom__api.html#autotoc_md378", null ],
        [ "static void custom_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2custom__api.html#autotoc_md379", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2custom__api.html#autotoc_md381", null ],
      [ "მაგალითები", "md_docs_2api__reference_2custom__api.html#autotoc_md383", null ]
    ] ],
    [ "Synapse Device Identity Service API Reference", "md_docs_2api__reference_2device__identity__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2device__identity__api.html#autotoc_md386", null ],
      [ "Device Identity Service API", "md_docs_2api__reference_2device__identity__api.html#autotoc_md388", [
        [ "<tt>const char* get_device_id(void);</tt>", "md_docs_2api__reference_2device__identity__api.html#autotoc_md389", null ],
        [ "<tt>const char* get_firmware_version(void);</tt>", "md_docs_2api__reference_2device__identity__api.html#autotoc_md390", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2device__identity__api.html#autotoc_md392", null ],
      [ "მაგალითები", "md_docs_2api__reference_2device__identity__api.html#autotoc_md394", null ]
    ] ],
    [ "Synapse Display Module API Reference", "md_docs_2api__reference_2display__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2display__api.html#autotoc_md397", null ],
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2display__api.html#autotoc_md399", [
        [ "module_t <em>ssd1306_module_create(const cJSON *config);", "md_docs_2api__reference_2display__api.html#autotoc_md400", null ],
        [ "static esp_err_t ssd1306_init(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md401", null ],
        [ "static esp_err_t ssd1306_enable(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md402", null ],
        [ "static esp_err_t ssd1306_disable(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md403", null ],
        [ "static esp_err_t ssd1306_deinit(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md404", null ]
      ] ],
      [ "Service API (Service Locator-ისთვის)", "md_docs_2api__reference_2display__api.html#autotoc_md406", [
        [ "esp_err_t ssd1306_api_enable(void);", "md_docs_2api__reference_2display__api.html#autotoc_md407", null ],
        [ "esp_err_t ssd1306_api_disable(void);", "md_docs_2api__reference_2display__api.html#autotoc_md408", null ],
        [ "esp_err_t ssd1306_api_clear(void);", "md_docs_2api__reference_2display__api.html#autotoc_md409", null ],
        [ "esp_err_t ssd1306_api_write_text(const char *text, uint8_t line);", "md_docs_2api__reference_2display__api.html#autotoc_md410", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2display__api.html#autotoc_md412", [
        [ "static void ssd1306_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2display__api.html#autotoc_md413", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2display__api.html#autotoc_md415", [
        [ "static esp_err_t parse_ssd1306_config(const cJSON *config, ssd1306_private_data_t *ssd1306_data);", "md_docs_2api__reference_2display__api.html#autotoc_md416", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2display__api.html#autotoc_md418", [
        [ "static module_status_t ssd1306_get_status(module_t *module);", "md_docs_2api__reference_2display__api.html#autotoc_md419", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2display__api.html#autotoc_md421", null ],
      [ "მაგალითები", "md_docs_2api__reference_2display__api.html#autotoc_md423", null ]
    ] ],
    [ "Synapse Drivers Module API Reference", "md_docs_2api__reference_2drivers__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2drivers__api.html#autotoc_md426", null ],
      [ "I2C Bus Driver API", "md_docs_2api__reference_2drivers__api.html#autotoc_md428", [
        [ "module_t *i2c_bus_module_create(const cJSON *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md429", null ],
        [ "esp_err_t i2c_bus_init(i2c_bus_handle_t *i2c_bus_handle, const i2c_bus_config_t *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md430", null ],
        [ "esp_err_t i2c_bus_read(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, uint8_t *data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md431", null ],
        [ "esp_err_t i2c_bus_write(i2c_bus_handle_t *i2c_bus_handle, uint8_t device_address, const uint8_t *data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md432", null ],
        [ "esp_err_t i2c_bus_scan(i2c_bus_handle_t *i2c_bus_handle);", "md_docs_2api__reference_2drivers__api.html#autotoc_md433", null ]
      ] ],
      [ "SPI Bus Driver API", "md_docs_2api__reference_2drivers__api.html#autotoc_md435", [
        [ "module_t *spi_bus_module_create(const cJSON *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md436", null ],
        [ "esp_err_t spi_bus_init(spi_bus_handle_t *spi_bus_handle, const spi_bus_config_t *config);", "md_docs_2api__reference_2drivers__api.html#autotoc_md437", null ],
        [ "esp_err_t spi_bus_transfer(spi_bus_handle_t *spi_bus_handle, const uint8_t *tx_data, uint8_t *rx_data, size_t data_length);", "md_docs_2api__reference_2drivers__api.html#autotoc_md438", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2drivers__api.html#autotoc_md440", null ],
      [ "მაგალითები", "md_docs_2api__reference_2drivers__api.html#autotoc_md442", null ]
    ] ],
    [ "Synapse Event Bus API Reference", "md_docs_2api__reference_2event__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2event__api.html#autotoc_md445", null ],
      [ "Event Bus API", "md_docs_2api__reference_2event__api.html#autotoc_md447", [
        [ "<tt>esp_err_t synapse_event_bus_init(void)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md448", null ],
        [ "<tt>esp_err_t synapse_event_bus_post(core_framework_event_id_t event_id, event_data_wrapper_t *data_wrapper)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md449", null ],
        [ "<tt>esp_err_t synapse_event_bus_subscribe(core_framework_event_id_t event_id, struct module_t *module)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md450", null ],
        [ "<tt>esp_err_t synapse_event_bus_unsubscribe(core_framework_event_id_t event_id, struct module_t *module)</tt>", "md_docs_2api__reference_2event__api.html#autotoc_md451", null ]
      ] ],
      [ "ივენთის მონაცემების მართვა (Reference Counting)", "md_docs_2api__reference_2event__api.html#autotoc_md453", null ],
      [ "Event Handler-ის მაგალითი", "md_docs_2api__reference_2event__api.html#autotoc_md455", null ],
      [ "Custom Event-ების აღწერა", "md_docs_2api__reference_2event__api.html#autotoc_md457", [
        [ "მაგალითი Custom Event-ის აღწერისა", "md_docs_2api__reference_2event__api.html#autotoc_md458", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2event__api.html#autotoc_md460", null ],
      [ "მაგალითები", "md_docs_2api__reference_2event__api.html#autotoc_md462", null ]
    ] ],
    [ "API რეფერენსი: Event Payloads (event_payloads.h)", "md_docs_2api__reference_2event__payloads__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md465", null ],
      [ "Payload სტრუქტურები", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md466", [
        [ "synapse_telemetry_payload_t", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md467", null ],
        [ "synapse_config_updated_payload_t", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md468", null ]
      ] ],
      [ "მეხსიერების მართვის ფუნქციები", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md469", [
        [ "synapse_payload_common_free", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md470", null ],
        [ "synapse_telemetry_payload_free", "md_docs_2api__reference_2event__payloads__api.html#autotoc_md471", null ]
      ] ]
    ] ],
    [ "Synapse Health Monitor Module API Reference", "md_docs_2api__reference_2health__monitor__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md473", null ],
      [ "Health Monitor Module API", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md475", [
        [ "module_t *health_monitor_module_create(const cJSON *config);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md476", null ],
        [ "esp_err_t health_monitor_module_start(module_t *module);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md477", null ],
        [ "esp_err_t health_monitor_module_stop(module_t *module);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md478", null ],
        [ "esp_err_t health_monitor_module_get_status(module_t *module, health_status_t *status);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md479", null ],
        [ "esp_err_t health_monitor_module_register_check(module_t *module, health_check_fn_t check_fn);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md480", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md482", [
        [ "static void health_monitor_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md483", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md485", null ],
      [ "მაგალითები", "md_docs_2api__reference_2health__monitor__api.html#autotoc_md487", null ]
    ] ],
    [ "Synapse Local Control Module API Reference", "md_docs_2api__reference_2local__ctrl__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md490", null ],
      [ "Local Control Module API", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md492", [
        [ "module_t *local_ctrl_module_create(const cJSON *config);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md493", null ],
        [ "esp_err_t local_ctrl_module_start(module_t *module);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md494", null ],
        [ "esp_err_t local_ctrl_module_stop(module_t *module);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md495", null ],
        [ "esp_err_t local_ctrl_module_register_endpoint(const char *uri, http_method_t method, local_ctrl_handler_t handler);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md496", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md498", [
        [ "static void local_ctrl_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md499", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md501", null ],
      [ "მაგალითები", "md_docs_2api__reference_2local__ctrl__api.html#autotoc_md503", null ]
    ] ],
    [ "Synapse Module API Reference", "md_docs_2api__reference_2module__api.html", [
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2module__api.html#autotoc_md506", [
        [ "<tt>module_t *{module_name}_create(const cJSON *config);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md507", null ],
        [ "<tt>static esp_err_t {module_name}_init(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md508", null ],
        [ "<tt>static esp_err_t {module_name}_start(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md509", null ],
        [ "<tt>static void {module_name}_deinit(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md510", null ]
      ] ],
      [ "Service API (Service Locator-ისთვის)", "md_docs_2api__reference_2module__api.html#autotoc_md512", [
        [ "<tt>esp_err_t {module_name}_api_{action}(...);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md513", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2module__api.html#autotoc_md515", [
        [ "<tt>static void {module_name}_handle_event(module_t *self, const char *event_name, void *event_data);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md516", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2module__api.html#autotoc_md518", [
        [ "<tt>static esp_err_t parse_{module_name}_config(const cJSON *config_node, {module_name}_private_data_t *p_data);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md519", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2module__api.html#autotoc_md521", [
        [ "<tt>static module_status_t {module_name}_get_status(module_t *self);</tt>", "md_docs_2api__reference_2module__api.html#autotoc_md522", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2module__api.html#autotoc_md524", null ]
    ] ],
    [ "Synapse MQTT Manager Service API Reference", "md_docs_2api__reference_2mqtt__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2mqtt__api.html#autotoc_md527", null ],
      [ "MQTT Manager API", "md_docs_2api__reference_2mqtt__api.html#autotoc_md529", [
        [ "სამომავლო გაფართოება", "md_docs_2api__reference_2mqtt__api.html#autotoc_md530", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2mqtt__api.html#autotoc_md532", null ],
      [ "მაგალითები", "md_docs_2api__reference_2mqtt__api.html#autotoc_md534", null ]
    ] ],
    [ "Synapse OTA (Over-the-Air) Module API Reference", "md_docs_2api__reference_2ota__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2ota__api.html#autotoc_md537", null ],
      [ "OTA Module API", "md_docs_2api__reference_2ota__api.html#autotoc_md539", [
        [ "module_t *ota_module_create(const cJSON *config);", "md_docs_2api__reference_2ota__api.html#autotoc_md540", null ],
        [ "esp_err_t ota_module_start_update(const char *url);", "md_docs_2api__reference_2ota__api.html#autotoc_md541", null ],
        [ "esp_err_t ota_module_get_status(ota_status_t *status);", "md_docs_2api__reference_2ota__api.html#autotoc_md542", null ],
        [ "esp_err_t ota_module_abort(void);", "md_docs_2api__reference_2ota__api.html#autotoc_md543", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2ota__api.html#autotoc_md545", [
        [ "static void ota_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2ota__api.html#autotoc_md546", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2ota__api.html#autotoc_md548", null ],
      [ "მაგალითები", "md_docs_2api__reference_2ota__api.html#autotoc_md550", null ]
    ] ],
    [ "Synapse Promise Manager API Reference", "md_docs_2api__reference_2promise__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2promise__api.html#autotoc_md553", null ],
      [ "1. Consumer Types (მომხმარებლისთვის)", "md_docs_2api__reference_2promise__api.html#autotoc_md555", [
        [ "<tt>promise_then_cb</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md556", null ],
        [ "<tt>promise_catch_cb</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md557", null ]
      ] ],
      [ "2. Provider API (სერვისის მომწოდებლისთვის)", "md_docs_2api__reference_2promise__api.html#autotoc_md559", [
        [ "<tt>promise_handle_t synapse_promise_create(promise_then_cb then_cb, promise_catch_cb catch_cb, void* user_context);</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md560", null ],
        [ "<tt>esp_err_t synapse_promise_resolve(promise_handle_t handle, void* result_data, void (*free_fn)(void*));</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md561", null ],
        [ "<tt>esp_err_t synapse_promise_reject(promise_handle_t handle, void* error_data, void (*free_fn)(void*));</tt>", "md_docs_2api__reference_2promise__api.html#autotoc_md562", null ]
      ] ],
      [ "💡 გამოყენების მაგალითი", "md_docs_2api__reference_2promise__api.html#autotoc_md564", null ]
    ] ],
    [ "📲 Synapse Provisioning Module API Reference", "md_docs_2api__reference_2provisioning__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2provisioning__api.html#autotoc_md566", null ],
      [ "🔵 BLE Provisioning Service API (იმპლემენტირებულია)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md568", [
        [ "ძირითადი ფუნქციები", "md_docs_2api__reference_2provisioning__api.html#autotoc_md569", null ]
      ] ],
      [ "📡 SoftAP Provisioning Module API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md571", [
        [ "module_t *softap_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md572", null ],
        [ "esp_err_t softap_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md573", null ],
        [ "esp_err_t softap_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md574", null ],
        [ "esp_err_t softap_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md575", null ],
        [ "esp_err_t softap_prov_module_get_status(module_t *module, softap_prov_status_t*status)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md576", null ]
      ] ],
      [ "🔳 QR Code Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md578", [
        [ "esp_err_t qr_code_prov_generate(const char *ssid, const char*password, char *qr_code_buffer, size_t buffer_size)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md579", null ],
        [ "esp_err_t qr_code_prov_display(const char *qr_code_buffer)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md580", null ]
      ] ],
      [ "🔌 Serial Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md582", [
        [ "module_t *serial_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md583", null ],
        [ "esp_err_t serial_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md584", null ],
        [ "esp_err_t serial_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md585", null ],
        [ "esp_err_t serial_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md586", null ]
      ] ],
      [ "☁️ Cloud Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md588", [
        [ "module_t *cloud_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md589", null ],
        [ "esp_err_t cloud_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md590", null ],
        [ "esp_err_t cloud_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md591", null ],
        [ "esp_err_t cloud_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md592", null ]
      ] ],
      [ "NFC Provisioning API", "md_docs_2api__reference_2provisioning__api.html#autotoc_md594", [
        [ "module_t *nfc_prov_module_create(const cJSON*config)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md595", null ],
        [ "esp_err_t nfc_prov_module_start(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md596", null ],
        [ "esp_err_t nfc_prov_module_stop(module_t *module)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md597", null ],
        [ "esp_err_t nfc_prov_module_set_credentials(module_t *module, const char*ssid, const char *password)", "md_docs_2api__reference_2provisioning__api.html#autotoc_md598", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2provisioning__api.html#autotoc_md600", null ],
      [ "მაგალითები", "md_docs_2api__reference_2provisioning__api.html#autotoc_md602", null ]
    ] ],
    [ "🔌 Relay Actuator API Reference", "md_docs_2api__reference_2relay__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2relay__api.html#autotoc_md605", null ],
      [ "1. ⌨️ CLI (Command Line Interface)", "md_docs_2api__reference_2relay__api.html#autotoc_md607", null ],
      [ "2. 📡 MQTT ინტერფეისი", "md_docs_2api__reference_2relay__api.html#autotoc_md609", [
        [ "ბრძანების გაგზავნა", "md_docs_2api__reference_2relay__api.html#autotoc_md610", null ],
        [ "სტატუსის მიღება", "md_docs_2api__reference_2relay__api.html#autotoc_md611", null ]
      ] ],
      [ "3. 📢 ივენთები (Events)", "md_docs_2api__reference_2relay__api.html#autotoc_md613", null ]
    ] ],
    [ "Synapse Resource Manager API Reference", "md_docs_2api__reference_2resource__manager__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md616", null ],
      [ "სამართავი რესურსების ტიპები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md618", null ],
      [ "API ფუნქციები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md620", [
        [ "<tt>esp_err_t synapse_resource_manager_init(void)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md621", null ],
        [ "<tt>esp_err_t synapse_resource_lock(synapse_resource_type_t type, uint8_t resource_id, const char *owner)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md623", null ],
        [ "<tt>esp_err_t synapse_resource_release(synapse_resource_type_t type, uint8_t resource_id, const char *owner)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md625", null ],
        [ "<tt>bool synapse_resource_is_locked(synapse_resource_type_t type, uint8_t resource_id)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md627", null ],
        [ "<tt>const char *synapse_resource_get_owner(synapse_resource_type_t type, uint8_t resource_id)</tt>", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md629", null ]
      ] ],
      [ "გამოყენების მაგალითი", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md631", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2api__reference_2resource__manager__api.html#autotoc_md633", null ]
    ] ],
    [ "🛡️ Security Status API Reference", "md_docs_2api__reference_2security__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2security__api.html#autotoc_md636", null ],
      [ "1. 🔌 Service API (<tt>security_status_api_t</tt>)", "md_docs_2api__reference_2security__api.html#autotoc_md638", [
        [ "API ფუნქციები", "md_docs_2api__reference_2security__api.html#autotoc_md639", null ]
      ] ],
      [ "2. ⌨️ CLI (Command Line Interface)", "md_docs_2api__reference_2security__api.html#autotoc_md641", null ]
    ] ],
    [ "Synapse Sensor Module API Reference", "md_docs_2api__reference_2sensor__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2sensor__api.html#autotoc_md643", null ],
      [ "მოდულის შექმნა და სიცოცხლის ციკლი", "md_docs_2api__reference_2sensor__api.html#autotoc_md645", [
        [ "module_t <em>dht22_module_create(const cJSON *config);", "md_docs_2api__reference_2sensor__api.html#autotoc_md646", null ],
        [ "static esp_err_t dht22_init(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md647", null ],
        [ "static esp_err_t dht22_enable(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md648", null ],
        [ "static esp_err_t dht22_disable(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md649", null ],
        [ "static esp_err_t dht22_deinit(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md650", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2sensor__api.html#autotoc_md652", [
        [ "static void dht22_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2sensor__api.html#autotoc_md653", null ],
        [ "static esp_err_t publish_sensor_data(dht22_private_data_t *dht22_data, float temperature, float humidity);", "md_docs_2api__reference_2sensor__api.html#autotoc_md654", null ]
      ] ],
      [ "Configuration Parsing API", "md_docs_2api__reference_2sensor__api.html#autotoc_md656", [
        [ "static esp_err_t parse_dht22_config(const cJSON *config, dht22_private_data_t *dht22_data);", "md_docs_2api__reference_2sensor__api.html#autotoc_md657", null ]
      ] ],
      [ "Status & Monitoring API", "md_docs_2api__reference_2sensor__api.html#autotoc_md659", [
        [ "static module_status_t dht22_get_status(module_t *module);", "md_docs_2api__reference_2sensor__api.html#autotoc_md660", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2sensor__api.html#autotoc_md662", null ],
      [ "მაგალითები", "md_docs_2api__reference_2sensor__api.html#autotoc_md664", null ]
    ] ],
    [ "💾 Synapse Storage Manager API Reference", "md_docs_2api__reference_2storage__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2storage__api.html#autotoc_md667", null ],
      [ "⚙️ API-ს სტრუქტურა (<tt>storage_api_t</tt>)", "md_docs_2api__reference_2storage__api.html#autotoc_md669", [
        [ "🔑 Key-Value ოპერაციები", "md_docs_2api__reference_2storage__api.html#autotoc_md670", null ],
        [ "🗂️ ფაილური ოპერაციები", "md_docs_2api__reference_2storage__api.html#autotoc_md672", null ]
      ] ],
      [ "💡 გამოყენების მაგალითი", "md_docs_2api__reference_2storage__api.html#autotoc_md674", null ]
    ] ],
    [ "Synapse System Module API Reference", "md_docs_2api__reference_2system__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2system__api.html#autotoc_md677", null ],
      [ "OTA Module API", "md_docs_2api__reference_2system__api.html#autotoc_md679", [
        [ "module_t *ota_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md680", null ],
        [ "esp_err_t ota_module_start_update(const char *url);", "md_docs_2api__reference_2system__api.html#autotoc_md681", null ],
        [ "esp_err_t ota_module_get_status(ota_status_t *status);", "md_docs_2api__reference_2system__api.html#autotoc_md682", null ]
      ] ],
      [ "Version Module API", "md_docs_2api__reference_2system__api.html#autotoc_md684", [
        [ "module_t *version_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md685", null ],
        [ "esp_err_t version_module_get_firmware_version(char *version_str, size_t size);", "md_docs_2api__reference_2system__api.html#autotoc_md686", null ]
      ] ],
      [ "Watchdog Module API", "md_docs_2api__reference_2system__api.html#autotoc_md688", [
        [ "module_t *watchdog_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md689", null ],
        [ "esp_err_t watchdog_module_feed(void);", "md_docs_2api__reference_2system__api.html#autotoc_md690", null ],
        [ "esp_err_t watchdog_module_set_timeout(uint32_t timeout_ms);", "md_docs_2api__reference_2system__api.html#autotoc_md691", null ]
      ] ],
      [ "Diagnostics Module API", "md_docs_2api__reference_2system__api.html#autotoc_md693", [
        [ "module_t *diagnostics_module_create(const cJSON *config);", "md_docs_2api__reference_2system__api.html#autotoc_md694", null ],
        [ "esp_err_t diagnostics_module_run_tests(void);", "md_docs_2api__reference_2system__api.html#autotoc_md695", null ],
        [ "esp_err_t diagnostics_module_get_report(char *report_buffer, size_t buffer_size);", "md_docs_2api__reference_2system__api.html#autotoc_md696", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2system__api.html#autotoc_md698", null ],
      [ "მაგალითები", "md_docs_2api__reference_2system__api.html#autotoc_md700", null ]
    ] ],
    [ "Synapse Task Pool Manager API Reference", "md_docs_2api__reference_2task__pool__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2task__pool__api.html#autotoc_md703", null ],
      [ "API-ს ტიპები და სტრუქტურები", "md_docs_2api__reference_2task__pool__api.html#autotoc_md705", [
        [ "<tt>synapse_job_handle_t</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md706", null ],
        [ "<tt>synapse_job_cb</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md707", null ]
      ] ],
      [ "API-ს ფუნქციები", "md_docs_2api__reference_2task__pool__api.html#autotoc_md709", [
        [ "<tt>synapse_job_handle_t synapse_task_pool_schedule_job(synapse_job_cb job_function, void* user_context, uint32_t interval_ms, bool is_periodic);</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md710", null ],
        [ "<tt>esp_err_t synapse_task_pool_cancel_job(synapse_job_handle_t handle);</tt>", "md_docs_2api__reference_2task__pool__api.html#autotoc_md711", null ]
      ] ],
      [ "💡 გამოყენების მაგალითი", "md_docs_2api__reference_2task__pool__api.html#autotoc_md713", null ]
    ] ],
    [ "Synapse Time Sync Module API Reference", "md_docs_2api__reference_2time__sync__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2time__sync__api.html#autotoc_md715", null ],
      [ "Time Sync Module API", "md_docs_2api__reference_2time__sync__api.html#autotoc_md717", [
        [ "module_t *time_sync_module_create(const cJSON *config);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md718", null ],
        [ "esp_err_t time_sync_module_start(module_t *module);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md719", null ],
        [ "esp_err_t time_sync_module_stop(module_t *module);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md720", null ],
        [ "esp_err_t time_sync_module_set_server(module_t *module, const char *server_url);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md721", null ],
        [ "esp_err_t time_sync_module_get_time(time_t *current_time);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md722", null ]
      ] ],
      [ "Event Handling API", "md_docs_2api__reference_2time__sync__api.html#autotoc_md724", [
        [ "static void time_sync_handle_event(module_t *module, int32_t event_id, void *event_data);", "md_docs_2api__reference_2time__sync__api.html#autotoc_md725", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2time__sync__api.html#autotoc_md727", null ],
      [ "მაგალითები", "md_docs_2api__reference_2time__sync__api.html#autotoc_md729", null ]
    ] ],
    [ "Synapse Utility Module API Reference", "md_docs_2api__reference_2utility__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2utility__api.html#autotoc_md732", null ],
      [ "Logger Module API", "md_docs_2api__reference_2utility__api.html#autotoc_md734", [
        [ "module_t *logger_module_create(const cJSON *config);", "md_docs_2api__reference_2utility__api.html#autotoc_md735", null ],
        [ "esp_err_t logger_module_log_info(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md736", null ],
        [ "esp_err_t logger_module_log_warning(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md737", null ],
        [ "esp_err_t logger_module_log_error(const char *message);", "md_docs_2api__reference_2utility__api.html#autotoc_md738", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2utility__api.html#autotoc_md740", null ],
      [ "მაგალითები", "md_docs_2api__reference_2utility__api.html#autotoc_md742", null ]
    ] ],
    [ "Synapse Utils API Reference", "md_docs_2api__reference_2utils__api.html", [
      [ "🎯 მიზანი", "md_docs_2api__reference_2utils__api.html#autotoc_md745", null ],
      [ "1. უსაფრთხო სტრიქონების მართვა", "md_docs_2api__reference_2utils__api.html#autotoc_md747", [
        [ "<tt>char* synapse_safe_strncpy(char *dest, const char *src, size_t size);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md748", null ]
      ] ],
      [ "2. კონფიგურაციის პარსინგის დამხმარე ფუნქციები", "md_docs_2api__reference_2utils__api.html#autotoc_md750", [
        [ "<tt>bool synapse_config_get_string_from_node(const char* tag, const cJSON *json_node, const char *key, char *dest, size_t dest_size);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md751", null ],
        [ "<tt>bool synapse_config_get_int_from_node(const char* tag, const cJSON *json_node, const char *key, int *out_value);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md752", null ],
        [ "<tt>bool synapse_config_get_bool_from_node(const char* tag, const cJSON *json_node, const char *key, bool *out_value);</tt>", "md_docs_2api__reference_2utils__api.html#autotoc_md753", null ]
      ] ]
    ] ],
    [ "Synapse Version Module API Reference", "md_docs_2api__reference_2version__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2version__api.html#autotoc_md755", null ],
      [ "Version Module API", "md_docs_2api__reference_2version__api.html#autotoc_md757", [
        [ "module_t *version_module_create(const cJSON *config);", "md_docs_2api__reference_2version__api.html#autotoc_md758", null ],
        [ "esp_err_t version_module_get_firmware_version(char *version_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md759", null ],
        [ "esp_err_t version_module_get_build_date(char *date_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md760", null ],
        [ "esp_err_t version_module_get_build_time(char *time_str, size_t size);", "md_docs_2api__reference_2version__api.html#autotoc_md761", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2version__api.html#autotoc_md763", null ],
      [ "მაგალითები", "md_docs_2api__reference_2version__api.html#autotoc_md765", null ]
    ] ],
    [ "Synapse WiFi Manager Service API Reference", "md_docs_2api__reference_2wifi__api.html", [
      [ "მიზანი", "md_docs_2api__reference_2wifi__api.html#autotoc_md768", null ],
      [ "WiFi Manager API", "md_docs_2api__reference_2wifi__api.html#autotoc_md770", [
        [ "სამომავლო გაფართოება", "md_docs_2api__reference_2wifi__api.html#autotoc_md771", null ]
      ] ],
      [ "Naming & Structure", "md_docs_2api__reference_2wifi__api.html#autotoc_md773", null ],
      [ "მაგალითები", "md_docs_2api__reference_2wifi__api.html#autotoc_md775", null ]
    ] ],
    [ "CHANGELOG: led_blinker ინიციალიზაციის პრობლემის დიაგნოსტიკა და გამოსწორება", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html", [
      [ "პრობლემა", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md778", null ],
      [ "დიაგნოსტიკა", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md779", null ],
      [ "მიზეზი", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md780", null ],
      [ "გამოსავალი", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md781", null ],
      [ "ცვლილებები კოდში", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md782", null ],
      [ "რეკომენდაცია", "md_docs_2changelog_22025-06-26-led__blinker-nvs-debug.html#autotoc_md783", null ]
    ] ],
    [ "ცვლილებების ჟურნალი — 2025-06-24", "md_docs_2changelog_2CHANGELOG__2025-06-24.html", [
      [ "დამატებული და განახლებული ფაილები", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md786", [
        [ "[core/CMakeLists.txt]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md787", null ],
        [ "[core/collect_modules.cmake]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md788", null ],
        [ "[core/include/system_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md789", null ],
        [ "[interfaces/include/base_module.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md790", null ],
        [ "[core/include/service_locator.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md791", null ],
        [ "[core/include/module_registry.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md792", null ],
        [ "[core/include/event_bus.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md793", null ],
        [ "[core/include/config_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md794", null ],
        [ "[core/include/system_event_ids.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md795", null ],
        [ "[core/include/resource_manager.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md796", null ],
        [ "[core/include/framework_config.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md797", null ],
        [ "[interfaces/include/logging.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md798", null ],
        [ "[interfaces/include/service_types.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md799", null ],
        [ "[interfaces/include/telemetry_events.h]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md800", null ],
        [ "[README.md]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md801", null ],
        [ "[.gitignore]", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md802", null ]
      ] ],
      [ "ძირითადი ცვლილებები", "md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md804", null ]
    ] ],
    [ "CHANGELOG - 2025-06-26", "md_docs_2changelog_2CHANGELOG__2025-06-26.html", [
      [ "[Major] Module System Automation & Refactor", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md807", [
        [ "Added", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md808", null ],
        [ "Changed", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md809", null ],
        [ "Fixed", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md810", null ],
        [ "Detailed File Changes", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md811", null ],
        [ "Why/How", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md812", null ],
        [ "Testing", "md_docs_2changelog_2CHANGELOG__2025-06-26.html#autotoc_md813", null ]
      ] ]
    ] ],
    [ "Changelog", "md_docs_2changelog_2CHANGELOG__2025-06-28.html", [
      [ "[2.0.0] - 2025-06-28", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md815", [
        [ "დამატებულია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md816", null ],
        [ "შეცვლილია", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md817", null ],
        [ "ცნობილი პრობლემები", "md_docs_2changelog_2CHANGELOG__2025-06-28.html#autotoc_md818", null ]
      ] ]
    ] ],
    [ "ცვლილებების ისტორია (Changelog)", "md_docs_2changelog_2v5.html", [
      [ "[5.8.0] - 2025-07-31", "md_docs_2changelog_2v5.html#autotoc_md820", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md821", null ],
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md822", null ]
      ] ],
      [ "[5.7.0] - 2025-07-30", "md_docs_2changelog_2v5.html#autotoc_md824", [
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md825", null ]
      ] ],
      [ "[5.6.0] - 2025-08-16", "md_docs_2changelog_2v5.html#autotoc_md826", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md827", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md828", null ]
      ] ],
      [ "[5.5.1] - 2025-08-16", "md_docs_2changelog_2v5.html#autotoc_md829", [
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md830", null ]
      ] ],
      [ "[5.5.0] - 2025-08-15", "md_docs_2changelog_2v5.html#autotoc_md832", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md833", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md834", null ],
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md835", null ]
      ] ],
      [ "[5.4.0] - 2025-08-14", "md_docs_2changelog_2v5.html#autotoc_md837", [
        [ "✨ დამატებული ფუნქციონალი (Added)", "md_docs_2changelog_2v5.html#autotoc_md838", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული ფუნქციონალი (Changed)", "md_docs_2changelog_2v5.html#autotoc_md839", null ],
        [ "🐛 გამოსწორებული შეცდომები (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md840", null ]
      ] ],
      [ "[5.3.0] - 2025-07-29", "md_docs_2changelog_2v5.html#autotoc_md841", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md842", null ],
        [ "🔄 Changed (შეცვლილი და გაუმჯობესებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md843", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md844", null ]
      ] ],
      [ "[5.2.0] - 2025-07-28", "md_docs_2changelog_2v5.html#autotoc_md846", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md847", null ],
        [ "🔄 Changed (შეცვლილი და გაუმჯობესებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md848", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md849", null ]
      ] ],
      [ "[5.1.0] - 2025-07-28", "md_docs_2changelog_2v5.html#autotoc_md851", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md852", null ],
        [ "🔄 Changed (შეცვლილი და გაუმჯობესებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md853", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md854", null ]
      ] ],
      [ "[5.0.1] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md856", [
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md857", null ],
        [ "🔄 Changed (შეცვლილი ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md858", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md859", null ]
      ] ],
      [ "[5.0.0] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md860", [
        [ "💥 Breaking Changes", "md_docs_2changelog_2v5.html#autotoc_md861", null ],
        [ "✨ Added (დამატებული ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md862", null ],
        [ "🔄 Changed (შეცვლილი ფუნქციონალი)", "md_docs_2changelog_2v5.html#autotoc_md863", null ],
        [ "🐛 Fixed (გამოსწორებული შეცდომები)", "md_docs_2changelog_2v5.html#autotoc_md864", null ]
      ] ],
      [ "[4.2.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md865", [
        [ "✨ დამატებულია (Added)", "md_docs_2changelog_2v5.html#autotoc_md866", null ],
        [ "🔄 შეცვლილია (Changed)", "md_docs_2changelog_2v5.html#autotoc_md867", null ],
        [ "🐞 გასწორებულია (Fixed)", "md_docs_2changelog_2v5.html#autotoc_md868", null ]
      ] ],
      [ "[v4.1.0] - 2025-07-19", "md_docs_2changelog_2v5.html#autotoc_md869", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md870", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md871", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md872", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md873", null ]
      ] ],
      [ "[v4.0.0] - 2025-07-19", "md_docs_2changelog_2v5.html#autotoc_md874", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md875", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md876", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md877", null ]
      ] ],
      [ "[v3.9.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md878", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md879", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md880", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md881", null ]
      ] ],
      [ "[v3.8.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md882", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md883", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md884", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md885", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md886", null ]
      ] ],
      [ "[v3.7.0] - 2025-07-18", "md_docs_2changelog_2v5.html#autotoc_md887", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md888", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md889", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md890", null ]
      ] ],
      [ "[v3.6.0] - 2025-07-17", "md_docs_2changelog_2v5.html#autotoc_md891", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md892", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md893", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md894", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md895", null ]
      ] ],
      [ "[v3.5.0] - 2025-07-16", "md_docs_2changelog_2v5.html#autotoc_md896", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md897", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md898", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md899", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md900", null ]
      ] ],
      [ "[v3.4.0] - 2025-07-13", "md_docs_2changelog_2v5.html#autotoc_md901", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md902", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md903", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md904", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md905", null ]
      ] ],
      [ "[v3.3.0] - 2025-07-10", "md_docs_2changelog_2v5.html#autotoc_md906", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md907", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md908", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md909", null ]
      ] ],
      [ "[v3.2.1] - 2025-07-08", "md_docs_2changelog_2v5.html#autotoc_md910", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md911", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md912", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v5.html#autotoc_md913", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md914", null ]
      ] ],
      [ "[v3.2.0] - 2025-07-08", "md_docs_2changelog_2v5.html#autotoc_md915", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md916", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md917", null ]
      ] ],
      [ "[v3.1.0] - 2025-07-06", "md_docs_2changelog_2v5.html#autotoc_md918", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md919", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md920", null ]
      ] ],
      [ "[v3.0.0] - 2025-07-05", "md_docs_2changelog_2v5.html#autotoc_md921", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md922", null ],
        [ "♻️ რეფაქტორინგი და გაუმჯობესებები (Refactoring & Improvements)", "md_docs_2changelog_2v5.html#autotoc_md923", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md924", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md925", null ]
      ] ],
      [ "[2.8.0] - 2025-07-05", "md_docs_2changelog_2v5.html#autotoc_md926", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md927", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md928", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md929", null ]
      ] ],
      [ "[2.7.0] - 2025-07-05", "md_docs_2changelog_2v5.html#autotoc_md930", [
        [ "✨ ახალი ფუნქციები (Features)", "md_docs_2changelog_2v5.html#autotoc_md931", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v5.html#autotoc_md932", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v5.html#autotoc_md933", null ]
      ] ],
      [ "[2.6.0] - 2025-07-04", "md_docs_2changelog_2v5.html#autotoc_md934", [
        [ "✨ დამატებულია", "md_docs_2changelog_2v5.html#autotoc_md935", null ],
        [ "🛠️ შეცვლილია", "md_docs_2changelog_2v5.html#autotoc_md936", null ],
        [ "🐛 გასწორებულია", "md_docs_2changelog_2v5.html#autotoc_md937", null ]
      ] ],
      [ "[v2.5.0] - 2025-07-02", "md_docs_2changelog_2v5.html#autotoc_md938", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md939", null ]
      ] ],
      [ "[v2.4.0] - 2025-07-02", "md_docs_2changelog_2v5.html#autotoc_md940", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md941", null ],
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md942", null ]
      ] ],
      [ "[v2.3.0] - 2025-07-02", "md_docs_2changelog_2v5.html#autotoc_md944", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md945", null ],
        [ "🏗️ ინფრასტრუქტურა", "md_docs_2changelog_2v5.html#autotoc_md946", null ]
      ] ],
      [ "[v2.2.0] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md947", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md948", null ],
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md949", null ]
      ] ],
      [ "[v2.1.1] - 2025-07-01", "md_docs_2changelog_2v5.html#autotoc_md950", [
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md951", null ],
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md952", null ]
      ] ],
      [ "[v2.1.0] - 2025-06-30", "md_docs_2changelog_2v5.html#autotoc_md954", [
        [ "✨ ახალი ფუნქციონალი", "md_docs_2changelog_2v5.html#autotoc_md955", null ],
        [ "🐛 შეცდომების გასწორება", "md_docs_2changelog_2v5.html#autotoc_md956", null ],
        [ "♻️ რეფაქტორინგი", "md_docs_2changelog_2v5.html#autotoc_md957", null ]
      ] ]
    ] ],
    [ "CHANGELOG", "md_docs_2changelog_2v6.html", [
      [ "[6.2.0] - 2025-08-24", "md_docs_2changelog_2v6.html#autotoc_md959", [
        [ "✨ დამატებული", "md_docs_2changelog_2v6.html#autotoc_md960", null ],
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v6.html#autotoc_md961", null ]
      ] ],
      [ "[6.1.0] - 2025-08-22", "md_docs_2changelog_2v6.html#autotoc_md963", null ],
      [ "✨ დამატებული", "md_docs_2changelog_2v6.html#autotoc_md964", [
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v6.html#autotoc_md965", null ],
        [ "📄 დოკუმენტაცია", "md_docs_2changelog_2v6.html#autotoc_md966", null ]
      ] ],
      [ "[6.0.0] - 2025-08-21", "md_docs_2changelog_2v6.html#autotoc_md968", [
        [ "✨ დამატებული", "md_docs_2changelog_2v6.html#autotoc_md969", null ],
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v6.html#autotoc_md970", null ],
        [ "📄 დოკუმენტაცია", "md_docs_2changelog_2v6.html#autotoc_md971", null ]
      ] ]
    ] ],
    [ "[7.2.0] - 2025-08-28", "md_docs_2changelog_2v7.html", [
      [ "[7.1.0] - 2025-08-27", "md_docs_2changelog_2v7.html#autotoc_md979", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v7.html#autotoc_md974", null ],
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v7.html#autotoc_md975", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v7.html#autotoc_md976", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v7.html#autotoc_md977", null ],
        [ "✨ გაუმჯობესებები (Improvements)", "md_docs_2changelog_2v7.html#autotoc_md980", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v7.html#autotoc_md981", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v7.html#autotoc_md982", null ]
      ] ],
      [ "[7.0.0] - 2025-08-26", "md_docs_2changelog_2v7.html#autotoc_md984", [
        [ "💥 არქიტექტურული ცვლილებები (Breaking Changes & Refactoring)", "md_docs_2changelog_2v7.html#autotoc_md985", null ],
        [ "🔄 შეცვლილი", "md_docs_2changelog_2v7.html#autotoc_md986", [
          [ "ძველი ვერსიების ისტორია", "md_docs_2changelog_2v7.html#autotoc_md988", null ]
        ] ]
      ] ]
    ] ],
    [ "[8.1.0] - 2025-08-30", "md_docs_2changelog_2v8.html", [
      [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v8.html#autotoc_md990", [
        [ "🔄 შეცვლილი და გაუმჯობესებული (Changed & Improved)", "md_docs_2changelog_2v8.html#autotoc_md991", null ],
        [ "🐛 შესწორებები (Bug Fixes)", "md_docs_2changelog_2v8.html#autotoc_md992", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v8.html#autotoc_md993", null ]
      ] ],
      [ "[8.0.0] - 2025-08-07", "md_docs_2changelog_2v8.html#autotoc_md995", [
        [ "✨ არქიტექტურული გაუმჯობესებები (Architectural Improvements)", "md_docs_2changelog_2v8.html#autotoc_md996", null ],
        [ "📄 დოკუმენტაცია (Documentation)", "md_docs_2changelog_2v8.html#autotoc_md997", null ]
      ] ]
    ] ],
    [ "Synapse Branching Strategy", "md_docs_2contributing_2branching__strategy.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2branching__strategy.html#autotoc_md1001", null ],
      [ "ძირითადი ბრენჩები", "md_docs_2contributing_2branching__strategy.html#autotoc_md1003", null ],
      [ "ბრენჩის სახელდების წესები", "md_docs_2contributing_2branching__strategy.html#autotoc_md1005", null ],
      [ "სამუშაო პროცესი", "md_docs_2contributing_2branching__strategy.html#autotoc_md1007", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2contributing_2branching__strategy.html#autotoc_md1009", null ],
      [ "რესურსები", "md_docs_2contributing_2branching__strategy.html#autotoc_md1011", null ]
    ] ],
    [ "ქცევის კოდექსი (Code of Conduct)", "md_docs_2contributing_2code__of__conduct.html", [
      [ "მიზანი", "md_docs_2contributing_2code__of__conduct.html#autotoc_md1015", null ],
      [ "ძირითადი პრინციპები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md1017", null ],
      [ "დაუშვებელი ქცევა", "md_docs_2contributing_2code__of__conduct.html#autotoc_md1019", null ],
      [ "პასუხისმგებლობები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md1021", null ],
      [ "დარღვევის შემთხვევაში მოქმედება", "md_docs_2contributing_2code__of__conduct.html#autotoc_md1023", null ],
      [ "დამატებითი რესურსები", "md_docs_2contributing_2code__of__conduct.html#autotoc_md1025", null ]
    ] ],
    [ "Synapse Commit Message Guidelines", "md_docs_2contributing_2commit__message__guidelines.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md1029", null ],
      [ "ძირითადი სტრუქტურა", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md1031", null ],
      [ "სათაურის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md1033", null ],
      [ "დეტალური აღწერის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md1035", null ],
      [ "Footer-ის წერის წესები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md1037", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md1039", null ],
      [ "რესურსები", "md_docs_2contributing_2commit__message__guidelines.html#autotoc_md1041", null ]
    ] ],
    [ "კონტრიბუციის სახელმძღვანელო (Contributing Guide)", "md_docs_2contributing_2contributing__guide.html", [
      [ "მიზანი", "md_docs_2contributing_2contributing__guide.html#autotoc_md1044", null ],
      [ "1. კონტრიბუციის პროცესი", "md_docs_2contributing_2contributing__guide.html#autotoc_md1046", null ],
      [ "2. კოდის სტანდარტები", "md_docs_2contributing_2contributing__guide.html#autotoc_md1047", null ],
      [ "3. დოკუმენტაციის მოთხოვნები", "md_docs_2contributing_2contributing__guide.html#autotoc_md1048", null ],
      [ "4. Pull Request-ის მიმოხილვა", "md_docs_2contributing_2contributing__guide.html#autotoc_md1049", null ],
      [ "5. Best Practices", "md_docs_2contributing_2contributing__guide.html#autotoc_md1050", null ],
      [ "6. აკრძალული პრაქტიკები", "md_docs_2contributing_2contributing__guide.html#autotoc_md1051", null ]
    ] ],
    [ "Synapse Contributing Reference Index", "md_docs_2contributing_2contributing__index.html", [
      [ "ძირითადი სახელმძღვანელოები", "md_docs_2contributing_2contributing__index.html#autotoc_md1055", null ],
      [ "შაბლონები", "md_docs_2contributing_2contributing__index.html#autotoc_md1056", null ]
    ] ],
    [ "კონვენციების ავტომატური შემმოწმებელი (<tt>check_conventions.py</tt>)", "md_docs_2contributing_2convention__linter__guide.html", [
      [ "1. შესავალი", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md1059", null ],
      [ "2. რას ამოწმებს სკრიპტი?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md1060", null ],
      [ "3. როგორ გამოვიყენოთ ლოკალურად?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md1065", null ],
      [ "4. ინტეგრაცია GitHub Actions-თან (ავტომატური შემოწმება)", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md1067", null ],
      [ "5. როგორ გამოვასწოროთ გავრცელებული შეცდომები?", "md_docs_2contributing_2convention__linter__guide.html#autotoc_md1068", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (FAQ)", "md_docs_2contributing_2faq.html", [
      [ "1. როგორ დავიწყო კონტრიბუცია Synapse Framework-ში?", "md_docs_2contributing_2faq.html#autotoc_md1071", null ],
      [ "2. რა სტანდარტებს და კონვენციებს უნდა დავიცვა?", "md_docs_2contributing_2faq.html#autotoc_md1072", null ],
      [ "3. როგორ შევქმნა Pull Request?", "md_docs_2contributing_2faq.html#autotoc_md1073", null ],
      [ "4. როგორ მოვამზადო ლოკალური გარემო?", "md_docs_2contributing_2faq.html#autotoc_md1074", null ],
      [ "5. როგორ გავუშვა ტესტები?", "md_docs_2contributing_2faq.html#autotoc_md1075", null ],
      [ "6. როგორ მოვაგვარო გარემოს ან ბილდის პრობლემები?", "md_docs_2contributing_2faq.html#autotoc_md1076", null ],
      [ "7. სად ვიპოვო დოკუმენტაციის სტრუქტურის და სტანდარტების შესახებ ინფორმაცია?", "md_docs_2contributing_2faq.html#autotoc_md1077", null ],
      [ "8. როგორ დავამატო ან განვაახლო დოკუმენტაცია?", "md_docs_2contributing_2faq.html#autotoc_md1078", null ],
      [ "9. როგორ დავიცვა ქცევის კოდექსი?", "md_docs_2contributing_2faq.html#autotoc_md1079", null ],
      [ "10. ვის მივმართო დამატებითი კითხვების ან პრობლემების შემთხვევაში?", "md_docs_2contributing_2faq.html#autotoc_md1080", null ]
    ] ],
    [ "Synapse Issue Template", "md_docs_2contributing_2issue__template.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2issue__template.html#autotoc_md1084", null ],
      [ "ძირითადი ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md1086", null ],
      [ "დეტალური აღწერა", "md_docs_2contributing_2issue__template.html#autotoc_md1088", null ],
      [ "გამეორების ნაბიჯები (თუ ეს ბაგია)", "md_docs_2contributing_2issue__template.html#autotoc_md1090", null ],
      [ "მოსალოდნელი შედეგი", "md_docs_2contributing_2issue__template.html#autotoc_md1092", null ],
      [ "ფაქტობრივი შედეგი", "md_docs_2contributing_2issue__template.html#autotoc_md1094", null ],
      [ "გარემოს ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md1096", null ],
      [ "დამატებითი ინფორმაცია", "md_docs_2contributing_2issue__template.html#autotoc_md1098", null ],
      [ "რესურსები", "md_docs_2contributing_2issue__template.html#autotoc_md1100", null ]
    ] ],
    [ "Synapse Local Setup Guide", "md_docs_2contributing_2local__setup.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2local__setup.html#autotoc_md1104", null ],
      [ "სისტემური მოთხოვნები", "md_docs_2contributing_2local__setup.html#autotoc_md1106", null ],
      [ "ESP-IDF-ის ინსტალაცია", "md_docs_2contributing_2local__setup.html#autotoc_md1108", null ],
      [ "პროექტის კლონირება", "md_docs_2contributing_2local__setup.html#autotoc_md1110", null ],
      [ "sdkconfig ფაილის კონფიგურაცია", "md_docs_2contributing_2local__setup.html#autotoc_md1112", null ],
      [ "პროექტის აგება და გაშვება", "md_docs_2contributing_2local__setup.html#autotoc_md1114", null ],
      [ "ტესტირება", "md_docs_2contributing_2local__setup.html#autotoc_md1116", null ],
      [ "პრობლემების შემთხვევაში", "md_docs_2contributing_2local__setup.html#autotoc_md1118", null ],
      [ "რესურსები", "md_docs_2contributing_2local__setup.html#autotoc_md1120", null ]
    ] ],
    [ "Synapse Pull Request Template", "md_docs_2contributing_2pull__request__template.html", [
      [ "მიმოხილვა", "md_docs_2contributing_2pull__request__template.html#autotoc_md1124", null ],
      [ "ძირითადი ინფორმაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md1126", null ],
      [ "ცვლილებების დეტალური აღწერა", "md_docs_2contributing_2pull__request__template.html#autotoc_md1128", null ],
      [ "ტესტირება და ვალიდაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md1130", null ],
      [ "მიმოხილვის რეკომენდაციები", "md_docs_2contributing_2pull__request__template.html#autotoc_md1132", null ],
      [ "დამატებითი ინფორმაცია", "md_docs_2contributing_2pull__request__template.html#autotoc_md1134", null ],
      [ "რესურსები", "md_docs_2contributing_2pull__request__template.html#autotoc_md1136", null ]
    ] ],
    [ "Synapse Review Guidelines", "md_docs_2contributing_2review__guidelines.html", [
      [ "მიმოხილვის მიზანი", "md_docs_2contributing_2review__guidelines.html#autotoc_md1140", null ],
      [ "მიმოხილვის პროცესი", "md_docs_2contributing_2review__guidelines.html#autotoc_md1142", null ],
      [ "მიმოხილვის კრიტერიუმები", "md_docs_2contributing_2review__guidelines.html#autotoc_md1144", null ],
      [ "საუკეთესო პრაქტიკები", "md_docs_2contributing_2review__guidelines.html#autotoc_md1146", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2contributing_2review__guidelines.html#autotoc_md1148", null ],
      [ "რესურსები", "md_docs_2contributing_2review__guidelines.html#autotoc_md1150", null ]
    ] ],
    [ "9. Doxygen კომენტარების სტანდარტი (v1.0)", "md_docs_2convention_2coding__doxygen__standard.html", [
      [ "1. შესავალი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1153", null ],
      [ "2. ფაილის სათაურის (Header) კომენტარი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1154", [
        [ "2.1. სტრუქტურა", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1155", null ],
        [ "2.2. მაგალითი (<tt>i2c_bus_module.h</tt>)", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1156", null ]
      ] ],
      [ "3. ფუნქციის კომენტარი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1157", [
        [ "3.1. სტრუქტურა", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1158", null ],
        [ "3.2. მაგალითი (<tt>service_locator.h</tt>)", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1159", null ]
      ] ],
      [ "4. სტრუქტურების და ტიპების კომენტარები", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1160", [
        [ "4.1. სტრუქტურის მაგალითი", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1161", null ]
      ] ],
      [ "5. Doxygen ტეგების გამოყენების წესები", "md_docs_2convention_2coding__doxygen__standard.html#autotoc_md1162", null ]
    ] ],
    [ "კომუნიკაციის პატერნების წესები", "md_docs_2convention_2communication__patterns.html", [
      [ "ძირითადი პრინციპი: მაქსიმალური იზოლაცია", "md_docs_2convention_2communication__patterns.html#autotoc_md1164", null ],
      [ "სამი ძირითადი პატერნი", "md_docs_2convention_2communication__patterns.html#autotoc_md1165", [
        [ "1. Service Locator Pattern", "md_docs_2convention_2communication__patterns.html#autotoc_md1166", null ],
        [ "2. Event Bus Pattern", "md_docs_2convention_2communication__patterns.html#autotoc_md1167", null ],
        [ "3. Command Router Pattern", "md_docs_2convention_2communication__patterns.html#autotoc_md1168", null ],
        [ "4. Promise Pattern (ახალი)", "md_docs_2convention_2communication__patterns.html#autotoc_md1169", null ]
      ] ],
      [ "კომუნიკაციის ტიპები", "md_docs_2convention_2communication__patterns.html#autotoc_md1170", [
        [ "Direct API Communication (Service Locator)", "md_docs_2convention_2communication__patterns.html#autotoc_md1171", null ],
        [ "Broadcast Communication (Event Bus)", "md_docs_2convention_2communication__patterns.html#autotoc_md1172", null ]
      ] ],
      [ "აკრძალული პატერნები", "md_docs_2convention_2communication__patterns.html#autotoc_md1173", [
        [ "❌ პირდაპირი დამოკიდებულებები", "md_docs_2convention_2communication__patterns.html#autotoc_md1174", null ],
        [ "❌ პატერნების არასწორი შერევა", "md_docs_2convention_2communication__patterns.html#autotoc_md1175", null ],
        [ "❌ System Manager MQTT Handling", "md_docs_2convention_2communication__patterns.html#autotoc_md1176", null ]
      ] ],
      [ "სწორი იმპლემენტაცია", "md_docs_2convention_2communication__patterns.html#autotoc_md1177", [
        [ "Service Locator-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md1178", [
          [ "1. Service Registration (Display Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md1179", null ],
          [ "2. Service Usage (MQTT Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md1180", null ]
        ] ],
        [ "Event Bus-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md1181", [
          [ "1. Event Publishing (Sensor Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md1182", null ],
          [ "2. Event Subscription (MQTT Module)", "md_docs_2convention_2communication__patterns.html#autotoc_md1183", null ]
        ] ]
      ] ],
      [ "დინამიური MQTT თემები <tt>{module_name}</tt> Placeholder-ით", "md_docs_2convention_2communication__patterns.html#autotoc_md1184", [
        [ "პატერნის აღწერა", "md_docs_2convention_2communication__patterns.html#autotoc_md1185", null ]
      ] ],
      [ "პატერნის არჩევის გზამკვლევი", "md_docs_2convention_2communication__patterns.html#autotoc_md1186", [
        [ "Service Locator-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md1187", null ],
        [ "Event Bus-ის გამოყენება", "md_docs_2convention_2communication__patterns.html#autotoc_md1188", null ]
      ] ],
      [ "Architecture Diagram", "md_docs_2convention_2communication__patterns.html#autotoc_md1189", null ],
      [ "შეჯამება", "md_docs_2convention_2communication__patterns.html#autotoc_md1190", null ]
    ] ],
    [ "კონფიგურაციის მართვის კონვენცია", "md_docs_2convention_2configuration__management.html", [
      [ "1. 📜 ძირითადი პრინციპი", "md_docs_2convention_2configuration__management.html#autotoc_md1192", null ],
      [ "2. 📂 კონფიგურაციის წყაროები და იერარქია", "md_docs_2convention_2configuration__management.html#autotoc_md1193", null ],
      [ "3. 🏗️ კონფიგურაციის სტრუქტურა", "md_docs_2convention_2configuration__management.html#autotoc_md1194", null ],
      [ "4. 📝 კონფიგურაციის პარსინგის პატერნი", "md_docs_2convention_2configuration__management.html#autotoc_md1195", [
        [ "<tt>parse_config</tt> ფუნქციის 3-ნაბიჯიანი ლოგიკა", "md_docs_2convention_2configuration__management.html#autotoc_md1196", null ],
        [ "<tt>parse_config</tt> ფუნქციის მაგალითი", "md_docs_2convention_2configuration__management.html#autotoc_md1197", null ]
      ] ],
      [ "5. 🌐 კონფიგურაციაზე წვდომა სხვა მოდულებიდან", "md_docs_2convention_2configuration__management.html#autotoc_md1198", null ],
      [ "6. ❌ აკრძალული პრაქტიკები", "md_docs_2convention_2configuration__management.html#autotoc_md1199", null ],
      [ "7. ✅ საუკეთესო პრაქტიკები", "md_docs_2convention_2configuration__management.html#autotoc_md1200", null ]
    ] ],
    [ "Synapse Convention Reference Index", "md_docs_2convention_2convention__index.html", [
      [ "Coding & Documentation", "md_docs_2convention_2convention__index.html#autotoc_md1203", null ],
      [ "Naming & Structure", "md_docs_2convention_2convention__index.html#autotoc_md1204", null ],
      [ "Patterns & Management", "md_docs_2convention_2convention__index.html#autotoc_md1205", null ]
    ] ],
    [ "Error Handling-ის წესები", "md_docs_2convention_2error__handling.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2error__handling.html#autotoc_md1208", [
        [ "1. ESP Error Codes გამოყენება", "md_docs_2convention_2error__handling.html#autotoc_md1209", null ],
        [ "2. Comprehensive Error Checking", "md_docs_2convention_2error__handling.html#autotoc_md1210", null ]
      ] ],
      [ "Standard Error Codes", "md_docs_2convention_2error__handling.html#autotoc_md1211", [
        [ "Framework-Specific Errors", "md_docs_2convention_2error__handling.html#autotoc_md1212", null ],
        [ "Common ESP Error Codes", "md_docs_2convention_2error__handling.html#autotoc_md1213", null ]
      ] ],
      [ "Error Handling Patterns", "md_docs_2convention_2error__handling.html#autotoc_md1214", [
        [ "1. Parameter Validation", "md_docs_2convention_2error__handling.html#autotoc_md1215", null ],
        [ "2. Resource Allocation Checking", "md_docs_2convention_2error__handling.html#autotoc_md1216", null ],
        [ "3. Service Locator Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md1217", null ],
        [ "4. Configuration Parsing Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md1218", null ],
        [ "5. Hardware Operation Error Handling", "md_docs_2convention_2error__handling.html#autotoc_md1219", null ]
      ] ],
      [ "Cleanup and Resource Management", "md_docs_2convention_2error__handling.html#autotoc_md1220", [
        [ "1. Proper Cleanup on Errors", "md_docs_2convention_2error__handling.html#autotoc_md1221", null ],
        [ "2. Resource Cleanup Functions", "md_docs_2convention_2error__handling.html#autotoc_md1222", null ]
      ] ],
      [ "Logging Standards", "md_docs_2convention_2error__handling.html#autotoc_md1223", [
        [ "1. Error Levels", "md_docs_2convention_2error__handling.html#autotoc_md1224", null ],
        [ "2. Error Context Information", "md_docs_2convention_2error__handling.html#autotoc_md1225", null ]
      ] ],
      [ "Error Recovery Strategies", "md_docs_2convention_2error__handling.html#autotoc_md1226", [
        [ "1. Retry Logic", "md_docs_2convention_2error__handling.html#autotoc_md1227", null ],
        [ "2. Graceful Degradation", "md_docs_2convention_2error__handling.html#autotoc_md1228", null ]
      ] ],
      [ "შეჯამება", "md_docs_2convention_2error__handling.html#autotoc_md1229", null ]
    ] ],
    [ "ფუნქციების დასახელების წესები", "md_docs_2convention_2function__naming.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2function__naming.html#autotoc_md1231", [
        [ "1. სტრუქტურა", "md_docs_2convention_2function__naming.html#autotoc_md1232", null ],
        [ "2. Scope პრეფიქსები", "md_docs_2convention_2function__naming.html#autotoc_md1233", [
          [ "Public API ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1234", null ],
          [ "Static/Private ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1235", null ],
          [ "Framework API ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1236", null ]
        ] ]
      ] ],
      [ "მოდულის ტიპების მიხედვით", "md_docs_2convention_2function__naming.html#autotoc_md1237", [
        [ "1. Service Locator", "md_docs_2convention_2function__naming.html#autotoc_md1238", null ],
        [ "2. Event Bus", "md_docs_2convention_2function__naming.html#autotoc_md1239", null ],
        [ "3. Display Modules", "md_docs_2convention_2function__naming.html#autotoc_md1240", null ],
        [ "4. Communication Modules", "md_docs_2convention_2function__naming.html#autotoc_md1241", null ],
        [ "5. Sensor Modules", "md_docs_2convention_2function__naming.html#autotoc_md1242", null ]
      ] ],
      [ "სპეციალური კონვენციები", "md_docs_2convention_2function__naming.html#autotoc_md1243", [
        [ "1. Module Lifecycle ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1244", null ],
        [ "2. Runtime Management ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1245", null ],
        [ "3. Event Handler ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1246", null ],
        [ "4. Task ფუნქციები", "md_docs_2convention_2function__naming.html#autotoc_md1247", null ]
      ] ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2function__naming.html#autotoc_md1248", null ],
      [ "მაგალითი სრული მოდული", "md_docs_2convention_2function__naming.html#autotoc_md1249", null ]
    ] ],
    [ "🚀 მოდულების ინიციალიზაციის დონეები (Initialization Levels)", "md_docs_2convention_2initialization__levels.html", [
      [ "🎯 მიზანი", "md_docs_2convention_2initialization__levels.html#autotoc_md1251", null ],
      [ "⚙️ როგორ მუშაობს?", "md_docs_2convention_2initialization__levels.html#autotoc_md1252", null ],
      [ "📜 ოქროს წესი", "md_docs_2convention_2initialization__levels.html#autotoc_md1253", [
        [ "💡 პრაქტიკული მაგალითი: <tt>wifi_manager</tt> და <tt>command_router</tt>", "md_docs_2convention_2initialization__levels.html#autotoc_md1254", null ]
      ] ],
      [ "📊 ინიციალიზაციის დონეების რეკომენდებული დიაპაზონები", "md_docs_2convention_2initialization__levels.html#autotoc_md1255", [
        [ "📦 <strong>დონე 1: ფუნდამენტური დრაივერები და რესურსები (0-19)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1257", null ],
        [ "🛠️ <strong>დონე 2: საბაზისო სისტემური სერვისები (20-39)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1259", null ],
        [ "📡 <strong>დონე 3: ქსელი, კონტროლი და კომუნიკაცია (40-59)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1261", null ],
        [ "💡 <strong>დონე 4: აპლიკაციის ლოგიკა და პერიფერია (60-79)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1263", null ],
        [ "🧩 <strong>დონე 5: მაღალი დონის კომპოზიტური მოდულები (80-100)</strong>", "md_docs_2convention_2initialization__levels.html#autotoc_md1265", null ]
      ] ],
      [ "📝 როგორ განვსაზღვროთ <tt>init_level</tt> ახალი მოდულისთვის?", "md_docs_2convention_2initialization__levels.html#autotoc_md1267", null ]
    ] ],
    [ "ლოგირების ფერთა სქემის კონვენცია", "md_docs_2convention_2logging__colors.html", [
      [ "1. 🎯 მიზანი", "md_docs_2convention_2logging__colors.html#autotoc_md1269", null ],
      [ "2. ⚙️ ტექნიკური იმპლემენტაცია", "md_docs_2convention_2logging__colors.html#autotoc_md1270", null ],
      [ "3. 🎨 ფერთა სქემა", "md_docs_2convention_2logging__colors.html#autotoc_md1271", [
        [ "**სრული ფერების სია (<tt>logging.h</tt>-დან)**", "md_docs_2convention_2logging__colors.html#autotoc_md1273", null ]
      ] ]
    ] ],
    [ "Logging-ის სტანდარტები", "md_docs_2convention_2logging__standards.html", [
      [ "Component Tag Definition", "md_docs_2convention_2logging__standards.html#autotoc_md1275", [
        [ "სტანდარტული ფორმატი", "md_docs_2convention_2logging__standards.html#autotoc_md1276", null ],
        [ "მაგალითები", "md_docs_2convention_2logging__standards.html#autotoc_md1277", null ]
      ] ],
      [ "Log Levels", "md_docs_2convention_2logging__standards.html#autotoc_md1278", [
        [ "1. ESP_LOGE - Error Level", "md_docs_2convention_2logging__standards.html#autotoc_md1279", null ],
        [ "2. ESP_LOGW - Warning Level", "md_docs_2convention_2logging__standards.html#autotoc_md1280", null ],
        [ "3. ESP_LOGI - Info Level", "md_docs_2convention_2logging__standards.html#autotoc_md1281", null ],
        [ "4. ESP_LOGD - Debug Level", "md_docs_2convention_2logging__standards.html#autotoc_md1282", null ],
        [ "5. ESP_LOGV - Verbose Level", "md_docs_2convention_2logging__standards.html#autotoc_md1283", null ]
      ] ],
      [ "Logging Patterns", "md_docs_2convention_2logging__standards.html#autotoc_md1284", [
        [ "1. Module Lifecycle Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1285", null ],
        [ "2. Service Registration Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1286", null ],
        [ "3. Configuration Parsing Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1287", null ],
        [ "4. Hardware Operation Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1288", null ],
        [ "5. Event Handling Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1289", null ],
        [ "6. Error Context Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1290", null ]
      ] ],
      [ "Runtime State Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1291", [
        [ "1. Module Enable/Disable", "md_docs_2convention_2logging__standards.html#autotoc_md1292", null ],
        [ "2. Task State Changes", "md_docs_2convention_2logging__standards.html#autotoc_md1293", null ]
      ] ],
      [ "Performance Logging", "md_docs_2convention_2logging__standards.html#autotoc_md1294", [
        [ "1. Operation Timing", "md_docs_2convention_2logging__standards.html#autotoc_md1295", null ],
        [ "2. Memory Usage", "md_docs_2convention_2logging__standards.html#autotoc_md1296", null ]
      ] ],
      [ "Log Message Format Standards", "md_docs_2convention_2logging__standards.html#autotoc_md1297", [
        [ "1. Consistent Formatting", "md_docs_2convention_2logging__standards.html#autotoc_md1298", null ],
        [ "2. Parameter Naming", "md_docs_2convention_2logging__standards.html#autotoc_md1299", null ]
      ] ],
      [ "აკრძალული პრაქტიკები", "md_docs_2convention_2logging__standards.html#autotoc_md1300", null ],
      [ "შეჯამება", "md_docs_2convention_2logging__standards.html#autotoc_md1301", null ]
    ] ],
    [ "მოდულის სტრუქტურის კონვენცია", "md_docs_2convention_2module__structure.html", [
      [ "1. 📜 მიმოხილვა", "md_docs_2convention_2module__structure.html#autotoc_md1303", null ],
      [ "2. 📂 საქაღალდის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1304", [
        [ "2.1. სტანდარტული მოდულის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1305", null ],
        [ "2.2. კატეგორიები", "md_docs_2convention_2module__structure.html#autotoc_md1306", null ]
      ] ],
      [ "3. 📄 ფაილების დანიშნულება და შინაარსი", "md_docs_2convention_2module__structure.html#autotoc_md1307", [
        [ "3.1. <tt>src/</tt> დირექტორიის დანაწევრება", "md_docs_2convention_2module__structure.html#autotoc_md1308", null ],
        [ "3.2. კონფიგურაციის ფაილები", "md_docs_2convention_2module__structure.html#autotoc_md1309", null ],
        [ "3.3. დოკუმენტაცია", "md_docs_2convention_2module__structure.html#autotoc_md1310", null ]
      ] ],
      [ "4. <tt>CMakeLists.txt</tt>-ის სტრუქტურა", "md_docs_2convention_2module__structure.html#autotoc_md1311", null ],
      [ "5. ❌ აკრძალული პრაქტიკები", "md_docs_2convention_2module__structure.html#autotoc_md1312", null ],
      [ "6. ✅ საუკეთესო პრაქტიკები", "md_docs_2convention_2module__structure.html#autotoc_md1313", null ]
    ] ],
    [ "asynchronous ოპერაციების მართვის პატერნი (Promise Pattern)", "md_docs_2convention_2promise__pattern.html", [
      [ "1. 🎯 მიზანი", "md_docs_2convention_2promise__pattern.html#autotoc_md1315", null ],
      [ "2. 🤔 პრობლემა: რატომ გვჭირდება Promise-ები?", "md_docs_2convention_2promise__pattern.html#autotoc_md1316", null ],
      [ "3. 💡 გადაწყვეტა: Promise პატერნი", "md_docs_2convention_2promise__pattern.html#autotoc_md1317", null ],
      [ "4. ⚙️ როგორ მუშაობს იმპლემენტაცია?", "md_docs_2convention_2promise__pattern.html#autotoc_md1318", null ],
      [ "5. 📝 გამოყენების პატერნი", "md_docs_2convention_2promise__pattern.html#autotoc_md1319", [
        [ "5.1. სერვისის მომწოდებლის მხარე (Provider - მაგ., <tt>wifi_manager</tt>)", "md_docs_2convention_2promise__pattern.html#autotoc_md1320", null ],
        [ "5.2. მომხმარებლის მხარე (Consumer - მაგ., <tt>ui_manager</tt>)", "md_docs_2convention_2promise__pattern.html#autotoc_md1321", null ]
      ] ],
      [ "6. ✅ როდის გამოვიყენოთ Promise და როდის Event Bus?", "md_docs_2convention_2promise__pattern.html#autotoc_md1322", null ]
    ] ],
    [ "რესურსების ოპტიმიზაციის პატერნი (Shared Task Pool)", "md_docs_2convention_2task__pool__pattern.html", [
      [ "1. 🎯 მიზანი", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1324", null ],
      [ "2. 🤔 პრობლემა: \"ერთი მოდული = ერთი ტასკი\" ანტი-პატერნი", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1325", null ],
      [ "3. 💡 გადაწყვეტა: <tt>Task Pool Manager</tt>", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1326", null ],
      [ "4. 📝 გამოყენების პატერნი", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1327", [
        [ "4.1. მოდულის <tt>private_data</tt> სტრუქტურა", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1328", null ],
        [ "4.2. სამუშაო ფუნქციის (<tt>Job</tt>) იმპლემენტაცია", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1329", null ],
        [ "4.3. <tt>_start</tt> და <tt>_deinit</tt> ფუნქციები", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1330", null ]
      ] ],
      [ "5. ✅ როდის გამოვიყენოთ <tt>Task Pool</tt> და როდის საკუთარი ტასკი?", "md_docs_2convention_2task__pool__pattern.html#autotoc_md1331", null ]
    ] ],
    [ "ცვლადების დასახელების წესები", "md_docs_2convention_2variable__naming.html", [
      [ "ძირითადი პრინციპები", "md_docs_2convention_2variable__naming.html#autotoc_md1333", [
        [ "1. დესკრიპტიული სახელები", "md_docs_2convention_2variable__naming.html#autotoc_md1334", null ],
        [ "2. აკრძალული პრეფიქსები", "md_docs_2convention_2variable__naming.html#autotoc_md1335", null ],
        [ "3. სწორი მიდგომა", "md_docs_2convention_2variable__naming.html#autotoc_md1336", [
          [ "Service Locator", "md_docs_2convention_2variable__naming.html#autotoc_md1337", null ],
          [ "Event Bus", "md_docs_2convention_2variable__naming.html#autotoc_md1338", null ],
          [ "Module Registry", "md_docs_2convention_2variable__naming.html#autotoc_md1339", null ],
          [ "Display Modules", "md_docs_2convention_2variable__naming.html#autotoc_md1340", null ]
        ] ],
        [ "4. კონვენციები", "md_docs_2convention_2variable__naming.html#autotoc_md1341", [
          [ "Global ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1342", null ],
          [ "Static ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1343", null ],
          [ "Mutex/Semaphore ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1344", null ],
          [ "Queue ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1345", null ],
          [ "API Structure ცვლადები", "md_docs_2convention_2variable__naming.html#autotoc_md1346", null ]
        ] ]
      ] ],
      [ "მაგალითები", "md_docs_2convention_2variable__naming.html#autotoc_md1347", [
        [ "Config Manager", "md_docs_2convention_2variable__naming.html#autotoc_md1348", null ],
        [ "I2C Bus Module", "md_docs_2convention_2variable__naming.html#autotoc_md1349", null ],
        [ "MQTT Module", "md_docs_2convention_2variable__naming.html#autotoc_md1350", null ]
      ] ],
      [ "Private Data Pointer Naming", "md_docs_2convention_2variable__naming.html#autotoc_md1351", [
        [ "❌ არასწორი", "md_docs_2convention_2variable__naming.html#autotoc_md1352", null ],
        [ "✅ სწორი", "md_docs_2convention_2variable__naming.html#autotoc_md1353", null ]
      ] ],
      [ "შეჯამება", "md_docs_2convention_2variable__naming.html#autotoc_md1354", null ]
    ] ],
    [ "Synapse ESP Framework — არქიტექტურის მიმოხილვა", "md_docs_2design__decisions_2architecture__overview.html", [
      [ "მიზანი", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1356", null ],
      [ "ძირითადი არქიტექტურული პრინციპები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1357", null ],
      [ "არქიტექტურული ფენები და ძირითადი კომპონენტები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1358", null ],
      [ "მოდულებს შორის კომუნიკაციის პატერნები", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1359", null ],
      [ "სიცოცხლის ციკლი", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1360", null ],
      [ "საქაღალდეებისა და ფაილების სტრუქტურა", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1361", null ],
      [ "არქიტექტურული დიაგრამა (მოკლე)", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1362", null ],
      [ "სტანდარტები და დოკუმენტაცია", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1363", null ],
      [ "დასკვნა", "md_docs_2design__decisions_2architecture__overview.html#autotoc_md1364", null ]
    ] ],
    [ "კომუნიკაციის არქიტექტურული არჩევანი (Communication Choices)", "md_docs_2design__decisions_2communication__choices.html", [
      [ "მიზანი", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1367", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1369", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1371", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1373", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1375", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1377", [
        [ "Service Locator", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1378", null ],
        [ "Event Bus", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1379", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1381", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1383", null ],
      [ "Event Bus Pattern", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1385", [
        [ "არქიტექტურული ცვლილება: Enum-დან სტრიქონებზე გადასვლა", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1386", null ],
        [ "ფრეიმვორქის API-სთან წვდომა: Facade Pattern (<tt>synapse.h</tt>)", "md_docs_2design__decisions_2communication__choices.html#autotoc_md1387", null ]
      ] ]
    ] ],
    [ "Core კომპონენტები (Core Components)", "md_docs_2design__decisions_2core__components.html", [
      [ "მიზანი", "md_docs_2design__decisions_2core__components.html#autotoc_md1391", null ],
      [ "ძირითადი კომპონენტები", "md_docs_2design__decisions_2core__components.html#autotoc_md1393", [
        [ "1. Service Locator", "md_docs_2design__decisions_2core__components.html#autotoc_md1394", null ],
        [ "2. Event Bus", "md_docs_2design__decisions_2core__components.html#autotoc_md1395", null ],
        [ "3. Module Register", "md_docs_2design__decisions_2core__components.html#autotoc_md1396", null ],
        [ "4. Module Factory", "md_docs_2design__decisions_2core__components.html#autotoc_md1397", null ],
        [ "5. Configuration Manager", "md_docs_2design__decisions_2core__components.html#autotoc_md1398", null ],
        [ "6. Logging", "md_docs_2design__decisions_2core__components.html#autotoc_md1399", null ],
        [ "7. Promise Manager", "md_docs_2design__decisions_2core__components.html#autotoc_md1400", null ]
      ] ],
      [ "ურთიერთქმედება მოდულებთან", "md_docs_2design__decisions_2core__components.html#autotoc_md1402", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2core__components.html#autotoc_md1404", null ]
    ] ],
    [ "არქიტექტურული გადაწყვეტილების ჩანაწერის შაბლონი (Decision Record Template)", "md_docs_2design__decisions_2decision__record__template.html", [
      [ "გადაწყვეტილების ID", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1408", null ],
      [ "თარიღი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1409", null ],
      [ "სტატუსი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1410", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1411", null ],
      [ "გადაწყვეტილება", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1412", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1413", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1414", null ],
      [ "განხორციელების ნაბიჯები", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1415", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2decision__record__template.html#autotoc_md1416", null ]
    ] ],
    [ "არქიტექტურული გადაწყვეტილება: დამოკიდებულებების ინექცია (Dependency Injection)", "md_docs_2design__decisions_2dependency__injection.html", [
      [ "1. კონტექსტი", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1419", null ],
      [ "2. გადაწყვეტილება", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1420", null ],
      [ "3. არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1421", null ],
      [ "4. შედეგები და გავლენა", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1422", [
        [ "4.1. კონვენცია მოდულის დეველოპერებისთვის", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1423", null ],
        [ "4.2. გამოყენების მაგალითი", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1424", null ]
      ] ],
      [ "5. დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2dependency__injection.html#autotoc_md1425", null ]
    ] ],
    [ "Synapse Design Decisions", "md_docs_2design__decisions_2design__decisions__index.html", [
      [ "ძირითადი დოკუმენტები", "md_docs_2design__decisions_2design__decisions__index.html#autotoc_md1428", null ]
    ] ],
    [ "შეცდომების დამუშავების არქიტექტურული გადაწყვეტილებები (Error Handling Decisions)", "md_docs_2design__decisions_2error__handling__decisions.html", [
      [ "მიზანი", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1432", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1434", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1436", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1438", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1440", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1442", [
        [ "პარამეტრების ვალიდაცია", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1443", null ],
        [ "რესურსების გამოყოფის შემოწმება", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1444", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1446", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2error__handling__decisions.html#autotoc_md1448", null ]
    ] ],
    [ "გაფართოებადობისა და მოდულარობის არქიტექტურული გადაწყვეტილებები (Extensibility & Modularity)", "md_docs_2design__decisions_2extensibility__and__modularity.html", [
      [ "მიზანი", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1452", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1454", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1456", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1458", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1460", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1462", [
        [ "ახალი მოდულის დამატება", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1463", null ],
        [ "Public API-ის გამოყენება", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1464", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1466", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2extensibility__and__modularity.html#autotoc_md1468", null ]
    ] ],
    [ "მიგრაციისა და თავსებადობის არქიტექტურული გადაწყვეტილებები (Migration & Compatibility)", "md_docs_2design__decisions_2migration__and__compatibility.html", [
      [ "მიზანი", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1472", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1474", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1476", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1478", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1480", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1482", [
        [ "სქემის ვერსიონირება", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1483", null ],
        [ "Deprecated API-ის მონიშვნა", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1484", null ],
        [ "მიგრაციის სკრიპტის გამოყენება", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1485", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1487", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2migration__and__compatibility.html#autotoc_md1489", null ]
    ] ],
    [ "წარმადობისა და რესურსების არქიტექტურული კომპრომისები (Performance Trade-offs)", "md_docs_2design__decisions_2performance__tradeoffs.html", [
      [ "მიზანი", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1493", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1495", null ],
      [ "არჩეული სტრატეგიები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1497", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1499", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1501", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1503", [
        [ "მოდულის გამორთვა ბილდიდან", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1504", null ],
        [ "Static allocation", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1505", null ],
        [ "Lazy initialization", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1506", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1508", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2performance__tradeoffs.html#autotoc_md1510", null ]
    ] ],
    [ "უსაფრთხოების არქიტექტურული გადაწყვეტილებები (Security Considerations)", "md_docs_2design__decisions_2security__considerations.html", [
      [ "მიზანი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1514", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1516", null ],
      [ "არჩეული გადაწყვეტილებები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1518", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1520", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1522", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1524", [
        [ "TLS/SSL ჩართვა MQTT-ში", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1525", null ],
        [ "OTA განახლების სიგნირება", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1526", null ],
        [ "ლოგირების კონტროლი", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1527", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1529", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2security__considerations.html#autotoc_md1531", null ]
    ] ],
    [ "მონაცემთა შენახვის არქიტექტურული არჩევანი (Storage Strategy)", "md_docs_2design__decisions_2storage__strategy.html", [
      [ "მიზანი", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1535", null ],
      [ "კონტექსტი", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1537", null ],
      [ "არჩეული გადაწყვეტილება", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1539", null ],
      [ "არგუმენტები და მოტივაცია", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1541", null ],
      [ "შედეგები და გავლენა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1543", null ],
      [ "გამოყენების მაგალითები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1545", [
        [ "NVS-ში სტრიქონის შენახვა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1546", null ],
        [ "NVS-დან სტრიქონის წაკითხვა", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1547", null ],
        [ "Flash-ში მონაცემების შენახვა (OTA)", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1548", null ]
      ] ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1550", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2design__decisions_2storage__strategy.html#autotoc_md1552", null ]
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
      [ "მიზანი", "md_docs_2glossary_2glossary__api.html#autotoc_md1556", null ]
    ] ],
    [ "არქიტექტურული ტერმინები (Glossary — Architecture)", "md_docs_2glossary_2glossary__architecture.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__architecture.html#autotoc_md1561", null ]
    ] ],
    [ "კონფიგურაციისა და შენახვის ტერმინები (Glossary — Configuration & Storage)", "md_docs_2glossary_2glossary__config.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__config.html#autotoc_md1566", null ]
    ] ],
    [ "Glossary (ტერმინები)", "md_docs_2glossary_2glossary__index.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__index.html#autotoc_md1571", null ],
      [ "თემატური ტერმინების დოკუმენტები", "md_docs_2glossary_2glossary__index.html#autotoc_md1573", null ],
      [ "გამოყენების წესი", "md_docs_2glossary_2glossary__index.html#autotoc_md1575", null ]
    ] ],
    [ "უსაფრთხოების და წარმადობის ტერმინები (Glossary — Security & Performance)", "md_docs_2glossary_2glossary__security.html", [
      [ "მიზანი", "md_docs_2glossary_2glossary__security.html#autotoc_md1579", null ]
    ] ],
    [ "Cloud ინტეგრაციის სახელმძღვანელო (Integration — Cloud)", "md_docs_2integration_2integration__cloud.html", [
      [ "მიზანი", "md_docs_2integration_2integration__cloud.html#autotoc_md1584", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__cloud.html#autotoc_md1586", null ],
      [ "Best Practices", "md_docs_2integration_2integration__cloud.html#autotoc_md1588", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__cloud.html#autotoc_md1590", null ]
    ] ],
    [ "Integration Index (ინტეგრაციის სახელმძღვანელოების ინდექსი)", "md_docs_2integration_2integration__index.html", [
      [ "მიზანი", "md_docs_2integration_2integration__index.html#autotoc_md1594", null ],
      [ "თემატური ინტეგრაციის სახელმძღვანელოები", "md_docs_2integration_2integration__index.html#autotoc_md1596", null ],
      [ "გამოყენების წესი", "md_docs_2integration_2integration__index.html#autotoc_md1598", null ]
    ] ],
    [ "MQTT ინტეგრაციის სახელმძღვანელო (Integration — MQTT)", "md_docs_2integration_2integration__mqtt.html", [
      [ "მიზანი", "md_docs_2integration_2integration__mqtt.html#autotoc_md1602", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__mqtt.html#autotoc_md1604", null ],
      [ "Best Practices", "md_docs_2integration_2integration__mqtt.html#autotoc_md1606", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__mqtt.html#autotoc_md1608", null ]
    ] ],
    [ "REST API ინტეგრაციის სახელმძღვანელო (Integration — REST API)", "md_docs_2integration_2integration__rest.html", [
      [ "მიზანი", "md_docs_2integration_2integration__rest.html#autotoc_md1612", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__rest.html#autotoc_md1614", null ],
      [ "Best Practices", "md_docs_2integration_2integration__rest.html#autotoc_md1616", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__rest.html#autotoc_md1618", null ]
    ] ],
    [ "WebSocket ინტეგრაციის სახელმძღვანელო (Integration — WebSocket)", "md_docs_2integration_2integration__websocket.html", [
      [ "მიზანი", "md_docs_2integration_2integration__websocket.html#autotoc_md1622", null ],
      [ "ძირითადი ნაბიჯები", "md_docs_2integration_2integration__websocket.html#autotoc_md1624", null ],
      [ "Best Practices", "md_docs_2integration_2integration__websocket.html#autotoc_md1626", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2integration_2integration__websocket.html#autotoc_md1628", null ]
    ] ],
    [ "✅ Synapse Framework — Documentation & Module Design Checklist", "md_docs_2internal_2synapse__checklist.html", [
      [ "1. API Naming & Structure", "md_docs_2internal_2synapse__checklist.html#autotoc_md1631", null ],
      [ "2. Module Implementation", "md_docs_2internal_2synapse__checklist.html#autotoc_md1632", null ],
      [ "3. Communication Integration (MQTT/WiFi/ESP-NOW)", "md_docs_2internal_2synapse__checklist.html#autotoc_md1633", null ],
      [ "4. Core Interaction", "md_docs_2internal_2synapse__checklist.html#autotoc_md1634", null ],
      [ "5. Documentation", "md_docs_2internal_2synapse__checklist.html#autotoc_md1635", null ],
      [ "6. Quality & Security", "md_docs_2internal_2synapse__checklist.html#autotoc_md1636", null ],
      [ "7. Testability", "md_docs_2internal_2synapse__checklist.html#autotoc_md1637", null ]
    ] ],
    [ "ბენჩმარკების ჩატარების სახელმძღვანელო (Performance Benchmarks Guide)", "md_docs_2performance_2performance__benchmarks.html", [
      [ "მიზანი", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1640", null ],
      [ "ბენჩმარკის ჩატარების ძირითადი ნაბიჯები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1642", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1644", [
        [ "სენსორის კითხვა", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1645", null ],
        [ "MQTT publish latency", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1646", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1648", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__benchmarks.html#autotoc_md1650", null ]
    ] ],
    [ "საუკეთესო პრაქტიკები წარმადობისთვის (Performance Best Practices)", "md_docs_2performance_2performance__best__practices.html", [
      [ "მიზანი", "md_docs_2performance_2performance__best__practices.html#autotoc_md1654", null ],
      [ "ძირითადი რეკომენდაციები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1656", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1658", null ],
      [ "Best Practices", "md_docs_2performance_2performance__best__practices.html#autotoc_md1660", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__best__practices.html#autotoc_md1662", null ]
    ] ],
    [ "ეკრანის მოდულების ოპტიმიზაცია (Performance — Display Modules)", "md_docs_2performance_2performance__display.html", [
      [ "მიზანი", "md_docs_2performance_2performance__display.html#autotoc_md1666", null ],
      [ "ოპტიმიზაციის რეკომენდაციები", "md_docs_2performance_2performance__display.html#autotoc_md1668", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__display.html#autotoc_md1670", [
        [ "Refresh interval-ის ოპტიმიზაცია", "md_docs_2performance_2performance__display.html#autotoc_md1671", null ],
        [ "Conditional redraw", "md_docs_2performance_2performance__display.html#autotoc_md1672", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__display.html#autotoc_md1674", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__display.html#autotoc_md1676", null ]
    ] ],
    [ "Performance (წარმადობის სახელმძღვანელო)", "md_docs_2performance_2performance__index.html", [
      [ "მიზანი", "md_docs_2performance_2performance__index.html#autotoc_md1680", null ],
      [ "თემატური performance სახელმძღვანელოები", "md_docs_2performance_2performance__index.html#autotoc_md1682", null ],
      [ "გამოყენების წესი", "md_docs_2performance_2performance__index.html#autotoc_md1684", null ]
    ] ],
    [ "ოპტიმიზაციის სახელმძღვანელოები (Performance Optimization Guide)", "md_docs_2performance_2performance__optimization.html", [
      [ "მიზანი", "md_docs_2performance_2performance__optimization.html#autotoc_md1688", null ],
      [ "კოდის ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1690", null ],
      [ "კონფიგურაციის ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1692", null ],
      [ "მოდულების ოპტიმიზაცია", "md_docs_2performance_2performance__optimization.html#autotoc_md1694", null ],
      [ "მაგალითები", "md_docs_2performance_2performance__optimization.html#autotoc_md1696", [
        [ "Static allocation", "md_docs_2performance_2performance__optimization.html#autotoc_md1697", null ],
        [ "Lazy initialization", "md_docs_2performance_2performance__optimization.html#autotoc_md1698", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__optimization.html#autotoc_md1700", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__optimization.html#autotoc_md1702", null ]
    ] ],
    [ "რესურსების გამოყენების სახელმძღვანელო (Performance Resource Usage Guide)", "md_docs_2performance_2performance__resource__usage.html", [
      [ "მიზანი", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1706", null ],
      [ "RAM-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1708", null ],
      [ "Flash-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1709", null ],
      [ "CPU-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1710", null ],
      [ "Stack-ის გამოყენება", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1711", null ],
      [ "მონიტორინგის მაგალითები", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1713", [
        [ "RAM", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1714", null ],
        [ "Stack", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1715", null ]
      ] ],
      [ "Best Practices", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1717", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2performance_2performance__resource__usage.html#autotoc_md1719", null ]
    ] ],
    [ "უსაფრთხოების საუკეთესო პრაქტიკები (Security Best Practices)", "md_docs_2security_2security__best__practices.html", [
      [ "მიზანი", "md_docs_2security_2security__best__practices.html#autotoc_md1723", null ],
      [ "ძირითადი რეკომენდაციები", "md_docs_2security_2security__best__practices.html#autotoc_md1725", null ],
      [ "მაგალითები", "md_docs_2security_2security__best__practices.html#autotoc_md1727", [
        [ "TLS/SSL ჩართვა MQTT-ში", "md_docs_2security_2security__best__practices.html#autotoc_md1728", null ],
        [ "OTA განახლების სიგნირება", "md_docs_2security_2security__best__practices.html#autotoc_md1729", null ],
        [ "ლოგირების კონტროლი", "md_docs_2security_2security__best__practices.html#autotoc_md1730", null ]
      ] ],
      [ "Best Practices", "md_docs_2security_2security__best__practices.html#autotoc_md1732", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__best__practices.html#autotoc_md1734", null ]
    ] ],
    [ "უსაფრთხოების შემოწმების სია (Security Checklist)", "md_docs_2security_2security__checklist.html", [
      [ "მიზანი", "md_docs_2security_2security__checklist.html#autotoc_md1738", null ],
      [ "Release Checklist", "md_docs_2security_2security__checklist.html#autotoc_md1740", null ],
      [ "Code Review Checklist", "md_docs_2security_2security__checklist.html#autotoc_md1741", null ],
      [ "Best Practices", "md_docs_2security_2security__checklist.html#autotoc_md1743", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__checklist.html#autotoc_md1745", null ]
    ] ],
    [ "უსაფრთხოების სახელმძღვანელოები (Security Guidelines)", "md_docs_2security_2security__guidelines.html", [
      [ "მიზანი", "md_docs_2security_2security__guidelines.html#autotoc_md1749", null ],
      [ "TLS/SSL", "md_docs_2security_2security__guidelines.html#autotoc_md1751", null ],
      [ "Authentication & Authorization", "md_docs_2security_2security__guidelines.html#autotoc_md1752", null ],
      [ "Input Validation", "md_docs_2security_2security__guidelines.html#autotoc_md1753", null ],
      [ "Credentials Management", "md_docs_2security_2security__guidelines.html#autotoc_md1754", null ],
      [ "OTA და Firmware Security", "md_docs_2security_2security__guidelines.html#autotoc_md1755", null ],
      [ "Logging", "md_docs_2security_2security__guidelines.html#autotoc_md1756", null ],
      [ "Best Practices", "md_docs_2security_2security__guidelines.html#autotoc_md1758", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__guidelines.html#autotoc_md1760", null ]
    ] ],
    [ "Security Index (უსაფრთხოების სახელმძღვანელოების ინდექსი)", "md_docs_2security_2security__index.html", [
      [ "მიზანი", "md_docs_2security_2security__index.html#autotoc_md1764", null ],
      [ "თემატური უსაფრთხოების სახელმძღვანელოები", "md_docs_2security_2security__index.html#autotoc_md1766", null ],
      [ "გამოყენების წესი", "md_docs_2security_2security__index.html#autotoc_md1768", null ]
    ] ],
    [ "Vulnerability Disclosure Policy (უსაფრთხოების ხარვეზების აღმოჩენისა და შეტყობინების პროცედურა)", "md_docs_2security_2security__vulnerability__disclosure.html", [
      [ "მიზანი", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1772", null ],
      [ "ხარვეზის აღმოჩენა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1774", null ],
      [ "შეტყობინების პროცედურა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1775", null ],
      [ "რეაგირება", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1776", null ],
      [ "პასუხისმგებლობა და ეთიკა", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1777", null ],
      [ "საკონტაქტო ინფორმაცია", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1779", null ],
      [ "დაკავშირებული დოკუმენტები", "md_docs_2security_2security__vulnerability__disclosure.html#autotoc_md1781", null ]
    ] ],
    [ "კომუნიკაციის პატერნები (Communication Patterns)", "md_docs_2structure_2communication__patterns.html", [
      [ "მიზანი", "md_docs_2structure_2communication__patterns.html#autotoc_md1784", null ],
      [ "ძირითადი პატერნები", "md_docs_2structure_2communication__patterns.html#autotoc_md1785", [
        [ "1. Service Locator Pattern", "md_docs_2structure_2communication__patterns.html#autotoc_md1786", null ],
        [ "2. Event Bus Pattern", "md_docs_2structure_2communication__patterns.html#autotoc_md1787", null ]
      ] ],
      [ "სწორი გამოყენების წესები", "md_docs_2structure_2communication__patterns.html#autotoc_md1788", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2structure_2communication__patterns.html#autotoc_md1789", null ],
      [ "მაგალითები", "md_docs_2structure_2communication__patterns.html#autotoc_md1790", [
        [ "Service Locator", "md_docs_2structure_2communication__patterns.html#autotoc_md1791", null ],
        [ "Event Bus", "md_docs_2structure_2communication__patterns.html#autotoc_md1792", null ]
      ] ]
    ] ],
    [ "კონფიგურაციის მართვის პროცესი (Configuration Flow)", "md_docs_2structure_2configuration__flow.html", [
      [ "მიზანი", "md_docs_2structure_2configuration__flow.html#autotoc_md1795", null ],
      [ "კონფიგურაციის წყაროები", "md_docs_2structure_2configuration__flow.html#autotoc_md1796", null ],
      [ "კონფიგურაციის სტრუქტურა", "md_docs_2structure_2configuration__flow.html#autotoc_md1797", null ],
      [ "კონფიგურაციის დამუშავების ეტაპები", "md_docs_2structure_2configuration__flow.html#autotoc_md1798", null ],
      [ "კონფიგურაციის წვდომის მაგალითები", "md_docs_2structure_2configuration__flow.html#autotoc_md1799", null ],
      [ "ვალიდაცია და Default მნიშვნელობები", "md_docs_2structure_2configuration__flow.html#autotoc_md1800", null ],
      [ "Runtime Reconfiguration", "md_docs_2structure_2configuration__flow.html#autotoc_md1801", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2configuration__flow.html#autotoc_md1802", null ]
    ] ],
    [ "Core კომპონენტები (Core Components)", "md_docs_2structure_2core__components.html", [
      [ "მიზანი", "md_docs_2structure_2core__components.html#autotoc_md1806", null ],
      [ "ძირითადი კომპონენტები", "md_docs_2structure_2core__components.html#autotoc_md1808", [
        [ "1. Service Locator", "md_docs_2structure_2core__components.html#autotoc_md1809", null ],
        [ "2. Event Bus", "md_docs_2structure_2core__components.html#autotoc_md1810", null ],
        [ "3. Module Register", "md_docs_2structure_2core__components.html#autotoc_md1811", null ],
        [ "4. Module Factory", "md_docs_2structure_2core__components.html#autotoc_md1812", null ],
        [ "5. Configuration Manager", "md_docs_2structure_2core__components.html#autotoc_md1813", null ],
        [ "6. Logging", "md_docs_2structure_2core__components.html#autotoc_md1814", null ]
      ] ],
      [ "ფასადის ინტერფეისი (<tt>synapse.h</tt>)", "md_docs_2structure_2core__components.html#autotoc_md1816", null ],
      [ "ურთიერთქმედება მოდულებთან", "md_docs_2structure_2core__components.html#autotoc_md1818", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2core__components.html#autotoc_md1820", null ]
    ] ],
    [ "საქაღალდეებისა და ფაილების სტრუქტურა (Directory Structure)", "md_docs_2structure_2directory__structure.html", [
      [ "ძირითადი სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1823", null ],
      [ "ძირითადი საქაღალდეების აღწერა", "md_docs_2structure_2directory__structure.html#autotoc_md1824", null ],
      [ "<tt>modules</tt> საქაღალდის სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1825", null ],
      [ "თითოეული მოდულის სტრუქტურა", "md_docs_2structure_2directory__structure.html#autotoc_md1826", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2directory__structure.html#autotoc_md1827", null ]
    ] ],
    [ "შეცდომების დამუშავება და ლოგირების სტანდარტები (Error Handling & Logging)", "md_docs_2structure_2error__handling__and__logging.html", [
      [ "მიზანი", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1830", null ],
      [ "შეცდომების დამუშავების პრინციპები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1831", null ],
      [ "შეცდომების კოდები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1832", null ],
      [ "შეცდომების დამუშავების მაგალითები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1833", null ],
      [ "ლოგირების სტანდარტები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1834", null ],
      [ "ლოგირების მაგალითები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1835", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2error__handling__and__logging.html#autotoc_md1836", null ]
    ] ],
    [ "ფრეიმვორქის გაფართოება და Best Practices (Extending the Framework)", "md_docs_2structure_2extending__framework.html", [
      [ "მიზანი", "md_docs_2structure_2extending__framework.html#autotoc_md1839", null ],
      [ "ახალი მოდულის დამატების პროცესი", "md_docs_2structure_2extending__framework.html#autotoc_md1840", null ],
      [ "Best Practices", "md_docs_2structure_2extending__framework.html#autotoc_md1841", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2structure_2extending__framework.html#autotoc_md1842", null ],
      [ "მაგალითი ახალი მოდულის დამატების", "md_docs_2structure_2extending__framework.html#autotoc_md1843", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (FAQ)", "md_docs_2structure_2faq.html", [
      [ "1. როგორ დავამატო ახალი მოდული?", "md_docs_2structure_2faq.html#autotoc_md1846", null ],
      [ "2. შეიძლება თუ არა ერთი და იმავე ტიპის რამდენიმე მოდულის დამატება?", "md_docs_2structure_2faq.html#autotoc_md1847", null ],
      [ "3. როგორ ხდება მოდულებს შორის კომუნიკაცია?", "md_docs_2structure_2faq.html#autotoc_md1848", null ],
      [ "4. როგორ მივიღო ან შევცვალო მოდულის კონფიგურაცია?", "md_docs_2structure_2faq.html#autotoc_md1849", null ],
      [ "5. როგორ დავამატო ახალი გლობალური პარამეტრი?", "md_docs_2structure_2faq.html#autotoc_md1850", null ],
      [ "6. როგორ დავიცვა naming და structure სტანდარტები?", "md_docs_2structure_2faq.html#autotoc_md1851", null ],
      [ "7. როგორ ხდება შეცდომების დამუშავება და ლოგირება?", "md_docs_2structure_2faq.html#autotoc_md1852", null ],
      [ "8. როგორ დავამატო ახალი კატეგორია მოდულებისთვის?", "md_docs_2structure_2faq.html#autotoc_md1853", null ],
      [ "9. როგორ დავამატო დოკუმენტაცია ან განვაახლო არსებული?", "md_docs_2structure_2faq.html#autotoc_md1854", null ],
      [ "10. როგორ დავიწყო პროექტის გამოყენება?", "md_docs_2structure_2faq.html#autotoc_md1855", null ]
    ] ],
    [ "მოდულის სიცოცხლის ციკლი და ინიციალიზაცია (Lifecycle & Initialization)", "md_docs_2structure_2lifecycle__and__init.html", [
      [ "მიზანი", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1858", null ],
      [ "სიცოცხლის ციკლის ეტაპები", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1859", null ],
      [ "დეტალური აღწერა", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1860", [
        [ "1. Create", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1861", null ],
        [ "2. Init", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1862", null ],
        [ "3. Enable/Disable", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1863", null ],
        [ "4. Deinit", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1864", null ]
      ] ],
      [ "სიცოცხლის ციკლის დიაგრამა", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1865", null ],
      [ "მაგალითი მოდულის სიცოცხლის ციკლის მართვის", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1866", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2lifecycle__and__init.html#autotoc_md1867", null ]
    ] ],
    [ "მოდულების არქიტექტურა (Module Architecture)", "md_docs_2structure_2module__architecture.html", [
      [ "ძირითადი პრინციპები", "md_docs_2structure_2module__architecture.html#autotoc_md1870", null ],
      [ "მოდულის ტიპები", "md_docs_2structure_2module__architecture.html#autotoc_md1871", null ],
      [ "მოდულების ურთიერთქმედება", "md_docs_2structure_2module__architecture.html#autotoc_md1872", null ],
      [ "მოდულის სიცოცხლის ციკლი (Lifecycle)", "md_docs_2structure_2module__architecture.html#autotoc_md1873", null ],
      [ "მოდულის სტრუქტურის მაგალითი", "md_docs_2structure_2module__architecture.html#autotoc_md1874", [
        [ "<tt>_create</tt> ფუნქციის კონტრაქტი", "md_docs_2structure_2module__architecture.html#autotoc_md1875", null ]
      ] ],
      [ "მოდულის გაფართოება/დამატება", "md_docs_2structure_2module__architecture.html#autotoc_md1876", null ],
      [ "აკრძალული და რეკომენდებული პრაქტიკები", "md_docs_2structure_2module__architecture.html#autotoc_md1877", null ]
    ] ],
    [ "🚀 <tt>create_module.py</tt>: Synapse მოდულის გენერატორი (v3.1)", "md_docs_2tools_2create__module.html", [
      [ "1. 📋 რა არის <tt>create_module.py</tt>?", "md_docs_2tools_2create__module.html#autotoc_md1888", null ],
      [ "2. 🎯 რას აკეთებს?", "md_docs_2tools_2create__module.html#autotoc_md1889", [
        [ "✅ გენერირებული ფაილები (10+ ფაილი)", "md_docs_2tools_2create__module.html#autotoc_md1890", null ],
        [ "✅ ავტომატური ინტეგრაცია", "md_docs_2tools_2create__module.html#autotoc_md1891", null ]
      ] ],
      [ "3. 🛠️ გამოყენების რეჟიმები", "md_docs_2tools_2create__module.html#autotoc_md1892", [
        [ "3.1. ინტერაქტიული რეჟიმი (რეკომენდებული)", "md_docs_2tools_2create__module.html#autotoc_md1893", null ],
        [ "3.2. ბრძანების ხაზის რეჟიმი (ავტომატიზაციისთვის)", "md_docs_2tools_2create__module.html#autotoc_md1894", null ]
      ] ],
      [ "4. 🏛️ არქეტიპები (Archetypes)", "md_docs_2tools_2create__module.html#autotoc_md1895", null ],
      [ "5. 🔄 სრული სამუშაო პროცესი (Workflow)", "md_docs_2tools_2create__module.html#autotoc_md1896", null ],
      [ "6. 🚨 მნიშვნელოვანი შენიშვნები", "md_docs_2tools_2create__module.html#autotoc_md1897", null ],
      [ "🎯 შეჯამება", "md_docs_2tools_2create__module.html#autotoc_md1899", null ]
    ] ],
    [ "Advanced Diagnostics (გაფართოებული დიაგნოსტიკა)", "md_docs_2troubleshooting_2advanced__diagnostics.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1901", null ],
      [ "1. Stack Overflow და Task Watchdog", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1903", null ],
      [ "2. Heap Corruption და Memory Leak", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1904", null ],
      [ "3. Core Dumps და Crash Reporting", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1905", null ],
      [ "4. Custom Health Checks", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1906", null ],
      [ "5. Peripheral Diagnostics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1907", null ],
      [ "6. Network Diagnostics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1908", null ],
      [ "7. Performance Metrics", "md_docs_2troubleshooting_2advanced__diagnostics.html#autotoc_md1909", null ]
    ] ],
    [ "გავრცელებული პრობლემები და გადაჭრის გზები (Common Issues & Solutions)", "md_docs_2troubleshooting_2common__issues.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1912", null ],
      [ "1. ბილდის შეცდომები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1914", [
        [ "პრობლემა: მოდული არ იბილდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1915", null ],
        [ "პრობლემა: include file not found", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1916", null ]
      ] ],
      [ "2. კონფიგურაციის პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1918", [
        [ "პრობლემა: მოდული არ ინიციალიზდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1919", null ],
        [ "პრობლემა: default მნიშვნელობები არ მუშაობს", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1920", null ]
      ] ],
      [ "3. კომუნიკაციის პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1922", [
        [ "პრობლემა: Event Bus-ზე მოვლენები არ ვრცელდება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1923", null ],
        [ "პრობლემა: Service Locator-ით API არ იძებნება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1924", null ]
      ] ],
      [ "4. ლოგირების და შეცდომების დიაგნოსტიკა", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1926", [
        [ "პრობლემა: ლოგები არ ჩანს", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1927", null ],
        [ "პრობლემა: შეცდომის კოდი არ იკითხება", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1928", null ]
      ] ],
      [ "5. Runtime პრობლემები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1930", [
        [ "პრობლემა: მოდული არ რეაგირებს enable/disable-ზე", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1931", null ],
        [ "პრობლემა: რესურსების გაჟონვა (memory leak)", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1932", null ],
        [ "პრობლემა: სისტემის კრახი <tt>Stack Overflow in task Tmr Svc</tt> შეცდომით", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1934", null ],
        [ "პრობლემა: სისტემის კრახი <tt>LoadProhibited</tt> ან <tt>assert failed: heap_caps_free</tt>", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1935", null ]
      ] ],
      [ "6. დამატებითი რჩევები", "md_docs_2troubleshooting_2common__issues.html#autotoc_md1937", null ]
    ] ],
    [ "Debugging Guide (დიბაგინგის სახელმძღვანელო)", "md_docs_2troubleshooting_2debugging__guide.html", [
      [ "მიზანი", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1940", null ],
      [ "1. ლოგირების ჩართვა და გამოყენება", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1942", null ],
      [ "2. Debug Build-ის კონფიგურაცია", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1943", null ],
      [ "3. GDB და Breakpoint-ების გამოყენება", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1944", null ],
      [ "4. Task და Stack Usage მონიტორინგი", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1945", null ],
      [ "5. Heap და Memory Usage", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1946", null ],
      [ "6. Event და Service Debugging", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1947", null ],
      [ "7. Assertion-ები და Error Checking", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1948", null ],
      [ "8. დამატებითი ინსტრუმენტები", "md_docs_2troubleshooting_2debugging__guide.html#autotoc_md1949", null ]
    ] ],
    [ "პრობლემა: როგორ გადავცეთ კონტექსტი სერვისის API-ს გამოძახებისას?", "md_docs_2troubleshooting_2dependency__injection__and__context.html", [
      [ "1. ❓ პრობლემის აღწერა", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1952", null ],
      [ "2. ❌ არასწორი (მაგრამ მაცდური) გადაწყვეტები", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1953", null ],
      [ "3. ✅ სწორი არქიტექტურული პატერნი: ორმაგი ინექცია", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1954", [
        [ "ეტაპი 1: <tt>private_data</tt> სტრუქტურის გაფართოება", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1955", null ],
        [ "ეტაპი 2: <tt>dependency_map</tt>-ის განსაზღვრა", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1956", null ],
        [ "ეტაპი 3: <tt>config.json</tt>-ის კონფიგურაცია", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1957", null ],
        [ "ეტაპი 4: გამოყენება", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1958", null ]
      ] ],
      [ "4. 💬 დასკვნა", "md_docs_2troubleshooting_2dependency__injection__and__context.html#autotoc_md1959", null ]
    ] ],
    [ "ხშირად დასმული კითხვები (Troubleshooting FAQ)", "md_docs_2troubleshooting_2faq.html", [
      [ "1. რატომ არ იბილდება ჩემი მოდული?", "md_docs_2troubleshooting_2faq.html#autotoc_md1961", null ],
      [ "2. რატომ არ ჩანს ლოგები?", "md_docs_2troubleshooting_2faq.html#autotoc_md1962", null ],
      [ "3. რატომ ვერ პოულობს Service Locator-ი ჩემს სერვისს?", "md_docs_2troubleshooting_2faq.html#autotoc_md1963", null ],
      [ "4. რატომ არ ვრცელდება Event Bus-ზე მოვლენები?", "md_docs_2troubleshooting_2faq.html#autotoc_md1964", null ],
      [ "5. რატომ არ ინიციალიზდება მოდული?", "md_docs_2troubleshooting_2faq.html#autotoc_md1965", null ],
      [ "6. როგორ გავიგო რომელი მოდული იწვევს პრობლემას?", "md_docs_2troubleshooting_2faq.html#autotoc_md1966", null ],
      [ "7. როგორ გამოვასწორო რესურსების გაჟონვა (memory leak)?", "md_docs_2troubleshooting_2faq.html#autotoc_md1967", null ],
      [ "8. როგორ დავადგინო კონფიგურაციის შეცდომა?", "md_docs_2troubleshooting_2faq.html#autotoc_md1968", null ],
      [ "9. როგორ დავამატო ახალი კითხვა ამ FAQ-ში?", "md_docs_2troubleshooting_2faq.html#autotoc_md1969", null ]
    ] ],
    [ "Advanced Usage (გაფართოებული გამოყენება)", "md_docs_2tutorials_2advanced__usage.html", [
      [ "მიზანი", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1972", null ],
      [ "1. მრავალმოდულიანი კონფიგურაცია", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1973", null ],
      [ "2. მოდულის დინამიური ჩართვა/გამორთვა", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1974", null ],
      [ "3. Runtime Reconfiguration", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1975", null ],
      [ "4. Custom Event-ების გამოყენება", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1976", null ],
      [ "5. ასინქრონული ოპერაციების მართვა (Promise Pattern)", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1977", [
        [ "5.1. სერვისის მომწოდებლის API (<tt>wifi_manager</tt>)", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1978", null ],
        [ "5.2. მომხმარებლის ლოგიკა (<tt>ui_manager</tt>)", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1979", null ]
      ] ],
      [ "6. მოდულის სტატუსის მონიტორინგი", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1980", null ],
      [ "7. ოპტიმიზაცია და რესურსების მართვა", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1981", null ],
      [ "8. Best Practices", "md_docs_2tutorials_2advanced__usage.html#autotoc_md1982", null ]
    ] ],
    [ "პირველი საკუთარი მოდულის შექმნა (Create Your First Module)", "md_docs_2tutorials_2create__first__module.html", [
      [ "მიზანი", "md_docs_2tutorials_2create__first__module.html#autotoc_md1985", null ],
      [ "1. საჭიროების განსაზღვრა", "md_docs_2tutorials_2create__first__module.html#autotoc_md1986", null ],
      [ "2. მოდულის ჩონჩხის გენერაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1987", null ],
      [ "3. კონფიგურაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1988", null ],
      [ "4. კოდის იმპლემენტაცია", "md_docs_2tutorials_2create__first__module.html#autotoc_md1989", null ],
      [ "5. ტესტირება", "md_docs_2tutorials_2create__first__module.html#autotoc_md1990", null ]
    ] ],
    [ "სწრაფი სტარტი (Getting Started)", "md_docs_2tutorials_2getting__started.html", [
      [ "მიზანი", "md_docs_2tutorials_2getting__started.html#autotoc_md1993", null ],
      [ "1. გარემოს მომზადება", "md_docs_2tutorials_2getting__started.html#autotoc_md1994", null ],
      [ "2. პროექტის სტრუქტურის გაცნობა", "md_docs_2tutorials_2getting__started.html#autotoc_md1995", null ],
      [ "3. პირველი მოდულის ჩართვა", "md_docs_2tutorials_2getting__started.html#autotoc_md1996", null ],
      [ "4. ბილდი და ფლეში", "md_docs_2tutorials_2getting__started.html#autotoc_md1997", null ],
      [ "5. ლოგების ნახვა და ტესტირება", "md_docs_2tutorials_2getting__started.html#autotoc_md1998", null ],
      [ "6. დამატებითი რესურსები", "md_docs_2tutorials_2getting__started.html#autotoc_md1999", null ]
    ] ],
    [ "ინტეგრაციის მაგალითი (Integration Example)", "md_docs_2tutorials_2integration__example.html", [
      [ "მიზანი", "md_docs_2tutorials_2integration__example.html#autotoc_md2002", null ],
      [ "1. ინტეგრაციის საჭიროების განსაზღვრა", "md_docs_2tutorials_2integration__example.html#autotoc_md2003", null ],
      [ "2. საჭირო მოდულების ჩართვა და კონფიგურაცია", "md_docs_2tutorials_2integration__example.html#autotoc_md2004", null ],
      [ "3. MQTT Integration (Publish/Subscribe)", "md_docs_2tutorials_2integration__example.html#autotoc_md2005", null ],
      [ "4. REST API Integration", "md_docs_2tutorials_2integration__example.html#autotoc_md2006", null ],
      [ "5. WebSocket Integration", "md_docs_2tutorials_2integration__example.html#autotoc_md2007", null ],
      [ "6. Best Practices", "md_docs_2tutorials_2integration__example.html#autotoc_md2008", null ],
      [ "7. Troubleshooting", "md_docs_2tutorials_2integration__example.html#autotoc_md2009", null ]
    ] ],
    [ "მოდულის გაფართოება ან ფუნქციონალის დამატება (Module Extension & Feature Addition)", "md_docs_2tutorials_2module__extension.html", [
      [ "მიზანი", "md_docs_2tutorials_2module__extension.html#autotoc_md2012", null ],
      [ "1. საჭიროების განსაზღვრა", "md_docs_2tutorials_2module__extension.html#autotoc_md2013", null ],
      [ "2. სტრუქტურის დაცვა", "md_docs_2tutorials_2module__extension.html#autotoc_md2014", null ],
      [ "3. ახალი API ან ფუნქციის დამატება", "md_docs_2tutorials_2module__extension.html#autotoc_md2015", null ],
      [ "4. კონფიგურაციის გაფართოება", "md_docs_2tutorials_2module__extension.html#autotoc_md2016", null ],
      [ "5. კომუნიკაციის გაფართოება", "md_docs_2tutorials_2module__extension.html#autotoc_md2017", null ],
      [ "6. ტესტირება და ლოგირება", "md_docs_2tutorials_2module__extension.html#autotoc_md2018", null ],
      [ "7. დოკუმენტაციის განახლება", "md_docs_2tutorials_2module__extension.html#autotoc_md2019", null ],
      [ "Best Practices", "md_docs_2tutorials_2module__extension.html#autotoc_md2020", null ],
      [ "აკრძალული პრაქტიკები", "md_docs_2tutorials_2module__extension.html#autotoc_md2021", null ]
    ] ],
    [ "სისტემის კონფიგურაცია (System Configuration)", "md_docs_2tutorials_2system__configuration.html", [
      [ "🎯 მიზანი", "md_docs_2tutorials_2system__configuration.html#autotoc_md2024", null ],
      [ "1. კონფიგურაციის ფაილების როლები", "md_docs_2tutorials_2system__configuration.html#autotoc_md2026", null ],
      [ "2. <tt>config.json</tt>-ის სტრუქტურა და მაგალითები", "md_docs_2tutorials_2system__configuration.html#autotoc_md2028", null ],
      [ "3. მოდულის კონფიგურაციის ობიექტის აღწერა", "md_docs_2tutorials_2system__configuration.html#autotoc_md2030", null ],
      [ "4. პარამეტრებზე წვდომა კოდიდან (Dot-Notation)", "md_docs_2tutorials_2system__configuration.html#autotoc_md2032", null ],
      [ "5. კონფიგურაციის ვალიდაცია და Default-ები", "md_docs_2tutorials_2system__configuration.html#autotoc_md2034", null ],
      [ "6. საუკეთესო პრაქტიკები (Best Practices)", "md_docs_2tutorials_2system__configuration.html#autotoc_md2036", null ]
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
"functions_u.html",
"md_components_2modules_2system_2connectivity__watchdog_2README.html#autotoc_md148",
"md_docs_2api__reference_2event__api.html#autotoc_md450",
"md_docs_2changelog_2CHANGELOG__2025-06-24.html#autotoc_md799",
"md_docs_2contributing_2local__setup.html#autotoc_md1118",
"md_docs_2design__decisions_2decision__record__template.html#autotoc_md1410",
"md_docs_2structure_2directory__structure.html#autotoc_md1824",
"resource__manager_8c.html#aacceb1b239d569bcaccb13ae9bb59b72",
"structdisplay__info__t.html#ab47f8096d9a1be6cdcd559206ab478c5",
"structui__manager__private__data__t.html#acb411d2d2d4497db2b125a6d98959f51"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';