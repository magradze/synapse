/**
 * @file ui_interface.h
 * @brief Defines the public API and contracts for the Modular UI system.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-09-12
 *
 * @details
 * This file establishes the "contract" between the central UI Manager and any
 * module that wishes to provide a UI component. It defines the structures and
 * function pointers that modules must implement to register and manage their
 * UI elements dynamically.
 */
#ifndef UI_INTERFACE_H
#define UI_INTERFACE_H

#include "synapse.h"
#include "display_driver_interface.h"

// Forward declarations
struct ui_component_t;

/**
 * @enum ui_component_type_t
 * @brief Enumerates the types of UI components a module can register.
 */
typedef enum {
    UI_COMP_SCREEN,     /**< A full-screen component, typically a main menu entry. */
    UI_COMP_PANEL,      /**< A component that occupies a portion of a screen. (Future use) */
    UI_COMP_BUTTON,     /**< An interactive button element. (Future use) */
    UI_COMP_INDICATOR,  /**< A status indicator, e.g., for the header. (Future use) */
} ui_component_type_t;

/**
 * @struct ui_event_t
 * @brief Describes a user interaction event to be handled by a UI component.
 */
typedef struct {
    const char* button_name; /**< The name of the button pressed (e.g., "UP", "OK", "BACK"). */
    // Other event details like coordinates for touch can be added here.
} ui_event_t;

/**
 * @struct ui_context_t
 * @brief Provides rendering context to a UI component's render callback.
 */
typedef struct {
    display_driver_handle_t* display; /**< Handle to the display driver for drawing operations. */
    bool is_active;                   /**< True if this component is currently active/selected. */
} ui_context_t;

/**
 * @brief Callback function pointer for rendering a UI component.
 * @param self A pointer to the module instance that owns this component.
 * @param context The rendering context, providing access to the display driver.
 */
typedef void (*ui_render_cb_t)(module_t* self, ui_context_t* context);

/**
 * @brief Callback function pointer for handling events within a UI component.
 * @param self A pointer to the module instance that owns this component.
 * @param event The user interaction event to be handled.
 */
typedef void (*ui_event_cb_t)(module_t* self, ui_event_t* event);

/**
 * @struct ui_component_t
 * @brief The core "contract" structure for a dynamically registered UI component.
 */
typedef struct ui_component_t {
    const char* id;             /**< A unique identifier for the component (e.g., "relay_control_screen"). */
    ui_component_type_t type;   /**< The type of the component. */
    const char* menu_text;      /**< The text to display in the main menu for this component. */
    ui_render_cb_t render_cb;   /**< Pointer to the component's rendering function. */
    ui_event_cb_t event_cb;     /**< Pointer to the component's event handling function. */
} ui_component_t;

/**
 * @struct ui_manager_api_t
 * @brief Defines the Service API provided by the UI Manager.
 */
typedef struct {
    /**
     * @brief Registers an array of UI components from a module.
     * @param owner The module instance that owns these components.
     * @param components A pointer to a NULL-terminated array of ui_component_t structs.
     * @return ESP_OK on success, or an error code on failure.
     */
    esp_err_t (*register_components)(module_t* owner, const ui_component_t** components);

    /**
     * @brief Unregisters all UI components previously registered by a specific module.
     * @param owner The module instance whose components should be unregistered.
     * @return ESP_OK on success.
     */
    esp_err_t (*unregister_components)(module_t* owner);
} ui_manager_api_t;

#endif // UI_INTERFACE_H