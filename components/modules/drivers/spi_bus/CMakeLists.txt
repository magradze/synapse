# SPI Bus Module CMake Configuration
# Component for A centralized, thread-safe driver for an SPI hardware bus.
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_SPI_BUS_ENABLED OR CONFIG_MODULE_SPI_BUS_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_SPI_BUS_ENABLED)
        message(STATUS "SPI Bus Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "SPI Bus Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/spi_bus.c"
        INCLUDE_DIRS "include"
            REQUIRES 
                core 
                interfaces
        PRIV_REQUIRES
            json
            driver
    )
else()
    # მოდული გამორთულია
    message(STATUS "SPI Bus Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// spi_bus module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors.\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_spi_bus.c" "${EMPTY_SOURCE_CONTENT}")
    
    idf_component_register(
        SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_spi_bus.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
