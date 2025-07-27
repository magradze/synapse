/**
 * @file button_input.c
 * @brief A module to handle hardware button presses and publish events.
 * @author Giorgi Magradze
 * @details This module uses GPIO interrupts to detect button presses. To avoid
 *          bouncing issues (debounce) and to keep the ISR fast, the ISR only
 *          sends the GPIO number to a FreeRTOS queue. A dedicated task then
 *          processes the queue, performs debouncing, and publishes a clean
 *          FMW_EVENT_BUTTON_PRESSED event.
 */
#include "button_input.h"
#include "logging.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "event_payloads.h"
#include "framework_events.h"
#include "resource_manager.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <inttypes.h>

DEFINE_COMPONENT_TAG("BUTTON_INPUT");

#define MAX_BUTTONS 10
#define DEBOUNCE_TIME_MS 50

/**
 * @internal @brief Configuration for a single button.
 */
typedef struct {
    char name[16];
    gpio_num_t gpio_pin;
    uint8_t active_level;
} button_config_t;

/**
 * @internal @brief Private data for the Button Input module instance.
 */
typedef struct {
    char instance_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];
    TaskHandle_t task_handle;
    QueueHandle_t gpio_evt_queue;
    button_config_t buttons[MAX_BUTTONS];
    uint8_t button_count;
} button_input_private_data_t;

// Global queue handle for ISR access
static QueueHandle_t g_button_gpio_evt_queue = NULL;

// --- Forward Declarations ---
static esp_err_t button_input_init(module_t *self);
static esp_err_t button_input_start(module_t *self);
static void button_input_deinit(module_t *self);
static esp_err_t parse_config(const cJSON *config_node, button_input_private_data_t *private_data);
static void button_task(void *pvParameters);
static void gpio_isr_handler(void *arg);

// --- Module Factory ---
module_t *button_input_create(const cJSON *config) {
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    button_input_private_data_t *private_data = (button_input_private_data_t *)calloc(1, sizeof(button_input_private_data_t));
    if (!module || !private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module); free(private_data);
        if (config) cJSON_Delete((cJSON*)config);
        return NULL;
    }

    module->current_config = (cJSON*)config;
    module->private_data = private_data;

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (parse_config(config_node, private_data) != ESP_OK) {
        ESP_LOGE(TAG, "Configuration parsing failed");
        button_input_deinit(module);
        return NULL;
    }

    snprintf(module->name, sizeof(module->name), "%s", private_data->instance_name);
    module->init_level = 60;
    module->base.init = button_input_init;
    module->base.start = button_input_start;
    module->base.deinit = button_input_deinit;
    module->base.handle_event = NULL; // This module only produces events

    ESP_LOGI(TAG, "Button Input module '%s' created with %d buttons.", module->name, private_data->button_count);
    return module;
}

/**
 * @internal
 * @brief Interrupt Service Routine for all configured buttons.
 * @details This function is kept minimal and fast. It receives the GPIO
 *          number that triggered the interrupt and sends it to a high-priority
 *          FreeRTOS queue for deferred processing.
 * @param arg The GPIO number, passed during ISR registration.
 */
static void IRAM_ATTR gpio_isr_handler(void *arg) {
    uint32_t gpio_num = (uint32_t) (intptr_t) arg;
    // This is a global variable pointing to the queue.
    // This is a safe and common pattern for ISRs.
    extern QueueHandle_t g_button_gpio_evt_queue;
    xQueueSendFromISR(g_button_gpio_evt_queue, &gpio_num, NULL);
}

/**
 * @internal
 * @brief Task that processes button events from the queue.
 * @details This task waits for GPIO events, applies debouncing, and publishes
 *          button press events. It runs indefinitely until the module is deinitialized.
 * @param pvParameters Pointer to the module instance.
 */
static void button_task(void *pvParameters) {
    module_t *self = (module_t *)pvParameters;
    button_input_private_data_t *private_data = (button_input_private_data_t *)self->private_data;
    uint32_t io_num;

    while (1) {
        if (xQueueReceive(private_data->gpio_evt_queue, &io_num, portMAX_DELAY)) {
            // Debounce delay: wait for the signal to stabilize
            vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_TIME_MS));

            // After the delay, read the final state of the pin
            int level = gpio_get_level(io_num);

            // Find which button it was
            for (int i = 0; i < private_data->button_count; i++)
            {
                if (private_data->buttons[i].gpio_pin == io_num)
                {
                    // Check if the final state is the "active" state
                    if (level == private_data->buttons[i].active_level)
                    {
                        ESP_LOGI(TAG, "Button '%s' press confirmed on GPIO %" PRIu32, private_data->buttons[i].name, io_num);

                        fmw_button_payload_t *payload = calloc(1, sizeof(fmw_button_payload_t));
                        if (payload) {
                            snprintf(payload->button_name, sizeof(payload->button_name), "%s", private_data->buttons[i].name);
                            
                            event_data_wrapper_t *wrapper;
                            if (fmw_event_data_wrap(payload, fmw_payload_common_free, &wrapper) == ESP_OK) {
                                fmw_event_bus_post(FMW_EVENT_BUTTON_PRESSED, wrapper);
                                fmw_event_data_release(wrapper);
                            } else {
                                free(payload);
                            }
                        }
                    }
                    break; // Found the button
                }
            }
        }
    }
}

/**
 * @internal
 * @brief Initializes the Button Input module.
 * @details Installs the global ISR service, creates the event queue, and
 *          iterates through the configured buttons to lock their GPIO resources
 *          and register an ISR for each one, passing the queue handle to the ISR.
 * @param self Pointer to the module instance.
 * @return ESP_OK on success, or an error code on failure.
 */
static esp_err_t button_input_init(module_t *self) {
    button_input_private_data_t *private_data = (button_input_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing Button Input module '%s'", self->name);

    g_button_gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    private_data->gpio_evt_queue = g_button_gpio_evt_queue;
    if (!private_data->gpio_evt_queue) {
        ESP_LOGE(TAG, "Failed to create GPIO event queue");
        return ESP_ERR_NO_MEM;
    }

    // Install ISR service once
    gpio_install_isr_service(0);

    for (int i = 0; i < private_data->button_count; i++) {
        button_config_t *btn = &private_data->buttons[i];
        if (fmw_resource_lock(FMW_RESOURCE_TYPE_GPIO, btn->gpio_pin, self->name) != ESP_OK) {
            // On failure, cleanup already locked resources
            for (int j = 0; j < i; j++) {
                fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->buttons[j].gpio_pin, self->name);
            }
            return ESP_FAIL;
        }

        gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << btn->gpio_pin),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = (btn->active_level == 0),
            .pull_down_en = (btn->active_level == 1),
            .intr_type = GPIO_INTR_ANYEDGE,
        };

        gpio_config(&io_conf);
        // ★★★ CORRECTED: Pass the GPIO pin number as the argument to the ISR ★★★
        gpio_isr_handler_add(btn->gpio_pin, gpio_isr_handler, (void *) (intptr_t) btn->gpio_pin);
        ESP_LOGI(TAG, "Button '%s' configured on GPIO %d", btn->name, btn->gpio_pin);
    }

    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}
static esp_err_t button_input_start(module_t *self) {
    button_input_private_data_t *private_data = (button_input_private_data_t *)self->private_data;
    BaseType_t ret = xTaskCreate(button_task, self->name, 3072, self, 10, &private_data->task_handle);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create button task");
        return ESP_FAIL;
    }
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void button_input_deinit(module_t *self) {
    if (!self) return;
    button_input_private_data_t *private_data = (button_input_private_data_t *)self->private_data;
    if (private_data) {
        ESP_LOGI(TAG, "De-initializing Button Input '%s'", self->name);
        if (private_data->task_handle) vTaskDelete(private_data->task_handle);
        if (private_data->gpio_evt_queue) {
            vQueueDelete(private_data->gpio_evt_queue);
            g_button_gpio_evt_queue = NULL;
        }

        for (int i = 0; i < private_data->button_count; i++) {
            gpio_isr_handler_remove(private_data->buttons[i].gpio_pin);
            fmw_resource_release(FMW_RESOURCE_TYPE_GPIO, private_data->buttons[i].gpio_pin, self->name);
        }
        free(private_data);
    }
    if (self->current_config) cJSON_Delete(self->current_config);
    free(self);
}

// --- Internal Config Parser ---
static esp_err_t parse_config(const cJSON *config_node, button_input_private_data_t *private_data) {
    if (!config_node) return ESP_ERR_INVALID_ARG;

    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    if (!cJSON_IsString(name_node)) return ESP_ERR_INVALID_ARG;
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", name_node->valuestring);

    const cJSON *buttons_array = cJSON_GetObjectItem(config_node, "buttons");
    if (!cJSON_IsArray(buttons_array)) {
        ESP_LOGE(TAG, "'buttons' array is missing in config");
        return ESP_ERR_INVALID_ARG;
    }

    private_data->button_count = 0;
    const cJSON *button_node;
    cJSON_ArrayForEach(button_node, buttons_array) {
        if (private_data->button_count >= MAX_BUTTONS) {
            ESP_LOGW(TAG, "Maximum number of buttons (%d) reached. Ignoring rest.", MAX_BUTTONS);
            break;
        }
        
        const cJSON *btn_name = cJSON_GetObjectItem(button_node, "name");
        const cJSON *btn_pin = cJSON_GetObjectItem(button_node, "gpio_pin");
        const cJSON *btn_level = cJSON_GetObjectItem(button_node, "active_level");

        if (cJSON_IsString(btn_name) && cJSON_IsNumber(btn_pin)) {
            button_config_t *btn = &private_data->buttons[private_data->button_count];
            snprintf(btn->name, sizeof(btn->name), "%s", btn_name->valuestring);
            btn->gpio_pin = btn_pin->valueint;
            btn->active_level = cJSON_IsNumber(btn_level) ? btn_level->valueint : 0; // Default to active-low
            private_data->button_count++;
        }
    }
    return ESP_OK;
}