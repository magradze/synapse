# ✅ Synapse Framework — Documentation & Module Design Checklist

## 1. API Naming & Structure

| რუბრიკა | მოთხოვნა | ✓ |
|--------|----------|----|
| ✅ ფუნქციის სახელი | იყენებს ფორმატს: `prefix_module_action()` ან `fmw_*` core-თვის |   |
| ✅ ლოგის TAG | განსაზღვრულია `DEFINE_COMPONENT_TAG(TAG_NAME)` |   |
| ✅ ფაილის სახელები | lowercase + `_api.md` ან `_module.c` სტილით |   |
| ✅ მოდულის სახელები | უნიკალურია სისტემაში (`module_instance_name`) |   |

## 2. Module Implementation

| რუბრიკა | მოთხოვნა | ✓ |
|--------|----------|----|
| ✅ არსებობს `*_create(config)` ფუნქცია | იღებს `cJSON*` და ასრულებს ინიციალიზაციას |   |
| ✅ კონფიგურაციის წაკითხვა | `fmw_config_get_*` ფუნქციების მეშვეობით |   |
| ✅ ლოგირება | ყველა მნიშვნელოვანი ქმედება ილოგება |   |
| ✅ error handling | აბრუნებს `esp_err_t`, ლოგავს შეცდომებს |   |
| ✅ ინდივიდუალური enable/disable | განსაზღვრულია მოდულის ჩართვა/გამორთვის მეთოდები |   |
| ✅ event handler | საჭიროების შემთხვევაში რეგისტრირებულია `event_bus_subscribe()` |   |
| ✅ სერვისის რეგისტრაცია | იყენებს `fmw_service_register()` საჭიროების შემთხვევაში |   |
| ✅ **რესურსების გასუფთავება** | `deinit` ფუნქცია სრულად ათავისუფლებს ყველა დაკავებულ რესურსს (mutex, memory, event subscriptions). | ☐ |

## 3. Communication Integration (MQTT/WiFi/ESP-NOW)

| რუბრიკა | მოთხოვნა | ✓ |
|--------|----------|----|
| ✅ MQTT publish/subscribe | გამოიყენება `mqtt_module_publish`, `subscribe` |   |
| ✅ topic სტრუქტურა | მიჰყვება `mqtt_topics_generated.h` ან დოკში აღწერილ სქემას |   |
| ✅ QoS | (თუ საჭიროა) მომხმარებელს შეუძლია QoS განსაზღვროს |   |
| ✅ WiFi provisioning | შესაძლებელია SSID/Password-ის დინამიური მიწოდება ან provisioning |   |
| ✅ reconnect logic | აღწერილია ან განხორციელებულია fallback/reconnect |   |

## 4. Core Interaction

| რუბრიკა | მოთხოვნა | ✓ |
|--------|----------|----|
| ✅ Service Locator | სერვისები რეგისტრირდება `fmw_service_register()` |   |
| ✅ Event Bus | მოიცავს `event_bus_post()` ან `event_bus_subscribe()` |   |
| ✅ Module Register | ყველა მოდული რეგისტრირდება `fmw_module_register()` |   |
| ✅ Module Factory | მართულია `fmw_module_factory_create()` |   |

## 5. Documentation

| რუბრიკა | მოთხოვნა | ✓ |
|--------|----------|----|
| ✅ API Reference | აღწერილია ყოველი public ფუნქცია `*_api.md` ფაილში |   |
| ✅ ფუნქცია დოკუმენტაცია | Signature, Description, Args, Returns |   |
| ✅ Tutorials | მინიმუმ 1 მაგალითი გამოყენების შესახებ |   |
| ✅ Naming standards | მოყვანილია `naming.md` ფაილში |   |
| ✅ Troubleshooting / FAQ | გავრცელებული შეცდომები დაფარულია შესაბამის ფაილში |   |

## 6. Quality & Security

| რუბრიკა | მოთხოვნა | ✓ |
|--------|----------|----|
| ✅ Static analysis | `idf.py build` warnings არ უნდა იყოს |   |
| ✅ Memory leakage | დამთავრებისას freed-resources |   |
| ✅ Use-after-free დაცვა | pointer reuse დაცულია |   |
| ✅ Safe concurrency | locks გამოყენებულია საჭირო ადგილას |   |
| ✅ Secret mgmt | პაროლები/კონფიდენციალური info არ hardcoded-დება |   |

## 7. Testability

| რუბრიკა | მოთხოვნა | ✓ |
|--------|----------|----|
| ✅ Unit tests | ძირითადი ფუნქციების დაფარვა |   |
| ✅ Mocking support | hardware abstraction თავსებადია mock-სთან |   |
| ✅ Config validation test | არასწორი კონფიგების მოწმობა |   |
| ✅ Logging test | იგზავნება სწორი log შეტყობინებები |   |
