# Wifi_Manager Module CMake Configuration
# Component for WiFi connection management
# Author: Synapse Framework Team
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება placeholder-ით
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით

# შევამოწმოთ არსებობს თუ არა CONFIG ცვლადი (configure ეტაპის შემდეგ)
if(DEFINED CONFIG_MODULE_WIFI_MANAGER_ENABLED)
    # CONFIG ცვლადი არსებობს - ვამოწმებთ მნიშვნელობას
    if(CONFIG_MODULE_WIFI_MANAGER_ENABLED)
        # მოდული ჩართულია - სრული რეგისტრაცია
        message(STATUS "Wifi_Manager Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
        idf_component_register(
            SRCS "src/wifi_manager.c"
            INCLUDE_DIRS "include"
            REQUIRES 
                core 
                interfaces
                esp_wifi
                esp_event
                nvs_flash
                wifi_provisioning
            PRIV_REQUIRES
                json
                esp_system
        )
    else()
        # მოდული გამორთულია - ცარიელი placeholder
        message(STATUS "Wifi_Manager Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
        
        # შევქმნათ ცარიელი source ფაილი რათა CMake არ ჩავარდეს
        set(EMPTY_SOURCE_CONTENT "// wifi_manager module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors\n")
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_placeholder.c" "${EMPTY_SOURCE_CONTENT}")
        
        idf_component_register(
            SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_placeholder.c"
            INCLUDE_DIRS "include"  # Header-ები მაინც საჭიროა factory-სთვის
            REQUIRES core interfaces
        )
    endif()
else()
    # CONFIG ცვლადი ჯერ არ არსებობს (configure ეტაპი) - default რეგისტრაცია
    message(STATUS "Wifi_Manager Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    idf_component_register(
        SRCS "src/wifi_manager.c"
        INCLUDE_DIRS "include"
        REQUIRES 
            core 
            interfaces
            esp_wifi
            esp_event
            nvs_flash
            wifi_provisioning
        PRIV_REQUIRES
            json
            esp_system
    )
endif()
