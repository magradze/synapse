/**
 * @file storage_manager.c
 * @brief Centralized, multi-backend storage management service.
 * @author Synapse Framework Team
 * @version 2.0.0
 * @date 2025-07-05
 * @details This module acts as a dispatcher for the Synapse Framework's storage
 *          operations. Based on the Kconfig selection, it initializes the
 *          appropriate storage backend (NVS, SPIFFS, or SD Card) and registers
 *          its API with the Service Locator. This provides a unified, abstract
 * "storage_interface.h"
 *          interface to all other modules, decoupling them from the underlying
 *          storage technology.
 */

// --- Core Synapse Includes ---
#include "synapse.h"
#include "storage_manager.h"
#include "storage_interface.h"

// --- Standard Library Includes ---
#include <string.h>
#include <stdlib.h>

// --- Conditional Backend Includes ---
// The build system will only include the header for the selected backend.
#if CONFIG_STORAGE_BACKEND_NVS
#include "storage_backend_nvs.h"
#endif
#if CONFIG_STORAGE_BACKEND_SPIFFS
#include "storage_backend_spiffs.h"
#endif
#if CONFIG_STORAGE_BACKEND_SD_CARD
#include "storage_backend_sd.h"
#endif

// --- Component Tag ---
DEFINE_COMPONENT_TAG("STORAGE_MANAGER", SYNAPSE_LOG_COLOR_YELLOW);

// --- Private Data Structure ---

/**
 * @internal
 * @struct storage_manager_private_data_t
 * @brief Private data for the Storage Manager module.
 * @details This structure holds the module's instance name and a pointer to the
 *          active storage API implementation, which is determined at compile time.
 */
typedef struct {
    char instance_name[CONFIG_STORAGE_MANAGER_INSTANCE_NAME_MAX_LEN]; /**< The unique instance name of the module. */
    storage_api_t active_storage_api; /**< A copy of the function pointers for the selected backend. */
} storage_manager_private_data_t;

// --- Forward Declarations for Lifecycle Functions ---
static esp_err_t storage_manager_init(module_t *self);
static void storage_manager_deinit(module_t *self);

// =========================================================================
//                      Module Lifecycle Implementation
// =========================================================================

module_t *storage_manager_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating storage_manager module instance...");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    storage_manager_private_data_t *private_data = (storage_manager_private_data_t *)calloc(1, sizeof(storage_manager_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for module structures.");
        free(private_data);
        free(module);
        return NULL;
    }

    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex) {
        ESP_LOGE(TAG, "Failed to create state mutex.");
        free(private_data);
        free(module);
        return NULL;
    }
    
    module->private_data = private_data;
    
    const char *instance_name = CONFIG_STORAGE_MANAGER_DEFAULT_INSTANCE_NAME;
    if (config) {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node)) {
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node) && name_node->valuestring) {
                instance_name = name_node->valuestring;
            }
        }
        module->current_config = cJSON_Duplicate(config, true);
    }
    
    strncpy(private_data->instance_name, instance_name, sizeof(private_data->instance_name) - 1);
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->init_level = 20;

    module->base.init = storage_manager_init;
    module->base.deinit = storage_manager_deinit;
    module->base.start = NULL;
    module->base.handle_event = NULL;
    module->base.enable = NULL;
    module->base.disable = NULL;
    module->base.reconfigure = NULL;
    module->base.get_status = NULL;

    // --- Service Registration Moved to Create Phase ---
    // Note: We register the POINTER to the API table in private_data.
    // The actual API table will be populated by the backend during the init phase.
    esp_err_t ret = synapse_service_register_with_status(
        module->name,
        SYNAPSE_SERVICE_TYPE_NVRAM_API, // This should probably be a more generic type
        &private_data->active_storage_api,
        SERVICE_STATUS_REGISTERED);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to register service for '%s' (%s). Cleaning up.", module->name, esp_err_to_name(ret));
        storage_manager_deinit(module);
        return NULL;
    }

    ESP_LOGI(TAG, "Storage Manager module '%s' created and service registered.", module->name);
    return module;
}

static esp_err_t storage_manager_init(module_t *self)
{
    if (!self || !self->private_data) {
        return ESP_ERR_INVALID_ARG;
    }
    
    storage_manager_private_data_t *private_data = (storage_manager_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing Storage Manager: %s", self->name);
    
    esp_err_t err = ESP_FAIL;

    // --- Backend Dispatcher Logic ---
    #if CONFIG_STORAGE_BACKEND_NVS
        ESP_LOGI(TAG, "Selected backend: NVS");
        err = storage_backend_nvs_init(&private_data->active_storage_api);
    #elif CONFIG_STORAGE_BACKEND_SPIFFS
        ESP_LOGI(TAG, "Selected backend: SPIFFS");
        err = storage_backend_spiffs_init(&private_data->active_storage_api);
    #elif CONFIG_STORAGE_BACKEND_SD_CARD
        ESP_LOGI(TAG, "Selected backend: SD Card");
        err = storage_backend_sd_init(&private_data->active_storage_api);
    #else
#error "No storage backend selected in Kconfig! Please enable one."
#endif

        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to initialize the selected storage backend.");
            self->status = MODULE_STATUS_ERROR;
            return err;
        }

        // Service is now registered in the _create function.
        // The pointer registered there now points to a valid, initialized API table.

        self->status = MODULE_STATUS_RUNNING;
        ESP_LOGI(TAG, "Storage Manager initialized successfully.");
        return ESP_OK;
}

/**
 * @internal
 * @brief Deinitializes the module and its active backend.
 * @details This function unregisters the service, calls the deinitialization
 *          function for the active backend (if it exists), and frees all
 *          allocated resources for the module.
 * @param[in] self Pointer to the module instance to be deinitialized.
 */
static void storage_manager_deinit(module_t *self)
{
    if (!self) {
        return;
    }
    
    storage_manager_private_data_t *private_data = (storage_manager_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Deinitializing %s module", self->name);
    
    // Unregister the service to prevent other modules from using it.
    synapse_service_unregister(self->name);

    // --- Backend De-initializer Dispatcher ---
    #if CONFIG_STORAGE_BACKEND_SPIFFS
        storage_backend_spiffs_deinit();
    #elif CONFIG_STORAGE_BACKEND_SD_CARD
        storage_backend_sd_deinit();
    #endif
    // NVS does not require a specific deinit call in this context.

    if (private_data) {
        free(private_data);
    }
    
    if (self->current_config) {
        cJSON_Delete(self->current_config);
    }
    
    if (self->state_mutex) {
        vSemaphoreDelete(self->state_mutex);
    }

    ESP_LOGI(TAG, "Storage Manager module deinitialized successfully.");
}