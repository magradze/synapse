# Timer Test Module Module CMake Configuration
# Component for A module to test the System Timer and RGB LED services.
# Author: Synapse Framework Team
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება placeholder-ით
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით

# შევამოწმოთ არსებობს თუ არა CONFIG ცვლადი (configure ეტაპის შემდეგ)
if(DEFINED CONFIG_MODULE_TIMER_TEST_ENABLED)
    # CONFIG ცვლადი არსებობს - ვამოწმებთ მნიშვნელობას
    if(CONFIG_MODULE_TIMER_TEST_ENABLED)
        # მოდული ჩართულია - სრული რეგისტრაცია
        message(STATUS "Timer Test Module Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
        idf_component_register(
            SRCS "src/timer_test_module.c"
            INCLUDE_DIRS "include"
            REQUIRES 
                core 
                interfaces
                system_timer      # ★★★ დამოკიდებულება ტაიმერის სერვისზე ★★★
                rgb_led_indicator # ★★★ დამოკიდებულება LED-ის სერვისზე ★★★
            PRIV_REQUIRES
                json
        )
    else()
        # მოდული გამორთულია - ცარიელი placeholder
        message(STATUS "Timer Test Module Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
        
        # შევქმნათ ცარიელი source ფაილი რათა CMake არ ჩავარდეს
        set(EMPTY_SOURCE_CONTENT "// timer_test_module module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors\n")
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_placeholder.c" "${EMPTY_SOURCE_CONTENT}")
        
        idf_component_register(
            SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_placeholder.c"
            INCLUDE_DIRS "include"  # Header-ები მაინც საჭიროა factory-სთვის
            REQUIRES core interfaces
        )
    endif()
else()
    # CONFIG ცვლადი ჯერ არ არსებობს (configure ეტაპი) - default რეგისტრაცია
    message(STATUS "Timer Test Module Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    idf_component_register(
        SRCS "src/timer_test_module.c"
        INCLUDE_DIRS "include"
        REQUIRES 
            core 
            interfaces
            system_timer      # ★★★ დამოკიდებულება ტაიმერის სერვისზე ★★★
            rgb_led_indicator # ★★★ დამოკიდებულება LED-ის სერვისზე ★★★
        PRIV_REQUIRES
            json
    )
endif()
