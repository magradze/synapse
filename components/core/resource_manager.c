/**
 * @file resource_manager.c
 * @brief რესურსების მენეჯერის იმპლემენტაცია.
 * @version 2.0
 * @date 2025-06-26
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს GPIO პინების მართვის იმპლემენტაციას, რათა თავიდან
 *          აცილებულ იქნას კონფლიქტები სხვადასხვა მოდულებს შორის. ის იყენებს
 *          Bitmap-ს დაკავებული პინების ეფექტური აღრიცხვისთვის და Mutex-ს
 *          ოპერაციების ნაკად-უსაფრთხოების (thread-safety) უზრუნველსაყოფად,
 *          რაც კრიტიკულია მრავალნაკადიან RTOS გარემოში.
 *
 *          ძირითადი მახასიათებლები:
 *          - GPIO პინების ნაკად-უსაფრთხო დაკავება და გათავისუფლება.
 *          - მფლობელის აღრიცხვა არაავტორიზებული გათავისუფლების თავიდან ასაცილებლად.
 *          - დეტალური ლოგირება რესურსების კონფლიქტის მარტივი დიაგნოსტიკისთვის.
 */
#include "resource_manager.h"
#include "logging.h"
#include "framework_config.h" // Kconfig პარამეტრებისთვის
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

DEFINE_COMPONENT_TAG("RESOURCE_MANAGER");

/**
 * @internal
 * @brief Mutex, რომელიც უზრუნველყოფს გაზიარებულ რესურსების მონაცემებზე ნაკად-უსაფრთხო წვდომას.
 */
static SemaphoreHandle_t resource_mutex = NULL;

/**
 * @internal
 * @brief Bitmap დაკავებული GPIO პინების აღსარიცხად. თითოეული ბიტი წარმოადგენს ერთ GPIO პინს.
 */
static uint64_t gpio_lock_bitmap = 0;

/**
 * @internal
 * @brief მასივი თითოეული დაკავებული პინის მფლობელის სახელის შესანახად. სასარგებლოა დიაგნოსტიკისთვის.
 */
static const char *gpio_owner_names[CONFIG_FMW_MAX_GPIO_PINS] = {0};


esp_err_t fmw_resource_manager_init(void) {
    ESP_LOGI(TAG, "Initializing resource manager...");

    if (resource_mutex == NULL) {
        resource_mutex = xSemaphoreCreateMutex();
        if (resource_mutex == NULL) {
            ESP_LOGE(TAG, "Failed to create resource mutex.");
            return ESP_ERR_NO_MEM;
        }
    }

    xSemaphoreTake(resource_mutex, portMAX_DELAY);

    gpio_lock_bitmap = 0;
    // memset-ის გამოყენება უფრო ეფექტურია
    memset(gpio_owner_names, 0, sizeof(gpio_owner_names));

    xSemaphoreGive(resource_mutex);

    ESP_LOGI(TAG, "Resource manager initialized successfully.");
    return ESP_OK;
}

void fmw_resource_manager_deinit(void) {
    if (resource_mutex != NULL) {
        vSemaphoreDelete(resource_mutex);
        resource_mutex = NULL;
        ESP_LOGI(TAG, "Resource manager deinitialized.");
    }
}

esp_err_t fmw_resource_lock_gpio(gpio_num_t pin, const char *owner) {
    if (pin >= CONFIG_FMW_MAX_GPIO_PINS || owner == NULL) {
        ESP_LOGE(TAG, "Cannot lock GPIO %d: Invalid arguments (pin or owner is null).", pin);
        return ESP_ERR_INVALID_ARG;
    }

    if (xSemaphoreTake(resource_mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to take resource mutex to lock GPIO %d.", pin);
        return ESP_ERR_TIMEOUT;
    }

    esp_err_t ret = ESP_OK;

    if ((gpio_lock_bitmap >> pin) & 1ULL) {
        ESP_LOGE(TAG, "Cannot lock GPIO %d for '%s'. Already locked by '%s'.",
                 pin, owner, gpio_owner_names[pin] ? gpio_owner_names[pin] : "UNKNOWN");
        ret = ESP_ERR_INVALID_STATE;
    } else {
        gpio_lock_bitmap |= (1ULL << pin);
        gpio_owner_names[pin] = owner;
        ESP_LOGI(TAG, "GPIO %d locked by '%s'.", pin, owner);
    }

    xSemaphoreGive(resource_mutex);
    return ret;
}

esp_err_t fmw_resource_release_gpio(gpio_num_t pin, const char *owner) {
    if (pin >= CONFIG_FMW_MAX_GPIO_PINS || owner == NULL) {
        ESP_LOGE(TAG, "Cannot release GPIO %d: Invalid arguments (pin or owner is null).", pin);
        return ESP_ERR_INVALID_ARG;
    }

    if (xSemaphoreTake(resource_mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to take resource mutex to release GPIO %d.", pin);
        return ESP_ERR_TIMEOUT;
    }

    esp_err_t ret = ESP_OK;

    if (!((gpio_lock_bitmap >> pin) & 1ULL)) {
        ESP_LOGW(TAG, "Attempt to release GPIO %d by '%s', but it was not locked.", pin, owner);
        ret = ESP_ERR_NOT_FOUND;
    } else if (strcmp(gpio_owner_names[pin], owner) != 0) {
        ESP_LOGE(TAG, "Cannot release GPIO %d. Owner mismatch. Locked by '%s', attempted by '%s'.",
                 pin, gpio_owner_names[pin], owner);
        ret = ESP_ERR_INVALID_STATE;
    } else {
        gpio_lock_bitmap &= ~(1ULL << pin);
        gpio_owner_names[pin] = NULL;
        ESP_LOGI(TAG, "GPIO %d released by '%s'.", pin, owner);
    }

    xSemaphoreGive(resource_mutex);
    return ret;
}

bool fmw_resource_is_gpio_locked(gpio_num_t pin) {
    if (pin >= CONFIG_FMW_MAX_GPIO_PINS) {
        return false;
    }

    if (xSemaphoreTake(resource_mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to take resource mutex while checking lock status for GPIO %d.", pin);
        return false; // უსაფრთხოების მიზნით, ჩავთვალოთ რომ არ არის დაკავებული, თუ mutex-ს ვერ ვიღებთ
    }

    bool is_locked = ((gpio_lock_bitmap >> pin) & 1ULL);

    xSemaphoreGive(resource_mutex);

    return is_locked;
}