/**
 * @file connectivity_watchdog.c
 * @brief Implementation of the universal Connectivity Watchdog module.
 * @author Synapse Framework Team
 * @version 2.1.0
 * @date 2025-07-13
 * @details This module implements a universal, multi-layered connectivity monitor.
 *          It supports both passive monitoring via generic heartbeat events
 *          (SYNAPSE_EVENT_CONNECTIVITY_ESTABLISHED) and active monitoring via PING checks.
 *          Recovery strategies are fully configurable through system_config.json.
 */

// --- Framework & System Includes ---
#include "synapse.h"
#include "connectivity_watchdog.h"

#include "timer_interface.h"
#include "system_manager_interface.h"
#include "alarms_manager_interface.h" // For triggering alarms

// --- ESP-IDF & Standard Lib Includes ---
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_ping.h"
#include "ping/ping_sock.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("CONN_WATCHDOG", SYNAPSE_LOG_COLOR_BLUE);

// --- Internal Event Names ---
#define EVT_CONN_WD_GRACE_PERIOD_END "CONN_WD_GRACE_PERIOD_END"
#define EVT_CONN_WD_CHECK_NOW        "CONN_WD_CHECK_NOW"

/** @internal @enum check_type_t Defines the type of connectivity check. */
typedef enum {
    CHECK_TYPE_UNKNOWN,
    CHECK_TYPE_HEARTBEAT,
    CHECK_TYPE_PING,
} check_type_t;

/** @internal @enum action_type_t Defines the type of recovery action. */
typedef enum {
    ACTION_TYPE_UNKNOWN,
    ACTION_TYPE_LOG_WARNING,
    ACTION_TYPE_RECONNECT_SERVICE,
    ACTION_TYPE_TRIGGER_ALARM,
    ACTION_TYPE_REBOOT_SYSTEM,
} action_type_t;

/** @internal @struct action_config_t Holds configuration for a single recovery action. */
typedef struct {
    uint8_t on_failure_count;
    action_type_t type;
    char param1[64]; // For message, service_name, or alarm_name
    uint32_t param2; // For delay_ms
} action_config_t;

/** @internal @struct check_config_t Holds configuration for a single monitored check. */
typedef struct {
    char name[32];
    check_type_t type;
    uint32_t timeout_sec;
    char target[64];
    action_config_t actions[CONFIG_CONNECTIVITY_WATCHDOG_MAX_ACTIONS_PER_CHECK];
    uint8_t num_actions;
} check_config_t;

/** @internal @struct check_state_t Holds the runtime state for a single monitored check. */
typedef struct {
    int64_t last_success_ts;
    uint8_t failure_count;
} check_state_t;

/** @internal @brief Private data structure for the Connectivity Watchdog module. */
typedef struct {
    char instance_name[CONFIG_CONNECTIVITY_WATCHDOG_INSTANCE_NAME_MAX_LEN];
    check_config_t checks[CONFIG_CONNECTIVITY_WATCHDOG_MAX_CHECKS];
    uint8_t num_checks;
    check_state_t states[CONFIG_CONNECTIVITY_WATCHDOG_MAX_CHECKS];
    bool is_in_grace_period;
    synapse_timer_handle_t grace_period_timer;
    synapse_timer_handle_t periodic_check_timer;
} connectivity_watchdog_private_data_t;

// --- Forward Declarations ---
static esp_err_t connectivity_watchdog_init(module_t *self);
static esp_err_t connectivity_watchdog_start(module_t *self);
static void connectivity_watchdog_deinit(module_t *self);
static void connectivity_watchdog_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t parse_watchdog_config(const cJSON *config, connectivity_watchdog_private_data_t *private_data);
static void check_all_monitors(module_t *self);
static void execute_recovery_action(const action_config_t *action);
static void perform_ping_check(check_config_t *check_cfg);
static void ping_check_cb(esp_ping_handle_t hdl, void *args);

module_t *connectivity_watchdog_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating Connectivity Watchdog module instance");
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    connectivity_watchdog_private_data_t *private_data = (connectivity_watchdog_private_data_t *)calloc(1, sizeof(connectivity_watchdog_private_data_t));
    if (!module || !private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        return NULL;
    }

    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex) {
        ESP_LOGE(TAG, "Failed to create state mutex");
        free(private_data);
        free(module);
        return NULL;
    }

    module->private_data = private_data;

    if (parse_watchdog_config(config, private_data) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to parse configuration. Aborting creation.");
        vSemaphoreDelete(module->state_mutex);
        free(private_data);
        free(module);
        return NULL;
    }
    
    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->init_level = 40;

    module->base.init = connectivity_watchdog_init;
    module->base.start = connectivity_watchdog_start;
    module->base.deinit = connectivity_watchdog_deinit;
    module->base.handle_event = connectivity_watchdog_handle_event;
    module->base.enable = NULL;
    module->base.disable = NULL;
    module->base.reconfigure = NULL;
    module->base.get_status = NULL;

    ESP_LOGI(TAG, "Connectivity Watchdog module created: '%s'", private_data->instance_name);
    return module;
}

static esp_err_t connectivity_watchdog_init(module_t *self)
{
    if (!self) return ESP_ERR_INVALID_ARG;
    connectivity_watchdog_private_data_t *private_data = (connectivity_watchdog_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing module: %s", self->name);

    private_data->is_in_grace_period = true;
    for (int i = 0; i < private_data->num_checks; i++) {
        private_data->states[i].last_success_ts = 0;
        private_data->states[i].failure_count = 0;
    }

    synapse_event_bus_subscribe(SYNAPSE_EVENT_CONNECTIVITY_ESTABLISHED, self);
    synapse_event_bus_subscribe(EVT_CONN_WD_GRACE_PERIOD_END, self);
    synapse_event_bus_subscribe(EVT_CONN_WD_CHECK_NOW, self);

    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "Module initialized successfully");
    return ESP_OK;
}

static esp_err_t connectivity_watchdog_start(module_t *self)
{
    if (!self) return ESP_ERR_INVALID_ARG;
    if (self->status != MODULE_STATUS_INITIALIZED) return ESP_ERR_INVALID_STATE;
    connectivity_watchdog_private_data_t *private_data = (connectivity_watchdog_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting module: %s", self->name);

    service_handle_t timer_service = synapse_service_get("main_timer_service");
    if (!timer_service) {
        ESP_LOGE(TAG, "System Timer service not found! Cannot start watchdog.");
        self->status = MODULE_STATUS_ERROR;
        return ESP_FAIL;
    }
    timer_api_t *timer_api = (timer_api_t *)timer_service;

    private_data->grace_period_timer = timer_api->schedule_event(EVT_CONN_WD_GRACE_PERIOD_END, CONFIG_CONNECTIVITY_WATCHDOG_GRACE_PERIOD_SEC * 1000, false);
    private_data->periodic_check_timer = timer_api->schedule_event(EVT_CONN_WD_CHECK_NOW, CONFIG_CONNECTIVITY_WATCHDOG_CHECK_INTERVAL_SEC * 1000, true);

    if (!private_data->grace_period_timer || !private_data->periodic_check_timer) {
        ESP_LOGE(TAG, "Failed to schedule watchdog timers.");
        if (private_data->grace_period_timer) timer_api->cancel_event(private_data->grace_period_timer);
        if (private_data->periodic_check_timer) timer_api->cancel_event(private_data->periodic_check_timer);
        self->status = MODULE_STATUS_ERROR;
        return ESP_FAIL;
    }

    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "Module started. Grace period: %d s, Check interval: %d s", CONFIG_CONNECTIVITY_WATCHDOG_GRACE_PERIOD_SEC, CONFIG_CONNECTIVITY_WATCHDOG_CHECK_INTERVAL_SEC);
    return ESP_OK;
}

static void connectivity_watchdog_deinit(module_t *self)
{
    if (!self) return;
    ESP_LOGI(TAG, "Deinitializing %s module", self->name);
    connectivity_watchdog_private_data_t *private_data = (connectivity_watchdog_private_data_t *)self->private_data;

    service_handle_t timer_service = synapse_service_get("main_timer_service");
    if (timer_service) {
        timer_api_t *timer_api = (timer_api_t *)timer_service;
        if (private_data->grace_period_timer) timer_api->cancel_event(private_data->grace_period_timer);
        if (private_data->periodic_check_timer) timer_api->cancel_event(private_data->periodic_check_timer);
    }

    synapse_event_bus_unsubscribe(SYNAPSE_EVENT_CONNECTIVITY_ESTABLISHED, self);
    synapse_event_bus_unsubscribe(EVT_CONN_WD_GRACE_PERIOD_END, self);
    synapse_event_bus_unsubscribe(EVT_CONN_WD_CHECK_NOW, self);

    if (self->private_data) free(self->private_data);
    if (self->current_config) cJSON_Delete(self->current_config);
    if (self->state_mutex) vSemaphoreDelete(self->state_mutex);

    ESP_LOGI(TAG, "Module deinitialized successfully");
}

static void connectivity_watchdog_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (!self || !event_name) {
        if (event_data)
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }
    connectivity_watchdog_private_data_t *private_data = (connectivity_watchdog_private_data_t *)self->private_data;
    ESP_LOGD(TAG, "Event received: '%s'", event_name);

    if (strcmp(event_name, SYNAPSE_EVENT_CONNECTIVITY_ESTABLISHED) == 0)
    {
        if (event_data) {
            event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;
            synapse_connectivity_payload_t *payload = (synapse_connectivity_payload_t *)wrapper->payload;
            for (int i = 0; i < private_data->num_checks; i++) {
                if (strcmp(private_data->checks[i].name, payload->check_name) == 0) {
                    private_data->states[i].last_success_ts = esp_timer_get_time();
                    if (private_data->states[i].failure_count > 0) {
                        ESP_LOGI(TAG, "Connectivity for '%s' recovered. Resetting failure count.", payload->check_name);
                        private_data->states[i].failure_count = 0;
                    }
                    break;
                }
            }
        }
    }
    else if (strcmp(event_name, EVT_CONN_WD_GRACE_PERIOD_END) == 0)
    {
        private_data->is_in_grace_period = false;
        ESP_LOGI(TAG, "Grace period ended. Connectivity monitoring is now active.");
    }
    else if (strcmp(event_name, EVT_CONN_WD_CHECK_NOW) == 0)
    {
        if (!private_data->is_in_grace_period) {
            check_all_monitors(self);
        }
    }

    if (event_data) {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

static void check_all_monitors(module_t *self)
{
    connectivity_watchdog_private_data_t *private_data = (connectivity_watchdog_private_data_t *)self->private_data;
    int64_t now_us = esp_timer_get_time();

    for (int i = 0; i < private_data->num_checks; i++) {
        check_config_t *check_cfg = &private_data->checks[i];
        check_state_t *check_state = &private_data->states[i];

        if (check_cfg->type == CHECK_TYPE_PING) {
            perform_ping_check(check_cfg);
        }

        int64_t time_since_last_success_sec = (now_us - check_state->last_success_ts) / 1000000;

        if (time_since_last_success_sec > check_cfg->timeout_sec) {
            check_state->failure_count++;
            ESP_LOGW(TAG, "Check '%s' TIMEOUT! Last success was %llds ago. Failures: %d",
                     check_cfg->name, time_since_last_success_sec, check_state->failure_count);

            for (int j = 0; j < check_cfg->num_actions; j++) {
                if (check_state->failure_count == check_cfg->actions[j].on_failure_count) {
                    execute_recovery_action(&check_cfg->actions[j]);
                }
            }
        } else {
             ESP_LOGI(TAG, "Check '%s' OK (last seen %llds ago).", check_cfg->name, time_since_last_success_sec);
        }
    }
}

/**
 * @internal
 * @brief Executes a recovery action based on the current failure state and configuration.
 * @param[in] action A pointer to the action configuration to execute.
 */
static void execute_recovery_action(const action_config_t *action)
{
    ESP_LOGI(TAG, "Executing recovery action for failure count %d", action->on_failure_count);
    switch (action->type) {
        case ACTION_TYPE_LOG_WARNING:
            ESP_LOGW(TAG, "Recovery Action: %s", action->param1);
            break;

        case ACTION_TYPE_RECONNECT_SERVICE:
            {
                // This is a conceptual implementation. It posts a generic event
                // that a specific module (like wifi_manager) could listen for.
                ESP_LOGI(TAG, "Posting reconnect request for service: %s", action->param1);
                // Example: synapse_event_bus_post("SYNAPSE_RECONNECT_REQUEST", ...payload with service name...);
            }
            break;

        case ACTION_TYPE_TRIGGER_ALARM:
            {
                // This is a conceptual implementation. The Alarms Manager currently
                // does not have a service to trigger alarms manually. This action
                // would require extending the Alarms Manager API.
                ESP_LOGW(TAG, "Triggering alarm: %s", action->param1);
                // Example:
                // service_handle_t alarm_handle = synapse_service_get("main_alarms_manager");
                // if (alarm_handle) {
                //     alarms_manager_api_t *alarm_api = (alarms_manager_api_t *)alarm_handle;
                //     alarm_api->trigger_manual_alarm(action->param1, NULL);
                // }
            }
            break;

        case ACTION_TYPE_REBOOT_SYSTEM:
            {
                ESP_LOGE(TAG, "Recovery Action: Rebooting system in %lums...", action->param2);
                service_handle_t sys_handle = synapse_service_get("system_manager");
                if (sys_handle) {
                    system_manager_api_t *sys_api = (system_manager_api_t *)sys_handle;
                    // A small delay to ensure the log message is sent
                    vTaskDelay(pdMS_TO_TICKS(100));
                    sys_api->reboot_system();
                } else {
                    ESP_LOGE(TAG, "Could not get System Manager service! Rebooting directly.");
                    vTaskDelay(pdMS_TO_TICKS(100));
                    esp_restart();
                }
            }
            break;

        default:
            ESP_LOGE(TAG, "Unknown or unsupported recovery action type!");
            break;
    }
}

static void perform_ping_check(check_config_t *check_cfg)
{
    esp_ping_config_t ping_config = ESP_PING_DEFAULT_CONFIG();
    ip_addr_t target_addr;
    if (ipaddr_aton(check_cfg->target, &target_addr)) {
        ping_config.target_addr = target_addr;
        ping_config.count = 1;
        ping_config.timeout_ms = 2000;

        esp_ping_callbacks_t cbs = { .on_ping_success = ping_check_cb, .on_ping_timeout = ping_check_cb, .on_ping_end = ping_check_cb, .cb_args = check_cfg };
        esp_ping_handle_t ping;
        esp_ping_new_session(&ping_config, &cbs, &ping);
        esp_ping_start(ping);
        ESP_LOGI(TAG, "Starting PING check for '%s' to target %s", check_cfg->name, check_cfg->target);
    } else {
        ESP_LOGE(TAG, "Invalid PING target IP address: %s", check_cfg->target);
    }
}

static void ping_check_cb(esp_ping_handle_t hdl, void *args)
{
    check_config_t *check_cfg = (check_config_t *)args;
    uint32_t transmitted, received;
    esp_ping_get_profile(hdl, ESP_PING_PROF_REQUEST, &transmitted, sizeof(transmitted));
    esp_ping_get_profile(hdl, ESP_PING_PROF_REPLY, &received, sizeof(received));

    if (received > 0) {
        ESP_LOGI(TAG, "PING check '%s' successful.", check_cfg->name);
        synapse_connectivity_payload_t *payload = malloc(sizeof(synapse_connectivity_payload_t));
        if(payload) {
            strncpy(payload->check_name, check_cfg->name, sizeof(payload->check_name) - 1);
            
            event_data_wrapper_t *wrapper;
            if (synapse_event_data_wrap(payload, free, &wrapper) == ESP_OK)
            {
                synapse_event_bus_post(SYNAPSE_EVENT_CONNECTIVITY_ESTABLISHED, wrapper);
                synapse_event_data_release(wrapper);
            }
            else
            {
                free(payload); // Cleanup if wrap fails
            }
        }
    }else {
        ESP_LOGW(TAG, "PING check '%s' failed (timeout).", check_cfg->name);
    }
    esp_ping_delete_session(hdl);
}

/**
 * @internal
 * @brief Parses the module's configuration from a cJSON object.
 *
 * @details This function reads the "config" object from the main JSON configuration,
 *          extracts the instance name, and then iterates through the "layers" array
 *          to populate the internal configuration structures.
 *
 * @param[in] config The root cJSON object for the module from system_config.json.
 * @param[out] private_data A pointer to the module's private data structure to be filled.
 * @return ESP_OK on successful parsing, or ESP_ERR_INVALID_ARG if essential fields are missing.
 */
static esp_err_t parse_watchdog_config(const cJSON *config, connectivity_watchdog_private_data_t *private_data)
{
    if (!private_data) return ESP_ERR_INVALID_ARG;
    
    strncpy(private_data->instance_name, CONFIG_CONNECTIVITY_WATCHDOG_DEFAULT_INSTANCE_NAME, sizeof(private_data->instance_name) - 1);

    if (!config) {
        ESP_LOGW(TAG, "No JSON config provided, using all defaults from Kconfig.");
        return ESP_OK;
    }

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (!cJSON_IsObject(config_node)) {
        ESP_LOGW(TAG, "No 'config' object in JSON, using all defaults.");
        return ESP_OK;
    }

    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    if (cJSON_IsString(name_node)) {
        strncpy(private_data->instance_name, name_node->valuestring, sizeof(private_data->instance_name) - 1);
    }

    const cJSON *checks_array = cJSON_GetObjectItem(config_node, "layers");
    if (!cJSON_IsArray(checks_array)) {
        ESP_LOGE(TAG, "'layers' array is missing or invalid in config. Watchdog will be inactive.");
        private_data->num_checks = 0;
        return ESP_OK;
    }

    private_data->num_checks = cJSON_GetArraySize(checks_array);
    if (private_data->num_checks > CONFIG_CONNECTIVITY_WATCHDOG_MAX_CHECKS) {
        ESP_LOGW(TAG, "Number of checks (%d) exceeds max (%d). Truncating.", private_data->num_checks, CONFIG_CONNECTIVITY_WATCHDOG_MAX_CHECKS);
        private_data->num_checks = CONFIG_CONNECTIVITY_WATCHDOG_MAX_CHECKS;
    }

    cJSON *check_item;
    int valid_checks_count = 0;
    cJSON_ArrayForEach(check_item, checks_array) {
        if (valid_checks_count >= private_data->num_checks) break;
        check_config_t *check_cfg = &private_data->checks[valid_checks_count];

        cJSON *type = cJSON_GetObjectItem(check_item, "type");
        cJSON *timeout = cJSON_GetObjectItem(check_item, "timeout_sec");
        if (!cJSON_IsString(type) || !cJSON_IsNumber(timeout)) {
            ESP_LOGE(TAG, "Skipping invalid check at index %d (missing type or timeout).", valid_checks_count);
            continue;
        }
        
        if (strcmp(type->valuestring, "MQTT") == 0) {
            check_cfg->type = CHECK_TYPE_HEARTBEAT;
            snprintf(check_cfg->name, sizeof(check_cfg->name), "MQTT_Heartbeat");
        } else if (strcmp(type->valuestring, "PING") == 0) {
            check_cfg->type = CHECK_TYPE_PING;
            cJSON *target_json = cJSON_GetObjectItem(check_item, "target");
            if (cJSON_IsString(target_json) && target_json->valuestring) {
                // --- FIX START ---
                // 1. შევინახოთ target-ის მნიშვნელობა დროებით ლოკალურ ცვლადში.
                char local_target_buffer[sizeof(check_cfg->target)];
                strncpy(local_target_buffer, target_json->valuestring, sizeof(local_target_buffer) - 1);
                local_target_buffer[sizeof(local_target_buffer) - 1] = '\0';

                // 2. დავაკოპიროთ ის ჩვენს სტრუქტურაში.
                strncpy(check_cfg->target, local_target_buffer, sizeof(check_cfg->target) - 1);

                // 3. გამოვიყენოთ ლოკალური ცვლადი snprintf-ისთვის.
                int max_ip_len = sizeof(check_cfg->name) - strlen("PING_") - 1;
                snprintf(check_cfg->name, sizeof(check_cfg->name), "PING_%.*s", max_ip_len, local_target_buffer);
                // --- FIX END ---

            } else {
                ESP_LOGE(TAG, "PING check is missing a 'target' IP address. Skipping.");
                continue;
            }
        } else {
            ESP_LOGW(TAG, "Unknown check type '%s'. Skipping.", type->valuestring);
            continue;
        }
        
        check_cfg->timeout_sec = timeout->valueint;

        cJSON *actions_array = cJSON_GetObjectItem(check_item, "actions");
        if (cJSON_IsArray(actions_array)) {
            check_cfg->num_actions = cJSON_GetArraySize(actions_array);
            if (check_cfg->num_actions > CONFIG_CONNECTIVITY_WATCHDOG_MAX_ACTIONS_PER_CHECK) {
                check_cfg->num_actions = CONFIG_CONNECTIVITY_WATCHDOG_MAX_ACTIONS_PER_CHECK;
            }
            
            cJSON *action_item;
            int valid_actions_count = 0;
            cJSON_ArrayForEach(action_item, actions_array) {
                if (valid_actions_count >= check_cfg->num_actions) break;
                action_config_t *action_cfg = &check_cfg->actions[valid_actions_count];
                
                cJSON *on_failure = cJSON_GetObjectItem(action_item, "on_failure_count");
                cJSON *do_action = cJSON_GetObjectItem(action_item, "do");
                if (!cJSON_IsNumber(on_failure) || !cJSON_IsString(do_action)) continue;

                action_cfg->on_failure_count = on_failure->valueint;
                
                if (strcmp(do_action->valuestring, "log_warning") == 0) {
                    action_cfg->type = ACTION_TYPE_LOG_WARNING;
                    cJSON *msg = cJSON_GetObjectItem(action_item, "message");
                    if (cJSON_IsString(msg)) strncpy(action_cfg->param1, msg->valuestring, sizeof(action_cfg->param1) - 1);
                } else if (strcmp(do_action->valuestring, "reboot_system") == 0) {
                    action_cfg->type = ACTION_TYPE_REBOOT_SYSTEM;
                    cJSON *delay = cJSON_GetObjectItem(action_item, "delay_ms");
                    action_cfg->param2 = cJSON_IsNumber(delay) ? delay->valueint : 1000;
                } else if (strcmp(do_action->valuestring, "reconnect_mqtt") == 0 || strcmp(do_action->valuestring, "reconnect_wifi") == 0) {
                    action_cfg->type = ACTION_TYPE_RECONNECT_SERVICE;
                    strncpy(action_cfg->param1, do_action->valuestring, sizeof(action_cfg->param1) - 1);
                } else if (strcmp(do_action->valuestring, "trigger_alarm") == 0) {
                    action_cfg->type = ACTION_TYPE_TRIGGER_ALARM;
                    cJSON *alarm_name = cJSON_GetObjectItem(action_item, "alarm_name");
                    if (cJSON_IsString(alarm_name)) strncpy(action_cfg->param1, alarm_name->valuestring, sizeof(action_cfg->param1) - 1);
                } else {
                    action_cfg->type = ACTION_TYPE_UNKNOWN;
                }

                if (action_cfg->type != ACTION_TYPE_UNKNOWN) {
                    valid_actions_count++;
                }
            }
            check_cfg->num_actions = valid_actions_count;
        }
        valid_checks_count++;
    }
    private_data->num_checks = valid_checks_count;
    ESP_LOGI(TAG, "Successfully parsed %d connectivity checks from config.", private_data->num_checks);
    return ESP_OK;
}