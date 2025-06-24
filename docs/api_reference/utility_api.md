# Synapse Utility Module API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის დამხმარე (utility) მოდულების სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## Logger Module API

### module_t *logger_module_create(const cJSON *config);
- ქმნის ახალი ლოგერის მოდულის ინსტანციას გადაცემული კონფიგურაციით.

### esp_err_t logger_module_log_info(const char *message);
- წერს info დონის შეტყობინებას ლოგში.

### esp_err_t logger_module_log_warning(const char *message);
- წერს warning დონის შეტყობინებას ლოგში.

### esp_err_t logger_module_log_error(const char *message);
- წერს error დონის შეტყობინებას ლოგში.

---

## Naming & Structure
- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

**Info ლოგი:**
```c
logger_module_log_info("System started successfully");
```

**Error ლოგი:**
```c
logger_module_log_error("Failed to initialize sensor");
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure] დოკუმენტები.

