/**
 * @file ui_menu.c
 * @brief Implements the dynamic main menu logic for the UI Manager.
 * @author Synapse Framework Team
 * @version 2.0.0
 * @date 2025-09-12
 */
#include "synapse.h"
#include "ui_manager_internal.h"
#include "ui_interface.h"
#include <sys/queue.h>

// DEFINE_COMPONENT_TAG("UI_MENU", SYNAPSE_LOG_COLOR_YELLOW);

#define START_Y 16
#define LINE_HEIGHT 12

// Helper to get the Nth component from the list
static registered_component_node_t *get_component_by_index(ui_manager_private_data_t *p_data, int index)
{
  if (xSemaphoreTake(p_data->registry_mutex, portMAX_DELAY) != pdTRUE)
    return NULL;

  registered_component_node_t *node = SLIST_FIRST(&p_data->component_registry_head);
  int current_index = 0;
  while (node != NULL && current_index < index)
  {
    node = SLIST_NEXT(node, entries);
    current_index++;
  }

  xSemaphoreGive(p_data->registry_mutex);
  return node;
}

// Helper to count components
static int count_components(ui_manager_private_data_t *p_data)
{
  if (xSemaphoreTake(p_data->registry_mutex, portMAX_DELAY) != pdTRUE)
    return 0;

  int count = 0;
  registered_component_node_t *node;
  SLIST_FOREACH(node, &p_data->component_registry_head, entries)
  {
    if (node->component_data->type == UI_COMP_SCREEN)
    {
      count++;
    }
  }

  xSemaphoreGive(p_data->registry_mutex);
  return count;
}

void ui_menu_handle_navigation(ui_manager_private_data_t *private_data, const char *button_name)
{
  if (private_data->current_state == UI_STATE_MAIN_MENU)
  {
    int total_items = count_components(private_data) + 1; // +1 for BACK
    if (total_items <= 1)
    { // Only BACK button
      if (strcmp(button_name, "OK") == 0)
      {
        private_data->current_state = UI_STATE_HOME;
      }
      return;
    }

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
      if (private_data->selected_item_index < total_items - 1)
      { // A component is selected
        private_data->previous_menu_selection = private_data->selected_item_index;
        private_data->active_component = get_component_by_index(private_data, private_data->selected_item_index);
        if (private_data->active_component)
        {
          private_data->current_state = UI_STATE_COMPONENT_ACTIVE;
          private_data->selected_item_index = 0; // Reset for the component screen
        }
      }
      else
      { // BACK is selected
        private_data->current_state = UI_STATE_HOME;
      }
    }
  }
  else if (private_data->current_state == UI_STATE_COMPONENT_ACTIVE)
  {
    // When a component is active, we manage a simple selection between
    // its primary action (index 0) and the BACK button (index 1).
    int total_items = 2; // Action + BACK

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
      { // Action selected
        // Forward the "OK" event to the component
        if (private_data->active_component && private_data->active_component->component_data->event_cb)
        {
          ui_event_t event = {.button_name = "OK"};
          private_data->active_component->component_data->event_cb(private_data->active_component->owner_module, &event);
        }
      }
      else
      { // BACK selected
        // Go back to the main menu
        private_data->current_state = UI_STATE_MAIN_MENU;
        private_data->active_component = NULL;
        private_data->selected_item_index = private_data->previous_menu_selection;
      }
    }
  }
}

void ui_menu_render(ui_manager_private_data_t *private_data)
{
  const display_driver_api_t *display = private_data->display->api;
  void *context = private_data->display->context;

  int component_count = count_components(private_data);
  if (component_count == 0)
  {
    display->draw_formatted_text(context, 2, START_Y, 1, "No components.");
  }
  else
  {
    if (xSemaphoreTake(private_data->registry_mutex, portMAX_DELAY) == pdTRUE)
    {
      registered_component_node_t *node;
      int i = 0;
      SLIST_FOREACH(node, &private_data->component_registry_head, entries)
      {
        if (node->component_data->type != UI_COMP_SCREEN)
          continue;

        int y_pos = START_Y + i * LINE_HEIGHT;

        char formatted_name[32];
        format_module_name(node->owner_module->name, formatted_name, sizeof(formatted_name));

        if (i == private_data->selected_item_index)
        {
          display->draw_formatted_text(context, 2, y_pos, 1, "> %s", formatted_name);
        }
        else
        {
          display->draw_formatted_text(context, 10, y_pos, 1, "%s", formatted_name);
        }
        i++;
      }
      xSemaphoreGive(private_data->registry_mutex);
    }
  }

  bool is_back_selected = (private_data->selected_item_index == component_count);
  render_footer_button(private_data, "BACK", is_back_selected);
}