/**
 * @file alarms_manager_interface.h
 * @brief Public Service API contract for the Alarms Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-13
 * @details This header defines the abstract interface for the Alarms Manager.
 *          Currently, the Alarms Manager is a pure event consumer and does not
 *          expose a public API for other modules to call. However, this file
 *          is created as a placeholder for future extensions and to maintain
 *          architectural consistency.
 *
 *          If, in the future, other modules need to programmatically trigger
 *          or query alarms, the API functions would be defined here.
 */

#ifndef SYNAPSE_ALARMS_MANAGER_INTERFACE_H
#define SYNAPSE_ALARMS_MANAGER_INTERFACE_H

#include "esp_err.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The public Service API structure for the Alarms Manager.
 * @details This structure is currently empty as the module's logic is
 *          entirely event-driven and configured via JSON. It serves as a
 *          placeholder for future API functions.
 *
 *          Example of a potential future function:
 *          esp_err_t (*trigger_manual_alarm)(const char* alarm_name, const cJSON* context);
 */
typedef struct
{
    /**
     * @brief A placeholder member to ensure the struct is not empty.
     *
     * C standard does not allow empty structs. This member can be removed
     * once a real API function is added.
     */
    void* reserved;

} alarms_manager_api_t;


#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_ALARMS_MANAGER_INTERFACE_H