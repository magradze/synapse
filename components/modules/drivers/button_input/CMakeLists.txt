# Button Input Module CMake Configuration
# Component for A module to handle hardware button presses and publish events.
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_BUTTON_INPUT_ENABLED OR CONFIG_MODULE_BUTTON_INPUT_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_BUTTON_INPUT_ENABLED)
        message(STATUS "Button Input Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "Button Input Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/button_input.c"
        INCLUDE_DIRS "include"
            REQUIRES 
                core 
                interfaces
        PRIV_REQUIRES
            json
    )
else()
    # მოდული გამორთულია
    message(STATUS "Button Input Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// button_input module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors.\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_button_input.c" "${EMPTY_SOURCE_CONTENT}")
    
    idf_component_register(
        SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_button_input.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
