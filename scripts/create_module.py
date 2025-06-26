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
        "build_enabled": True,
        "conditional_config": f"CONFIG_MODULE_{module_name.upper()}_ENABLED"
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
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "{module_name.upper()}";

/**
 * @brief {module_name.title()} მოდულის private მონაცემების სტრუქტურა
 * @details შეიცავს მოდულის შიდა მდგომარეობას და კონფიგურაციას
 */
typedef struct {{
    module_t base;                          /**< საბაზისო მოდულის სტრუქტურა */
    {module_name}_config_t config;          /**< მოდულის კონფიგურაცია */
    bool is_initialized;                    /**< ინიციალიზაციის ფლაგი */
    bool is_enabled;                        /**< ჩართვის ფლაგი */
}} {module_name}_module_t;

// --- Forward declarations ---
static esp_err_t {module_name}_init(module_t *self);
static esp_err_t {module_name}_start(module_t *self);
static esp_err_t {module_name}_enable(module_t *self);
static esp_err_t {module_name}_disable(module_t *self);
static esp_err_t {module_name}_reconfigure(module_t *self, const cJSON *new_config);
static module_status_t {module_name}_get_status(module_t *self);
static void {module_name}_handle_event(module_t *self, core_framework_event_id_t event_id, void *event_data);

module_t *{module_name}_create(const cJSON *config)
{{
    ESP_LOGI(TAG, "Creating {module_name} module instance");
    
    {module_name}_module_t *module = calloc(1, sizeof({module_name}_module_t));
    if (!module) {{
        ESP_LOGE(TAG, "Failed to allocate memory for {module_name} module");
        return NULL;
    }}
    
    // Initialize base module structure
    module->base.status = MODULE_STATUS_UNINITIALIZED;
    module->base.current_config = NULL;
    module->base.private_data = module;
    
    // Set up function pointers
    module->base.base.init = {module_name}_init;
    module->base.base.start = {module_name}_start;
    module->base.base.enable = {module_name}_enable;
    module->base.base.disable = {module_name}_disable;
    module->base.base.reconfigure = {module_name}_reconfigure;
    module->base.base.get_status = {module_name}_get_status;
    module->base.base.handle_event = {module_name}_handle_event;
    
    // Set default configuration
    strncpy(module->base.name, 
#ifdef CONFIG_{module_name.upper()}_DEFAULT_INSTANCE_NAME
            CONFIG_{module_name.upper()}_DEFAULT_INSTANCE_NAME,
#else
            "main_{module_name}",
#endif
            sizeof(module->base.name) - 1);
    
    strncpy(module->config.instance_name, module->base.name, sizeof(module->config.instance_name) - 1);
    module->config.auto_start = true;
    
    ESP_LOGI(TAG, "{module_name.title()} module created successfully with name: %s", module->base.name);
    return (module_t *)module;
}}

static esp_err_t {module_name}_init(module_t *self)
{{
    if (!self) {{
        return ESP_ERR_INVALID_ARG;
    }}
    
    {module_name}_module_t *{module_name}_mod = ({module_name}_module_t *)self->private_data;
    ESP_LOGI(TAG, "Initializing {module_name} module: %s", self->name);
    
    if ({module_name}_mod->is_initialized) {{
        ESP_LOGW(TAG, "Module already initialized");
        return ESP_ERR_INVALID_STATE;
    }}
    
    // Parse configuration if provided
    if (self->current_config) {{
        const cJSON *instance_name = cJSON_GetObjectItem(self->current_config, "instance_name");
        if (cJSON_IsString(instance_name)) {{
            strncpy({module_name}_mod->config.instance_name, 
                   instance_name->valuestring, 
                   sizeof({module_name}_mod->config.instance_name) - 1);
            strncpy(self->name, instance_name->valuestring, sizeof(self->name) - 1);
        }}
        
        const cJSON *auto_start = cJSON_GetObjectItem(self->current_config, "auto_start");
        if (cJSON_IsBool(auto_start)) {{
            {module_name}_mod->config.auto_start = cJSON_IsTrue(auto_start);
        }}
        
        // TODO: Parse other configuration parameters
    }}
    
    // TODO: Implement initialization logic here
    
    {module_name}_mod->is_initialized = true;
    self->status = MODULE_STATUS_INITIALIZED;
    
    ESP_LOGI(TAG, "{module_name.title()} module initialized successfully");
    return ESP_OK;
}}

static esp_err_t {module_name}_start(module_t *self)
{{
    if (!self) {{
        return ESP_ERR_INVALID_ARG;
    }}
    
    {module_name}_module_t *{module_name}_mod = ({module_name}_module_t *)self->private_data;
    
    if (!{module_name}_mod->is_initialized) {{
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
    {module_name}_mod->is_enabled = true;
    
    ESP_LOGI(TAG, "{module_name.title()} module started successfully");
    return ESP_OK;
}}

static esp_err_t {module_name}_enable(module_t *self)
{{
    if (!self) {{
        return ESP_ERR_INVALID_ARG;
    }}
    
    {module_name}_module_t *{module_name}_mod = ({module_name}_module_t *)self->private_data;
    
    ESP_LOGI(TAG, "Enabling {module_name} module: %s", self->name);
    
    if ({module_name}_mod->is_enabled) {{
        ESP_LOGW(TAG, "Module already enabled");
        return ESP_OK;
    }}
    
    // TODO: Implement enable logic
    
    {module_name}_mod->is_enabled = true;
    ESP_LOGI(TAG, "{module_name.title()} module enabled");
    
    return ESP_OK;
}}

static esp_err_t {module_name}_disable(module_t *self)
{{
    if (!self) {{
        return ESP_ERR_INVALID_ARG;
    }}
    
    {module_name}_module_t *{module_name}_mod = ({module_name}_module_t *)self->private_data;
    
    ESP_LOGI(TAG, "Disabling {module_name} module: %s", self->name);
    
    if (!{module_name}_mod->is_enabled) {{
        ESP_LOGW(TAG, "Module already disabled");
        return ESP_OK;
    }}
    
    // TODO: Implement disable logic
    
    {module_name}_mod->is_enabled = false;
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

static void {module_name}_handle_event(module_t *self, core_framework_event_id_t event_id, void *event_data)
{{
    if (!self) {{
        return;
    }}
    
    {module_name}_module_t *{module_name}_mod = ({module_name}_module_t *)self->private_data;
    
    ESP_LOGD(TAG, "Module %s handling event: %d", self->name, event_id);
    
    // TODO: Implement event handling logic based on event_id
    switch (event_id) {{
        case FRAMEWORK_EVENT_SYSTEM_INIT:
            ESP_LOGD(TAG, "System initialization event received");
            break;
        case FRAMEWORK_EVENT_SYSTEM_START:
            ESP_LOGD(TAG, "System start event received");
            break;
        default:
            ESP_LOGD(TAG, "Unhandled event: %d", event_id);
            break;
    }}
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
