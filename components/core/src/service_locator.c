/**
 * @file service_locator.c
 * @brief Service Locator პატერნის იმპლემენტაცია.
 * @date 2025-06-26
 * @version 1.1
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს Service Locator პატერნის კონკრეტულ იმპლემენტაციას.
 *          ის საშუალებას აძლევს მოდულებს, დინამიურად იპოვონ და გამოიყენონ
 *          სხვა მოდულების API-ები პირდაპირი, compile-time დამოკიდებულებების გარეშე.
 *
 *          ძირითადი მახასიათებლები:
 *          - სერვისის რეგისტრაცია უნიკალური სახელითა და ტიპით.
 *          - სერვისის მოძიება სახელით.
 *          - სერვისის ტიპის მოძიება.
 *          - ნაკად-უსაფრთხო (thread-safe) ოპერაციები FreeRTOS mutex-ის გამოყენებით.
 *
 *          იმპლემენტაცია იყენებს ცალმხრივად დაკავშირებულ სიას (singly-linked list)
 *          რეგისტრირებული სერვისების შესანახად და mutex-ს ერთდროული წვდომის
 *          უსაფრთხოდ სამართავად.
 */
#include "service_locator.h"
#include "logging.h"
#include "framework_config.h" // Kconfig პარამეტრებისთვის
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <string.h>
#include <stdlib.h>
#include <sys/queue.h> // SLIST-ისთვის

DEFINE_COMPONENT_TAG("SERVICE_LOCATOR", SYNAPSE_LOG_COLOR_BLUE);

/**
 * @internal
 * @struct service_entry_t
 * @brief განსაზღვრავს ერთი რეგისტრირებული სერვისის ჩანაწერის სტრუქტურას.
 *
 * @details ეს შიდა სტრუქტურა ინახავს სერვისის ყველა მეტამონაცემს,
 *          მათ შორის მის სახელს, ტიპს და მისამართს მის Public API სტრუქტურაზე.
 *          ეს ჩანაწერები ქმნიან ცალმხრივად დაკავშირებულ სიას სერვისების რეესტრისთვის.
 */
typedef struct service_entry_t {
    char name[CONFIG_SYNAPSE_SERVICE_NAME_MAX_LENGTH]; /**< @brief სერვისის უნიკალური სახელი. */
    synapse_service_type_t type;                       /**< @brief სერვისის ტიპი (enum). */
    void *service_handle;                          /**< @brief მაჩვენებელი სერვისის API სტრუქტურაზე. */
    SLIST_ENTRY(service_entry_t) entries;          /**< @brief მაჩვენებელი სიის შემდეგ ელემენტზე. */
} service_entry_t;

/**
 * @internal
 * @brief რეგისტრირებული სერვისების სიის "თავი" (head).
 *
 * @details ეს არის ცალმხრივად დაკავშირებული სიის თავი, რომელიც ინახავს
 *          ყველა რეგისტრირებული სერვისის ჩანაწერს.
 */
static SLIST_HEAD(service_list_head, service_entry_t) service_registry_head;

/**
 * @internal
 * @brief Mutex სერვისების რეესტრზე წვდომის სინქრონიზაციისთვის.
 *
 * @details ეს mutex-ი უზრუნველყოფს ნაკად-უსაფრთხო წვდომას, როდესაც ხდება
 *          სერვისების რეესტრში ჩანაწერების რეგისტრაცია, მოძიება ან ცვლილება.
 */
static SemaphoreHandle_t service_registry_mutex = NULL;

esp_err_t synapse_service_locator_init(void)
{
    // შევამოწმოთ, ხომ არ არის უკვე ინიციალიზებული
    if (service_registry_mutex != NULL) {
        ESP_LOGW(TAG, "Service Locator უკვე ინიციალიზებულია.");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "Service Locator-ის ინიციალიზაცია...");
    SLIST_INIT(&service_registry_head);
    service_registry_mutex = xSemaphoreCreateMutex();
    if (service_registry_mutex == NULL) {
        ESP_LOGE(TAG, "Service registry mutex-ის შექმნა ვერ მოხერხდა!");
        return ESP_ERR_NO_MEM;
    }
    ESP_LOGI(TAG, "Service Locator წარმატებით ინიციალიზდა.");
    return ESP_OK;
}

esp_err_t synapse_service_register(const char *service_name, synapse_service_type_t service_type, service_handle_t service_handle)
{
    if (!service_name || !service_handle) {
        ESP_LOGE(TAG, "რეგისტრაცია ვერ მოხერხდა: არასწორი არგუმენტები (სახელი ან handle არის NULL).");
        return ESP_ERR_INVALID_ARG;
    }

    if (xSemaphoreTake(service_registry_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_SEMAPHORE_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "რეგისტრაციისთვის service registry mutex-ის დაკავება ვერ მოხერხდა.");
        return ESP_ERR_TIMEOUT;
    }

    // შევამოწმოთ, ხომ არ არის უკვე რეგისტრირებული სერვისი იგივე სახელით
    service_entry_t *it;
    SLIST_FOREACH(it, &service_registry_head, entries) {
        if (strcmp(it->name, service_name) == 0) {
            ESP_LOGE(TAG, "სერვისი სახელით '%s' უკვე რეგისტრირებულია!", service_name);
            xSemaphoreGive(service_registry_mutex);
            return ESP_ERR_INVALID_STATE;
        }
    }

    // შევქმნათ ახალი ჩანაწერი
    service_entry_t *new_entry = malloc(sizeof(service_entry_t));
    if (!new_entry) {
        ESP_LOGE(TAG, "ახალი სერვისის ('%s') ჩანაწერისთვის მეხსიერების გამოყოფა ვერ მოხერხდა.", service_name);
        xSemaphoreGive(service_registry_mutex);
        return ESP_ERR_NO_MEM;
    }

    strncpy(new_entry->name, service_name, sizeof(new_entry->name) - 1);
    new_entry->name[sizeof(new_entry->name) - 1] = '\0';
    new_entry->type = service_type;
    new_entry->service_handle = service_handle;

    SLIST_INSERT_HEAD(&service_registry_head, new_entry, entries);

    xSemaphoreGive(service_registry_mutex);

    ESP_LOGI(TAG, "სერვისი '%s' (ტიპი: '%s') წარმატებით დარეგისტრირდა.", service_name, synapse_service_type_to_string(service_type));
    return ESP_OK;
}

esp_err_t synapse_service_unregister(const char *service_name)
{
    if (!service_name)
    {
        ESP_LOGE(TAG, "რეგისტრაციის გაუქმება ვერ მოხერხდა: service_name არის NULL.");
        return ESP_ERR_INVALID_ARG;
    }

    if (xSemaphoreTake(service_registry_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_SEMAPHORE_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Unregister ოპერაციისთვის service registry mutex-ის დაკავება ვერ მოხერხდა.");
        return ESP_ERR_TIMEOUT;
    }

    service_entry_t *current_entry = SLIST_FIRST(&service_registry_head);
    service_entry_t *prev_entry = NULL;

    // ხელით იტერაცია სიის გასწვრივ
    while (current_entry != NULL)
    {
        if (strcmp(current_entry->name, service_name) == 0)
        {
            // ვიპოვეთ წასაშლელი ელემენტი

            if (prev_entry == NULL)
            {
                // წასაშლელი ელემენტი არის სიის თავში (head)
                SLIST_REMOVE_HEAD(&service_registry_head, entries);
            }
            else
            {
                // წასაშლელი ელემენტი არის სიის შუაში ან ბოლოში
                SLIST_REMOVE_AFTER(prev_entry, entries);
            }

            free(current_entry); // მეხსიერების გათავისუფლება
            xSemaphoreGive(service_registry_mutex);
            ESP_LOGI(TAG, "სერვისი '%s' წარმატებით გაუქმდა.", service_name);
            return ESP_OK;
        }

        // გადავდივართ შემდეგ ელემენტზე და ვიმახსოვრებთ წინას
        prev_entry = current_entry;
        current_entry = SLIST_NEXT(current_entry, entries);
    }

    // თუ ციკლი დასრულდა და ელემენტი ვერ მოიძებნა
    xSemaphoreGive(service_registry_mutex);
    ESP_LOGW(TAG, "სერვისი '%s' ვერ მოიძებნა რეგისტრაციის გაუქმებისას.", service_name);
    return ESP_ERR_NOT_FOUND;
}

service_handle_t synapse_service_get(const char *service_name)
{
    if (!service_name) {
        ESP_LOGE(TAG, "სერვისის მოძიება ვერ მოხერხდა: service_name არის NULL.");
        return NULL;
    }

    if (xSemaphoreTake(service_registry_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_SEMAPHORE_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Get ოპერაციისთვის service registry mutex-ის დაკავება ვერ მოხერხდა.");
        return NULL;
    }

    service_handle_t found_handle = NULL;
    service_entry_t *it;
    SLIST_FOREACH(it, &service_registry_head, entries) {
        if (strcmp(it->name, service_name) == 0) {
            found_handle = it->service_handle;
            break;
        }
    }

    xSemaphoreGive(service_registry_mutex);

    if (found_handle) {
        ESP_LOGD(TAG, "სერვისი '%s' ნაპოვნია.", service_name);
    } else {
        ESP_LOGW(TAG, "სერვისი '%s' ვერ მოიძებნა.", service_name);
    }

    return found_handle;
}

esp_err_t synapse_service_get_type(const char *service_name, synapse_service_type_t *out_service_type)
{
    if (!service_name || !out_service_type) {
        ESP_LOGE(TAG, "სერვისის ტიპის მოძიება ვერ მოხერხდა: service_name ან out_service_type არის NULL.");
        return ESP_ERR_INVALID_ARG;
    }

    if (xSemaphoreTake(service_registry_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_SEMAPHORE_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Get_type ოპერაციისთვის service registry mutex-ის დაკავება ვერ მოხერხდა.");
        return ESP_ERR_TIMEOUT;
    }

    bool found = false;
    service_entry_t *it;
    SLIST_FOREACH(it, &service_registry_head, entries) {
        if (strcmp(it->name, service_name) == 0) {
            *out_service_type = it->type;
            found = true;
            break;
        }
    }

    xSemaphoreGive(service_registry_mutex);

    if (found) {
        ESP_LOGD(TAG, "სერვისს '%s' აქვს ტიპი '%s'.", service_name, synapse_service_type_to_string(*out_service_type));
        return ESP_OK;
    } else {
        ESP_LOGW(TAG, "სერვისი '%s' ვერ მოიძებნა ტიპის მოთხოვნისას.", service_name);
        return ESP_ERR_NOT_FOUND;
    }
}

service_handle_t synapse_service_lookup_by_type(synapse_service_type_t service_type)
{
    // აქ შეიძლება enum-ის დიაპაზონის შემოწმება, თუ საჭიროა, მაგრამ NULL-ზე შემოწმება არასწორია.
    // მაგალითად: if (service_type >= SYNAPSE_SERVICE_TYPE_MAX) return NULL;

    if (xSemaphoreTake(service_registry_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_SEMAPHORE_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Lookup_by_type ოპერაციისთვის service registry mutex-ის დაკავება ვერ მოხერხდა.");
        return NULL;
    }

    service_handle_t found_handle = NULL;
    service_entry_t *it;
    SLIST_FOREACH(it, &service_registry_head, entries) {
        // ⭐️ მთავარი შესწორება: პირდაპირი შედარება
        if (it->type == service_type)
        {
            found_handle = it->service_handle;
            break; // დავაბრუნოთ პირველი ნაპოვნი
        }
    }

    xSemaphoreGive(service_registry_mutex);

    // ლოგირებისთვის ვიყენებთ დამხმარე ფუნქციას, რომ enum გადავიყვანოთ სტრიქონში
    if (found_handle) {
        ESP_LOGD(TAG, "ნაპოვნია სერვისი ტიპით '%s'.", synapse_service_type_to_string(service_type));
    } else {
        ESP_LOGW(TAG, "სერვისი ტიპით '%s' ვერ მოიძებნა.", synapse_service_type_to_string(service_type));
    }

    return found_handle;
}