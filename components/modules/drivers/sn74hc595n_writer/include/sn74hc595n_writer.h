/**
 * @file sn74hc595n_writer.h
 * @brief Public header for the SN74HC595N Writer module.
 * @author (Your Name) & Synapse AI Assistant
 * @version 1.0.0
 * @date 2025-08-20
 */
#ifndef SN74HC595N_WRITER_H
#define SN74HC595N_WRITER_H

#include "base_module.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a new instance of the SN74HC595N Writer module.
 *
 * @param[in] config A pointer to the cJSON object containing the module's configuration.
 *                   The module takes ownership of this object.
 *
 * @return A pointer to the newly created module_t instance, or NULL on failure.
 */
module_t* sn74hc595n_writer_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // SN74HC595N_WRITER_H