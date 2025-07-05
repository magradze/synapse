/**
 * @file ble_prov_interface.h
 * @brief Public Service API contract for the BLE Provisioning module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-05
 */

#ifndef FMW_BLE_PROV_INTERFACE_H
#define FMW_BLE_PROV_INTERFACE_H

#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The public Service API structure for the BLE Provisioning module.
 */
typedef struct
{
    /**
     * @brief Checks if the BLE provisioning process is currently active.
     *
     * @details A module can call this function to determine if the device is
     *          currently in a state where it is discoverable and waiting for
     *          a client to connect for provisioning.
     *
     * @return
     *      - true: Provisioning is active.
     *      - false: Provisioning is not active.
     */
    bool (*is_provisioning_active)(void);

    /**
     * @brief Manually starts the provisioning process.
     *
     * @details This can be used to re-trigger provisioning, for example,
     *          from a CLI command or a physical button press.
     *
     * @return
     *      - ESP_OK: If the provisioning process was started successfully.
     *      - ESP_ERR_INVALID_STATE: If provisioning is already active.
     */
    esp_err_t (*start_provisioning)(void);

} ble_prov_api_t;


#ifdef __cplusplus
}
#endif

#endif // FMW_BLE_PROV_INTERFACE_H