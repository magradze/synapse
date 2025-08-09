/**
 * @file ui_menu.h
 * @brief Manages menu state, navigation, and rendering for the UI Manager.
 * @details This header declares the core functions responsible for all menu-related
 *          logic within the UI system. This includes handling user navigation input
 *          (up, down, ok), rendering the appropriate menu screen based on the
 *          current UI state, and populating the dynamic list of modules.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-08-14
 */
#ifndef UI_MENU_H
#define UI_MENU_H

#include "ui_manager.h"

/**
 * @brief Processes button presses when the UI is in a menu state.
 * @details This function contains the state machine logic for menu navigation.
 *          It updates the `selected_item_index`, `top_visible_item`, and
 *          `current_state` fields in the private_data structure based on the
 *          button pressed.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] button_name A string representing the button that was pressed (e.g., "UP", "OK").
 */
void ui_menu_handle_navigation(ui_manager_private_data_t *private_data, const char* button_name);

/**
 * @brief Renders the currently active menu screen.
 * @details This function checks the `current_state` and calls the appropriate
 *          internal rendering function for that menu (e.g., `render_static_menu`
 *          for the main menu, `render_modules_menu` for the dynamic list).
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
void ui_menu_render(ui_manager_private_data_t *private_data);

/**
 * @brief Populates the dynamic menu with the list of currently running modules.
 * @details This function calls the System Manager service to get a list of all
 *          registered modules and stores them in the `dynamic_menu_modules` array
 *          within the private_data structure.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
void ui_menu_populate_items(ui_manager_private_data_t *private_data);

#endif // UI_MENU_H