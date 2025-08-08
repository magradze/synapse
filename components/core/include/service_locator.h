/**
 * @file service_locator.h
 * @brief Framework-ის სერვის ლოკატორის საჯარო API
 *
 * ეს კომპონენტი უზრუნველყოფს სერვისების რეგისტრაციისა და მოძიების ცენტრალიზებულ მექანიზმს.
 * ის საშუალებას აძლევს მოდულებს, მიიღონ სხვა მოდულების API-ები პირდაპირი #include დამოკიდებულების გარეშე.
 * ეს პატერნი ხელს უწყობს კომპონენტების იზოლაციას და სისტემის მოქნილობას.
 *
 * @author Giorgi Magradze
 * @date 2025-06-24
 * @version 1.0
 */

#ifndef SYNAPSE_SERVICE_LOCATOR_H
#define SYNAPSE_SERVICE_LOCATOR_H

#include "esp_err.h"
#include "service_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @typedef service_handle_t
   * @brief გენერიკული ტიპი, რომელიც წარმოადგენს რეგისტრირებული სერვისის ჰენდლს
   *
   * ფაქტობრივად, ეს არის მაჩვენებელი სერვისის API სტრუქტურაზე.
   */
  typedef void *service_handle_t;

  /**
   * @brief სერვის ლოკატორის ინიციალიზაცია
   *
   * უნდა გამოიძახოთ ერთხელ სისტემის ინიციალიზაციისას, სანამ რომელიმე მოდული სცდის სერვისის რეგისტრაციას ან მოძიებას.
   *
   * @return esp_err_t ESP_OK წარმატების შემთხვევაში, ან შეცდომის კოდი
   * @retval ESP_OK სერვის ლოკატორი წარმატებით ინიციალიზდა
   * @retval ESP_ERR_NO_MEM მეხსიერების გამოყოფა ვერ მოხერხდა
   * @retval ESP_ERR_INVALID_STATE სერვის ლოკატორი უკვე ინიციალიზებულია
   */
  esp_err_t synapse_service_locator_init(void);

  /**
   * @brief ახალი სერვისის რეგისტრაცია უნიკალური სახელით და ტიპით.
   *
   * @details ეს ფუნქცია ამატებს სერვისს ცენტრალურ რეესტრში. რეგისტრაცია არ
   *          განხორციელდება, თუ ამ სახელით სერვისი უკვე არსებობს. ოპერაცია
   *          ნაკად-უსაფრთხოა (thread-safe).
   *
   * @param[in] service_name სერვისის უნიკალური სახელი (მაგ: მოდულის `instance_name`).
   * @param[in] service_type სერვისის ტიპი `synapse_service_type_t` enum-იდან (მაგ: `SERVICE_TYPE_DISPLAY_API`).
   * @param[in] service_handle მაჩვენებელი სერვისის API სტრუქტურაზე.
   *
   * @return esp_err_t ოპერაციის წარმატების კოდი.
   * @retval ESP_OK სერვისი წარმატებით დარეგისტრირდა.
   * @retval ESP_ERR_INVALID_ARG თუ `service_name`, `service_handle` არის NULL, ან `service_type` არავალიდურია.
   * @retval ESP_ERR_NO_MEM თუ მეხსიერება ვერ გამოიყო ახალი სერვისისთვის.
   * @retval ESP_ERR_INVALID_STATE თუ ამ სახელით სერვისი უკვე დარეგისტრირებულია.
   * @retval ESP_ERR_TIMEOUT თუ mutex-ის დაკავება ვერ მოხერხდა.
   */
  esp_err_t synapse_service_register(const char *service_name, synapse_service_type_t service_type, service_handle_t service_handle);

  /**
   * @brief სერვისის რეგისტრაციის გაუქმება მისი სახელით.
   *
   * ეს ფუნქცია შლის მითითებული სახელის მქონე სერვისს სერვის ლოკატორიდან,
   * რის შემდეგაც ის აღარ იქნება ხელმისაწვდომი მოძიებისთვის ან გამოყენებისთვის.
   * თუ სერვისი ვერ მოიძებნა, დაბრუნდება შესაბამისი შეცდომის კოდი.
   *
   * @param[in] service_name სერვისის სახელი, რომლის რეგისტრაციაც უნდა გაუქმდეს. უნდა იყოს null-ით დასრულებული სტრიქონი.
   *
   * @return
   *      - ESP_OK: სერვისი წარმატებით გაუქმდა.
   *      - ESP_ERR_NOT_FOUND: მითითებული სახელის მქონე სერვისი ვერ მოიძებნა.
   *      - ESP_ERR_INVALID_ARG: service_name არგუმენტი არის NULL ან არასწორია.
   *      - სხვა esp_err_t კოდები საჭიროებისამებრ.
   */
  esp_err_t synapse_service_unregister(const char *service_name);

  /**
   * @brief აბრუნებს რეგისტრირებულ სერვისს სახელით
   *
   * @param[in] service_name მოსაძებნი სერვისის სახელი
   *
   * @return service_handle_t
   * @retval სერვისის API სტრუქტურაზე მაჩვენებელი თუ სერვისი მოიძებნა
   * @retval NULL თუ ასეთი სახელით სერვისი არ არის რეგისტრირებული
   */
  service_handle_t
  synapse_service_get(const char *service_name);

  /**
   * @brief აბრუნებს სერვისის ტიპს `enum` სახით.
   *
   * @details ეძებს სერვისს მისი სახელით და აბრუნებს მის `synapse_service_type_t`
   *          ტიპს. ეს საშუალებას გაძლევთ, პროგრამულად შეამოწმოთ, თუ რა ტიპის
   *          სერვისთან გაქვთ საქმე.
   *
   * @param[in] service_name მოსაძებნი სერვისის სახელი.
   * @param[out] out_service_type მაჩვენებელი, სადაც ჩაიწერება ნაპოვნი სერვისის ტიპი.
   *
   * @return esp_err_t ოპერაციის წარმატების კოდი.
   * @retval ESP_OK თუ სერვისი მოიძებნა და ტიპი წარმატებით იქნა მიღებული.
   * @retval ESP_ERR_INVALID_ARG თუ `service_name` ან `out_service_type` არის NULL.
   * @retval ESP_ERR_NOT_FOUND თუ მითითებული სახელით სერვისი ვერ მოიძებნა.
   * @retval ESP_ERR_TIMEOUT თუ mutex-ის დაკავება ვერ მოხერხდა.
   */
  esp_err_t synapse_service_get_type(const char *service_name, synapse_service_type_t *out_service_type);

  /**
   * @brief აბრუნებს მითითებული ტიპის პირველ რეგისტრირებულ სერვისს
   *
   * ეს სასარგებლოა, როცა მოდულს სჭირდება კონკრეტული ტიპის სერვისი, მაგრამ არ აინტერესებს კონკრეტული პროვაიდერი.
   *
   * @param[in] service_type მოსაძებნი სერვისის ტიპი (მაგ: "display_api")
   *
   * @return service_handle_t
   * @retval სერვისის API სტრუქტურაზე მაჩვენებელი თუ შესაბამისი ტიპის სერვისი მოიძებნა
   * @retval NULL თუ ასეთი ტიპის სერვისი არ არის რეგისტრირებული
   */
  service_handle_t synapse_service_lookup_by_type(synapse_service_type_t service_type);

#ifdef __cplusplus
}
#endif

#endif /* SYNAPSE_SERVICE_LOCATOR_H */