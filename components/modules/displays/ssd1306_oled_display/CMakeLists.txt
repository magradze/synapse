# SSD1306 OLED Display Module CMake Configuration
# Component for A module to control SSD1306 OLED displays via I2C.
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_SSD1306_OLED_DISPLAY_ENABLED OR CONFIG_MODULE_SSD1306_OLED_DISPLAY_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_SSD1306_OLED_DISPLAY_ENABLED)
        message(STATUS "SSD1306 OLED Display Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "SSD1306 OLED Display Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/ssd1306_oled_display.c" 
             "src/ssd1306_fonts.c"
             "include/wifi_icons.h"
        INCLUDE_DIRS "include"
            REQUIRES 
                core 
                interfaces
        PRIV_REQUIRES
            json
    )
else()
    # მოდული გამორთულია
    message(STATUS "SSD1306 OLED Display Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// ssd1306_oled_display module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors.\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_ssd1306_oled_display.c" "${EMPTY_SOURCE_CONTENT}")
    
    idf_component_register(
        SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_ssd1306_oled_display.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
