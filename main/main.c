#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "system_manager.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "=== Synapse Framework Starting ===");

    // Initialize the framework core
    esp_err_t ret = fmw_system_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize framework core: %s", esp_err_to_name(ret));
        return;
    }

    // Start all modules
    ret = fmw_system_start();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start framework modules: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "=== Synapse Framework Started Successfully ===");
}
