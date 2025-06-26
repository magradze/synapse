#include "base_module.h"
#include "cJSON.h"

typedef module_t* (*module_create_fn_t)(const cJSON *config);

typedef struct {
    const char* module_type_name;
    module_create_fn_t create_fn;
} module_factory_map_t;