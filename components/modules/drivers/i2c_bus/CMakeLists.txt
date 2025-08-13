# I2C Bus Module CMake Configuration
# Component for A module for I2C Bus
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_I2C_BUS_ENABLED OR CONFIG_MODULE_I2C_BUS_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_I2C_BUS_ENABLED)
        message(STATUS "I2C Bus Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "I2C Bus Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/i2c_bus.c"
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
    message(STATUS "I2C Bus Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// i2c_bus module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors.\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_i2c_bus.c" "${EMPTY_SOURCE_CONTENT}")
    
    idf_component_register(
        SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_i2c_bus.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
