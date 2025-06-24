#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Synapse Framework-ის კონვენციების ავტომატური შემმოწმებელი.

ეს სკრიპტი ამოწმებს პროექტის მოდულებს შემდეგი წესების მიხედვით:
1.  აუცილებელი ფაილების არსებობა (`CMakeLists.txt`, `module.json` და ა.შ.).
2.  `DEFINE_COMPONENT_TAG`-ის არსებობა ყველა `.c` ფაილში.
3.  Doxygen-ის სტილის კომენტარების არსებობა public ფუნქციებისთვის `.h` ფაილებში.
4.  მოდულებს შორის პირდაპირი `#include`-ების აკრძალვა იზოლაციის უზრუნველსაყოფად.

გამოიყენება GitHub Actions-ში Pull Request-ების ავტომატური ვალიდაციისთვის.
"""

import os
import re
import sys
from pathlib import Path
from typing import List, Set, NoReturn

# --- კონფიგურაცია ---

# ფაილები, რომლებიც ყველა მოდულს უნდა ჰქონდეს
REQUIRED_FILES = [
    'CMakeLists.txt',
    'module.json',
    'README.md',
]

# პრეფიქსები ან ფაილის სახელები, რომელთა include-ც ნებადართულია ნებისმიერი მოდულიდან.
ALLOWED_INCLUDE_PATTERNS = [
    re.compile(r'<.*>'),          # სისტემური ჰედერები, მაგ: <stdio.h>
    re.compile(r'^esp_.*\.h$'),    # ESP-IDF ჰედერები
    re.compile(r'^driver/.*\.h$'),
    re.compile(r'^freertos/.*\.h$'),
    re.compile(r'^nvs_flash\.h$'),
    re.compile(r'^cJSON\.h$'),
    # --- Synapse Core და Interfaces ---
    re.compile(r'^service_locator\.h$'),
    re.compile(r'^base_module\.h$'),
    re.compile(r'^logging\.h$'),
    re.compile(r'^event_bus\.h$'),
    re.compile(r'^config_manager\.h$'),
    re.compile(r'^module_registry\.h$'),
    re.compile(r'^system_manager\.h$'),
    re.compile(r'^resource_manager\.h$'),
    re.compile(r'^framework_config\.h$'),
    re.compile(r'^service_types\.h$'),
    re.compile(r'^telemetry_events\.h$'),
]

# --- გლობალური ცვლადები ---

# შეცდომების შესანახი სია
errors_found: List[str] = []

# --- დამხმარე ფუნქციები ---

def print_info(message: str):
    """ბეჭდავს ლურჯ [INFO] შეტყობინებას."""
    print(f'\033[94m[INFO]\033[0m {message}')

def print_error(message: str):
    """ბეჭდავს წითელ [ERROR] შეტყობინებას და ამატებს გლობალურ სიაში."""
    print(f'\033[91m[ERROR]\033[0m {message}')
    errors_found.append(message)

# --- შემოწმების ფუნქციები ---

def check_required_files(module_path: Path, module_name: str):
    """ამოწმებს, აქვს თუ არა მოდულს ყველა სავალდებულო ფაილი."""
    print_info(f"ვამოწმებ ფაილების სტრუქტურას: {module_name}")
    missing_files = []
    for required_file in REQUIRED_FILES:
        if not (module_path / required_file).is_file():
            missing_files.append(required_file)
            
    header_file = module_path / 'include' / f'{module_name}.h'
    if not header_file.is_file():
        missing_files.append(f'include/{module_name}.h')
        
    src_dir = module_path / 'src'
    if not src_dir.is_dir() or not any(src_dir.glob('*.c')):
        missing_files.append('src/*.c (მინიმუმ ერთი C ფაილი)')
        
    if missing_files:
        print_error(f"{module_path}: აკლია აუცილებელი ფაილები: {', '.join(missing_files)}")

def check_define_component_tag(src_dir: Path):
    """ამოწმებს `DEFINE_COMPONENT_TAG`-ის არსებობას ყველა .c ფაილში."""
    print_info(f"ვამოწმებ DEFINE_COMPONENT_TAG-ს: {src_dir.parent.name}")
    for c_file in src_dir.glob('*.c'):
        try:
            with c_file.open('r', encoding='utf-8') as f:
                if 'DEFINE_COMPONENT_TAG' not in f.read():
                    print_error(f"{c_file}: ფაილს აკლია `DEFINE_COMPONENT_TAG(...)`.")
        except IOError as e:
            print_error(f"ვერ ვკითხულობ ფაილს: {c_file}, მიზეზი: {e}")


def _has_doxygen_comment_before(line_index: int, all_lines: list) -> bool:
    """
    დამხმარე ფუნქცია, რომელიც ამოწმებს, აქვს თუ არა მოცემულ ხაზს Doxygen კომენტარი.
    იგი მოძრაობს უკან, ტოვებს ცარიელ ხაზებს და ამოწმებს, თუ პირველივე
    არაცარიელი ხაზი იწყება `/**`-ით.
    """
    current_index = line_index - 1
    while current_index >= 0:
        previous_line = all_lines[current_index].strip()
        if not previous_line:
            current_index -= 1
            continue
        return previous_line.startswith('/**')
    return False

def check_doxygen_comments(header_file: Path):
    """
    (რეფაქტორირებული) ამოწმებს Doxygen კომენტარების არსებობას public ფუნქციებისთვის.
    """
    print_info(f"ვამოწმებ Doxygen კომენტარებს: {header_file.name}")
    func_pattern = re.compile(r'^\s*(?!static\b)\w+[\w\s\*]+\s+\w+\s*\(.*\)\s*;')
    
    try:
        with header_file.open('r', encoding='utf-8') as f:
            lines = f.readlines()
    except IOError as e:
        print_error(f"ვერ ვკითხულობ ფაილს: {header_file}, მიზეზი: {e}")
        return

    for i, line in enumerate(lines):
        if func_pattern.match(line.strip()):
            if not _has_doxygen_comment_before(i, lines):
                try:
                    func_name = line.strip().split('(')[0].split()[-1].replace('*', '')
                except IndexError:
                    func_name = "[სახელი ვერ მოიძებნა]"
                print_error(f"{header_file}:{i+1}: Public ფუნქციას '{func_name}' აკლია Doxygen კომენტარი.")

def is_include_allowed(include_path: str, current_module_name: str, all_module_names: Set[str]) -> bool:
    """
    ამოწმებს, არის თუ არა მოცემული include ნებადართული.
    აკრძალულია სხვა მოდულის ჰედერის პირდაპირი ჩართვა.
    """
    if include_path.startswith(f'{current_module_name}'):
        return True
    
    for pattern in ALLOWED_INCLUDE_PATTERNS:
        if pattern.match(include_path):
            return True
            
    for other_module in all_module_names:
        if current_module_name != other_module and include_path == f"{other_module}.h":
            return False

    return True

def check_forbidden_includes(file_path: Path, module_name: str, all_module_names: Set[str]):
    """ამოწმებს ფაილში აკრძალული #include-ების არსებობას."""
    print_info(f"ვამოწმებ include-ებს: {file_path.relative_to(Path.cwd())}")
    include_pattern = re.compile(r'^\s*#include\s+["<]([^">]+)[">]')
    
    try:
        with file_path.open('r', encoding='utf-8') as f:
            for i, line in enumerate(f, 1):
                match = include_pattern.match(line)
                if match:
                    include_path = match.group(1).strip()
                    if not is_include_allowed(include_path, module_name, all_module_names):
                        print_error(f"{file_path}:{i}: აკრძალული/დაუშვებელი include: `{include_path}`")
    except IOError as e:
        print_error(f"ვერ ვკითხულობ ფაილს: {file_path}, მიზეზი: {e}")

# --- მთავარი ლოგიკის ფუნქციები (რეფაქტორინგის შედეგი) ---

def find_all_modules(root_path: Path) -> List[Path]:
    """
    პოულობს პროექტში ყველა მოდულის 경로ს და აბრუნებს სიის სახით.
    """
    if not root_path.is_dir():
        print_error(f"მოდულების მთავარი საქაღალდე ვერ მოიძებნა: {root_path}")
        return []

    module_paths = []
    for category_dir in root_path.iterdir():
        if category_dir.is_dir():
            for module_dir in category_dir.iterdir():
                if module_dir.is_dir():
                    module_paths.append(module_dir)
    return module_paths

def run_checks_on_module(module_path: Path, all_module_names: Set[str]):
    """
    უშვებს ყველა საჭირო შემოწმებას ერთ კონკრეტულ მოდულზე.
    """
    module_name = module_path.name
    
    check_required_files(module_path, module_name)
    
    src_dir = module_path / 'src'
    if src_dir.is_dir():
        check_define_component_tag(src_dir)

    header_file = module_path / 'include' / f'{module_name}.h'
    if header_file.is_file():
        check_doxygen_comments(header_file)
        
    for file_to_check in module_path.glob('**/*.[ch]'):
        check_forbidden_includes(file_to_check, module_name, all_module_names)

def main() -> NoReturn:
    """
    (რეფაქტორირებული) სკრიპტის მთავარი შესრულებადი ნაწილი.
    კოგნიტური სირთულე შემცირებულია ლოგიკის დაშლით.
    """
    project_root = Path.cwd()
    modules_root = project_root / 'components' / 'modules'

    print_info("ვიწყებ Synapse Framework-ის მოდულების შემოწმებას...")

    all_module_paths = find_all_modules(modules_root)
    if not all_module_paths:
        print_error("პროექტში მოდულები ვერ მოიძებნა. ვასრულებ მუშაობას.")
        sys.exit(1)

    all_module_names = {p.name for p in all_module_paths}

    for module_path in all_module_paths:
        run_checks_on_module(module_path, all_module_names)
            
    if errors_found:
        print("\n" + "="*50)
        print(f"\033[91mშემოწმება დასრულდა შეცდომებით. სულ აღმოჩენილია: {len(errors_found)} პრობლემა.\033[0m")
        print("="*50)
        sys.exit(1)
    
    print("\n" + "="*50)
    print("\033[92mშესანიშნავია! ყველა კონვენცია დაცულია.\033[0m")
    print("="*50)
    sys.exit(0)

if __name__ == "__main__":
    main()