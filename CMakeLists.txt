cmake_minimum_required(VERSION 3.16)

# First, include the script that will scan modules and define which ones are enabled.
include(components/core/collect_modules.cmake)

# Now, set the component directories based on the output of the script.
# This ensures only enabled modules are visible to the build system.
set(EXTRA_COMPONENT_DIRS
    components/core
    components/interfaces
    ${ENABLED_MODULE_PATHS} # This variable is set by collect_modules.cmake
    dependencies
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(synapse-fmw)

# This adds the -DCONFIG_DHT_USE_FREERTOS_SPINLOCK=1 flag during the compilation of the entire project
idf_build_set_property(COMPILE_DEFINITIONS
        "CONFIG_DHT_USE_FREERTOS_SPINLOCK=1"
        "CONFIG_DHT_RMT_SUPPORT=1"
        APPEND)