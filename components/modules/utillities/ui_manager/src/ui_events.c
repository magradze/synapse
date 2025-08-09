/**
 * @file ui_events.c
 * @brief Implements the main event handling logic for the UI Manager.
 * @author Synapse Framework Team
 * @version 2.0.0
 * @date 2025-08-12
 */

#include "synapse.h"
#include "ui_events.h"
#include "ui_menu.h"
#include "esp_timer.h"
#include "relay_interface.h"
#include "wifi_interface.h"

DEFINE_COMPONENT_TAG("UI_EVENTS", SYNAPSE_LOG_COLOR_YELLOW);

// --- Forward Declaration for the new promise callback ---
static void on_wifi_status_received(void *result_data, void *user_context);
static void on_wifi_status_failed(void *error_data, void *user_context);

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
 *          - Ensuring `synapse_event_data_release` is called on all event data wrappers.
 *
 * @param[in] self A pointer to the `ui_manager` module instance.
 * @param[in] event_name The name of the event that was triggered.
 * @param[in] event_data A pointer to the event's data wrapper.
 */
void ui_events_handle(module_t *self, const char *event_name, void *event_data)
{
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;
    bool needs_render = false;

    // --- ლოგიკა ეკრანის გაღვიძებისთვის ---
    if (strcmp(event_name, SYNAPSE_EVENT_BUTTON_PRESSED) == 0)
    {
        reset_screen_off_timer(private_data);

        // თუ ეკრანი გამორთული იყო, ჩავრთოთ და ხელახლა გავუშვათ ტაიმერები
        if (!private_data->is_screen_on)
        {
            ESP_LOGI(TAG, "Waking up screen and restarting timers.");
            private_data->display->api->set_power(private_data->display->context, true);
            private_data->is_screen_on = true;

            // ხელახლა გავუშვათ განახლების ტაიმერები
            if (private_data->timer)
            {
                if (private_data->current_state == UI_STATE_HOME)
                {
                    private_data->home_screen_timer = private_data->timer->schedule_event("UI_HOME_UPDATE", HOME_SCREEN_UPDATE_INTERVAL_MS, true);
                }
                private_data->wifi_status_timer = private_data->timer->schedule_event(WIFI_STATUS_TIMER_EVENT, WIFI_STATUS_UPDATE_INTERVAL_MS, true);
            }
            needs_render = true;
        }
        else // თუ ეკრანი უკვე ჩართული იყო, დავამუშაოთ ღილაკი
        {
            int64_t now = esp_timer_get_time();
            if ((now - private_data->last_button_press_time) < (BUTTON_COOLDOWN_MS * 1000))
            {
                if (event_data)
                    synapse_event_data_release((event_data_wrapper_t *)event_data);
                return;
            }
            private_data->last_button_press_time = now;

            if (!event_data || !((event_data_wrapper_t *)event_data)->payload)
            {
                if (event_data)
                    synapse_event_data_release((event_data_wrapper_t *)event_data);
                return;
            }
            const char *button_name = ((synapse_button_payload_t *)((event_data_wrapper_t *)event_data)->payload)->button_name;

            if (private_data->current_state == UI_STATE_HOME)
            {
                if (strcmp(button_name, "OK") == 0)
                {
                    private_data->current_state = UI_STATE_MAIN_MENU;
                    private_data->selected_item_index = 0;
                }
            }
            else
            {
                ui_menu_handle_navigation(private_data, button_name);
            }

            if (private_data->current_state == UI_STATE_MODULE_CONTROL && strcmp(button_name, "OK") == 0)
            {
                const module_t *module = private_data->selected_control_module;
                if (module)
                {
                    synapse_service_type_t type;
                    if (synapse_service_get_type(module->name, &type) == ESP_OK && type == SYNAPSE_SERVICE_TYPE_RELAY_API)
                    {
                        relay_api_t *relay_api = synapse_service_get(module->name);
                        if (relay_api)
                        {
                            relay_api->toggle((void *)module);
                            private_data->timer->schedule_event(MODULE_CONTROL_ACTION_TIMER_EVENT, 100, false);
                        }
                    }
                }
            }
            needs_render = true;
        }
    }
    // --- ეკრანის გათიშვის ლოგიკა ---
    else if (strcmp(event_name, SCREEN_OFF_TIMER_EVENT) == 0)
    {
        ESP_LOGI(TAG, "Screen off timer fired. Turning off display and stopping timers.");
        private_data->display->api->set_power(private_data->display->context, false);
        private_data->is_screen_on = false;

        // გავაჩეროთ განახლების ტაიმერები ენერგიის დასაზოგად
        if (private_data->timer)
        {
            if (private_data->home_screen_timer)
            {
                private_data->timer->cancel_event(private_data->home_screen_timer);
                private_data->home_screen_timer = NULL; // Handle-ის გასუფთავება
            }
            if (private_data->wifi_status_timer)
            {
                private_data->timer->cancel_event(private_data->wifi_status_timer);
                private_data->wifi_status_timer = NULL; // Handle-ის გასუფთავება
            }
        }
    }
    // --- სხვა ივენთების დამუშავება ---
    else if (private_data->is_screen_on) // დავამუშაოთ სხვა ივენთები მხოლოდ თუ ეკრანი ჩართულია
    {
        if (strcmp(event_name, SPLASH_SCREEN_TIMER_EVENT) == 0)
        {
            ESP_LOGI(TAG, "Splash screen timer ended. Transitioning to HOME state.");
            private_data->current_state = UI_STATE_HOME;
            ui_menu_populate_items(private_data);
            private_data->home_screen_timer = private_data->timer->schedule_event("UI_HOME_UPDATE", HOME_SCREEN_UPDATE_INTERVAL_MS, true);
            private_data->wifi_status_timer = private_data->timer->schedule_event(WIFI_STATUS_TIMER_EVENT, WIFI_STATUS_UPDATE_INTERVAL_MS, true);
            reset_screen_off_timer(private_data);
            needs_render = true;
        }
        else if (strcmp(event_name, "UI_HOME_UPDATE") == 0)
        {
            if (private_data->current_state == UI_STATE_HOME)
                needs_render = true;
        }
        else if (strcmp(event_name, MODULE_CONTROL_ACTION_TIMER_EVENT) == 0)
        {
            needs_render = true;
        }
        else if (strcmp(event_name, WIFI_STATUS_TIMER_EVENT) == 0)
        {
            ESP_LOGD(TAG, "WiFi status timer fired. Requesting status via Promise API.");
            if (private_data->wifi_service && private_data->wifi_service->get_status_async && private_data->wifi_module_handle)
            {
                private_data->wifi_service->get_status_async(
                    private_data->wifi_module_handle,
                    on_wifi_status_received,
                    on_wifi_status_failed,
                    self);
            }
            else
            {
                ESP_LOGE(TAG, "Cannot request WiFi status: wifi_service or wifi_module_handle not injected.");
            }
        }
    }

    if (needs_render && private_data->is_screen_on)
    {
        render_current_state(private_data);
    }

    if (event_data)
    {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

static void on_wifi_status_received(void *result_data, void *user_context)
{
    module_t *self = (module_t *)user_context;
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;
    char *json_string = (char *)result_data;

    if (!json_string)
        return;

    cJSON *root = cJSON_Parse(json_string);
    if (root)
    {
        const cJSON *conn_status = cJSON_GetObjectItem(root, "connection_status");
        const cJSON *rssi_json = cJSON_GetObjectItem(root, "rssi");
        bool new_status = (cJSON_IsString(conn_status) && strcmp(conn_status->valuestring, "Connected") == 0);
        int32_t new_rssi = cJSON_IsNumber(rssi_json) ? rssi_json->valueint : -100;

        if (new_status != private_data->wifi_connected || new_rssi != private_data->wifi_rssi)
        {
            ESP_LOGI(TAG, "Promise resolved: WiFi status changed. Connected: %d, RSSI: %d. Updating.", new_status, (int)new_rssi);
            private_data->wifi_connected = new_status;
            private_data->wifi_rssi = new_rssi;

            // Since we are in the promise task context, we need to tell the UI to re-render.
            // A simple way is to post a self-event.
            synapse_event_bus_post("UI_HOME_UPDATE", NULL);
        }
        cJSON_Delete(root);
    }
}

static void on_wifi_status_failed(void *error_data, void *user_context)
{
    ESP_LOGE(TAG, "Promise rejected: Failed to get WiFi status.");
    // Optionally, update UI to show an error icon
}