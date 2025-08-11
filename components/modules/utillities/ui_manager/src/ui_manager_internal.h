/**
 * @file ui_manager_internal.h
 * @brief Internal shared definitions for the UI Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-09-12
 *
 * @details
 * This private header file is shared ONLY among the .c files within the
 * ui_manager module. It defines the internal data structures, such as the
 * component registry node and the extended private_data struct, ensuring
 * that all parts of the module have a consistent view of the internal state.
 * It MUST NOT be included by any file outside of this module.
 */
#ifndef UI_MANAGER_INTERNAL_H
#define UI_MANAGER_INTERNAL_H

#include "ui_manager.h"
#include "ui_interface.h"
#include <sys/queue.h>

// --- Include all necessary interface headers ---
#include "display_driver_interface.h"
#include "system_manager_interface.h"
#include "timer_interface.h"
#include "time_sync_interface.h"
#include "wifi_interface.h"

// --- Internal Constants and Event Names ---

/** @brief Timeout in milliseconds for turning off the screen after inactivity. */
#define SCREEN_OFF_TIMEOUT_MS 30000
/** @brief Interval in milliseconds for updating the home screen (e.g., the clock). */
#define HOME_SCREEN_UPDATE_INTERVAL_MS 1000
/** @brief Interval in milliseconds for periodically requesting WiFi status. */
#define WIFI_STATUS_UPDATE_INTERVAL_MS 5000
/** @brief Cooldown period in milliseconds to prevent button press bouncing. */
#define BUTTON_COOLDOWN_MS 200
/** @brief Maximum number of items in the dynamic module menu, linked to a Kconfig value. */
#define MAX_MENU_ITEMS CONFIG_SYNAPSE_MAX_MODULES
/** @brief Maximum number of menu items that can be displayed on the screen at once. */
#define MAX_VISIBLE_MENU_ITEMS 3

/** @brief Internal event name for the screen-off timer callback. */
#define SCREEN_OFF_TIMER_EVENT "UI_SCREEN_OFF_TIMER"
/** @brief Internal event name for the splash screen timer callback. */
#define SPLASH_SCREEN_TIMER_EVENT "UI_SPLASH_END"
/** @brief Internal event name for the periodic WiFi status request timer. */
#define WIFI_STATUS_TIMER_EVENT "UI_WIFI_STATUS_TIMER"
/** @brief Internal event name for a short delay after a module action. */
#define MODULE_CONTROL_ACTION_TIMER_EVENT "UI_MODULE_ACTION_TIMER"
/** @brief Internal event name posted by a component to request navigation back to the menu. */
#define UI_NAVIGATE_BACK "UI_NAVIGATE_BACK" 

/**
 * @internal
 * @struct registered_component_node_t
 * @brief A node in the linked list for the UI component registry.
 * @details Each node represents a single UI component that has been registered
 *          by a module.
 */
typedef struct registered_component_node_t {
    module_t* owner_module;                 /**< @brief Pointer to the module that owns/registered this component. */
    const ui_component_t* component_data;   /**< @brief Pointer to the component's static definition (the "contract"). */
    SLIST_ENTRY(registered_component_node_t) entries; /**< @brief SLIST macro for linking nodes. */
} registered_component_node_t;

/**
 * @internal
 * @enum ui_state_t
 * @brief Defines the internal states (screens) of the UI Manager.
 */
typedef enum {
    UI_STATE_SPLASH,           /**< @brief The initial splash screen. */
    UI_STATE_HOME,             /**< @brief The main home screen. */
    UI_STATE_MAIN_MENU,        /**< @brief The dynamic main menu listing registered components. */
    UI_STATE_COMPONENT_ACTIVE, /**< @brief A generic state when a registered component's screen is active. */
} ui_state_t;

/**
 * @internal
 * @enum ui_cmd_type_t
 * @brief Defines command types for the internal UI task queue.
 */
typedef enum {
    UI_CMD_PROCESS_EVENT, /**< @brief Command to process an event from the Event Bus. */
} ui_cmd_type_t;

/**
 * @internal
 * @struct ui_cmd_t
 * @brief Structure for a command sent to the UI task's queue.
 * @details This decouples event handling from the caller's context (e.g., Event Bus task)
 *          and allows processing in the dedicated UI task.
 */
typedef struct {
    ui_cmd_type_t type;                      /**< @brief The type of the command. */
    char event_name[32];                     /**< @brief Buffer to store the event name string. */
    struct event_data_wrapper_t *event_data; /**< @brief Pointer to the event data wrapper. */
} ui_cmd_t;

/**
 * @internal
 * @struct ui_manager_private_data_t
 * @brief The complete internal state of the UI Manager module.
 */
struct ui_manager_private_data_t
{
    // --- Core & Injected Dependencies ---
    module_t *module;                       /**< @brief Back-pointer to the parent module structure. */
    display_driver_handle_t *display;       /**< @brief Injected: Handle to the active display driver service. */
    system_manager_api_t *system_manager;   /**< @brief Injected: Handle to the system manager service. */
    timer_api_t *timer;                     /**< @brief Injected: Handle to the system timer service. */
    time_sync_api_t *time_sync;             /**< @brief Injected: Handle to the time synchronization service. */
    wifi_api_t *wifi_service;               /**< @brief Injected: Handle to the WiFi service API. */
    module_t *wifi_module_handle;           /**< @brief Injected: Handle to the WiFi module instance for context. */

    // --- Display Properties ---
    uint16_t display_width;                 /**< @brief Cached width of the display in pixels. */
    uint16_t display_height;                /**< @brief Cached height of the display in pixels. */
    font_metrics_t small_font_metrics;      /**< @brief Cached metrics of the driver's default small font. */

    // --- UI State & Timers ---
    ui_state_t current_state;               /**< @brief The current screen being displayed. */
    synapse_timer_handle_t screen_off_timer;/**< @brief Handle for the screen auto-off timer. */
    synapse_timer_handle_t home_screen_timer;/**< @brief Handle for the periodic home screen update timer. */
    synapse_timer_handle_t wifi_status_timer;/**< @brief Handle for the periodic WiFi status request timer. */
    bool is_screen_on;                      /**< @brief Tracks if the display panel is currently powered on. */

    // --- Menu & Navigation State ---
    int selected_item_index;                /**< @brief The index of the currently selected item in a menu. */
    int top_visible_item;                   /**< @brief The index of the topmost visible item for scrolling menus. */
    int previous_menu_selection;            /**< @brief The index of the previously selected item in a menu. */

    // --- Component Registry ---
    SLIST_HEAD(component_registry_head_t, registered_component_node_t) component_registry_head; /**< @brief Head of the linked list for registered UI components. */
    SemaphoreHandle_t registry_mutex;       /**< @brief Mutex to protect access to the component registry. */
    registered_component_node_t* active_component; /**< @brief Pointer to the node of the currently active/viewed component. */

    // --- Cached Status ---
    bool wifi_connected;                    /**< @brief Cached WiFi connection status. */
    int32_t wifi_rssi;                      /**< @brief Cached WiFi signal strength (RSSI). */

    // --- Task & Input Handling ---
    int64_t last_button_press_time;         /**< @brief Timestamp of the last button press for debouncing/cooldown. */
    TaskHandle_t ui_task_handle;            /**< @brief Handle to the dedicated UI processing task. */
    QueueHandle_t ui_cmd_queue;             /**< @brief Queue for sending commands from event handlers to the UI task. */
};

// --- Forward declarations for internal functions shared across .c files ---

/**
 * @brief The main event handler function for the UI Manager module.
 * @param[in] self A pointer to the `ui_manager` module instance.
 * @param[in] event_name The name of the event that was triggered.
 * @param[in] event_data A pointer to the event's data wrapper.
 */
void ui_events_handle(module_t *self, const char *event_name, void *event_data);

/**
 * @brief Processes button presses when the UI is in a menu or component state.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] button_name A string representing the button that was pressed.
 */
void ui_menu_handle_navigation(ui_manager_private_data_t *private_data, const char* button_name);

/**
 * @brief Renders the currently active menu screen by iterating through registered components.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
void ui_menu_render(ui_manager_private_data_t *private_data);

/**
 * @brief The main rendering orchestrator function. Calls the appropriate screen/component renderer.
 * @param[in] private_data A pointer to the module's private data structure.
 */
void render_current_state(ui_manager_private_data_t *private_data);

/**
 * @brief Resets the screen's auto-off timer upon user activity.
 * @param[in] private_data A pointer to the module's private data structure.
 */
void reset_screen_off_timer(ui_manager_private_data_t *private_data);

/**
 * @brief Renders the status header at the top of the screen.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] title The title text to display on the left side of the header.
 */
void render_header(ui_manager_private_data_t *private_data, const char* title);

/**
 * @brief Renders the initial splash screen.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
void render_splash_screen(ui_manager_private_data_t *private_data);

/**
 * @brief Renders the main home screen.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
void render_home_screen(ui_manager_private_data_t *private_data);

/**
 * @brief Renders a generic button in the footer area.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] text The text to display inside the button.
 * @param[in] is_selected If true, the button is rendered with inverted colors.
 */
void render_footer_button(ui_manager_private_data_t *private_data, const char* text, bool is_selected);

/**
 * @brief Formats a snake_case module name into a Title Case string for display.
 * @param[in] original The original module name (e.g., "relay_actuator").
 * @param[out] formatted The buffer to store the formatted name (e.g., "Relay Actuator").
 * @param[in] size The size of the formatted buffer.
 */
void format_module_name(const char *original, char *formatted, size_t size);

#endif // UI_MANAGER_INTERNAL_H