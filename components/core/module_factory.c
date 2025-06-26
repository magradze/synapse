/**
 * @file module_factory.c
 * @brief მოდულების შემქმნელი (Factory) კომპონენტის იმპლემენტაცია.
 * @version 1.0
 * @date 2025-06-26
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს Module Factory-ის Public API-ს იმპლემენტაციას.
 *          იგი იყენებს ავტომატურად გენერირებულ `fmw_module_factory_get` ფუნქციას,
 *          რათა დააკავშიროს მოდულის ტიპის სტრიქონი მის შესაბამის შემქმნელ ფუნქციასთან.
 */
#include "module_factory.h"
#include "logging.h"
#include <stddef.h> // NULL-ისთვის

// ეს არის ავტომატურად გენერირებული ჰედერი, რომელიც შეიცავს fmw_module_factory_get-ის დეკლარაციას.
// ის რეალურად იქმნება build/esp-idf/core/generated_module_factory.h-ში.
// Temporary workaround: forward declare the function to avoid header dependency issues
module_create_fn_t fmw_module_factory_get(const char *module_type);
// #include "generated_module_factory.h" 

DEFINE_COMPONENT_TAG("MODULE_FACTORY");

// --- Public API იმპლემენტაცია ---

module_t* fmw_module_factory_create(const char *module_type, const cJSON *config) {
    if (!module_type) {
        ESP_LOGE(TAG, "Cannot create module: module_type is NULL.");
        return NULL;
    }

    // 1. მოვძებნოთ შესაბამისი create ფუნქცია ავტომატურად გენერირებულ რუქაში.
    module_create_fn_t create_fn = fmw_module_factory_get(module_type);
    
    if (!create_fn) {
        ESP_LOGW(TAG, "No factory function found for module type: '%s'", module_type);
        return NULL;
    }
    
    ESP_LOGI(TAG, "Found factory for '%s'. Creating module instance...", module_type);

    // 2. გამოვიძახოთ ნაპოვნი create ფუნქცია და გადავცეთ კონფიგურაცია.
    module_t *new_module = create_fn(config);

    if (!new_module) {
        ESP_LOGE(TAG, "Factory function for '%s' failed to create module instance.", module_type);
    } else {
        ESP_LOGI(TAG, "Module instance for '%s' created successfully at %p.", module_type, new_module);
    }
    
    return new_module;
}