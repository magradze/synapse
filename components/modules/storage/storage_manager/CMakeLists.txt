# Storage Manager Module CMake Configuration
# Component for centralized, multi-backend data persistence.
# Author: Synapse Framework Team
# Version: 2.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება placeholder-ით
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით

# შევამოწმოთ არსებობს თუ არა CONFIG ცვლადი (configure ეტაპის შემდეგ)
if(DEFINED CONFIG_MODULE_STORAGE_MANAGER_ENABLED)
    # CONFIG ცვლადი არსებობს - ვამოწმებთ მნიშვნელობას
    if(CONFIG_MODULE_STORAGE_MANAGER_ENABLED)
        # --- მოდული ჩართულია - სრული რეგისტრაცია ---
        message(STATUS "Storage Manager Module: ENABLED - კომპილირდება სრული ფუნქციონალით")

        # ★★★ ახალი ლოგიკა: დინამიურად ავაწყოთ წყარო ფაილების და დამოკიდებულებების სია ★★★
        
        # საბაზისო წყარო ფაილი
        set(STORAGE_SRCS "src/storage_manager.c")
        
        # საბაზისო დამოკიდებულებები
        set(STORAGE_REQUIRES core interfaces)

        # დავამატოთ backend-ის სპეციფიკური ფაილები და დამოკიდებულებები
        if(CONFIG_STORAGE_BACKEND_NVS)
            message(STATUS "Storage Manager: Compiling NVS backend.")
            list(APPEND STORAGE_SRCS "src/storage_backend_nvs.c")
            list(APPEND STORAGE_REQUIRES nvs_flash)
        elseif(CONFIG_STORAGE_BACKEND_SPIFFS)
            message(STATUS "Storage Manager: Compiling SPIFFS backend.")
            list(APPEND STORAGE_SRCS "src/storage_backend_spiffs.c")
            list(APPEND STORAGE_REQUIRES spiffs)
        elseif(CONFIG_STORAGE_BACKEND_SD_CARD)
            message(STATUS "Storage Manager: Compiling SD Card backend.")
            list(APPEND STORAGE_SRCS "src/storage_backend_sd.c")
            # SD ბარათს სჭირდება driver და sdmmc კომპონენტები
            list(APPEND STORAGE_REQUIRES driver sdmmc)
        endif()

        idf_component_register(
            SRCS ${STORAGE_SRCS}
            INCLUDE_DIRS "include" "src" # "src" დავამატოთ, რათა backend-ის .h ფაილები ხელმისაწვდომი იყოს
            REQUIRES ${STORAGE_REQUIRES}
            PRIV_REQUIRES
                json
        )
    else()
        # --- მოდული გამორთულია - ცარიელი placeholder ---
        message(STATUS "Storage Manager Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
        
        # შევქმნათ ცარიელი source ფაილი რათა CMake არ ჩავარდეს
        set(EMPTY_SOURCE_CONTENT "// storage_manager module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors\n")
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/storage_manager_placeholder.c" "${EMPTY_SOURCE_CONTENT}")
        
        idf_component_register(
            SRCS "${CMAKE_CURRENT_BINARY_DIR}/storage_manager_placeholder.c"
            INCLUDE_DIRS "include"  # Header-ები მაინც საჭიროა factory-სთვის
            REQUIRES core interfaces
        )
    endif()
else()
    # --- CONFIG ცვლადი ჯერ არ არსებობს (configure ეტაპი) - default რეგისტრაცია ---
    # ამ ეტაპზე, ჩვენ უნდა დავარეგისტრიროთ ყველა შესაძლო ფაილი და დამოკიდებულება,
    # რათა CMake-მ შეძლოს პროექტის სრული "გრაფის" აგება.
    message(STATUS "Storage Manager Module: CONFIGURE STAGE - დროებითი რეგისტრაცია ყველა შესაძლო წყაროთი")
    
    idf_component_register(
        SRCS 
            "src/storage_manager.c"
            "src/storage_backend_nvs.c"
            "src/storage_backend_spiffs.c"
            "src/storage_backend_sd.c"
        INCLUDE_DIRS "include" "src"
        REQUIRES 
            core 
            interfaces
            nvs_flash
            spiffs
            driver
            sdmmc
        PRIV_REQUIRES
            json
    )
endif()