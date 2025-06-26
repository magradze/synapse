/**
 * @file event_bus.c
 * @brief Event Bus კომპონენტის იმპლემენტაცია.
 * @version 2.0
 * @date 2025-06-25
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს Event Bus-ის იმპლემენტაციას, რომელიც ამუშავებს
 *          ასინქრონულ ივენთებს FreeRTOS-ის რიგებისა და ტასკების გამოყენებით.
 */
#include "event_bus.h"
#include "logging.h"
#include "base_module.h"        // საჭიროა module_t სტრუქტურის სრული დეფინიციისთვის
#include "framework_config.h"   // Kconfig-ის პარამეტრებისთვის
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("EVENT_BUS");

// --- შიდა სტრუქტურები ---

/**
 * @internal
 * @brief აღწერს ერთ გამომწერს (მოდულს).
 */
typedef struct {
    struct module_t *module; /**< @brief მაჩვენებელი გამომწერ მოდულზე. */
} event_subscriber_t;

/**
 * @internal
 * @brief აღწერს ერთი ივენთის ყველა გამომწერის სიას.
 */
typedef struct {
    event_subscriber_t subscribers[CONFIG_FMW_MAX_SUBSCRIBERS_PER_EVENT]; /**< @brief გამომწერების მასივი. */
    uint8_t count;                                                      /**< @brief გამომწერების მიმდინარე რაოდენობა. */
} event_subscription_list_t;

/**
 * @internal
 * @brief აღწერს ერთ ივენთს, რომელიც იგზავნება რიგში.
 */
typedef struct {
    core_framework_event_id_t event_id;         /**< @brief ივენთის ID. */
    event_data_wrapper_t *data_wrapper; /**< @brief მაჩვენებელი ივენთის მონაცემებზე. */
} event_message_t;

// --- კომპონენტის შიდა ცვლადები ---

/**
 * @internal
 * @brief გამოწერების სიების დინამიური მასივი. ინდექსი არის event_id.
 */
static event_subscription_list_t **event_subscriptions = NULL;
static size_t event_subscriptions_capacity = 0;

static QueueHandle_t event_bus_queue = NULL;
static SemaphoreHandle_t subscription_mutex = NULL;

// --- შიდა ფუნქციების წინასწარი დეკლარაცია ---
static void event_bus_task(void *pvParameters);
static esp_err_t ensure_subscription_capacity(core_framework_event_id_t event_id);


/**
 * @internal
 * @brief Event Bus-ის მთავარი ტასკი, რომელიც ამუშავებს ივენთებს.
 * @details ეს ტასკი უსასრულოდ ელოდება `event_bus_queue`-ში ახალ შეტყობინებას.
 *          როდესაც შეტყობინება მიიღება, ის უსაფრთხოდ (mutex-ის ქვეშ) ქმნის
 *          გამომწერების სიის ასლს და ამ ასლზე დაყრდნობით აგზავნის ივენთებს,
 *          რათა თავიდან აიცილოს race condition-ები გამოწერების სიის ცვლილებისას.
 * @param pvParameters არგუმენტები ტასკისთვის (არ გამოიყენება).
 */
static void event_bus_task(void *pvParameters) {
    event_message_t msg;
    while (1) {
        if (xQueueReceive(event_bus_queue, &msg, portMAX_DELAY) == pdPASS) {
            ESP_LOGD(TAG, "Processing event ID: %d", msg.event_id);

            event_subscription_list_t local_sub_list;
            bool list_found = false;

            // კრიტიკული სექცია: წავიკითხოთ გამომწერების სია უსაფრთხოდ
            if (xSemaphoreTake(subscription_mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) == pdTRUE) {
                if (msg.event_id < event_subscriptions_capacity && event_subscriptions[msg.event_id] != NULL) {
                    // შევქმნათ ლოკალური ასლი, რათა მალე გავათავისუფლოთ mutex-ი
                    memcpy(&local_sub_list, event_subscriptions[msg.event_id], sizeof(event_subscription_list_t));
                    list_found = true;
                }
                xSemaphoreGive(subscription_mutex);
            }

            if (list_found) {
                ESP_LOGD(TAG, "Dispatching event %d to %d subscribers", msg.event_id, local_sub_list.count);
                // ვიმუშაოთ ლოკალურ ასლთან
                for (int i = 0; i < local_sub_list.count; i++) {
                    event_subscriber_t *sub = &local_sub_list.subscribers[i];
                    if (sub->module && sub->module->base.handle_event) {
                        if (msg.data_wrapper) {
                            fmw_event_data_acquire(msg.data_wrapper); // გავზარდოთ ref_count თითოეული მიმღებისთვის
                        }
                        sub->module->base.handle_event(sub->module, msg.event_id, msg.data_wrapper);
                    }
                }
            }

            // გავათავისუფლოთ საწყისი reference, რომელიც `post` ფუნქციამ შექმნა
            if (msg.data_wrapper) {
                fmw_event_data_release(msg.data_wrapper);
            }
        }
    }
}


/**
 * @internal
 * @brief უზრუნველყოფს, რომ გამოწერების მასივი საკმარისი ზომისაა მითითებული event_id-სთვის.
 * @details ეს ფუნქცია უნდა გამოიძახოს მხოლოდ `mutex`-ით დაცულ კრიტიკულ სექციაში.
 * @param event_id შესამოწმებელი ივენთის ID.
 * @return esp_err_t ოპერაციის სტატუსი.
 */
static esp_err_t ensure_subscription_capacity(core_framework_event_id_t event_id) {
    if (event_id >= event_subscriptions_capacity) {
        size_t new_capacity = event_id + 16; // გაზრდის სტრატეგია
        ESP_LOGI(TAG, "Resizing subscription array from %d to %d", event_subscriptions_capacity, new_capacity);
        
        event_subscription_list_t **new_array = realloc(event_subscriptions, new_capacity * sizeof(event_subscription_list_t *));
        if (!new_array) {
            ESP_LOGE(TAG, "Failed to reallocate memory for subscriptions!");
            return ESP_ERR_NO_MEM;
        }
        
        // ახალი მეხსიერების განულება
        memset(&new_array[event_subscriptions_capacity], 0, (new_capacity - event_subscriptions_capacity) * sizeof(event_subscription_list_t *));
        
        event_subscriptions = new_array;
        event_subscriptions_capacity = new_capacity;
    }
    return ESP_OK;
}

// --- საჯარო API ფუნქციები ---

esp_err_t fmw_event_bus_init(void) {
    ESP_LOGI(TAG, "Initializing Event Bus...");
    subscription_mutex = xSemaphoreCreateMutex();
    if (!subscription_mutex) {
        ESP_LOGE(TAG, "Failed to create subscription mutex.");
        return ESP_ERR_NO_MEM;
    }

    event_bus_queue = xQueueCreate(CONFIG_FMW_EVENT_QUEUE_LENGTH, sizeof(event_message_t));
    if (!event_bus_queue) {
        ESP_LOGE(TAG, "Failed to create event queue.");
        vSemaphoreDelete(subscription_mutex);
        subscription_mutex = NULL;
        return ESP_ERR_NO_MEM;
    }

    // საწყისი მეხსიერების გამოყოფა
    event_subscriptions_capacity = 32; // საწყისი ზომა
    event_subscriptions = calloc(event_subscriptions_capacity, sizeof(event_subscription_list_t *));
     if (!event_subscriptions) {
        ESP_LOGE(TAG, "Failed to allocate initial subscription array.");
        vQueueDelete(event_bus_queue);
        vSemaphoreDelete(subscription_mutex);
        return ESP_ERR_NO_MEM;
    }

    BaseType_t result = xTaskCreate(event_bus_task, "event_bus_task", CONFIG_FMW_EVENT_BUS_TASK_STACK_SIZE, NULL, CONFIG_FMW_EVENT_BUS_TASK_PRIORITY, NULL);
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create event bus task.");
        free(event_subscriptions);
        vQueueDelete(event_bus_queue);
        vSemaphoreDelete(subscription_mutex);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Event Bus initialized and task started.");
    return ESP_OK;
}

esp_err_t fmw_event_bus_post(core_framework_event_id_t event_id, event_data_wrapper_t *data_wrapper) {
    if (event_id >= MAX_CORE_FRAMEWORK_EVENT) { // ვალიდაცია
        ESP_LOGE(TAG, "Invalid event ID: %d", event_id);
        return ESP_ERR_INVALID_ARG;
    }
    
    event_message_t msg = {
        .event_id = event_id,
        .data_wrapper = data_wrapper,
    };
    
    // გაგზავნამდე გავზარდოთ ref_count, რადგან ტასკი მას გამოიყენებს
    if (data_wrapper) {
        fmw_event_data_acquire(data_wrapper);
    }

    if (xQueueSend(event_bus_queue, &msg, pdMS_TO_TICKS(CONFIG_FMW_TASK_QUEUE_TIMEOUT_MS)) != pdPASS) {
        ESP_LOGE(TAG, "Failed to post event %d. Queue might be full.", event_id);
        // თუ გაგზავნა ვერ მოხერხდა, დავაკლოთ ref_count
        if (data_wrapper) {
            fmw_event_data_release(data_wrapper);
        }
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t fmw_event_bus_subscribe(core_framework_event_id_t event_id, struct module_t *module) {
    if (!module || module->name[0] == '\0') {
        return ESP_ERR_INVALID_ARG;
    }
    
    esp_err_t ret = ESP_OK;
    if (xSemaphoreTake(subscription_mutex, pdMS_TO_TICKS(CONFIG_FMW_MUTEX_TIMEOUT_MS)) == pdTRUE) {
        // 1. დავრწმუნდეთ, რომ მასივი საკმარისი ზომისაა
        ret = ensure_subscription_capacity(event_id);
        if (ret != ESP_OK) {
            xSemaphoreGive(subscription_mutex);
            return ret;
        }

        // 2. შევქმნათ გამოწერების სია, თუ ის არ არსებობს
        if (event_subscriptions[event_id] == NULL) {
            event_subscriptions[event_id] = calloc(1, sizeof(event_subscription_list_t));
            if (!event_subscriptions[event_id]) {
                ESP_LOGE(TAG, "Failed to allocate memory for subscription list for event %d", event_id);
                xSemaphoreGive(subscription_mutex);
                return ESP_ERR_NO_MEM;
            }
        }
        
        event_subscription_list_t *sub_list = event_subscriptions[event_id];
        
        // 3. შევამოწმოთ, ხომ არ არის უკვე გამოწერილი
        for (uint8_t i = 0; i < sub_list->count; i++) {
            if (sub_list->subscribers[i].module == module) {
                ESP_LOGW(TAG, "Module '%s' is already subscribed to event %d", module->name, event_id);
                xSemaphoreGive(subscription_mutex);
                return ESP_OK; // უკვე გამოწერილია, შეცდომა არ არის
            }
        }

        // 4. დავამატოთ ახალი გამომწერი, თუ ლიმიტი არ არის მიღწეული
        if (sub_list->count < CONFIG_FMW_MAX_SUBSCRIBERS_PER_EVENT) {
            sub_list->subscribers[sub_list->count].module = module;
            sub_list->count++;
            ESP_LOGI(TAG, "Module '%s' subscribed successfully to event ID %d", module->name, event_id);
        } else {
            ESP_LOGE(TAG, "Cannot subscribe module '%s' to event %d. Subscriber limit reached.", module->name, event_id);
            ret = ESP_ERR_NO_MEM;
        }
        
        xSemaphoreGive(subscription_mutex);
    } else {
        ESP_LOGE(TAG, "Failed to acquire subscription mutex for module '%s' and event %d", module->name, event_id);
        ret = ESP_ERR_TIMEOUT;
    }
    return ret;
}