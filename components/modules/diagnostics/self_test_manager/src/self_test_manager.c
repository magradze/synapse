/**
 * @file self_test_manager.c
 * @brief Implementation of the Self Test Manager module.
 * @author Synapse Framework Team
 * @version 1.2.0
 * @date 2025-07-16
 * @details This module provides a centralized mechanism for running
 *          diagnostics on various framework components and services.
 *          It is controlled via the Command Router and reports results
 *          to the console. The tests are executed in a non-blocking
 *          manner in a dedicated FreeRTOS task to avoid blocking the
 *          main application or the command shell.
 */

// --- Framework & System Includes ---
#include "self_test_manager.h"
#include "base_module.h"
#include "logging.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "framework_events.h"
#include "service_locator.h"
#include "cmd_router_interface.h"
#include "health_interface.h"
#include "storage_interface.h"
#include "system_manager_interface.h"
#include "event_payloads.h" // For telemetry payloads
#include "security_status_interface.h"
#include "ssd1306_interface.h"

// --- ESP-IDF & Standard Lib Includes ---
#include "esp_log.h"
#include "esp_timer.h"
#include "sdkconfig.h" // For Kconfig values
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h> // For va_list

DEFINE_COMPONENT_TAG("SELF_TEST_MGR");

// =========================================================================
//                      Internal Definitions & Structures
// =========================================================================

/**
 * @internal
 * @enum test_result_t
 * @brief Enumerates the possible outcomes of a single self-test.
 */
typedef enum
{
    TEST_RESULT_NOT_RUN, /**< @brief The test has not been executed yet. */
    TEST_RESULT_PASS,    /**< @brief The test completed successfully. */
    TEST_RESULT_FAIL,    /**< @brief The test failed due to a critical error. */
    TEST_RESULT_WARN,    /**< @brief The test passed, but with non-critical issues. */
    TEST_RESULT_SKIPPED  /**< @brief The test was skipped, e.g., due to a missing dependency. */
} test_result_t;

/**
 * @internal
 * @brief Converts a test_result_t enum to its string representation for printing.
 * @param[in] result The enum value to convert.
 * @return A constant string representing the test result.
 */
const char *test_result_to_str(test_result_t result)
{
    switch (result)
    {
    case TEST_RESULT_PASS:
        return "PASS";
    case TEST_RESULT_FAIL:
        return "FAIL";
    case TEST_RESULT_WARN:
        return "WARN";
    case TEST_RESULT_SKIPPED:
        return "SKIPPED";
    default:
        return "NOT RUN";
    }
}

/**
 * @internal
 * @struct test_report_line_t
 * @brief Represents a single line in the final self-test report.
 */
typedef struct
{
    char description[64]; /**< @brief A short description of the test case. */
    test_result_t result; /**< @brief The outcome of the test. */
    char details[96];     /**< @brief Additional details, e.g., error message or measured value. */
} test_report_line_t;

/**
 * @internal
 * @struct self_test_private_data_t
 * @brief Private data structure for the Self Test Manager module.
 * @details Holds the module's state, including the test report,
 *          synchronization primitives, and task handles.
 */
typedef struct
{
    char instance_name[CONFIG_SELF_TEST_MANAGER_INSTANCE_NAME_MAX_LEN];   /**< @brief Unique instance name. */
    test_report_line_t report[CONFIG_SELF_TEST_MANAGER_REPORT_MAX_LINES]; /**< @brief Array to store test results. */
    uint8_t report_line_count;                                            /**< @brief Current number of lines in the report. */
    int64_t last_run_timestamp;                                           /**< @brief Timestamp of the last test run. */
    SemaphoreHandle_t report_mutex;                                       /**< @brief Mutex to protect access to the report. */
    TaskHandle_t test_task_handle;                                        /**< @brief Handle to the currently running test task, if any. */
} self_test_private_data_t;

/**
 * @internal
 * @struct test_task_params_t
 * @brief A structure to pass multiple arguments to the test task.
 * @details This is necessary because xTaskCreate only accepts a single `void*` parameter.
 */
typedef struct
{
    module_t *self;     /**< @brief A pointer to the module instance itself. */
    char *suite_to_run; /**< @brief A dynamically allocated string with the name of the test suite to run. */
} test_task_params_t;

// --- Forward Declarations ---
static esp_err_t self_test_manager_init(module_t *self);
static void self_test_manager_deinit(module_t *self);
static void self_test_manager_handle_event(module_t *self, const char *event_name, void *event_data);
static esp_err_t cmd_handler(int argc, char **argv, void *context);
static void run_tests_task(void *pvParameters);
static cJSON *generate_report_json(self_test_private_data_t *private_data); // <<< FIX: Add forward declaration
static void add_report_line(self_test_private_data_t *private_data, const char *desc, test_result_t result, const char *details_fmt, ...);

// Test suite function declarations
static void run_core_services_check(self_test_private_data_t *private_data);
static void run_system_health_check(self_test_private_data_t *private_data);
static void run_storage_check(self_test_private_data_t *private_data);
static void run_connectivity_check(self_test_private_data_t *private_data);
static void run_security_check(self_test_private_data_t *private_data);
static void run_display_check(self_test_private_data_t *private_data);

// =========================================================================
//                      Module Lifecycle & Core Logic
// =========================================================================

/**
 * @brief Creates a new instance of the Self Test Manager module.
 * @details This function allocates memory for the module and its private data,
 *          initializes its state, and sets up its base function pointers.
 * @param[in] config Module configuration from system_config.json.
 * @return A pointer to the created module, or NULL on failure.
 */
module_t *self_test_manager_create(const cJSON *config)
{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    self_test_private_data_t *private_data = (self_test_private_data_t *)calloc(1, sizeof(self_test_private_data_t));
    if (!module || !private_data)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        free(module);
        free(private_data);
        return NULL;
    }

    private_data->report_mutex = xSemaphoreCreateMutex();
    if (!private_data->report_mutex)
    {
        ESP_LOGE(TAG, "Failed to create report mutex");
        free(private_data);
        free(module);
        return NULL;
    }
    private_data->test_task_handle = NULL; // Explicitly initialize to NULL

    module->private_data = private_data;
    const char *instance_name = CONFIG_SELF_TEST_MANAGER_DEFAULT_INSTANCE_NAME;
    // Configuration parsing (if any) would go here
    if (config)
    {
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node))
        {
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node) && name_node->valuestring)
            {
                instance_name = name_node->valuestring;
                ESP_LOGI(TAG, "Found 'instance_name' in config: \"%s\"", instance_name);
            }
        }
    }

    strncpy(private_data->instance_name, instance_name, sizeof(private_data->instance_name) - 1);
    snprintf(module->name, sizeof(module->name), "%s", instance_name);

    module->init_level = 80;
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->base.init = self_test_manager_init;
    module->base.deinit = self_test_manager_deinit;
    module->base.handle_event = self_test_manager_handle_event;

    ESP_LOGI(TAG, "Self Test Manager module created: '%s'", private_data->instance_name);
    return module;
}

/**
 * @internal
 * @brief Initializes the Self Test Manager module.
 * @details Subscribes to the system start event to know when it's safe
 *          to register its CLI command.
 * @param[in] self A pointer to the module instance.
 * @return ESP_OK on success.
 */
static esp_err_t self_test_manager_init(module_t *self)
{
    if (!self)
        return ESP_ERR_INVALID_ARG;
    ESP_LOGI(TAG, "Initializing module: %s", self->name);

    fmw_event_bus_subscribe(FMW_EVENT_SYSTEM_START_COMPLETE, self);

    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

/**
 * @internal
 * @brief Deinitializes the module, freeing all resources.
 * @details Unregisters the CLI command, unsubscribes from events, and
 *          deletes the mutex and allocated memory.
 * @param[in] self A pointer to the module instance.
 */
static void self_test_manager_deinit(module_t *self)
{
    if (!self)
        return;
    self_test_private_data_t *private_data = (self_test_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Deinitializing module: %s", self->name);

    service_handle_t cmd_router = fmw_service_get("main_cmd_router");
    if (cmd_router)
    {
        ((cmd_router_api_t *)cmd_router)->unregister_command("selftest");
    }

    fmw_event_bus_unsubscribe(FMW_EVENT_SYSTEM_START_COMPLETE, self);
    if (private_data->report_mutex)
        vSemaphoreDelete(private_data->report_mutex);
    free(private_data);
    free(self);
}

/**
 * @internal
 * @brief Handles events from the Event Bus.
 * @details Specifically listens for FMW_EVENT_SYSTEM_START_COMPLETE to register
 *          its command with the now-ready Command Router.
 * @param[in] self A pointer to the module instance.
 * @param[in] event_name The name of the received event.
 * @param[in] event_data A pointer to the event data wrapper.
 */
static void self_test_manager_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (strcmp(event_name, FMW_EVENT_SYSTEM_START_COMPLETE) == 0)
    {
        ESP_LOGI(TAG, "System start complete, registering CLI command.");
        service_handle_t cmd_router = fmw_service_get("main_cmd_router");
        if (cmd_router)
        {
            static cmd_t selftest_cmd = {
                .command = "selftest",
                .help = "Run system self-diagnostics.",
                .usage = "selftest [--run [all|core|health|storage|conn|security|display]] [--report]",
                .min_args = 2,
                .max_args = 3,
                .handler = cmd_handler,
            };
            selftest_cmd.context = self;
            ((cmd_router_api_t *)cmd_router)->register_command(&selftest_cmd);
        }
        else
        {
            ESP_LOGE(TAG, "Command Router service not found. Cannot register command.");
        }
    }
    if (event_data)
    {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}

// =========================================================================
//                      CLI Command Handling & Reporting
// =========================================================================

/**
 * @internal
 * @brief Adds a new line to the self-test report in a thread-safe manner.
 * @param[in] private_data Pointer to the module's private data.
 * @param[in] desc A short description of the test.
 * @param[in] result The outcome of the test.
 * @param[in] details_fmt A printf-style format string for additional details.
 * @param[in] ... Optional arguments for the format string.
 */
static void add_report_line(self_test_private_data_t *private_data, const char *desc, test_result_t result, const char *details_fmt, ...)
{
    if (private_data->report_line_count >= CONFIG_SELF_TEST_MANAGER_REPORT_MAX_LINES)
        return;

    if (xSemaphoreTake(private_data->report_mutex, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Failed to lock report to add line: %s", desc);
        return;
    }

    test_report_line_t *line = &private_data->report[private_data->report_line_count];
    strncpy(line->description, desc, sizeof(line->description) - 1);
    line->result = result;

    if (details_fmt && strlen(details_fmt) > 0)
    {
        va_list args;
        va_start(args, details_fmt);
        vsnprintf(line->details, sizeof(line->details), details_fmt, args);
        va_end(args);
    }
    else
    {
        line->details[0] = '\0';
    }
    private_data->report_line_count++;

    xSemaphoreGive(private_data->report_mutex);
}

/**
 * @internal
 * @brief Prints the last generated self-test report to the console.
 * @param[in] private_data Pointer to the module's private data.
 */
static void print_report(self_test_private_data_t *private_data)
{
    if (xSemaphoreTake(private_data->report_mutex, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        printf("Error: Could not lock report.\n");
        return;
    }

    if (private_data->last_run_timestamp == 0)
    {
        printf("No test report available. Run 'selftest --run' first.\n");
        xSemaphoreGive(private_data->report_mutex);
        return;
    }

    printf("----------------------------------\n");
    printf("  Synapse Self-Test Report\n");
    printf("----------------------------------\n");
    printf("  Last Run: %lld ms ago\n\n", (esp_timer_get_time() - private_data->last_run_timestamp) / 1000);

    for (int i = 0; i < private_data->report_line_count; i++)
    {
        test_report_line_t *line = &private_data->report[i];
        printf("  [%-7s] %-30s %s\n", test_result_to_str(line->result), line->description, line->details);
    }
    printf("----------------------------------\n");

    xSemaphoreGive(private_data->report_mutex);
}

/**
 * @internal
 * @brief The handler function for the 'selftest' CLI command.
 * @details Parses command arguments and either prints the last report or
 *          starts a new test run in a separate task.
 * @param[in] argc Argument count.
 * @param[in] argv Argument vector.
 * @param[in] context Pointer to the module instance.
 * @return ESP_OK on success, or an error code.
 */
static esp_err_t cmd_handler(int argc, char **argv, void *context)
{
    module_t *self = (module_t *)context;
    self_test_private_data_t *private_data = (self_test_private_data_t *)self->private_data;

    if (argc < 2)
    {
        printf("Error: Missing argument.\nUsage: %s\n", "selftest [--run [suite]] [--report]");
        return ESP_ERR_INVALID_ARG;
    }

    if (strcmp(argv[1], "--report") == 0)
    {
        // Print human-readable report to console
        print_report(private_data);

        // Generate and publish JSON report as an event
        cJSON *report_json = generate_report_json(private_data);
        if (report_json)
        {
            char *json_string = cJSON_PrintUnformatted(report_json);
            if (json_string)
            {
                // Wrap and post the event
                event_data_wrapper_t *wrapper;
                fmw_telemetry_payload_t *payload = malloc(sizeof(fmw_telemetry_payload_t));
                if (payload)
                {
                    strncpy(payload->module_name, self->name, sizeof(payload->module_name) - 1);
                    payload->json_data = json_string; // The string will be freed by the payload's free function

                    if (fmw_event_data_wrap(payload, fmw_telemetry_payload_free, &wrapper) == ESP_OK)
                    {
                        fmw_event_bus_post(FMW_EVENT_SELF_TEST_REPORT_READY, wrapper);
                        fmw_event_data_release(wrapper);
                    }
                    else
                    {
                        free(json_string);
                        free(payload);
                    }
                }
                else
                {
                    free(json_string);
                }
            }
            cJSON_Delete(report_json);
        }
    }
    else if (strcmp(argv[1], "--run") == 0)
    {
        if (private_data->test_task_handle != NULL)
        {
            printf("Error: A test is already in progress.\n");
            return ESP_ERR_INVALID_STATE;
        }

        const char *suite_to_run = (argc == 3) ? argv[2] : "all";

        printf("Starting self-test sequence for suite: '%s'...\n", suite_to_run);

        test_task_params_t *task_params = malloc(sizeof(test_task_params_t));
        if (!task_params)
        {
            printf("Error: Failed to allocate memory for task parameters.\n");
            return ESP_ERR_NO_MEM;
        }

        task_params->self = self;
        task_params->suite_to_run = strdup(suite_to_run);
        if (!task_params->suite_to_run)
        {
            printf("Error: Failed to allocate memory for suite name.\n");
            free(task_params);
            return ESP_ERR_NO_MEM;
        }

        BaseType_t task_created = xTaskCreate(run_tests_task, "selftest_task", CONFIG_SELF_TEST_MANAGER_TASK_STACK_SIZE, task_params, 5, &private_data->test_task_handle);

        if (task_created != pdPASS)
        {
            printf("Error: Failed to create self-test task.\n");
            private_data->test_task_handle = NULL;
            free(task_params->suite_to_run);
            free(task_params);
            return ESP_FAIL;
        }
    }
    else
    {
        printf("Error: Unknown argument '%s'.\nUsage: %s\n", argv[1], "selftest [--run [suite]] [--report]");
        return ESP_ERR_INVALID_ARG;
    }
    return ESP_OK;
}

// =========================================================================
//                      Test Execution Task & Suites
// =========================================================================

/**
 * @internal
 * @brief The main FreeRTOS task that executes all or specific test suites.
 * @param[in] pvParameters A pointer to a dynamically allocated
 *                        `test_task_params_t` structure.
 */
static void run_tests_task(void *pvParameters)
{
    test_task_params_t *task_params = (test_task_params_t *)pvParameters;
    module_t *self = task_params->self;
    char *suite_to_run = task_params->suite_to_run;

    self_test_private_data_t *private_data = (self_test_private_data_t *)self->private_data;

    if (xSemaphoreTake(private_data->report_mutex, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Failed to lock report for new test run.");
        goto cleanup;
    }
    private_data->report_line_count = 0;
    memset(private_data->report, 0, sizeof(private_data->report));
    private_data->last_run_timestamp = esp_timer_get_time();
    xSemaphoreGive(private_data->report_mutex);

    bool run_all = (strcmp(suite_to_run, "all") == 0);

    if (run_all || strcmp(suite_to_run, "core") == 0)
    {
        printf("Running Core Services Check...\n");
        run_core_services_check(private_data);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    if (run_all || strcmp(suite_to_run, "health") == 0)
    {
        printf("Running System Health Check...\n");
        run_system_health_check(private_data);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    if (run_all || strcmp(suite_to_run, "storage") == 0)
    {
        printf("Running Storage Subsystem Check...\n");
        run_storage_check(private_data);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    if (run_all || strcmp(suite_to_run, "conn") == 0)
    {
        printf("Running Connectivity Check...\n");
        run_connectivity_check(private_data);
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    if (run_all || strcmp(suite_to_run, "security") == 0)
    {
        printf("Running Security Status Check...\n");
        run_security_check(private_data);
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    if (run_all || strcmp(suite_to_run, "display") == 0)
    {
        printf("Running Display Check...\n");
        run_display_check(private_data);
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    printf("\nSelf-test sequence complete. Use 'selftest --report' to see results.\n");

cleanup:
    ESP_LOGI(TAG, "Self-test task finishing. Clearing task handle.");
    private_data->test_task_handle = NULL;
    free(suite_to_run);
    free(task_params);
    vTaskDelete(NULL);
}

/**
 * @internal
 * @brief Test suite for checking the availability of core framework services.
 * @param[in] private_data Pointer to the module's private data to store results.
 */
static void run_core_services_check(self_test_private_data_t *private_data)
{
    const char *services_to_check[] = {
        "main_cmd_router", "main_timer_service", "main_storage", "main_identity_service", NULL};
    bool all_pass = true;
    for (int i = 0; services_to_check[i] != NULL; i++)
    {
        if (fmw_service_get(services_to_check[i]) == NULL)
        {
            all_pass = false;
            ESP_LOGE(TAG, "Core service check failed for: %s", services_to_check[i]);
        }
    }
    add_report_line(private_data, "Core Services Check",
                    all_pass ? TEST_RESULT_PASS : TEST_RESULT_FAIL,
                    all_pass ? "" : "One or more core services not found");
}

/**
 * @internal
 * @brief Test suite for checking system resources via the Health Monitor service.
 * @param[in] private_data Pointer to the module's private data to store results.
 */
static void run_system_health_check(self_test_private_data_t *private_data)
{
    service_handle_t health_handle = fmw_service_get("health_monitor");
    if (!health_handle)
    {
        add_report_line(private_data, "System Health Check", TEST_RESULT_SKIPPED, "Health Monitor service not found");
        return;
    }
    health_api_t *health_api = (health_api_t *)health_handle;
    cJSON *report = NULL;
    esp_err_t ret = health_api->get_system_health_report(&report);
    if (ret != ESP_OK || !report)
    {
        add_report_line(private_data, "System Health Check", TEST_RESULT_FAIL, "Failed to get health report");
        if (report)
            cJSON_Delete(report);
        return;
    }
    const cJSON *heap = cJSON_GetObjectItem(report, "free_heap_bytes");
    if (cJSON_IsNumber(heap))
    {
        add_report_line(private_data, "Free Heap", TEST_RESULT_PASS, "%d bytes", heap->valueint);
    }
    else
    {
        add_report_line(private_data, "Free Heap", TEST_RESULT_FAIL, "Could not read value");
    }
    cJSON_Delete(report);
}

/**
 * @internal
 * @brief Test suite for the Storage Manager service.
 * @details Performs a write, read, and delete operation to verify
 *          the storage backend is functional. It dynamically finds the
 *          storage service by its type, making it independent of the
 *          instance name in the configuration.
 * @param[in] private_data Pointer to the module's private data.
 */
static void run_storage_check(self_test_private_data_t *private_data)
{
    // Find the service by its type, not by a hardcoded name.
    // This is the architecturally correct way to get a dependency.
    service_handle_t storage_handle = fmw_service_lookup_by_type(FMW_SERVICE_TYPE_NVRAM_API);

    if (!storage_handle)
    {
        add_report_line(private_data, "Storage Check", TEST_RESULT_SKIPPED, "Storage Manager service not found");
        return;
    }
    storage_api_t *storage_api = (storage_api_t *)storage_handle;

    const char *test_key = "selftest_dummy";
    const char *test_val = "12345";
    char read_buf[16];
    size_t read_len = sizeof(read_buf);

    // Use the module's own instance name as the owner to create a unique namespace.
    esp_err_t err = storage_api->set_string(private_data->instance_name, test_key, test_val);
    if (err != ESP_OK)
    {
        add_report_line(private_data, "Storage Check", TEST_RESULT_FAIL, "set_string failed: %s", esp_err_to_name(err));
        return;
    }

    err = storage_api->get_string(private_data->instance_name, test_key, read_buf, &read_len);
    if (err != ESP_OK)
    {
        add_report_line(private_data, "Storage Check", TEST_RESULT_FAIL, "get_string failed: %s", esp_err_to_name(err));
        storage_api->erase_key(private_data->instance_name, test_key);
        return;
    }

    if (strcmp(test_val, read_buf) != 0)
    {
        add_report_line(private_data, "Storage Check", TEST_RESULT_FAIL, "Readback value mismatch");
        storage_api->erase_key(private_data->instance_name, test_key);
        return;
    }

    err = storage_api->erase_key(private_data->instance_name, test_key);
    if (err != ESP_OK)
    {
        add_report_line(private_data, "Storage Check", TEST_RESULT_WARN, "erase_key failed: %s", esp_err_to_name(err));
        return;
    }

    add_report_line(private_data, "Storage Check", TEST_RESULT_PASS, "Write/Read/Erase OK");
}

/**
 * @internal
 * @brief Test suite for basic connectivity services.
 * @details Checks if WiFi and MQTT services are available and running by
 *          querying their status via the System Manager service.
 * @param[in] private_data Pointer to the module's private data.
 */
static void run_connectivity_check(self_test_private_data_t *private_data)
{
    service_handle_t sys_handle = fmw_service_get("system_manager");
    if (!sys_handle)
    {
        add_report_line(private_data, "Connectivity Check", TEST_RESULT_FAIL, "System Manager not found");
        return;
    }
    system_manager_api_t *sys_api = (system_manager_api_t *)sys_handle;
    const module_t **all_modules = NULL;
    uint8_t module_count = 0;
    if (sys_api->get_all_modules(&all_modules, &module_count) != ESP_OK)
    {
        add_report_line(private_data, "Connectivity Check", TEST_RESULT_FAIL, "Could not get module list");
        return;
    }

    const char *modules_to_check[] = {"main_wifi_manager", "main_mqtt_broker", NULL};
    for (int i = 0; modules_to_check[i] != NULL; i++)
    {
        bool found = false;
        for (int j = 0; j < module_count; j++)
        {
            if (strcmp(all_modules[j]->name, modules_to_check[i]) == 0)
            {
                add_report_line(private_data, modules_to_check[i],
                                (all_modules[j]->status == MODULE_STATUS_RUNNING) ? TEST_RESULT_PASS : TEST_RESULT_FAIL,
                                (all_modules[j]->status == MODULE_STATUS_RUNNING) ? "Running" : "Not Running");
                found = true;
                break;
            }
        }
        if (!found)
        {
            add_report_line(private_data, modules_to_check[i], TEST_RESULT_FAIL, "Module not found in registry");
        }
    }
}

/**
 * @internal
 * @brief Generates a cJSON object containing the test report.
 * @param[in] private_data Pointer to the module's private data.
 * @return A pointer to the created cJSON object, or NULL on failure.
 *         The caller is responsible for deleting the returned object.
 */
static cJSON* generate_report_json(self_test_private_data_t *private_data) {
    if (xSemaphoreTake(private_data->report_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return NULL;
    }

    if (private_data->last_run_timestamp == 0) {
        xSemaphoreGive(private_data->report_mutex);
        return NULL; // No report available
    }

    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "last_run_ms_ago", (esp_timer_get_time() - private_data->last_run_timestamp) / 1000);
    
    cJSON *results_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "results", results_array);

    for (int i = 0; i < private_data->report_line_count; i++) {
        test_report_line_t *line = &private_data->report[i];
        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "test", line->description);
        cJSON_AddStringToObject(item, "status", test_result_to_str(line->result));
        cJSON_AddStringToObject(item, "details", line->details);
        cJSON_AddItemToArray(results_array, item);
    }

    xSemaphoreGive(private_data->report_mutex);
    return root;
}

/**
 * @internal
 * @brief Test suite for the Security Status Reporter service.
 * @details It finds the security service by its type and checks the status
 *          of Secure Boot and Flash Encryption. For a production device,
 *          it's expected that both are enabled.
 * @param[in] private_data Pointer to the module's private data to store results.
 */
static void run_security_check(self_test_private_data_t *private_data)
{
    service_handle_t sec_handle = fmw_service_lookup_by_type(FMW_SERVICE_TYPE_SECURITY_API);
    if (!sec_handle)
    {
        add_report_line(private_data, "Security Check", TEST_RESULT_SKIPPED, "Service not found");
        return;
    }
    security_status_api_t *sec_api = (security_status_api_t *)sec_handle;

    bool sb_enabled = sec_api->is_secure_boot_enabled();
    bool fe_enabled = sec_api->is_flash_encryption_enabled();

    add_report_line(private_data, "Secure Boot",
                    sb_enabled ? TEST_RESULT_PASS : TEST_RESULT_WARN,
                    sb_enabled ? "Enabled" : "Disabled (OK for Dev)");

    add_report_line(private_data, "Flash Encryption",
                    fe_enabled ? TEST_RESULT_PASS : TEST_RESULT_WARN,
                    fe_enabled ? "Enabled" : "Disabled (OK for Dev)");

    // Overall check
    if (sb_enabled && fe_enabled)
    {
        add_report_line(private_data, "Overall Security", TEST_RESULT_PASS, "Device is secured");
    }
    else
    {
        add_report_line(private_data, "Overall Security", TEST_RESULT_WARN, "Device not in production secure state");
    }
}

/**
 * @internal
 * @brief Test suite for the SSD1306 Display module.
 * @details Checks if the display service is available and performs a quick
 *          visual test by drawing various graphical primitives.
 */
static void run_display_check(self_test_private_data_t *private_data)
{
    const char *test_name = "Display Check";

    // 1. Find the display service
    service_handle_t display_handle = fmw_service_get("main_display");
    if (!display_handle)
    {
        add_report_line(private_data, test_name, TEST_RESULT_SKIPPED, "Display service not found");
        return;
    }
    ssd1306_handle_t *display = (ssd1306_handle_t *)display_handle;

    // 2. Perform drawing operations on the internal buffer
    display->api->clear(display->context);
    display->api->write_text(display->context, 0, 0, "Display Test:");

    // Draw some graphics
    display->api->draw_hline(display->context, 0, 10, 128);
    display->api->draw_rect(display->context, 5, 15, 30, 20);
    display->api->fill_rect(display->context, 40, 15, 30, 20);
    display->api->draw_vline(display->context, 75, 12, 25);

    display->api->write_text(display->context, 0, 5, "GFX OK!");

    // 3. Push the buffer to the screen to make it visible
    if (display->api->update_screen(display->context) == ESP_OK)
    {
        add_report_line(private_data, test_name, TEST_RESULT_PASS, "GFX primitives drawn");
    }
    else
    {
        add_report_line(private_data, test_name, TEST_RESULT_FAIL, "Failed to update screen");
    }

    vTaskDelay(pdMS_TO_TICKS(3000));

    // Clean up by clearing the screen and pushing the changes
    ESP_LOGI(TAG, "Clearing display after test...");
    display->api->clear(display->context);
    display->api->update_screen(display->context);
}