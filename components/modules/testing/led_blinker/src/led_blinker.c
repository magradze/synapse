/**
 * @file led_blinker.c
 * @brief GPIO2-ზე LED-ის მარტივი ციმციმის მოდული
 * @author Synapse Framework Team
 * @version 1.0.0
 * @details Led_Blinker მოდულის იმპლემენტაცია Synapse Framework-ისთვის.
 *          აღზევს ყველა საჭირო base_module interface ფუნქციას.
 */

#include "led_blinker.h"
#include "base_module.h"
#include "event_bus.h"
#include "logging.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <inttypes.h>

DEFINE_COMPONENT_TAG("LED_BLINKER");

/**
 * @brief Led_Blinker მოდულის private მონაცემების სტრუქტურა
 * @details შეიცავს მოდულის შიდა მდგომარეობას და კონფიგურაციას
 */
typedef struct {
    module_t base;                          /**< საბაზისო მოდულის სტრუქტურა */
    led_blinker_config_t config;            /**< მოდულის კონფიგურაცია */
    bool is_initialized;                    /**< ინიციალიზაციის ფლაგი */
    bool is_enabled;                        /**< ჩართვის ფლაგი */
    TaskHandle_t blink_task_handle;         /**< ციმციმის ტასკის ჰენდლი */
    uint32_t interval_ms;                   /**< ციმციმის ინტერვალი მილიწამებში */
} led_blinker_module_t;

static void led_blinker_task(void *pvParameter)
{
    led_blinker_module_t *mod = (led_blinker_module_t *)pvParameter;
    bool led_on = false;
    while (mod->is_enabled) {
        gpio_set_level(CONFIG_LED_BLINKER_GPIO, led_on ? 1 : 0);
        led_on = !led_on;
        vTaskDelay(pdMS_TO_TICKS(mod->interval_ms));
    }
    gpio_set_level(CONFIG_LED_BLINKER_GPIO, 0);
    vTaskDelete(NULL);
}

// --- Forward declarations ---
static esp_err_t led_blinker_init(module_t *self);
static esp_err_t led_blinker_start(module_t *self);
static esp_err_t led_blinker_enable(module_t *self);
static esp_err_t led_blinker_disable(module_t *self);
static esp_err_t led_blinker_reconfigure(module_t *self, const cJSON *new_config);
static void led_blinker_handle_event(module_t *self, const char *event_name, void *event_data);
static int led_blinker_event_wrapper(module_t *self, const char *event_name, struct event_data_wrapper_t *event_data);

module_t *led_blinker_create(const cJSON *config)
{
    (void)config; // Unused parameter
    ESP_LOGI(TAG, "Creating led_blinker module instance");
    
    led_blinker_module_t *module = calloc(1, sizeof(led_blinker_module_t));
    if (!module) {
        ESP_LOGE(TAG, "Failed to allocate memory for led_blinker module");
        return NULL;
    }
    
    // Initialize base module structure
    module->base.status = MODULE_STATUS_UNINITIALIZED;
    module->base.current_config = NULL;
    module->base.private_data = module;
    
    // Set up function pointers
    module->base.base.init = led_blinker_init;
    module->base.base.start = led_blinker_start;
    module->base.base.enable = led_blinker_enable;
    module->base.base.disable = led_blinker_disable;
    module->base.base.reconfigure = led_blinker_reconfigure;
    module->base.base.handle_event = led_blinker_handle_event;

    // Set default configuration
    strncpy(module->base.name, 
#ifdef CONFIG_LED_BLINKER_DEFAULT_INSTANCE_NAME
            CONFIG_LED_BLINKER_DEFAULT_INSTANCE_NAME,
#else
            "main_led_blinker",
#endif
            sizeof(module->base.name) - 1);
    
    strncpy(module->config.instance_name, module->base.name, sizeof(module->config.instance_name) - 1);
    module->config.auto_start = true;
    
    ESP_LOGI(TAG, "Led_Blinker module created successfully with name: %s", module->base.name);
    return (module_t *)module;
}

static esp_err_t led_blinker_init(module_t *self)
{
    if (!self) {
        return ESP_ERR_INVALID_ARG;
    }
    led_blinker_module_t *led_blinker_mod = (led_blinker_module_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing led_blinker module: %s", self->name);
    if (led_blinker_mod->is_initialized) {
        ESP_LOGW(TAG, "Module already initialized");
        return ESP_ERR_INVALID_STATE;
    }
    // Parse configuration if provided
    if (self->current_config) {
        const cJSON *instance_name = cJSON_GetObjectItem(self->current_config, "instance_name");
        if (cJSON_IsString(instance_name)) {
            strncpy(led_blinker_mod->config.instance_name, 
                   instance_name->valuestring, 
                   sizeof(led_blinker_mod->config.instance_name) - 1);
            strncpy(self->name, instance_name->valuestring, sizeof(self->name) - 1);
        }
        const cJSON *auto_start = cJSON_GetObjectItem(self->current_config, "auto_start");
        if (cJSON_IsBool(auto_start)) {
            led_blinker_mod->config.auto_start = cJSON_IsTrue(auto_start);
        }
        const cJSON *interval = cJSON_GetObjectItem(self->current_config, "interval_ms");
        if (cJSON_IsNumber(interval)) {
            led_blinker_mod->interval_ms = interval->valueint;
        } else {
            led_blinker_mod->interval_ms = CONFIG_LED_BLINKER_INTERVAL_MS;
        }
    } else {
        led_blinker_mod->interval_ms = CONFIG_LED_BLINKER_INTERVAL_MS;
    }
    // GPIO init
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << CONFIG_LED_BLINKER_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
    gpio_set_level(CONFIG_LED_BLINKER_GPIO, 0);
    led_blinker_mod->blink_task_handle = NULL;
    led_blinker_mod->is_initialized = true;
    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "Led_Blinker module initialized successfully");
    return ESP_OK;
}

static esp_err_t led_blinker_start(module_t *self)
{
    if (!self) {
        return ESP_ERR_INVALID_ARG;
    }
    led_blinker_module_t *led_blinker_mod = (led_blinker_module_t *)self->private_data;
    
    if (!led_blinker_mod->is_initialized) {
        ESP_LOGE(TAG, "Cannot start uninitialized module");
        return ESP_ERR_INVALID_STATE;
    }
    
    if (self->status == MODULE_STATUS_RUNNING) {
        ESP_LOGW(TAG, "Module already running");
        return ESP_OK;
    }
    
    ESP_LOGI(TAG, "Starting led_blinker module: %s", self->name);
    // თუ უკვე გაშვებულია ტასკი, არ გააჩაღო თავიდან
    if (led_blinker_mod->blink_task_handle == NULL) {
        led_blinker_mod->is_enabled = true;
        xTaskCreate(led_blinker_task, "led_blinker_task", 2048, led_blinker_mod, 5, &led_blinker_mod->blink_task_handle);
        ESP_LOGI(TAG, "Led_Blinker task started");
    }
    
    self->status = MODULE_STATUS_RUNNING;
    led_blinker_mod->is_enabled = true;
    
    ESP_LOGI(TAG, "Led_Blinker module started successfully");
    return ESP_OK;
}

static esp_err_t led_blinker_enable(module_t *self)
{
    if (!self) {
        return ESP_ERR_INVALID_ARG;
    }
    led_blinker_module_t *led_blinker_mod = (led_blinker_module_t *)self->private_data;
    ESP_LOGI(TAG, "Enabling led_blinker module: %s", self->name);
    if (led_blinker_mod->is_enabled) {
        ESP_LOGW(TAG, "Module already enabled");
        return ESP_OK;
    }
    led_blinker_mod->is_enabled = true;
    if (led_blinker_mod->blink_task_handle == NULL) {
        BaseType_t res = xTaskCreate(
            led_blinker_task,
            "led_blinker_task",
            2048,
            led_blinker_mod,
            tskIDLE_PRIORITY + 1,
            &led_blinker_mod->blink_task_handle
        );
        if (res != pdPASS) {
            ESP_LOGE(TAG, "Failed to create led_blinker task");
            led_blinker_mod->is_enabled = false;
            return ESP_FAIL;
        }
        ESP_LOGI(TAG, "Led_Blinker task started");
    }
    ESP_LOGI(TAG, "Led_Blinker module enabled");
    return ESP_OK;
}

static esp_err_t led_blinker_disable(module_t *self)
{
    if (!self) {
        return ESP_ERR_INVALID_ARG;
    }
    led_blinker_module_t *led_blinker_mod = (led_blinker_module_t *)self->private_data;
    ESP_LOGI(TAG, "Disabling led_blinker module: %s", self->name);
    if (!led_blinker_mod->is_enabled) {
        ESP_LOGW(TAG, "Module already disabled");
        return ESP_OK;
    }
    led_blinker_mod->is_enabled = false;
    if (led_blinker_mod->blink_task_handle) {
        // დაველოდოთ ტასკის დასრულებას
        vTaskDelay(pdMS_TO_TICKS(20));
        led_blinker_mod->blink_task_handle = NULL;
    }
    gpio_set_level(CONFIG_LED_BLINKER_GPIO, 0);
    ESP_LOGI(TAG, "Led_Blinker module disabled");
    return ESP_OK;
}

/* static module_status_t led_blinker_get_status(module_t *self)
{
    if (!self) {
        return MODULE_STATUS_ERROR;
    }
    return self->status;
} */
static void led_blinker_handle_event(module_t *self, const char *event_name, void *event_data)
{
    (void)self;
    (void)event_name;
    (void)event_data;
    // ამ ეტაპზე სპეციალური event handling საჭირო არ არის
}

// Wrapper to match module_event_handler_fn signature
static int led_blinker_event_wrapper(module_t *self, const char *event_name, struct event_data_wrapper_t *event_data)
{
    // If you want to map event_name to core_framework_event_id_t, do it here.
    // For now, just call the original handler with dummy values.
    led_blinker_handle_event(self, 0, NULL);
    return 0;
}

static esp_err_t led_blinker_reconfigure(module_t *self, const cJSON *new_config)
{
    if (!self || !new_config) {
        return ESP_ERR_INVALID_ARG;
    }
    led_blinker_module_t *led_blinker_mod = (led_blinker_module_t *)self->private_data;
    ESP_LOGI(TAG, "Reconfiguring led_blinker module: %s", self->name);
    const cJSON *interval = cJSON_GetObjectItem(new_config, "interval_ms");
    if (cJSON_IsNumber(interval)) {
        led_blinker_mod->interval_ms = interval->valueint;
        ESP_LOGI(TAG, "Interval updated to %"PRIu32" ms", led_blinker_mod->interval_ms);
    }
    // სხვა პარამეტრების დამუშავება საჭირო არ არის
    ESP_LOGI(TAG, "Led_Blinker module reconfigured");
    return ESP_OK;
}


