#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Synapse Framework - მოდულის გენერატორი
სკრიპტი ავტომატურად ქმნის ახალი მოდულის სტრუქტურას,
ყველა საჭირო ფაილითა და საბაზისო შაბლონით, რომელიც
თავსებადია ფრეიმვორქის განახლებულ არქიტექტურასთან.
"""
import os
import sys
import json
import argparse
import datetime

# ==============================================================================
#  C კოდის შაბლონები (განახლებული ვერსიები)
# ==============================================================================

# ⭐️ HEADER_TEMPLATE განახლებულია: ამოღებულია config_t, გამარტივებულია
HEADER_TEMPLATE = """/**
 * @file {module_name}.h
 * @brief {description}-ის Public API.
 * @author {author}
 * @version 1.0.0
 * @date {date}
 */

#ifndef {module_name_upper}_H
#define {module_name_upper}_H

#include "base_module.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {{
#endif

/*
// --- სერვისის API-ს შაბლონი (გააქტიურეთ საჭიროების შემთხვევაში) ---
typedef struct {{
    // esp_err_t (*some_function)(void);
}} {module_name}_api_t;
*/

/**
 * @brief ქმნის ახალ {module_name} მოდულის ინსტანციას.
 *
 * @param[in] config მოდულის კონფიგურაცია (JSON ობიექტი) `system_config.json`-დან.
 * @return მაჩვენებელი შექმნილ მოდულზე ან NULL შეცდომისას.
 */
module_t *{module_name}_create(const cJSON *config);

#ifdef __cplusplus
}}
#endif

#endif // {module_name_upper}_H
"""

# ⭐️ SOURCE_TEMPLATE სრულად გადაკეთებულია ახალ არქიტექტურაზე
SOURCE_TEMPLATE = """/**
 * @file {module_name}.c
 * @brief {description}-ის იმპლემენტაცია.
 * @author {author}
 * @version 1.0.0
 * @date {date}
 */

#include "{module_name}.h"
#include "base_module.h"
#include "event_bus.h"
#include "logging.h"
#include "event_data_wrapper.h"

#include "esp_log.h"
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

DEFINE_COMPONENT_TAG("{tag}");

// ==============================================================================
//  პრივატული მონაცემების სტრუქტურა
// ==============================================================================
typedef struct {{
    // TODO: დაამატეთ თქვენი მოდულის მდგომარეობისთვის საჭირო ველები.
    // მაგ: int pin_number; TaskHandle_t task_handle;
}} {module_name}_private_data_t;

// ==============================================================================
//  პრივატული ფუნქციების დეკლარაციები
// ==============================================================================
static esp_err_t {module_name}_init(module_t* self);
static esp_err_t {module_name}_start(module_t* self);
static void {module_name}_deinit(module_t* self);
static void {module_name}_handle_event(module_t* self, const char* event_name, void* event_data);

// ==============================================================================
//  Public API - მოდულის შემქმნელი ფუნქცია
// ==============================================================================
module_t* {module_name}_create(const cJSON *config) {{
    ESP_LOGI(TAG, "მოდულის '{module_name}' ინსტანციის შექმნა...");

    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    if (!module) {{
        ESP_LOGE(TAG, "მოდულისთვის მეხსიერების გამოყოფა ვერ მოხერხდა.");
        return NULL;
    }}

    {module_name}_private_data_t *private_data = ({module_name}_private_data_t *)calloc(1, sizeof({module_name}_private_data_t));
    if (!private_data) {{
        ESP_LOGE(TAG, "პრივატული მონაცემებისთვის მეხსიერების გამოყოფა ვერ მოხერხდა.");
        free(module);
        return NULL;
    }}

    module->state_mutex = xSemaphoreCreateMutex();
    if (!module->state_mutex) {{
        ESP_LOGE(TAG, "State mutex-ის შექმნა ვერ მოხერხდა.");
        free(private_data);
        free(module);
        return NULL;
    }}
    
    module->private_data = private_data;
    module->status = MODULE_STATUS_UNINITIALIZED;

    // TODO: წაიკითხეთ და დაამუშავეთ კონფიგურაცია `config` cJSON ობიექტიდან
    // და შეავსეთ `private_data` სტრუქტურა. `strncpy` გამოიყენეთ სახელისთვის.
    // მაგალითად:
    // const cJSON *name_node = cJSON_GetObjectItem(cJSON_GetObjectItem(config, "config"), "instance_name");
    // if (name_node) {{
    //     strncpy(module->name, name_node->valuestring, sizeof(module->name) - 1);
    // }}

    if (config) {{
        module->current_config = cJSON_Duplicate(config, true);
    }}

    // ფუნქციის მაჩვენებლების მიბმა (ახალი, "ბრტყელი" სტრუქტურა)
    module->base.init = {module_name}_init;
    module->base.start = {module_name}_start;
    module->base.deinit = {module_name}_deinit;
    module->base.handle_event = {module_name}_handle_event;
    // TODO: მიაბით სხვა base ფუნქციები, თუ იყენებთ (მაგ. enable/disable)
    module->base.enable = NULL; 
    module->base.disable = NULL;
    module->base.reconfigure = NULL;
    module->base.get_status = NULL;

    ESP_LOGI(TAG, "მოდული '{module_name}' წარმატებით შეიქმნა.");
    return module;
}}

// ==============================================================================
//  პრივატული ფუნქციების იმპლემენტაცია
// ==============================================================================
static esp_err_t {module_name}_init(module_t* self) {{
    if (!self || !self->private_data) return ESP_ERR_INVALID_ARG;
    ESP_LOGI(TAG, "'%s' მოდულის ინიციალიზაცია...", self->name);

    // TODO: დაამატეთ ინიციალიზაციის ლოგიკა.
    // მაგ: fmw_event_bus_subscribe("SOME_EVENT", self);

    self->status = MODULE_STATUS_INITIALIZED;
    return ESP_OK;
}}

static esp_err_t {module_name}_start(module_t* self) {{
    if (!self || !self->private_data) return ESP_ERR_INVALID_ARG;
    ESP_LOGI(TAG, "'%s' მოდულის გაშვება...", self->name);
    
    // TODO: დაამატეთ გაშვების ლოგიკა.

    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}}

static void {module_name}_deinit(module_t* self) {{
    if (!self) return;
    ESP_LOGI(TAG, "'%s' მოდულის რესურსების გათავისუფლება...", self->name);
    
    // TODO: გააუქმეთ ყველა ივენთის გამოწერა.
    // fmw_event_bus_unsubscribe("SOME_EVENT", self);

    if (self->private_data) {{
        free(self->private_data);
        self->private_data = NULL;
    }}
    if (self->current_config) {{
        cJSON_Delete(self->current_config);
        self->current_config = NULL;
    }}
    if (self->state_mutex) {{
        vSemaphoreDelete(self->state_mutex);
        self->state_mutex = NULL;
    }}
    free(self);
}}

static void {module_name}_handle_event(module_t* self, const char* event_name, void* event_data) {{
    if (!self || !event_name) {{
        if(event_data) fmw_event_data_release((event_data_wrapper_t *)event_data);
        return;
    }}
    ESP_LOGD(TAG, "[%s] მიიღო ივენთი: %s", self->name, event_name);
    
    // TODO: დაამატეთ ივენთების დამუშავების ლოგიკა.
    // if (strcmp(event_name, "SOME_EVENT") == 0) {{ ... }}

    if (event_data) {{
        fmw_event_data_release((event_data_wrapper_t *)event_data);
    }}
}}
"""

# ==============================================================================
#  Python სკრიპტის ძირითადი ლოგიკა (თქვენი ვერსიიდან, მცირე შესწორებებით)
# ==============================================================================

def create_file_with_content(path, content):
    """ქმნის ფაილს მითითებულ გზაზე და წერს მასში კონტენტს."""
    try:
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"  ✅ შეიქმნა: {path}")
    except IOError as e:
        print(f"  ❌ ვერ შეიქმნა ფაილი {path}: {e}")
        sys.exit(1)

def get_git_user_name():
    """აბრუნებს Git-ის მომხმარებლის სახელს."""
    try:
        name = os.popen('git config --get user.name').read().strip()
        return name if name else "Synapse Developer"
    except Exception:
        return "Synapse Developer"

def generate_module(args):
    """მოდულის გენერაციის მთავარი ფუნქცია."""
    module_name = args.name.lower().replace(" ", "_")
    human_name = args.name.title()
    category = args.category.lower()
    description = args.description
    author = args.author if args.author else get_git_user_name()
    date = datetime.datetime.now().strftime("%Y-%m-%d")

    base_dir = Path("components") / "modules" / category / module_name
    src_dir = base_dir / "src"
    include_dir = base_dir / "include"

    if base_dir.exists():
        print(f"❌ შეცდომა: მოდული '{module_name}' უკვე არსებობს მისამართზე: {base_dir}")
        return

    print(f"🚀 ვიწყებ '{human_name}' მოდულის გენერაციას...")
    print(f"   - კატეგორია: {category}")
    print(f"   - მდებარეობა: {base_dir}")

    # 1. module.json
    module_json_content = {
        "name": module_name,
        "version": "1.0.0",
        "description": description,
        "author": author,
        "init_function": f"{module_name}_create"
    }
    create_file_with_content(base_dir / "module.json", json.dumps(module_json_content, indent=4, ensure_ascii=False))

    # 2. CMakeLists.txt
    cmake_content = f"""# {human_name} Module CMake Configuration
if(CONFIG_MODULE_{module_name.upper()}_ENABLED)
    idf_component_register(
        SRCS "src/{module_name}.c"
        INCLUDE_DIRS "include"
        REQUIRES 
            core 
            interfaces
    )
endif()
"""
    create_file_with_content(base_dir / "CMakeLists.txt", cmake_content)

    # 3. Kconfig
    kconfig_content = f"""menu "{human_name} Module Configuration"

    config MODULE_{module_name.upper()}_ENABLED
        bool "Enable {human_name} Module"
        default y
        help
            Enables the {human_name} module for {description}.

endmenu
"""
    create_file_with_content(base_dir / "Kconfig", kconfig_content)
    
    # 4. Header ფაილი
    header_content = HEADER_TEMPLATE.format(
        guard=f"{module_name.upper()}_H",
        module_name=module_name,
        description=description,
        author=author,
        date=date,
        module_name_title=human_name
    )
    create_file_with_content(include_dir / f"{module_name}.h", header_content)

    # 5. Source ფაილი
    source_content = SOURCE_TEMPLATE.format(
        module_name=module_name,
        description=description,
        author=author,
        date=date,
        tag=module_name.upper()
    )
    create_file_with_content(src_dir / f"{module_name}.c", source_content)

    print(f"\n🎉 მოდული '{human_name}' წარმატებით შეიქმნა!")
    print("   შემდეგი ნაბიჯები:")
    print(f"   1. დაარედაქტირეთ გენერირებული ფაილები '{base_dir}'-ში (დაამატეთ ლოგიკა TODO-ების მიხედვით).")
    # ⭐️ შესწორებულია ინსტრუქცია
    print("   2. გაუშვით 'idf.py reconfigure', რათა სისტემამ ავტომატურად აღმოაჩინოს ახალი მოდული.")
    print("   3. გაუშვით 'idf.py build', რათა დარწმუნდეთ, რომ ყველაფერი კარგად კომპილირდება.")

def main():
    """სკრიპტის მთავარი ფუნქცია."""
    parser = argparse.ArgumentParser(
        description="Synapse Framework-ისთვის ახალი მოდულის შემქმნელი სკრიპტი.",
        formatter_class=argparse.RawTextHelpFormatter
    )
    parser.add_argument(
        "name",
        type=str,
        help="მოდულის სახელი (მაგ. 'wifi manager', 'ota updater')."
    )
    parser.add_argument(
        "-c", "--category",
        type=str,
        required=True,
        help="მოდულის კატეგორია (მაგ. 'sensors', 'comms', 'utils')."
    )
    parser.add_argument(
        "-d", "--description",
        type=str,
        required=True,
        help="მოდულის მოკლე, ერთსიტყვიანი აღწერა (მაგ: 'WiFi კავშირის მართვა')."
    )
    parser.add_argument(
        "--author",
        type=str,
        help="მოდულის ავტორი (ნაგულისხმევად გამოიყენება Git-ის სახელი)."
    )

    args = parser.parse_args()
    generate_module(args)

if __name__ == "__main__":
    main()