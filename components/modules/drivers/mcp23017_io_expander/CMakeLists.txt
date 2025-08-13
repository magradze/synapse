# MCP23017 IO Expander Module CMake Configuration
# Component for A driver for the MCP23017 I2C 16-bit I/O expander.
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_MCP23017_IO_EXPANDER_ENABLED OR CONFIG_MODULE_MCP23017_IO_EXPANDER_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_MCP23017_IO_EXPANDER_ENABLED)
        message(STATUS "MCP23017 IO Expander Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "MCP23017 IO Expander Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/mcp23017_io_expander.c"
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
    message(STATUS "MCP23017 IO Expander Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// mcp23017_io_expander module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors.\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_mcp23017_io_expander.c" "${EMPTY_SOURCE_CONTENT}")
    
    idf_component_register(
        SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_mcp23017_io_expander.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
