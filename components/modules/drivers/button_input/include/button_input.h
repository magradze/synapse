/**
 * @file button_input.h
 * @brief A module to handle hardware button presses and publish events.
 * @author Giorgi Magradze
 */
#ifndef BUTTON_INPUT_H
#define BUTTON_INPUT_H

#include "base_module.h"
#include "cJSON.h"

/**
 * @brief Creates a new instance of the Button Input module.
 * @param[in] config A pointer to the cJSON configuration object for this instance.
 * @return A pointer to the newly created module_t structure, or NULL on failure.
 */
module_t *button_input_create(const cJSON *config);

#endif // BUTTON_INPUT_H