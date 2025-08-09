/**
 * @file module_factory.c
 * @brief მოდულების შემქმნელი (Factory) კომპონენტის იმპლემენტაცია.
 * @version 1.2
 * @date 2025-06-27
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს Module Factory-ის Public API-ს იმპლემენტაციას.
 *          იგი იყენებს ავტომატურად გენერირებულ `synapse_module_factory_get` ფუნქციას,
 *          რათა დააკავშიროს მოდულის ტიპის სტრიქონი მის შესაბამის შემქმნელ ფუნქციასთან.
 *          ეს მიდგომა უზრუნველყოფს, რომ Factory-ის კოდი არ საჭიროებს ცვლილებას
 *          ახალი მოდულების დამატებისას.
 */
#include "module_factory.h"
#include "logging.h"
#include <stddef.h> // NULL-ისთვის

// ეს არის ავტომატურად გენერირებული ჰედერი, რომელიც შეიცავს synapse_module_factory_get-ის დეკლარაციას.
// იდეალურ შემთხვევაში, ბილდ-სისტემამ უნდა უზრუნველყოს ამ ჰედერის ხელმისაწვდომობა.
// Forward declaration გამოიყენება როგორც ალტერნატივა, თუ include-ის დამატება რთულია.
#if defined(SYNAPSE_USE_GENERATED_FACTORY_HEADER)
#include "generated_module_factory.h"
#else
// Forward-declare the function if the generated header is not used/available.
module_create_fn_t synapse_module_factory_get(const char *module_type);
#endif

DEFINE_COMPONENT_TAG("MODULE_FACTORY", SYNAPSE_LOG_COLOR_BLUE);

// --- Public API იმპლემენტაცია ---

/**
 * @brief ქმნის მოდულის ახალ ინსტანციას მითითებული ტიპისა და კონფიგურაციის მიხედვით.
 *
 * @details ეს ფუნქცია არის Synapse Framework-ის მოდულების დინამიური შექმნის
 *          ცენტრალური წერტილი. ის იღებს მოდულის ტიპს სტრიქონის სახით,
 *          პოულობს შესაბამის `_create` ფუნქციას და იძახებს მას.
 *
 * @param[in] module_type შესაქმნელი მოდულის ტიპის სტრიქონი (მაგ., "relay_module").
 * @param[in] config მაჩვენებელი cJSON ობიექტზე, რომელიც შეიცავს მოდულის კონფიგურაციას.
 *
 * @return module_t* მაჩვენებელი ახლად შექმნილ მოდულზე წარმატების შემთხვევაში.
 * @return NULL თუ მოხდა შეცდომა (მაგ., ტიპი არ მოიძებნა, მეხსიერება ვერ გამოიყო).
 */
module_t *synapse_module_factory_create(const char *module_type, const cJSON *config)
{
    if (!module_type) {
        // ⭐️ შესწორებულია: მოშორებულია ზედმეტი არგუმენტი
        ESP_LOGE(TAG, "Cannot create module: %s", "module_type is NULL");
        return NULL;
    }

    // config შეიძლება იყოს NULL ზოგიერთი მოდულისთვის, ამიტომ მას არ ვამოწმებთ აქ.
    // თავად create_fn ფუნქციაა პასუხისმგებელი მის ვალიდაციაზე.

    // 1. მოვძებნოთ შესაბამისი create ფუნქცია ავტომატურად გენერირებულ რუქაში.
    module_create_fn_t create_fn = synapse_module_factory_get(module_type);

    if (!create_fn) {
        ESP_LOGW(TAG, "No factory function found for module type: '%s'", module_type);
        return NULL;
    }

    ESP_LOGD(TAG, "Found factory for '%s'. Creating module instance...", module_type);

    // 2. გამოვიძახოთ ნაპოვნი create ფუნქცია და გადავცეთ კონფიგურაცია.
    module_t *new_module = create_fn(config);

    if (!new_module) {
        ESP_LOGE(TAG, "Factory function for '%s' failed to create module instance.", module_type);
    }

    ESP_LOGI(TAG, "Module instance for '%s' created successfully at %p.", module_type, new_module);

    return new_module;
}