/**
 * @file module_helpers.c
 * @brief მოდულების დამხმარე (Helper) ფუნქციების იმპლემენტაცია.
 */
#include "module_helpers.h"
#include "logging.h"

DEFINE_COMPONENT_TAG("MODULE_HELPERS");

esp_err_t fmw_module_helper_simple_enable(struct module_t* module) {
    if (!module) {
        return ESP_ERR_INVALID_ARG;
    }
    if (module->status == MODULE_STATUS_RUNNING) {
        ESP_LOGW(TAG, "Module '%s' is already running.", module->name);
        return ESP_ERR_INVALID_STATE;
    }
    ESP_LOGI(TAG, "Enabling module '%s'.", module->name);
    module->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

esp_err_t fmw_module_helper_simple_disable(struct module_t* module) {
    if (!module) {
        return ESP_ERR_INVALID_ARG;
    }
    if (module->status == MODULE_STATUS_DISABLED) {
        ESP_LOGW(TAG, "Module '%s' is already disabled.", module->name);
        return ESP_ERR_INVALID_STATE;
    }
    ESP_LOGI(TAG, "Disabling module '%s'.", module->name);
    module->status = MODULE_STATUS_DISABLED;
    return ESP_OK;
}

module_status_t fmw_module_helper_simple_get_status(struct module_t* module) {
    if (!module) {
        return MODULE_STATUS_UNKNOWN;
    }
    return module->status;
}