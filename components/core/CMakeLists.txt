# ძირითადი კომპონენტის წყარო ფაილების სია
set(SRCS
    "event_bus.c"
    "config_manager.c"
    "system_manager.c"
    "module_registry.c"
    "module_factory.c"
    "resource_manager.c"
    "event_data_wrapper.c"
    "service_locator.c"
)

# ჩასვით სკრიპტი, რომელიც ავტომატურად აღმოაჩენს მოდულებს და გენერირებს factory-ს.
# ეს სკრიპტი მშობელ scope-ში განსაზღვრავს რამდენიმე ცვლადს, მათ შორის
# COLLECTED_MODULE_DEPENDENCIES-ს და სხვა factory-ს გენერაციისთვის საჭირო ცვლადებს.
include(${CMAKE_CURRENT_LIST_DIR}/collect_modules.cmake)

# ჩასვით სკრიპტი, რომელიც აგროვებს MQTT თემებს მოდულებიდან და ქმნის ცენტრალიზებულ header-ს
# include(${CMAKE_CURRENT_LIST_DIR}/collect_mqtt_topics.cmake)

# გენერაცია MQTT თემების header ფაილის module.json-ებიდან
# collect_mqtt_topics()

# collect_modules სკრიპტი აგენერირებს ამ C ფაილს, ამიტომ უნდა დავამატოთ ჩვენს წყაროებში.
list(APPEND SRCS "${CMAKE_CURRENT_BINARY_DIR}/generated_module_factory.c")

# დაარეგისტრირეთ კომპონენტი მის წყარო ფაილებთან და დამოკიდებულებებთან ერთად.
# შენიშვნა: არჩევითი მოდულების პირდაპირი დამოკიდებულებები ამოღებულია.
# Build სისტემა ამ დამოკიდებულებებს გადმოიღებს ტრანზიტიულად გენერირებული factory ფაილის include-ებიდან.
idf_component_register(SRCS ${SRCS}
                    INCLUDE_DIRS "include" "${CMAKE_CURRENT_BINARY_DIR}"
                    EMBED_FILES "../../configs/system_config.json"
                    REQUIRES esp_event
                             nvs_flash
                             json
                             driver
                             esp_wifi
                             interfaces
                             # esp_tls # ეს დამოკიდებულება მხოლოდ იმ მოდულს უნდა დაემატოს, რომელსაც ეს სჭირდება და არა core-ს.
                             # ყველა აღმოჩენილი მოდული დაემატოს build-time დამოკიდებულებად,
                             # რათა build სისტემამ იცოდეს მათი header-ების მდებარეობა.
                             ${COLLECTED_MODULE_DEPENDENCIES}
)