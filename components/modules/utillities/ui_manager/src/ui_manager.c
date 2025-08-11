/**
 * @file ui_manager.c
 * @brief Main orchestrator for the Modular UI system.
 * @author Synapse Framework Team
 * @version 6.0.0
 * @date 2025-09-12
 *
 * @details
 * This module acts as a "stage manager" for UI components. It does not
 * contain any hardcoded UI logic itself. Instead, it provides a registration
 * service (ui_manager_api_t) that other modules use to inject their own
 * UI components (screens, indicators, etc.). The UI Manager is then responsible
 * for managing the lifecycle of these components, orchestrating rendering, and
 * dispatching user input events to the currently active component.
 */
#include "synapse.h"
#include "ui_manager.h"
#include "ui_manager_internal.h"
#include "ui_interface.h"
#include <sys/queue.h>

DEFINE_COMPONENT_TAG("UI_MANAGER", SYNAPSE_LOG_COLOR_YELLOW);

// --- Forward Declarations for static functions ---
static esp_err_t ui_manager_init(module_t *self);
static esp_err_t ui_manager_start(module_t *self);
static void ui_manager_deinit(module_t *self);
static void ui_manager_handle_event(module_t *self, const char *event_name, void *event_data);
static void ui_task(void *pvParameters);

/**
 * @internal
 * @brief Service API implementation for registering UI components.
 * @details This function is part of the service API table and is called by
 *          other modules to add their UI components to the UI Manager's registry.
 * @see ui_manager_api_t
 */
static esp_err_t register_components_impl(module_t *owner, const ui_component_t **components);

/**
 * @internal
 * @brief Service API implementation for unregistering UI components.
 * @details This function is part of the service API table and is called by
 *          other modules during their deinitialization to cleanly remove
 *          their UI components.
 * @see ui_manager_api_t
 */
static esp_err_t unregister_components_impl(module_t *owner);

// --- Service API Table ---
static ui_manager_api_t s_ui_manager_api = {
    .register_components = register_components_impl,
    .unregister_components = unregister_components_impl,
};

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
    SYNAPSE_GUARD(module && private_data, TAG, NULL, "Failed to allocate memory");

    module->private_data = private_data;
    module->current_config = cJSON_Duplicate(config, true);
    SYNAPSE_GUARD(module->current_config, TAG, NULL, "Failed to duplicate config");

    module->dependency_map = s_dependencies;
    private_data->module = module;
    module->init_level = 80;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    synapse_config_get_string_from_node(TAG, config_node, "instance_name", module->name, sizeof(module->name));

    module->base.init = ui_manager_init;
    module->base.start = ui_manager_start;
    module->base.deinit = ui_manager_deinit;
    module->base.handle_event = ui_manager_handle_event;

    // Initialize the linked list for the component registry
    SLIST_INIT(&private_data->component_registry_head);
    private_data->registry_mutex = xSemaphoreCreateMutex();
    SYNAPSE_GUARD(private_data->registry_mutex, TAG, NULL, "Failed to create registry mutex");

    // Register the UI Manager's own service
    esp_err_t ret = synapse_service_register_with_status(
        module->name, SYNAPSE_SERVICE_TYPE_UI_MANAGER_API, &s_ui_manager_api, SERVICE_STATUS_REGISTERED);
    if (ret != ESP_OK)
    {
        ui_manager_deinit(module); // Cleanup on failure
        return NULL;
    }

    return module;
}

static esp_err_t ui_manager_init(module_t *self)
{
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;

    private_data->ui_cmd_queue = xQueueCreate(10, sizeof(ui_cmd_t));
    SYNAPSE_GUARD(private_data->ui_cmd_queue, TAG, ESP_ERR_NO_MEM, "Failed to create UI command queue");

    // Dependencies are injected by System Manager, we just need to get the handles
    private_data->system_manager = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_SYSTEM_API);
    private_data->timer = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_TIMER_API);
    private_data->time_sync = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_TIME_SYNC_API);

    SYNAPSE_GUARD(private_data->display && private_data->system_manager && private_data->timer && private_data->wifi_service && private_data->wifi_module_handle,
                  TAG, ESP_ERR_INVALID_STATE, "Dependency injection failed!");

    if (!private_data->time_sync)
    {
        ESP_LOGW(TAG, "Time Sync service not found. Time will not be displayed.");
    }

    // Subscribe to events
    synapse_event_bus_subscribe(SYNAPSE_EVENT_BUTTON_PRESSED, self);
    synapse_event_bus_subscribe(SCREEN_OFF_TIMER_EVENT, self);
    synapse_event_bus_subscribe("UI_HOME_UPDATE", self);
    synapse_event_bus_subscribe(WIFI_STATUS_TIMER_EVENT, self);
    synapse_event_bus_subscribe(SPLASH_SCREEN_TIMER_EVENT, self);
    synapse_event_bus_subscribe("UI_NAVIGATE_BACK", self);

    private_data->current_state = UI_STATE_SPLASH;
    private_data->is_screen_on = true;
    private_data->wifi_connected = false;
    private_data->wifi_rssi = -100;
    private_data->last_button_press_time = 0;

    ESP_LOGI(TAG, "UI Manager initialized.");
    return ESP_OK;
}

static esp_err_t ui_manager_start(module_t *self)
{
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;

    BaseType_t ret = xTaskCreate(ui_task, "ui_task", 4096, self, 10, &private_data->ui_task_handle);
    SYNAPSE_GUARD(ret == pdPASS, TAG, ESP_FAIL, "Failed to create UI task");

    display_info_t display_info;
    private_data->display->api->get_info(private_data->display->context, &display_info);
    private_data->display_width = display_info.width;
    private_data->display_height = display_info.height;
    private_data->display->api->get_small_font_metrics(private_data->display->context, &private_data->small_font_metrics);

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
    if (!private_data)
        return;

    ESP_LOGI(TAG, "Deinitializing UI Manager '%s'...", self->name);

    // Stop all activities - kill the task first
    if (private_data->ui_task_handle)
    {
        vTaskDelete(private_data->ui_task_handle);
        private_data->ui_task_handle = NULL;
    }

    // Clean up FreeRTOS objects
    if (private_data->ui_cmd_queue)
    {
        vQueueDelete(private_data->ui_cmd_queue);
        private_data->ui_cmd_queue = NULL;
    }

    // Cancel timers
    if (private_data->timer)
    {
        if (private_data->screen_off_timer)
            private_data->timer->cancel_event(private_data->screen_off_timer);
        if (private_data->home_screen_timer)
            private_data->timer->cancel_event(private_data->home_screen_timer);
        if (private_data->wifi_status_timer)
            private_data->timer->cancel_event(private_data->wifi_status_timer);
    }

    // Unsubscribe from all events
    synapse_event_bus_unsubscribe(SYNAPSE_EVENT_BUTTON_PRESSED, self);
    synapse_event_bus_unsubscribe(SCREEN_OFF_TIMER_EVENT, self);
    synapse_event_bus_unsubscribe("UI_HOME_UPDATE", self);
    synapse_event_bus_unsubscribe(WIFI_STATUS_TIMER_EVENT, self);
    synapse_event_bus_unsubscribe(SPLASH_SCREEN_TIMER_EVENT, self);
    synapse_event_bus_unsubscribe("UI_NAVIGATE_BACK", self);

    // Clean up the internal component registry
    if (private_data->registry_mutex)
    {
        if (xSemaphoreTake(private_data->registry_mutex, pdMS_TO_TICKS(100)) == pdTRUE)
        {
            registered_component_node_t *current, *tmp;
            SLIST_FOREACH_SAFE(current, &private_data->component_registry_head, entries, tmp)
            {
                SLIST_REMOVE(&private_data->component_registry_head, current, registered_component_node_t, entries);
                free(current);
            }
            xSemaphoreGive(private_data->registry_mutex);
        }
        vSemaphoreDelete(private_data->registry_mutex);
        private_data->registry_mutex = NULL;
    }

    // Free allocated memory
    if (self->current_config)
    {
        cJSON_Delete(self->current_config);
        self->current_config = NULL;
    }
    free(private_data);
    self->private_data = NULL;

    ESP_LOGI(TAG, "UI Manager deinitialized successfully.");
}

static void ui_manager_handle_event(module_t *self, const char *event_name, void *event_data)
{
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;
    ui_cmd_t cmd = {.type = UI_CMD_PROCESS_EVENT, .event_data = event_data};
    synapse_safe_strncpy(cmd.event_name, event_name, sizeof(cmd.event_name));

    if (xQueueSend(private_data->ui_cmd_queue, &cmd, 0) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to send event '%s' to UI queue. Releasing data.", event_name);
        if (event_data)
        {
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        }
    }
}

void render_current_state(ui_manager_private_data_t *private_data)
{
    SYNAPSE_GUARD_VOID(private_data && private_data->display && private_data->is_screen_on, TAG, "Render preconditions not met");

    const display_driver_api_t *display = private_data->display->api;
    void *context = private_data->display->context;
    display->clear(context);

    switch (private_data->current_state)
    {
    case UI_STATE_SPLASH:
        render_splash_screen(private_data);
        break;
    case UI_STATE_HOME:
        render_header(private_data, "Home"); // Title for Home screen
        render_home_screen(private_data);
        break;
    case UI_STATE_MAIN_MENU:
        render_header(private_data, "Main Menu"); // Title for Main Menu
        ui_menu_render(private_data);
        break;
    case UI_STATE_COMPONENT_ACTIVE:
        if (private_data->active_component && private_data->active_component->component_data->render_cb)
        {
            // Format the module name for the header
            char formatted_title[32];
            format_module_name(private_data->active_component->owner_module->name, formatted_title, sizeof(formatted_title));
            render_header(private_data, formatted_title); // Use formatted module name as title

            ui_context_t render_ctx = {
                .display = private_data->display,
                .is_active = (private_data->selected_item_index == 0)};
            private_data->active_component->component_data->render_cb(private_data->active_component->owner_module, &render_ctx);
        }
        break;
    default:
        display->draw_formatted_text(context, 10, 30, 1, "Unknown State");
        break;
    }
    display->update_screen(context);
}

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

static void ui_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    ui_manager_private_data_t *private_data = (ui_manager_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "UI processing task started.");

    while (1)
    {
        ui_cmd_t cmd;
        if (xQueueReceive(private_data->ui_cmd_queue, &cmd, portMAX_DELAY) == pdPASS)
        {
            if (cmd.type == UI_CMD_PROCESS_EVENT)
            {
                ui_events_handle(self, cmd.event_name, cmd.event_data);
            }
        }
        // DEBUG: Log the stack high watermark
        UBaseType_t hwm = uxTaskGetStackHighWaterMark(NULL);
        ESP_LOGD(TAG, ">>>>>>>>>>>> UI Task Stack HWM: %u bytes", hwm);
    }
}

// --- Service API Implementation ---
static esp_err_t register_components_impl(module_t *owner, const ui_component_t **components)
{
    SYNAPSE_GUARD(owner && components, TAG, ESP_ERR_INVALID_ARG, "Invalid args for register_components");

    // Since this API is part of the UI Manager, we can assume there's only one instance.
    // A more robust way would be to pass the UI Manager's own module handle as context
    // during service registration, but this is a safe simplification for now.
    module_t *ui_manager_module = synapse_module_registry_find_by_name("main_ui_manager");
    SYNAPSE_GUARD(ui_manager_module, TAG, ESP_ERR_INVALID_STATE, "UI Manager module instance not found");
    ui_manager_private_data_t *p_data = (ui_manager_private_data_t *)ui_manager_module->private_data;
    if (xSemaphoreTake(p_data->registry_mutex, portMAX_DELAY) != pdTRUE)
        return ESP_ERR_TIMEOUT;

    for (int i = 0; components[i] != NULL; i++)
    {
        registered_component_node_t *new_node = calloc(1, sizeof(registered_component_node_t));
        if (!new_node)
        {
            ESP_LOGE(TAG, "Failed to allocate memory for UI component node");
            continue; // Try to register others
        }
        new_node->owner_module = owner;
        new_node->component_data = components[i];
        SLIST_INSERT_HEAD(&p_data->component_registry_head, new_node, entries);
        ESP_LOGI(TAG, "Module '%s' registered UI component: '%s'", owner->name, components[i]->id);
    }

    xSemaphoreGive(p_data->registry_mutex);
    return ESP_OK;
}

static esp_err_t unregister_components_impl(module_t *owner)
{
    SYNAPSE_GUARD(owner, TAG, ESP_ERR_INVALID_ARG, "Owner module cannot be NULL for unregister");

    ui_manager_private_data_t *p_data = (ui_manager_private_data_t *)((module_t *)synapse_module_registry_find_by_name("main_ui_manager"))->private_data;
    SYNAPSE_GUARD(p_data, TAG, ESP_ERR_INVALID_STATE, "UI Manager private data not found");

    if (xSemaphoreTake(p_data->registry_mutex, portMAX_DELAY) != pdTRUE)
        return ESP_ERR_TIMEOUT;

    registered_component_node_t *current, *tmp;
    SLIST_FOREACH_SAFE(current, &p_data->component_registry_head, entries, tmp)
    {
        if (current->owner_module == owner)
        {
            ESP_LOGI(TAG, "Unregistering UI component '%s' from module '%s'", current->component_data->id, owner->name);
            SLIST_REMOVE(&p_data->component_registry_head, current, registered_component_node_t, entries);
            free(current);
        }
    }

    xSemaphoreGive(p_data->registry_mutex);
    return ESP_OK;
}