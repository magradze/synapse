cmake_minimum_required(VERSION 3.16)

# First, include the script that will scan modules and define which ones are enabled.
include(components/core/collect_modules.cmake)

# Initialize the list of component directories
set(SYNAPSE_COMPONENT_DIRS
    components/core
    components/interfaces
    ${ENABLED_MODULE_PATHS} # This variable is set by collect_modules.cmake
    dependencies
)

# IMPORTANT: The 'managed_components' directory is created by the component manager
# AFTER the initial cmake run. We only add it to the path if it already exists
# to avoid errors on the first configuration run.
if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/managed_components)
    list(PREPEND SYNAPSE_COMPONENT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/managed_components)
endif()

# Now, set the final variable that ESP-IDF uses
set(EXTRA_COMPONENT_DIRS ${SYNAPSE_COMPONENT_DIRS})

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(synapse-fmw)

# This adds the -DCONFIG_DHT_USE_FREERTOS_SPINLOCK=1 flag during the compilation of the entire project
idf_build_set_property(COMPILE_DEFINITIONS
        "CONFIG_DHT_USE_FREERTOS_SPINLOCK=1"
        "CONFIG_DHT_RMT_SUPPORT=1"
        APPEND)