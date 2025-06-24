/**
 * @file config_manager.h
 * @brief კონფიგურაციის მენეჯერის API
 * @version 1.0
 * @date 2025-06-24
 * @author Giorgi Magradze
 * @details ეს კომპონენტი უზრუნველყოფს Non-Volatile Storage (NVS) მეხსიერებასთან
 *          მუშაობის აბსტრაქციას. ის საშუალებას იძლევა, მარტივად შეინახოს და
 *          წაიკითხოს სხვადასხვა ტიპის პარამეტრები (string, i32, i8).
 *          ასევე, უზრუნველყოფს საწყისი კონფიგურაციის ჩატვირთვას პირველი გაშვებისას.
 */

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "esp_err.h"
#include <stddef.h> // size_t-სთვის
#include <stdint.h> // uint8_t, int32_t და ა.შ.

/**
 * @brief ახდენს კონფიგურაციის მენეჯერის და NVS-ის ინიციალიზაციას.
 *
 * @details ეს ფუნქცია უნდა გამოიძახოს `system_manager`-მა სისტემის ინიციალიზაციის
 *          ადრეულ ეტაპზე, რათა მომზადდეს NVS მეხსიერება სხვა კომპონენტებისთვის.
 *          თუ NVS-ის ინიციალიზაცია ვერ ხერხდება კრიტიკული შეცდომის (მაგ. NVS დანაყოფი არ არსებობს) გამო,
 *          ფუნქცია აბრუნებს შეცდომის კოდს.
 *
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ ინიციალიზაცია წარმატებით დასრულდა.
 * @retval ESP_FAIL თუ ინიციალიზაცია ვერ მოხერხდა.
 */
esp_err_t fmw_config_manager_init(void);

/**
 * @brief ამოწმებს, საჭიროა თუ არა საწყისი კონფიგურაციის ჩატვირთვა და ტვირთავს მას.
 *
 * @details ეს ფუნქცია უნდა გამოიძახოს `Module Registry`-მ ინიციალიზაციისას.
 *          ის ამოწმებს `sys.init_done` NVS ფლაგს. თუ ფლაგირების მარკერი არ არსებობს,
 *          ეს ნიშნავს, რომ მოწყობილობა პირველად ეშვება (ან NVS გასუფთავდა).
 *          ამ შემთხვევაში, ფუნქცია `system_config.json` ფაილიდან კითხულობს
 *          პარამეტრებს და წერს მათ NVS-ში, რის შემდეგაც აყენებს ფლაგს,
 *          რათა ეს პროცესი ხელმეორედ აღარ შესრულდეს.
 */
void fmw_config_load_defaults_if_needed(void);

/**
 * @brief ინახავს სტრიქონის ტიპის პარამეტრს NVS მეხსიერებაში.
 *
 * @param[in] key პარამეტრის უნიკალური გასაღები (null-terminated string).
 * @param[in] value შესანახი სტრიქონი.
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ შენახვა წარმატებით დასრულდა.
 * @retval ESP_ERR_NVS_INVALID_NAME თუ გასაღები არავალიდურია.
 * @retval ESP_ERR_NVS_INVALID_HANDLE თუ NVS handle არასწორია.
 * @retval ESP_ERR_NVS_READ_ONLY თუ NVS დანაყოფი მხოლოდ წაკითხვადია.
 * @retval ESP_FAIL სხვა შეცდომის შემთხვევაში.
 */
esp_err_t fmw_config_set_string(const char *key, const char *value);

/**
 * @brief კითხულობს სტრიქონის ტიპის პარამეტრს NVS მეხსიერებიდან.
 *
 * @param[in] key პარამეტრის გასაღები.
 * @param[out] buffer ბუფერი, სადაც ჩაიწერება წაკითხული მნიშვნელობა.
 * @param[in] buffer_size ბუფერის ზომა ბაიტებში.
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ წაკითხვა წარმატებით დასრულდა.
 * @retval ESP_ERR_NVS_NOT_FOUND თუ მითითებული გასაღები არ არსებობს.
 * @retval ESP_ERR_NVS_INVALID_HANDLE თუ NVS handle არასწორია.
 * @retval ESP_ERR_NVS_INVALID_NAME თუ გასაღები არავალიდურია.
 * @retval ESP_ERR_NVS_INVALID_LENGTH თუ ბუფერის ზომა არასაკმარისია.
 */
esp_err_t fmw_config_get_string(const char *key, char *buffer, size_t buffer_size);

/**
 * @brief ინახავს 32-ბიტიან მთელ რიცხვს (int32_t) NVS მეხსიერებაში.
 *
 * @param[in] key პარამეტრის უნიკალური გასაღები.
 * @param[in] value შესანახი რიცხვითი მნიშვნელობა.
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ შენახვა წარმატებით დასრულდა.
 */
esp_err_t fmw_config_set_i32(const char *key, int32_t value);

/**
 * @brief კითხულობს 32-ბიტიან მთელ რიცხვს (int32_t) NVS მეხსიერებიდან.
 *
 * @param[in] key პარამეტრის გასაღები.
 * @param[out] out_value მაჩვენებელი, სადაც ჩაიწერება წაკითხული მნიშვნელობა.
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ წაკითხვა წარმატებით დასრულდა.
 * @retval ESP_ERR_NVS_NOT_FOUND თუ მითითებული გასაღები არ არსებობს.
 */
esp_err_t fmw_config_get_i32(const char *key, int32_t *out_value);

/**
 * @brief ინახავს 8-ბიტიან მთელ რიცხვს (int8_t) NVS მეხსიერებაში.
 *
 * @param[in] key პარამეტრის უნიკალური გასაღები.
 * @param[in] value შესანახი რიცხვითი მნიშვნელობა.
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ შენახვა წარმატებით დასრულდა.
 */
esp_err_t fmw_config_set_i8(const char *key, int8_t value);

/**
 * @brief კითხულობს 8-ბიტიან მთელ რიცხვს (int8_t) NVS მეხსიერებიდან.
 *
 * @param[in] key პარამეტრის გასაღები.
 * @param[out] out_value მაჩვენებელი, სადაც ჩაიწერება წაკითხული მნიშვნელობა.
 * @return esp_err_t ოპერაციის წარმატების კოდი.
 * @retval ESP_OK თუ წაკითხვა წარმატებით დასრულდა.
 * @retval ESP_ERR_NVS_NOT_FOUND თუ მითითებული გასაღები არ არსებობს.
 */
esp_err_t fmw_config_get_i8(const char *key, int8_t *out_value);

#endif // CONFIG_MANAGER_H