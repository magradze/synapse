/**
 * @file cmd_router_interface.h
 * @brief Public Service API contract for the Command Router module.
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-07-03
 * @details This header defines the abstract interface (API structure) for the
 *          Command Router service. Any module wishing to register commands
 *          should include this file to interact with the router via the
 *          Service Locator. This ensures decoupling from the specific
 *          implementation of the command router.
 */

#ifndef SYNAPSE_CMD_ROUTER_INTERFACE_H
#define SYNAPSE_CMD_ROUTER_INTERFACE_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defines the structure for a single command to be registered.
 * @details This structure encapsulates all necessary information for a command,
 *          including its name, help text, usage format, argument constraints,
 *          and the handler function to be executed.
 */
typedef struct {
    /**
     * @brief The command name (e.g., "relay", "wifi"). This is what the user types.
     */
    const char *command;

    /**
     * @brief A brief description of what the command does. Used by the 'help' command.
     */
    const char *help;

    /**
     * @brief A string showing the correct usage format (e.g., "relay <name> <on|off>").
     */
    const char *usage;

    /**
     * @brief The minimum number of arguments required (including the command itself).
     */
    int min_args;

    /**
     * @brief The maximum number of arguments allowed (including the command itself).
     */
    int max_args;

    /**
     * @brief Pointer to the handler function that will be executed for this command.
     * @param argc The number of arguments (argument count).
     * @param argv An array of strings representing the arguments (argument vector).
     * @param context A user-defined context pointer, passed during registration.
     * @return ESP_OK on success, or an error code on failure.
     */
    esp_err_t (*handler)(int argc, char **argv, void *context);

    /**
     * @brief A generic context pointer that will be passed to the handler.
     *        This is often a pointer to the registering module's private data.
     */
    void *context;

} cmd_t;


/**
 * @brief The public Service API structure for the Command Router.
 * @details This is the "contract" that the command_router module implements
 *          and other modules consume via the Service Locator.
 */
typedef struct
{
    /**
     * @brief Registers a new command with the router.
     * @param[in] command A pointer to a cmd_t structure describing the command.
     *                  The pointed-to structure must remain valid for the lifetime
     *                  of the registration.
     * @return 
     *      - ESP_OK: If the command was registered successfully.
     *      - ESP_ERR_INVALID_ARG: If the command pointer or its members are invalid.
     *      - ESP_ERR_NO_MEM: If the command list is full.
     *      - ESP_ERR_INVALID_STATE: If a command with the same name already exists.
     */
    esp_err_t (*register_command)(const cmd_t *command);

    /**
     * @brief Unregisters a previously registered command.
     * @details This function should be called by a module in its deinit phase
     *          to clean up its registered commands.
     * @param[in] command_name The name of the command to unregister.
     * @return 
     *      - ESP_OK: If the command was unregistered successfully.
     *      - ESP_ERR_NOT_FOUND: If no command with the given name was found.
     */
    esp_err_t (*unregister_command)(const char *command_name);

    /**
     * @brief Checks if a command with the given name is already registered.
     * @param[in] command_name The name of the command to check.
     * @return
     *      - true: If the command is already registered.
     *      - false: If the command is not registered.
     */
    bool (*is_command_registered)(const char *command_name);

} cmd_router_api_t;


#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_CMD_ROUTER_INTERFACE_H