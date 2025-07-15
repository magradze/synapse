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

// --- Module-specific Status/Data Events ---
#define FMW_EVENT_WIFI_STATUS_READY "WIFI_STATUS_READY" // WiFi module reports its status
#define FMW_EVENT_DEVICE_INFO_READY "DEVICE_INFO_READY" // Device identity service reports its information

// --- Self Test Events ---
#define FMW_EVENT_SELF_TEST_REPORT_READY "FMW_SELF_TEST_REPORT_READY" // Self Test Manager publishes this when a report is ready

// --- Sensor Aggregator Events ---
#define FMW_EVENT_AGGREGATED_SENSOR_REPORT "FMW_AGGREGATED_SENSOR_REPORT" // Sensor Aggregator publishes this when it has aggregated data

// --- Security Status Events ---
#define FMW_EVENT_SECURITY_STATUS_READY "FMW_SECURITY_STATUS_READY"

/**
 * @brief Generic event published by any module that establishes a critical external connection.
 * @details The payload should be of type fmw_connectivity_payload_t.
 */
#define FMW_EVENT_CONNECTIVITY_ESTABLISHED "FMW_CONNECTIVITY_ESTABLISHED"

/**
 * @brief Generic event published by any module that loses a critical external connection.
 * @details The payload should be of type fmw_connectivity_payload_t.
 */
#define FMW_EVENT_CONNECTIVITY_LOST "FMW_CONNECTIVITY_LOST"

#endif // FRAMEWORK_EVENTS_H