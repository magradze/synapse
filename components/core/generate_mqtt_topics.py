#!/usr/bin/env python3
"""
Synapse ESP Framework - MQTT Topics Auto-Generation Script

This script collects MQTT topic definitions from all module.json files
and generates a centralized mqtt_topics_generated.h header file.
It uses a new 'mqtt_interface' section in module.json to define
published and subscribed topics, making the system more robust and maintainable.

Usage: Called automatically by CMake during the build process.
"""

import json
import os
import sys
import glob
import re
from datetime import datetime
import argparse

def find_module_json_files(modules_dir):
    """
    Recursively finds all 'module.json' files within the specified directory.
    
    Args:
        modules_dir (str): The root directory to search for modules.
        
    Returns:
        list: A list of paths to the found module.json files.
    """
    pattern = os.path.join(modules_dir, "**", "module.json")
    return glob.glob(pattern, recursive=True)

def parse_module_topics(module_json_file):
    """
    Parses MQTT topics from a module.json file using the 'mqtt_interface' structure.
    
    Args:
        module_json_file (str): Path to the module.json file.
        
    Returns:
        tuple: A tuple containing (module_name, mqtt_interface_data), or (None, None) if not found.
    """
    try:
        with open(module_json_file, 'r', encoding='utf-8') as f:
            data = json.load(f)
        
        module_name = data.get('name')
        if not module_name:
            return None, None
            
        mqtt_interface = data.get('mqtt_interface', {})
        if not mqtt_interface:
            return None, None
        
        print(f"  📨 Processing MQTT interface for: {module_name}")
        return module_name, mqtt_interface
    
    except (json.JSONDecodeError, FileNotFoundError) as e:
        print(f"  ❌ Error parsing {module_json_file}: {e}")
        return None, None

def generate_topic_defines(module_name, interface_data):
    """
    Generates C-style #define statements and Doxygen comments for each topic.
    
    Args:
        module_name (str): The name of the module (e.g., 'wifi_manager').
        interface_data (dict): The 'mqtt_interface' dictionary from module.json.
        
    Returns:
        tuple: A tuple containing (list_of_defines, list_of_docs, topic_count).
    """
    defines = []
    docs = []
    count = 0
    
    # Base topic format for Synapse Framework
    base_topic_format = "synapse/devices/%s/{}"

    module_upper = re.sub(r'[^A-Z0-9_]', '_', module_name.upper())

    # Process 'publishes' topics
    for key, sub_topic in interface_data.get('publishes', {}).items():
        key_upper = key.upper()
        define_name = f"MQTT_TOPIC_PUB_{module_upper}_{key_upper}"
        full_topic = base_topic_format.format(sub_topic)
        
        defines.append(f"#define {define_name} \"{full_topic}\"")
        docs.append(f" * - {define_name}: .../{sub_topic}")
        count += 1

    # Process 'subscribes' topics
    for key, sub_topic in interface_data.get('subscribes', {}).items():
        key_upper = key.upper()
        define_name = f"MQTT_TOPIC_SUB_{module_upper}_{key_upper}"
        full_topic = base_topic_format.format(sub_topic)
        
        defines.append(f"#define {define_name} \"{full_topic}\"")
        docs.append(f" * - {define_name}: .../{sub_topic}")
        count += 1
    
    return defines, docs, count

def main():
    """
    Main execution function. Parses arguments, finds modules, generates topics,
    and writes the final header file from a template.
    """
    parser = argparse.ArgumentParser(description='Generate MQTT topics from module.json files for Synapse Framework')
    parser.add_argument('modules_dir', help='Directory containing modules')
    parser.add_argument('output_file', help='Output header file path')
    parser.add_argument('template_file', help='Template file path')
    
    args = parser.parse_args()
    
    print("🔍 Collecting MQTT topics from Synapse modules...")
    
    module_files = find_module_json_files(args.modules_dir)
    
    all_defines = []
    all_docs = []
    processed_modules = []
    total_topics = 0
    
    for module_file in module_files:
        module_name, interface_data = parse_module_topics(module_file)
        
        if module_name and interface_data:
            processed_modules.append(module_name)
            defines, docs, count = generate_topic_defines(module_name, interface_data)
            
            all_defines.extend(defines)
            all_docs.extend(docs)
            total_topics += count

    try:
        with open(args.template_file, 'r', encoding='utf-8') as f:
            template_content = f.read()
    except FileNotFoundError:
        print(f"❌ Template file not found: {args.template_file}")
        sys.exit(1)

    timestamp = datetime.utcnow().strftime("%Y-%m-%d %H:%M:%S UTC")
    
    # --- Corrected placeholder replacement logic ---
    
    # First, join the lists into multi-line strings
    all_defines_str = "\n".join(all_defines)
    all_docs_str = "\n".join(all_docs)

    # Then, replace the placeholders in the template content
    output_content = template_content
    output_content = output_content.replace('@GENERATION_TIMESTAMP@', timestamp)
    output_content = output_content.replace('@TOTAL_TOPICS@', str(total_topics))
    output_content = output_content.replace('@PROCESSED_MODULES@', ", ".join(sorted(list(set(processed_modules)))))
    output_content = output_content.replace('@ALL_TOPIC_DEFINES@', all_defines_str)
    output_content = output_content.replace('@ALL_TOPIC_DOCS@', all_docs_str)
    
    # Write the final content to the output file
    with open(args.output_file, 'w', encoding='utf-8') as f:
        f.write(output_content)
        
    print(f"✅ Generated {args.output_file} with {total_topics} topics for {len(set(processed_modules))} modules.")

if __name__ == "__main__":
    main()