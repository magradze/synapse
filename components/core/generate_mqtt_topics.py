#!/usr/bin/env python3
"""
MindLab ESP Framework - MQTT Topics Auto-Generation Script

This script collects MQTT topic definitions from all module.json files
and generates a centralized mqtt_topics_generated.h header file.

Usage: python generate_mqtt_topics.py [source_dir] [system_config] [output_file] [template_file]
Author: MindLab Development Team
Version: 1.0
"""

import json
import os
import sys
import glob
import re
from datetime import datetime
import argparse

def find_module_json_files(modules_dir):
    """Find all module.json files recursively."""
    pattern = os.path.join(modules_dir, "**", "module.json")
    return glob.glob(pattern, recursive=True)

def parse_system_config_instances(system_config_file):
    """Parse system_config.json to get a map of module_type to instance_names."""
    instances_by_type = {}
    try:
        with open(system_config_file, 'r') as f:
            data = json.load(f)
        
        print(f"🔍 DEBUG: Looking for config at: {system_config_file}")  # <-- ეს
        
        for module_config in data.get('modules', []):
            if not module_config.get('enabled', False):
                continue
            
            module_type = module_config.get('type')
            instance_name = module_config.get('config', {}).get('instance_name')
            
            if module_type and instance_name:
                if module_type not in instances_by_type:
                    instances_by_type[module_type] = []
                instances_by_type[module_type].append(instance_name)
    except (json.JSONDecodeError, FileNotFoundError) as e:
        print(f"  ⚠️  Warning: Could not read or parse {system_config_file}: {e}. Fallback: using module types.")
    return instances_by_type

def parse_module_topics(module_json_file):
    """Parse MQTT topics from a module.json file."""
    try:
        with open(module_json_file, 'r') as f:
            data = json.load(f)
        
        module_type = data.get('type')
        if not module_type:
            return None, None
            
        mqtt_topics = data.get('mqtt_topics', {})
        if not mqtt_topics:
            return None, None
        
        print(f"  📨 Processing MQTT topics for: {module_type}")
        return module_type, mqtt_topics
    
    except (json.JSONDecodeError, FileNotFoundError) as e:
        print(f"  ❌ Error parsing {module_json_file}: {e}")
        return None, None

def generate_topic_defines(module_type, topics, instance_names):
    """Generate #define statements for each instance of a module."""
    defines = []
    docs = []
    count = 0

    # If no specific instances are enabled, fallback to module_type for generic topics
    if not instance_names:
        instance_names = [module_type]

    for instance_name in instance_names:
        instance_upper = re.sub(r'[^A-Z0-9_]', '_', instance_name.upper())

        for category, topic_list in topics.items():
            category_short = category.upper()[:-1]

            for topic_action in topic_list:
                action_upper = topic_action.upper()
                
                define_name = f"MQTT_TOPIC_{category_short}_{instance_upper}_{action_upper}"
                
                # New topic path format: .../cmd/{instance_name}/{action}
                if category == 'commands':
                    full_topic = f"mindlab/v1/%s/cmd/{instance_name}/{topic_action}"
                elif category == 'status':
                    full_topic = f"mindlab/v1/%s/status/{instance_name}/{topic_action}"
                elif category == 'data':
                    full_topic = f"mindlab/v1/%s/data/{instance_name}/{topic_action}"
                else:
                    continue
                
                defines.append(f"#define {define_name} \"{full_topic}\"")
                docs.append(f" * - {define_name}: .../{instance_name}/{topic_action}")
                count += 1
    
    return defines, docs, count

def main():
    parser = argparse.ArgumentParser(description='Generate MQTT topics from module.json files')
    parser.add_argument('modules_dir', help='Directory containing modules')
    parser.add_argument('system_config', help='Path to system_config.json')
    parser.add_argument('output_file', help='Output header file path')
    parser.add_argument('template_file', help='Template file path')
    
    args = parser.parse_args()
    
    print("🔍 Collecting instance-based MQTT topics...")
    
    instances_by_type = parse_system_config_instances(args.system_config)
    module_files = find_module_json_files(args.modules_dir)
    
    all_defines = []
    all_docs = []
    processed_items = []
    total_topics = 0
    
    for module_file in module_files:
        module_type, topics = parse_module_topics(module_file)
        
        if module_type and topics:
            instance_names = instances_by_type.get(module_type, [])
            
            # Use instance names if available, otherwise fallback to module_type
            if instance_names:
                 processed_items.extend(instance_names)
            else:
                 processed_items.append(module_type)

            defines, docs, count = generate_topic_defines(module_type, topics, instance_names)
            
            all_defines.extend(defines)
            all_docs.extend(docs)
            total_topics += count

    try:
        with open(args.template_file, 'r') as f:
            template_content = f.read()
    except FileNotFoundError:
        print(f"❌ Template file not found: {args.template_file}")
        sys.exit(1)

    timestamp = datetime.utcnow().strftime("%Y-%m-%d %H:%M:%S UTC")
    
    output_content = template_content
    output_content = re.sub(r'@GENERATION_TIMESTAMP@', timestamp, output_content)
    output_content = re.sub(r'@TOTAL_TOPICS@', str(total_topics), output_content)
    output_content = re.sub(r'@PROCESSED_MODULES@', ", ".join(sorted(list(set(processed_items)))), output_content)
    output_content = re.sub(r'@ALL_TOPIC_DEFINES@', "\\n".join(all_defines), output_content)
    output_content = re.sub(r'@ALL_TOPIC_DOCS@', "\\n".join(all_docs), output_content)
    
    with open(args.output_file, 'w') as f:
        f.write(output_content)
        
    print(f"✅ Generated {args.output_file} with {total_topics} topics for {len(set(processed_items))} instances/types.")

if __name__ == "__main__":
    main() 