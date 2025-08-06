/**
 * @file ui_manager.c
 * @brief Main orchestrator for the UI system.
 * @author Synapse Framework Team
 * @version 4.2.0
 * @date 2025-08-13
 */

#include "synapse.h"
#include "ui_manager.h"
#include "ui_events.h"
#include "ui_menu.h"
#include "ui_screens.h"

DEFINE_COMPONENT_TAG("UI_MANAGER");

// --- Forward Declarations for static lifecycle functions ---
static esp_err_t ui_manager_init(module_t *self);
static esp_err_t ui_manager_start(module_t *self);
static void ui_manager_deinit(module_t *self);
static void ui_manager_handle_event(module_t *self, const char *event_name, void *event_data);

/**
 * @brief Creates a new instance of the UI Manager module.
 * @details This is the factory function for the module, called by the Module Factory.
 *          It allocates memory for the module and its private data, takes ownership
 *          of the provided config object, parses the instance name, and sets up
 *          the module's base lifecycle function pointers.
 * @param[in] config A pointer to the cJSON configuration object for this instance.
 *                   The function takes ownership of this object.
 * @return A pointer to the newly created module_t structure, or NULL on failure.
 */
module_t *ui_manager_create(const cJSON *config) {
    module_t *module = calloc(1, sizeof(module_t));
    ui_manager_private_data_t *private_data = calloc(1, sizeof(ui_manager_private_data_t));
    if (!module || !private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module); free(private_data);
        if (config) cJSON_Delete((cJSON*)config);
        return NULL;
    }
    module->private_data = private_data;
    module->current_config = (cJSON*)config;
    private_data->module = module;
    module->init_level = 80;
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    snprintf(module->name, sizeof(module->name), "%s", cJSON_GetObjectItem(config_node, "instance_name")->valuestring);
    
    module->base.init = ui_manager_init;
    module->base.start = ui_manager_start;
    module->base.deinit = ui_manager_deinit;
    module->base.handle_event = ui_manager_handle_event;
    
    return module;
}

/**
 * @brief Deinitializes the UI Manager module instance.
 * @details This function is responsible for cleaning up all resources allocated
 *          by the module. It cancels any active timers, unsubscribes from all
 *          Event Bus events, and frees all allocated memory, including the
 *          private data structure and the configuration object.
 * @param[in] self A pointer to the module instance to deinitialize.
 */
static void ui_manager_deinit(module_t *self) {
    if (!self) return;
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;
    if (private_data->timer) {
        if (private_data->screen_off_timer) private_data->timer->cancel_event(private_data->screen_off_timer);
        if (private_data->home_screen_timer) private_data->timer->cancel_event(private_data->home_screen_timer);
        if (private_data->wifi_status_timer) private_data->timer->cancel_event(private_data->wifi_status_timer);
    }
    fmw_event_bus_unsubscribe(FMW_EVENT_BUTTON_PRESSED, self);
    fmw_event_bus_unsubscribe(FMW_EVENT_WIFI_STATUS_READY, self);
    fmw_event_bus_unsubscribe(SCREEN_OFF_TIMER_EVENT, self);
    fmw_event_bus_unsubscribe("UI_HOME_UPDATE", self);
    fmw_event_bus_unsubscribe(WIFI_STATUS_TIMER_EVENT, self);
    fmw_event_bus_unsubscribe(SPLASH_SCREEN_TIMER_EVENT, self);
    if (self->current_config) cJSON_Delete(self->current_config);
    free(private_data);
    free(self);
    ESP_LOGI(TAG, "UI Manager deinitialized.");
}

/**
 * @brief Initializes the UI Manager module instance.
 * @details This function is called by the System Manager after the module is created.
 *          It acquires handles to all required services (display, system manager, timer, etc.)
 *          from the Service Locator. It also subscribes to all necessary system events
 *          and sets the initial state of the UI to `UI_STATE_SPLASH`.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK on success, or an error code if required services are not found.
 */
static esp_err_t ui_manager_init(module_t *self) {
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;
    const cJSON *config_node = cJSON_GetObjectItem(self->current_config, "config");
    const char* driver_service_name = cJSON_GetObjectItem(config_node, "display_driver_service")->valuestring;

    private_data->display = fmw_service_get(driver_service_name);
    private_data->system_manager = fmw_service_lookup_by_type(FMW_SERVICE_TYPE_SYSTEM_API);
    private_data->timer = fmw_service_lookup_by_type(FMW_SERVICE_TYPE_TIMER_API);
    private_data->time_sync = fmw_service_lookup_by_type(FMW_SERVICE_TYPE_TIME_SYNC_API);

    if (!private_data->display || !private_data->system_manager || !private_data->timer) {
        ESP_LOGE(TAG, "Failed to acquire required services!");
        return ESP_ERR_NOT_FOUND;
    }
    if (!private_data->time_sync) {
        ESP_LOGW(TAG, "Time Sync service not found. Time will not be displayed.");
    }

    fmw_event_bus_subscribe(FMW_EVENT_BUTTON_PRESSED, self);
    fmw_event_bus_subscribe(FMW_EVENT_WIFI_STATUS_READY, self);
    fmw_event_bus_subscribe(SCREEN_OFF_TIMER_EVENT, self);
    fmw_event_bus_subscribe("UI_HOME_UPDATE", self);
    fmw_event_bus_subscribe(WIFI_STATUS_TIMER_EVENT, self);
    fmw_event_bus_subscribe(SPLASH_SCREEN_TIMER_EVENT, self);

    private_data->current_state = UI_STATE_SPLASH;
    private_data->is_screen_on = true;
    private_data->wifi_connected = false;
    private_data->wifi_rssi = -100;
    private_data->last_button_press_time = 0;

    ESP_LOGI(TAG, "UI Manager initialized, will use display driver '%s'", driver_service_name);
    return ESP_OK;
}

/**
 * @brief Starts the UI Manager's operation.
 * @details Called by the System Manager after all modules have been initialized.
 *          This function retrieves the display's physical dimensions and font metrics,
 *          schedules the initial timer for the splash screen, and performs the first
 *          render of the UI.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK on success.
 */
static esp_err_t ui_manager_start(module_t *self) {
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;
    
    display_info_t display_info;
    if (private_data->display->api->get_info(private_data->display->context, &display_info) == ESP_OK) {
        private_data->display_width = display_info.width;
        private_data->display_height = display_info.height;
    } else {
        private_data->display_width = 128;
        private_data->display_height = 64;
    }

    if (private_data->display->api->get_small_font_metrics(private_data->display->context, &private_data->small_font_metrics) != ESP_OK) {
        private_data->small_font_metrics.width = 6;
        private_data->small_font_metrics.height = 8;
    }

    private_data->timer->schedule_event(SPLASH_SCREEN_TIMER_EVENT, 3000, false);
    render_current_state(private_data);
    
    ESP_LOGI(TAG, "UI Manager started, showing splash screen.");
    return ESP_OK;
}

/**
 * @brief The main event handler function for the module.
 * @details This function is registered as the module's `handle_event` callback and
 *          is called by the Event Bus. It acts as a high-level dispatcher, delegating
 *          all event processing to the specialized `ui_events_handle` function.
 * @param[in] self A pointer to the module instance.
 * @param[in] event_name The name of the event that was triggered.
 * @param[in] event_data A pointer to the event's data wrapper.
 */
static void ui_manager_handle_event(module_t *self, const char *event_name, void *event_data) {
    ui_events_handle(self, event_name, event_data);
}

/**
 * @brief The main rendering orchestrator function.
 * @details This function is called whenever the UI needs to be redrawn. It checks
 *          the current UI state and calls the appropriate rendering function for
 *          that specific screen (e.g., `render_splash_screen`, `render_home_screen`,
 *          or `ui_menu_render`). It is responsible for clearing the screen before
 *          drawing and pushing the final buffer to the display.
 * @param[in] private_data A pointer to the module's private data structure.
 */
void render_current_state(ui_manager_private_data_t *private_data) {
    if (!private_data || !private_data->display || !private_data->is_screen_on) return;
    const display_driver_api_t *display = private_data->display->api;
    void *context = private_data->display->context;
    display->clear(context);
    
    switch (private_data->current_state) {
        case UI_STATE_SPLASH:
            render_splash_screen(private_data);
            break;
        case UI_STATE_HOME:
            render_header(private_data);
            render_home_screen(private_data);
            break;
        case UI_STATE_MAIN_MENU:
        case UI_STATE_MODULES_MENU:
        case UI_STATE_SETTINGS_MENU:
            render_header(private_data);
            ui_menu_render(private_data);
            break;
        case UI_STATE_MODULE_CONTROL:
            render_header(private_data);
            render_module_control_screen(private_data);
            break;
        default:
            display->draw_formatted_text(context, 10, 30, 1, "Unknown State");
            break;
    }
    display->update_screen(context);
}

/**
 * @brief Resets the screen's auto-off timer.
 * @details This function cancels any existing screen-off timer and schedules a new
 *          one. It should be called whenever there is user activity (like a button
 *          press) to prevent the screen from turning off prematurely.
 * @param[in] private_data A pointer to the module's private data structure.
 */
void reset_screen_off_timer(ui_manager_private_data_t *private_data) {
    if (private_data->timer) {
        if (private_data->screen_off_timer) {
            private_data->timer->cancel_event(private_data->screen_off_timer);
        }
        private_data->screen_off_timer = private_data->timer->schedule_event(SCREEN_OFF_TIMER_EVENT, 30000, false);
    }
}