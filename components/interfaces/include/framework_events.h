#ifndef FRAMEWORK_EVENTS_H
#define FRAMEWORK_EVENTS_H

/**
 * @file framework_events.h
 * @brief Defines standardized, framework-wide event identifiers.
 * @details This file contains event names that are considered part of the
 *          Synapse Framework's public eventing API. Any module can rely on
 *          these events being present and having a consistent meaning.
 */

// --- Command & Control Events ---
#define FMW_EVENT_EXECUTE_COMMAND_STRING "FMW_EXECUTE_COMMAND_STRING"

// --- System State Events ---
#define FMW_EVENT_SYSTEM_START_COMPLETE "FMW_SYSTEM_START_COMPLETE"
#define FMW_EVENT_CONFIG_UPDATED "FMW_CONFIG_UPDATED"

// --- Module Lifecycle Events ---
#define FMW_EVENT_MODULE_ENABLED "FMW_MODULE_ENABLED"
#define FMW_EVENT_MODULE_DISABLED "FMW_MODULE_DISABLED"

#endif // FRAMEWORK_EVENTS_H