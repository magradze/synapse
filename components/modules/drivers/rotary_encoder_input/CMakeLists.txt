# Rotary Encoder Input Module CMake Configuration
# Component for A universal module to handle rotary encoder input and publish standard navigation events.
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_ROTARY_ENCODER_INPUT_ENABLED OR CONFIG_MODULE_ROTARY_ENCODER_INPUT_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_ROTARY_ENCODER_INPUT_ENABLED)
        message(STATUS "Rotary Encoder Input Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "Rotary Encoder Input Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/rotary_encoder_input.c"
        INCLUDE_DIRS "include"
            REQUIRES 
                core 
                interfaces
                driver
        PRIV_REQUIRES
            json
    )
else()
    # მოდული გამორთულია
    message(STATUS "Rotary Encoder Input Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// rotary_encoder_input module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors.\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_rotary_encoder_input.c" "${EMPTY_SOURCE_CONTENT}")
    
    idf_component_register(
        SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_rotary_encoder_input.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
