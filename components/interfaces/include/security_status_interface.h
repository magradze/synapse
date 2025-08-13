/**
 * @file security_status_interface.h
 * @brief Public Service API contract for the Security Status Reporter module.
 * @author Giorgi Magradze
 * @version 1.0.0
 * @date 2025-07-18
 * @details This header defines the abstract interface (API structure) for the
 *          Security Status Reporter service. Any module wishing to query the
 *          device's security state (e.g., Secure Boot, Flash Encryption)
 *          should include this file and use the Service Locator to get a handle
 *          to this API. This ensures decoupling from the specific implementation
 *          of the reporter module.
 */

#ifndef SYNAPSE_SECURITY_STATUS_INTERFACE_H
#define SYNAPSE_SECURITY_STATUS_INTERFACE_H

#include <stdbool.h> // For bool type

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The public Service API structure for the Security Status Reporter.
 * @details This is the "contract" that the security_status_reporter module
 *          implements and other modules (like self_test_manager) consume.
 */
typedef struct
{
    /**
     * @brief Checks if Secure Boot is enabled on the device.
     * @details This function returns a cached value that is read once at system
     *          startup. It provides a quick and efficient way to verify if the
     *          device is running in a secure boot environment.
     * @return 
     *      - `true`: If Secure Boot is enabled.
     *      - `false`: If Secure Boot is disabled or the service is not available.
     */
    bool (*is_secure_boot_enabled)(void);

    /**
     * @brief Checks if Flash Encryption is enabled on the device.
     * @details Similar to the secure boot check, this returns a cached value.
     *          It allows other modules to confirm that the device's flash
     *          memory is encrypted, protecting the firmware and data at rest.
     * @return 
     *      - `true`: If Flash Encryption is enabled.
     *      - `false`: If Flash Encryption is disabled or the service is not available.
     */
    bool (*is_flash_encryption_enabled)(void);

} security_status_api_t;


#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_SECURITY_STATUS_INTERFACE_H