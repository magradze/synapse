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

#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H

#include "esp_err.h"

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
  esp_err_t fmw_service_locator_init(void);

  /**
   * @brief ახალი სერვისის რეგისტრაცია უნიკალური სახელით და ტიპით
   *
   * @param[in] service_name სერვისის უნიკალური სახელი (მაგ: მოდულის instance_name)
   * @param[in] service_type სერვისის ტიპი (მაგ: "display_api", "sensor_api", "actuator_api")
   * @param[in] service_handle მაჩვენებელი სერვისის API სტრუქტურაზე
   *
   * @return esp_err_t
   * @retval ESP_OK სერვისი წარმატებით დარეგისტრირდა
   * @retval ESP_ERR_INVALID_ARG თუ service_name, service_type ან service_handle არის NULL
   * @retval ESP_ERR_NO_MEM თუ მეხსიერება ვერ გამოიყო ახალი სერვისისთვის
   * @retval ESP_ERR_INVALID_STATE თუ ამ სახელით სერვისი უკვე დარეგისტრირებულია
   */
  esp_err_t fmw_service_register(const char *service_name,
                                 const char *service_type,
                                 service_handle_t service_handle);

  /**
   * @brief აბრუნებს რეგისტრირებულ სერვისს სახელით
   *
   * @param[in] service_name მოსაძებნი სერვისის სახელი
   *
   * @return service_handle_t
   * @retval სერვისის API სტრუქტურაზე მაჩვენებელი თუ სერვისი მოიძებნა
   * @retval NULL თუ ასეთი სახელით სერვისი არ არის რეგისტრირებული
   */
  service_handle_t fmw_service_get(const char *service_name);

  /**
   * @brief აბრუნებს სერვისის ტიპს სახელით
   *
   * @param[in] service_name მოსაძებნი სერვისის სახელი
   *
   * @return const char*
   * @retval სერვისის ტიპის სტრინგი თუ სერვისი მოიძებნა
   * @retval NULL თუ ასეთი სახელით სერვისი არ არის რეგისტრირებული
   */
  const char *fmw_service_get_type(const char *service_name);

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
  service_handle_t fmw_service_lookup_by_type(const char *service_type);

#ifdef __cplusplus
}
#endif

#endif /* SERVICE_LOCATOR_H */