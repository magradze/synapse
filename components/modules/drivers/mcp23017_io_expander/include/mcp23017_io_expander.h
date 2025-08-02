/**
 * @file mcp23017_io_expander.h
 * @brief Public header for the MCP23017 I/O Expander module.
 * @author Synapse Team
 */
#ifndef MCP23017_IO_EXPANDER_H
#define MCP23017_IO_EXPANDER_H

#include "base_module.h"
#include "cJSON.h"

/**
 * @brief Creates a new mcp23017_io_expander module instance.
 */
module_t *mcp23017_io_expander_create(const cJSON *config);

#endif // MCP23017_IO_EXPANDER_H