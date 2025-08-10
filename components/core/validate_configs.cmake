# components/core/validate_configs.cmake

function(validate_synapse_configs)
    message(STATUS "Synapse: Starting configuration validation...")

    find_package(Python3 COMPONENTS Interpreter REQUIRED)

    set(VALIDATOR_SCRIPT "${CMAKE_SOURCE_DIR}/tools/validate_jsons.py")
    set(MODULE_SCHEMA "${CMAKE_SOURCE_DIR}/tools/schemas/module_schema.json")
    set(MODULES_DIR "${CMAKE_SOURCE_DIR}/components/modules")

    # --- Validate all module.json files ---
    file(GLOB_RECURSE MODULE_JSON_FILES "${MODULES_DIR}/**/module.json")
    foreach(MODULE_JSON ${MODULE_JSON_FILES})
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/validation_stamp/module_${MODULE_JSON_FILES_COUNT}.stamp"
            COMMAND ${Python3_EXECUTABLE} ${VALIDATOR_SCRIPT} ${MODULE_JSON} ${MODULE_SCHEMA} "module"
            DEPENDS ${MODULE_JSON} ${MODULE_SCHEMA} ${VALIDATOR_SCRIPT}
            COMMENT "Validating ${MODULE_JSON}"
            VERBATIM
        )
        list(APPEND VALIDATION_STAMPS "${CMAKE_CURRENT_BINARY_DIR}/validation_stamp/module_${MODULE_JSON_FILES_COUNT}.stamp")
        math(EXPR MODULE_JSON_FILES_COUNT "${MODULE_JSON_FILES_COUNT} + 1")
    endforeach()

    # --- Validate all config.json files that have a corresponding schema.json ---
    file(GLOB_RECURSE CONFIG_SCHEMA_FILES "${MODULES_DIR}/**/schema.json")
    foreach(CONFIG_SCHEMA ${CONFIG_SCHEMA_FILES})
        get_filename_component(MODULE_DIR ${CONFIG_SCHEMA} DIRECTORY)
        set(CONFIG_JSON "${MODULE_DIR}/config.json")
        if(EXISTS ${CONFIG_JSON})
            add_custom_command(
                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/validation_stamp/config_${CONFIG_SCHEMA_FILES_COUNT}.stamp"
                COMMAND ${Python3_EXECUTABLE} ${VALIDATOR_SCRIPT} ${CONFIG_JSON} ${CONFIG_SCHEMA} "config"
                DEPENDS ${CONFIG_JSON} ${CONFIG_SCHEMA} ${VALIDATOR_SCRIPT}
                COMMENT "Validating ${CONFIG_JSON}"
                VERBATIM
            )
            list(APPEND VALIDATION_STAMPS "${CMAKE_CURRENT_BINARY_DIR}/validation_stamp/config_${CONFIG_SCHEMA_FILES_COUNT}.stamp")
            math(EXPR CONFIG_SCHEMA_FILES_COUNT "${CONFIG_SCHEMA_FILES_COUNT} + 1")
        endif()
    endforeach()

    # Create a single target that depends on all validation steps
    if(VALIDATION_STAMPS)
        add_custom_target(validate_configs ALL DEPENDS ${VALIDATION_STAMPS})
    endif()

    message(STATUS "Synapse: Config validation setup complete.")
endfunction()