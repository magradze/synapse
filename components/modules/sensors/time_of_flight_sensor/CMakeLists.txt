# Time of Flight Sensor Module CMake Configuration
# Component for Driver for I2C-based Time-of-Flight (TOF) distance sensors like VL53L0X.
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_TIME_OF_FLIGHT_SENSOR_ENABLED OR CONFIG_MODULE_TIME_OF_FLIGHT_SENSOR_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_TIME_OF_FLIGHT_SENSOR_ENABLED)
        message(STATUS "Time of Flight Sensor Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "Time of Flight Sensor Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/time_of_flight_sensor.c"
        INCLUDE_DIRS "include"
            REQUIRES 
                core 
                interfaces
        PRIV_REQUIRES
            json
    )
else()
    # მოდული გამორთულია
    message(STATUS "Time of Flight Sensor Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// time_of_flight_sensor module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors.\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_time_of_flight_sensor.c" "${EMPTY_SOURCE_CONTENT}")
    
    idf_component_register(
        SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_time_of_flight_sensor.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
