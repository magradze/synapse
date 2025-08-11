/**
 * @file relay_actuator_ui.c
 * @brief Implements the UI components for the Relay Actuator module.
 */
#include "synapse.h"
#include "relay_actuator_internal.h"
#include "ui_interface.h"

DEFINE_COMPONENT_TAG("RELAY_UI", SYNAPSE_LOG_COLOR_GREEN);

// --- Forward Declarations for Callbacks ---
static void relay_render_screen_cb(module_t* self, ui_context_t* context);
static void relay_event_cb(module_t* self, ui_event_t* event);

// --- UI Component Definition ---
static const ui_component_t s_relay_control_screen = {
    .id = "relay_control_screen",
    .type = UI_COMP_SCREEN,
    .menu_text = "Relay Control",
    .render_cb = relay_render_screen_cb,
    .event_cb = relay_event_cb,
};

static const ui_component_t* s_relay_ui_components[] = {
    &s_relay_control_screen,
    NULL
};

// --- UI Lifecycle Functions ---
void relay_ui_init(module_t* self)
{
    ui_manager_api_t* ui_api = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_UI_MANAGER_API);
    if (ui_api) {
        ui_api->register_components(self, s_relay_ui_components);
    } else {
        ESP_LOGE(TAG, "UI Manager service not found for module '%s'", self->name);
    }
}

void relay_ui_deinit(module_t* self)
{
    ui_manager_api_t* ui_api = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_UI_MANAGER_API);
    if (ui_api) {
        ui_api->unregister_components(self);
    }
}

// --- Local UI Helper ---
// This is a simplified, local version of the footer button renderer.
// In a more advanced system, this could be part of a shared UI library or the UI Manager's API.
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

static void relay_render_screen_cb(module_t* self, ui_context_t* context)
{
    relay_private_data_t* p_data = (relay_private_data_t*)self->private_data;
    const display_driver_api_t* display = context->display->api;
    void* disp_ctx = context->display->context;

    // The header is now drawn by the UI Manager.
    // We only need to draw the content specific to this screen.
    
    // Display Current State
    display->draw_formatted_text(disp_ctx, 2, 20, 1, "State: %s", p_data->current_state ? "ON" : "OFF");

    // Display Action Button
    const char* action_text = p_data->current_state ? "Turn OFF" : "Turn ON";
    
    bool is_action_selected = (context->is_active);

    if (is_action_selected) {
         display->draw_formatted_text(disp_ctx, 2, 32, 1, "> %s", action_text);
    } else {
         display->draw_formatted_text(disp_ctx, 10, 32, 1, "%s", action_text);
    }

    local_render_footer_button(context, "BACK", !is_action_selected);
}

static void relay_event_cb(module_t* self, ui_event_t* event)
{
    // The logic is now simpler. UI Manager decides if we get the event.
    // If we get an "OK", it means our action was selected.
    if (strcmp(event->button_name, "OK") == 0) {
        ESP_LOGI(TAG, "UI Toggle action received for relay '%s'", self->name);
        set_relay_state(self, !((relay_private_data_t*)self->private_data)->current_state);
    }
    // We no longer need to handle the "BACK" event here.
}