/**
 * @file self_test_manager.h
 * @brief Public header for the Self Test Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-15
 * @details This file only contains the create function, as the module
 *          does not expose a public service API. It is controlled via
 *          the Command Router.
 */

#ifndef SELF_TEST_MANAGER_H
#define SELF_TEST_MANAGER_H

#include "base_module.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the Self Test Manager module.
 * 
 * @param[in] config Module configuration from system_config.json.
 * @return A pointer to the created module, or NULL on failure.
 */
module_t *self_test_manager_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // SELF_TEST_MANAGER_H