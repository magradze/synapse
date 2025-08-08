/**
 * @file logger_module.c
 * @brief ლოგერ მოდულის იმპლემენტაცია - ცენტრალიზებული ლოგირება და ივენთების მონიტორინგი
 * @author Synapse Framework Team
 * @version 2.2.0
 * @date 2025-08-09
 */
#include "synapse.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "esp_log.h"

DEFINE_COMPONENT_TAG("LOGGER_MODULE");

#define LOGGER_DEFAULT_INSTANCE_NAME "main_logger"
#define LOGGER_INSTANCE_NAME_MAX_LEN 32

typedef struct
{
    bool enabled;
    char instance_name[LOGGER_INSTANCE_NAME_MAX_LEN];
} logger_private_data_t;

// --- Forward Declarations ---
static esp_err_t logger_module_init(module_t *self);
static esp_err_t logger_module_start(module_t *self);
static void logger_module_handle_event(module_t *self, const char *event_name, void *event_data);
static void logger_module_deinit(module_t *self);
static esp_err_t logger_module_enable(module_t *self);
static esp_err_t logger_module_disable(module_t *self);
static module_status_t logger_module_get_status(module_t *self);

// --- Public API ---
module_t *logger_module_create(const cJSON *config)
{
    ESP_LOGI(TAG, "ლოგერ მოდულის ინსტანციის შექმნა...");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    logger_private_data_t *private_data = (logger_private_data_t *)calloc(1, sizeof(logger_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "მეხსიერების გამოყოფა ვერ მოხერხდა.");
        free(module);
        free(private_data);
        return NULL;
    }

    module->private_data = private_data;
    // We don't take ownership of config as this simple module doesn't need it after creation

    const char *instance_name = LOGGER_DEFAULT_INSTANCE_NAME;
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
        }
    }

    private_data->enabled = true;
    strncpy(private_data->instance_name, instance_name, LOGGER_INSTANCE_NAME_MAX_LEN - 1);
    snprintf(module->name, sizeof(module->name), "%s", instance_name);

    module->status = MODULE_STATUS_UNINITIALIZED;
    module->init_level = 1;

    module->base.init = logger_module_init;
    module->base.start = logger_module_start;
    module->base.handle_event = logger_module_handle_event;
    module->base.deinit = logger_module_deinit;
    module->base.enable = logger_module_enable;
    module->base.disable = logger_module_disable;
    module->base.get_status = logger_module_get_status;

    ESP_LOGI(TAG, "ლოგერ მოდულის ინსტანცია '%s' წარმატებით შეიქმნა.", instance_name);
    return module;
}

// --- Base Module Function Implementations ---

static esp_err_t logger_module_init(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    ESP_LOGI(TAG, "'%s' მოდულის ინიციალიზაცია...", self->name);

    esp_err_t ret = synapse_event_bus_subscribe("*", self);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "ყველა ივენთზე გამოწერა ვერ მოხერხდა: %s", esp_err_to_name(ret));
        return ret;
    }

    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "'%s' მოდული წარმატებით ინიციალიზდა.", self->name);
    return ESP_OK;
}

static esp_err_t logger_module_start(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "'%s' მოდული წარმატებით გაეშვა.", self->name);
    return ESP_OK;
}

static void logger_module_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (event_name && (strcmp(event_name, "UI_HOME_UPDATE") == 0 || strcmp(event_name, "UI_WIFI_STATUS_TIMER") == 0))
    {
        if (event_data)
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    if (!self || !self->private_data)
    {
        if (event_data)
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    logger_private_data_t *private_data = (logger_private_data_t *)self->private_data;
    if (!private_data->enabled)
    {
        if (event_data)
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    if (event_name)
    {
        ESP_LOGI(TAG, "[%s] ივენთი: '%s'", private_data->instance_name, event_name);
    }
    else
    {
        ESP_LOGW(TAG, "[%s] მიღებულია ივენთი NULL სახელით.", private_data->instance_name);
    }

    if (event_data)
    {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

static esp_err_t logger_module_enable(module_t *self)
{
    if (!self || !self->private_data)
        return ESP_ERR_INVALID_ARG;
    logger_private_data_t *private_data = (logger_private_data_t *)self->private_data;

    private_data->enabled = true;
    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "'%s' მოდული ჩაირთო.", private_data->instance_name);
    return ESP_OK;
}

static esp_err_t logger_module_disable(module_t *self)
{
    if (!self || !self->private_data)
        return ESP_ERR_INVALID_ARG;
    logger_private_data_t *private_data = (logger_private_data_t *)self->private_data;

    private_data->enabled = false;
    self->status = MODULE_STATUS_DISABLED;
    ESP_LOGI(TAG, "'%s' მოდული გამოირთო.", self->name);
    return ESP_OK;
}

static module_status_t logger_module_get_status(module_t *self)
{
    if (!self)
        return MODULE_STATUS_UNKNOWN;
    return self->status;
}

static void logger_module_deinit(module_t *self)
{
    if (!self)
        return;
    ESP_LOGI(TAG, "'%s' მოდულის რესურსების გათავისუფლება...", self->name);

    synapse_event_bus_unsubscribe("*", self);

    if (self->private_data)
    {
        free(self->private_data);
    }
    // No need to free current_config as we didn't take ownership
}