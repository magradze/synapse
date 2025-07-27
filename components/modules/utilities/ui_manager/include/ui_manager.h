/**
 * @file ui_manager.h
 * @brief Main header for the UI Manager module, defining core structures, enums, and constants.
 * @details This file serves as the central hub for the UI system's definitions.
 *          It declares the main private data structure that holds the UI state,
 *          enumerations for different screens, and prototypes for the module's
 *          public and shared internal functions. All other UI-related source
 *          files (`ui_events.c`, `ui_menu.c`, etc.) include this header to access
 *          the shared state and definitions.
 * @author Synapse Framework Team
 * @version 4.3.0
 * @date 2025-08-14
 */
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "base_module.h"
#include "cJSON.h"
#include "display_driver_interface.h"
#include "system_manager_interface.h"
#include "timer_interface.h"
#include "time_sync_interface.h"

// --- Constants ---
#define SCREEN_OFF_TIMEOUT_MS 30000
#define HOME_SCREEN_UPDATE_INTERVAL_MS 1000
#define WIFI_STATUS_UPDATE_INTERVAL_MS 5000
#define BUTTON_COOLDOWN_MS 200
#define MAX_MENU_ITEMS CONFIG_FMW_MAX_MODULES
#define MAX_VISIBLE_MENU_ITEMS 3

// --- Event Names ---
#define SCREEN_OFF_TIMER_EVENT "UI_SCREEN_OFF_TIMER"
#define SPLASH_SCREEN_TIMER_EVENT "UI_SPLASH_END"
#define WIFI_STATUS_TIMER_EVENT "UI_WIFI_STATUS_TIMER"
#define MODULE_CONTROL_ACTION_TIMER_EVENT "UI_MODULE_ACTION_TIMER"

/**
 * @enum ui_state_t
 * @brief Defines the possible states (screens) of the user interface.
 */
typedef enum
{
    UI_STATE_SPLASH,         /**< @brief The initial splash screen shown on startup. */
    UI_STATE_HOME,           /**< @brief The main home screen, showing time and status. */
    UI_STATE_MAIN_MENU,      /**< @brief The top-level menu. */
    UI_STATE_MODULES_MENU,   /**< @brief The sub-menu listing all available modules. */
    UI_STATE_MODULE_CONTROL, /**< @brief The control screen for a selected module. */
    UI_STATE_SETTINGS_MENU,  /**< @brief The sub-menu for system settings. */
} ui_state_t;

/**
 * @struct menu_item_t
 * @brief Represents a single item within a static menu.
 */
typedef struct {
    const char* label;      /**< @brief The text to be displayed for this menu item. */
    ui_state_t target_state;/**< @brief The UI state to transition to when this item is selected. */
} menu_item_t;

/**
 * @struct static_menu_t
 * @brief Represents a complete static menu, containing a title and a list of items.
 */
typedef struct {
    const char* title;          /**< @brief The title of the menu to be displayed in the header. */
    const menu_item_t* items;   /**< @brief A pointer to an array of `menu_item_t`. */
    int item_count;             /**< @brief The number of items in the `items` array. */
} static_menu_t;

/**
 * @struct ui_manager_private_data_t
 * @brief The central private data structure holding the entire state of the UI Manager.
 */
typedef struct {
    module_t *module;                   /**< @brief Back-pointer to the parent module structure. */
    display_driver_handle_t *display;   /**< @brief Handle to the active display driver service. */
    system_manager_api_t *system_manager;/**< @brief Handle to the system manager service. */
    timer_api_t *timer;                 /**< @brief Handle to the system timer service. */
    time_sync_api_t *time_sync;         /**< @brief Handle to the time synchronization service. */

    uint16_t display_width;             /**< @brief Cached width of the display in pixels. */
    uint16_t display_height;            /**< @brief Cached height of the display in pixels. */
    font_metrics_t small_font_metrics;  /**< @brief Cached metrics of the driver's default small font. */

    ui_state_t current_state;           /**< @brief The current screen being displayed. */
    fmw_timer_handle_t screen_off_timer;/**< @brief Handle for the screen auto-off timer. */
    fmw_timer_handle_t home_screen_timer;/**< @brief Handle for the periodic home screen update timer. */
    fmw_timer_handle_t wifi_status_timer;/**< @brief Handle for the periodic WiFi status request timer. */
    bool is_screen_on;                  /**< @brief Tracks if the display panel is currently powered on. */

    int selected_item_index;            /**< @brief The index of the currently selected item in a menu. */
    int top_visible_item;               /**< @brief The index of the topmost visible item for scrolling menus. */
    int previous_menu_selection;        /**< @brief The last selected item index before entering a sub-menu. */
    const module_t* dynamic_menu_modules[MAX_MENU_ITEMS]; /**< @brief Array of module pointers for the dynamic menu. */
    int dynamic_menu_item_count;        /**< @brief The number of modules in the dynamic menu. */
    const module_t *selected_control_module; /**< @brief The module currently selected for control actions. */

    bool wifi_connected;                /**< @brief Cached WiFi connection status. */
    int32_t wifi_rssi;                  /**< @brief Cached WiFi signal strength (RSSI). */

    int64_t last_button_press_time;     /**< @brief Timestamp of the last button press for debouncing/cooldown. */
} ui_manager_private_data_t;

/**
 * @brief Creates a new instance of the UI Manager module.
 * @details This is the factory function for the module, called by the Module Factory.
 * @param[in] config A cJSON object containing the configuration for this instance.
 *                   The function takes ownership of this object.
 * @return A pointer to the newly created module_t instance, or NULL on failure.
 */
module_t* ui_manager_create(const cJSON *config);

/**
 * @brief The main rendering orchestrator function.
 * @details This function is called whenever the UI needs to be redrawn. It checks
 *          the current UI state and calls the appropriate rendering function for
 *          that specific screen (e.g., `render_home_screen`, `ui_menu_render`).
 * @param[in] private_data A pointer to the module's private data structure.
 */
void render_current_state(ui_manager_private_data_t *private_data);

/**
 * @brief Resets the screen's auto-off timer.
 * @details This function cancels any existing screen-off timer and schedules a new
 *          one. It should be called whenever there is user activity to prevent
 *          the screen from turning off.
 * @param[in] private_data A pointer to the module's private data structure.
 */
void reset_screen_off_timer(ui_manager_private_data_t *private_data);

#endif // UI_MANAGER_H