/**
 * @file resource_manager.h
 * @brief რესურსების (GPIO, I2C, SPI და სხვა) მართვის Public API
 * @version 2.0
 * @date 2025-06-25
 * @author Giorgi Magradze
 * @details ეს კომპონენტი უზრუნველყოფს სისტემაში საერთო რესურსების უსაფრთხო და ექსკლუზიურ გამოყენებას.
 *          თითოეულ მოდულს შეუძლია "დალოქოს" რესურსი, რათა თავიდან აიცილოს კონფლიქტი სხვა მოდულებთან. რესურსის გათავისუფლება
 *          აუცილებელია მოდულის გაჩერების ან გამორთვისას. ეს ხელს უწყობს სისტემის სტაბილურობას და მოდულებს შორის იზოლაციას.
 *          განახლებული ვერსია იყენებს გენერიკულ მიდგომას, რათა მართოს სხვადასხვა ტიპის რესურსი.
 */
#ifndef FMW_RESOURCE_MANAGER_H
#define FMW_RESOURCE_MANAGER_H

#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @enum fmw_resource_type_t
 * @brief სისტემაში სამართავი რესურსების ტიპების ჩამონათვალი.
 */
typedef enum
{
  FMW_RESOURCE_TYPE_GPIO,        /**< @brief GPIO პინი. `resource_id` არის `gpio_num_t`. */
  FMW_RESOURCE_TYPE_I2C_PORT,    /**< @brief I2C პორტი. `resource_id` არის `i2c_port_t`. */
  FMW_RESOURCE_TYPE_SPI_HOST,    /**< @brief SPI ჰოსტი. `resource_id` არის `spi_host_device_t`. */
  FMW_RESOURCE_TYPE_ADC_CHANNEL, /**< @brief ADC არხი. `resource_id` არის `adc_channel_t`. */
  FMW_RESOURCE_TYPE_TIMER_GROUP, /**< @brief ტაიმერების ჯგუფი. `resource_id` არის `timer_group_t`. */
  FMW_RESOURCE_TYPE_RMT_CHANNEL, /**< @brief RMT არხი. `resource_id` არის `rmt_channel_t`. */
  FMW_RESOURCE_TYPE_UART_PORT,   /**< @brief UART პორტი. `resource_id` არის `uart_port_t`. */
  FMW_RESOURCE_TYPE_MAX,         /**< @brief ჩამონათვალის დასასრული, გამოიყენება შიდა ვალიდაციისთვის. */
} fmw_resource_type_t;

/**
 * @brief რესურს მენეჯერის ინიციალიზაცია.
 *
 * ასუფთავებს ყველა დაკავებული რესურსის სიას. უნდა გამოიძახოს System Manager-მა ერთხელ სისტემის გაშვებისას.
 *
 * @return esp_err_t
 * @retval ESP_OK თუ რესურს მენეჯერი წარმატებით ინიციალიზდა.
 * @retval ESP_ERR_NO_MEM თუ მეხსიერების გამოყოფა ვერ მოხერხდა.
 */
esp_err_t fmw_resource_manager_init(void);

/**
 * @brief რესურსის დაკავება (დალოქვა) კონკრეტული მოდულისთვის.
 *
 * სანამ მოდული გამოიყენებს რესურსს, მან უნდა "დალოქოს" იგი, რათა სხვა მოდულმა ვერ გამოიყენოს.
 *
 * @param[in] type რესურსის ტიპი `fmw_resource_type_t`-დან.
 * @param[in] resource_id რესურსის უნიკალური იდენტიფიკატორი (მაგ: პინის ნომერი, პორტის ნომერი).
 * @param[in] owner მოდულის სახელი, რომელიც იკავებს რესურსს (სასარგებლოა დიაგნოსტიკისთვის).
 *
 * @return esp_err_t
 * @retval ESP_OK თუ რესურსი წარმატებით დაილოქა.
 * @retval ESP_ERR_INVALID_ARG თუ `type`, `resource_id` ან `owner` არავალიდურია.
 * @retval ESP_ERR_INVALID_STATE თუ რესურსი უკვე დაკავებულია სხვა მოდულის მიერ.
 * @retval ESP_ERR_NO_MEM თუ მეხსიერების გამოყოფა ვერ მოხერხდა.
 */
esp_err_t fmw_resource_lock(fmw_resource_type_t type, uint8_t resource_id, const char *owner);

/**
 * @brief დაკავებული რესურსის გათავისუფლება.
 *
 * როდესაც მოდული ასრულებს მუშაობას რესურსზე (მაგ: `deinit` ფუნქციაში), მან უნდა გაათავისუფლოს იგი.
 *
 * @param[in] type რესურსის ტიპი `fmw_resource_type_t`-დან.
 * @param[in] resource_id გასათავისუფლებელი რესურსის იდენტიფიკატორი.
 * @param[in] owner მოდულის სახელი, რომელიც ათავისუფლებს რესურსს.
 *
 * @return esp_err_t
 * @retval ESP_OK თუ რესურსი წარმატებით გათავისუფლდა.
 * @retval ESP_ERR_INVALID_ARG თუ `type` ან `resource_id` არავალიდურია.
 * @retval ESP_ERR_NOT_FOUND თუ მითითებული რესურსი დაკავებული არ იყო.
 * @retval ESP_ERR_INVALID_STATE თუ გათავისუფლება სცადა არა მფლობელმა მოდულმა.
 */
esp_err_t fmw_resource_release(fmw_resource_type_t type, uint8_t resource_id, const char *owner);

/**
 * @brief ამოწმებს, დაკავებულია თუ არა კონკრეტული რესურსი.
 *
 * @param[in] type რესურსის ტიპი `fmw_resource_type_t`-დან.
 * @param[in] resource_id შესამოწმებელი რესურსის იდენტიფიკატორი.
 *
 * @return true თუ რესურსი დაკავებულია.
 * @return false თუ რესურსი თავისუფალია.
 */
bool fmw_resource_is_locked(fmw_resource_type_t type, uint8_t resource_id);

/**
 * @brief აბრუნებს რესურსის მფლობელის სახელს.
 *
 * სასარგებლოა დიაგნოსტიკისთვის, რათა გაირკვეს, რომელი მოდული იყენებს კონკრეტულ რესურსს.
 *
 * @param[in] type რესურსის ტიპი `fmw_resource_type_t`-დან.
 * @param[in] resource_id შესამოწმებელი რესურსის იდენტიფიკატორი.
 * @param[out] out_owner მაჩვენებელი სტრიქონზე, რომელიც მიუთითებს რესურსის მფლობელის სახელზე, თუ დაკავებულია.
 *
 * @return esp_err_t
 * @retval ESP_OK თუ რესურსი დაკავებულია და მფლობელის სახელი დაბრუნდა.
 * @retval ESP_ERR_NOT_FOUND თუ რესურსი თავისუფალია ან ვერ მოიძებნა.
 * @retval ESP_ERR_INVALID_ARG თუ არგუმენტები არავალიდურია.
 */
esp_err_t fmw_resource_get_owner(fmw_resource_type_t type, uint8_t resource_id, const char **out_owner);

/**
 * @brief ათავისუფლებს რესურს მენეჯერს და ასრულებს საჭირო დასუფთავების ოპერაციებს.
 *
 * ეს ფუნქცია გამოიყენება რესურს მენეჯერის დეინიციალიზაციისთვის. მისი გამოძახების შემდეგ,
 * ყველა რესურსი, რომელიც მენეჯერმა გამოიყენა ან გამოყო, უნდა გათავისუფლდეს.
 * ფუნქციის გამოძახება აუცილებელია სისტემის გამორთვამდე ან რესურს მენეჯერის ხელახლა ინიციალიზაციამდე,
 * რათა თავიდან ავიცილოთ მეხსიერების გაჟონვა ან სხვა რესურსებთან დაკავშირებული პრობლემები.
 *
 * @note ფუნქცია არ იღებს პარამეტრებს და არ აბრუნებს მნიშვნელობას.
 */
void fmw_resource_manager_deinit(void);

#endif /* FMW_RESOURCE_MANAGER_H */