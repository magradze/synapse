# components/core/collect_modules.cmake
function(autogen_c_file_only)
    # --- ვეძებთ კონკრეტულად components საქაღალდეში ---
    file(GLOB_RECURSE module_json_files "${CMAKE_SOURCE_DIR}/components/**/module.json")
    message(STATUS "მოდულების ძიება... ნაპოვნია: ${module_json_files}")

    set(c_code "")
    # ... (აქ იგივე კოდია, რაც C ფაილს აგენერირებს) ...
    # ... ვამატებთ ჰედერებს და ა.შ. ...
    set(c_code "#include \"system_manager.h\"\n#include \"esp_log.h\"\n\n")
    
    foreach(json_file ${module_json_files})
        get_filename_component(module_dir ${json_file} DIRECTORY)
        get_filename_component(module_name ${module_dir} NAME)
        string(APPEND c_code "#include \"${module_name}.h\"\n")
    endforeach()
    
    string(APPEND c_code "\nvoid autogen_register_all_modules(void) {\n")
    foreach(json_file ${module_json_files})
        file(READ ${json_file} json_content)
        # ვცდილობთ წავიკითხოთ init_function ველი
        set(init_func "")
        string(JSON init_func_error ERROR_VARIABLE json_error GET ${json_content} "init_function")
        if(NOT json_error)
            string(JSON init_func GET ${json_content} "init_function")
        endif()
        get_filename_component(module_dir ${json_file} DIRECTORY)
        get_filename_component(module_name ${module_dir} NAME)
        if(init_func)
            string(APPEND c_code "    system_register_module(${init_func}());\n")
        else()
            string(APPEND c_code "    system_register_module(${module_name}_create(NULL));\n")
        endif()
    endforeach()
    string(APPEND c_code "}\n")

    set(output_file "${CMAKE_BINARY_DIR}/autogen_module_registry.c")
    file(WRITE ${output_file} "${c_code}")

    set(AUTOGEN_C_FILE ${output_file} PARENT_SCOPE)
endfunction()

#
# CMake სკრიპტი, რომელიც პოულობს ყველა module.json ფაილს, აგენერირებს მოდულის factory-ს,
# და განსაზღვრავს რომელი მოდულები უნდა იყოს ჩართული აგებაში.
#

# --- ნაბიჯი 1: მოძებნეთ ყველა module.json ფაილი რეკურსიულად ---
file(GLOB_RECURSE MODULE_JSON_FILES "${CMAKE_CURRENT_LIST_DIR}/../modules/**/module.json")

# --- ნაბიჯი 2: დაამუშავეთ თითოეული მოდული ---
set(MODULE_CREATE_FUNCTIONS "")
set(MODULE_INCLUDE_HEADERS "")
set(COLLECTED_MODULE_DEPENDENCIES "")
set(ENABLED_MODULE_PATHS "")

foreach(MODULE_JSON_FILE ${MODULE_JSON_FILES})
    file(READ ${MODULE_JSON_FILE} MODULE_JSON_CONTENT)

    # --- ნაწილები მოდულის ინფორმაციისთვის ---
    string(JSON MODULE_NAME GET ${MODULE_JSON_CONTENT} name)
    get_filename_component(MODULE_DIR ${MODULE_JSON_FILE} DIRECTORY)
    get_filename_component(COMPONENT_NAME ${MODULE_DIR} NAME)

    # --- შეამოწმეთ Kconfig-ის მხარდაჭერა ---
    # ვეძებთ module.json-ში conditional compilation ველს
    string(JSON CONDITIONAL_CONFIG ERROR_VARIABLE json_error GET ${MODULE_JSON_CONTENT} "conditional_config")
    set(HAS_CONDITIONAL_CONFIG FALSE)
    set(CONFIG_VARIABLE "")
    
    if(NOT json_error AND CONDITIONAL_CONFIG)
        set(HAS_CONDITIONAL_CONFIG TRUE)
        set(CONFIG_VARIABLE ${CONDITIONAL_CONFIG})
        message(STATUS "მოდული ${MODULE_NAME} იყენებს კონდიციურ კონფიგურაციას: ${CONFIG_VARIABLE}")
    endif()
    
    # --- შეამოწმეთ, ჩართულია თუ არა მოდული აგებისთვის module.json-ით ---
    string(JSON BUILD_ENABLED GET ${MODULE_JSON_CONTENT} build_enabled)

    # თუ "build_enabled" მნიშვნელობა არის "false", გამოტოვეთ მოდული
    if("${BUILD_ENABLED}" STREQUAL "false")
        message(STATUS "აგებიდან გამოტოვებულია module.json-ით გამორთული მოდული: ${MODULE_NAME}")
        continue()
    endif()

    # --- თუ ჩართულია, დაამატეთ აგებაში ---
    string(JSON INIT_FUNCTION GET ${MODULE_JSON_CONTENT} init_function)

    # დაამატეთ კომპონენტის ბილიკი EXTRA_COMPONENT_DIRS სიაში
    get_filename_component(MODULE_PATH ${MODULE_JSON_FILE} DIRECTORY)
    list(APPEND ENABLED_MODULE_PATHS ${MODULE_PATH})

    # --- გენერაცია factory-სთვის include სტეიტმენტის (კონდიციურად თუ საჭიროა)
    if(HAS_CONDITIONAL_CONFIG)
        string(APPEND MODULE_INCLUDE_HEADERS "#ifdef ${CONFIG_VARIABLE}\n")
        string(APPEND MODULE_INCLUDE_HEADERS "#include \"${COMPONENT_NAME}.h\"\n")
        string(APPEND MODULE_INCLUDE_HEADERS "#endif\n")
    else()
        # თუ არ არის კონდიციური, მაინც ჩავსვათ კონდიციური include უსაფრთხოებისთვის
        # ვინაიდან ყველა მოდული CMake-ში რეგისტრირდება, მაგრამ runtime-ში ჩართვა კონტროლდება Kconfig-ით
        # logger_module-ისთვის ვიყენებთ არსებულ CONFIG_MODULE_LOGGER_ENABLED კონფიგურაციას
        if("${COMPONENT_NAME}" STREQUAL "logger_module")
            string(APPEND MODULE_INCLUDE_HEADERS "#ifdef CONFIG_MODULE_LOGGER_ENABLED\n")
            string(APPEND MODULE_INCLUDE_HEADERS "#include \"${COMPONENT_NAME}.h\"\n")
            string(APPEND MODULE_INCLUDE_HEADERS "#endif\n")
        else()
            # სხვა მოდულებისთვის ვიყენებთ ზოგად ფორმატს
            string(TOUPPER "${MODULE_NAME}" MODULE_NAME_UPPER)
            string(APPEND MODULE_INCLUDE_HEADERS "#ifdef CONFIG_MODULE_${MODULE_NAME_UPPER}_ENABLED\n")
            string(APPEND MODULE_INCLUDE_HEADERS "#include \"${COMPONENT_NAME}.h\"\n")
            string(APPEND MODULE_INCLUDE_HEADERS "#endif\n")
        endif()
    endif()

    # გენერაცია factory-ს ჩანაწერის შესაბამისი function pointer-ის ტიპით (კონდიციურად)
    if(HAS_CONDITIONAL_CONFIG)
        string(APPEND MODULE_CREATE_FUNCTIONS "#ifdef ${CONFIG_VARIABLE}\n")
        string(APPEND MODULE_CREATE_FUNCTIONS "    { \"${MODULE_NAME}\", (module_create_fn_t)&${INIT_FUNCTION} },\n")
        string(APPEND MODULE_CREATE_FUNCTIONS "#endif\n")
    else()
        # თუ არ არის კონდიციური, მაინც ჩავსვათ კონდიციური factory უსაფრთხოებისთვის
        if("${COMPONENT_NAME}" STREQUAL "logger_module")
            string(APPEND MODULE_CREATE_FUNCTIONS "#ifdef CONFIG_MODULE_LOGGER_ENABLED\n")
            string(APPEND MODULE_CREATE_FUNCTIONS "    { \"${MODULE_NAME}\", (module_create_fn_t)&${INIT_FUNCTION} },\n")
            string(APPEND MODULE_CREATE_FUNCTIONS "#endif\n")
        else()
            # სხვა მოდულებისთვის ვიყენებთ ზოგად ფორმატს
            string(APPEND MODULE_CREATE_FUNCTIONS "#ifdef CONFIG_MODULE_${MODULE_NAME_UPPER}_ENABLED\n")
            string(APPEND MODULE_CREATE_FUNCTIONS "    { \"${MODULE_NAME}\", (module_create_fn_t)&${INIT_FUNCTION} },\n")
            string(APPEND MODULE_CREATE_FUNCTIONS "#endif\n")
        endif()
    endif()
    
    # ყველა მოდული (conditional და unconditional) დამოკიდებულებების სიაში ემატება
    list(APPEND COLLECTED_MODULE_DEPENDENCIES ${COMPONENT_NAME})
endforeach()

# --- ნაბიჯი 3: ლოგირება დებაგისთვის ---
message(STATUS "საბოლოო აგების დამოკიდებულებები: ${COLLECTED_MODULE_DEPENDENCIES}")
message(STATUS "ჩართული მოდულების ბილიკები: ${ENABLED_MODULE_PATHS}")

# Export the enabled module paths for use in main CMakeLists.txt
# Note: Since this script is included (not called as function), we don't need PARENT_SCOPE

message(STATUS "--- გენერირებული factory ფაილების კონფიგურაცია ---")
message(STATUS "სათაურები ჩასასმელად:\n${MODULE_INCLUDE_HEADERS}")
message(STATUS "სასაქმებელი factory ფუნქციები:\n${MODULE_CREATE_FUNCTIONS}")

# გამოიყენეთ configure_file ორივე template ფაილში placeholder-ების ჩასანაცვლებლად.
configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/generated_module_factory.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated_module_factory.h"
)
configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/generated_module_factory.c.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated_module_factory.c"
)

# --- ნაბიჯი 5: დაამატეთ გენერირებული ფაილი კომპონენტის წყაროების სიაში ---
# Note: SRCS is used within the core component's CMakeLists.txt context
list(APPEND SRCS "${CMAKE_CURRENT_BINARY_DIR}/generated_module_factory.c")

# --- ნაბიჯი 6: გენერირება Kconfig.projbuild ფაილისა ---
# მოძებნეთ ყველა მოდულის Kconfig ფაილი
set(MODULE_KCONFIG_SOURCES "")
foreach(MODULE_JSON_FILE ${MODULE_JSON_FILES})
    get_filename_component(MODULE_DIR ${MODULE_JSON_FILE} DIRECTORY)
    set(KCONFIG_PATH "${MODULE_DIR}/Kconfig")
    
    # შევამოწმოთ არსებობს თუ არა Kconfig ფაილი
    if(EXISTS ${KCONFIG_PATH})
        # გადავაკეთოთ relative path core დირექტორიიდან
        file(RELATIVE_PATH RELATIVE_KCONFIG_PATH "${CMAKE_CURRENT_LIST_DIR}" "${KCONFIG_PATH}")
        # დაამატოთ rsource ბრძანება
        string(APPEND MODULE_KCONFIG_SOURCES "    rsource \"${RELATIVE_KCONFIG_PATH}\"\n")
        message(STATUS "ნაპოვნია მოდულის Kconfig: ${RELATIVE_KCONFIG_PATH}")
    endif()
endforeach()

# შევქმნათ ავტომატური Kconfig.projbuild ფაილი
set(AUTO_KCONFIG_CONTENT "# Auto-generated Kconfig.projbuild for Synapse Framework
# This file is automatically generated by collect_modules.cmake
# Do not edit manually!

menu \"Synapse Framework\"

    # --- Framework Core Configuration ---
    rsource \"Kconfig\"

    # --- Module Configuration ---
${MODULE_KCONFIG_SOURCES}
endmenu
")

# ჩავწეროთ ფაილში
file(WRITE "${CMAKE_CURRENT_LIST_DIR}/Kconfig.projbuild" "${AUTO_KCONFIG_CONTENT}")
message(STATUS "აგენერირებულია ავტომატური Kconfig.projbuild ფაილი")
message(STATUS "ნაპოვნია ${CMAKE_MATCH_COUNT} მოდულის Kconfig ფაილი")