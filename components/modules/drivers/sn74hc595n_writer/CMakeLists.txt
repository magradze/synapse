# SN74HC595N Writer Module CMake Configuration
# Component for A driver module for the SN74HC595N shift register to expand output pins using SPI.
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_SN74HC595N_WRITER_ENABLED OR CONFIG_MODULE_SN74HC595N_WRITER_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_SN74HC595N_WRITER_ENABLED)
        message(STATUS "SN74HC595N Writer Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "SN74HC595N Writer Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/sn74hc595n_writer.c"
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
    message(STATUS "SN74HC595N Writer Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// sn74hc595n_writer module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors.\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_sn74hc595n_writer.c" "${EMPTY_SOURCE_CONTENT}")
    
    idf_component_register(
        SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_sn74hc595n_writer.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
