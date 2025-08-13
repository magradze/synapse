/**
 * @file ssd1306_driver.h
 * @brief Public header for the SSD1306 Driver module.
 * @details This file only contains the declaration for the module's create
 *          function, which is required by the Module Factory. The actual
 *          service API provided by this module is defined in the abstract
 *          `display_driver_interface.h`.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-30
 */

#ifndef SSD1306_DRIVER_H
#define SSD1306_DRIVER_H

#include "base_module.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the SSD1306 driver module.
 * @details This function is the entry point for the Module Factory. It allocates
 *          memory for the module, parses its configuration, and sets up its
 *          base lifecycle function pointers.
 * @param[in] config A cJSON object containing the configuration for this instance.
 *                   The function takes ownership of this object.
 * @return A pointer to the newly created module_t instance, or NULL on failure.
 */
module_t* ssd1306_driver_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // SSD1306_DRIVER_H