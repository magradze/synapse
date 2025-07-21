#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Synapse Framework Module Generator v2.1
=======================================

ავტომატურად ქმნის ახალ მოდულს Synapse Framework-ისთვის, სრული,
არქეტიპებზე დაფუძნებული კოდის გენერაციით და ინტერაქტიული ინტერფეისით.

ავტორი: Giorgi Magradze & Synapse AI Assistant
ვერსია: 2.1.0
"""

import os
import sys
import json
import argparse
import subprocess
from pathlib import Path
from typing import List, Dict, Optional

# --- კონფიგურაცია ---

# ხელმისაწვდომი კატეგორიები და მათი აღწერები
CATEGORIES = {
    "actuators": "actuators",
    "communications": "communications",
    "diagnostics": "diagnostics",
    "displays": "displays",
    "drivers": "drivers",
    "provisioning": "provisioning",
    "security": "security",
    "sensors": "sensors",
    "storage": "storage",
    "system": "system",
    "testing": "testing",
    "utilities": "utilities"
}

# ხელმისაწვდომი არქეტიპები და მათი აღწერები
ARCHETYPES = {
    "basic": "Basic (მინიმალური შაბლონი)",
    "event_producer": "Event Producer (აგზავნის ივენთებს, მაგ. სენსორი)",
    "service_provider": "Service Provider (აწვდის API-ს, მაგ. დრაივერი)",
    "command_handler": "Command Handler (ამუშავებს CLI ბრძანებებს)",
    "event_service_provider": "Event Producer & Service Provider (ჰიბრიდული)",
    "event_command_handler": "Event Producer & Command Handler (ჰიბრიდული)",
    "service_command_handler": "Service Provider & Command Handler (ჰიბრიდული)"
}

# --- დამხმარე ფუნქციები ინტერაქტიული რეჟიმისთვის ---

def get_git_user_name() -> str:
    """იღებს მომხმარებლის სახელს Git-ის კონფიგურაციიდან."""
    try:
        result = subprocess.run(['git', 'config', 'user.name'], capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except (subprocess.CalledProcessError, FileNotFoundError):
        return "Synapse Framework Team"

def prompt_for_input(prompt_text: str, default: Optional[str] = None) -> str:
    """სთხოვს მომხმარებელს მონაცემის შეყვანას."""
    prompt_suffix = f" (default: {default})" if default else ""
    full_prompt = f"? {prompt_text}{prompt_suffix}: "
    user_input = input(full_prompt).strip()
    return user_input if user_input else default

def prompt_for_choice(prompt_text: str, choices: Dict[str, str]) -> str:
    """სთხოვს მომხმარებელს არჩევანის გაკეთებას სიიდან."""
    print(f"? {prompt_text}:")
    choice_keys = list(choices.keys())
    for i, key in enumerate(choice_keys):
        print(f"  [{i}] {choices[key]}")
    
    while True:
        try:
            choice_index = int(input("> "))
            if 0 <= choice_index < len(choice_keys):
                return choice_keys[choice_index]
            else:
                print("! არასწორი არჩევანი. გთხოვთ, სცადოთ თავიდან.")
        except ValueError:
            print("! გთხოვთ, შეიყვანოთ რიცხვი.")

# --- შაბლონების გენერატორი კლასები ---

class TemplateGenerator:
    """საბაზისო კლასი შაბლონების გენერაციისთვის."""
    def __init__(self, params: Dict):
        self.params = params

    def generate(self) -> Dict[str, str]:
        """აგენერირებს ფაილების კონტენტს."""
        return {
            "module.json": self._generate_module_json(),
            "config.json": self._generate_config_json(), # <--- ახალი ხაზი
            "CMakeLists.txt": self._generate_cmake(),
            "Kconfig": self._generate_kconfig(),
            "README.md": self._generate_readme(),
            f"include/{self.params['module_name']}.h": self._generate_header(),
            f"src/{self.params['module_name']}.c": self._generate_source(),
        }

    def _generate_module_json(self) -> str:
        module_json = {
            "name": self.params['module_name'],
            "version": "1.0.0",
            "description": self.params['description'],
            "author": self.params['author'],
            "init_function": f"{self.params['module_name']}_create",
            "init_level": self.params['init_level'],
            "type": self.params['category'],
            "build_enabled": True,
            "conditional_config": f"CONFIG_MODULE_{self.params['module_name'].upper()}_ENABLED",
            "mqtt_interface": {
                "publishes": {},
                "subscribes": {}
            },
        }
        return json.dumps(module_json, indent=4, ensure_ascii=False)

    def _generate_cmake(self) -> str:
        module_name = self.params['module_name']
        module_title = self.params['module_title']
        deps = self.params['deps']
        
        # დამოკიდებულებების ფორმირება
        deps_str = "\n".join([f"                {dep}" for dep in deps]) if deps else ""
        requires_section = f"""            REQUIRES 
                core 
                interfaces
{deps_str}""" if deps_str else """            REQUIRES 
                core 
                interfaces"""

        return f"""# {module_title} Module CMake Configuration
# Component for {self.params['description']}
# Author: {self.params['author']}
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_{module_name.upper()}_ENABLED OR CONFIG_MODULE_{module_name.upper()}_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_{module_name.upper()}_ENABLED)
        message(STATUS "{module_title} Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "{module_title} Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/{module_name}.c"
        INCLUDE_DIRS "include"
{requires_section}
        PRIV_REQUIRES
            json
    )
else()
    # მოდული გამორთულია
    message(STATUS "{module_title} Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// {module_name} module disabled by Kconfig\\n// This is an empty placeholder to prevent CMake errors.\\n")
    file(WRITE "${{CMAKE_CURRENT_BINARY_DIR}}/empty_{module_name}.c" "${{EMPTY_SOURCE_CONTENT}}")
    
    idf_component_register(
        SRCS "${{CMAKE_CURRENT_BINARY_DIR}}/empty_{module_name}.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
"""

    def _generate_kconfig(self) -> str:
        module_name = self.params['module_name']
        module_title = self.params['module_title']
        description = self.params['description']

        return f"""menu "{module_title}"

    config MODULE_{module_name.upper()}_ENABLED
        bool "Enable {module_title} Module"
        default y
        help
            Enables the {module_name} module for {description}.

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
"""
    def _generate_readme(self) -> str:
        # This will be overridden by archetypes
        return f"""# 🔌 მოდული: `{self.params['module_name']}`

## 1. 📜 მიმოხილვა
{self.params['description']}
"""

    def _generate_header(self) -> str:
        # This will be overridden by archetypes
        return f"""/**
 * @file {self.params['module_name']}.h
 * @brief {self.params['description']}
 * @author {self.params['author']}
 */
#ifndef {self.params['module_name'].upper()}_H
#define {self.params['module_name'].upper()}_H

#include "base_module.h"
#include "cJSON.h"

module_t *{self.params['module_name']}_create(const cJSON *config);

#endif // {self.params['module_name'].upper()}_H
"""

    def _generate_source(self) -> str:
        # This will be overridden by archetypes
        return f"""/**
 * @file {self.params['module_name']}.c
 * @brief {self.params['description']}
 * @author {self.params['author']}
 */
#include "{self.params['module_name']}.h"
#include "logging.h"

DEFINE_COMPONENT_TAG("{self.params['module_name'].upper()}");

// ... Basic implementation ...
"""

    def _generate_config_json(self) -> str:
        """Generates a default config.json template."""
        config_data = [
            {
                "type": self.params['module_name'],
                "enabled": True,
                "config": {
                    "instance_name": f"main_{self.params['module_name']}"
                    # TODO: Add module-specific default configuration parameters here.
                    # Example:
                    # "gpio_pin": 23,
                    # "update_interval_sec": 30
                }
            }
        ]
        return json.dumps(config_data, indent=4, ensure_ascii=False)


class ArchetypeGenerator(TemplateGenerator):
    """კლასი, რომელიც აგენერირებს კოდს არქეტიპების მიხედვით."""
    
    def __init__(self, params: Dict):
        super().__init__(params)
        self.is_event_producer = "event" in self.params['archetype']
        self.is_service_provider = "service" in self.params['archetype']
        self.is_command_handler = "command" in self.params['archetype']

    def _generate_readme(self) -> str:
        readme = f"""# 🔌 მოდული: `{self.params['module_name']}`

## 1. 📜 მიმოხილვა
{self.params['description']}

## 2. ⚙️ კონფიგურაცია
| პარამეტრი | ტიპი | აღწერა |
|:---|:---|:---|
| `instance_name` | string | მოდულის უნიკალური სახელი. |
"""
        if self.is_service_provider:
            readme += "\n## 3. 🔌 Service API\n- TODO: აღწერეთ თქვენი სერვისის API აქ.\n"
        if self.is_event_producer:
            readme += "\n## 4. 📢 ივენთები\n- TODO: აღწერეთ თქვენს მიერ გამოქვეყნებული ივენთები.\n"
        if self.is_command_handler:
            readme += "\n## 5. ⌨️ CLI ბრძანებები\n- TODO: აღწერეთ თქვენი CLI ბრძანებები.\n"
        return readme

    def _generate_header(self) -> str:
        header = f"""/**
 * @file {self.params['module_name']}.h
 * @brief {self.params['description']}
 * @author {self.params['author']}
 */
#ifndef {self.params['module_name'].upper()}_H
#define {self.params['module_name'].upper()}_H

#include "base_module.h"
#include "cJSON.h"
"""
        if self.is_service_provider:
            header += f"""
/**
 * @brief {self.params['module_title']} მოდულის საჯარო Service API.
 */
typedef struct {{
    // TODO: დაამატეთ თქვენი API ფუნქციების მაჩვენებლები.
    // მაგალითად: esp_err_t (*do_something)(int param);
    void* reserved; // ცარიელი სტრუქტურების თავიდან ასაცილებლად
}} {self.params['module_name']}_api_t;
"""
        header += f"""
/**
 * @brief ქმნის ახალ {self.params['module_name']} მოდულის ინსტანციას.
 */
module_t *{self.params['module_name']}_create(const cJSON *config);

#endif // {self.params['module_name'].upper()}_H
"""
        return header

    def _generate_source(self) -> str:
        module_name = self.params['module_name']
        module_title = self.params['module_title']
        init_level = self.params['init_level']
        
        includes = [
            f'#include "{module_name}.h"',
            '#include "logging.h"',
            '#include "event_bus.h"',
            '#include "event_data_wrapper.h"',
        ]
        if self.is_service_provider:
            includes.append('#include "service_locator.h"')
            includes.append('#include "service_types.h"')
        if self.is_command_handler:
            includes.append('#include "cmd_router_interface.h"')
            includes.append('#include "service_locator.h"') # Command Router-ის მისაღებად
        if self.is_event_producer:
            includes.append('#include "freertos/task.h"')
            includes.append('#include "system_timer_interface.h"') # ტაიმერის სერვისისთვის
            includes.append('#include "service_locator.h"') # ტაიმერის სერვისის მისაღებად

        source = f"""/**
 * @file {module_name}.c
 * @brief {self.params['description']}
 * @author {self.params['author']}
 */
{os.linesep.join(sorted(list(set(includes))))}
#include <string.h>
#include <stdlib.h>

DEFINE_COMPONENT_TAG("{module_name.upper()}");

typedef struct {{
    // TODO: დაამატეთ თქვენი მოდულის მდგომარეობისთვის საჭირო ველები.
    char instance_name[CONFIG_FMW_MODULE_NAME_MAX_LENGTH];
"""
        if self.is_event_producer:
            source += "    TaskHandle_t task_handle;\n"
        source += f"}} {module_name}_private_data_t;\n\n"

        # --- Service Provider-ის ლოგიკა ---
        if self.is_service_provider:
            source += f"""// --- Service Provider Globals & API Implementation ---
/**
 * @internal
 * @brief გლობალური მაჩვენებელი მოდულის ინსტანციაზე.
 * @details აუცილებელია, რადგან Service API ფუნქციები არ იღებენ module_t* პარამეტრს.
 *          ეს მაჩვენებელი ინიციალიზდება init() ფუნქციაში.
 */
static module_t *global_{module_name}_instance = NULL;
...
"""
        # --- Command Handler-ის ლოგიკა ---
        if self.is_command_handler:
            source += f"""
// --- Command Handler Implementation ---

/**
 * @internal
 * @brief {module_name} მოდულის ბრძანებების აღმწერი სტრუქტურა.
 */
static cmd_t {module_name}_command_definition;

/**
 * @internal
 * @brief {module_name} მოდულის ბრძანებების დამმუშავებელი (handler) ფუნქცია.
 */
static esp_err_t {module_name}_cmd_handler(int argc, char **argv, void *context) {{
    module_t *self = (module_t *)context;
    ESP_LOGI(TAG, "Command handler for '{module_name}' called.");
    
    // TODO: დაამატეთ ბრძანების დამუშავების ლოგიკა.
    if (argc == 2 && strcmp(argv[1], "status") == 0) {{
        printf("Module {module_name} is currently: %d\\n", self->status);
    }} else {{
        printf("Usage: {module_name} status\\n");
    }}
    
    return ESP_OK;
}}
"""
        # --- Event Producer-ის ლოგიკა ---
        if self.is_event_producer:
            source += f"""
// --- Event Producer Task ---
#define {module_name.upper()}_EVENT "EVT_{module_name.upper()}_DATA"

static void {module_name}_task(void *pvParameters) {{
    module_t *self = (module_t *)pvParameters;
    ESP_LOGI(TAG, "Task for {module_name} started.");

    while (1) {{
        // TODO: დაამატეთ თქვენი ლოგიკა (მაგ. სენსორის წაკითხვა)
        
        ESP_LOGI(TAG, "Posting event: %s", {module_name.upper()}_EVENT);
        fmw_event_bus_post({module_name.upper()}_EVENT, NULL);

        vTaskDelay(pdMS_TO_TICKS(15000)); // მაგალითად, ყოველ 15 წამში
    }}
}}
"""
        # --- Forward declarations ---
        source += f"""
// --- Base Module Function Declarations ---
static esp_err_t {module_name}_init(module_t *self);
static esp_err_t {module_name}_start(module_t *self);
static void {module_name}_deinit(module_t *self);
static void {module_name}_handle_event(module_t *self, const char *event_name, void *event_data);

// --- Create Function ---
module_t *{module_name}_create(const cJSON *config) {{
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module) {{ return NULL; }}

    {module_name}_private_data_t *private_data = ({module_name}_private_data_t *)calloc(1, sizeof({module_name}_private_data_t));
    if (!private_data) {{ free(module); return NULL; }}
    
    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex) {{ free(private_data); free(module); return NULL; }}

    module->private_data = private_data;
    module->init_level = {init_level};
    
    // TODO: წაიკითხეთ კონფიგურაცია და შეავსეთ private_data
    // ...

    module->base.init = {module_name}_init;
    module->base.start = {module_name}_start;
    module->base.deinit = {module_name}_deinit;
    module->base.handle_event = {module_name}_handle_event;
    
    ESP_LOGI(TAG, "{module_title} module created.");
    return module;
}}

// --- Base Module Function Implementations ---
static esp_err_t {module_name}_init(module_t *self) {{
    ESP_LOGI(TAG, "Initializing {module_name} module.");
    {module_name}_private_data_t *p_data = ({module_name}_private_data_t *)self->private_data;
"""
        if self.is_service_provider:
            source += f"""
    global_{module_name}_instance = self; // შევინახოთ გლობალური ინსტანცია
    esp_err_t err = fmw_service_register(p_data->instance_name, FMW_SERVICE_TYPE_CUSTOM_API, &s_api);
    if (err != ESP_OK) {{
        ESP_LOGE(TAG, "Failed to register service: %s", esp_err_to_name(err));
        return err;
    }}
"""
        if self.is_command_handler:
            source += """
    // ბრძანების რეგისტრაცია მოხდება FMW_EVENT_SYSTEM_START_COMPLETE-ის შემდეგ,
    // რათა Command Router გარანტირებულად იყოს ხელმისაწვდომი.
    fmw_event_bus_subscribe("FMW_SYSTEM_START_COMPLETE", self);
"""
        source += """
    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}

static esp_err_t {module_name}_start(module_t *self) {{
    ESP_LOGI(TAG, "Starting {module_name} module.");
"""
        if self.is_event_producer:
            source += f"""
    BaseType_t ret = xTaskCreate({module_name}_task, "{module_name}_task", 4096, self, 5, &(({module_name}_private_data_t *)self->private_data)->task_handle);
    if (ret != pdPASS) {{
        ESP_LOGE(TAG, "Failed to create task for {module_name}.");
        return ESP_FAIL;
    }}
"""
        source += """
    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void {module_name}_deinit(module_t *self) {{
    if (!self) return;
    ESP_LOGI(TAG, "Deinitializing {module_name} module.");
    {module_name}_private_data_t *p_data = ({module_name}_private_data_t *)self->private_data;
"""
        if self.is_event_producer:
            source += """
    if (p_data->task_handle) {
        vTaskDelete(p_data->task_handle);
    }
"""
        if self.is_service_provider:
            source += """
    fmw_service_unregister(p_data->instance_name);
"""
        if self.is_command_handler:
            source += """
    fmw_event_bus_unsubscribe("FMW_SYSTEM_START_COMPLETE", self);
    service_handle_t cmd_router = fmw_service_get("main_cmd_router");
    if (cmd_router) {
        ((cmd_router_api_t *)cmd_router)->unregister_command("{module_name}");
    }
"""
        source += """
    if (self->private_data) free(self->private_data);
    if (self->state_mutex) vSemaphoreDelete(self->state_mutex);
    free(self);
}

static void {module_name}_handle_event(module_t *self, const char *event_name, void *event_data) {{
    ESP_LOGD(TAG, "Event received: '%s'", event_name);
"""
        if self.is_command_handler:
            source += f"""
    if (strcmp(event_name, "FMW_SYSTEM_START_COMPLETE") == 0) {{
        service_handle_t cmd_router = fmw_service_get("main_cmd_router");
        if (cmd_router) {{
            // შევავსოთ წინასწარ დეკლარირებული ბრძანების სტრუქტურა
            {module_name}_command_definition = (cmd_t){{
                .command = "{module_name}",
                .help = "Command for {module_title}",
                .usage = "{module_name} status", // საწყისი მაგალითი
                .min_args = 1,
                .max_args = 2,
                .handler = {module_name}_cmd_handler,
                .context = self
            }};
            ((cmd_router_api_t *)cmd_router)->register_command(&{module_name}_command_definition);
            ESP_LOGI(TAG, "Command '{module_name}' registered successfully.");
        }} else {{
            ESP_LOGE(TAG, "Failed to get Command Router service to register command.");
        }}
    }}
"""
        source += """
    if (event_data) {
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }
}
"""
        return source

# --- მთავარი ლოგიკა ---

def create_module_files(params: Dict):
    """ქმნის მოდულის ფაილებს და დირექტორიებს."""
    base_path = Path(__file__).parent.parent
    module_path = base_path / "components" / "modules" / params['category'] / params['module_name']
    
    if module_path.exists():
        print(f"! შეცდომა: დირექტორია '{module_path}' უკვე არსებობს.")
        sys.exit(1)

    (module_path / "src").mkdir(parents=True)
    (module_path / "include").mkdir(parents=True)

    generator = ArchetypeGenerator(params)
    files_content = generator.generate()

    for file_name, content in files_content.items():
        # Correctly handle nested paths for include/ and src/
        if file_name.startswith("include/") or file_name.startswith("src/"):
            full_path = module_path / file_name
        else:
            full_path = module_path / file_name
        
        with open(full_path, "w", encoding="utf-8") as f:
            f.write(content)
    
    print("\n" + "="*50)
    print(f"✅ მოდული '{params['module_title']}' ({params['module_name']}) წარმატებით შეიქმნა!")
    print(f"   - კატეგორია: {params['category']}")
    print(f"   - არქეტიპი: {ARCHETYPES[params['archetype']]}")
    print(f"   - მდებარეობა: {module_path}")
    print("="*50)
    print("\n📋 შემდეგი ნაბიჯები:")
    print("   1. გახსენით და შეავსეთ ახლად შექმნილი `config.json` ფაილი.")
    print("   2. `idf.py menuconfig`  # დარწმუნდით, რომ მოდული ჩართულია.")
    print("   3. `idf.py build`       # ააგეთ პროექტი.")
    print("   4. შეავსეთ `TODO` კომენტარები გენერირებულ კოდში.")


def main():
    """სკრიპტის მთავარი შესრულების წერტილი."""
    parser = argparse.ArgumentParser(
        description="Synapse Framework Module Generator v2.1",
        formatter_class=argparse.RawTextHelpFormatter
    )
    parser.add_argument("module_title", nargs='?', default=None, help="მოდულის სრული სახელი (მაგ: 'OLED Display'). თუ არ მიუთითებთ, გაეშვება ინტერაქტიული რეჟიმი.")
    parser.add_argument("-c", "--category", choices=CATEGORIES.keys(), help="მოდულის კატეგორია.")
    parser.add_argument("-a", "--archetype", choices=ARCHETYPES.keys(), help="მოდულის არქეტიპი (შაბლონის ტიპი).")
    parser.add_argument("-d", "--description", help="მოდულის მოკლე აღწერა.")
    parser.add_argument("--author", help="მოდულის ავტორი.")
    parser.add_argument("--init_level", type=int, help="საწყისი init_level.")
    parser.add_argument("--deps", help="ESP-IDF კომპონენტებზე დამოკიდებულებები (მძიმით გამოყოფილი).")
    
    args = parser.parse_args()

    params = {}

    if args.module_title:
        # ბრძანების ხაზის რეჟიმი
        if not all([args.category, args.archetype, args.description]):
            print("! შეცდომა: ბრძანების ხაზის რეჟიმში აუცილებელია --category, --archetype და --description პარამეტრების მითითება.")
            sys.exit(1)
        params['module_title'] = args.module_title
        params['category'] = args.category
        params['archetype'] = args.archetype
        params['description'] = args.description
        params['author'] = args.author or get_git_user_name()
        params['init_level'] = args.init_level or 60
        params['deps'] = [dep.strip() for dep in args.deps.split(',')] if args.deps else []
    else:
        # ინტერაქტიული რეჟიმი
        print("--- Synapse Module Generator (ინტერაქტიული რეჟიმი) ---")
        while not (module_title := prompt_for_input("შეიყვანეთ მოდულის სრული სახელი (მაგ: 'OLED Display')")):
            print("! მოდულის სახელი სავალდებულოა.")
        params['module_title'] = module_title
        params['category'] = prompt_for_choice("აირჩიეთ კატეგორია", CATEGORIES)
        params['archetype'] = prompt_for_choice("აირჩიეთ მოდულის არქეტიპი", ARCHETYPES)
        params['description'] = prompt_for_input("შეიყვანეთ მოკლე აღწერა", f"A module for {params['module_title']}")
        params['author'] = prompt_for_input("შეიყვანეთ ავტორი", get_git_user_name())
        params['init_level'] = int(prompt_for_input("შეიყვანეთ საწყისი init_level", "60"))
        deps_input = prompt_for_input("შეიყვანეთ ESP-IDF დამოკიდებულებები (მძიმით გამოყოფილი, მაგ: esp_http_client)", "")
        params['deps'] = [dep.strip() for dep in deps_input.split(',')] if deps_input else []

    params['module_name'] = params['module_title'].lower().replace(' ', '_').replace('-', '_')
    
    create_module_files(params)

if __name__ == "__main__":
    main()