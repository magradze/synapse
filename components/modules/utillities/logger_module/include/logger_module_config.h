/**
 * @file logger_module_config.h
 * @brief Logger Module Configuration Constants and Defaults
 * @details This header file contains all configuration constants, default values,
 *          and compile-time settings for the logger module. It centralizes
 *          configuration management and integrates with the Kconfig system.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-06-26
 */

#ifndef LOGGER_MODULE_CONFIG_H
#define LOGGER_MODULE_CONFIG_H

#include "sdkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// Module Enable/Disable Configuration
// =============================================================================

/** @brief Module compilation flag - determines if logger module is included */
#ifdef CONFIG_MODULE_LOGGER_ENABLED
    #define LOGGER_MODULE_ENABLED 1
#else
    #define LOGGER_MODULE_ENABLED 0
#endif

// =============================================================================
// Instance Name Configuration
// =============================================================================

/** @brief Default instance name for logger module */
#ifdef CONFIG_LOGGER_DEFAULT_INSTANCE_NAME
    #define LOGGER_DEFAULT_INSTANCE_NAME CONFIG_LOGGER_DEFAULT_INSTANCE_NAME
#else
    #define LOGGER_DEFAULT_INSTANCE_NAME "main_logger"
#endif

/** @brief Maximum length for instance name string */
#ifdef CONFIG_LOGGER_INSTANCE_NAME_MAX_LEN
    #define LOGGER_INSTANCE_NAME_MAX_LEN CONFIG_LOGGER_INSTANCE_NAME_MAX_LEN
#else
    #define LOGGER_INSTANCE_NAME_MAX_LEN 32
#endif

// =============================================================================
// Runtime Control Configuration
// =============================================================================

/** @brief Runtime control enable flag */
#ifdef CONFIG_LOGGER_RUNTIME_CONTROL_ENABLED
    #define LOGGER_RUNTIME_CONTROL_ENABLED 1
#else
    #define LOGGER_RUNTIME_CONTROL_ENABLED 0
#endif

/** @brief Console output enable flag */
#ifdef CONFIG_LOGGER_CONSOLE_OUTPUT_ENABLED
    #define LOGGER_CONSOLE_OUTPUT_ENABLED 1
#else
    #define LOGGER_CONSOLE_OUTPUT_ENABLED 0
#endif

// =============================================================================
// Buffer and Memory Configuration
// =============================================================================

/** @brief Event buffer size for message formatting */
#ifdef CONFIG_LOGGER_EVENT_BUFFER_SIZE
    #define LOGGER_EVENT_BUFFER_SIZE CONFIG_LOGGER_EVENT_BUFFER_SIZE
#else
    #define LOGGER_EVENT_BUFFER_SIZE 128
#endif

/** @brief Maximum event data size to process */
#ifdef CONFIG_LOGGER_MAX_EVENT_DATA_SIZE
    #define LOGGER_MAX_EVENT_DATA_SIZE CONFIG_LOGGER_MAX_EVENT_DATA_SIZE
#else
    #define LOGGER_MAX_EVENT_DATA_SIZE 256
#endif

// =============================================================================
// Validation Macros
// =============================================================================

/** @brief Compile-time validation of instance name length */
#if LOGGER_INSTANCE_NAME_MAX_LEN < 8
    #error "LOGGER_INSTANCE_NAME_MAX_LEN must be at least 8 characters"
#endif

#if LOGGER_INSTANCE_NAME_MAX_LEN > 64
    #error "LOGGER_INSTANCE_NAME_MAX_LEN cannot exceed 64 characters"
#endif

/** @brief Compile-time validation of buffer sizes */
#if LOGGER_EVENT_BUFFER_SIZE < 64
    #error "LOGGER_EVENT_BUFFER_SIZE must be at least 64 bytes"
#endif

#if LOGGER_MAX_EVENT_DATA_SIZE < 64
    #error "LOGGER_MAX_EVENT_DATA_SIZE must be at least 64 bytes"
#endif

// =============================================================================
// Helper Macros
// =============================================================================

/** @brief Conditional compilation macro for logger module features */
#if LOGGER_MODULE_ENABLED
    #define LOGGER_IF_ENABLED(code) do { code } while(0)
#else
    #define LOGGER_IF_ENABLED(code) do { } while(0)
#endif

/** @brief Conditional compilation macro for runtime control features */
#if LOGGER_RUNTIME_CONTROL_ENABLED
    #define LOGGER_IF_RUNTIME_CONTROL(code) do { code } while(0)
#else
    #define LOGGER_IF_RUNTIME_CONTROL(code) do { } while(0)
#endif

/** @brief Conditional compilation macro for console output */
#if LOGGER_CONSOLE_OUTPUT_ENABLED
    #define LOGGER_IF_CONSOLE_OUTPUT(code) do { code } while(0)
#else
    #define LOGGER_IF_CONSOLE_OUTPUT(code) do { } while(0)
#endif

#ifdef __cplusplus
}
#endif

#endif // LOGGER_MODULE_CONFIG_H
