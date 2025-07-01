/**
 * @file health_monitor.h
 * @brief System Health Monitor Module API
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-07-01
 * @details Defines the public service API for the Health Monitor module,
 *          allowing other modules to query system health status and register
 *          custom health checks.
 */

#ifndef HEALTH_MONITOR_H
#define HEALTH_MONITOR_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief ქმნის ახალ health_monitor მოდულის ინსტანციას.
     *
     * @param[in] config მოდულის კონფიგურაცია (JSON ობიექტი) `system_config.json`-დან.
     * @return მაჩვენებელი შექმნილ მოდულზე ან NULL შეცდომისას.
     */
    module_t *health_monitor_create(const cJSON *config);

#ifdef __cplusplus
}
#endif

#endif // HEALTH_MONITOR_H