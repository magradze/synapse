/**
 * @file event_payloads.c
 * @brief ივენთების მონაცემთა სტრუქტურების (payloads) დამხმარე ფუნქციების იმპლემენტაცია.
 * @version 1.0
 * @date 2025-06-26
 * @author Giorgi Magradze
 * @details ეს ფაილი შეიცავს მეხსიერების გასუფთავების იმ ფუნქციებს,
 *          რომლებსაც სჭირდებათ სპეციალური ლოგიკა და არ კმაყოფილდებიან
 *          ზოგადი `fmw_payload_common_free` ფუნქციით.
 */

#include "event_payloads.h" // ამ ფაილის შესაბამისი ჰედერი
#include <stdlib.h>         // free() ფუნქციისთვის

/**
 * @brief ათავისუფლებს `fmw_telemetry_payload_t` სტრუქტურის მიერ დაკავებულ მეხსიერებას.
 *
 * @details ეს ფუნქცია გადაეცემა `fmw_event_data_wrap`-ს ტელემეტრიული ივენთების
 *          შექმნისას, რათა Event Bus-მა შეძლოს მეხსიერების უსაფრთხოდ გათავისუფლება.
 *          ფუნქცია ჯერ ათავისუფლებს შიდა, დინამიურად გამოყოფილ `json_data` სტრიქონს,
 *          შემდეგ კი თავად კონტეინერ სტრუქტურას.
 *
 * @param payload void მაჩვენებელი გასათავისუფლებელ `fmw_telemetry_payload_t` ობიექტზე.
 *                NULL მნიშვნელობა უსაფრთხოდ იგნორირებულია.
 */
void fmw_telemetry_payload_free(void *payload)
{
    if (!payload)
    {
        return;
    }

    fmw_telemetry_payload_t *telemetry_payload = (fmw_telemetry_payload_t *)payload;

    // ჯერ ვათავისუფლებთ შიდა, დინამიურ ველს
    if (telemetry_payload->json_data)
    {
        free(telemetry_payload->json_data);
    }

    // შემდეგ ვათავისუფლებთ თავად კონტეინერ სტრუქტურას
    free(telemetry_payload);
}