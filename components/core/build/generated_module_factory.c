/**
 * @file generated_module_factory.c
 * @brief ავტომატურად გენერირებული მოდულის ქარხანა. არ შეცვალოთ ხელით.
 *
 * @details ეს ფაილი ავტომატურად გენერირდება build სისტემის მიერ პროექტში არსებული module.json ფაილების მიხედვით. იგი ასახავს მოდულის ტიპის სტრიქონებს შესაბამის ქარხნის ფუნქციებზე, რაც უზრუნველყოფს მოდულების დინამიურ შექმნას მათი module registry-ში ხელით ჩამოწერის გარეშე.
 */

#include "generated_module_factory.h"
#include <string.h>
#include <stddef.h>

// --- ყველა რეგისტრირებული მოდულის ჩასამატებელი ---
// ეს ბლოკი ავტომატურად ივსება CMake-ის მიერ
#ifdef CONFIG_MODULE_LOGGER_ENABLED
#include "logger_module.h"
#endif


// --- ავტომატურად გენერირებული Factory Map ---
static const module_factory_map_t module_factories[] = {
    #ifdef CONFIG_MODULE_LOGGER_ENABLED
    { "logger_module", (module_create_fn_t)&logger_module_create },
#endif

    // სიის დასასრული
    { NULL, NULL }
};

// --- საჯარო API ---
module_create_fn_t fmw_module_factory_get(const char* module_type)
{
    if (module_type == NULL) {
        return NULL;
    }

    for (int i = 0; module_factories[i].module_type_name != NULL; i++)
    {
        if (strcmp(module_factories[i].module_type_name, module_type) == 0)
        {
            return module_factories[i].create_fn;
        }
    }

    return NULL; // მოდულის ტიპი ვერ მოიძებნა
}
