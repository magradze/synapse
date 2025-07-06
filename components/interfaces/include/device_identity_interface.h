/**
 * @file device_identity_interface.h
 * @brief Public Service API contract for the Device Identity Service.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-09
 */

#ifndef FMW_DEVICE_IDENTITY_INTERFACE_H
#define FMW_DEVICE_IDENTITY_INTERFACE_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The public Service API structure for the Device Identity Service.
 */
typedef struct
{
    /**
     * @brief Retrieves the unique device identifier string.
     * @details The returned string is guaranteed to be persistent for the
     *          lifetime of the device's operation. The caller should NOT
     *          free the returned pointer. The ID is typically derived from
     *          the device's MAC address.
     * @return A const char* pointer to the null-terminated device ID string.
     *         Returns a default "unknown_id" string if the service is not ready.
     */
    const char* (*get_device_id)(void);

    /**
     * @brief Retrieves the current firmware version string.
     * @details The version is typically defined at compile time. The caller
     *          should NOT free the returned pointer.
     * @return A const char* pointer to the null-terminated firmware version string.
     */
    const char* (*get_firmware_version)(void);

} device_identity_api_t;


#ifdef __cplusplus
}
#endif

#endif // FMW_DEVICE_IDENTITY_INTERFACE_H