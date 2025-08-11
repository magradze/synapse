/**
 * @file wifi_manager_ui.c
 * @brief Implements the UI components for the WiFi Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-09-12
 */
#include "synapse.h"
#include "wifi_manager_internal.h"
#include "ui_interface.h"

DEFINE_COMPONENT_TAG("WIFI_UI", SYNAPSE_LOG_COLOR_CYAN);

// --- Forward Declarations for Callbacks ---
static void wifi_render_screen_cb(module_t* self, ui_context_t* context);
static void wifi_event_cb(module_t* self, ui_event_t* event);

static void local_render_footer_button(ui_context_t* context, const char* text, bool is_selected);

// --- UI Component Definition ---
static const ui_component_t s_wifi_status_screen = {
    .id = "wifi_status_screen",
    .type = UI_COMP_SCREEN,
    .menu_text = "WiFi Status", // This text will appear in the main menu
    .render_cb = wifi_render_screen_cb,
    .event_cb = wifi_event_cb,
};

static const ui_component_t* s_wifi_ui_components[] = {
    &s_wifi_status_screen,
    NULL // Terminator
};

// --- UI Lifecycle Functions ---
void wifi_ui_init(module_t* self)
{
    ui_manager_api_t* ui_api = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_UI_MANAGER_API);
    if (ui_api) {
        ui_api->register_components(self, s_wifi_ui_components);
    } else {
        ESP_LOGE(TAG, "UI Manager service not found for module '%s'", self->name);
    }
}

void wifi_ui_deinit(module_t* self)
{
    ui_manager_api_t* ui_api = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_UI_MANAGER_API);
    if (ui_api) {
        ui_api->unregister_components(self);
    }
}

// --- Callback Implementations ---

static void wifi_render_screen_cb(module_t* self, ui_context_t* context)
{
    wifi_manager_private_data_t* p_data = (wifi_manager_private_data_t*)self->private_data;
    const display_driver_api_t* display = context->display->api;
    void* disp_ctx = context->display->context;

    // Display Connection Status
    display->draw_formatted_text(disp_ctx, 2, 16, 1, "Status: %s",
                                 p_data->is_connected ? "Connected" : "Disconnected");

    // Display SSID if connected
    if (p_data->is_connected) {
        display->draw_formatted_text(disp_ctx, 2, 28, 1, "SSID: %s", (char*)p_data->wifi_config.sta.ssid);
    }

    // Display IP Address if connected
    esp_netif_ip_info_t ip_info;
    esp_netif_t* netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (p_data->is_connected && netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
        display->draw_formatted_text(disp_ctx, 2, 40, 1, "IP: " IPSTR, IP2STR(&ip_info.ip));
    } else {
        display->draw_formatted_text(disp_ctx, 2, 40, 1, "IP: N/A");
    }

    // The BACK button is always the only "action" on this screen
    local_render_footer_button(context, "BACK", true);
}

static void wifi_event_cb(module_t* self, ui_event_t* event)
{
    // This screen is informational, so "OK" does nothing.
    // "BACK" is the only relevant action.
    if (strcmp(event->button_name, "BACK") == 0 || strcmp(event->button_name, "OK") == 0) {
        synapse_event_bus_post("UI_NAVIGATE_BACK", NULL);
    }
}

static void local_render_footer_button(ui_context_t* context, const char* text, bool is_selected)
{
    const display_driver_api_t *display = context->display->api;
    void *disp_ctx = context->display->context;

    display_info_t info;
    display->get_info(disp_ctx, &info);
    
    font_metrics_t font;
    display->get_small_font_metrics(disp_ctx, &font);

    int padding_x = 4;
    int padding_y = 2;
    int margin = 2;

    int text_width = strlen(text) * font.width;
    int button_width = text_width + (padding_x * 2);
    int button_height = font.height + (padding_y * 2);

    int button_x = info.width - button_width - margin;
    int button_y = info.height - button_height - margin;

    uint32_t bg_color = is_selected ? 1 : 0;
    uint32_t text_color = is_selected ? 0 : 1;

    display->fill_rect(disp_ctx, button_x, button_y, button_width, button_height, bg_color);
    display->draw_formatted_text(disp_ctx, button_x + padding_x, button_y + padding_y, text_color, text);
}