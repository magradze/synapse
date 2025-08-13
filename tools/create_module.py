#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Synapse Framework Module Generator v3.1
=======================================

An interactive, archetype-based tool that scaffolds a complete, standards-compliant
module for the Synapse ESP Framework. This script automates the creation of all
necessary files, including modular C sources, headers, build scripts, and
configuration templates, ensuring perfect alignment with the framework's
rigorous architectural conventions.

Author: Giorgi Magradze
Version: 3.1.0
"""

import os
import sys
import json
import argparse
import subprocess
from pathlib import Path
from typing import List, Dict, Optional

# --- Configuration ---

CATEGORIES = {
    "actuators": "Actuators (e.g., relays, motors)",
    "communications": "Communication protocols (e.g., MQTT, WiFi)",
    "diagnostics": "System health and diagnostics",
    "displays": "Display drivers and UI components",
    "drivers": "Low-level hardware drivers (e.g., I2C, SPI, GPIO expanders)",
    "provisioning": "Device provisioning methods",
    "security": "Security-related modules",
    "sensors": "Sensor reading and data processing",
    "storage": "Storage and filesystem management",
    "system": "Core system services (e.g., timers, watchdogs)",
    "testing": "Modules for testing and simulation",
    "utilities": "General-purpose utility modules"
}

ARCHETYPES = {
    "basic": "Basic (a minimal, empty module structure)",
    "event_producer": "Event Producer (e.g., a sensor that periodically emits data)",
    "service_provider": "Service Provider (e.g., a driver that offers a public API)",
    "command_handler": "Command Handler (registers and processes CLI commands)",
    "event_service_provider": "Event Producer & Service Provider (Hybrid)",
    "event_command_handler": "Event Producer & Command Handler (Hybrid)",
    "service_command_handler": "Service Provider & Command Handler (Hybrid)"
}

# --- Helper Functions for Interactive Mode ---

def get_git_user_name() -> str:
    """Retrieves the user's name from the global Git configuration."""
    try:
        result = subprocess.run(['git', 'config', 'user.name'], capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except (subprocess.CalledProcessError, FileNotFoundError):
        return "Giorgi Magradze"

def prompt_for_input(prompt_text: str, default: Optional[str] = None) -> str:
    """Prompts the user for text input with an optional default value."""
    prompt_suffix = f" (default: {default})" if default else ""
    full_prompt = f"? {prompt_text}{prompt_suffix}: "
    user_input = input(full_prompt).strip()
    return user_input if user_input else default

def prompt_for_choice(prompt_text: str, choices: Dict[str, str]) -> str:
    """Presents a list of choices to the user and returns the selected key."""
    print(f"? {prompt_text}:")
    choice_keys = list(choices.keys())
    for i, key in enumerate(choice_keys):
        print(f"  [{i}] {choices[key]}")
    
    while True:
        try:
            choice_index_str = input("> ").strip()
            if not choice_index_str:
                continue
            choice_index = int(choice_index_str)
            if 0 <= choice_index < len(choice_keys):
                return choice_keys[choice_index]
            else:
                print("! Invalid choice. Please try again.")
        except ValueError:
            print("! Please enter a number.")

# --- Template Generator Classes ---

class TemplateGenerator:
    """Base class for template generation, holding shared parameters."""
    def __init__(self, params: Dict):
        self.params = params
        self.module_name = params['module_name']
        self.module_upper = self.module_name.upper()
        self.module_title = params['module_title']
        self.author = params['author']
        self.description = params['description']
        self.deps = params.get('deps', [])
        self.creation_date = params.get('creation_date', 'YYYY-MM-DD')

    def generate(self) -> Dict[str, str]:
        """Generates content for all required module files."""
        # This method is intended to be overridden by the ArchetypeGenerator.
        return {}

    def _generate_module_json(self) -> str:
        module_json = {
            "name": self.module_name,
            "version": "1.0.0",
            "description": self.description,
            "author": self.author,
            "init_function": f"{self.module_name}_create",
            "init_level": self.params['init_level'],
            "type": self.params['category'],
            "build_enabled": True,
            "conditional_config": f"CONFIG_MODULE_{self.module_upper}_ENABLED",
            "mqtt_interface": {"publishes": {}, "subscribes": {}},
        }
        return json.dumps(module_json, indent=4, ensure_ascii=False)

    def _generate_config_json(self) -> str:
        config_data = [{
            "type": self.module_name,
            "enabled": True,
            "config": {
                "instance_name": f"main_{self.module_name}"
            }
        }]
        return json.dumps(config_data, indent=4, ensure_ascii=False)

    def _generate_kconfig(self) -> str:
        return f"""menu "{self.module_title}"

    config MODULE_{self.module_upper}_ENABLED
        bool "Enable {self.module_title} Module"
        default y
        help
            Enables the {self.module_name} module for {self.description}.
            When enabled, the module provides full functionality.
            If disabled, the module code will not be included in 
            the final firmware, saving memory and processing resources.

    config {self.module_upper}_DEFAULT_INSTANCE_NAME
        string "Default Instance Name"
        default "main_{self.module_name}"
        depends on MODULE_{self.module_upper}_ENABLED
        help
            Default instance name for the {self.module_name} module.

    config {self.module_upper}_INSTANCE_NAME_MAX_LEN
        int "Maximum Instance Name Length"
        default 32
        range 8 64
        depends on MODULE_{self.module_upper}_ENABLED
        help
            Maximum length for the {self.module_name} module instance name string.

endmenu
"""


class ArchetypeGenerator(TemplateGenerator):
    """Generates code based on selected archetypes and the modular source structure."""
    
    def __init__(self, params: Dict):
        super().__init__(params)
        archetype = self.params['archetype']
        self.is_event_producer = "event" in archetype
        self.is_service_provider = "service" in archetype
        self.is_command_handler = "command" in archetype
        self.with_ui = self.params.get('with_ui', False)

    def generate(self) -> Dict[str, str]:
        """Generates a dictionary of filenames and their content based on the archetype."""
        files = {
            "module.json": self._generate_module_json(),
            "config.json": self._generate_config_json(),
            "schema.json": self._generate_schema_json(),
            "Kconfig": self._generate_kconfig(),
            "README.md": self._generate_readme(),
            f"include/{self.module_name}.h": self._generate_public_header(),
            f"src/{self.module_name}_internal.h": self._generate_internal_header(),
            f"src/{self.module_name}.c": self._generate_main_source(),
        }
        
        if self.is_service_provider:
            files[f"src/{self.module_name}_api.c"] = self._generate_api_source()
        if self.is_command_handler:
            files[f"src/{self.module_name}_cmd.c"] = self._generate_cmd_source()
        if self.is_event_producer or self.is_command_handler:
            files[f"src/{self.module_name}_events.c"] = self._generate_events_source()
        if self.with_ui:
            files[f"src/{self.module_name}_ui.c"] = self._generate_ui_source()
            
        files["CMakeLists.txt"] = self._generate_cmake(list(files.keys()))
        return files

    def _generate_cmake(self, all_files: List[str]) -> str:
        # --- NEW: Dynamically find source files from the generated file list ---
        src_files = [f'"{path}"' for path in all_files if path.startswith("src/") and path.endswith(".c")]
        src_list = "\n                ".join(src_files)
        # --------------------------------------------------------------------

        # Prepare dependencies list
        requires_list = "core\n                interfaces"
        if self.deps:
            # Assuming deps are public interfaces for simplicity
            deps_formatted = "\n                ".join(self.deps)
            requires_list += f"\n                {deps_formatted}"

        return f"""# {self.module_title} Module CMake Configuration
# Component for {self.description}.
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. If the CONFIG variable does not exist yet (configure stage), register a placeholder.
# 2. If the CONFIG variable exists and is enabled, register with full functionality.
# 3. If the CONFIG variable exists and is disabled, register an empty placeholder.

if(DEFINED CONFIG_MODULE_{self.module_upper}_ENABLED)
    # CONFIG variable exists - check its value
    if(CONFIG_MODULE_{self.module_upper}_ENABLED)
        # Module is ENABLED - full registration
        message(STATUS "{self.module_title} Module: ENABLED - Compiling with full functionality")
        idf_component_register(
            SRCS
                {src_list}
            INCLUDE_DIRS "include" "src"
            REQUIRES 
                {requires_list}
            PRIV_REQUIRES
                json
        )
    else()
        # Module is DISABLED - register an empty placeholder
        message(STATUS "{self.module_title} Module: DISABLED - Compiling an empty placeholder")
        
        # Create an empty source file to prevent CMake from failing
        set(EMPTY_SOURCE_CONTENT "// {self.module_name} module disabled by Kconfig\\n// This is an empty placeholder to prevent CMake errors.\\n")
        file(WRITE "${{CMAKE_CURRENT_BINARY_DIR}}/empty_{self.module_name}.c" "${{EMPTY_SOURCE_CONTENT}}")
        
        idf_component_register(
            SRCS "${{CMAKE_CURRENT_BINARY_DIR}}/empty_{self.module_name}.c"
            INCLUDE_DIRS "include"  # Header is still needed for the factory
            REQUIRES core interfaces
        )
    endif()
else()
    # CONFIG variable does not exist yet (configure stage) - temporary registration
    message(STATUS "{self.module_title} Module: CONFIGURE STAGE - Registering temporarily")
    idf_component_register(
        SRCS "src/{self.module_name}.c" # Register at least one file to satisfy CMake
        INCLUDE_DIRS "include" "src"
        REQUIRES 
            core 
            interfaces
        PRIV_REQUIRES
            json
    )
endif()
"""

    def _generate_readme(self) -> str:
        readme = f"""# 🔌 Module: `{self.module_name}`

## 1. 📜 Overview
{self.description}

This module is generated based on the **{self.params['archetype']}** archetype.

## 2. ⚙️ Configuration (`config.json`)
| Parameter | Type | Description | Required |
|:---|:---|:---|:---:|
| `instance_name` | string | The unique name for this module instance. | ✅ |
| `...` | `...` | TODO: Add other configuration parameters here. | |

"""
        if self.is_service_provider:
            readme += "\n## 3. 🔌 Service API\n- TODO: Describe the Service API provided by this module. You will need to create a corresponding `_interface.h` file in `components/interfaces/include`.\n"
        if self.is_event_producer:
            readme += "\n## 4. 📢 Published Events\n- TODO: Describe the events published by this module.\n"
        if self.is_command_handler:
            readme += "\n## 5. ⌨️ CLI Commands\n- TODO: Describe the CLI commands registered by this module.\n"
        return readme

    def _generate_public_header(self) -> str:
        return f"""/**
 * @file {self.module_name}.h
 * @brief Public header for the {self.module_title}.
 * @author {self.author}
 * @version 1.0.0
 */
#ifndef {self.module_upper}_H
#define {self.module_upper}_H

#include "synapse.h"

#ifdef __cplusplus
extern "C" {{
#endif

/**
 * @brief Factory function to create a new instance of the {self.module_name} module.
 * @param[in] config The cJSON configuration object for this instance.
 * @return A pointer to the new module_t instance, or NULL on failure.
 */
module_t* {self.module_name}_create(const cJSON *config);

#ifdef __cplusplus
}}
#endif

#endif // {self.module_upper}_H
"""

    def _generate_internal_header(self) -> str:
        # Start with the basic header content
        internal_header = f"""/**
 * @file {self.module_name}_internal.h
 * @brief Internal declarations for the {self.module_title}.
 * @author {self.author}
 * @version 1.0.0
 * @date {self.creation_date}
 */
#ifndef {self.module_upper}_INTERNAL_H
#define {self.module_upper}_INTERNAL_H

#include "{self.module_name}.h"
#include "synapse.h"
"""
        # Conditionally include the service interface header
        if self.is_service_provider:
            internal_header += f'#include "{self.module_name}_interface.h" // IMPORTANT: Create this file in components/interfaces/include\n'
        
        # Conditionally include the UI interface header
        if self.with_ui:
            internal_header += '#include "ui_interface.h"\n'

        # Start the private data struct definition
        internal_header += f"""
// --- Private Data Structure ---
typedef struct {{
    char instance_name[CONFIG_SYNAPSE_MODULE_NAME_MAX_LENGTH];
"""
        # Add fields based on archetype and UI flag
        if self.is_event_producer:
            internal_header += "    TaskHandle_t task_handle; // Handle for the main processing task\n"
        if self.is_service_provider:
            internal_header += f"    {self.module_name}_api_t service_api; // Instance of the service API table\n"
        if self.with_ui:
            internal_header += "    ui_manager_api_t* ui_api; // Cached handle to the UI Manager service\n"
        
        internal_header += f"""    // TODO: Add other private data fields here.
}} {self.module_name}_private_data_t;

// --- Forward Declarations for functions shared across .c files ---
"""
        # Add forward declarations based on archetype and UI flag
        if self.is_event_producer:
            internal_header += f"void {self.module_name}_task(void *pvParameters);\n"
        if self.is_command_handler:
            internal_header += "void register_cli_commands(module_t *self);\n"
        if self.is_event_producer or self.is_command_handler:
            internal_header += f"void {self.module_name}_handle_event(module_t *self, const char *event_name, void *event_data);\n"
        
        if self.with_ui:
            internal_header += f"""
// --- UI Lifecycle Hooks ---
void {self.module_name}_ui_init(module_t* self);
void {self.module_name}_ui_deinit(module_t* self);
"""

        internal_header += f"\n#endif // {self.module_upper}_INTERNAL_H\n"
        return internal_header

    def _generate_main_source(self) -> str:
        handle_event_assignment = ""
        if self.is_event_producer or self.is_command_handler:
            handle_event_assignment = f"    module->base.handle_event = {self.module_name}_handle_event;"

        ui_hooks_assignment = ""
        if self.with_ui:
            ui_hooks_assignment = f"""    module->base.ui_init = {self.module_name}_ui_init;
    module->base.ui_deinit = {self.module_name}_ui_deinit;"""

        start_task_logic = ""
        if self.is_event_producer:
            start_task_logic = f"""
    // Example of starting a dedicated task.
    // For simple periodic jobs, consider using the Task Pool Manager service instead.
    BaseType_t ret = xTaskCreate({self.module_name}_task, self->name, 4096, self, 5, &private_data->task_handle);
    SYNAPSE_GUARD(ret == pdPASS, TAG, ESP_FAIL, "Failed to create task for {self.module_name}.");
"""

        deinit_task_logic = ""
        if self.is_event_producer:
            deinit_task_logic += "    if (private_data->task_handle) vTaskDelete(private_data->task_handle);\n"
        if self.is_service_provider:
            deinit_task_logic += "    synapse_service_unregister(self->name);\n"

        task_implementation = ""
        if self.is_event_producer:
            task_implementation = f"""
// --- Main Processing Task ---
void {self.module_name}_task(void *pvParameters)
{{
    module_t *self = (module_t *)pvParameters;
    // {self.module_name}_private_data_t *private_data = ({self.module_name}_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Task for '%s' started.", self->name);

    while (1) {{
        // TODO: Implement your main task logic here (e.g., sensor reading).
        // Example of posting an event:
        // synapse_event_bus_post("MY_CUSTOM_EVENT", NULL);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }}
}}"""

        service_registration_logic = ""
        if self.is_service_provider:
            service_registration_logic = f"""
    // TODO: Initialize your service API function pointers here.
    // private_data->service_api.my_function = my_api_function_impl;

    // Register the service in the create phase
    esp_err_t ret = synapse_service_register_with_status(
        module->name,
        SYNAPSE_SERVICE_TYPE_CUSTOM_API, // IMPORTANT: Replace with the correct service type enum
        &private_data->service_api,
        SERVICE_STATUS_REGISTERED
    );

    if (ret != ESP_OK) {{
        ESP_LOGE(TAG, "Failed to register service for '%s' (%s).", module->name, esp_err_to_name(ret));
        {self.module_name}_deinit(module); // Use the generated deinit for cleanup
        return NULL;
    }}
"""
        return f"""/**
 * @file {self.module_name}.c
 * @brief Implements the lifecycle and core logic for the {self.module_title}.
 * @author {self.author}
 * @version 1.0.0
 * @date {self.creation_date}
 *
 * @details
 * This module is responsible for {self.description}.
 * It follows the standard Synapse module architecture, including:
 *  - Creation via the Module Factory.
 *  - Lifecycle management (init, start, deinit) by the System Manager.
 *  - Interaction with other modules via the Service Locator and Event Bus.
 *
 * @note This file contains the core lifecycle functions. Service API, CLI, and
 *       event handling logic are separated into respective `_api.c`,
 *       `_cmd.c`, and `_events.c` files.
 */
#include "{self.module_name}_internal.h"

DEFINE_COMPONENT_TAG("{self.module_upper}", SYNAPSE_LOG_COLOR_YELLOW); // Default color

// --- Forward Declarations for static functions in this file ---
static esp_err_t {self.module_name}_init(module_t *self);
static esp_err_t {self.module_name}_start(module_t *self);
static void {self.module_name}_deinit(module_t *self);

// --- Factory Function ---
module_t* {self.module_name}_create(const cJSON *config)
{{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    {self.module_name}_private_data_t *private_data = ({self.module_name}_private_data_t *)calloc(1, sizeof({self.module_name}_private_data_t));
    SYNAPSE_GUARD(module && private_data, TAG, NULL, "Failed to allocate memory for {self.module_name} module");

    module->private_data = private_data;
    module->current_config = cJSON_Duplicate(config, true);
    SYNAPSE_GUARD(module->current_config, TAG, NULL, "Failed to duplicate configuration object.");
    
    module->init_level = {self.params['init_level']};

    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    // TODO: Implement robust parsing using synapse_utils here.
    // Example: synapse_config_get_string_from_node(TAG, config_node, "instance_name", module->name, sizeof(module->name));
    const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
    snprintf(module->name, sizeof(module->name), "%s", name_node->valuestring);
    snprintf(private_data->instance_name, sizeof(private_data->instance_name), "%s", name_node->valuestring);

    module->base.init = {self.module_name}_init;
    module->base.start = {self.module_name}_start;
    module->base.deinit = {self.module_name}_deinit;
{handle_event_assignment}
{ui_hooks_assignment}
{service_registration_logic}
    ESP_LOGI(TAG, "{self.module_title} ('%s') created.", module->name);
    return module;
}}

// --- Lifecycle Functions ---
static esp_err_t {self.module_name}_init(module_t *self)
{{
    ESP_LOGI(TAG, "Initializing '%s'.", self->name);
    // TODO: Add resource allocation (e.g., mutexes, queues) and
    // event subscriptions (synapse_event_bus_subscribe) here.
    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}}

static esp_err_t {self.module_name}_start(module_t *self)
{{
    ESP_LOGI(TAG, "Starting '%s'.", self->name);
    {self.module_name}_private_data_t *private_data = ({self.module_name}_private_data_t *)self->private_data;
{start_task_logic}
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}}

static void {self.module_name}_deinit(module_t *self)
{{
    if (!self) return;
    ESP_LOGI(TAG, "Deinitializing '%s'.", self->name);
    {self.module_name}_private_data_t *private_data = ({self.module_name}_private_data_t *)self->private_data;
{deinit_task_logic}
    
    // TODO: Unsubscribe from events here.

    if (self->current_config) cJSON_Delete(self->current_config);
    free(self->private_data);
    self->private_data = NULL;
}}
{task_implementation}
"""

    def _generate_api_source(self) -> str:
        return f"""/**
 * @file {self.module_name}_api.c
 * @brief Implements the public Service API for the {self.module_title}.
 * @author {self.author}
 */
#include "{self.module_name}_internal.h"

DEFINE_COMPONENT_TAG("{self.module_upper}_API", SYNAPSE_LOG_COLOR_YELLOW); // Default color

// TODO: Implement your service API functions here.
// These functions will be assigned to the service_api struct in the main .c file.
//
// Example:
// esp_err_t {self.module_name}_api_do_something(void* context, int param) {{
//     module_t* self = (module_t*)context;
//     {self.module_name}_private_data_t* private_data = ({self.module_name}_private_data_t*)self->private_data;
//     // ... implementation ...
//     return ESP_OK;
// }}
"""

    def _generate_cmd_source(self) -> str:
        return f"""/**
 * @file {self.module_name}_cmd.c
 * @brief Implements the CLI command handlers for the {self.module_title}.
 * @author {self.author}
 */
#include "{self.module_name}_internal.h"
#include "cmd_router_interface.h"

DEFINE_COMPONENT_TAG("{self.module_upper}_CMD", SYNAPSE_LOG_COLOR_YELLOW); // Default color

static esp_err_t {self.module_name}_cmd_handler(int argc, char **argv, void *context)
{{
    module_t *self = (module_t *)context;
    ESP_LOGI(TAG, "Command handler executed for module '%s'.", self->name);
    // TODO: Implement your command logic here.
    printf("Hello from {self.module_name} command!\\n");
    return ESP_OK;
}}

void register_cli_commands(module_t *self)
{{
    static cmd_t cmd_def; // Use a static definition
    cmd_def = (cmd_t){{
        .command = "{self.module_name}",
        .help = "Control the {self.module_title}",
        .usage = "{self.module_name} <subcommand>",
        .min_args = 1,
        .max_args = 2,
        .handler = {self.module_name}_cmd_handler,
        .context = self
    }};
    
    service_handle_t cmd_router = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_CMD_ROUTER_API);
    if (cmd_router) {{
        cmd_router_api_t* cmd_api = (cmd_router_api_t*)cmd_router;
        // Prevent re-registering the same command from multiple instances
        if (!cmd_api->is_command_registered("{self.module_name}")) {{
            cmd_api->register_command(&cmd_def);
            ESP_LOGI(TAG, "Command '{self.module_name}' registered by instance '%s'.", self->name);
        }}
    }} else {{
        ESP_LOGW(TAG, "Command Router service not found, cannot register CLI commands.");
    }}
}}
"""

    def _generate_events_source(self) -> str:
        return f"""/**
 * @file {self.module_name}_events.c
 * @brief Implements the event handlers for the {self.module_title}.
 * @author {self.author}
 */
#include "{self.module_name}_internal.h"

DEFINE_COMPONENT_TAG("{self.module_upper}_EVENTS", SYNAPSE_LOG_COLOR_YELLOW); // Default color

void {self.module_name}_handle_event(module_t *self, const char *event_name, void *event_data)
{{
    ESP_LOGD(TAG, "Instance '%s' received event: '%s'", self->name, event_name);
    
    {'if (strcmp(event_name, SYNAPSE_EVENT_SYSTEM_START_COMPLETE) == 0) {{' if self.is_command_handler else ''}
    {'    register_cli_commands(self);' if self.is_command_handler else ''}
    {'}}' if self.is_command_handler else ''}

    // TODO: Add your specific event handling logic here.

    if (event_data) {{
        synapse_event_data_release((event_data_wrapper_t *)event_data);
    }}
}}
"""

    def _generate_ui_source(self) -> str:
        return f"""/**
 * @file {self.module_name}_ui.c
 * @brief Implements the UI components for the {self.module_title}.
 * @author {self.author}
 */
#include "{self.module_name}_internal.h"
#include "ui_interface.h"

DEFINE_COMPONENT_TAG("{self.module_upper}_UI", SYNAPSE_LOG_COLOR_YELLOW); // Default color

// --- Forward Declarations for Callbacks ---
static void {self.module_name}_render_screen_cb(module_t* self, ui_context_t* context);
static void {self.module_name}_event_cb(module_t* self, ui_event_t* event);

// --- UI Component Definition ---
static const ui_component_t s_{self.module_name}_screen = {{
    .id = "{self.module_name}_screen",
    .type = UI_COMP_SCREEN,
    .menu_text = "{self.module_title}",
    .render_cb = {self.module_name}_render_screen_cb,
    .event_cb = {self.module_name}_event_cb,
}};

static const ui_component_t* s_{self.module_name}_ui_components[] = {{ &s_{self.module_name}_screen, NULL }};

// --- UI Lifecycle Functions ---
void {self.module_name}_ui_init(module_t* self)
{{
    {self.module_name}_private_data_t* private_data = ({self.module_name}_private_data_t*)self->private_data;
    
    private_data->ui_api = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_UI_MANAGER_API);
    
    if (private_data->ui_api) {{
        private_data->ui_api->register_components(self, s_test_module_ui_components);
    }} else {{
        ESP_LOGE(TAG, "UI Manager service not found for module '%s'", self->name);
    }}
}}

void {self.module_name}_ui_deinit(module_t* self)
{{
    ui_manager_api_t* ui_api = synapse_service_lookup_by_type(SYNAPSE_SERVICE_TYPE_UI_MANAGER_API);
    if (ui_api) {{
        ui_api->unregister_components(self);
    }}
}}

// --- Callback Implementations ---
static void {self.module_name}_render_screen_cb(module_t* self, ui_context_t* context)
{{
    {self.module_name}_private_data_t* private_data = ({self.module_name}_private_data_t*)self->private_data;
    const display_driver_api_t* display = context->display->api;
    void* disp_ctx = context->display->context;

    display->draw_formatted_text(disp_ctx, 2, 16, 1, "Welcome to {self.module_title}!");
    
    if (private_data->ui_api) {{
        // On this simple screen, BACK is always the selected action
        private_data->ui_api->draw_footer_button(context, "BACK", true);
    }}
}}

static void {self.module_name}_event_cb(module_t* self, ui_event_t* event)
{{
    ESP_LOGI(TAG, "Event received in {self.module_title}: %s", event->button_name);
    
    // TODO: Implement your event handling logic here.

    if (strcmp(event->button_name, "BACK") == 0 || strcmp(event->button_name, "OK") == 0) {{
        synapse_event_bus_post("UI_NAVIGATE_BACK", NULL);
    }}
}}
"""

    def _generate_schema_json(self) -> str:
        """Generates a boilerplate schema.json for the module's config."""
        schema = {
            "$schema": "http://json-schema.org/draft-07/schema#",
            "title": f"{self.module_title} Config Schema",
            "description": f"Defines the structure for a {self.module_name} config.json entry.",
            "type": "object",
            "properties": {
                "type": {"const": self.module_name},
                "enabled": {"type": "boolean"},
                "config": {
                    "type": "object",
                    "properties": {
                        "instance_name": {"type": "string", "minLength": 1}
                        # TODO: Add schema definitions for your module's specific config parameters here.
                        # Example:
                        # "pin": { "type": "integer", "minimum": 0 },
                        # "update_interval": { "type": "number", "minimum": 1 }
                    },
                    "required": ["instance_name"],
                    "additionalProperties": False
                }
            },
            "required": ["type", "config"]
        }
        return json.dumps(schema, indent=4, ensure_ascii=False)
# --- Main Execution Logic ---

def create_module_files(params: Dict):
    """Creates the module directory and file structure."""
    base_path = Path(__file__).resolve().parent.parent
    module_path = base_path / "components" / "modules" / params['category'] / params['module_name']
    
    if module_path.exists():
        print(f"! Error: Directory '{module_path}' already exists.")
        sys.exit(1)

    (module_path / "src").mkdir(parents=True)
    (module_path / "include").mkdir(parents=True)

    generator = ArchetypeGenerator(params)
    files_content = generator.generate()

    for file_path_str, content in files_content.items():
        full_path = module_path / file_path_str
        with open(full_path, "w", encoding="utf-8") as f:
            f.write(content)
    
    print("\n" + "="*50)
    print(f"✅ Module '{params['module_title']}' created successfully!")
    print(f"   - Category: {params['category']}")
    print(f"   - Archetype: {ARCHETYPES[params['archetype']]}")
    print(f"   - Location: {module_path}")
    print("="*50)
    print("\n📋 Next Steps:")
    print("   1. Review and customize the generated `config.json` file.")
    print("   2. If you chose a Service Provider, create the `_interface.h` file in `components/interfaces/include`.")
    print("   3. Run `idf.py menuconfig` to ensure the module is enabled.")
    print("   4. Run `idf.py build` to compile your new module.")
    print("   5. Fill in the `TODO` comments in the generated source code.")

def main():
    """Main entry point for the script."""
    parser = argparse.ArgumentParser(
        description="Synapse Framework Module Generator v3.1",
        formatter_class=argparse.RawTextHelpFormatter
    )
    parser.add_argument("module_title", nargs='?', default=None, help="The full, human-readable name of the module (e.g., 'My Awesome Module').\nIf omitted, interactive mode will be launched.")
    parser.add_argument("-c", "--category", choices=CATEGORIES.keys(), help="The module's category.")
    parser.add_argument("-a", "--archetype", choices=ARCHETYPES.keys(), help="The module's archetype (code template type).")
    parser.add_argument("-d", "--description", help="A short description of the module.")
    parser.add_argument("--author", help="The author of the module.")
    parser.add_argument("--init_level", type=int, help="The initialization level (priority).")
    parser.add_argument("--deps", help="Comma-separated list of public ESP-IDF/component dependencies (e.g., 'esp_http_client').")
    parser.add_argument("--with-ui", action='store_true', help="Generate a UI component stub (_ui.c) for this module.")
    
    args = parser.parse_args()
    params = {}

    if args.module_title:
        if not all([args.category, args.archetype, args.description]):
            print("! Error: In command-line mode, --category, --archetype, and --description are required.")
            sys.exit(1)
        params['module_title'] = args.module_title
        params['category'] = args.category
        params['archetype'] = args.archetype
        params['with_ui'] = args.with_ui
        params['description'] = args.description
        params['author'] = args.author or get_git_user_name()
        params['init_level'] = args.init_level or 60
        params['deps'] = [dep.strip() for dep in args.deps.split(',')] if args.deps else []
    else:
        print("--- Synapse Module Generator (Interactive Mode) ---")
        while not (module_title := prompt_for_input("Enter the full module name (e.g., 'My Awesome Module')")):
            print("! Module name is required.")
        params['module_title'] = module_title
        params['category'] = prompt_for_choice("Select a category", CATEGORIES)
        params['archetype'] = prompt_for_choice("Select a module archetype", ARCHETYPES)
        ui_input = prompt_for_input("Does this module need a UI component? (y/N)", "n").lower()
        params['with_ui'] = (ui_input == 'y')
        params['description'] = prompt_for_input("Enter a short description", f"A module for {params['module_title']}")
        params['author'] = prompt_for_input("Enter the author", get_git_user_name())
        params['init_level'] = int(prompt_for_input("Enter the initialization level", "60"))
        deps_input = prompt_for_input("Enter public dependencies (comma-separated, e.g., esp_http_client)", "")
        params['deps'] = [dep.strip() for dep in deps_input.split(',')] if deps_input else []

    params['module_name'] = params['module_title'].lower().replace(' ', '_').replace('-', '_')
    
    from datetime import datetime
    params['creation_date'] = datetime.now().strftime("%Y-%m-%d")
    
    create_module_files(params)

if __name__ == "__main__":
    main()