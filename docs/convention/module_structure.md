# მოდულის სტრუქტურის წესები

## საქაღალდის სტრუქტურა

### სტანდარტული მოდულის layout

```plaintext
components/modules/{category}/{module_name}/
├── CMakeLists.txt
├── Kconfig
├── module.json
├── config.json         # <--- ახალი
├── README.md
├── include/
│   └── {module_name}.h
└── src/
    └── {module_name}.c
```

### კატეგორიები

```plaintext
components/modules/
├── actuators/
├── communications/
├── displays/
├── drivers/
├── provisioning/
├── security/
├── sensors/
├── storage/
├── system/
├── testing/
└── utilities/
```

## ფაილის სტრუქტურა

### 1. `config.json` (ახალი სექცია)

ეს ფაილი განსაზღვრავს მოდულის default runtime კონფიგურაციას. ის უნდა შეიცავდეს JSON მასივს, რომლის თითოეული ელემენტი აღწერს მოდულის ერთ ინსტანციას.

```json
[
  {
    "type": "{module_name}",
    "enabled": true,
    "config": {
      "instance_name": "main_{module_name}"
    }
  }
]
```

### 2. `module.json`

ეს ფაილი შეიცავს მოდულის მეტამონაცემებს build-სისტემისთვის.

```json
{
    "name": "{module_name}",
    "version": "1.0.0",
    "description": "Module description.",
    "author": "Author Name",
    "init_function": "{module_name}_create",
    "init_level": 60,
    "build_enabled": true,
    "conditional_config": "CONFIG_MODULE_{MODULE_NAME_UPPER}_ENABLED",
    "mqtt_interface": {
        "publishes": {},
        "subscribes": {}
    }
}
```

### 3. `src/{module_name}.c` (განახლებული შაბლონი)

Source ფაილმა უნდა დაიცვას მეხსიერების მართვის ახალი წესები.

```c
// ... Includes ...

DEFINE_COMPONENT_TAG("{MODULE_NAME_UPPER}");

// ... Private Data, API Globals, etc. ...

// --- Create & Deinit Functions ---

module_t *{module_name}_create(const cJSON *config) {
    // 1. Allocate memory for module and private data
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    // ... error handling ...
    
    // 2. Take ownership of the config object
    module->current_config = (cJSON*)config;

    // 3. Parse the configuration
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (!config_node) {
        // ... error handling and cleanup ...
        return NULL;
    }
    // ... parse instance_name and other parameters ...
    
    // 4. Set up base function pointers
    module->base.init = {module_name}_init;
    module->base.deinit = {module_name}_deinit;
    // ... other base functions ...

    return module;
}

static void {module_name}_deinit(module_t *self) {
    if (!self) return;

    // ... Unregister services, unsubscribe from events, delete tasks/queues ...

    // Free all allocated memory
    if (self->current_config) {
        cJSON_Delete(self->current_config);
    }
    if (self->private_data) {
        free(self->private_data);
    }
    free(self);
}

// ... Other module functions (init, start, handle_event, etc.) ...
```

## `CMakeLists.txt` სტრუქტურა

რეკომენდებულია გამარტივებული `CMakeLists.txt`-ის გამოყენება. პირობითი კომპილაცია იმართება `core` კომპონენტის მიერ გენერირებული `module_factory`-ით.

```cmake
# Standard simplified structure
idf_component_register(
    SRCS "src/{module_name}.c"
    INCLUDE_DIRS "include"
    REQUIRES
        core
        interfaces
        # Other direct dependencies (e.g., a specific driver)
    PRIV_REQUIRES
        json
        # Other private dependencies (e.g., esp_timer)
)
```

## აკრძალული პრაქტიკები

❌ **არასდროს:**

- პირდაპირი `#include` სხვა მოდულის ჰედერების.
- `module.json`-ში runtime პარამეტრების შენახვა.
- `_create` ფუნქციაში გადაცემული `config` ობიექტის დუბლირება (`cJSON_Duplicate`).

✅ **ყოველთვის:**

- გამოიყენეთ `Service Locator` და `Event Bus` კომუნიკაციისთვის.
- განსაზღვრეთ runtime პარამეტრები `config.json`-ში.
- `_create` ფუნქციამ უნდა "მიითვისოს" (`take ownership`) გადაცემული `config` ობიექტი.
- `_deinit` ფუნქციამ უნდა გაათავისუფლოს `module->current_config`.

---

ამ ფაილის განახლებით, ჩვენი დოკუმენტაციის რეფაქტორინგი დასრულებულია. ის ახლა სრულად შეესაბამება Synapse Framework v5.0.0-ის არქიტექტურას.
