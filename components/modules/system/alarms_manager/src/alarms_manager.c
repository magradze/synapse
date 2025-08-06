/**
 * @file alarms_manager.c
 * @brief Implementation of the System Alarms Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-08
 * @details
 * This module provides a centralized, rule-based system for reacting to
 * critical framework events. It subscribes to events specified in its
 * configuration, tracks their frequency and conditions, and executes a
 * series of predefined actions (e.g., logging, LED indication, system reboot)
 * when alarm thresholds are met. This approach centralizes error response
 * logic, making the system more robust and easier to manage.
 */

// --- Synapse Framework Includes ---
#include "synapse.h"
#include "alarms_manager.h"
#include "system_manager_interface.h"
#include "rgb_led_interface.h"

// --- Standard & ESP-IDF Includes ---
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>

// --- Component Tag ---
DEFINE_COMPONENT_TAG("ALARMS_MANAGER");

// --- Private Definitions ---
#define MAX_ALARM_RULES                 CONFIG_ALARMS_MANAGER_MAX_RULES
#define MAX_ALARM_ACTIONS               CONFIG_ALARMS_MANAGER_MAX_ACTIONS_PER_RULE
#define MAX_EVENT_NAME_LEN              CONFIG_ALARMS_MANAGER_MAX_EVENT_NAME_LEN
#define MAX_CONDITION_KEY_LEN           CONFIG_ALARMS_MANAGER_MAX_CONDITION_KEY_LEN
#define MAX_CONDITION_VALUE_LEN         CONFIG_ALARMS_MANAGER_MAX_CONDITION_VALUE_LEN
#define MAX_LOG_MESSAGE_LEN             CONFIG_ALARMS_MANAGER_MAX_LOG_MESSAGE_LEN

// --- Private Data Structures ---

/**
 * @internal
 * @enum alarm_action_type_t
 * @brief Defines the types of actions an alarm can execute.
 */
typedef enum {
    ALARM_ACTION_UNKNOWN,
    ALARM_ACTION_LOG_CRITICAL,
    ALARM_ACTION_SET_LED,
    ALARM_ACTION_REBOOT_SYSTEM,
} alarm_action_type_t;

/**
 * @internal
 * @struct alarm_action_t
 * @brief Represents a single action to be performed when an alarm is triggered.
 */
typedef struct {
    alarm_action_type_t type;
    char service_name[CONFIG_FMW_SERVICE_NAME_MAX_LENGTH];
    union {
        char log_message[MAX_LOG_MESSAGE_LEN];
        struct {
            uint8_t r, g, b;
        } led_color;
        struct {
            uint32_t delay_ms;
        } reboot_params;
    } params;
} alarm_action_t;

/**
 * @internal
 * @struct alarm_rule_t
 * @brief Represents a single, configurable alarm rule.
 */
typedef struct {
    char name[32];
    char trigger_event[MAX_EVENT_NAME_LEN];
    char condition_key[MAX_CONDITION_KEY_LEN];
    char condition_value[MAX_CONDITION_VALUE_LEN];
    uint32_t threshold_count;
    uint32_t time_window_sec;
    alarm_action_t actions[MAX_ALARM_ACTIONS];
    uint8_t action_count;
} alarm_rule_t;

/**
 * @internal
 * @struct alarm_state_t
 * @brief Tracks the runtime state of a triggered alarm rule.
 */
typedef struct {
    const alarm_rule_t *rule;
    uint32_t trigger_count;
    int64_t first_trigger_timestamp_us;
} alarm_state_t;

/**
 * @internal
 * @struct alarms_manager_private_data_t
 * @brief Private data for the Alarms Manager module.
 */
typedef struct {
    char instance_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];
    alarm_rule_t rules[MAX_ALARM_RULES];
    uint8_t rule_count;
    alarm_state_t active_alarms[MAX_ALARM_RULES];
    SemaphoreHandle_t state_mutex;
} alarms_manager_private_data_t;

// --- Forward Declarations ---
static esp_err_t alarms_manager_init(module_t *self);
static void alarms_manager_deinit(module_t *self);
static void alarms_manager_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t parse_config(const cJSON *config, alarms_manager_private_data_t *p_data);
static void execute_actions(const alarm_rule_t *rule);

// =========================================================================
//                      Module Lifecycle Implementation
// =========================================================================

module_t *alarms_manager_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating Alarms Manager module instance...");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module) {
        ESP_LOGE(TAG, "Failed to allocate memory for module_t.");
        return NULL;
    }

    alarms_manager_private_data_t *private_data = (alarms_manager_private_data_t *)calloc(1, sizeof(alarms_manager_private_data_t));
    if (!private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory for private_data.");
        free(module);
        return NULL;
    }
    module->private_data = private_data;

    private_data->state_mutex = xSemaphoreCreateMutex();
    if (!private_data->state_mutex) {
        ESP_LOGE(TAG, "Failed to create state mutex.");
        free(private_data);
        free(module);
        return NULL;
    }

    const char *instance_name = "main_alarms_manager";
    if (config) {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node)) {
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node)) instance_name = name_node->valuestring;
        }
        module->current_config = cJSON_Duplicate(config, true);
    }

    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    strncpy(private_data->instance_name, instance_name, sizeof(private_data->instance_name) - 1);
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->init_level = 38; // After core services, before most app modules.

    module->base.init = alarms_manager_init;
    module->base.deinit = alarms_manager_deinit;
    module->base.handle_event = alarms_manager_handle_event;
    // This is a background service module, no need for start/stop/enable/disable API.
    module->base.start = NULL;
    module->base.enable = NULL;
    module->base.disable = NULL;
    module->base.reconfigure = NULL;
    module->base.get_status = NULL;

    if (parse_config(config, private_data) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to parse configuration. Aborting creation.");
        vSemaphoreDelete(private_data->state_mutex);
        free(private_data);
        if (module->current_config) cJSON_Delete(module->current_config);
        free(module);
        return NULL;
    }

    ESP_LOGI(TAG, "Alarms Manager module '%s' created.", module->name);
    return module;
}

static esp_err_t alarms_manager_init(module_t *self)
{
    if (!self) return ESP_ERR_INVALID_ARG;
    alarms_manager_private_data_t *p_data = (alarms_manager_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing Alarms Manager: %s", self->name);

    // Subscribe to all unique events defined in the rules
    for (uint8_t i = 0; i < p_data->rule_count; i++) {
        bool already_subscribed = false;
        for (uint8_t j = 0; j < i; j++) {
            if (strcmp(p_data->rules[i].trigger_event, p_data->rules[j].trigger_event) == 0) {
                already_subscribed = true;
                break;
            }
        }
        if (!already_subscribed) {
            ESP_LOGI(TAG, "Subscribing to event: '%s'", p_data->rules[i].trigger_event);
            esp_err_t err = fmw_event_bus_subscribe(p_data->rules[i].trigger_event, self);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Failed to subscribe to event '%s': %s", p_data->rules[i].trigger_event, esp_err_to_name(err));
                // In a real scenario, we might want to handle this more gracefully.
            }
        }
    }

    self->status = MODULE_STATUS_RUNNING; // This module is active right after init.
    ESP_LOGI(TAG, "Alarms Manager initialized and running.");
    return ESP_OK;
}

static void alarms_manager_deinit(module_t *self)
{
    if (!self) return;
    alarms_manager_private_data_t *p_data = (alarms_manager_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Deinitializing Alarms Manager: %s", self->name);

    // Unsubscribe from all events
    for (uint8_t i = 0; i < p_data->rule_count; i++) {
        bool already_unsubscribed = false;
        for (uint8_t j = 0; j < i; j++) {
            if (strcmp(p_data->rules[i].trigger_event, p_data->rules[j].trigger_event) == 0) {
                already_unsubscribed = true;
                break;
            }
        }
        if (!already_unsubscribed) {
            fmw_event_bus_unsubscribe(p_data->rules[i].trigger_event, self);
        }
    }

    if (p_data->state_mutex) vSemaphoreDelete(p_data->state_mutex);
    if (self->private_data) free(self->private_data);
    if (self->current_config) cJSON_Delete(self->current_config);
    if (self->state_mutex) vSemaphoreDelete(self->state_mutex);
    free(self);
}

// =========================================================================
//                      Event Handling and Core Logic
// =========================================================================

static void alarms_manager_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (!self || !event_name) {
        if (event_data) fmw_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    alarms_manager_private_data_t *p_data = (alarms_manager_private_data_t *)self->private_data;
    event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;

    ESP_LOGD(TAG, "Event received: '%s'", event_name);

    if (xSemaphoreTake(p_data->state_mutex, pdMS_TO_TICKS(50)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to take state mutex for event handling.");
        if (wrapper) fmw_event_data_release(wrapper);
        return;
    }

    for (uint8_t i = 0; i < p_data->rule_count; i++) {
        alarm_rule_t *rule = &p_data->rules[i];
        if (strcmp(rule->trigger_event, event_name) == 0) {
            // Rule's trigger event matches the received event.
            
            // Step 1: Check condition if specified
            bool condition_met = true;
            if (strlen(rule->condition_key) > 0) {
                condition_met = false; // Assume not met until proven otherwise
                if (wrapper && wrapper->payload) {
                    // We assume payload is a cJSON string for simplicity
                    cJSON *payload_json = cJSON_Parse((char*)wrapper->payload);
                    if (payload_json) {
                        const cJSON *value_node = cJSON_GetObjectItem(payload_json, rule->condition_key);
                        if (cJSON_IsString(value_node) && strcmp(value_node->valuestring, rule->condition_value) == 0) {
                            condition_met = true;
                        }
                        cJSON_Delete(payload_json);
                    }
                }
            }

            if (!condition_met) {
                continue; // Condition not met, move to the next rule
            }

            // Step 2: Find or create the alarm state for this rule
            alarm_state_t *state = NULL;
            for (uint8_t j = 0; j < p_data->rule_count; j++) {
                if (p_data->active_alarms[j].rule == rule) {
                    state = &p_data->active_alarms[j];
                    break;
                }
            }
            // If not found, find an empty slot
            if (!state) {
                for (uint8_t j = 0; j < p_data->rule_count; j++) {
                    if (p_data->active_alarms[j].rule == NULL) {
                        state = &p_data->active_alarms[j];
                        state->rule = rule;
                        state->trigger_count = 0;
                        state->first_trigger_timestamp_us = 0;
                        break;
                    }
                }
            }

            if (!state) {
                ESP_LOGE(TAG, "No available alarm state slots for rule '%s'", rule->name);
                continue;
            }

            // Step 3: Update alarm state and check thresholds
            int64_t now_us = esp_timer_get_time();
            if (state->trigger_count == 0 || (now_us - state->first_trigger_timestamp_us) > (rule->time_window_sec * 1000000ULL)) {
                // Reset if this is the first trigger or the time window has expired
                state->first_trigger_timestamp_us = now_us;
                state->trigger_count = 1;
            } else {
                state->trigger_count++;
            }

            ESP_LOGI(TAG, "Alarm '%s' triggered. Count: %u/%u", rule->name, (unsigned int)state->trigger_count, (unsigned int)rule->threshold_count);

            if (state->trigger_count >= rule->threshold_count) {
                ESP_LOGW(TAG, "ALARM TRIGGERED: '%s' threshold reached!", rule->name);
                execute_actions(rule);
                // Reset the state after triggering
                state->trigger_count = 0;
                state->first_trigger_timestamp_us = 0;
            }
        }
    }

    xSemaphoreGive(p_data->state_mutex);
    if (wrapper) fmw_event_data_release(wrapper);
}

// =========================================================================
//                      Internal Helper Functions
// =========================================================================

static void execute_actions(const alarm_rule_t *rule)
{
    ESP_LOGI(TAG, "Executing %d actions for alarm '%s'", rule->action_count, rule->name);
    for (uint8_t i = 0; i < rule->action_count; i++) {
        const alarm_action_t *action = &rule->actions[i];
        switch (action->type) {
            case ALARM_ACTION_LOG_CRITICAL:
                ESP_LOGE("ALARM_TRIGGER", "%s", action->params.log_message);
                break;

            case ALARM_ACTION_SET_LED:
                {
                    service_handle_t handle = fmw_service_get(action->service_name);
                    if (handle) {
                        rgb_led_api_t *led_api = (rgb_led_api_t *)handle;
                        led_api->set_color(action->params.led_color.r, action->params.led_color.g, action->params.led_color.b);
                    } else {
                        ESP_LOGE(TAG, "LED service '%s' not found for alarm action.", action->service_name);
                    }
                }
                break;

            case ALARM_ACTION_REBOOT_SYSTEM:
                {
                    service_handle_t handle = fmw_service_get("system_manager");
                    if (handle) {
                        system_manager_api_t *sys_api = (system_manager_api_t *)handle;
                        ESP_LOGW(TAG, "Rebooting system in %" PRIu32 "ms due to alarm '%s'", action->params.reboot_params.delay_ms, rule->name);
                        vTaskDelay(pdMS_TO_TICKS(action->params.reboot_params.delay_ms));
                        sys_api->reboot_system();
                    } else {
                        ESP_LOGE(TAG, "System Manager service not found for reboot action.");
                    }
                }
                break;

            default:
                ESP_LOGW(TAG, "Unknown action type for alarm '%s'", rule->name);
                break;
        }
    }
}

static alarm_action_type_t action_str_to_type(const char *str) {
    if (strcmp(str, "log_critical") == 0) return ALARM_ACTION_LOG_CRITICAL;
    if (strcmp(str, "set_led") == 0) return ALARM_ACTION_SET_LED;
    if (strcmp(str, "reboot_system") == 0) return ALARM_ACTION_REBOOT_SYSTEM;
    return ALARM_ACTION_UNKNOWN;
}

static esp_err_t parse_config(const cJSON *config, alarms_manager_private_data_t *p_data)
{
    if (!config || !p_data) return ESP_ERR_INVALID_ARG;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (!config_node) {
        ESP_LOGW(TAG, "No 'config' object found. No alarms will be configured.");
        return ESP_OK;
    }

    const cJSON *alarms_array = cJSON_GetObjectItem(config_node, "alarms");
    if (!cJSON_IsArray(alarms_array)) {
        ESP_LOGW(TAG, "No 'alarms' array in config. No alarms will be configured.");
        return ESP_OK;
    }

    p_data->rule_count = 0;
    cJSON *rule_json;
    cJSON_ArrayForEach(rule_json, alarms_array) {
        if (p_data->rule_count >= MAX_ALARM_RULES) {
            ESP_LOGE(TAG, "Maximum number of alarm rules (%d) reached. Ignoring further rules.", MAX_ALARM_RULES);
            break;
        }
        alarm_rule_t *rule = &p_data->rules[p_data->rule_count];
        memset(rule, 0, sizeof(alarm_rule_t)); // Clear the rule structure

        cJSON *name = cJSON_GetObjectItem(rule_json, "name");
        if (cJSON_IsString(name)) strncpy(rule->name, name->valuestring, sizeof(rule->name) - 1);

        cJSON *trigger = cJSON_GetObjectItem(rule_json, "trigger_event");
        if (cJSON_IsString(trigger)) strncpy(rule->trigger_event, trigger->valuestring, sizeof(rule->trigger_event) - 1);

        cJSON *cond_key = cJSON_GetObjectItem(rule_json, "condition_payload_key");
        if (cJSON_IsString(cond_key)) strncpy(rule->condition_key, cond_key->valuestring, sizeof(rule->condition_key) - 1);

        cJSON *cond_val = cJSON_GetObjectItem(rule_json, "condition_payload_value");
        if (cJSON_IsString(cond_val)) strncpy(rule->condition_value, cond_val->valuestring, sizeof(rule->condition_value) - 1);

        cJSON *thresh = cJSON_GetObjectItem(rule_json, "threshold_count");
        rule->threshold_count = cJSON_IsNumber(thresh) ? thresh->valueint : 1;

        cJSON *window = cJSON_GetObjectItem(rule_json, "time_window_sec");
        rule->time_window_sec = cJSON_IsNumber(window) ? window->valueint : 0;

        const cJSON *actions_array = cJSON_GetObjectItem(rule_json, "actions");
        if (cJSON_IsArray(actions_array)) {
            rule->action_count = 0;
            cJSON *action_json;
            cJSON_ArrayForEach(action_json, actions_array) {
                if (rule->action_count >= MAX_ALARM_ACTIONS) {
                    ESP_LOGW(TAG, "Max actions for rule '%s' reached.", rule->name);
                    break;
                }
                alarm_action_t *action = &rule->actions[rule->action_count];
                memset(action, 0, sizeof(alarm_action_t));

                cJSON *type = cJSON_GetObjectItem(action_json, "type");
                if (cJSON_IsString(type)) {
                    action->type = action_str_to_type(type->valuestring);
                }

                cJSON *service = cJSON_GetObjectItem(action_json, "service_name");
                if (cJSON_IsString(service)) strncpy(action->service_name, service->valuestring, sizeof(action->service_name) - 1);

                switch (action->type) {
                    case ALARM_ACTION_LOG_CRITICAL:
                        {
                            cJSON *msg = cJSON_GetObjectItem(action_json, "message");
                            if (cJSON_IsString(msg)) strncpy(action->params.log_message, msg->valuestring, sizeof(action->params.log_message) - 1);
                        }
                        break;
                    case ALARM_ACTION_SET_LED:
                        {
                            cJSON *r = cJSON_GetObjectItem(action_json, "r");
                            cJSON *g = cJSON_GetObjectItem(action_json, "g");
                            cJSON *b = cJSON_GetObjectItem(action_json, "b");
                            if (cJSON_IsNumber(r)) action->params.led_color.r = r->valueint;
                            if (cJSON_IsNumber(g)) action->params.led_color.g = g->valueint;
                            if (cJSON_IsNumber(b)) action->params.led_color.b = b->valueint;
                        }
                        break;
                    case ALARM_ACTION_REBOOT_SYSTEM:
                        {
                            cJSON *delay = cJSON_GetObjectItem(action_json, "delay_ms");
                            action->params.reboot_params.delay_ms = cJSON_IsNumber(delay) ? delay->valueint : 500;
                        }
                        break;
                    default:
                        break;
                }
                rule->action_count++;
            }
        }
        p_data->rule_count++;
    }
    ESP_LOGI(TAG, "Parsed %d alarm rules from configuration.", p_data->rule_count);
    return ESP_OK;
}