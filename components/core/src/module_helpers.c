/**
 * @file module_helpers.c
 * @brief მოდულების დამხმარე (Helper) ფუნქციების იმპლემენტაცია.
 * @version 1.1
 * @date 2025-06-27
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს სტანდარტულ, მრავალჯერად ფუნქციებს, რომლებიც
 *          ამარტივებენ მოდულების სიცოცხლის ციკლის საბაზისო ოპერაციების
 *          (enable, disable, get_status) იმპლემენტაციას.
 *          ყველა ოპერაცია არის ნაკად-უსაფრთხო (thread-safe).
 */
#include "module_helpers.h"
#include "logging.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "base_module.h"

DEFINE_COMPONENT_TAG("MODULE_HELPERS");

esp_err_t synapse_module_helper_simple_enable(struct module_t *module)
{
    if (!module || !module->state_mutex)
    { // დაემატა mutex-ის შემოწმება
        return ESP_ERR_INVALID_ARG;
    }

    // ⭐️ დაცვა mutex-ით race condition-ის თავიდან ასაცილებლად
    if (xSemaphoreTake(module->state_mutex, portMAX_DELAY) != pdTRUE)
    {
        ESP_LOGE(TAG, "Failed to take state mutex for module '%s'", module->name);
        return ESP_ERR_TIMEOUT;
    }

    if (module->status == MODULE_STATUS_RUNNING) {
        xSemaphoreGive(module->state_mutex); // აუცილებლად გავათავისუფლოთ mutex-ი დაბრუნებამდე
        ESP_LOGW(TAG, "Module '%s' is already running.", module->name);
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "Enabling module '%s'.", module->name);
    module->status = MODULE_STATUS_RUNNING;

    xSemaphoreGive(module->state_mutex);
    return ESP_OK;
}

esp_err_t synapse_module_helper_simple_disable(struct module_t *module)
{
    if (!module || !module->state_mutex)
    {
        return ESP_ERR_INVALID_ARG;
    }

    // ⭐️ დაცვა mutex-ით
    if (xSemaphoreTake(module->state_mutex, portMAX_DELAY) != pdTRUE)
    {
        ESP_LOGE(TAG, "Failed to take state mutex for module '%s'", module->name);
        return ESP_ERR_TIMEOUT;
    }

    if (module->status == MODULE_STATUS_DISABLED) {
        xSemaphoreGive(module->state_mutex);
        ESP_LOGW(TAG, "Module '%s' is already disabled.", module->name);
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "Disabling module '%s'.", module->name);
    module->status = MODULE_STATUS_DISABLED;

    xSemaphoreGive(module->state_mutex);
    return ESP_OK;
}

module_status_t synapse_module_helper_simple_get_status(struct module_t *module)
{
    if (!module || !module->state_mutex)
    {
        return MODULE_STATUS_UNKNOWN;
    }

    module_status_t current_status;

    // ⭐️ დაცვა mutex-ით
    if (xSemaphoreTake(module->state_mutex, portMAX_DELAY) != pdTRUE)
    {
        ESP_LOGE(TAG, "Failed to take state mutex for module '%s'", module->name);
        return MODULE_STATUS_UNKNOWN;
    }

    current_status = module->status;

    xSemaphoreGive(module->state_mutex);

    return current_status;
}