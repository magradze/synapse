/**
 * @file framework_config.h
 * @brief Framework-ის გლობალური კონფიგურაციის მაკროები
 * @version 1.0
 * @date 2025-06-24
 * @author Giorgi Magradze
 * @details ეს ფაილი განსაზღვრავს ყველა ძირითად კონფიგურაციულ მაკროს, რომლებიც გამოიყენება ბირთვის, მოდულების,
 *          სერვისების, რესურსების, ტაიმაუტების, უსაფრთხოების და პერფორმანსის პარამეტრების განსასაზღვრად.
 *          ყველა მნიშვნელობა ადვილად შესაცვლელია პროექტის საჭიროებებიდან გამომდინარე და უზრუნველყოფს
 *          სისტემის მოქნილ და ოპტიმიზირებულ მუშაობას.
 */
#ifndef FRAMEWORK_CONFIG_H
#define FRAMEWORK_CONFIG_H

#include <stdint.h>

/**
 * @defgroup EventBusConfig Event Bus კონფიგურაცია
 * @{
 */
#define FRAMEWORK_MAX_SUBSCRIBERS_PER_EVENT 15 /**< მაქსიმალური გამომწერების რაოდენობა ერთ ივენთზე */
#define FRAMEWORK_EVENT_QUEUE_LENGTH 50        /**< Event Bus-ის რიგის სიგრძე */
/** @} */

/**
 * @defgroup ModuleRegistryConfig Module Registry კონფიგურაცია
 * @{
 */
#define FRAMEWORK_MAX_MODULES 20 /**< მაქსიმალური მოდულების რაოდენობა */
/** @} */

/**
 * @defgroup ServiceLocatorConfig Service Locator კონფიგურაცია
 * @{
 */
#define FRAMEWORK_MAX_SERVICES 32 /**< მაქსიმალური სერვისების რაოდენობა */
/** @} */

/**
 * @defgroup ResourceManagerConfig Resource Manager კონფიგურაცია
 * @{
 */
#define FRAMEWORK_MAX_GPIO_PINS 40 /**< მაქსიმალური GPIO პინების რაოდენობა */
#define FRAMEWORK_MAX_I2C_BUSES 2  /**< მაქსიმალური I2C ავტობუსების რაოდენობა */
#define FRAMEWORK_MAX_SPI_BUSES 2  /**< მაქსიმალური SPI ავტობუსების რაოდენობა */
/** @} */

/**
 * @defgroup TimeoutConfig Timeout კონფიგურაცია
 * @{
 */
#define FRAMEWORK_MUTEX_TIMEOUT_MS 1000      /**< Mutex-ის timeout მილიწამებში */
#define FRAMEWORK_QUEUE_TIMEOUT_MS 100       /**< Queue-ის timeout მილიწამებში */
#define FRAMEWORK_SEMAPHORE_TIMEOUT_MS 500   /**< Semaphore-ის timeout მილიწამებში */
#define FRAMEWORK_TASK_QUEUE_TIMEOUT_MS 1000 /**< Task queue-ის timeout მილიწამებში */
/** @} */

/**
 * @defgroup TaskConfig Task კონფიგურაცია
 * @{
 */
#define FRAMEWORK_EVENT_BUS_TASK_STACK_SIZE 4096 /**< Event Bus ტასკის stack ზომა */
#define FRAMEWORK_EVENT_BUS_TASK_PRIORITY 12     /**< Event Bus ტასკის პრიორიტეტი */
/** @} */

/**
 * @defgroup MemoryConfig Memory კონფიგურაცია
 * @{
 */
#define FRAMEWORK_SERVICE_NAME_MAX_LENGTH 32 /**< სერვისის სახელის მაქსიმალური სიგრძე */
#define FRAMEWORK_SERVICE_TYPE_MAX_LENGTH 32 /**< სერვისის ტიპის მაქსიმალური სიგრძე */
#define FRAMEWORK_MODULE_NAME_MAX_LENGTH 32  /**< მოდულის სახელის მაქსიმალური სიგრძე */
/** @} */

/**
 * @defgroup ModuleConfig Module კონფიგურაცია
 * @{
 */
#define FRAMEWORK_SSD1306_QUEUE_LENGTH 5      /**< SSD1306 მოდულის ბრძანებების რიგის სიგრძე */
#define FRAMEWORK_SSD1306_LINE_BUFFER_SIZE 21 /**< SSD1306 მოდულის ხაზის ბუფერის ზომა */
#define FRAMEWORK_LCD1602_QUEUE_LENGTH 10     /**< LCD1602 მოდულის ბრძანებების რიგის სიგრძე */
#define FRAMEWORK_LCD1602_LINE_BUFFER_SIZE 17 /**< LCD1602 მოდულის ხაზის ბუფერის ზომა */
/** @} */

/**
 * @defgroup RuntimeOptimizationConfig Runtime Module Optimization კონფიგურაცია
 * @{
 */
#define FRAMEWORK_RUNTIME_TASK_STACK_SIZE 2048 /**< Runtime მოდულების ოპტიმიზებული stack ზომა */
#define FRAMEWORK_RUNTIME_QUEUE_LENGTH 3       /**< Runtime მოდულების ოპტიმიზებული queue ზომა */
#define FRAMEWORK_INSTANCE_NAME_MAX_LENGTH 16  /**< Instance name-ის მაქსიმალური სიგრძე (ოპტიმიზებული) */
#define FRAMEWORK_SHARED_TASK_STACK_SIZE 3072  /**< Shared task-ის stack ზომა მრავალი მოდულისთვის */
/** @} */

/**
 * @defgroup SecurityConfig Security კონფიგურაცია
 * @{
 */
#define FRAMEWORK_MQTT_COMMAND_MAX_LENGTH 512 /**< MQTT ბრძანების მაქსიმალური სიგრძე */
#define FRAMEWORK_AUTH_TOKEN_MAX_LENGTH 64    /**< ავთენტიფიკაციის ტოკენის მაქსიმალური სიგრძე */
#define FRAMEWORK_ENABLE_MQTT_TLS 1           /**< MQTT TLS მხარდაჭერის ჩართვა (1) ან გამორთვა (0) */
/** @} */

/**
 * @defgroup PerformanceMonitoringConfig Performance Monitoring კონფიგურაცია
 * @{
 */
#define FRAMEWORK_CPU_MONITORING_ENABLED 1         /**< CPU მონიტორინგის ჩართვა */
#define FRAMEWORK_MEMORY_MONITORING_ENABLED 1      /**< მეხსიერების მონიტორინგის ჩართვა */
#define FRAMEWORK_PERFORMANCE_LOG_INTERVAL_MS 5000 /**< პერფორმანსის ლოგის ინტერვალი */
/** @} */

#endif /* FRAMEWORK_CONFIG_H */