/**
 * @file event_data_wrapper.c
 * @brief Event Bus-ისთვის მონაცემთა შეფუთვის (Wrapper) იმპლემენტაცია.
 * @version 2.0
 * @date 2025-06-25
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს ფუნქციების იმპლემენტაციას, რომლებიც მართავენ
 *          მონაცემთა "შეფუთვას" და Reference Counting-ს, რაც უზრუნველყოფს
 *          მეხსიერების უსაფრთხო მართვას ასინქრონულ გარემოში.
 */

#include "event_data_wrapper.h"
#include "logging.h"
#include "framework_config.h" // Kconfig-ის პარამეტრებისთვის
#include <stdlib.h>
#include <string.h>
#include <inttypes.h> // PRId32-სთვის

DEFINE_COMPONENT_TAG("EVENT_WRAPPER");

esp_err_t fmw_event_data_wrap(const void *payload, void (*free_fn)(void *payload), event_data_wrapper_t **wrapper_out)
{
    if (!payload || !wrapper_out)
    {
        ESP_LOGE(TAG, "Cannot wrap payload: invalid arguments provided.");
        return ESP_ERR_INVALID_ARG;
    }

    event_data_wrapper_t *wrapper = calloc(1, sizeof(event_data_wrapper_t));
    if (!wrapper)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for wrapper.");
        return ESP_ERR_NO_MEM;
    }

    wrapper->mutex = xSemaphoreCreateMutex();
    if (!wrapper->mutex)
    {
        ESP_LOGE(TAG, "Failed to create mutex for wrapper.");
        free(wrapper);
        return ESP_ERR_NO_MEM;
    }

    wrapper->payload = (void *)payload;
    wrapper->ref_count = 1; // საწყისი მფლობელი არის გამომძახებელი.
    wrapper->free_payload_fn = free_fn;

    *wrapper_out = wrapper;

    ESP_LOGD(TAG, "Wrapped payload %p in new wrapper %p. Initial ref_count: 1", payload, wrapper);

    return ESP_OK;
}

esp_err_t fmw_event_data_acquire(event_data_wrapper_t* wrapper)
{
    if (!wrapper || !wrapper->mutex)
    {
        ESP_LOGE(TAG, "Acquire failed: wrapper or its mutex is NULL.");
        return ESP_ERR_INVALID_ARG;
    }

    if (xSemaphoreTake(wrapper->mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) == pdTRUE)
    {
        wrapper->ref_count++;
        ESP_LOGD(TAG, "Acquired wrapper %p, ref_count is now %" PRId32, wrapper, wrapper->ref_count);
        xSemaphoreGive(wrapper->mutex);
        return ESP_OK;
    }
    
    ESP_LOGW(TAG, "Failed to take mutex for acquiring wrapper %p", wrapper);
    return ESP_ERR_TIMEOUT;
}

esp_err_t fmw_event_data_release(event_data_wrapper_t* wrapper)
{
    if (!wrapper)
    {
        ESP_LOGW(TAG, "Release called on a NULL wrapper.");
        return ESP_ERR_INVALID_ARG;
    }

    if (!wrapper->mutex)
    {
        ESP_LOGE(TAG, "Release failed: wrapper's mutex is NULL. Memory will be leaked.");
        return ESP_ERR_INVALID_STATE;
    }

    bool should_free = false;

    if (xSemaphoreTake(wrapper->mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) == pdTRUE)
    {
        wrapper->ref_count--;
        ESP_LOGD(TAG, "Released wrapper %p, ref_count is now %" PRId32, wrapper, wrapper->ref_count);

        if (wrapper->ref_count == 0)
        {
            should_free = true;
        }
        else if (wrapper->ref_count < 0)
        {
            ESP_LOGE(TAG, "CRITICAL: ref_count for wrapper %p dropped below zero! Memory corruption likely.", wrapper);
        }
        
        xSemaphoreGive(wrapper->mutex);
    }
    else
    {
        ESP_LOGW(TAG, "Failed to take mutex for releasing wrapper %p. Potential memory leak.", wrapper);
        return ESP_ERR_TIMEOUT;
    }

    if (should_free)
    {
        ESP_LOGI(TAG, "ref_count is zero. Freeing wrapper %p and its payload %p.", wrapper, wrapper->payload);

        if (wrapper->payload)
        {
            if (wrapper->free_payload_fn)
            {
                wrapper->free_payload_fn(wrapper->payload);
            }
            else
            {
                free(wrapper->payload);
            }
        }

        vSemaphoreDelete(wrapper->mutex);
        wrapper->mutex = NULL;

        free(wrapper);
    }
    
    return ESP_OK;
}