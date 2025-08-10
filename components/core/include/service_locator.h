/**
 * @file service_locator.h
 * @brief Framework-ის სერვის ლოკატორის საჯარო API
 *
 * @author Giorgi Magradze
 * @date 2025-09-12
 * @version 2.0
 */

#ifndef SYNAPSE_SERVICE_LOCATOR_H
#define SYNAPSE_SERVICE_LOCATOR_H

#include "service_types.h"
#include "service_status.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef void *service_handle_t;

  esp_err_t synapse_service_locator_init(void);

  /**
   * @brief ახალი სერვისის რეგისტრაცია.
   * @deprecated This function is deprecated and will be removed in future versions.
   *             Use synapse_service_register_with_status() instead. It registers
   *             the service with a default status of REGISTERED.
   */
  esp_err_t synapse_service_register(const char *service_name, synapse_service_type_t service_type, service_handle_t service_handle) __attribute__((deprecated("Use synapse_service_register_with_status() instead")));

  /**
   * @brief Registers a new service with a specific initial status. (NEW)
   *
   * @param[in] service_name The unique name of the service.
   * @param[in] service_type The type of the service from the synapse_service_type_t enum.
   * @param[in] service_handle A pointer to the service's API structure.
   * @param[in] initial_status The initial status of the service (typically SERVICE_STATUS_REGISTERED).
   *
   * @return esp_err_t
   * @retval ESP_OK If the service was registered successfully.
   * @retval ESP_ERR_INVALID_ARG If any argument is invalid.
   * @retval ESP_ERR_INVALID_STATE If a service with this name already exists.
   * @retval ESP_ERR_NO_MEM If memory allocation failed.
   */
  esp_err_t synapse_service_register_with_status(const char *service_name, synapse_service_type_t service_type, service_handle_t service_handle, service_status_t initial_status);

  esp_err_t synapse_service_unregister(const char *service_name);

  /**
   * @brief Sets the operational status of a registered service. (NEW)
   * @details This function is thread-safe. When the status changes, it will
   *          post a SYNAPSE_EVENT_SERVICE_STATUS_CHANGED event to the Event Bus.
   *
   * @param[in] service_name The name of the service to update.
   * @param[in] new_status The new status to set.
   * @return esp_err_t
   * @retval ESP_OK on success.
   * @retval ESP_ERR_NOT_FOUND if the service does not exist.
   */
  esp_err_t synapse_service_set_status(const char *service_name, service_status_t new_status);

  /**
   * @brief Gets the current operational status of a registered service. (NEW)
   * @details This function is thread-safe.
   *
   * @param[in] service_name The name of the service to query.
   * @param[out] out_status A pointer where the current status will be stored.
   * @return esp_err_t
   * @retval ESP_OK on success.
   * @retval ESP_ERR_NOT_FOUND if the service does not exist.
   */
  esp_err_t synapse_service_get_status(const char *service_name, service_status_t *out_status);

  service_handle_t synapse_service_get(const char *service_name);

  esp_err_t synapse_service_get_type(const char *service_name, synapse_service_type_t *out_service_type);

  service_handle_t synapse_service_lookup_by_type(synapse_service_type_t service_type);

#ifdef __cplusplus
}
#endif

#endif /* SYNAPSE_SERVICE_LOCATOR_H */