# ==============================================================================
# Synapse Framework - MQTT Topics Auto-Generation
# ==============================================================================
function(collect_mqtt_topics)
    message(STATUS "🔍 Collecting MQTT topics from modules...")
    
    # Set paths
    set(MODULES_DIR "${CMAKE_SOURCE_DIR}/components/modules")
    set(OUTPUT_FILE "${CMAKE_SOURCE_DIR}/components/interfaces/include/mqtt_topics_generated.h")
    set(TEMPLATE_FILE "${CMAKE_SOURCE_DIR}/components/core/mqtt_topics_generated.h.in")
    set(PYTHON_SCRIPT "${CMAKE_SOURCE_DIR}/components/core/generate_mqtt_topics.py")
    
    # Find Python executable
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    
    # Execute Python script to generate MQTT topics
    # Note: system_config.json is no longer passed as an argument
    execute_process(
        COMMAND ${Python3_EXECUTABLE} ${PYTHON_SCRIPT} ${MODULES_DIR} ${OUTPUT_FILE} ${TEMPLATE_FILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE PYTHON_RESULT
        OUTPUT_VARIABLE PYTHON_OUTPUT
        ERROR_VARIABLE PYTHON_ERROR
    )
    
    # Check if Python script succeeded
    if(PYTHON_RESULT EQUAL 0)
        if(PYTHON_OUTPUT)
            message(STATUS "${PYTHON_OUTPUT}")
        endif()
    else()
        message(WARNING "❌ Python MQTT topics generation failed:")
        if(PYTHON_ERROR)
            message(WARNING "${PYTHON_ERROR}")
        endif()
        if(PYTHON_OUTPUT)
            message(WARNING "${PYTHON_OUTPUT}")
        endif()
        
        # Fallback: create minimal header file
        message(STATUS "Creating fallback MQTT topics header...")
        file(WRITE ${OUTPUT_FILE} 
"#ifndef MQTT_TOPICS_GENERATED_H
#define MQTT_TOPICS_GENERATED_H

// Fallback header - Python generation failed

#define MQTT_TOPIC_BASE \"synapse/devices/%s\"

#endif // MQTT_TOPICS_GENERATED_H
")
    endif()
    
endfunction()