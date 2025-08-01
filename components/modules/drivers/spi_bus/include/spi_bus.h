/**
 * @file spi_bus.h
 * @brief Public header for the SPI Bus module.
 * @author Synapse Team
 * @version 1.0.0
 * @date 2025-08-20
 * @details This file contains the public function prototypes and structures
 *          for the SPI Bus module, primarily the module's create function.
 */
#ifndef SPI_BUS_H
#define SPI_BUS_H

#include "base_module.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the SPI Bus module.
 *
 * @details This function is the entry point for creating an SPI Bus module.
 *          It allocates memory, parses the provided JSON configuration, and
 *          sets up the module's lifecycle function pointers.
 *
 * @param[in] config A pointer to the cJSON object containing the module's configuration.
 *                   The module takes ownership of this object and will free it
 *                   during deinitialization.
 *
 * @return A pointer to the newly created module_t instance, or NULL on failure.
 */
module_t* spi_bus_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // SPI_BUS_H