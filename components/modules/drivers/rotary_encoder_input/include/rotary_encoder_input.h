/**
 * @file rotary_encoder_input.h
 * @brief Public header for the Rotary Encoder Input module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-08-23
 * @details This file declares the factory function for the rotary encoder input module.
 */

#ifndef ROTARY_ENCODER_INPUT_H
#define ROTARY_ENCODER_INPUT_H

#include "base_module.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Factory function to create a new instance of the Rotary Encoder Input module.
 * @param[in] config A pointer to the cJSON configuration object for this instance.
 *                   The function takes ownership of this object.
 * @return A pointer to the newly created module_t structure, or NULL on failure.
 */
module_t* rotary_encoder_input_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // ROTARY_ENCODER_INPUT_H