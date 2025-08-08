/**
 * @file logging.h
 * @brief Synapse ESP Framework-ის ლოგირების დამხმარე ინტერფეისი.
 * @author Giorgi Magradze
 * @version 2.0
 * @date 2025-06-26
 *
 * @details
 * ეს ფაილი განსაზღვრავს ლოგირების დამხმარე მაკროს, რომელიც უზრუნველყოფს
 * ერთიან სტილს კომპონენტის ლოგირების ტეგის (TAG) განსაზღვრისთვის.
 * ფრეიმვორქი იყენებს სტანდარტულ `esp_log.h` ბიბლიოთეკას ლოგირებისთვის.
 *
 * @note ყველა .c ფაილმა, რომელსაც სჭირდება ლოგირება, უნდა გამოიყენოს
 *       `DEFINE_COMPONENT_TAG` მაკრო საკუთარი, უნიკალური ტეგის შესაქმნელად.
 */

#ifndef SYNAPSE_LOGGING_H
#define SYNAPSE_LOGGING_H

#include "esp_log.h"

/**
 * @brief განსაზღვრავს ლოკალურ, სტატიკურ TAG ცვლადს ლოგირებისთვის.
 *
 * @details ეს მაკრო ქმნის `static const char* TAG` ცვლადს, რომელიც გამოიყენება
 *          `ESP_LOGx` მაკროებში. მისი გამოყენება უზრუნველყოფს, რომ თითოეული
 *          კომპონენტიდან წამოსული ლოგი ადვილად იდენტიფიცირებადი იყოს.
 *
 *          გამოყენების მაგალითი .c ფაილში:
 *          @code
 *          #include "logging.h"
 *          DEFINE_COMPONENT_TAG("MY_MODULE");
 *
 *          void my_function() {
 *              ESP_LOGI(TAG, "Hello from my module!");
 *          }
 *          @endcode
 *
 * @param component_name კომპონენტის/მოდულის სახელი (სტრიქონი), რომელიც გამოჩნდება ლოგებში.
 */
#define DEFINE_COMPONENT_TAG(component_name) \
  static const char *TAG = component_name

#endif // SYNAPSE_LOGGING_H