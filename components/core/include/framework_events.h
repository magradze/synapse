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
#define SYNAPSE_EVENT_EXECUTE_COMMAND_STRING "SYNAPSE_EXECUTE_COMMAND_STRING"

// --- System State Events ---
#define SYNAPSE_EVENT_SYSTEM_START_COMPLETE "SYNAPSE_SYSTEM_START_COMPLETE"
#define SYNAPSE_EVENT_CONFIG_UPDATED "SYNAPSE_CONFIG_UPDATED"

// --- Module Lifecycle Events ---
#define SYNAPSE_EVENT_MODULE_ENABLED "SYNAPSE_MODULE_ENABLED"
#define SYNAPSE_EVENT_MODULE_DISABLED "SYNAPSE_MODULE_DISABLED"

// --- Module-specific Status/Data Events ---
#define SYNAPSE_EVENT_WIFI_STATUS_READY "WIFI_STATUS_READY" // WiFi module reports its status
#define SYNAPSE_EVENT_DEVICE_INFO_READY "DEVICE_INFO_READY" // Device identity service reports its information

// --- Self Test Events ---
#define SYNAPSE_EVENT_SELF_TEST_REPORT_READY "SYNAPSE_SELF_TEST_REPORT_READY" // Self Test Manager publishes this when a report is ready

// --- Sensor Aggregator Events ---
#define SYNAPSE_EVENT_AGGREGATED_SENSOR_REPORT "SYNAPSE_AGGREGATED_SENSOR_REPORT" // Sensor Aggregator publishes this when it has aggregated data

// --- Security Status Events ---
#define SYNAPSE_EVENT_SECURITY_STATUS_READY "SYNAPSE_SECURITY_STATUS_READY"

// --- Relay Actuator Events ---
#define SYNAPSE_EVENT_RELAY_STATE_CHANGED "SYNAPSE_RELAY_STATE_CHANGED"

// --- Watchdog Manager Events ---
#define SYNAPSE_EVENT_WATCHDOG_CHECK_TICK "SYNAPSE_WATCHDOG_CHECK_TICK"
#define SYNAPSE_EVENT_HEARTBEAT_MISSED "SYNAPSE_HEARTBEAT_MISSED"
#define SYNAPSE_EVENT_WATCHDOG_ALL_OK "SYNAPSE_WATCHDOG_ALL_OK"

// --- User Input Events ---
#define SYNAPSE_EVENT_BUTTON_PRESSED "BUTTON_PRESSED"
// #define SYNAPSE_EVENT_BUTTON_LONG_PRESSED "BUTTON_LONG_PRESSED"

/**
 * @brief Generic event published by any module that establishes a critical external connection.
 * @details The payload should be of type synapse_connectivity_payload_t.
 */
#define SYNAPSE_EVENT_CONNECTIVITY_ESTABLISHED "SYNAPSE_CONNECTIVITY_ESTABLISHED"

/**
 * @brief Generic event published by any module that loses a critical external connection.
 * @details The payload should be of type synapse_connectivity_payload_t.
 */
#define SYNAPSE_EVENT_CONNECTIVITY_LOST "SYNAPSE_CONNECTIVITY_LOST"

// --- System Shutdown Events ---
#define SYNAPSE_EVENT_SYSTEM_SHUTDOWN_REQUESTED "SYNAPSE_SYSTEM_SHUTDOWN_REQUESTED"

#endif // FRAMEWORK_EVENTS_H