/**
 * @file relay_actuator_events.c
 * @brief Event handler for the Relay Actuator module.
 */
#include "synapse.h"
#include "relay_actuator_internal.h"
#include "cmd_router_interface.h"

DEFINE_COMPONENT_TAG("RELAY_EVENTS", SYNAPSE_LOG_COLOR_GREEN);

void relay_module_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (strcmp(event_name, SYNAPSE_EVENT_SYSTEM_START_COMPLETE) == 0) {
        cmd_router_api_t *cmd_api = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_CMD_ROUTER_API);
        if (cmd_api && !cmd_api->is_command_registered("relay")) {
            ESP_LOGI(TAG, "Module '%s' is registering the generic 'relay' command.", self->name);
            static cmd_t relay_cmd = {
                .command = "relay", .help = "Controls a relay.", .usage = "relay <name> <on|off|toggle>",
                .min_args = 3, .max_args = 3, .handler = generic_relay_cmd_handler, .context = NULL,
            };
            cmd_api->register_command(&relay_cmd);
        }
    }
    if (event_data) {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}