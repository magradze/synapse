#!/usr/bin/env python3
"""
Synapse Framework Module Generator
ავტომატურად ქმნის ახალ მოდულს ყველა საჭირო ფაილით
"""

import os
import sys
import json
import argparse
from pathlib import Path

def create_module_structure(module_name, module_type, description, author="Giorgi Magradze"):
    """
    ქმნის მოდულის სრულ სტრუქტურას
    """
    # მოდულის ფოლდერის ბილიკი
    base_path = Path(__file__).parent.parent
    module_path = base_path / "components" / "modules" / module_type / module_name
    
    # შევქმნათ დირექტორიები
    (module_path / "src").mkdir(parents=True, exist_ok=True)
    (module_path / "include").mkdir(parents=True, exist_ok=True)
    
    # 1. module.json
    module_json = {
        "name": module_name,
        "version": "1.0.0",
        "description": description,
        "author": author,
        "init_function": f"{module_name}_create",
        "init_level": 10,  # ინიციალიზაციის დონე
        "dependencies": [],  # აქ შეგიძლიათ დაამატოთ დამოკიდებულებები
        "type": module_type,
        "build_enabled": True,
        "conditional_config": f"CONFIG_MODULE_{module_name.upper()}_ENABLED",
        "runtime_commands": {
            "start": f"{module_name}_start",
            "stop": f"{module_name}_disable",
            "enable": f"{module_name}_enable",
            "disable": f"{module_name}_disable",
            "reconfigure": f"{module_name}_reconfigure",
            "status": f"{module_name}_get_status",
            "handle_event": f"{module_name}_handle_event"
        }
    }
    
    with open(module_path / "module.json", "w", encoding="utf-8") as f:
        json.dump(module_json, f, indent=4, ensure_ascii=False)
    
    # 2. CMakeLists.txt
    cmake_content = f'''# {module_name.title()} Module CMake Configuration
# Component for {description}
# Author: {author}
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება placeholder-ით
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით

# შევამოწმოთ არსებობს თუ არა CONFIG ცვლადი (configure ეტაპის შემდეგ)
if(DEFINED CONFIG_MODULE_{module_name.upper()}_ENABLED)
    # CONFIG ცვლადი არსებობს - ვამოწმებთ მნიშვნელობას
    if(CONFIG_MODULE_{module_name.upper()}_ENABLED)
        # მოდული ჩართულია - სრული რეგისტრაცია
        message(STATUS "{module_name.title()} Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
        idf_component_register(
            SRCS "src/{module_name}.c"
            INCLUDE_DIRS "include"
            REQUIRES 
                core 
                interfaces
            PRIV_REQUIRES
                json
        )
    else()
        # მოდული გამორთულია - ცარიელი placeholder
        message(STATUS "{module_name.title()} Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
        
        # შევქმნათ ცარიელი source ფაილი რათა CMake არ ჩავარდეს
        set(EMPTY_SOURCE_CONTENT "// {module_name} module disabled by Kconfig\\n// This is an empty placeholder to prevent CMake errors\\n")
        file(WRITE "${{CMAKE_CURRENT_BINARY_DIR}}/empty_placeholder.c" "${{EMPTY_SOURCE_CONTENT}}")
        
        idf_component_register(
            SRCS "${{CMAKE_CURRENT_BINARY_DIR}}/empty_placeholder.c"
            INCLUDE_DIRS "include"  # Header-ები მაინც საჭიროა factory-სთვის
            REQUIRES core interfaces
        )
    endif()
else()
    # CONFIG ცვლადი ჯერ არ არსებობს (configure ეტაპი) - default რეგისტრაცია
    message(STATUS "{module_name.title()} Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    idf_component_register(
        SRCS "src/{module_name}.c"
        INCLUDE_DIRS "include"
        REQUIRES 
            core 
            interfaces
        PRIV_REQUIRES
            json
    )
endif()
'''
    
    with open(module_path / "CMakeLists.txt", "w", encoding="utf-8") as f:
        f.write(cmake_content)
    
    # 3. Kconfig
    kconfig_content = f'''menu "{module_name.title()} Module Configuration"

    config MODULE_{module_name.upper()}_ENABLED
        bool "Enable {module_name.title()} Module"
        default y
        help
            Enables the {module_name} module for {description}.
            When enabled, the module provides full functionality.
            If disabled, the module code will not be included in 
            the final firmware, saving memory and processing resources.

    config {module_name.upper()}_DEFAULT_INSTANCE_NAME
        string "Default Instance Name"
        default "main_{module_name}"
        depends on MODULE_{module_name.upper()}_ENABLED
        help
            Default instance name for the {module_name} module.

    config {module_name.upper()}_INSTANCE_NAME_MAX_LEN
        int "Maximum Instance Name Length"
        default 32
        range 8 64
        depends on MODULE_{module_name.upper()}_ENABLED
        help
            Maximum length for the {module_name} module instance name string.

endmenu
'''
    
    with open(module_path / "Kconfig", "w", encoding="utf-8") as f:
        f.write(kconfig_content)
    
    # 4. Header file
    header_content = f'''/**
 * @file {module_name}.h
 * @brief {description}
 * @author {author}
 * @version 1.0.0
 * @details {module_name.title()} მოდული რომელიც აღზევს {description} ფუნქციონალურობას.
 *          მოდული მუშაობს Synapse Framework-ის base_module interface-ის საფუძველზე.
 */

#ifndef {module_name.upper()}_H
#define {module_name.upper()}_H

#include "base_module.h"
#include "cJSON.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {{
#endif

/**
 * @brief {module_name.title()} მოდულის კონფიგურაციის სტრუქტურა
 * @details შეიცავს მოდულის ყველა კონფიგურაციის პარამეტრს
 */
typedef struct {{
    char instance_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];  /**< მოდულის ინსტანციის სახელი */
    bool auto_start;                                        /**< ავტომატური გაშვება */
    // TODO: დაამატეთ მოდულის სპეციფიკური კონფიგურაციის ფილდები
}} {module_name}_config_t;

/**
 * @brief ქმნის ახალ {module_name} მოდულის ინსტანციას
 * 
 * @param[in] config მოდულის კონფიგურაცია (JSON ობიექტი) ან NULL default-ებისთვის
 * @return მაჩვენებელი შექმნილ მოდულზე ან NULL შეცდომისას
 * 
 * @note შექმნილი მოდული უნდა დარეგისტრირდეს system_manager-ში
 * @warning მეხსიერება უნდა გათავისუფლდეს module->base.destroy() ფუნქციით
 */
module_t *{module_name}_create(const cJSON *config);

#ifdef __cplusplus
}}
#endif

#endif // {module_name.upper()}_H
'''
    
    with open(module_path / "include" / f"{module_name}.h", "w", encoding="utf-8") as f:
        f.write(header_content)
    
    # 5. Source file
    source_content = f'''/**
 * @file {module_name}.c
 * @brief {description}
 * @author {author}
 * @version 1.0.0
 * @details {module_name.title()} მოდულის იმპლემენტაცია Synapse Framework-ისთვის.
 *          აღზევს ყველა საჭირო base_module interface ფუნქციას.
 */

#include "{module_name}.h"
#include "base_module.h"
#include "event_bus.h"
#include "event_data_wrapper.h"
#include "framework_config.h"
#include "logging.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("{module_name.upper()}");

/**
 * @brief {module_name.title()} მოდულის private მონაცემების სტრუქტურა
 * @details შეიცავს მოდულის შიდა მდგომარეობას და კონფიგურაციას
 */
typedef struct {{
    bool enabled;                                           /**< ჩართვის ფლაგი */
    char instance_name[CONFIG_{module_name.upper()}_INSTANCE_NAME_MAX_LEN]; /**< მოდულის ინსტანციის სახელი */
    // TODO: დაამატეთ მოდულის სპეციფიკური ველები
}} {module_name}_private_data_t;

// --- Forward declarations ---
static esp_err_t {module_name}_init(module_t *self);
static esp_err_t {module_name}_start(module_t *self);
static esp_err_t {module_name}_enable(module_t *self);
static esp_err_t {module_name}_disable(module_t *self);
static void {module_name}_deinit(module_t *self);
static esp_err_t {module_name}_reconfigure(module_t *self, const cJSON *new_config);
static module_status_t {module_name}_get_status(module_t *self);
static void {module_name}_handle_event(module_t *self, const char *event_name, void *event_data);

module_t *{module_name}_create(const cJSON *config)
{{
    ESP_LOGI(TAG, "Creating {module_name} module instance");
    
    // გამოვყოთ მეხსიერება module_t სტრუქტურისთვის
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module) {{
        ESP_LOGE(TAG, "Failed to allocate memory for module");
        return NULL;
    }}
    
    // გამოვყოთ მეხსიერება private data-სთვის
    {module_name}_private_data_t *private_data = ({module_name}_private_data_t *)calloc(1, sizeof({module_name}_private_data_t));
    if (!private_data) {{
        ESP_LOGE(TAG, "Failed to allocate memory for private data");
        free(module);
        return NULL;
    }}
    
    // შევქმნათ state mutex
    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex) {{
        ESP_LOGE(TAG, "Failed to create state mutex");
        free(private_data);
        free(module);
        return NULL;
    }}
    
    // დავაკავშიროთ private data
    module->private_data = private_data;
    
    // დავაყენოთ default კონფიგურაცია
    const char *instance_name = CONFIG_{module_name.upper()}_DEFAULT_INSTANCE_NAME;
    if (config) {{
        const cJSON *config_node = cJSON_GetObjectItem(config, "config");
        if (cJSON_IsObject(config_node)) {{
            const cJSON *name_node = cJSON_GetObjectItem(config_node, "instance_name");
            if (cJSON_IsString(name_node) && name_node->valuestring) {{
                instance_name = name_node->valuestring;
            }}
        }}
        module->current_config = cJSON_Duplicate(config, true);
    }}
    
    // ინიციალიზაცია
    private_data->enabled = true;
    strncpy(private_data->instance_name, instance_name, CONFIG_{module_name.upper()}_INSTANCE_NAME_MAX_LEN - 1);
    snprintf(module->name, sizeof(module->name), "%s", instance_name);
    module->status = MODULE_STATUS_UNINITIALIZED;
    
    // დავაყენოთ ფუნქციების pointers
    module->init_level = 100;  // TODO:შეცვალე საჭირო ლეველზე
    module->base.init = {module_name}_init;
    module->base.start = {module_name}_start;
    module->base.handle_event = {module_name}_handle_event;
    module->base.deinit = {module_name}_deinit;
    module->base.enable = {module_name}_enable;
    module->base.disable = {module_name}_disable;
    module->base.reconfigure = {module_name}_reconfigure;
    module->base.get_status = {module_name}_get_status;
    
    ESP_LOGI(TAG, "{module_name.title()} module created: '%s'", instance_name);
    return module;
}}

static esp_err_t {module_name}_init(module_t *self)
{{
    if (!self) {{
        return ESP_ERR_INVALID_ARG;
    }}
    
    {module_name}_private_data_t *private_data = ({module_name}_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing {module_name} module: %s", self->name);
    
    // TODO: დაამატეთ ინიციალიზაციის ლოგიკა
    // მაგალითად, Event Bus-ზე გამოწერა:
    // esp_err_t ret = fmw_event_bus_subscribe("some_event", self);
    
    self->status = MODULE_STATUS_INITIALIZED;
    ESP_LOGI(TAG, "{module_name.title()} module initialized successfully");
    return ESP_OK;
}}

static esp_err_t {module_name}_start(module_t *self)
{{
    if (!self) {{
        return ESP_ERR_INVALID_ARG;
    }}
    
    {module_name}_private_data_t *private_data = ({module_name}_private_data_t *)self->private_data;
    
    if (self->status != MODULE_STATUS_INITIALIZED) {{
        ESP_LOGE(TAG, "Cannot start uninitialized module");
        return ESP_ERR_INVALID_STATE;
    }}
    
    if (self->status == MODULE_STATUS_RUNNING) {{
        ESP_LOGW(TAG, "Module already running");
        return ESP_OK;
    }}
    
    ESP_LOGI(TAG, "Starting {module_name} module: %s", self->name);
    
    // TODO: Implement module start logic
    
    self->status = MODULE_STATUS_RUNNING;
    private_data->enabled = true;
    
    ESP_LOGI(TAG, "{module_name.title()} module started successfully");
    return ESP_OK;
}}

static esp_err_t {module_name}_enable(module_t *self)
{{
    if (!self) {{
        return ESP_ERR_INVALID_ARG;
    }}
    
    {module_name}_private_data_t *private_data = ({module_name}_private_data_t *)self->private_data;
    
    ESP_LOGI(TAG, "Enabling {module_name} module: %s", self->name);
    
    if (private_data->enabled) {{
        ESP_LOGW(TAG, "Module already enabled");
        return ESP_OK;
    }}
    
    // TODO: Implement enable logic
    
    private_data->enabled = true;
    self->status = MODULE_STATUS_RUNNING;
    ESP_LOGI(TAG, "{module_name.title()} module enabled");
    
    return ESP_OK;
}}

static esp_err_t {module_name}_disable(module_t *self)
{{
    if (!self) {{
        return ESP_ERR_INVALID_ARG;
    }}
    
    {module_name}_private_data_t *private_data = ({module_name}_private_data_t *)self->private_data;
    
    ESP_LOGI(TAG, "Disabling {module_name} module: %s", self->name);
    
    if (!private_data->enabled) {{
        ESP_LOGW(TAG, "Module already disabled");
        return ESP_OK;
    }}
    
    // TODO: Implement disable logic
    
    private_data->enabled = false;
    self->status = MODULE_STATUS_DISABLED;
    ESP_LOGI(TAG, "{module_name.title()} module disabled");
    
    return ESP_OK;
}}

static esp_err_t {module_name}_reconfigure(module_t *self, const cJSON *new_config)
{{
    if (!self || !new_config) {{
        return ESP_ERR_INVALID_ARG;
    }}
    
    ESP_LOGI(TAG, "Reconfiguring {module_name} module: %s", self->name);
    
    // TODO: Implement reconfiguration logic
    // Update self->current_config with new_config
    
    if (self->current_config) {{
        cJSON_Delete(self->current_config);
    }}
    self->current_config = cJSON_Duplicate(new_config, true);
    
    ESP_LOGI(TAG, "{module_name.title()} module reconfigured");
    return ESP_OK;
}}

static module_status_t {module_name}_get_status(module_t *self)
{{
    if (!self) {{
        return MODULE_STATUS_ERROR;
    }}
    
    return self->status;
}}

static void {module_name}_handle_event(module_t *self, const char *event_name, void *event_data)
{{
    if (!self || !self->private_data) {{
        if (event_data) {{
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        }}
        return;
    }}
    
    {module_name}_private_data_t *private_data = ({module_name}_private_data_t *)self->private_data;
    
    if (!private_data->enabled) {{
        if (event_data) {{
            fmw_event_data_release((event_data_wrapper_t *)event_data);
        }}
        return;
    }}
    
    if (event_name) {{
        ESP_LOGD(TAG, "[%s] Event received: '%s'", private_data->instance_name, event_name);
        
        // TODO: Handle specific events
        // მაგალითად:
        // if (strcmp(event_name, "some_event") == 0) {{
        //     // Handle the event
        // }}
    }}
    
    // Always release event data
    if (event_data) {{
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }}
}}

static void {module_name}_deinit(module_t *self)
{{
    if (!self) {{
        return;
    }}
    
    ESP_LOGI(TAG, "Deinitializing %s module", self->name);
    
    // TODO: Unsubscribe from events if needed
    // fmw_event_bus_unsubscribe("some_event", self);
    
    if (self->private_data) {{
        free(self->private_data);
    }}
    
    if (self->current_config) {{
        cJSON_Delete(self->current_config);
    }}
    
    if (self->state_mutex) {{
        vSemaphoreDelete(self->state_mutex);
    }}
    
    free(self);
    
    ESP_LOGI(TAG, "Module deinitialized successfully");
}}
'''
    
    with open(module_path / "src" / f"{module_name}.c", "w", encoding="utf-8") as f:
        f.write(source_content)
    
    print(f"✅ მოდული '{module_name}' წარმატებით შეიქმნა:")
    print(f"   📁 Path: {module_path}")
    print(f"   📄 Files: module.json, CMakeLists.txt, Kconfig, {module_name}.h, {module_name}.c")
    print(f"   🔧 Config: CONFIG_MODULE_{module_name.upper()}_ENABLED")
    print("\n📋 შემდეგი ნაბიჯები:")
    print("   1. idf.py reconfigure  # სისტემა აღმოაჩენს ახალ მოდულს")
    print("   2. idf.py menuconfig   # კონფიგურაცია Kconfig-ში")
    print("   3. idf.py build        # აგება ახალი მოდულით")

def main():
    parser = argparse.ArgumentParser(description="Create new Synapse Framework module")
    parser.add_argument("module_name", help="Module name (e.g., wifi_manager)")
    parser.add_argument("--type", default="utilities", help="Module type (utilities, sensors, displays, etc.)")
    parser.add_argument("--description", help="Module description")
    parser.add_argument("--author", default="Synapse Framework Team", help="Author name")
    
    args = parser.parse_args()
    
    if not args.description:
        args.description = f"{args.module_name.replace('_', ' ').title()} functionality"
    
    create_module_structure(args.module_name, args.type, args.description, args.author)

if __name__ == "__main__":
    main()