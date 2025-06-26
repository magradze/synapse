/**
 * @file generated_module_factory.h.in
 * @brief ავტომატურად გენერირებული მოდულების factory-ის ჰედერი.
 *
 * ეს ფაილი გენერირდება CMake-ის მიერ და განსაზღვრავს მოდულების factory map-ს,
 * რომელიც გამოიყენება მოდულების დინამიური რეგისტრაციისა და შექმნისთვის.
 * არ შეცვალოთ ხელით — ცვლილებები დაიკარგება შემდეგი აგებისას.
 */
#ifndef GENERATED_MODULE_FACTORY_H
#define GENERATED_MODULE_FACTORY_H

#include "base_module.h"
#include "cJSON.h"

// <<<<--- ყურადღება მიაქციეთ ამ ხაზს (ეს ხაზიც ქართულად)
#ifdef CONFIG_MODULE_LOGGER_ENABLED
#include "logger_module.h"
#endif


/**
 * @brief მოდულის შემქმნელი (factory) ფუნქციის ტიპი.
 */
typedef module_t *(*module_create_fn_t)(const cJSON *config);

/**
 * @brief ასოცირებს მოდულის ტიპის სტრიქონს მის factory ფუნქციასთან.
 * @details ეს სტრუქტურა გამოიყენება ავტომატურად გენერირებულ factory map-ში.
 */
typedef struct
{
    const char *module_type_name; /**< მოდულის ტიპის სტრიქონი, მაგ: "wifi_module". */
    module_create_fn_t create_fn; /**< მოდულის factory ფუნქციის მაჩვენებელი. */
} module_factory_map_t;

/**
 * @brief აბრუნებს მითითებული მოდულის ტიპისთვის შესაბამის create ფუნქციას.
 *
 * ეს ფუნქცია გენერირდება CMake-ის მიერ და შეიცავს ყველა აღმოჩენილი მოდულის
 * ასოცირებულ map-ს. უზრუნველყოფს მოდულების factory pattern-ის ბირთვს.
 *
 * @param[in] module_type მოდულის ტიპის იდენტიფიკატორი (მაგ: "wifi_module").
 * @return მოდულის create ფუნქციის მაჩვენებელი ან NULL თუ ვერ მოიძებნა.
 */
module_create_fn_t fmw_module_factory_get(const char *module_type);

#endif // GENERATED_MODULE_FACTORY_H
