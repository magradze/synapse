# მოდულის სტრუქტურის კონვენცია

## საქაღალდის სტრუქტურა

### სტანდარტული მოდულის სტრუქტურა

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

### 1. `config.json`

ეს ფაილი განსაზღვრავს მოდულის **default runtime კონფიგურაციას**. ის უნდა შეიცავდეს JSON მასივს, რომლის თითოეული ელემენტი არის ობიექტი, რომელიც აღწერს მოდულის ერთ ინსტანციას.

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
    "description": "მოდულის აღწერა.",
    "author": "ავტორის სახელი",
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

### 3. `src/{module_name}.c` (განახლებული შაბლონი Dependency Injection-ის მხარდაჭერით)

Source ფაილმა უნდა დაიცვას მეხსიერების მართვისა და დამოკიდებულებების ინექციის ახალი წესები.

```c
#include "{module_name}.h"
#include "synapse.h" // ვიყენებთ ფრეიმვორქის ერთიან ჰედერს

// --- Dependency Injection-ის კონვენცია ---
// თუ ეს მოდული დამოკიდებულია რომელიმე სერვისზე, მისი handle-ის მაჩვენებელი
// აუცილებლად უნდა იყოს private_data სტრუქტურის პირველი წევრი.
// მაგალითი:
// typedef struct {
//     mcp23017_handle_t *expander_handle; // აუცილებლად პირველი წევრი!
//     // ... სხვა private მონაცემების ველები
// } my_module_private_data_t;

DEFINE_COMPONENT_TAG("{MODULE_NAME_UPPER}");

// ... Private მონაცემები, API გლობალები და ა.შ. ...

// --- Create და Deinit ფუნქციები ---

module_t *{module_name}_create(const cJSON *config) {
    // --- Dependency Map ---
    // ეს რუკა ეუბნება System Manager-ს, თუ როგორ მოახდინოს დამოკიდებულებების ინექცია.
    // ის აკავშირებს config.json-ის გასაღებს private_data სტრუქტურის ველთან.
    static const module_dependency_t s_dependencies[] = {
        // { "config_key_ending_with_service", offsetof(my_module_private_data_t, handle_pointer_field) },
        { NULL, 0 } // მასივის დამასრულებელი (terminator)
    };
    // 1. გამოვყოთ მეხსიერება მოდულისა და private მონაცემებისთვის
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    // ... შეცდომის დამუშავება ...
    
    // 2. ავიღოთ მფლობელობა (ownership) config ობიექტზე
    module->current_config = (cJSON*)config;

    module->dependency_map = s_dependencies; // მივანიჭოთ დამოკიდებულებების რუკა

    // 3. დავამუშაოთ კონფიგურაცია
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    if (!config_node) {
        // ... შეცდომის დამუშავება და სრული გასუფთავება ...
        return NULL;
    }
    // ... instance_name-ის და სხვა პარამეტრების დამუშავება ...
    
    // 4. დავაყენოთ საბაზისო ფუნქციების მაჩვენებლები
    module->base.init = {module_name}_init;
    module->base.deinit = {module_name}_deinit;
    // ... სხვა საბაზისო ფუნქციები ...

    return module;
}

static void {module_name}_deinit(module_t *self) {
    if (!self) return;

    // ... სერვისების რეგისტრაციის გაუქმება, ივენთებიდან გამოწერის გაუქმება, ტასკების/რიგების წაშლა ...

    // გამოვათავისუფლოთ ყველა გამოყოფილი მეხსიერება
    if (self->current_config) {
        cJSON_Delete(self->current_config);
    }
    if (self->private_data) {
        free(self->private_data);
    }
    free(self);
}

// --- მოდულის Init ფუნქცია (დამოკიდებულებების გათვალისწინებით) ---

static esp_err_t {module_name}_init(module_t *self) {
    my_module_private_data_t *private_data = (my_module_private_data_t *)self->private_data;

    // სერვისის handle-ი უკვე "ინექცირებულია" System Manager-ის მიერ init-ის გამოძახებამდე.
    // fmw_service_get()-ის გამოძახების ნაცვლად, ჩვენ უბრალოდ ვამოწმებთ ინექცირებულ handle-ს.
    if (!private_data->expander_handle) {
        ESP_LOGE(TAG, "Dependency injection failed: expander_handle is NULL!");
        return ESP_ERR_INVALID_STATE;
    }

    // ... ვაგრძელებთ ინიციალიზაციას handle-ის გამოყენებით ...
    return ESP_OK;
}

// ... სხვა მოდულის ფუნქციები (start, handle_event და ა.შ.) ...
```

## `CMakeLists.txt` სტრუქტურა

რეკომენდებულია გამარტივებული `CMakeLists.txt`-ის გამოყენება. პირობითი კომპილაცია იმართება `core` კომპონენტის მიერ გენერირებული `module_factory`-ით.

```cmake
# სტანდარტული გამარტივებული სტრუქტურა
idf_component_register(
    SRCS "src/{module_name}.c"
    INCLUDE_DIRS "include"
    REQUIRES
        synapse # ანაცვლებს 'core'-ს და 'interfaces'-ს ფრეიმვორქის API-სთვის
        # სხვა მოდულების ინტერფეისებზე დამოკიდებულებები (მაგ., mcp23017_interface)
    PRIV_REQUIRES
        json
        # სხვა private დამოკიდებულებები (მაგ., esp_timer)
)
```

## აკრძალული პრაქტიკები

❌ **არასდროს:**

- სხვა მოდულის ჰედერების პირდაპირი `#include`.
- runtime პარამეტრების შენახვა `module.json`-ში.
- `_create` ფუნქციაში გადაცემული `config` ობიექტის დუბლირება (`cJSON_Duplicate`).
- `fmw_service_get()`-ის გამოძახება `_init` ფუნქციაში იმ დამოკიდებულებებისთვის, რომელთა ინექციაც შესაძლებელია.

✅ **ყოველთვის:**

- გამოიყენეთ `Service Locator` და `Event Bus` კომუნიკაციისთვის.
- აღწერეთ სერვისებზე დამოკიდებულებები `dependency_map`-ის საშუალებით.
- განსაზღვრეთ runtime პარამეტრები `config.json`-ში.
- `_create` ფუნქციამ უნდა "მიითვისოს" (`take ownership`) გადაცემული `config` ობიექტი.
- `_deinit` ფუნქციამ უნდა გაათავისუფლოს `module->current_config`.
- **გამოაცხადეთ სერვისზე დამოკიდებულების handle-ი `private_data` სტრუქტურის პირველ წევრად, რათა იმუშაოს ავტომატურმა ინექციამ.**

---

ეს განახლება ასრულებს დოკუმენტაციის რეფაქტორინგს. ის ახლა სრულად შეესაბამება Synapse Framework v7.0.0-ის არქიტექტურას, Dependency Injection-ის პატერნის ჩათვლით.
