/**
 * @file service_status.h
 * @brief Defines the lifecycle status for services in the Service Locator.
 * @author Giorgi Magradze
 * @version 2.0.0
 * @date 2025-09-12
 *
 * @details This header provides a centralized, typed definition for all possible
 *          states a service can be in throughout its lifecycle. This is a key
 *          component of the state-aware Service Locator.
 */
#ifndef SERVICE_STATUS_H
#define SERVICE_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum service_status_t
 * @brief Enumeration of possible service lifecycle statuses.
 */
typedef enum {
    SERVICE_STATUS_UNKNOWN = 0,    /**< @brief The status is unknown or cannot be determined. */
    SERVICE_STATUS_REGISTERED,     /**< @brief The service has been created and registered, but not yet initialized. */
    SERVICE_STATUS_INITIALIZING,   /**< @brief The service is currently executing its init() and start() functions. */
    SERVICE_STATUS_ACTIVE,         /**< @brief The service is fully operational and ready for use. */
    SERVICE_STATUS_STOPPING,       /**< @brief The service is in its deinit() phase during a graceful shutdown. */
    SERVICE_STATUS_ERROR,          /**< @brief The service encountered a critical error and is not operational. */
} service_status_t;

/**
 * @brief Converts a service_status_t enum to its string representation.
 * @details Useful for logging and debugging purposes.
 * @param[in] status The status enum value.
 * @return A constant string representing the status.
 */
static inline const char* service_status_to_string(service_status_t status) {
    switch(status) {
        case SERVICE_STATUS_UNKNOWN:    return "UNKNOWN";
        case SERVICE_STATUS_REGISTERED: return "REGISTERED";
        case SERVICE_STATUS_INITIALIZING: return "INITIALIZING";
        case SERVICE_STATUS_ACTIVE:     return "ACTIVE";
        case SERVICE_STATUS_STOPPING:   return "STOPPING";
        case SERVICE_STATUS_ERROR:      return "ERROR";
        default:                        return "INVALID_STATUS";
    }
}

#ifdef __cplusplus
}
#endif

#endif // SERVICE_STATUS_H