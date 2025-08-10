# მოდულის სტრუქტურის კონვენცია

**სტატუსი:** უმკაცრესი მოთხოვნა (Mandatory)
**ვერსია:** 2.0.0

## 1. 📜 მიმოხილვა

ეს დოკუმენტი განსაზღვრავს Synapse Framework-ის მოდულების ფაილური და კოდის სტრუქტურის **ერთიან, სავალდებულო სტანდარტს**. ამ კონვენციის ზედმიწევნით დაცვა უზრუნველყოფს კოდის ერთგვაროვნებას, ამარტივებს ავტომატური ინსტრუმენტების (`create_module.py`, CMake სკრიპტები) მუშაობას და აჩქარებს ახალი დეველოპერების ადაპტაციას.

## 2. 📂 საქაღალდის სტრუქტურა

### 2.1. სტანდარტული მოდულის სტრუქტურა

ყველა მოდული უნდა მისდევდეს შემდეგ იერარქიას. ფაილები, რომლებიც მონიშნულია `(არასავალდებულო)`-თი, იქმნება მხოლოდ საჭიროების შემთხვევაში, მოდულის არქეტიპიდან გამომდინარე.

```plaintext
components/modules/{category}/{module_name}/
├── CMakeLists.txt
├── Kconfig
├── module.json
├── config.json
├── README.md
├── include/
│   └── {module_name}.h
└── src/
    ├── {module_name}_internal.h
    ├── {module_name}.c
    ├── {module_name}_api.c      # (არასავალდებულო, თუ აქვს Service API)
    ├── {module_name}_cmd.c      # (არასავალდებულო, თუ აქვს CLI ბრძანებები)
    └── {module_name}_events.c   # (არასავალდებულო, თუ ამუშავებს ივენთებს)
```

### 2.2. კატეგორიები

მოდულები უნდა განთავსდეს შესაბამის კატეგორიის დირექტორიაში `components/modules/`-ის ქვეშ:

```plaintext
actuators/ communications/ displays/ drivers/ provisioning/ security/ sensors/ storage/ system/ testing/ utilities/
```

## 3. 📄 ფაილების დანიშნულება და შინაარსი

### 3.1. `src/` დირექტორიის დანაწევრება

მოდულის კოდის წაკითხვადობისა და მართვადობის გასაუმჯობესებლად, `src` დირექტორიაში კოდი იყოფა ლოგიკური პასუხისმგებლობების მიხედვით:

- **`{module_name}.c` (დირიჟორი):** მთავარი ფაილი. ის პასუხისმგებელია მოდულის სიცოცხლის ციკლზე (`_create`, `_init`, `_start`, `_deinit`) და, როგორც წესი, შეიცავს მოდულის მთავარ ტასკს.

- **`{module_name}_internal.h` (შიდა კონტრაქტი):** **კრიტიკულად მნიშვნელოვანი ფაილი.** ეს არის მოდულის **პრივატული ჰედერი**. ის შეიცავს `private_data` სტრუქტურის დეკლარაციას და ყველა იმ ფუნქციის `Forward Declaration`-ს, რომლებიც `src` დირექტორიის სხვადასხვა `.c` ფაილებს შორის უნდა გაზიარდეს.

- **`{module_name}_api.c`:** (არასავალდებულო) შეიცავს მოდულის მიერ მოწოდებული `Service API`-ს ფუნქციების იმპლემენტაციას.

- **`{module_name}_cmd.c`:** (არასავალდებულო) შეიცავს `Command Router`-ისთვის განკუთვნილი CLI ბრძანებების `handler` ფუნქციების იმპლემენტაციას.

- **`{module_name}_events.c`:** (არასავალდებულო) შეიცავს `Event Bus`-იდან მიღებული ივენთების და/ან ESP-IDF-ის სისტემური ივენთების დამმუშავებელ ფუნქციებს.

### 3.2. კონფიგურაციის ფაილები

- **`config.json`:** განსაზღვრავს მოდულის **default runtime კონფიგურაციას**. ის უნდა შეიცავდეს JSON მასივს, რომლის თითოეული ელემენტი აღწერს მოდულის ერთ ინსტანციას. ეს ფაილი ავტომატურად ერთიანდება სისტემის საერთო კონფიგურაციაში.

- **`module.json`:** შეიცავს მოდულის მეტამონაცემებს build-სისტემისთვის (`name`, `version`, `init_function`, `init_level` და ა.შ.).

- **`Kconfig`:** განსაზღვრავს build-დროის კონფიგურაციის პარამეტრებს, რომლებიც ხელმისაწვდომია `idf.py menuconfig`-ში. მთავარი პარამეტრია `CONFIG_MODULE_{MODULE_NAME_UPPER}_ENABLED`, რომელიც რთავს ან თიშავს მოდულის კომპილაციას.

### 3.3. დოკუმენტაცია

- **`README.md`:** მოდულის დეტალური დოკუმენტაცია. უნდა შეიცავდეს მიმოხილვას, არქიტექტურულ პრინციპებს, კონფიგურაციის პარამეტრების აღწერას, Service API-ს, ივენთების, CLI ბრძანებების და გამოყენების მაგალითების სექციებს.
- **`include/{module_name}.h`:** საჯარო ჰედერი. უნდა შეიცავდეს მხოლოდ `_create` factory ფუნქციის დეკლარაციას და Doxygen-ის სტილის დოკუმენტაციას.

### 3.4. კოდის სტრუქტურა (`_create` ფუნქცია)

მოდულის `_create` ფუნქცია არის მისი მთავარი "შესასვლელი წერტილი". მან უნდა დაიცვას შემდეგი ლოგიკური თანმიმდევრობა:

1. **მეხსიერების გამოყოფა:** `calloc`-ით გამოიყოფა მეხსიერება `module_t` და `private_data_t` სტრუქტურებისთვის.
2. **კონფიგურაციის დუბლირება:** `cJSON_Duplicate`-ით იქმნება გადაცემული კონფიგურაციის დამოუკიდებელი ასლი.
3. **დამოკიდებულებების რუკის მინიჭება:** `module->dependency_map`-ს ენიჭება `static const module_dependency_t` მასივის მისამართი.
4. **კონფიგურაციის პარსინგი:** გამოიძახება `parse_config` ფუნქცია.
5. **სახელის მინიჭება:** `snprintf`-ით `module->name`-ს ენიჭება `instance_name`.
6. **სერვისის რეგისტრაცია:** (თუ მოდული `Service Provider`-ია) `synapse_service_register_with_status`-ით ხდება სერვისის რეგისტრაცია.
7. **`base` ფუნქციების მინიჭება:** `module->base` სტრუქტურის ველებს ენიჭება შესაბამისი `lifecycle` ფუნქციების მისამართები.

**ახალი, არასავალდებულო `debug_print` ფუნქცია:**

- `base` სტრუქტურას დაემატა `debug_print` ფუნქციის მაჩვენებელი.
- თუ მოდულს სურს, ჰქონდეს `modules --info` ბრძანების მხარდაჭერა, მან უნდა მოახდინოს ამ ფუნქციის იმპლემენტაცია და მიანიჭოს ის `_create` ფუნქციაში.

## 4. `CMakeLists.txt`-ის სტრუქტურა

Synapse Framework-ი იყენებს **"ჭკვიანი" პირობითი კომპილაციის** მექანიზმს, რათა უზრუნველყოს, რომ `menuconfig`-ში გამორთული მოდულები არ მოხვდნენ საბოლოო firmware-ში. ყველა მოდულის `CMakeLists.txt` ფაილი უნდა მისდევდეს ამ სტანდარტულ შაბლონს.

```cmake
# {Module Name} CMake Configuration
# Author: {Author Name}
# Version: {Version}

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. If the CONFIG variable does not exist yet (configure stage), register with a placeholder.
# 2. If the CONFIG variable exists and is enabled, register with full functionality.
# 3. If the CONFIG variable exists and is disabled, register an empty placeholder.

if(DEFINED CONFIG_MODULE_{MODULE_NAME_UPPER}_ENABLED)
    # CONFIG variable exists - check its value
    if(CONFIG_MODULE_{MODULE_NAME_UPPER}_ENABLED)
        # Module is ENABLED - full registration
        message(STATUS "{Module Name}: ENABLED - Compiling with full functionality")
        idf_component_register(
            SRCS
                "src/{module_name}.c"
                "src/{module_name}_api.c"    # Add/remove files as needed
                "src/{module_name}_cmd.c"
                "src/{module_name}_events.c"
            INCLUDE_DIRS "include"
            REQUIRES
                synapse # Provides all core framework APIs
                # Add other public interface dependencies here (e.g., storage_interface)
            PRIV_REQUIRES
                json
                # Add other private dependencies here (e.g., esp_wifi)
        )
    else()
        # Module is DISABLED - register an empty placeholder
        message(STATUS "{Module Name}: DISABLED - Compiling an empty placeholder")
        
        # Create an empty source file to prevent CMake from failing
        set(EMPTY_SOURCE_CONTENT "// {module_name} disabled by Kconfig\n")
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_{module_name}.c" "${EMPTY_SOURCE_CONTENT}")
        
        idf_component_register(
            SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_{module_name}.c"
            INCLUDE_DIRS "include"  # Header is still needed for the factory
            REQUIRES synapse
        )
    endif()
else()
    # CONFIG variable does not exist yet (configure stage) - temporary registration
    message(STATUS "{Module Name}: CONFIGURE STAGE - Registering temporarily")
    idf_component_register(
        SRCS "src/{module_name}.c" # Register at least one file
        INCLUDE_DIRS "include"
        REQUIRES synapse
    )
endif()
```

## 5. ❌ აკრძალული პრაქტიკები

- **არასდროს:**
  - სხვა მოდულის `.h` ან `_internal.h` ფაილის პირდაპირი `#include`.
  - `src` დირექტორიაში ქვედირექტორიების შექმნა.
  - `_create` ფუნქციაში გადაცემული `config` ობიექტის დუბლირება (`cJSON_Duplicate`).
  - `synapse_service_get()`-ის გამოძახება `_init` ფუნქციაში იმ დამოკიდებულებებისთვის, რომელთა ინექციაც შესაძლებელია `dependency_map`-ით.

## 6. ✅ საუკეთესო პრაქტიკები

- **ყოველთვის:**
  - გამოიყენეთ `create_module.py` სკრიპტი ახალი მოდულის ჩონჩხის შესაქმნელად.
  - აღწერეთ სერვისებზე დამოკიდებულებები `dependency_map`-ის საშუალებით.
  - `_deinit` ფუნქციამ უნდა გაათავისუფლოს `module->current_config` და `private_data`.
  - დაიცავით Doxygen-ის სტანდარტი ყველა ჰედერ ფაილსა და საჯარო ფუნქციაში.
  - (არასავალდებულო) დააიმპლემენტეთ `debug_print` ფუნქცია, რათა გაამარტივოთ მოდულის დიაგნოსტიკა CLI-დან.
