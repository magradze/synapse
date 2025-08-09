/**
 * @file ui_menu.c
 * @brief Implements menu logic for the UI Manager.
 * @author Synapse Framework Team
 * @version 1.3.0
 * @date 2025-08-13
 */

#include "synapse.h"
#include "ui_menu.h"
#include "ui_screens.h"
#include "display_driver_interface.h"
#include <string.h>

DEFINE_COMPONENT_TAG("UI_MENU", SYNAPSE_LOG_COLOR_YELLOW);

#define START_Y 16
#define LINE_HEIGHT 12

// --- Static Menu Definitions ---

// 1. Define the items array first
static const menu_item_t main_menu_items[] = {
    {"Modules", UI_STATE_MODULES_MENU},
    {"Settings", UI_STATE_SETTINGS_MENU},
};
// 2. Then define the menu struct that uses it
const static_menu_t main_menu = {
    .title = "Menu",
    .items = main_menu_items,
    .item_count = sizeof(main_menu_items) / sizeof(menu_item_t)};

// 1. Define the items array first
static const menu_item_t settings_menu_items[] = {
    {"Languages", UI_STATE_HOME},
    {"Themes", UI_STATE_HOME},
    {"Time", UI_STATE_HOME},
    {"About", UI_STATE_HOME},
};
// 2. Then define the menu struct that uses it
const static_menu_t settings_menu = {
    .title = "Settings",
    .items = settings_menu_items,
    .item_count = sizeof(settings_menu_items) / sizeof(menu_item_t)};

// --- Forward Declarations for local rendering functions ---
static void render_static_menu(ui_manager_private_data_t *private_data, const static_menu_t *menu);
static void render_modules_menu(ui_manager_private_data_t *private_data);

/**
 * @brief Processes button presses to navigate through different menu states.
 * @details This function contains the core state machine logic for all menus.
 *          Based on the current UI state (`private_data->current_state`) and the
 *          button pressed, it updates the selected item index, adjusts the
 *          scrolling position (`top_visible_item`), and transitions to new UI states.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] button_name A string representing the button that was pressed (e.g., "UP", "OK").
 */
void ui_menu_handle_navigation(ui_manager_private_data_t *private_data, const char *button_name)
{
  switch (private_data->current_state)
  {
  case UI_STATE_MAIN_MENU:
  {
    int total_items = main_menu.item_count + 1;
    if (strcmp(button_name, "DOWN") == 0)
      private_data->selected_item_index = (private_data->selected_item_index + 1) % total_items;
    else if (strcmp(button_name, "UP") == 0)
      private_data->selected_item_index = (private_data->selected_item_index - 1 + total_items) % total_items;
    else if (strcmp(button_name, "OK") == 0)
    {
      if (private_data->selected_item_index < main_menu.item_count)
      {
        private_data->current_state = main_menu.items[private_data->selected_item_index].target_state;
        private_data->selected_item_index = 0;
      }
      else
      {
        private_data->current_state = UI_STATE_HOME;
      }
    }
    break;
  }
  case UI_STATE_MODULES_MENU:
  {
    int total_items = private_data->dynamic_menu_item_count + 1;
    if (total_items > 1)
    {
      if (strcmp(button_name, "DOWN") == 0)
      {
        private_data->selected_item_index = (private_data->selected_item_index + 1) % total_items;
        if (private_data->selected_item_index >= private_data->top_visible_item + MAX_VISIBLE_MENU_ITEMS)
          private_data->top_visible_item++;
        if (private_data->selected_item_index == 0)
          private_data->top_visible_item = 0;
      }
      else if (strcmp(button_name, "UP") == 0)
      {
        private_data->selected_item_index = (private_data->selected_item_index - 1 + total_items) % total_items;
        if (private_data->selected_item_index < private_data->top_visible_item)
          private_data->top_visible_item--;
        if (private_data->selected_item_index == total_items - 1)
          private_data->top_visible_item = (total_items - 1) - MAX_VISIBLE_MENU_ITEMS < 0 ? 0 : (total_items - 1) - MAX_VISIBLE_MENU_ITEMS;
      }
    }
    if (strcmp(button_name, "OK") == 0)
    {
      if (private_data->selected_item_index < private_data->dynamic_menu_item_count)
      {
        private_data->previous_menu_selection = private_data->selected_item_index;
        private_data->selected_control_module = private_data->dynamic_menu_modules[private_data->selected_item_index];
        private_data->current_state = UI_STATE_MODULE_CONTROL;
        private_data->selected_item_index = 0;
      }
      else
      {
        private_data->current_state = UI_STATE_MAIN_MENU;
        private_data->selected_item_index = 0;
        private_data->top_visible_item = 0;
      }
    }
    break;
  }
  case UI_STATE_SETTINGS_MENU:
  {
    int total_items = settings_menu.item_count + 1;
    if (strcmp(button_name, "DOWN") == 0)
      private_data->selected_item_index = (private_data->selected_item_index + 1) % total_items;
    else if (strcmp(button_name, "UP") == 0)
      private_data->selected_item_index = (private_data->selected_item_index - 1 + total_items) % total_items;
    else if (strcmp(button_name, "OK") == 0)
    {
      if (private_data->selected_item_index < settings_menu.item_count)
      {
        // Future: Go to selected setting state
      }
      else
      {
        private_data->current_state = UI_STATE_MAIN_MENU;
        private_data->selected_item_index = 0;
      }
    }
    break;
  }
  case UI_STATE_MODULE_CONTROL:
  {
    int total_items = 2;

    if (strcmp(button_name, "DOWN") == 0)
    {
      private_data->selected_item_index = (private_data->selected_item_index + 1) % total_items;
    }
    else if (strcmp(button_name, "UP") == 0)
    {
      private_data->selected_item_index = (private_data->selected_item_index - 1 + total_items) % total_items;
    }
    else if (strcmp(button_name, "OK") == 0)
    {
      if (private_data->selected_item_index == 0)
      {
        // Action is handled in ui_events.c
      }
      else
      { // BACK selected
        private_data->current_state = UI_STATE_MODULES_MENU;
        private_data->selected_item_index = private_data->previous_menu_selection;
      }
    }
    break;
  }
  default:
    break;
  }
}

/**
 * @brief Renders the currently active menu screen.
 * @details This function acts as a dispatcher for menu rendering. It checks the
 *          `current_state` and calls the appropriate specialized rendering function
 *          (e.g., `render_static_menu`, `render_modules_menu`) to draw the
 *          correct menu on the screen.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
void ui_menu_render(ui_manager_private_data_t *private_data)
{
  switch (private_data->current_state)
  {
  case UI_STATE_MAIN_MENU:
    render_static_menu(private_data, &main_menu);
    break;
  case UI_STATE_MODULES_MENU:
    render_modules_menu(private_data);
    break;
  case UI_STATE_SETTINGS_MENU:
    render_static_menu(private_data, &settings_menu);
    break;
  default:
    break;
  }
}

/**
 * @brief Populates the dynamic menu list with currently registered modules.
 * @details This function calls the System Manager service to get a fresh list of
 *          all modules in the system. It then populates the `dynamic_menu_modules`
 *          array in the private data structure, which is used by `render_modules_menu`
 *          to display the list.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
void ui_menu_populate_items(ui_manager_private_data_t *private_data)
{
  const module_t **modules = NULL;
  uint8_t count = 0;
  private_data->dynamic_menu_item_count = 0;

  if (private_data->system_manager && private_data->system_manager->get_all_modules(&modules, &count) == ESP_OK)
  {
    for (uint8_t i = 0; i < count && i < MAX_MENU_ITEMS; i++)
    {
      private_data->dynamic_menu_modules[i] = modules[i];
    }
    private_data->dynamic_menu_item_count = count;
  }
  ESP_LOGI(TAG, "Populated dynamic menu with %d items.", private_data->dynamic_menu_item_count);
}

/**
 * @internal
 * @brief Renders a menu with a predefined, static list of items.
 * @details This function is used for menus like the Main Menu and Settings Menu,
 *          where the items are known at compile time. It iterates through the
 *          items of the provided `static_menu_t` struct, highlights the selected
 *          one, and renders the footer button.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 * @param[in] menu A pointer to the `static_menu_t` structure to be rendered.
 */
static void render_static_menu(ui_manager_private_data_t *private_data, const static_menu_t *menu)
{
  const display_driver_api_t *display = private_data->display->api;
  void *context = private_data->display->context;

  int start_y = START_Y;
  int line_height = LINE_HEIGHT;

  for (int i = 0; i < menu->item_count; ++i)
  {
    int y_pos = start_y + i * line_height;
    if (i == private_data->selected_item_index)
    {
      display->draw_formatted_text(context, 2, y_pos, 1, "> %s", menu->items[i].label);
    }
    else
    {
      display->draw_formatted_text(context, 10, y_pos, 1, "%s", menu->items[i].label);
    }
  }

  bool is_back_selected = (private_data->selected_item_index == menu->item_count);
  render_footer_button(private_data, "BACK", is_back_selected);
}

/**
 * @internal
 * @brief Renders the dynamic menu that lists all system modules.
 * @details This function displays the list of modules fetched by `ui_menu_populate_items`.
 *          It implements the scrolling logic, ensuring that only a subset of items
 *          (`MAX_VISIBLE_MENU_ITEMS`) is visible at a time and that the selected
 *          item is always in view.
 * @param[in] private_data A pointer to the main UI Manager's private data structure.
 */
static void render_modules_menu(ui_manager_private_data_t *private_data)
{
  const display_driver_api_t *display = private_data->display->api;
  void *context = private_data->display->context;

  int start_y = START_Y;
  int line_height = LINE_HEIGHT;

  if (private_data->dynamic_menu_item_count == 0)
  {
    display->draw_formatted_text(context, 2, start_y, 1, "No modules found.");
  }
  else
  {
    for (int i = 0; i < MAX_VISIBLE_MENU_ITEMS; ++i)
    {
      int item_index = private_data->top_visible_item + i;
      if (item_index >= private_data->dynamic_menu_item_count)
        break;

      int y_pos = start_y + i * line_height;
      const char *original_name = private_data->dynamic_menu_modules[item_index]->name;

      char formatted_name[CONFIG_SYNAPSE_MODULE_NAME_MAX_LENGTH];
      format_module_name(original_name, formatted_name, sizeof(formatted_name));

      if (item_index == private_data->selected_item_index)
      {
        display->draw_formatted_text(context, 2, y_pos, 1, "> %s", formatted_name);
      }
      else
      {
        display->draw_formatted_text(context, 10, y_pos, 1, "%s", formatted_name);
      }
    }
  }

  bool is_back_selected = (private_data->selected_item_index == private_data->dynamic_menu_item_count);
  render_footer_button(private_data, "BACK", is_back_selected);
}