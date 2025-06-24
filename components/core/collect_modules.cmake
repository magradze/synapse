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

# --- ნაბიჯი 1: მოძებნეთ ყველა module.json ფაილი ---
file(GLOB_RECURSE MODULE_JSON_FILES "${CMAKE_CURRENT_LIST_DIR}/../modules/*/module.json")

# --- ნაბიჯი 2: დაამუშავეთ თითოეული მოდული ---
set(MODULE_CREATE_FUNCTIONS "")
set(MODULE_INCLUDE_HEADERS "")
set(COLLECTED_MODULE_DEPENDENCIES "")
set(ENABLED_MODULE_PATHS "")

foreach(MODULE_JSON_FILE ${MODULE_JSON_FILES})
    file(READ ${MODULE_JSON_FILE} MODULE_JSON_CONTENT)

    # --- შეამოწმეთ, ჩართულია თუ არა მოდული აგებისთვის ---
    string(JSON BUILD_ENABLED GET ${MODULE_JSON_CONTENT} build_enabled)

    # თუ "build_enabled" მნიშვნელობა არის "false", გამოტოვეთ მოდული
    if("${BUILD_ENABLED}" STREQUAL "false")
        string(JSON MODULE_NAME GET ${MODULE_JSON_CONTENT} name)
        message(STATUS "აგებიდან გამოტოვებულია გამორთული მოდული: ${MODULE_NAME}")
        continue()
    endif()

    # --- თუ ჩართულია, დაამატეთ აგებაში ---
    string(JSON MODULE_NAME GET ${MODULE_JSON_CONTENT} name)
    string(JSON INIT_FUNCTION GET ${MODULE_JSON_CONTENT} init_function)
    get_filename_component(MODULE_DIR ${MODULE_JSON_FILE} DIRECTORY)
    get_filename_component(COMPONENT_NAME ${MODULE_DIR} NAME)

    # დაამატეთ კომპონენტის სახელი core კომპონენტის დამოკიდებულებების სიაში
    list(APPEND COLLECTED_MODULE_DEPENDENCIES ${COMPONENT_NAME})

    # დაამატეთ კომპონენტის ბილიკი EXTRA_COMPONENT_DIRS სიაში
    get_filename_component(MODULE_PATH ${MODULE_JSON_FILE} DIRECTORY)
    list(APPEND ENABLED_MODULE_PATHS ${MODULE_PATH})

    # გენერაცია factory-სთვის include სტეიტმენტის
    string(APPEND MODULE_INCLUDE_HEADERS "#include \"${COMPONENT_NAME}.h\"\n")

    # გენერაცია factory-ს ჩანაწერის შესაბამისი function pointer-ის ტიპით
    string(APPEND MODULE_CREATE_FUNCTIONS "    { \"${MODULE_NAME}\", (module_create_fn_t)&${INIT_FUNCTION} },\n")
endforeach()

# --- ნაბიჯი 3: ლოგირება დებაგისთვის ---
message(STATUS "საბოლოო აგების დამოკიდებულებები: ${COLLECTED_MODULE_DEPENDENCIES}")
message(STATUS "ჩართული მოდულების ბილიკები: ${ENABLED_MODULE_PATHS}")

# აქ ცვლადების სახელები უნდა ემთხვეოდეს .in ფაილებში არსებულ placeholders-ს.
set(MODULE_INCLUDE_HEADERS "${MODULE_INCLUDE_HEADERS}")
set(MODULE_CREATE_FUNCTIONS "${MODULE_CREATE_FUNCTIONS}")
set(ENABLED_MODULE_PATHS ${ENABLED_MODULE_PATHS} PARENT_SCOPE)

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
list(APPEND SRCS "${CMAKE_CURRENT_BINARY_DIR}/generated_module_factory.c")
set(SRCS ${SRCS} PARENT_SCOPE)