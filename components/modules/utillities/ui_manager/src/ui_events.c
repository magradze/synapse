/**
 * @file ui_events.c
 * @brief Implements the main event handling logic for the UI Manager.
 * @author Synapse Framework Team
 * @version 3.1.0
 * @date 2025-09-12
 */
#include "synapse.h"
#include "ui_manager_internal.h"
#include "esp_timer.h"

DEFINE_COMPONENT_TAG("UI_EVENTS", SYNAPSE_LOG_COLOR_YELLOW);

// --- Promise Callback Forward Declarations ---
static void on_wifi_status_received(void *result_data, void *user_context);
static void on_wifi_status_failed(void *error_data, void *user_context);

/**
 * @brief The main event handler function for the UI Manager module.
 */
void ui_events_handle(module_t *self, const char *event_name, void *event_data)
{
    ui_manager_private_data_t *p_data = (ui_manager_private_data_t *)self->private_data;
    bool needs_render = false;

    if (strcmp(event_name, SYNAPSE_EVENT_BUTTON_PRESSED) == 0)
    {
        reset_screen_off_timer(p_data);
        if (!p_data->is_screen_on)
        {
            p_data->display->api->set_power(p_data->display->context, true);
            p_data->is_screen_on = true;

            if (p_data->timer)
            {
                if (p_data->current_state == UI_STATE_HOME)
                {
                    p_data->home_screen_timer = p_data->timer->schedule_event("UI_HOME_UPDATE", HOME_SCREEN_UPDATE_INTERVAL_MS, true);
                }
                p_data->wifi_status_timer = p_data->timer->schedule_event(WIFI_STATUS_TIMER_EVENT, WIFI_STATUS_UPDATE_INTERVAL_MS, true);
            }
            needs_render = true;
        }
        else
        {
            int64_t now = esp_timer_get_time();
            if ((now - p_data->last_button_press_time) < (BUTTON_COOLDOWN_MS * 1000))
            {
                goto cleanup;
            }
            p_data->last_button_press_time = now;

            const char *button_name = ((synapse_button_payload_t *)((event_data_wrapper_t *)event_data)->payload)->button_name;

            if (p_data->current_state == UI_STATE_HOME)
            {
                if (strcmp(button_name, "OK") == 0)
                {
                    p_data->current_state = UI_STATE_MAIN_MENU;
                    p_data->selected_item_index = 0;
                }
            }
            else
            {
                ui_menu_handle_navigation(p_data, button_name);
            }
            needs_render = true;
        }
    }
    // --- Screen Off Logic ---
    else if (strcmp(event_name, SCREEN_OFF_TIMER_EVENT) == 0)
    {
        p_data->display->api->set_power(p_data->display->context, false);
        p_data->is_screen_on = false;
        // Stop timers...
        if (p_data->timer)
        {
            if (p_data->home_screen_timer)
                p_data->timer->cancel_event(p_data->home_screen_timer);
            if (p_data->wifi_status_timer)
                p_data->timer->cancel_event(p_data->wifi_status_timer);
            p_data->home_screen_timer = NULL;
            p_data->wifi_status_timer = NULL;
        }
    }
    // --- Navigation Request from a Component ---
    else if (strcmp(event_name, "UI_NAVIGATE_BACK") == 0)
    {
        if (p_data->current_state == UI_STATE_COMPONENT_ACTIVE)
        {
            ESP_LOGI(TAG, "Received navigate back event. Returning to main menu.");
            p_data->current_state = UI_STATE_MAIN_MENU;
            p_data->active_component = NULL;
            p_data->selected_item_index = p_data->previous_menu_selection;
            needs_render = true;
        }
    }
    // --- Other events only if screen is on ---
    else if (p_data->is_screen_on)
    {
        if (strcmp(event_name, SPLASH_SCREEN_TIMER_EVENT) == 0)
        {
            p_data->current_state = UI_STATE_HOME;
            if (p_data->timer)
            {
                p_data->home_screen_timer = p_data->timer->schedule_event("UI_HOME_UPDATE", HOME_SCREEN_UPDATE_INTERVAL_MS, true);
                p_data->wifi_status_timer = p_data->timer->schedule_event(WIFI_STATUS_TIMER_EVENT, WIFI_STATUS_UPDATE_INTERVAL_MS, true);
            }
            reset_screen_off_timer(p_data);
            needs_render = true;
        }
        else if (strcmp(event_name, "UI_HOME_UPDATE") == 0)
        {
            needs_render = true;
        }
        else if (strcmp(event_name, WIFI_STATUS_TIMER_EVENT) == 0)
        {
            if (p_data->wifi_service)
            {
                p_data->wifi_service->get_status_async(p_data->wifi_module_handle, on_wifi_status_received, on_wifi_status_failed, self);
            }
        }
    }

    if (needs_render && p_data->is_screen_on)
    {
        render_current_state(p_data);
    }

cleanup:
    if (event_data)
    {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

static void on_wifi_status_received(void *result_data, void *user_context)
{
    module_t *self = (module_t *)user_context;
    ui_manager_private_data_t *p_data = (ui_manager_private_data_t *)self->private_data;
    char *json_string = (char *)result_data;

    SYNAPSE_GUARD_VOID(json_string, TAG, "Promise resolved with NULL data.");

    cJSON *root = cJSON_Parse(json_string);
    SYNAPSE_GUARD_VOID(root, TAG, "Failed to parse WiFi status JSON.");

    const cJSON *conn_status_item = cJSON_GetObjectItem(root, "connection_status");
    const cJSON *rssi_item = cJSON_GetObjectItem(root, "rssi");

    bool new_status = (cJSON_IsString(conn_status_item) && strcmp(conn_status_item->valuestring, "Connected") == 0);
    int32_t new_rssi = cJSON_IsNumber(rssi_item) ? rssi_item->valueint : -100;

    if (new_status != p_data->wifi_connected || new_rssi != p_data->wifi_rssi)
    {
        ESP_LOGI(TAG, "WiFi status changed. Connected: %d -> %d, RSSI: %d -> %d. Requesting UI update.",
                 p_data->wifi_connected, new_status, (int)p_data->wifi_rssi, (int)new_rssi);

        p_data->wifi_connected = new_status;
        p_data->wifi_rssi = new_rssi;

        if (p_data->is_screen_on)
        {
            synapse_event_bus_post("UI_HOME_UPDATE", NULL);
        }
    }

    cJSON_Delete(root);
}

/**
 * @brief Callback for a rejected WiFi status promise.
 */
static void on_wifi_status_failed(void *error_data, void *user_context)
{
    ESP_LOGE(TAG, "Promise rejected: Failed to get WiFi status.");
}