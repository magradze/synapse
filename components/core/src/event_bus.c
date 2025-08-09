/**
 * @file event_bus.c
 * @brief Event Bus კომპონენტის იმპლემენტაცია.
 * @version 2.1
 * @date 2025-06-27
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს Event Bus-ის იმპლემენტაციას, რომელიც ამუშავებს
 *          ასინქრონულ ივენთებს FreeRTOS-ის რიგებისა და ტასკების გამოყენებით.
 */
#include "event_bus.h"
#include "logging.h"
#include "base_module.h"
#include "event_data_wrapper.h" // <--- დამატებულია სრული განმარტებისთვის
#include "framework_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("EVENT_BUS", SYNAPSE_LOG_COLOR_BLUE);

// --- შიდა სტრუქტურები ---

/**
 * @internal
 * @brief აღწერს ერთ გამომწერს (მოდულს).
 */
typedef struct {
    module_t *module; /**< @brief მაჩვენებელი გამომწერ მოდულზე. */
} event_subscriber_t;

/**
 * @internal
 * @brief აღწერს ერთი ივენთის ყველა გამომწერის სიას.
 */
typedef struct {
    event_subscriber_t subscribers[CONFIG_SYNAPSE_MAX_SUBSCRIBERS_PER_EVENT]; /**< @brief გამომწერების მასივი. */
    uint8_t count;                                                      /**< @brief გამომწერების მიმდინარე რაოდენობა. */
} event_subscription_list_t;

/**
 * @internal
 * @brief გამოწერის კვანძი, რომელიც აკავშირებს ივენთის სახელს გამომწერების სიასთან.
 *        გამოიყენება დაკავშირებულ სიაში.
 */
typedef struct event_subscription_node_t
{
    char *event_name;                            /**< @brief ივენთის უნიკალური სახელი. */
    event_subscription_list_t subscription_list; /**< @brief ამ ივენთის გამომწერების სია. */
    struct event_subscription_node_t *next;      /**< @brief მაჩვენებელი შემდეგ კვანძზე სიაში. */
} event_subscription_node_t;

/**
 * @internal
 * @brief აღწერს ერთ ივენთს, რომელიც იგზავნება რიგში.
 */
typedef struct {
    char *event_name;                   /**< @brief ივენთის სახელი (დინამიურად გამოყოფილი). */
    event_data_wrapper_t *data_wrapper; /**< @brief მაჩვენებელი ივენთის მონაცემებზე. */
} event_message_t;

// --- კომპონენტის შიდა ცვლადები ---

/**
 * @internal
 * @brief გამოწერების დაკავშირებული სიის თავი.
 */
static event_subscription_node_t *subscription_head = NULL;

static QueueHandle_t event_bus_queue = NULL;
static SemaphoreHandle_t subscription_mutex = NULL;

// --- შიდა ფუნქციების წინასწარი დეკლარაცია ---
static void event_bus_task(void *pvParameters);
static void remove_subscriber(event_subscription_list_t *sub_list, int index);
static void dispatch_event_to_subscribers(const event_message_t *msg);
static event_subscription_node_t *find_subscription_node(const char *event_name, bool create_if_not_found);

/**
 * @internal
 * @brief Event Bus-ის მთავარი ტასკი, რომელიც ამუშავებს ივენთებს რიგიდან.
 */
static void event_bus_task(void *pvParameters)
{
    (void)pvParameters; // კომპილერის გაფრთხილების თავიდან ასაცილებლად
    event_message_t msg;
    while (1)
    {
        if (xQueueReceive(event_bus_queue, &msg, portMAX_DELAY) == pdPASS)
        {
            ESP_LOGD(TAG, "Processing event: '%s'", msg.event_name);

            dispatch_event_to_subscribers(&msg);

            // გავათავისუფლოთ ივენთის სახელისთვის გამოყოფილი მეხსიერება
            free(msg.event_name);
            msg.event_name = NULL;

            // გავათავისუფლოთ საწყისი reference, რომელიც `post` ფუნქციამ შექმნა
            if (msg.data_wrapper)
            {
                synapse_event_data_release(msg.data_wrapper);
            }
        }
    }
}

/**
 * @internal
 * @brief აგზავნის ერთ ივენთს ყველა შესაბამის გამომწერთან.
 * @details ეს ფუნქცია ეძებს როგორც კონკრეტულ (`event_name`), ისე ზოგად (`*`)
 *          გამომწერებს. რათა მინიმუმამდე დაიყვანოს Mutex-ის დაკავების დრო
 *          და თავიდან აიცილოს `race condition`-ები, ის ჯერ ქმნის გამომწერების
 *          სიების ლოკალურ ასლებს და მხოლოდ ამის შემდეგ იძახებს მათ `handle_event`
 *          ფუნქციებს.
 *
 * @param[in] msg მაჩვენებელი გასაგზავნ ივენთის შეტყობინებაზე (`event_message_t`).
 */
static void dispatch_event_to_subscribers(const event_message_t *msg)
{
    if (!msg || !msg->event_name)
    {
        return; // დავიცვათ თავი NULL მაჩვენებლებისგან
    }

    event_subscription_list_t specific_subs_copy;
    event_subscription_list_t wildcard_subs_copy;
    bool specific_found = false;
    bool wildcard_found = false;

    // --- კრიტიკული სექციის დასაწყისი ---
    if (xSemaphoreTake(subscription_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_MUTEX_TIMEOUT_MS)) == pdTRUE)
    {
        event_subscription_node_t *node = subscription_head;
        while (node != NULL)
        {
            // 1. ვეძებთ კონკრეტული ივენთის გამომწერებს
            if (strcmp(node->event_name, msg->event_name) == 0)
            {
                memcpy(&specific_subs_copy, &node->subscription_list, sizeof(event_subscription_list_t));
                specific_found = true;
            }
            // 2. ვეძებთ wildcard ("*") გამომწერებს
            if (strcmp(node->event_name, "*") == 0)
            {
                memcpy(&wildcard_subs_copy, &node->subscription_list, sizeof(event_subscription_list_t));
                wildcard_found = true;
            }
            node = node->next;
        }
        xSemaphoreGive(subscription_mutex);
    }
    else
    {
        ESP_LOGE(TAG, "დისპეტჩერმა ვერ შეძლო Mutex-ის დაკავება ივენთისთვის: '%s'", msg->event_name);
        return; // თუ Mutex-ს ვერ ვიღებთ, ოპერაციას ვაუქმებთ
    }
    // --- კრიტიკული სექციის დასასრული ---

    // --- ივენთების გაგზავნა (Mutex-ის გარეთ) ---

    // 3. ჯერ ვუგზავნით კონკრეტულ გამომწერებს
    if (specific_found)
    {
        ESP_LOGD(TAG, "ივენთის '%s' გაგზავნა %d კონკრეტულ გამომწერზე", msg->event_name, specific_subs_copy.count);
        for (int i = 0; i < specific_subs_copy.count; i++)
        {
            event_subscriber_t *sub = &specific_subs_copy.subscribers[i];
            if (sub->module && sub->module->base.handle_event)
            {
                if (msg->data_wrapper)
                {
                    synapse_event_data_acquire(msg->data_wrapper);
                }
                sub->module->base.handle_event(sub->module, msg->event_name, msg->data_wrapper);
            }
        }
    }

    // 4. შემდეგ ვუგზავნით wildcard გამომწერებს (მაგ. ლოგერს)
    if (wildcard_found)
    {
        ESP_LOGD(TAG, "ივენთის '%s' გაგზავნა %d ზოგად (wildcard) გამომწერზე", msg->event_name, wildcard_subs_copy.count);
        for (int i = 0; i < wildcard_subs_copy.count; i++)
        {
            event_subscriber_t *sub = &wildcard_subs_copy.subscribers[i];
            // დავრწმუნდეთ, რომ კონკრეტულმა გამომწერმა ივენთი მეორედ არ მიიღო
            bool already_sent = false;
            if (specific_found)
            {
                for (int j = 0; j < specific_subs_copy.count; j++)
                {
                    if (specific_subs_copy.subscribers[j].module == sub->module)
                    {
                        already_sent = true;
                        break;
                    }
                }
            }

            if (!already_sent && sub->module && sub->module->base.handle_event)
            {
                if (msg->data_wrapper)
                {
                    synapse_event_data_acquire(msg->data_wrapper);
                }
                sub->module->base.handle_event(sub->module, msg->event_name, msg->data_wrapper);
            }
        }
    }
}

/**
 * @internal
 * @brief პოულობს გამოწერის კვანძს ივენთის სახელის მიხედვით.
 * @param event_name მოსაძებნი ივენთის სახელი.
 * @param create_if_not_found თუ true, შექმნის ახალ კვანძს თუ ვერ იპოვა.
 * @return მაჩვენებელი ნაპოვნ ან ახლად შექმნილ კვანძზე, ან NULL თუ ვერ იპოვა და create_if_not_found არის false.
 * @note ეს ფუნქცია უნდა გამოიძახოს მხოლოდ subscription_mutex-ის დაცულ კრიტიკულ სექციაში.
 */
static event_subscription_node_t *find_subscription_node(const char *event_name, bool create_if_not_found)
{
    event_subscription_node_t *node = subscription_head;
    event_subscription_node_t *prev = NULL;

    // კვანძის ძებნა
    while (node != NULL)
    {
        if (strcmp(node->event_name, event_name) == 0)
        {
            return node;
        }
        prev = node;
        node = node->next;
    }

    // თუ კვანძი ვერ მოიძებნა და საჭიროა მისი შექმნა
    if (create_if_not_found)
    {
        event_subscription_node_t *new_node = (event_subscription_node_t *)calloc(1, sizeof(event_subscription_node_t));
        if (!new_node)
        {
            ESP_LOGE(TAG, "Failed to allocate memory for new subscription node for event '%s'", event_name);
            return NULL;
        }

        new_node->event_name = strdup(event_name);
        if (!new_node->event_name)
        {
            ESP_LOGE(TAG, "Failed to duplicate event name string for '%s'", event_name);
            free(new_node);
            return NULL;
        }

        // ახალი კვანძის დამატება სიის ბოლოში (ან თავში თუ სია ცარიელია)
        if (prev == NULL)
        {
            subscription_head = new_node;
        }
        else
        {
            prev->next = new_node;
        }
        ESP_LOGD(TAG, "Created new subscription node for event '%s'", event_name);
        return new_node;
    }

    return NULL;
}

/**
 * @internal
 * @brief შლის გამომწერს სიიდან მითითებულ ინდექსზე.
 */
static void remove_subscriber(event_subscription_list_t *sub_list, int index)
{
    if (sub_list == NULL || index < 0 || (unsigned int)index >= sub_list->count)
    {
        return;
    }
    // ელემენტების მარცხნივ გადაწევა სიცარიელის ამოსავსებლად
    for (uint8_t i = (uint8_t)index; i < sub_list->count - 1; i++)
    {
        sub_list->subscribers[i] = sub_list->subscribers[i + 1];
    }
    sub_list->count--;
}

// --- საჯარო API ფუნქციები ---

esp_err_t synapse_event_bus_init(void)
{
    ESP_LOGI(TAG, "Initializing Event Bus...");
    if (subscription_mutex != NULL)
    {
        ESP_LOGW(TAG, "Event Bus is already initialized.");
        return ESP_ERR_INVALID_STATE;
    }

    subscription_mutex = xSemaphoreCreateMutex();
    if (!subscription_mutex) {
        ESP_LOGE(TAG, "Failed to create subscription mutex.");
        return ESP_ERR_NO_MEM;
    }

    event_bus_queue = xQueueCreate(CONFIG_SYNAPSE_EVENT_QUEUE_LENGTH, sizeof(event_message_t));
    if (!event_bus_queue) {
        ESP_LOGE(TAG, "Failed to create event queue.");
        vSemaphoreDelete(subscription_mutex);
        subscription_mutex = NULL;
        return ESP_ERR_NO_MEM;
    }

    // აღარ არის საჭირო `event_subscriptions` მასივის წინასწარი გამოყოფა
    subscription_head = NULL;

    BaseType_t result = xTaskCreate(event_bus_task, "event_bus_task", CONFIG_SYNAPSE_EVENT_BUS_TASK_STACK_SIZE, NULL, CONFIG_SYNAPSE_EVENT_BUS_TASK_PRIORITY, NULL);
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create event bus task.");
        // აქ საჭიროა გამოწერების სიის გასუფთავება, თუმცა ამ ეტაპზე ის ყოველთვის ცარიელია.
        vQueueDelete(event_bus_queue);
        vSemaphoreDelete(subscription_mutex);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Event Bus initialized and task started.");
    return ESP_OK;
}

esp_err_t synapse_event_bus_post(const char *event_name, event_data_wrapper_t *data_wrapper)
{
    if (!event_name || strlen(event_name) == 0)
    {
        ESP_LOGE(TAG, "Invalid event name: NULL or empty string.");
        return ESP_ERR_INVALID_ARG;
    }

    char *event_name_copy = strdup(event_name);
    if (!event_name_copy)
    {
        ESP_LOGE(TAG, "Failed to duplicate event name '%s'. Out of memory.", event_name);
        return ESP_ERR_NO_MEM;
    }

    event_message_t msg = {
        .event_name = event_name_copy,
        .data_wrapper = data_wrapper,
    };

    if (data_wrapper) {
        synapse_event_data_acquire(data_wrapper);
    }

    if (xQueueSend(event_bus_queue, &msg, pdMS_TO_TICKS(CONFIG_SYNAPSE_TASK_QUEUE_TIMEOUT_MS)) != pdPASS)
    {
        ESP_LOGE(TAG, "Failed to post event '%s'. Queue might be full.", event_name);
        free(event_name_copy); // გავათავისუფლოთ ასლი თუ რიგში ვერ ჩაიწერა
        if (data_wrapper) {
            synapse_event_data_release(data_wrapper);
        }
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t synapse_event_bus_subscribe(const char *event_name, module_t *module)
{
    if (!event_name || strlen(event_name) == 0)
    {
        ESP_LOGE(TAG, "Subscribe failed: event_name is NULL or empty.");
        return ESP_ERR_INVALID_ARG;
    }

    if (!module || !module->base.handle_event)
    {
        ESP_LOGE(TAG, "Subscribe failed: module or its event handler is NULL.");
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = ESP_OK;
    if (xSemaphoreTake(subscription_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_MUTEX_TIMEOUT_MS)) == pdTRUE)
    {
        event_subscription_node_t *node = find_subscription_node(event_name, true);

        if (!node)
        {
            ESP_LOGE(TAG, "Failed to find or create subscription node for event '%s'", event_name);
            xSemaphoreGive(subscription_mutex);
            return ESP_ERR_NO_MEM; // find_subscription_node-მა უკვე დაწერა ლოგი
        }

        event_subscription_list_t *sub_list = &node->subscription_list;

        // შევამოწმოთ, მოდული უკვე გამოწერილი ხომ არ არის
        for (uint8_t i = 0; i < sub_list->count; i++) {
            if (sub_list->subscribers[i].module == module) {
                ESP_LOGW(TAG, "Module '%s' is already subscribed to event '%s'", module->name, event_name);
                xSemaphoreGive(subscription_mutex);
                return ESP_OK; // არ არის შეცდომა, უბრალოდ უკვე გამოწერილია
            }
        }

        // ახალი გამომწერის დამატება
        if (sub_list->count < CONFIG_SYNAPSE_MAX_SUBSCRIBERS_PER_EVENT)
        {
            sub_list->subscribers[sub_list->count].module = module;
            sub_list->count++;
            ESP_LOGI(TAG, "Module '%s' subscribed successfully to event '%s'", module->name, event_name);
        }
        else
        {
            ESP_LOGE(TAG, "Cannot subscribe module '%s' to event '%s'. Subscriber limit reached.", module->name, event_name);
            ret = ESP_ERR_NO_MEM;
        }

        xSemaphoreGive(subscription_mutex);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to acquire subscription mutex for module '%s' and event '%s'", module->name, event_name);
        ret = ESP_ERR_TIMEOUT;
    }
    return ret;
}

esp_err_t synapse_event_bus_unsubscribe(const char *event_name, module_t *module)
{
    if (!event_name || strlen(event_name) == 0 || !module)
    {
        return ESP_ERR_INVALID_ARG;
    }

    if (xSemaphoreTake(subscription_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_MUTEX_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Failed to acquire subscription mutex for unsubscribe (module '%s', event '%s')", module->name, event_name);
        return ESP_ERR_TIMEOUT;
    }

    esp_err_t ret = ESP_ERR_NOT_FOUND;
    event_subscription_node_t *node = subscription_head;
    event_subscription_node_t *prev = NULL;

    while (node != NULL)
    {
        if (strcmp(node->event_name, event_name) == 0)
        {
            event_subscription_list_t *sub_list = &node->subscription_list;
            for (int i = 0; i < sub_list->count; i++)
            {
                if (sub_list->subscribers[i].module == module)
                {
                    remove_subscriber(sub_list, i);
                    ESP_LOGI(TAG, "Module '%s' unsubscribed successfully from event '%s'", module->name, event_name);
                    ret = ESP_OK;

                    // თუ ამ ივენთს გამომწერები აღარ დარჩა, წავშალოთ კვანძი
                    if (sub_list->count == 0)
                    {
                        ESP_LOGD(TAG, "Removing empty subscription node for event '%s'", event_name);
                        if (prev == NULL)
                        { // თუ პირველი ელემენტია
                            subscription_head = node->next;
                        }
                        else
                        {
                            prev->next = node->next;
                        }
                        free(node->event_name);
                        free(node);
                    }
                    goto cleanup; // გამოვიდეთ ორივე ციკლიდან
                }
            }
            // თუ ციკლი დასრულდა და მოდული ვერ ვიპოვეთ ამ კვანძში
            goto cleanup;
        }
        prev = node;
        node = node->next;
    }

cleanup:
    if (ret == ESP_ERR_NOT_FOUND)
    {
        ESP_LOGW(TAG, "Module '%s' was not subscribed to event '%s'", module->name, event_name);
    }

    xSemaphoreGive(subscription_mutex);
    return ret;
}