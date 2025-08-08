/**
 * @file resource_manager.c
 * @brief რესურსების მენეჯერის ზოგადი იმპლემენტაცია.
 * @version 3.1
 * @date 2025-06-27
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს Synapse Framework-ის რესურსების მენეჯერის
 *          ზოგად იმპლემენტაციას. იგი შექმნილია სხვადასხვა ტიპის
 *          გაზიარებული რესურსების (მაგ. GPIO, ADC, I2C) კონფლიქტების გარეშე
 *          მართვისთვის. თითოეული რესურსის ტიპი იმართება საკუთარ, იზოლირებულ
 *          ნაკრებში (pool), რაც უზრუნველყოფს მაღალ წარმადობას და
 *          ნაკად-უსაფრთხოებას (thread-safety) ინდივიდუალური Mutex-ების
 *          გამოყენებით.
 *
 *          ძირითადი მახასიათებლები:
 *          - სხვადასხვა ტიპის რესურსების ზოგადი მართვა.
 *          - თითოეული რესურსის ტიპისთვის იზოლირებული და ნაკად-უსაფრთხო გარემო.
 *          - რესურსის მფლობელის აღრიცხვა არაავტორიზებული წვდომის თავიდან ასაცილებლად.
 *          - დეტალური, ქართულენოვანი ლოგირება პრობლემების მარტივი დიაგნოსტიკისთვის.
 *          - "Fail-safe" მიდგომა: მდგომარეობის შემოწმებისას, თუ რესურსის სტატუსი
 *            უცნობია (მაგ. Mutex-ის დაკავების შეცდომა), ის ჩაითვლება დაკავებულად.
 */
#include "resource_manager.h"
#include "logging.h"
#include "framework_config.h" // Kconfig პარამეტრებისთვის
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

DEFINE_COMPONENT_TAG("RESOURCE_MANAGER");

#define MAX_RESOURCES_PER_POOL 64 // Bitmap-ის ზომიდან გამომდინარე (uint64_t)

/**
 * @internal
 * @brief აღწერს ერთი ტიპის რესურსების ნაკრებს (pool).
 * @details თითოეული ნაკრები შეიცავს საკუთარ Mutex-ს, bitmap-ს დაკავებული
 *          რესურსების აღსარიცხად და მფლობელების სიას. ეს სტრუქტურა
 *          უზრუნველყოფს, რომ სხვადასხვა ტიპის რესურსებზე ოპერაციები
 *          ერთმანეთს ხელს არ უშლიან.
 */
typedef struct
{
    SemaphoreHandle_t pool_mutex;                    /**< Mutex ამ კონკრეტული ნაკრებისთვის. */
    uint64_t lock_bitmap;                            /**< Bitmap დაკავებული რესურსების აღსარიცხად. */
    const char *owner_names[MAX_RESOURCES_PER_POOL]; /**< მასივი რესურსების მფლობელების სახელების შესანახად. */
} resource_pool_t;

/**
 * @internal
 * @brief ყველა ტიპის რესურსის ნაკრებების გლობალური მასივი.
 * @details მასივის ინდექსად გამოიყენება `synapse_resource_type_t` ენამი.
 */
static resource_pool_t resource_pools[SYNAPSE_RESOURCE_TYPE_MAX];

esp_err_t synapse_resource_manager_init(void)
{
    ESP_LOGI(TAG, "რესურსების მენეჯერის ინიციალიზაცია...");

    for (int i = 0; i < SYNAPSE_RESOURCE_TYPE_MAX; i++)
    {
        resource_pool_t *pool = &resource_pools[i];

        pool->pool_mutex = xSemaphoreCreateMutex();
        if (pool->pool_mutex == NULL)
        {
            ESP_LOGE(TAG, "რესურსის ნაკრებისთვის (ტიპი: %d) Mutex-ის შექმნა ვერ მოხერხდა.", i);
            // უკვე შექმნილი Mutex-ების გასუფთავება
            synapse_resource_manager_deinit();
            return ESP_ERR_NO_MEM;
        }

        // საწყისი მდგომარეობის დასმა
        pool->lock_bitmap = 0;
        memset(pool->owner_names, 0, sizeof(pool->owner_names));
    }

    ESP_LOGI(TAG, "რესურსების მენეჯერი წარმატებით ინიციალიზდა.");
    return ESP_OK;
}

void synapse_resource_manager_deinit(void)
{
    ESP_LOGI(TAG, "რესურსების მენეჯერის დეინიციალიზაცია...");
    for (int i = 0; i < SYNAPSE_RESOURCE_TYPE_MAX; i++)
    {
        if (resource_pools[i].pool_mutex != NULL)
        {
            vSemaphoreDelete(resource_pools[i].pool_mutex);
            resource_pools[i].pool_mutex = NULL;
        }
    }
    ESP_LOGI(TAG, "რესურსების მენეჯერი წარმატებით დეინიციალიზდა.");
}

esp_err_t synapse_resource_lock(synapse_resource_type_t type, uint8_t resource_id, const char *owner)
{
    if (type >= SYNAPSE_RESOURCE_TYPE_MAX || resource_id >= MAX_RESOURCES_PER_POOL || owner == NULL)
    {
        ESP_LOGE(TAG, "რესურსის (ტიპი: %d, ID: %d) დაკავება ვერ მოხერხდა: არავალიდური არგუმენტები.", type, resource_id);
        return ESP_ERR_INVALID_ARG;
    }

    resource_pool_t *pool = &resource_pools[type];

    if (xSemaphoreTake(pool->pool_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_MUTEX_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "რესურსის (ტიპი: %d, ID: %d) დასაკავებლად Mutex-ის აღება ვერ მოხერხდა.", type, resource_id);
        return ESP_ERR_TIMEOUT;
    }

    esp_err_t ret = ESP_OK;

    if ((pool->lock_bitmap >> resource_id) & 1ULL)
    {
        ESP_LOGE(TAG, "რესურსის (ტიპი: %d, ID: %d) დაკავება მფლობელის '%s' მიერ ვერ მოხერხდა. ის უკვე დაკავებულია '%s'-ის მიერ.",
                 type, resource_id, owner, pool->owner_names[resource_id] ? pool->owner_names[resource_id] : "უცნობი");
        ret = ESP_ERR_INVALID_STATE;
    }
    else
    {
        pool->lock_bitmap |= (1ULL << resource_id);
        pool->owner_names[resource_id] = owner;
        ESP_LOGI(TAG, "რესურსი (ტიპი: %d, ID: %d) წარმატებით დაიკავა '%s'-მა.", type, resource_id, owner);
    }

    xSemaphoreGive(pool->pool_mutex);
    return ret;
}

esp_err_t synapse_resource_release(synapse_resource_type_t type, uint8_t resource_id, const char *owner)
{
    if (type >= SYNAPSE_RESOURCE_TYPE_MAX || resource_id >= MAX_RESOURCES_PER_POOL || owner == NULL)
    {
        ESP_LOGE(TAG, "რესურსის (ტიპი: %d, ID: %d) გათავისუფლება ვერ მოხერხდა: არავალიდური არგუმენტები.", type, resource_id);
        return ESP_ERR_INVALID_ARG;
    }

    resource_pool_t *pool = &resource_pools[type];

    if (xSemaphoreTake(pool->pool_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_MUTEX_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "რესურსის (ტიპი: %d, ID: %d) გასათავისუფლებლად Mutex-ის აღება ვერ მოხერხდა.", type, resource_id);
        return ESP_ERR_TIMEOUT;
    }

    esp_err_t ret = ESP_OK;

    if (!((pool->lock_bitmap >> resource_id) & 1ULL))
    {
        ESP_LOGW(TAG, "'%s'-ის მცდელობა გაათავისუფლოს რესურსი (ტიპი: %d, ID: %d), რომელიც დაკავებული არ არის.",
                 owner, type, resource_id);
        ret = ESP_ERR_NOT_FOUND;
    }
    else if (strcmp(pool->owner_names[resource_id], owner) != 0)
    {
        ESP_LOGE(TAG, "რესურსის (ტიპი: %d, ID: %d) გათავისუფლება ვერ მოხერხდა. მფლობელი არ ემთხვევა. დაკავებულია '%s'-ის მიერ, მცდელობაა '%s'-ის მიერ.",
                 type, resource_id, pool->owner_names[resource_id], owner);
        ret = ESP_ERR_INVALID_STATE;
    }
    else
    {
        pool->lock_bitmap &= ~(1ULL << resource_id);
        pool->owner_names[resource_id] = NULL;
        ESP_LOGI(TAG, "რესურსი (ტიპი: %d, ID: %d) წარმატებით გაათავისუფლა '%s'-მა.", type, resource_id, owner);
    }

    xSemaphoreGive(pool->pool_mutex);
    return ret;
}

bool synapse_resource_is_locked(synapse_resource_type_t type, uint8_t resource_id)
{
    if (type >= SYNAPSE_RESOURCE_TYPE_MAX || resource_id >= MAX_RESOURCES_PER_POOL)
    {
        ESP_LOGE(TAG, "რესურსის (ტიპი: %d, ID: %d) სტატუსის შემოწმება ვერ მოხერხდა: არავალიდური არგუმენტები.", type, resource_id);
        return true; // Fail-safe: თუ არგუმენტი არასწორია, ჩავთვალოთ დაკავებულად.
    }

    resource_pool_t *pool = &resource_pools[type];

    if (xSemaphoreTake(pool->pool_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_MUTEX_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "რესურსის (ტიპი: %d, ID: %d) სტატუსის შესამოწმებლად Mutex-ის აღება ვერ მოხერხდა. ვიღებთ fail-safe გადაწყვეტილებას.", type, resource_id);
        return true; // Fail-safe: თუ მდგომარეობა უცნობია, ვთვლით რომ დაკავებულია კონფლიქტის თავიდან ასაცილებლად.
    }

    bool is_locked = ((pool->lock_bitmap >> resource_id) & 1ULL);

    xSemaphoreGive(pool->pool_mutex);

    return is_locked;
}

/**
 * @brief იღებს მითითებული რესურსის მფლობელის სახელს.
 * @details ეს ფუნქცია ამოწმებს, თუ ვინ არის კონკრეტული რესურსის მფლობელი.
 *          თუ რესურსი დაკავებული არ არის, `out_owner` იქნება NULL, მაგრამ ფუნქცია
 *          დააბრუნებს ESP_OK-ს.
 *
 * @param[in]  type        რესურსის ტიპი (`synapse_resource_type_t`).
 * @param[in]  resource_id რესურსის უნიკალური ID (0-63).
 * @param[out] out_owner   ორმაგი პოინტერი, სადაც ჩაიწერება მფლობელის სახელის პოინტერი.
 *                         შეცდომის ან თუ რესურსი თავისუფალია, იქნება NULL.
 *
 * @return
 *      - ESP_OK: ოპერაცია წარმატებით დასრულდა.
 *      - ESP_ERR_INVALID_ARG: თუ `type`, `resource_id` ან `out_owner` არავალიდურია.
 *      - ESP_ERR_TIMEOUT: თუ Mutex-ის აღება ვერ მოხერხდა განსაზღვრულ დროში.
 */
esp_err_t synapse_resource_get_owner(synapse_resource_type_t type, uint8_t resource_id, const char **out_owner)
{
    if (type >= SYNAPSE_RESOURCE_TYPE_MAX || resource_id >= MAX_RESOURCES_PER_POOL || out_owner == NULL)
    {
        ESP_LOGE(TAG, "რესურსის (ტიპი: %d, ID: %d) მფლობელის მიღება ვერ მოხერხდა: არავალიდური არგუმენტები.", type, resource_id);
        if (out_owner != NULL)
            *out_owner = NULL;
        return ESP_ERR_INVALID_ARG;
    }

    *out_owner = NULL; // საწყისი მნიშვნელობა

    resource_pool_t *pool = &resource_pools[type];

    if (xSemaphoreTake(pool->pool_mutex, pdMS_TO_TICKS(CONFIG_SYNAPSE_MUTEX_TIMEOUT_MS)) != pdTRUE)
    {
        ESP_LOGE(TAG, "რესურსის (ტიპი: %d, ID: %d) მფლობელის წასაკითხად Mutex-ის აღება ვერ მოხერხდა.", type, resource_id);
        return ESP_ERR_TIMEOUT;
    }

    if ((pool->lock_bitmap >> resource_id) & 1ULL)
    {
        *out_owner = pool->owner_names[resource_id];
    }

    xSemaphoreGive(pool->pool_mutex);

    return ESP_OK;
}