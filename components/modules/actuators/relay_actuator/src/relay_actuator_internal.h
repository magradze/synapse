/**
 * @file relay_actuator_internal.h
 * @brief Internal shared definitions for the Relay Actuator module.
 * @author Synapse Framework Team
 * @version 2.0.0
 * @date 2025-09-12
 *
 * @details
 * This private header file is shared ONLY among the .c files within the
 * relay_actuator module. It defines the internal data structures and provides
 * forward declarations for all functions shared between the module's different
 * logical parts (lifecycle, API, events, commands, UI).
 */
#ifndef RELAY_ACTUATOR_INTERNAL_H
#define RELAY_ACTUATOR_INTERNAL_H

#include "relay_actuator.h"
#include "driver/gpio.h"
#include "storage_interface.h"

// --- Private Data & Definitions ---

/**
 * @internal
 * @enum initial_state_mode_t
 * @brief Defines the relay's behavior on power-up or module start.
 */
typedef enum {
    INITIAL_STATE_OFF,  /**< @brief The relay will always start in the OFF state. */
    INITIAL_STATE_ON,   /**< @brief The relay will always start in the ON state. */
    INITIAL_STATE_LAST, /**< @brief The relay will restore its last known state from persistent storage. */
} initial_state_mode_t;

/**
 * @internal
 * @struct relay_private_data_t
 * @brief The complete internal state of a Relay Actuator module instance.
 */
typedef struct {
    // --- Configuration ---
    gpio_num_t gpio_pin;                /**< @brief The GPIO pin number connected to the relay. */
    uint8_t active_level;               /**< @brief The GPIO level (0 or 1) that activates the relay. */
    initial_state_mode_t initial_state; /**< @brief The configured power-on behavior. */
    
    // --- Runtime State ---
    bool current_state;                 /**< @brief The current logical state of the relay (true=ON, false=OFF). */
    
    // --- Injected Dependencies ---
    storage_api_t* storage_service;     /**< @brief Handle to the storage service for persisting state. (Not yet implemented via DI). */
} relay_private_data_t;


// --- Forward Declarations for internal functions ---

// --- Lifecycle ---
/**
 * @internal @brief Initializes the relay module instance.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK on success.
 */
esp_err_t relay_actuator_init(module_t *self);

/**
 * @internal @brief Starts the relay module's operation and sets its initial state.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK on success.
 */
esp_err_t relay_actuator_start(module_t *self);

/**
 * @internal @brief Deinitializes the relay module, releasing all resources.
 * @param[in] self A pointer to the module instance.
 */
void relay_actuator_deinit(module_t *self);


// --- Service API ---
/**
 * @internal @brief Service API implementation for setting the relay state.
 * @param[in] context A pointer to the module instance (`module_t*`).
 * @param[in] is_on The desired new state.
 * @return ESP_OK on success.
 */
esp_err_t relay_api_set_state(void *context, bool is_on);

/**
 * @internal @brief Service API implementation for toggling the relay state.
 * @param[in] context A pointer to the module instance (`module_t*`).
 * @return ESP_OK on success.
 */
esp_err_t relay_api_toggle(void *context);

/**
 * @internal @brief Service API implementation for getting the current relay state.
 * @param[in] context A pointer to the module instance (`module_t*`).
 * @return The current logical state (true=ON, false=OFF).
 */
bool relay_api_get_state(void *context);


// --- Event Handling ---
/**
 * @internal @brief Handles events from the Event Bus, primarily for CLI registration.
 * @param[in] self A pointer to the module instance.
 * @param[in] event_name The name of the event.
 * @param[in] event_data A pointer to the event's data wrapper.
 */
void relay_module_handle_event(module_t *self, const char *event_name, void *event_data);


// --- Command Handling ---
/**
 * @internal @brief The generic handler for the "relay" CLI command.
 * @param[in] argc Number of arguments.
 * @param[in] argv Array of argument strings.
 * @param[in] context User-defined context (unused).
 * @return ESP_OK on success.
 */
esp_err_t generic_relay_cmd_handler(int argc, char **argv, void *context);


// --- Helpers ---
/**
 * @internal @brief The core logic to change the relay's physical state and publish updates.
 * @param[in] self A pointer to the module instance.
 * @param[in] is_on The desired new logical state.
 * @return ESP_OK on success.
 */
esp_err_t set_relay_state(module_t *self, bool is_on);


// --- UI Lifecycle Hooks ---
/**
 * @internal @brief Initializes and registers the module's UI components.
 * @details This function is assigned to `module->base.ui_init` and is called
 *          by the System Manager after the module has successfully started.
 * @param[in] self A pointer to the module instance.
 */
void relay_ui_init(module_t* self);

/**
 * @internal @brief Deinitializes and unregisters the module's UI components.
 * @details This function is assigned to `module->base.ui_deinit` and is called
 *          by the System Manager during graceful shutdown, before `deinit`.
 * @param[in] self A pointer to the module instance.
 */
void relay_ui_deinit(module_t* self);

#endif // RELAY_ACTUATOR_INTERNAL_H