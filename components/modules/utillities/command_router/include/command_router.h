/**
 * @file command_router.h
 * @brief Public header for the Command Router & System Shell Module.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-07-03
 * @details This module provides a central point for command registration and execution.
 *          It is designed to be extensible and source-agnostic.
 */

#ifndef COMMAND_ROUTER_H
#define COMMAND_ROUTER_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the Command Router module.
 *
 * This function allocates and initializes all necessary resources for the module
 * based on the provided JSON configuration.
 *
 * @param[in] config A cJSON object containing the module's configuration,
 *                   typically from `system_config.json`. Can be NULL to use
 *                   defaults from Kconfig.
 * @return A pointer to the created module (`module_t*`), or NULL on failure.
 *
 * @note The created module must be registered with the system_manager to be
 *       part of the system's lifecycle.
 * @warning The caller is responsible for ensuring the module's memory is eventually
 *          freed by calling its `base.deinit()` function.
 */
module_t *command_router_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // COMMAND_ROUTER_H