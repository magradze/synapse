/**
 * @file logger_module.h
 * @brief Logger Module - Centralized logging and event monitoring module
 * @details This module provides centralized logging functionality by capturing
 *          all events from the Event Bus and optionally outputting them to
 *          the serial console for debugging purposes. It supports runtime
 *          enable/disable through framework command events.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-06-26
 */

#ifndef LOGGER_MODULE_H
#define LOGGER_MODULE_H

#include "base_module.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new Logger Module instance.
 * @details This function creates and initializes a new logger module instance
 *          that subscribes to all events on the Event Bus and outputs debugging
 *          information to the serial console. The module supports runtime control
 *          through framework command events.
 * 
 * @param[in] config JSON configuration object containing module settings.
 *                   Expected fields:
 *                   - "instance_name" (string): Name of this logger instance
 *                                               (default: "main_logger")
 * 
 * @return module_t* Pointer to the created module instance, or NULL on failure.
 * 
 * @note The module will automatically subscribe to all system events through
 *       the System Manager's universal event subscription mechanism.
 * @note This module is typically disabled in production builds to avoid
 *       excessive logging output.
 * 
 * @see base_module.h for module interface details
 * @see event_bus.h for event subscription mechanism
 */
module_t *logger_module_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // LOGGER_MODULE_H