/**
 * @file i2c_bus.h
 * @brief Public header for the I2C Bus module.
 * @author Giorgi Magradze
 * @version 2.2.0
 */

#ifndef I2C_BUS_H
#define I2C_BUS_H

#include "synapse.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Factory function to create a new instance of the I2C Bus module.
     * @param[in] config The cJSON configuration object for this instance.
     * @return A pointer to the new module_t instance, or NULL on failure.
     */
    module_t *i2c_bus_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // I2C_BUS_H