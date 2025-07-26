/**
 * @file ui_events.h
 * @brief Handles all incoming events for the UI Manager.
 * @details This header declares the main event handling function for the UI system.
 *          This function acts as the primary entry point for processing events
 *          from the Event Bus, such as button presses and system status updates,
 *          and delegates actions to other UI components like the menu handler.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-08-14
 */
#ifndef UI_EVENTS_H
#define UI_EVENTS_H

#include "ui_manager.h"

/**
 * @brief The main event handler function for the UI Manager module.
 * @details This function is registered as the module's `handle_event` callback.
 *          It receives all events the module is subscribed to and orchestrates
 *          the UI's response. It manages UI state transitions, screen wake-up
 *          logic, and delegates navigation-specific actions to the menu handler.
 * @param[in] self A pointer to the `ui_manager` module instance.
 * @param[in] event_name The name of the event that was triggered.
 * @param[in] event_data A pointer to the event's data wrapper. The handler is
 *                       responsible for calling `fmw_event_data_release` on this
 *                       pointer after processing the data.
 */
void ui_events_handle(module_t *self, const char *event_name, void *event_data);

#endif // UI_EVENTS_H