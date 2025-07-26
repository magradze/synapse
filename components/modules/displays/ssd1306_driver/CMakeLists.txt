# SSD1306 Driver Module CMake Configuration
# Component for Provides a low-level driver for SSD1306 OLED displays.
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_SSD1306_DRIVER_ENABLED OR CONFIG_MODULE_SSD1306_DRIVER_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_SSD1306_DRIVER_ENABLED)
        message(STATUS "SSD1306 Driver Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "SSD1306 Driver Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/ssd1306_driver.c" 
             "src/ssd1306_fonts.c"
        INCLUDE_DIRS "include"
            REQUIRES 
                core 
                interfaces
        PRIV_REQUIRES
            json
    )
else()
    # მოდული გამორთულია
    message(STATUS "SSD1306 Driver Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// ssd1306_driver module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors.\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_ssd1306_driver.c" "${EMPTY_SOURCE_CONTENT}")
    
    idf_component_register(
        SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_ssd1306_driver.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
