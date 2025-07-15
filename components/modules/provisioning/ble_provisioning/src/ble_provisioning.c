/**
 * @file ble_provisioning.c
 * @brief ESP-IDF native BLE provisioning implementation with a Service API.
 * @author Synapse Framework Team
 * @version 2.3.1 (Final Build Fix & Full Documentation)
 * @date 2025-07-05
 * @details This module implements BLE provisioning using the ESP-IDF `wifi_provisioning`
 *          manager. It provides a service to check the provisioning status and
 *          handles system events to start/stop the process automatically. It can be
 *          configured via Kconfig and overridden by `system_config.json`.
 */

// --- Synapse Framework Includes ---
#include "ble_provisioning.h"
#include "ble_prov_interface.h"
#include "base_module.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "service_locator.h"
#include "service_types.h"
#include "framework_config.h"
#include "logging.h"

// --- Standard & ESP-IDF Includes ---
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include <wifi_provisioning/manager.h>
#include <wifi_provisioning/scheme_ble.h>
#include "esp_bt.h"

// --- Component Tag ---
DEFINE_COMPONENT_TAG("BLE_PROV");

// --- Event Names ---
#define EVT_PROV_CREDENTIALS_RECEIVED "PROV_CREDENTIALS_RECEIVED"
#define EVT_WIFI_IP_ASSIGNED "WIFI_EVENT_IP_ASSIGNED"
#define EVT_PROV_STARTED "PROV_STARTED"
#define EVT_PROV_ENDED "PROV_ENDED"
#define EVT_PROV_START_REQUESTED "PROV_START_REQUESTED"

// --- Private Data Structure ---

/**
 * @internal
 * @struct ble_provisioning_private_data_t
 * @brief Private data for the BLE Provisioning module.
 */
typedef struct
{
    char instance_name[CONFIG_BLE_PROVISIONING_INSTANCE_NAME_MAX_LEN];
    bool is_active;       /**< True if the provisioning process is currently running. */
    char device_name[32]; /**< BLE device name for advertising. */
    char pop[9];          /**< Proof-of-Possession string for secure provisioning. */
    int security_version; /**< Security version (0 or 1). */
} ble_provisioning_private_data_t;

// --- Forward Declarations ---
static esp_err_t ble_provisioning_init(module_t *self);
static esp_err_t ble_provisioning_start(module_t *self);
static void ble_provisioning_deinit(module_t *self);
static void ble_provisioning_handle_event(module_t *self, const char *event_name, void *event_data);
static void prov_event_handler(void *handler_arg, wifi_prov_cb_event_t event, void *event_data);

static bool api_is_provisioning_active(void);
static esp_err_t api_start_provisioning(void);

// --- Global Static Variables ---
static module_t *global_ble_prov_instance = NULL;

static ble_prov_api_t ble_prov_service_api = {
    .is_provisioning_active = api_is_provisioning_active,
    .start_provisioning = api_start_provisioning,
};

// =========================================================================
//                      Module Lifecycle Implementation
// =========================================================================

/**
 * @brief Creates a new instance of the BLE Provisioning module.
 * @details This function allocates memory for the module and its private data,
 *          sets the instance name and other parameters based on Kconfig defaults,
 *          and then overrides them with any values provided in the JSON config.
 * @param[in] config A cJSON object containing the module's configuration.
 * @return A pointer to the created module (`module_t*`), or NULL on failure.
 */
module_t *ble_provisioning_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating ble_provisioning module instance...");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for module_t.");
        return NULL;
    }

    ble_provisioning_private_data_t *private_data = (ble_provisioning_private_data_t *)calloc(1, sizeof(ble_provisioning_private_data_t));
    if (!private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for private_data.");
        free(module);
        return NULL;
    }

    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex)
    {
        ESP_LOGE(TAG, "Failed to create state mutex.");
        free(private_data);
        free(module);
        return NULL;
    }

    module->private_data = private_data;

    // 1. Set default configuration from Kconfig
    strncpy(private_data->device_name, CONFIG_ESP_PROV_DEVICE_NAME, sizeof(private_data->device_name) - 1);
    strncpy(private_data->pop, CONFIG_ESP_PROV_POP, sizeof(private_data->pop) - 1);
    private_data->security_version = CONFIG_ESP_PROV_SECURITY_VERSION;
    const char *instance_name = CONFIG_BLE_PROVISIONING_DEFAULT_INSTANCE_NAME;

    // 2. Override with values from JSON config if provided
    if (config)
    {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node))
        {
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node) && name_node->valuestring)
            {
                instance_name = name_node->valuestring;
            }

            const cJSON *device_name_node = cJSON_GetObjectItem(config_node, "device_name");
            if (cJSON_IsString(device_name_node) && device_name_node->valuestring)
            {
                strncpy(private_data->device_name, device_name_node->valuestring, sizeof(private_data->device_name) - 1);
            }

            const cJSON *pop_node = cJSON_GetObjectItem(config_node, "pop");
            if (cJSON_IsString(pop_node) && pop_node->valuestring)
            {
                strncpy(private_data->pop, pop_node->valuestring, sizeof(private_data->pop) - 1);
            }

            const cJSON *sec_ver_node = cJSON_GetObjectItem(config_node, "security_version");
            if (cJSON_IsNumber(sec_ver_node) && (sec_ver_node->valueint == 0 || sec_ver_node->valueint == 1))
            {
                private_data->security_version = sec_ver_node->valueint;
            }
        }
        module->current_config = cJSON_Duplicate(config, true);
    }

    // 3. Finalize module setup
    strncpy(private_data->instance_name, instance_name, sizeof(private_data->instance_name) - 1);
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;

    module->init_level = 2;

    module->base.init = ble_provisioning_init;
    module->base.start = ble_provisioning_start;
    module->base.deinit = ble_provisioning_deinit;
    module->base.handle_event = ble_provisioning_handle_event;

    global_ble_prov_instance = module;
    ESP_LOGI(TAG, "BLE Provisioning module '%s' created.", module->name);
    return module;
}

/**
 * @internal
 * @brief Initializes the module, registers its service and subscribes to events.
 */
static esp_err_t ble_provisioning_init(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    ESP_LOGI(TAG, "Initializing ble_provisioning module: %s", self->name);

    esp_err_t err = fmw_service_register(self->name, FMW_SERVICE_TYPE_CUSTOM_API, &ble_prov_service_api);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register BLE Provisioning service: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "BLE Provisioning service registered successfully.");

    fmw_event_bus_subscribe(EVT_WIFI_IP_ASSIGNED, self);
    fmw_event_bus_subscribe(EVT_PROV_START_REQUESTED, self);

    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

/**
 * @internal
 * @brief Starts the module's main logic.
 * @details Checks if the device is already provisioned. If not, it starts the
 *          BLE provisioning process automatically.
 */
static esp_err_t ble_provisioning_start(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;

    ESP_LOGI(TAG, "Starting ble_provisioning module: %s", self->name);

    bool provisioned = false;
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));

    if (provisioned)
    {
        ESP_LOGI(TAG, "Device is already provisioned. Skipping auto-start of provisioning.");
    }
    else
    {
        ESP_LOGI(TAG, "Device not provisioned. Starting BLE provisioning automatically...");
        api_start_provisioning();
    }

    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

/**
 * @internal
 * @brief Deinitializes the module, unregistering services and freeing resources.
 */
static void ble_provisioning_deinit(module_t *self)
{
    if (!self)
        return;
    ble_provisioning_private_data_t *private_data = (ble_provisioning_private_data_t *)self->private_data;

    fmw_service_unregister(self->name);
    fmw_event_bus_unsubscribe(EVT_WIFI_IP_ASSIGNED, self);
    fmw_event_bus_unsubscribe(EVT_PROV_START_REQUESTED, self);

    if (private_data && private_data->is_active)
    {
        wifi_prov_mgr_deinit();
    }

    global_ble_prov_instance = NULL;

    if (self->private_data)
        free(self->private_data);
    if (self->current_config)
        cJSON_Delete(self->current_config);
    if (self->state_mutex)
        vSemaphoreDelete(self->state_mutex);
    free(self);

    ESP_LOGI(TAG, "BLE Provisioning module deinitialized.");
}

// =========================================================================
//                      Event Handlers
// =========================================================================

/**
 * @internal
 * @brief Handles events from the Synapse Event Bus.
 * @details Reacts to system-wide events to control the provisioning lifecycle.
 * @param self Pointer to the module instance.
 * @param event_name The name of the received event.
 * @param event_data Pointer to the event data wrapper.
 */
static void ble_provisioning_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (!self || !self->private_data)
    {
        if (event_data)
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }
    ble_provisioning_private_data_t *private_data = (ble_provisioning_private_data_t *)self->private_data;

    if (strcmp(event_name, EVT_PROV_START_REQUESTED) == 0)
    {
        ESP_LOGI(TAG, "Provisioning start requested via event.");
        api_start_provisioning();
    }
    else if (strcmp(event_name, EVT_WIFI_IP_ASSIGNED) == 0)
    {
        ESP_LOGI(TAG, "WiFi connected with IP. Stopping provisioning if active.");
        if (private_data->is_active)
        {
            // The provisioning manager will automatically send a success message
            // to the client and then generate a WIFI_PROV_END event.
            // We don't need to call stop or send_success manually.
            // The manager handles this internally upon receiving IP_EVENT_STA_GOT_IP.
            // Forcing a stop here can sometimes interfere.
            // Let's rely on the internal mechanism.
        }
    }

    if (event_data)
    {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

/**
 * @internal
 * @brief Handles events from the ESP-IDF WiFi Provisioning Manager.
 * @details This is a callback function, not a standard event loop handler.
 *          It directly receives provisioning-specific events.
 * @param handler_arg User data, a pointer to our module_t instance.
 * @param event The specific provisioning event that occurred.
 * @param event_data Data associated with the event.
 */
static void prov_event_handler(void *handler_arg, wifi_prov_cb_event_t event, void *event_data)
{
    module_t *self = (module_t *)handler_arg;
    if (!self || !self->private_data)
        return;

    ble_provisioning_private_data_t *private_data = (ble_provisioning_private_data_t *)self->private_data;

    switch (event)
    {
    case WIFI_PROV_START:
        ESP_LOGI(TAG, "Provisioning process started.");
        private_data->is_active = true;
        fmw_event_bus_post(EVT_PROV_STARTED, NULL);
        break;

    case WIFI_PROV_CRED_RECV:
    {
        wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
        ESP_LOGI(TAG, "Received Wi-Fi credentials for SSID: %s", (char *)wifi_sta_cfg->ssid);

        cJSON *creds = cJSON_CreateObject();
        if (!creds)
            break;

        cJSON_AddStringToObject(creds, "ssid", (char *)wifi_sta_cfg->ssid);
        cJSON_AddStringToObject(creds, "password", (char *)wifi_sta_cfg->password);
        char *json_str = cJSON_PrintUnformatted(creds);
        cJSON_Delete(creds);

        if (json_str)
        {
            event_data_wrapper_t *wrapper = NULL;
            if (fmw_event_data_wrap(json_str, free, &wrapper) == ESP_OK)
            {
                fmw_event_bus_post(EVT_PROV_CREDENTIALS_RECEIVED, wrapper);
            }
            else
            {
                free(json_str);
            }
        }
        break;
    }

    case WIFI_PROV_CRED_SUCCESS:
        ESP_LOGI(TAG, "Provisioning successful. Device will now connect to the network.");
        break;

    case WIFI_PROV_CRED_FAIL:
    {
        wifi_prov_sta_fail_reason_t *reason = (wifi_prov_sta_fail_reason_t *)event_data;
        ESP_LOGE(TAG, "Provisioning failed! Reason: %s",
                 (*reason == WIFI_PROV_STA_AUTH_ERROR) ? "Authentication Error" : "AP Not Found");
        private_data->is_active = false; // Stop trying on failure
        break;
    }

    case WIFI_PROV_END:
        ESP_LOGI(TAG, "Provisioning process ended.");
        private_data->is_active = false;

        // De-initialize the provisioning manager
        wifi_prov_mgr_deinit();

        // ★★★ THE FIX: Use the high-level public API to release all BT memory ★★★
        ESP_LOGI(TAG, "Releasing all Bluetooth memory for mode BLE...");
        esp_bt_mem_release(ESP_BT_MODE_BLE);

        fmw_event_bus_post(EVT_PROV_ENDED, NULL);
        break;

    default:
        break;
    }
}

// =========================================================================
//                      Service API Implementation
// =========================================================================

/**
 * @internal
 * @brief Implements the is_provisioning_active API function.
 * @see ble_prov_interface.h
 */
static bool api_is_provisioning_active(void)
{
    if (!global_ble_prov_instance || !global_ble_prov_instance->private_data)
    {
        return false;
    }
    ble_provisioning_private_data_t *private_data = (ble_provisioning_private_data_t *)global_ble_prov_instance->private_data;
    return private_data->is_active;
}

/**
 * @internal
 * @brief Implements the start_provisioning API function.
 * @details This is the central function for initiating the provisioning process.
 *          It de-initializes any previous provisioning instance to ensure a
 *          clean state, then configures and starts the provisioning manager.
 * @see ble_prov_interface.h
 */
static esp_err_t api_start_provisioning(void)
{
    if (!global_ble_prov_instance || !global_ble_prov_instance->private_data)
    {
        return ESP_ERR_INVALID_STATE;
    }
    ble_provisioning_private_data_t *private_data = (ble_provisioning_private_data_t *)global_ble_prov_instance->private_data;

    if (private_data->is_active)
    {
        ESP_LOGW(TAG, "Start provisioning called, but it is already active.");
        return ESP_ERR_INVALID_STATE;
    }

    wifi_prov_mgr_deinit();

    wifi_prov_mgr_config_t config = {
        .scheme = wifi_prov_scheme_ble,
        .scheme_event_handler = WIFI_PROV_SCHEME_BLE_EVENT_HANDLER_FREE_BTDM,
        .app_event_handler = {
            .event_cb = prov_event_handler,
            .user_data = global_ble_prov_instance}};
    esp_err_t err = wifi_prov_mgr_init(config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize provisioning manager: %s", esp_err_to_name(err));
        return err;
    }

    const char *service_name = private_data->device_name;
    const char *pop = (private_data->security_version == 1) ? private_data->pop : NULL;
    wifi_prov_security_t security = (private_data->security_version == 0) ? WIFI_PROV_SECURITY_0 : WIFI_PROV_SECURITY_1;

    ESP_LOGI(TAG, "Starting provisioning with: Device Name='%s', Security=v%d", service_name, private_data->security_version);

    err = wifi_prov_mgr_start_provisioning(security, pop, service_name, NULL);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start provisioning: %s", esp_err_to_name(err));
        wifi_prov_mgr_deinit();
    }

    return err;
}