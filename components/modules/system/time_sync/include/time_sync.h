/**
 * @file time_sync.h
 * @brief Synchronizes system time using NTP/SNTP.
 * @author Giorgi Magradze
 */
#ifndef TIME_SYNC_H
#define TIME_SYNC_H

#include "base_module.h"
#include "cJSON.h"

/**
 * @brief ქმნის ახალ time_sync მოდულის ინსტანციას.
 */
module_t *time_sync_create(const cJSON *config);

#endif // TIME_SYNC_H
