/**
 * @file synapse_assert.h
 * @brief Custom assertion macro for the Synapse Framework.
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-09-10
 * @details Provides a powerful assertion macro that can be enabled or disabled
 *          via Kconfig. When enabled, it provides descriptive, formatted error
 *          messages upon failure, significantly aiding the debugging process.
 */

#ifndef SYNAPSE_ASSERT_H
#define SYNAPSE_ASSERT_H

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_SYNAPSE_ENABLE_ASSERTS

/**
 * @brief Custom assertion macro for Synapse Framework.
 *
 * @details Checks a condition and, if it is false, prints a formatted error
 *          message to the console and halts execution. This should be used to
 *          check for logical programming errors that should never occur in a
 *          correctly functioning program.
 *
 *          This macro is only active when CONFIG_SYNAPSE_ENABLE_ASSERTS is enabled
 *          in menuconfig. In release builds, it compiles to nothing.
 *
 * @param condition The condition to check.
 * @param format A printf-style format string for the error message.
 * @param ... Variable arguments for the format string.
 *
 * @example
 * SYNAPSE_ASSERT(handle != NULL, "Service handle for '%s' cannot be NULL!", service_name);
 */
#define SYNAPSE_ASSERT(condition, format, ...) \
    do { \
        if (!(condition)) { \
            ets_printf("SYNAPSE_ASSERT FAILED: %s:%d: ", __FILE__, __LINE__); \
            ets_printf(format, ##__VA_ARGS__); \
            ets_printf("\n"); \
            abort(); \
        } \
    } while(0)

#else // CONFIG_SYNAPSE_ENABLE_ASSERTS is disabled

// When disabled, the macro compiles to nothing.
#define SYNAPSE_ASSERT(condition, format, ...) ((void)0)

#endif // CONFIG_SYNAPSE_ENABLE_ASSERTS


#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_ASSERT_H