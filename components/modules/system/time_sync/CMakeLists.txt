# Time Sync Module CMake Configuration
# Component for Synchronizes system time using NTP/SNTP.
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_TIME_SYNC_ENABLED OR CONFIG_MODULE_TIME_SYNC_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_TIME_SYNC_ENABLED)
        message(STATUS "Time Sync Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "Time Sync Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/time_sync.c"
        INCLUDE_DIRS "include"
            REQUIRES 
                core 
                interfaces
                esp_netif
        PRIV_REQUIRES
            json
            lwip
    )
else()
    # მოდული გამორთულია
    message(STATUS "Time Sync Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// time_sync module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors.\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_time_sync.c" "${EMPTY_SOURCE_CONTENT}")
    
    idf_component_register(
        SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_time_sync.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
