# ==============================================================================
# Synapse Framework - Decentralized Config Collector (v2.0 - Synthesized)
# ==============================================================================
# This script finds all `config.json` files, stages them for embedding with
# unique names, and generates a C header file to access them at runtime.

function(collect_and_prepare_module_configs)
    message(STATUS "Synapse: Collecting decentralized module configurations...")

    set(MODULES_DIR "${CMAKE_SOURCE_DIR}/components/modules")
    if(NOT EXISTS "${MODULES_DIR}")
        message(WARNING "Synapse: 'components/modules' directory not found. No module configs will be embedded.")
        set(COLLECTED_EMBED_FILES "" PARENT_SCOPE)
        set(CONFIG_SYMBOLS_DECLARE "// No module configs found." PARENT_SCOPE)
        set(CONFIG_ARRAY_ENTRIES "" PARENT_SCOPE)
        return()
    endif()

    file(GLOB_RECURSE MODULE_CONFIG_FILES "${MODULES_DIR}/**/config.json")

    # *** START OF DEBUGGING BLOCK ***
    message(STATUS "--- DEBUG: Found the following config.json files: ---")
    foreach(FILE ${MODULE_CONFIG_FILES})
        message(STATUS "  -> ${FILE}")
    endforeach()
    message(STATUS "--- END OF DEBUGGING BLOCK ---")
    # *** END OF DEBUGGING BLOCK ***

    if(NOT MODULE_CONFIG_FILES)
        message(STATUS "Synapse: No 'config.json' files found in modules. Skipping generation.")
        set(COLLECTED_EMBED_FILES "" PARENT_SCOPE)
        set(CONFIG_SYMBOLS_DECLARE "// No module configs found." PARENT_SCOPE)
        set(CONFIG_ARRAY_ENTRIES "" PARENT_SCOPE)
        return()
    endif()

    set(EMBEDDED_FILES_LIST "")
    set(SYMBOLS_DECLARE_CONTENT "")
    set(ARRAY_ENTRIES_CONTENT "")
    set(MODULE_COUNT 0)

    foreach(CONFIG_FILE ${MODULE_CONFIG_FILES})
        get_filename_component(MODULE_DIR ${CONFIG_FILE} DIRECTORY)
        get_filename_component(MODULE_NAME ${MODULE_DIR} NAME)

        set(DEST_FILENAME "${MODULE_NAME}_config.json")
        set(DEST_PATH "${CMAKE_CURRENT_BINARY_DIR}/${DEST_FILENAME}")
        
        # Copy file to build dir to ensure a unique name for embedding
        configure_file(${CONFIG_FILE} ${DEST_PATH} COPYONLY)
        list(APPEND EMBEDDED_FILES_LIST ${DEST_PATH})

        # Generate C symbol names based on the unique filename.
        # ESP-IDF sanitizes the filename to create the symbol.
        set(SYMBOL_BASE_NAME "_binary_${MODULE_NAME}_config_json")

        # Generate C code snippets for the header file
        string(APPEND SYMBOLS_DECLARE_CONTENT "extern const uint8_t ${SYMBOL_BASE_NAME}_start[];\n")
        string(APPEND SYMBOLS_DECLARE_CONTENT "extern const uint8_t ${SYMBOL_BASE_NAME}_end[];\n")
        string(APPEND ARRAY_ENTRIES_CONTENT "    { \"${MODULE_NAME}\", ${SYMBOL_BASE_NAME}_start, ${SYMBOL_BASE_NAME}_end },\n")
        
        math(EXPR MODULE_COUNT "${MODULE_COUNT} + 1")
        message(STATUS "Synapse: Staged config for module '${MODULE_NAME}'")
    endforeach()

    # Generate the final C header file from the template
    set(CONFIG_SYMBOLS_DECLARE ${SYMBOLS_DECLARE_CONTENT})
    set(CONFIG_ARRAY_ENTRIES ${ARRAY_ENTRIES_CONTENT})
    
    configure_file(
        "${CMAKE_CURRENT_LIST_DIR}/embedded_configs.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/embedded_configs.h"
    )
    
    message(STATUS "Synapse: Generated 'embedded_configs.h' for ${MODULE_COUNT} modules.")

    # Expose the list of files to be embedded to the parent scope
    set(COLLECTED_EMBED_FILES ${EMBEDDED_FILES_LIST} PARENT_SCOPE)
endfunction()