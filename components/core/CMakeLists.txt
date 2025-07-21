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
    "event_payloads.c"
    "module_helpers.c"
)

# --- Synapse Build Automation ---

# ჩასვით და გაუშვით სკრიპტი, რომელიც აგროვებს დეცენტრალიზებულ კონფიგურაციებს
include(${CMAKE_CURRENT_LIST_DIR}/collect_configs.cmake)
collect_and_prepare_module_configs() # ეს ფუნქცია განსაზღვრავს COLLECTED_EMBED_FILES ცვლადს

# ჩასვით სკრიპტი, რომელიც აგროვებს მოდულებს და აგენერირებს factory-ს
include(${CMAKE_CURRENT_LIST_DIR}/collect_modules.cmake)

# ჩასვით სკრიპტი, რომელიც აგროვებს MQTT თემებს
include(${CMAKE_CURRENT_LIST_DIR}/collect_mqtt_topics.cmake)
collect_mqtt_topics()

# დაამატეთ ავტომატურად გენერირებული factory წყაროების სიას
list(APPEND SRCS "${CMAKE_CURRENT_BINARY_DIR}/generated_module_factory.c")

# --- კომპონენტის რეგისტრაცია ---
idf_component_register(SRCS ${SRCS}
                    INCLUDE_DIRS
                        "include"
                        "${CMAKE_CURRENT_BINARY_DIR}" # დაამატეთ build დირექტორია გენერირებული ჰედერებისთვის
                    EMBED_FILES
                        "../../configs/system_config.json" # ბაზისური კონფიგურაცია
                        ${COLLECTED_EMBED_FILES}           # შეგროვებული მოდულების კონფიგურაციები
                    REQUIRES
                        esp_event
                        nvs_flash
                        json
                        driver
                        esp_wifi
                        interfaces
                        ${COLLECTED_MODULE_DEPENDENCIES}
)