/**
 * @file synapse_utils.h
 * @brief Public API for the Synapse core utility library.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-09-11
 * @details This header provides a collection of common, reusable helper
 *          functions to simplify module development, reduce code duplication,
 *          and enforce best practices for tasks like safe string handling
 *          and configuration parsing.
 */

#ifndef SYNAPSE_UTILS_H
#define SYNAPSE_UTILS_H

#include "synapse.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

// =========================================================================
//      Safe String Handling
// =========================================================================

/**
 * @brief A safe version of strncpy that guarantees null-termination.
 *
 * @param[out] dest The destination buffer.
 * @param[in]  src The source string.
 * @param[in]  size The full size of the destination buffer.
 * @return A pointer to the destination buffer `dest`.
 */
char* synapse_safe_strncpy(char *dest, const char *src, size_t size);


// =========================================================================
//      Safe Configuration Parsing Helpers
// =========================================================================

/**
 * @brief Safely retrieves a string value from a cJSON object.
 *
 * @details This function checks if the specified key exists, is a valid string,
 *          and copies it into the destination buffer using synapse_safe_strncpy.
 *          If any check fails, it logs a warning and leaves the destination buffer
 *          untouched (preserving its default value).
 *
 * @param[in]  tag The logging tag of the calling module.
 * @param[in]  json_node The parent cJSON object to search within.
 * @param[in]  key The key of the string value to retrieve.
 * @param[out] dest The destination buffer for the string.
 * @param[in]  dest_size The full size of the destination buffer.
 * @return
 *      - true: If the value was successfully found and copied.
 *      - false: If the key was not found or was not a valid string.
 */
bool synapse_config_get_string_from_node(const char* tag, const cJSON *json_node, const char *key, char *dest, size_t dest_size);

/**
 * @brief Safely retrieves an integer value from a cJSON object.
 *
 * @details Checks if the key exists and is a number. If successful, it updates
 *          the value pointed to by `out_value`. Otherwise, it logs a warning
 *          and leaves the original value unchanged.
 *
 * @param[in]  tag The logging tag of the calling module.
 * @param[in]  json_node The parent cJSON object.
 * @param[in]  key The key of the integer value.
 * @param[out] out_value A pointer to the integer variable to be updated.
 * @return
 *      - true: If the value was successfully found and updated.
 *      - false: If the key was not found or was not a valid number.
 */
bool synapse_config_get_int_from_node(const char* tag, const cJSON *json_node, const char *key, int *out_value);

/**
 * @brief Safely retrieves a boolean value from a cJSON object.
 *
 * @param[in]  tag The logging tag of the calling module.
 * @param[in]  json_node The parent cJSON object.
 * @param[in]  key The key of the boolean value.
 * @param[out] out_value A pointer to the bool variable to be updated.
 * @return
 *      - true: If the value was successfully found and updated.
 *      - false: If the key was not found or was not a valid boolean.
 */
bool synapse_config_get_bool_from_node(const char* tag, const cJSON *json_node, const char *key, bool *out_value);


#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_UTILS_H