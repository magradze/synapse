/**
 * @file logger_module.c
 * @brief ლოგერ მოდულის იმპლემენტაცია - ცენტრალიზებული ლოგირება და ივენთების მონიტორინგი
 * @details ეს მოდული ახდენს ივენთების სრულ მონიტორინგს, იჭერს ყველა შეტყობინებას
 *          Event Bus-იდან და უზრუნველყოფს გამართვისთვის საჭირო ინფორმაციის გამოტანას.
 *          მისი მართვა შესაძლებელია რეალურ დროში, ფრეიმვორქის ბრძანებების მეშვეობით.
 * @author Synapse Framework Team
 * @version 2.1.0
 * @date 2025-06-27
 */

// სტანდარტული ბიბლიოთეკები
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ESP-IDF ბიბლიოთეკები
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// ფრეიმვორქის ძირითადი კომპონენტები
#include "base_module.h"
#include "event_bus.h"
#include "logging.h"
#include "event_data_wrapper.h"
#include "event_payloads.h" // ვივარაუდოთ, რომ fmw_telemetry_payload_t აქ არის

/** @brief კომპონენტის ტეგი ESP-IDF ლოგირების სისტემისთვის */
DEFINE_COMPONENT_TAG("LOGGER_MODULE");

// =============================================================================
// მოდულის პრივატული მონაცემების სტრუქტურა და კონსტანტები
// =============================================================================

#define LOGGER_DEFAULT_INSTANCE_NAME "main_logger"
#define LOGGER_INSTANCE_NAME_MAX_LEN 32

/**
 * @brief ლოგერ მოდულის პრივატული მონაცემების სტრუქტურა.
 */
typedef struct
{
    bool enabled;
    char instance_name[LOGGER_INSTANCE_NAME_MAX_LEN];
} logger_private_data_t;

// =============================================================================
// პრივატული ფუნქციების პროტოტიპები
// =============================================================================

static esp_err_t logger_module_init(module_t *self);
static esp_err_t logger_module_start(module_t *self);
// ⭐️ შესწორებულია: ფუნქციის დაბრუნების ტიპი არის void
static void logger_module_handle_event(module_t *self, const char *event_name, void *event_data);
static void logger_module_deinit(module_t *self);
static esp_err_t logger_module_enable(module_t *self);
static esp_err_t logger_module_disable(module_t *self);
static module_status_t logger_module_get_status(module_t *self);
static void logger_module_process_command(module_t *self, const fmw_telemetry_payload_t *command);

// =============================================================================
// Public API - მოდულის შემქმნელი ფუნქცია
// =============================================================================

module_t *logger_module_create(const cJSON *config)
{
    ESP_LOGI(TAG, "ლოგერ მოდულის ინსტანციის შექმნა...");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module)
    {
        ESP_LOGE(TAG, "ლოგერ მოდულისთვის მეხსიერების გამოყოფა ვერ მოხერხდა.");
        return NULL;
    }

    logger_private_data_t *private_data = (logger_private_data_t *)calloc(1, sizeof(logger_private_data_t));
    if (!private_data)
    {
        ESP_LOGE(TAG, "პრივატული მონაცემებისთვის მეხსიერების გამოყოფა ვერ მოხერხდა.");
        free(module);
        return NULL;
    }

    // ⭐️ შესწორებულია: state_mutex-ის შექმნა
    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex)
    {
        ESP_LOGE(TAG, "State mutex-ის შექმნა ვერ მოხერხდა.");
        free(private_data);
        free(module);
        return NULL;
    }

    module->private_data = private_data;

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
        module->current_config = cJSON_Duplicate(config, true);
    }

    private_data->enabled = true;
    strncpy(private_data->instance_name, instance_name, LOGGER_INSTANCE_NAME_MAX_LEN - 1);

    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;

    module->init_level = 1;

    // საბაზისო ფუნქციების მიბმა
    module->base.init = logger_module_init;
    module->base.start = logger_module_start;
    module->base.handle_event = logger_module_handle_event;
    module->base.deinit = logger_module_deinit;
    module->base.enable = logger_module_enable;
    module->base.disable = logger_module_disable;
    module->base.reconfigure = NULL; // ამ მოდულს არ აქვს reconfigure
    module->base.get_status = logger_module_get_status;

    ESP_LOGI(TAG, "ლოგერ მოდულის ინსტანცია '%s' წარმატებით შეიქმნა.", instance_name);
    return module;
}

// =============================================================================
// Private Function Implementations
// =============================================================================

static void logger_module_process_command(module_t *self, const fmw_telemetry_payload_t *command)
{
    if (!self || !self->private_data || !command || !command->json_data)
    {
        ESP_LOGW(TAG, "არავალიდური პარამეტრები ბრძანების დამუშავებისას.");
        return;
    }

    logger_private_data_t *private_data = (logger_private_data_t *)self->private_data;

    if (strcmp(command->module_name, private_data->instance_name) != 0)
    {
        return; // ბრძანება ჩვენ არ გვეხება
    }

    ESP_LOGI(TAG, "მიღებულია ბრძანება მოდულისთვის: %s", command->module_name);

    cJSON *root = cJSON_Parse(command->json_data);
    if (!root)
    {
        ESP_LOGW(TAG, "JSON ბრძანების გარჩევა ვერ მოხერხდა: %s", command->json_data);
        return;
    }

    cJSON *action = cJSON_GetObjectItem(root, "action");
    if (cJSON_IsString(action) && action->valuestring)
    {
        if (strcmp(action->valuestring, "enable") == 0)
        {
            logger_module_enable(self);
        }
        else if (strcmp(action->valuestring, "disable") == 0)
        {
            logger_module_disable(self);
        }
        else
        {
            ESP_LOGW(TAG, "უცნობი მოქმედება: %s", action->valuestring);
        }
    }
    else
    {
        ESP_LOGW(TAG, "არავალიდური 'action' ველი ბრძანებაში.");
    }

    cJSON_Delete(root);
}

// =============================================================================
// Base Module Function Implementations
// =============================================================================

static esp_err_t logger_module_init(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;

    ESP_LOGI(TAG, "'%s' მოდულის ინიციალიზაცია...", self->name);

    // ⭐️ გამოვიწეროთ ყველა (*) ივენთი. ამისათვის სპეციალური სახელი გვჭირდება.
    esp_err_t ret = fmw_event_bus_subscribe("*", self);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "ყველა ივენთზე გამოწერა ვერ მოხერხდა: %s", esp_err_to_name(ret));
        // შეგვიძლია დავაბრუნოთ შეცდომა, რათა სისტემამ იცოდეს, რომ ლოგერი ვერ ჩაირთო
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
    if (!self || !self->private_data)
    {
        if (event_data)
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    logger_private_data_t *private_data = (logger_private_data_t *)self->private_data;

    if (!private_data->enabled)
    {
        if (event_data)
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    // ლოგირება ნებისმიერი ივენთისთვის, რომელსაც აქვს სახელი
    if (event_name)
    {
        ESP_LOGI(TAG, "[%s] ივენთი: '%s'", private_data->instance_name, event_name);

        // შევამოწმოთ, ხომ არ არის ეს კონკრეტული ბრძანების ივენთი
        // ეს მიდგომა გარანტირებულად აგვარებს nonnull გაფრთხილებას.
        const char *command_event = FRAMEWORK_EVENT_COMMAND_RECEIVED;
        if (command_event && strcmp(event_name, command_event) == 0)
        {
            event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;
            if (wrapper && wrapper->payload)
            {
                const fmw_telemetry_payload_t *command = (const fmw_telemetry_payload_t *)wrapper->payload;
                logger_module_process_command(self, command);
            }
            else
            {
                ESP_LOGW(TAG, "ბრძანების ივენთი '%s' მონაცემების გარეშე.", event_name);
            }
        }
    }
    else
    {
        ESP_LOGW(TAG, "[%s] მიღებულია ივენთი NULL სახელით.", private_data->instance_name);
    }

    if (event_data)
    {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
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

    // ⭐️ გამოწერის გაუქმება
    fmw_event_bus_unsubscribe("*", self); // ან "*"

    if (self->private_data)
    {
        free(self->private_data);
    }
    if (self->current_config)
    {
        cJSON_Delete(self->current_config);
    }
    // ⭐️ state_mutex-ის განადგურება
    if (self->state_mutex)
    {
        vSemaphoreDelete(self->state_mutex);
    }
    free(self);
}