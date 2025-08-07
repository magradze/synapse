# Synapse API Reference Index

ეს დოკუმენტი წარმოადგენს Synapse Framework-ის ყველა API დოკუმენტის მოკლე მიმოხილვას და ბმულებს.

---

## Core API

- [core_api.md](core_api.md) — ძირითადი ბირთვის (Service Locator, Event Bus, Module Register, Factory, Configuration, Logging) API
- [event_api.md](event_api.md) — Event Bus და custom event-ების დეტალური API
- [configuration_api.md](configuration_api.md) — კონფიგურაციის მენეჯერის API
- [event_payloads_api.md](event_payloads_api.md) — სტანდარტული მონაცემთა სტრუქტურები ივენთებისთვის
- [promise_api.md](promise_api.md) — ასინქრონული ოპერაციების მართვის (Promise) API

## Module API

- [module_api.md](module_api.md) — მოდულების სიცოცხლის ციკლი, Service API, Event Handling, კონფიგურაცია
- [display_api.md](display_api.md) — ეკრანის მოდულების API
- [sensor_api.md](sensor_api.md) — სენსორების მოდულების API
- [communication_api.md](communication_api.md) — ზოგადი საკომუნიკაციო მოდულების API
- [wifi_api.md](wifi_api.md) — WiFi Manager-ის API (სამომავლო)
- [mqtt_api.md](mqtt_api.md) — MQTT Manager-ის API (სამომავლო)
- [drivers_api.md](drivers_api.md) — დრაივერების (I2C, SPI) მოდულების API
- [utility_api.md](utility_api.md) — დამხმარე მოდულების (logger და სხვ.) API
- [actuators_api.md](actuators_api.md) — აქტუატორების (რელე და სხვ.) მოდულების API
- [provisioning_api.md](provisioning_api.md) — provisioning (BLE, SoftAP, Serial, Cloud, NFC, QR) მოდულების API

## System & Extensions

- [system_api.md](system_api.md) — სისტემური მოდულების (OTA, Version, Watchdog, Diagnostics) API
- [device_identity_api.md](device_identity_api.md) — Device Identity Service-ის API
- [ota_api.md](ota_api.md) — OTA განახლების მოდულის API
- [security_api.md](security_api.md) — უსაფრთხოების სტატუსის API
- [version_api.md](version_api.md) — Firmware ვერსიის მოდულის API
- [health_monitor_api.md](health_monitor_api.md) — სისტემის ჯანმრთელობის მონიტორინგის მოდულის API
- [storage_api.md](storage_api.md) — მეხსიერების (NVS/Flash) მოდულების API
- [time_sync_api.md](time_sync_api.md) — დროის სინქრონიზაციის (NTP/SNTP) მოდულის API
- [local_ctrl_api.md](local_ctrl_api.md) — ადგილობრივი კონტროლის (Web UI, REST API) მოდულის API
- [custom_api.md](custom_api.md) — მომხმარებლის მიერ დამატებული ან გაფართოებული API-ების მაგალითები

---

ყველა API დოკუმენტი მიჰყვება [variable_naming.md](../convention/variable_naming.md), [function_naming.md](../convention/function_naming.md) და [module_structure.md](../convention/module_structure.md) წესებს.

დამატებითი კითხვებისთვის იხილეთ შესაბამისი დოკუმენტაცია ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს.
