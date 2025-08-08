/**
 * @file command_router.c
 * @brief Command Router & System Shell Module Implementation (FIXED)
 * @author Synapse Framework Team & Claude AI
 * @version 1.4.0
 * @date 2025-07-04
 * @details
 * Final, robust implementation that correctly handles UART conflicts between
 * logging and the console by properly initializing the VFS and UART driver
 * for esp_console.
 */

// --- Includes ---
#include "synapse.h"
#include "command_router.h"
#include "cmd_router_interface.h"

#include "system_manager_interface.h"
#include "sdkconfig.h"

// ESP-IDF & Standard Library Includes
#include "esp_log.h"
#include "esp_console.h"
#include "esp_system.h"
#include "esp_vfs.h"          // For VFS
#include "driver/uart.h"      // For UART driver
#include "driver/uart_vfs.h"
#include "esp_vfs_dev.h" // <-- Add this include for esp_vfs_uart_use_driver
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "nvs.h"
#include "nvs_flash.h"

// --- Component Tag ---
DEFINE_COMPONENT_TAG("CMD_ROUTER");

// --- Type Definitions ---

/**
 * @internal
 * @struct cmd_router_private_data_t
 * @brief Private data structure for the Command Router module.
 * @details Holds the module's internal state, configuration, and resources,
 *          such as the list of registered commands and the serial shell task handle.
 */
typedef struct {
    bool serial_shell_enabled;
    char prompt[CONFIG_COMMAND_ROUTER_PROMPT_MAX_LEN];
    TaskHandle_t serial_shell_task_handle;
    const cmd_t* registered_commands[CONFIG_COMMAND_ROUTER_MAX_COMMANDS];
    uint8_t command_count;
    SemaphoreHandle_t commands_mutex;
    bool console_initialized;
    volatile bool is_running;
} cmd_router_private_data_t;



// --- Forward Declarations for Internal Functions ---
static esp_err_t command_router_init(module_t *self);
static esp_err_t command_router_start(module_t *self);
static void command_router_deinit(module_t *self);
static void command_router_handle_event(module_t *self, const char *event_name, void *event_data);
static void serial_shell_task(void *pvParameters);
static int generic_command_executor(int argc, char **argv);
static esp_err_t register_all_commands_to_console(void);
static esp_err_t cmd_handler_help(int argc, char **argv, void *context);
static esp_err_t cmd_handler_modules(int argc, char **argv, void *context);
static esp_err_t cmd_handler_nvs_inspect(int argc, char **argv, void *context);
static esp_err_t cmd_handler_reboot(int argc, char **argv, void *context);
static esp_err_t service_api_register_command(const cmd_t *command);
static esp_err_t service_api_unregister_command(const char *command_name);
static bool service_api_is_command_registered(const char *command_name);

// --- Module-static variables ---
static module_t *global_cmd_router_instance = NULL;
static cmd_router_api_t command_router_service_api = {
    .register_command = service_api_register_command,
    .unregister_command = service_api_unregister_command,
    .is_command_registered = service_api_is_command_registered,
};

/**
 * @internal
 * @brief A generic argument table for all commands.
 * @details Since we do our own argument parsing inside the handlers,
 *          we provide a simple, generic argtable to esp_console that
 *          accepts any arguments. This must be static to remain valid.
 */
static struct {
    struct arg_end *end;
} generic_command_args;

// =========================================================================
//                      Module Lifecycle Implementation
// =========================================================================

/**
 * @brief Creates a new instance of the Command Router module.
 * @see command_router.h
 */
module_t *command_router_create(const cJSON *config)
{
    ESP_LOGI(TAG, "Creating Command Router module instance...");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module) {
        ESP_LOGE(TAG, "Failed to allocate memory for module_t.");
        return NULL;
    }

    cmd_router_private_data_t *private_data = (cmd_router_private_data_t *)calloc(1, sizeof(cmd_router_private_data_t));
    if (!private_data) {
        ESP_LOGE(TAG, "Failed to allocate memory for private_data.");
        free(module);
        return NULL;
    }
    module->private_data = private_data;

    module->state_mutex = xSemaphoreCreateMutex();
    private_data->commands_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex || !private_data->commands_mutex) {
        ESP_LOGE(TAG, "Failed to create mutexes.");
        if (module->state_mutex) vSemaphoreDelete(module->state_mutex);
        if (private_data->commands_mutex) vSemaphoreDelete(private_data->commands_mutex);
        free(private_data);
        free(module);
        return NULL;
    }

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    const char *instance_name = CONFIG_COMMAND_ROUTER_DEFAULT_INSTANCE_NAME;
    private_data->serial_shell_enabled = CONFIG_COMMAND_ROUTER_ENABLE_SERIAL_SHELL;
    strncpy(private_data->prompt, CONFIG_COMMAND_ROUTER_PROMPT, sizeof(private_data->prompt) - 1);
    private_data->prompt[sizeof(private_data->prompt) - 1] = '\0'; // Ensure null-termination

    if (cJSON_IsObject(config_node)) {
        const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
        if (cJSON_IsString(name_node)) instance_name = name_node->valuestring;

        const cJSON *shell_node = cJSON_GetObjectItem(config_node, "enable_serial_shell");
        if (cJSON_IsBool(shell_node)) private_data->serial_shell_enabled = cJSON_IsTrue(shell_node);

        const cJSON *prompt_node = cJSON_GetObjectItem(config_node, "serial_shell_prompt");
        if (cJSON_IsString(prompt_node)) {
            strncpy(private_data->prompt, prompt_node->valuestring, sizeof(private_data->prompt) - 1);
            private_data->prompt[sizeof(private_data->prompt) - 1] = '\0';
        }
    }

    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;
    module->init_level = 50;
    module->base.init = command_router_init;
    module->base.start = command_router_start;
    module->base.deinit = command_router_deinit;
    module->base.handle_event = command_router_handle_event;

    global_cmd_router_instance = module;

    ESP_LOGI(TAG, "Command Router module '%s' created.", module->name);
    return module;
}

/**
 * @internal
 * @brief Initializes the Command Router module.
 * @details This function registers the module's service API, registers all
 *          built-in commands, and subscribes to the command execution event.
 * @param self Pointer to the module instance.
 * @return ESP_OK on success, or an error code on failure.
 */
static esp_err_t command_router_init(module_t *self)
{
    if (!self) return ESP_ERR_INVALID_ARG;
    ESP_LOGI(TAG, "Initializing Command Router module: %s", self->name);

    esp_err_t err = synapse_service_register(self->name, SYNAPSE_SERVICE_TYPE_CMD_ROUTER_API, &command_router_service_api);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register Command Router service: %s", esp_err_to_name(err));
        return err;
    }

    static cmd_t help_cmd;
    help_cmd = (cmd_t){"help", "Show list of available commands", "help", 1, 1, cmd_handler_help, self};
    
    static cmd_t modules_cmd;
    modules_cmd = (cmd_t){"modules", "List all registered modules and their status", "modules", 1, 1, cmd_handler_modules, self};

    static cmd_t nvs_inspect_cmd;
    nvs_inspect_cmd = (cmd_t){"nvs_inspect", "Inspect NVS entries", "nvs_inspect", 1, 2, cmd_handler_nvs_inspect, self};

    static cmd_t reboot_cmd;
    reboot_cmd = (cmd_t){"reboot", "Reboot the device", "reboot", 1, 1, cmd_handler_reboot, self};
    
    service_api_register_command(&help_cmd);
    service_api_register_command(&modules_cmd);
    service_api_register_command(&nvs_inspect_cmd);
    service_api_register_command(&reboot_cmd);

    err = synapse_event_bus_subscribe(SYNAPSE_EVENT_EXECUTE_COMMAND_STRING, self);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to subscribe to command events: %s", esp_err_to_name(err));
        synapse_service_unregister(self->name);
        return err;
    }

    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "Command Router module initialized successfully.");
    return ESP_OK;
}

/**
 * @internal
 * @brief Starts the Command Router module's operational tasks.
 * @details If enabled in the configuration, this function creates and starts
 *          the interactive serial shell task.
 * @param self Pointer to the module instance.
 * @return ESP_OK on success, or ESP_FAIL if the task creation fails.
 */
static esp_err_t command_router_start(module_t *self)
{
    if (!self) return ESP_ERR_INVALID_ARG;
    cmd_router_private_data_t *private_data = (cmd_router_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Starting Command Router module: %s", self->name);

    if (private_data->serial_shell_enabled) {
        BaseType_t task_created = xTaskCreate(
            serial_shell_task, "serial_shell_task",
            CONFIG_COMMAND_ROUTER_SERIAL_TASK_STACK_SIZE, self,
            CONFIG_COMMAND_ROUTER_SERIAL_TASK_PRIORITY, &private_data->serial_shell_task_handle
        );
        if (task_created != pdPASS) {
            ESP_LOGE(TAG, "Failed to create serial shell task.");
            return ESP_FAIL;
        }
    }

    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

/**
 * @internal
 * @brief Deinitializes the module and frees all allocated resources.
 * @param self Pointer to the module instance to be deinitialized.
 */
static void command_router_deinit(module_t *self)
{
    if (!self) return;
    cmd_router_private_data_t *private_data = (cmd_router_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Deinitializing Command Router module: %s", self->name);

    // --- Graceful shutdown for the serial shell task ---
    if (private_data->serial_shell_task_handle) {
        // 1. Signal the task to stop its loop
        private_data->is_running = false;

        // 2. Unblock linenoise by writing a newline to UART
        // This is a crucial step to make the task exit `linenoise()` call.
        uart_write_bytes(CONFIG_ESP_CONSOLE_UART_NUM, "\n", 1);

        // 3. Wait a moment for the task to process the signal and exit
        vTaskDelay(pdMS_TO_TICKS(100));

        // 4. The task will deinit the console and delete itself.
        // We just nullify the handle.
        private_data->serial_shell_task_handle = NULL;
    }

    // Unsubscribe from events and unregister the service
    synapse_event_bus_unsubscribe(SYNAPSE_EVENT_EXECUTE_COMMAND_STRING, self);
    synapse_service_unregister(self->name);

    // Free resources
    if (private_data->commands_mutex) vSemaphoreDelete(private_data->commands_mutex);
    if (self->state_mutex) vSemaphoreDelete(self->state_mutex);
    
    free(private_data);

    global_cmd_router_instance = NULL;
    // free(self) is now handled by the System Manager
}

// =========================================================================
//                      Event & Command Handling
// =========================================================================

/**
 * @internal
 * @brief Handles events received from the Event Bus.
 * @details This function is the entry point for commands sent programmatically
 *          from other modules. It parses the payload and executes the command.
 * @param self Pointer to the module instance.
 * @param event_name The name of the received event.
 * @param event_data Pointer to the event data wrapper.
 */
static void command_router_handle_event(module_t *self, const char *event_name, void *event_data)
{
    if (!self || !event_name) {
        if (event_data)
            synapse_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }

    if (strcmp(event_name, SYNAPSE_EVENT_EXECUTE_COMMAND_STRING) == 0)
    {
        event_data_wrapper_t *wrapper = (event_data_wrapper_t *)event_data;
        if (wrapper && wrapper->payload) {
            synapse_command_payload_t *payload = (synapse_command_payload_t *)wrapper->payload;
            ESP_LOGI(TAG, "Executing command from event source '%s': \"%s\"", payload->source, payload->command_string);
            
            char *argv[CONFIG_COMMAND_ROUTER_MAX_ARGS];
            char *line_copy = strdup(payload->command_string);
            if(line_copy) {
                int argc = esp_console_split_argv(line_copy, argv, CONFIG_COMMAND_ROUTER_MAX_ARGS);
                if (argc > 0) {
                    generic_command_executor(argc, argv);
                }
                free(line_copy);
            }
        }
    }

    if (event_data) {
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }
}

/**
 * @internal
 * @brief Initializes the UART for the console.
 * @details This function correctly configures the UART driver and VFS for use
 *          with the esp_console component. It uses the available (though deprecated)
 *          API functions for compatibility with the current build environment.
 */
static void initialize_console()
{
    // Drain stdout/stderr to prevent printing strange characters
    fflush(stdout);
    fsync(fileno(stdout));
    fflush(stderr);
    fsync(fileno(stderr));

    // Disable buffering on stdin
    setvbuf(stdin, NULL, _IONBF, 0);

    // --- **გამოვიყენოთ ის API, რომელიც მუშაობს (გაფრთხილებებით)** ---
    // These functions in this IDF version apply to the default console UART
    // and do not take the port number as an argument.
    uart_vfs_dev_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    uart_vfs_dev_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);

    // Configure UART.
    const uart_config_t uart_config = {
            .baud_rate = CONFIG_ESP_CONSOLE_UART_BAUDRATE,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .source_clk = UART_SCLK_DEFAULT,
    };
    // Install UART driver
    ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(CONFIG_ESP_CONSOLE_UART_NUM, &uart_config));

    // Tell VFS to use UART driver for the console UART
    uart_vfs_dev_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);

    // Initialize the console
    esp_console_config_t console_config = {
            .max_cmdline_length = 256,
            .max_cmdline_args = 8,
            .hint_color = 36, // Cyan
    };
    ESP_ERROR_CHECK(esp_console_init(&console_config));

    // Clear any garbage in the input buffer before starting
    uart_flush_input(CONFIG_ESP_CONSOLE_UART_NUM);

    // Configure linenoise
    linenoiseSetMultiLine(1);
    linenoiseSetMaxLineLen(console_config.max_cmdline_length);
    linenoiseHistorySetMaxLen(100);
    linenoiseAllowEmpty(false);
}

/**
 * @internal
 * @brief Main task for the interactive serial shell.
 */
static void serial_shell_task(void *pvParameters)
{
    module_t *self = (module_t *)pvParameters;
    cmd_router_private_data_t *private_data = (cmd_router_private_data_t *)self->private_data;

    initialize_console();
    private_data->console_initialized = true; // Set flag after successful init

    ESP_ERROR_CHECK(register_all_commands_to_console());

    ESP_LOGI(TAG, "Serial console ready.");
    
    const char* prompt = private_data->prompt;
    printf("\n==================================================\n");
    printf("  Welcome to the Synapse System Shell! \n");
    printf("  Type 'help' to see available commands.\n");
    printf("==================================================\n");

    private_data->is_running = true;
    while (private_data->is_running)
    { // <--- შეცვლილია პირობა
        char *line = linenoise(prompt);
        if (line == NULL)
        {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        if (strlen(line) > 0) {
            linenoiseHistoryAdd(line);

            char *argv[CONFIG_COMMAND_ROUTER_MAX_ARGS];
            int argc = esp_console_split_argv(line, argv, CONFIG_COMMAND_ROUTER_MAX_ARGS);

            if (argc > 0)
            {
                bool command_exists = service_api_is_command_registered(argv[0]);
                if (command_exists)
                {
                    generic_command_executor(argc, argv);
                }
                else
                {
                    printf("Error: Command not found: %s\n", argv[0]);
                }
            }
        }
        linenoiseFree(line);
    }

    ESP_LOGI(TAG, "Serial shell task is shutting down.");
    if (private_data->console_initialized)
    {
        esp_console_deinit();
        private_data->console_initialized = false;
    }
    vTaskDelete(NULL);
}

/**
 * @internal
 * @brief A generic executor function that acts as a bridge to our command system.
 * @details This function is registered with `esp_console` for all commands. It finds
 *          the corresponding `cmd_t` structure in our internal list and calls the
 *          actual handler with the correct context.
 * @param argc Number of arguments.
 * @param argv Array of argument strings.
 * @return 0 on success, 1 on failure.
 */
static int generic_command_executor(int argc, char **argv)
{
    if (argc < 1) return 1;

    cmd_router_private_data_t *private_data = (cmd_router_private_data_t *)global_cmd_router_instance->private_data;
    if (xSemaphoreTake(private_data->commands_mutex, portMAX_DELAY) != pdTRUE) {
        printf("Error: Could not take command mutex\n");
        return 1;
    }

    const cmd_t *found_cmd = NULL;
    for (int i = 0; i < private_data->command_count; i++) {
        if (strcmp(argv[0], private_data->registered_commands[i]->command) == 0) {
            found_cmd = private_data->registered_commands[i];
            break;
        }
    }
    xSemaphoreGive(private_data->commands_mutex);

    if (!found_cmd) {
        // This case is handled by esp_console_run, but we add it for safety
        return 1;
    }

    if (argc < found_cmd->min_args || argc > found_cmd->max_args) {
        printf("Error: Invalid number of arguments for '%s'.\n", found_cmd->command);
        if(found_cmd->usage) printf("Usage: %s\n", found_cmd->usage);
        return 1;
    }

    esp_err_t err = found_cmd->handler(argc, argv, found_cmd->context);
    if (err != ESP_OK) {
        // The handler itself should print a more specific error.
        printf("Error: Command '%s' failed with code 0x%x (%s)\n", argv[0], err, esp_err_to_name(err));
        return 1;
    }

    return 0; // Success
}

// =========================================================================
//                      Service API Implementation
// =========================================================================

static esp_err_t service_api_register_command(const cmd_t *command)
{
    if (!global_cmd_router_instance || !command || !command->command || !command->handler) {
        return ESP_ERR_INVALID_ARG;
    }
    cmd_router_private_data_t *private_data = (cmd_router_private_data_t *)global_cmd_router_instance->private_data;

    if (xSemaphoreTake(private_data->commands_mutex, portMAX_DELAY) != pdTRUE) return ESP_ERR_TIMEOUT;

    if (private_data->command_count >= CONFIG_COMMAND_ROUTER_MAX_COMMANDS) {
        xSemaphoreGive(private_data->commands_mutex);
        ESP_LOGE(TAG, "Cannot register command '%s': command list is full.", command->command);
        return ESP_ERR_NO_MEM;
    }

    for (int i = 0; i < private_data->command_count; i++) {
        if (strcmp(private_data->registered_commands[i]->command, command->command) == 0) {
            xSemaphoreGive(private_data->commands_mutex);
            ESP_LOGE(TAG, "Cannot register command '%s': already exists.", command->command);
            return ESP_ERR_INVALID_STATE;
        }
    }

    private_data->registered_commands[private_data->command_count++] = command;
    xSemaphoreGive(private_data->commands_mutex);

    ESP_LOGI(TAG, "Command registered: '%s'", command->command);
    return ESP_OK;
}

static esp_err_t service_api_unregister_command(const char *command_name)
{
    if (!global_cmd_router_instance || !command_name) return ESP_ERR_INVALID_ARG;
    cmd_router_private_data_t *private_data = (cmd_router_private_data_t *)global_cmd_router_instance->private_data;

    if (xSemaphoreTake(private_data->commands_mutex, portMAX_DELAY) != pdTRUE) return ESP_ERR_TIMEOUT;

    int found_index = -1;
    for (int i = 0; i < private_data->command_count; i++) {
        if (strcmp(private_data->registered_commands[i]->command, command_name) == 0) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        xSemaphoreGive(private_data->commands_mutex);
        return ESP_ERR_NOT_FOUND;
    }

    for (int i = found_index; i < private_data->command_count - 1; i++) {
        private_data->registered_commands[i] = private_data->registered_commands[i + 1];
    }
    private_data->command_count--;

    xSemaphoreGive(private_data->commands_mutex);
    ESP_LOGI(TAG, "Command unregistered: '%s'", command_name);
    return ESP_OK;
}

/**
 * @internal
 * @brief API function to check if a command is registered.
 * @see cmd_router_interface.h
 */
static bool service_api_is_command_registered(const char *command_name)
{
    if (!global_cmd_router_instance || !command_name)
    {
        return false;
    }
    cmd_router_private_data_t *private_data = (cmd_router_private_data_t *)global_cmd_router_instance->private_data;

    if (xSemaphoreTake(private_data->commands_mutex, pdMS_TO_TICKS(50)) != pdTRUE)
    {
        ESP_LOGE(TAG, "Failed to take mutex to check command registration.");
        return false; // Fail-safe: assume it exists to prevent duplicates
    }

    bool found = false;
    for (int i = 0; i < private_data->command_count; i++)
    {
        if (strcmp(private_data->registered_commands[i]->command, command_name) == 0)
        {
            found = true;
            break;
        }
    }

    xSemaphoreGive(private_data->commands_mutex);
    return found;
}

// =========================================================================
//                      Built-in Command Handlers
// =========================================================================

static esp_err_t cmd_handler_help(int argc, char **argv, void *context)
{
    module_t *self = (module_t *)context;
    cmd_router_private_data_t *private_data = (cmd_router_private_data_t *)self->private_data;

    printf("Available commands:\n-------------------\n");
    if (xSemaphoreTake(private_data->commands_mutex, portMAX_DELAY) != pdTRUE) return ESP_ERR_TIMEOUT;

    for (int i = 0; i < private_data->command_count; i++) {
        const cmd_t *cmd = private_data->registered_commands[i];
        printf("  %-15s - %s\n", cmd->command, cmd->help);
        if (cmd->usage) {
            printf("    Usage: %s\n", cmd->usage);
        }
    }
    xSemaphoreGive(private_data->commands_mutex);
    printf("-------------------\n");
    return ESP_OK;
}

static esp_err_t cmd_handler_modules(int argc, char **argv, void *context)
{
    service_handle_t handle = synapse_service_get("system_manager");
    if (!handle) {
        printf("Error: System Manager service not available.\n");
        return ESP_ERR_NOT_FOUND;
    }

    synapse_service_type_t service_type;
    if (synapse_service_get_type("system_manager", &service_type) != ESP_OK || service_type != SYNAPSE_SERVICE_TYPE_SYSTEM_API)
    {
        printf("Error: Invalid service type for 'system_manager'.\n");
        return ESP_ERR_INVALID_STATE;
    }

    system_manager_api_t *sys_api = (system_manager_api_t *)handle;
    const module_t **modules = NULL;
    uint8_t module_count = 0;
    esp_err_t err = sys_api->get_all_modules(&modules, &module_count);

    if (err != ESP_OK) {
        printf("Error: Failed to retrieve module list: %s\n", esp_err_to_name(err));
        return err;
    }

    if (module_count == 0) {
        printf("No modules are currently registered in the system.\n");
        return ESP_OK;
    }

    printf("--------------------------------------------------\n");
    printf("  %-32s | %s\n", "Module Instance Name", "Status");
    printf("--------------------------------------------------\n");

    for (uint8_t i = 0; i < module_count; i++) {
        const module_t *module = modules[i];
        if (module) {
            const char *status_str = "UNKNOWN";
            switch (module->status) {
                case MODULE_STATUS_UNINITIALIZED: status_str = "Uninitialized"; break;
                case MODULE_STATUS_INITIALIZED:   status_str = "Initialized";   break;
                case MODULE_STATUS_RUNNING:       status_str = "Running";       break;
                case MODULE_STATUS_DISABLED:      status_str = "Disabled";      break;
                case MODULE_STATUS_ERROR:         status_str = "Error";         break;
                default:                          status_str = "Unknown";       break;
            }
            printf("  %-32s | %s\n", module->name, status_str);
        }
    }
    printf("--------------------------------------------------\n");
    printf("Total modules: %u\n", module_count);
    return ESP_OK;
}

static esp_err_t cmd_handler_nvs_inspect(int argc, char **argv, void *context)
{
    nvs_handle_t handle;
    if (nvs_open("synapse_cfg", NVS_READONLY, &handle) != ESP_OK)
    {
        printf("❌ Failed to open NVS namespace 'synapse_cfg'\n");
        return ESP_FAIL;
    }

    nvs_iterator_t it = NULL;
    esp_err_t err = nvs_entry_find("nvs", "synapse_cfg", NVS_TYPE_STR, &it);
    if (err != ESP_OK || it == NULL)
    {
        printf("ℹ️ No string entries found in NVS.\n");
        nvs_close(handle);
        return ESP_OK;
    }

    printf("🔍 NVS Entries in 'synapse_cfg':\n");
    while (it != NULL)
    {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);

        size_t len = 0;
        if (nvs_get_str(handle, info.key, NULL, &len) == ESP_OK && len > 0)
        {
            char *value = malloc(len);
            if (value)
            {
                if (nvs_get_str(handle, info.key, value, &len) == ESP_OK)
                {
                    printf("  🗝️  %-16s => %s\n", info.key, value);
                }
                free(value);
            }
        }

        if (nvs_entry_next(&it) != ESP_OK)
            break;
    }

    nvs_release_iterator(it);
    nvs_close(handle);
    return ESP_OK;
}

static esp_err_t cmd_handler_reboot(int argc, char **argv, void *context)
{
    ESP_LOGW(TAG, "Reboot command received. Initiating graceful shutdown.");
    synapse_system_shutdown();
    // This part of the code will not be reached
    return ESP_OK;
}
/**
 * @internal
 * @brief Registers all commands from our internal list to the esp_console system.
 */
static esp_err_t register_all_commands_to_console(void)
{
    if (!global_cmd_router_instance) return ESP_ERR_INVALID_STATE;
    cmd_router_private_data_t *private_data = (cmd_router_private_data_t *)global_cmd_router_instance->private_data;

    // Initialize the static argtable once
    generic_command_args.end = arg_end(20); // <--- **შესწორება**

    if (xSemaphoreTake(private_data->commands_mutex, portMAX_DELAY) != pdTRUE) return ESP_ERR_TIMEOUT;

    for (int i = 0; i < private_data->command_count; i++) {
        const cmd_t *cmd_to_reg = private_data->registered_commands[i];
        
        const esp_console_cmd_t console_cmd = {
            .command = cmd_to_reg->command,
            .help = cmd_to_reg->help,
            .hint = NULL,
            .func = &generic_command_executor,
            .argtable = &generic_command_args
        };
        ESP_ERROR_CHECK(esp_console_cmd_register(&console_cmd));
    }

    xSemaphoreGive(private_data->commands_mutex);
    return ESP_OK;
}