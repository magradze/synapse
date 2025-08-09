/**
 * @file logging.h
 * @brief Synapse ESP Framework's enhanced logging interface with color support.
 * @author Synapse Framework Team
 * @version 3.0.0
 * @date 2025-09-10
 *
 * @details
 * This file provides a macro for defining color-coded component tags for use
 * with the standard `esp_log.h` library. By embedding ANSI color codes
 * directly into the tag string, logs become easier to read and parse visually
 * without any performance overhead.
 */

#ifndef SYNAPSE_LOGGING_H
#define SYNAPSE_LOGGING_H

#include "esp_log.h"

// --- ANSI Color Code Definitions ---
// Use these constants as the second argument to DEFINE_COMPONENT_TAG.
#define SYNAPSE_LOG_COLOR_BLACK "\033[0;30m"
#define SYNAPSE_LOG_COLOR_RED "\033[0;31m"
#define SYNAPSE_LOG_COLOR_GREEN "\033[0;32m"
#define SYNAPSE_LOG_COLOR_YELLOW "\033[0;33m"
#define SYNAPSE_LOG_COLOR_BLUE "\033[0;34m"
#define SYNAPSE_LOG_COLOR_MAGENTA "\033[0;35m"
#define SYNAPSE_LOG_COLOR_CYAN "\033[0;36m"
#define SYNAPSE_LOG_COLOR_WHITE "\033[0;37m"
#define SYNAPSE_LOG_COLOR_RESET "\033[0m"

// Bright versions
#define SYNAPSE_LOG_COLOR_BRIGHT_BLUE "\033[1;34m"
#define SYNAPSE_LOG_COLOR_BRIGHT_MAGENTA "\033[1;35m"
#define SYNAPSE_LOG_COLOR_BRIGHT_CYAN "\033[1;36m"

/**
 * @brief Defines a local, static, color-coded TAG variable for logging.
 *
 * @details This macro creates a `static const char* TAG` variable that includes
 *          ANSI escape codes to color the tag in the console output. It should
 *          be used at the top of every .c file that requires logging.
 *
 *          Usage example in a .c file:
 *          @code
 *          #include "synapse.h" // Includes logging.h
 *
 *          // Define a bright blue tag for a core component
 *          DEFINE_COMPONENT_TAG("SYSTEM_MANAGER", SYNAPSE_LOG_COLOR_BRIGHT_BLUE);
 *
 *          void my_function() {
 *              // Use the standard ESP_LOGx macros as usual
 *              ESP_LOGI(TAG, "System manager has started.");
 *          }
 *          @endcode
 *
 * @param component_name The name of the component/module (string) to be displayed.
 * @param color The ANSI color code for the tag (e.g., SYNAPSE_LOG_COLOR_BLUE).
 */
#define DEFINE_COMPONENT_TAG(component_name, color) \
  static const char *TAG = color component_name SYNAPSE_LOG_COLOR_BLACK

#endif // SYNAPSE_LOGGING_H