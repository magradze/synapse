/**
 * @file synapse_utils.c
 * @brief Implementation of the Synapse core utility library.
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-09-11
 */
#include "synapse_utils.h"
#include <string.h>

// =========================================================================
//      Safe String Handling
// =========================================================================

char* synapse_safe_strncpy(char *dest, const char *src, size_t size)
{
    if (size == 0) {
        return dest;
    }
    
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
    return dest;
}

// =========================================================================
//      Safe Configuration Parsing Helpers
// =========================================================================

bool synapse_config_get_string_from_node(const char* tag, const cJSON *json_node, const char *key, char *dest, size_t dest_size)
{
    if (!json_node || !key || !dest || dest_size == 0) {
        return false; // Invalid arguments
    }

    const cJSON *item = cJSON_GetObjectItem(json_node, key);
    if (cJSON_IsString(item) && (item->valuestring != NULL)) {
        synapse_safe_strncpy(dest, item->valuestring, dest_size);
        return true;
    }

    ESP_LOGD(tag, "Config key '%s' not found or not a string. Using default.", key);
    return false;
}

bool synapse_config_get_int_from_node(const char* tag, const cJSON *json_node, const char *key, int *out_value)
{
    if (!json_node || !key || !out_value) {
        return false; // Invalid arguments
    }

    const cJSON *item = cJSON_GetObjectItem(json_node, key);
    if (cJSON_IsNumber(item)) {
        *out_value = item->valueint;
        return true;
    }
    
    ESP_LOGD(tag, "Config key '%s' not found or not a number. Using default.", key);
    return false;
}

bool synapse_config_get_bool_from_node(const char* tag, const cJSON *json_node, const char *key, bool *out_value)
{
    if (!json_node || !key || !out_value) {
        return false; // Invalid arguments
    }

    const cJSON *item = cJSON_GetObjectItem(json_node, key);
    if (cJSON_IsBool(item)) {
        *out_value = cJSON_IsTrue(item);
        return true;
    }

    ESP_LOGD(tag, "Config key '%s' not found or not a boolean. Using default.", key);
    return false;
}