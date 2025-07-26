/**
 * @file ui_events.c
 * @brief Implements the main event handling logic for the UI Manager.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-08-12
 */
#include "ui_events.h"
#include "ui_menu.h"
#include "logging.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "event_payloads.h"
#include "framework_events.h"
#include "esp_timer.h"

DEFINE_COMPONENT_TAG("UI_EVENTS");

/**
 * @brief The main event handler function for the UI Manager module.
 * @details This function is registered as the module's `handle_event` callback and
 *          is called by the Event Bus for every subscribed event. It serves as the
 *          central processing unit for all incoming events.
 *
 *          Its responsibilities include:
 *          - Handling the splash screen timer and transitioning to the home screen.
 *          - Managing screen wake-up and resetting the auto-off timer upon user activity.
 *          - Debouncing button presses to prevent multiple triggers.
 *          - Dispatching button press events to the appropriate handler (e.g., `ui_menu_handle_navigation`).
 *          - Triggering periodic requests for system status updates (e.g., WiFi).
 *          - Processing status update responses and flagging the UI for a re-render.
 *          - Handling the screen auto-off event.
 *          - Ensuring `fmw_event_data_release` is called on all event data wrappers.
 *
 * @param[in] self A pointer to the `ui_manager` module instance.
 * @param[in] event_name The name of the event that was triggered.
 * @param[in] event_data A pointer to the event's data wrapper.
 */
void ui_events_handle(module_t *self, const char *event_name, void *event_data) {
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;
    bool needs_render = false;

    if (private_data->current_state == UI_STATE_SPLASH) {
        if (strcmp(event_name, SPLASH_SCREEN_TIMER_EVENT) == 0) {
            ESP_LOGI(TAG, "Splash screen timer ended. Transitioning to HOME state.");
            private_data->current_state = UI_STATE_HOME;
            ui_menu_populate_items(private_data);
            private_data->home_screen_timer = private_data->timer->schedule_event("UI_HOME_UPDATE", HOME_SCREEN_UPDATE_INTERVAL_MS, true);
            private_data->wifi_status_timer = private_data->timer->schedule_event(WIFI_STATUS_TIMER_EVENT, WIFI_STATUS_UPDATE_INTERVAL_MS, true);
            reset_screen_off_timer(private_data);
            needs_render = true;
        }
    } else {
        if (strcmp(event_name, FMW_EVENT_BUTTON_PRESSED) == 0) {
            reset_screen_off_timer(private_data);
            if (!private_data->is_screen_on) {
                private_data->display->api->set_power(private_data->display->context, true);
                private_data->is_screen_on = true;
                needs_render = true;
            } else {
                int64_t now = esp_timer_get_time();
                if ((now - private_data->last_button_press_time) < (BUTTON_COOLDOWN_MS * 1000)) {
                    if (event_data) fmw_event_data_release((event_data_wrapper_t *)event_data);
                    return;
                }
                private_data->last_button_press_time = now;

                if (!event_data || !((event_data_wrapper_t *)event_data)->payload) {
                    if (event_data) fmw_event_data_release((event_data_wrapper_t *)event_data);
                    return;
                }
                const char *button_name = ((fmw_button_payload_t *)((event_data_wrapper_t *)event_data)->payload)->button_name;

                if (private_data->current_state == UI_STATE_HOME) {
                    if (strcmp(button_name, "OK") == 0) {
                        private_data->current_state = UI_STATE_MAIN_MENU;
                        private_data->selected_item_index = 0;
                    }
                } else {
                    ui_menu_handle_navigation(private_data, button_name);
                }
                needs_render = true;
            }
        } else if (strcmp(event_name, "UI_HOME_UPDATE") == 0) {
            if (private_data->current_state == UI_STATE_HOME) needs_render = true;
        } else if (strcmp(event_name, WIFI_STATUS_TIMER_EVENT) == 0) {
            fmw_command_payload_t *payload = calloc(1, sizeof(fmw_command_payload_t));
            if (payload) {
                snprintf(payload->command_string, sizeof(payload->command_string), "wifi status --silent");
                snprintf(payload->source, sizeof(payload->source), self->name);
                event_data_wrapper_t *wrapper;
                if (fmw_event_data_wrap(payload, fmw_payload_common_free, &wrapper) == ESP_OK) {
                    fmw_event_bus_post(FMW_EVENT_EXECUTE_COMMAND_STRING, wrapper);
                    fmw_event_data_release(wrapper);
                } else {
                    free(payload);
                }
            }
        } else if (strcmp(event_name, FMW_EVENT_WIFI_STATUS_READY) == 0) {
            event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;
            if (wrapper && wrapper->payload) {
                fmw_telemetry_payload_t *payload = (fmw_telemetry_payload_t *)wrapper->payload;
                if (payload->json_data) {
                    cJSON *root = cJSON_Parse(payload->json_data);
                    if (root) {
                        const cJSON *conn_status = cJSON_GetObjectItem(root, "connection_status");
                        const cJSON *rssi_json = cJSON_GetObjectItem(root, "rssi");
                        bool new_status = (cJSON_IsString(conn_status) && strcmp(conn_status->valuestring, "Connected") == 0);
                        int32_t new_rssi = cJSON_IsNumber(rssi_json) ? rssi_json->valueint : -100;
                        if (new_status != private_data->wifi_connected || new_rssi != private_data->wifi_rssi) {
                            ESP_LOGI(TAG, "WiFi status changed. Connected: %d, RSSI: %d. Updating.", new_status, (int)new_rssi);
                            private_data->wifi_connected = new_status;
                            private_data->wifi_rssi = new_rssi;
                            needs_render = true;
                        }
                        cJSON_Delete(root);
                    }
                }
            }
        } else if (strcmp(event_name, SCREEN_OFF_TIMER_EVENT) == 0) {
            ESP_LOGI(TAG, "Screen off timer fired. Turning off display.");
            private_data->display->api->set_power(private_data->display->context, false);
            private_data->is_screen_on = false;
        }
    }

    if (needs_render && private_data->is_screen_on) {
        render_current_state(private_data);
    }

    if (event_data) {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}