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

DEFINE_COMPONENT_TAG("UI_MANAGER", SYNAPSE_LOG_COLOR_YELLOW);

// --- Forward Declarations for static lifecycle functions ---
static esp_err_t ui_manager_init(module_t *self);
static esp_err_t ui_manager_start(module_t *self);
static void ui_manager_deinit(module_t *self);
static void ui_manager_handle_event(module_t *self, const char *event_name, void *event_data);
static void ui_task(void *pvParameters);

// --- Dependency Map ---
static const module_dependency_t s_dependencies[] = {
    {"display_driver_service", offsetof(ui_manager_private_data_t, display)},
    {"wifi_service", offsetof(ui_manager_private_data_t, wifi_service)},
    {"wifi_module_handle", offsetof(ui_manager_private_data_t, wifi_module_handle)},
    {NULL, 0} // Terminator
};

module_t *ui_manager_create(const cJSON *config)
{
    module_t *module = calloc(1, sizeof(module_t));
    ui_manager_private_data_t *private_data = calloc(1, sizeof(ui_manager_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        if (config)
            cJSON_Delete((cJSON *)config);
        return NULL;
    }
    module->private_data = private_data;
    module->current_config = cJSON_Duplicate(config, true);
    if (!module->current_config)
    {
        ESP_LOGE(TAG, "Failed to duplicate configuration object.");
        // Note: This assumes 'private_data' and 'module' are allocated.
        // Manual check might be needed for each file's cleanup logic.
        free(private_data);
        free(module);
        return NULL;
    }
    module->dependency_map = s_dependencies;
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

static esp_err_t ui_manager_init(module_t *self)
{
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;

    private_data->ui_cmd_queue = xQueueCreate(10, sizeof(ui_cmd_t));
    if (!private_data->ui_cmd_queue)
    {
        ESP_LOGE(TAG, "Failed to create UI command queue");
        return ESP_ERR_NO_MEM;
    }

    const cJSON *config_node = cJSON_GetObjectItem(self->current_config, "config");
    const char *driver_service_name = cJSON_GetObjectItem(config_node, "display_driver_service")->valuestring;

    private_data->display = synapse_service_get(driver_service_name);
    private_data->system_manager = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_SYSTEM_API);
    private_data->timer = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_TIMER_API);
    private_data->time_sync = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_TIME_SYNC_API);

    // Dependency Injection-ის ვალიდაცია
    if (!private_data->display || !private_data->system_manager || !private_data->timer || !private_data->wifi_service || !private_data->wifi_module_handle)
    {
        ESP_LOGE(TAG, "Dependency injection failed! Check config and module init levels.");
        return ESP_ERR_INVALID_STATE;
    }
    if (!private_data->time_sync)
    {
        ESP_LOGW(TAG, "Time Sync service not found. Time will not be displayed.");
    }

    synapse_event_bus_subscribe(SYNAPSE_EVENT_BUTTON_PRESSED, self);
    synapse_event_bus_subscribe(SYNAPSE_EVENT_WIFI_STATUS_READY, self);
    synapse_event_bus_subscribe(SCREEN_OFF_TIMER_EVENT, self);
    synapse_event_bus_subscribe("UI_HOME_UPDATE", self);
    synapse_event_bus_subscribe(WIFI_STATUS_TIMER_EVENT, self);
    synapse_event_bus_subscribe(SPLASH_SCREEN_TIMER_EVENT, self);

    private_data->current_state = UI_STATE_SPLASH;
    private_data->is_screen_on = true;
    private_data->wifi_connected = false;
    private_data->wifi_rssi = -100;
    private_data->last_button_press_time = 0;

    ESP_LOGI(TAG, "UI Manager initialized, will use display driver '%s'", driver_service_name);
    return ESP_OK;
}

static esp_err_t ui_manager_start(module_t *self)
{
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;

    BaseType_t ret = xTaskCreate(ui_task, "ui_task", 4096, self, 10, &private_data->ui_task_handle);
    if (ret != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to create UI task");
        return ESP_FAIL;
    }

    display_info_t display_info;
    if (private_data->display->api->get_info(private_data->display->context, &display_info) == ESP_OK)
    {
        private_data->display_width = display_info.width;
        private_data->display_height = display_info.height;
    }
    else
    {
        private_data->display_width = 128;
        private_data->display_height = 64;
    }

    if (private_data->display->api->get_small_font_metrics(private_data->display->context, &private_data->small_font_metrics) != ESP_OK)
    {
        private_data->small_font_metrics.width = 6;
        private_data->small_font_metrics.height = 8;
    }

    private_data->timer->schedule_event(SPLASH_SCREEN_TIMER_EVENT, 3000, false);
    render_current_state(private_data);

    ESP_LOGI(TAG, "UI Manager started, showing splash screen.");
    return ESP_OK;
}

static void ui_manager_deinit(module_t *self)
{
    if (!self)
        return;
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;

    if (private_data->ui_task_handle)
    {
        vTaskDelete(private_data->ui_task_handle);
        private_data->ui_task_handle = NULL;
    }
    if (private_data->ui_cmd_queue)
    {
        vQueueDelete(private_data->ui_cmd_queue);
        private_data->ui_cmd_queue = NULL;
    }

    if (private_data->timer)
    {
        if (private_data->screen_off_timer)
            private_data->timer->cancel_event(private_data->screen_off_timer);
        if (private_data->home_screen_timer)
            private_data->timer->cancel_event(private_data->home_screen_timer);
        if (private_data->wifi_status_timer)
            private_data->timer->cancel_event(private_data->wifi_status_timer);
    }
    synapse_event_bus_unsubscribe(SYNAPSE_EVENT_BUTTON_PRESSED, self);
    synapse_event_bus_unsubscribe(SYNAPSE_EVENT_WIFI_STATUS_READY, self);
    synapse_event_bus_unsubscribe(SCREEN_OFF_TIMER_EVENT, self);
    synapse_event_bus_unsubscribe("UI_HOME_UPDATE", self);
    synapse_event_bus_unsubscribe(WIFI_STATUS_TIMER_EVENT, self);
    synapse_event_bus_unsubscribe(SPLASH_SCREEN_TIMER_EVENT, self);
    if (self->current_config)
        cJSON_Delete(self->current_config);
    free(private_data);

    ESP_LOGI(TAG, "UI Manager deinitialized.");
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
static void ui_manager_handle_event(module_t *self, const char *event_name, void *event_data)
{
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;

    ui_cmd_t cmd = {
        .type = UI_CMD_PROCESS_EVENT,
        .event_data = event_data,
    };
    // Safely copy the event name
    strncpy(cmd.event_name, event_name, sizeof(cmd.event_name) - 1);
    cmd.event_name[sizeof(cmd.event_name) - 1] = '\0';

    // Send the command to the UI task's queue.
    // Use a timeout of 0 to avoid blocking the caller (e.g., the Event Bus task).
    if (xQueueSend(private_data->ui_cmd_queue, &cmd, (TickType_t)0) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to send event '%s' to UI queue. Queue is full. Releasing data.", event_name);
        // If we can't queue the event, we are now responsible for releasing the data.
        if (event_data)
        {
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        }
    }
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
void render_current_state(ui_manager_private_data_t *private_data)
{
    if (!private_data || !private_data->display || !private_data->is_screen_on)
        return;
    const display_driver_api_t *display = private_data->display->api;
    void *context = private_data->display->context;
    display->clear(context);

    switch (private_data->current_state)
    {
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
void reset_screen_off_timer(ui_manager_private_data_t *private_data)
{
    if (private_data->timer)
    {
        if (private_data->screen_off_timer)
        {
            private_data->timer->cancel_event(private_data->screen_off_timer);
        }
        private_data->screen_off_timer = private_data->timer->schedule_event(SCREEN_OFF_TIMER_EVENT, 30000, false);
    }
}

/**
 * @internal
 * @brief The dedicated FreeRTOS task for the UI Manager.
 * @details This task waits for commands on its queue and processes them.
 *          All heavy operations, such as menu population and screen rendering,
 *          are executed in the context of this task, which has its own,
 *          sufficiently large stack. This prevents stack overflows in other
 *          system tasks like Tmr Svc.
 * @param[in] pvParameters A pointer to the `ui_manager` module instance.
 */
static void ui_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;

    ESP_LOGI(TAG, "UI processing task started.");

    while (1)
    {
        ui_cmd_t received_cmd;
        // Wait indefinitely for a command to arrive.
        if (xQueueReceive(private_data->ui_cmd_queue, &received_cmd, portMAX_DELAY) == pdPASS)
        {

            if (received_cmd.type == UI_CMD_PROCESS_EVENT)
            {
                // Call the original event handler, but now safely in our own task context.
                // The event handler is responsible for releasing the event_data.
                ui_events_handle(self, received_cmd.event_name, received_cmd.event_data);
            }
            // Other command types can be handled here in the future.
        }
    }
}