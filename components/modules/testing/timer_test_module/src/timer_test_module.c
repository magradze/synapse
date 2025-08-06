/**
 * @file timer_test.c
 * @brief A module to test the System Timer and RGB LED services.
 * @author Synapse Framework Team
 * @version 1.1.0 (Build Fixes & Cleanup)
 * @date 2025-07-06
 */

// --- Synapse Framework Includes ---
#include "synapse.h"
#include "timer_test_module.h"

#include "timer_interface.h"
#include "rgb_led_interface.h"
#include "framework_config.h"

// --- Standard & ESP-IDF Includes ---
#include <string.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_random.h"
#include <inttypes.h>

DEFINE_COMPONENT_TAG("TIMER_TEST");

// --- Event Names ---
#define EVT_TOGGLE_LED_COLOR "EVT_TOGGLE_LED_COLOR"

// --- Private Data Structure ---

/**
 * @internal
 * @struct timer_test_module_private_data_t
 * @brief Private data for the Timer Test module.
 */
typedef struct {
    char instance_name[CONFIG_TIMER_TEST_MODULE_INSTANCE_NAME_MAX_LEN];
    fmw_timer_handle_t led_timer_handle; /**< Handle for the scheduled timer. */
    rgb_led_api_t *led_api;              /**< Cached pointer to the RGB LED service API. */
    bool is_led_on;                      /**< Tracks the current state of the LED for toggling. */
} timer_test_module_private_data_t;

// --- Forward Declarations ---
// ★★★ შესწორება: ფუნქციების სახელები გასწორდა კონვენციის მიხედვით ★★★
static esp_err_t timer_test_init(module_t *self);
static esp_err_t timer_test_start(module_t *self);
static void timer_test_deinit(module_t *self);
static void timer_test_handle_event(module_t *self, const char *event_name, void *event_data);

// --- Module Create Function ---
module_t *timer_test_create(const cJSON *config) {
    ESP_LOGI(TAG, "Creating Timer Test module...");
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module) {
        ESP_LOGE(TAG, "Failed to allocate memory for module_t.");
        return NULL;
    }
    
    timer_test_module_private_data_t *private_data = (timer_test_module_private_data_t *)calloc(1, sizeof(timer_test_module_private_data_t));
    if (!private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory for private_data.");
        free(module);
        return NULL;
    }
    module->private_data = private_data;
    
    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex) {
        ESP_LOGE(TAG, "Failed to create state mutex.");
        free(private_data);
        free(module);
        return NULL;
    }

    const char *instance_name = CONFIG_TIMER_TEST_MODULE_DEFAULT_INSTANCE_NAME;
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
    
    module->init_level = 80;
    
    module->base.init = timer_test_init;
    module->base.start = timer_test_start;
    module->base.deinit = timer_test_deinit;
    module->base.handle_event = timer_test_handle_event;
    
    ESP_LOGI(TAG, "Timer Test module '%s' created.", module->name);
    return module;
}

// --- Module Lifecycle ---
static esp_err_t timer_test_init(module_t *self) {
    ESP_LOGI(TAG, "Initializing Timer Test module: %s", self->name);
    
    fmw_event_bus_subscribe(EVT_TOGGLE_LED_COLOR, self);
    
    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

static esp_err_t timer_test_start(module_t *self) {
    ESP_LOGI(TAG, "Starting Timer Test module: %s", self->name);
    timer_test_module_private_data_t *private_data = (timer_test_module_private_data_t *)self->private_data;

    service_handle_t led_handle = fmw_service_get("status_led");
    if (!led_handle) {
        ESP_LOGE(TAG, "RGB LED Indicator service not found! Test cannot run.");
        self->status = MODULE_STATUS_ERROR;
        return ESP_ERR_NOT_FOUND;
    }
    private_data->led_api = (rgb_led_api_t *)led_handle;

    service_handle_t timer_handle = fmw_service_get("main_timer_service");
    if (!timer_handle) {
        ESP_LOGE(TAG, "System Timer service not found! Test cannot run.");
        self->status = MODULE_STATUS_ERROR;
        return ESP_ERR_NOT_FOUND;
    }
    timer_api_t *timer_api = (timer_api_t *)timer_handle;

    uint32_t interval_ms = 3000;
    private_data->led_timer_handle = timer_api->schedule_event(EVT_TOGGLE_LED_COLOR, interval_ms, true);

    if (private_data->led_timer_handle == NULL) {
        ESP_LOGE(TAG, "Failed to schedule LED toggle event.");
        self->status = MODULE_STATUS_ERROR;
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Test timer scheduled successfully. LED will toggle color every %" PRIu32 "ms.", interval_ms);
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void timer_test_deinit(module_t *self) {
    if (!self) return;
    ESP_LOGI(TAG, "Deinitializing Timer Test module: %s", self->name);
    timer_test_module_private_data_t *private_data = (timer_test_module_private_data_t *)self->private_data;

    if (private_data->led_timer_handle) {
        service_handle_t timer_handle = fmw_service_get("main_timer_service");
        if (timer_handle) {
            timer_api_t *timer_api = (timer_api_t *)timer_handle;
            timer_api->cancel_event(private_data->led_timer_handle);
        }
    }
    
    if (private_data->led_api) {
        private_data->led_api->release_control();
    }

    fmw_event_bus_unsubscribe(EVT_TOGGLE_LED_COLOR, self);
    
    if (self->private_data) free(self->private_data);
    if (self->current_config) cJSON_Delete(self->current_config);
    if (self->state_mutex) vSemaphoreDelete(self->state_mutex);
    free(self);
}

// --- Event Handler ---
static void timer_test_handle_event(module_t *self, const char *event_name, void *event_data) {
    if (strcmp(event_name, EVT_TOGGLE_LED_COLOR) == 0) {
        timer_test_module_private_data_t *private_data = (timer_test_module_private_data_t *)self->private_data;
        
        if (private_data->is_led_on) {
            ESP_LOGI(TAG, "Timer event: Turning LED off.");
            private_data->led_api->turn_off();
            private_data->is_led_on = false;
        } else {
            uint8_t r = esp_random() % 256;
            uint8_t g = esp_random() % 256;
            uint8_t b = esp_random() % 256;
            ESP_LOGI(TAG, "Timer event: Setting LED to random color (R:%d, G:%d, B:%d).", r, g, b);
            private_data->led_api->set_color(r, g, b);
            private_data->is_led_on = true;
        }
    }

    if (event_data) {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}