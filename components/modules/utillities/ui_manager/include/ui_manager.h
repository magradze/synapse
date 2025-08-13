/**
 * @file ui_manager.h
 * @brief Public factory function for the UI Manager module.
 * @author Synapse Framework Team
 * @version 6.0.0
 * @date 2025-09-12
 */
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "synapse.h"

// Forward declaration of the private data struct.
// The full definition will be in ui_manager_internal.h
typedef struct ui_manager_private_data_t ui_manager_private_data_t;

/**
 * @brief Creates a new instance of the UI Manager module.
 * @details This is the factory function for the module, called by the Module Factory.
 * @param[in] config A cJSON object containing the configuration for this instance.
 * @return A pointer to the newly created module_t instance, or NULL on failure.
 */
module_t* ui_manager_create(const cJSON *config);

#endif // UI_MANAGER_H