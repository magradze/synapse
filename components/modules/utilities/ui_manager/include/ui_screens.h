/**
 * @file ui_screens.h
 * @brief Declares the rendering functions for all UI screens and common components.
 * @details This file contains the function prototypes for each distinct screen
 *          (like the home screen and splash screen) and reusable UI components
 *          (like the header and footer buttons). These functions are responsible
 *          for all drawing operations and are called by the main UI orchestrator.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-08-14
 */
#ifndef UI_SCREENS_H
#define UI_SCREENS_H

#include "ui_manager.h"

/**
 * @brief Renders the status header at the top of the screen.
 * @details The header typically contains status icons such as WiFi signal
 *          strength and battery level.
 * @param[in] private_data A pointer to the main UI Manager's private data structure,
 *                         used to access status information and the display driver.
 */
void render_header(ui_manager_private_data_t *private_data);

/**
 * @brief Renders the initial splash screen.
 * @details This screen is displayed for a short duration when the device starts up.
 *          It typically shows a logo or brand name.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
void render_splash_screen(ui_manager_private_data_t *private_data);

/**
 * @brief Renders the main home screen.
 * @details The home screen is the default view after startup, usually displaying
 *          key information like the current time and primary sensor readings.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
void render_home_screen(ui_manager_private_data_t *private_data);

/**
 * @brief Renders a generic button in the footer area (bottom-right).
 * @details This function dynamically calculates the button's size based on the
 *          text length and applies different styling if the button is selected.
 *          Used for "MENU" and "BACK" buttons.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] text The text to display inside the button.
 * @param[in] is_selected If true, the button is rendered with inverted colors to
 *                        indicate it is currently selected by the user.
 */
void render_footer_button(ui_manager_private_data_t *private_data, const char* text, bool is_selected);

#endif // UI_SCREENS_H