/**
 * @file relay_actuator_cmd.c
 * @brief CLI command handler for the Relay Actuator module.
 */
#include "synapse.h"
#include "relay_actuator_internal.h"

DEFINE_COMPONENT_TAG("RELAY_CMD", SYNAPSE_LOG_COLOR_GREEN);

// These are defined in relay_actuator.c but we need access to them.
extern module_t *s_relay_instances[];
extern uint8_t s_relay_instance_count;

esp_err_t generic_relay_cmd_handler(int argc, char **argv, void *context)
{
    // The context argument is unused in this generic handler, but must be present
    // to match the required function signature.
    (void)context;

    SYNAPSE_GUARD(argc == 3, TAG, ESP_ERR_INVALID_ARG, "Usage: relay <name> <on|off|toggle>");
    
    const char *instance_name = argv[1];
    const char *action = argv[2];

    module_t *target_module = NULL;
    for (int i = 0; i < s_relay_instance_count; i++) {
        if (strcmp(s_relay_instances[i]->name, instance_name) == 0) {
            target_module = s_relay_instances[i];
            break;
        }
    }
    SYNAPSE_GUARD(target_module, TAG, ESP_ERR_NOT_FOUND, "Relay '%s' not found.", instance_name);

    relay_private_data_t *p_data = (relay_private_data_t *)target_module->private_data;
    if (strcmp(action, "on") == 0)      return set_relay_state(target_module, true);
    if (strcmp(action, "off") == 0)     return set_relay_state(target_module, false);
    if (strcmp(action, "toggle") == 0)  return set_relay_state(target_module, !p_data->current_state);

    ESP_LOGE(TAG, "Unknown action '%s'. Use 'on', 'off', or 'toggle'.", action);
    return ESP_ERR_INVALID_ARG;
}