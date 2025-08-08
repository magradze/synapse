# API რეფერენსი: Event Payloads (event_payloads.h)

## მიზანი

ეს დოკუმენტი აღწერს სტანდარტული მონაცემთა სტრუქტურებს (payloads), რომლებიც გამოიყენება Synapse Framework-ის Event Bus-ში, როგორც ივენთებთან ასოცირებული მონაცემები. ეს სტრუქტურები უზრუნველყოფენ ტიპიზირებულ და უსაფრთხო მონაცემთა გაცვლას მოდულებს შორის.

## Payload სტრუქტურები

ქვემოთ მოცემულია ძირითადი payload სტრუქტურები და მათი დანიშნულება.

### synapse_telemetry_payload_t

ეს სტრუქტურა გამოიყენება ტელემეტრიული მონაცემების ან ბრძანებების გადასაცემად.

- `const char* module_name`: იმ მოდულის სახელი, რომელსაც ეხება ბრძანება ან მონაცემი.
- `const char* json_data`: მონაცემები JSON ფორმატის სტრიქონში.
- `void* user_context`: დამატებითი, მომხმარებლის მიერ განსაზღვრული კონტექსტი.

### synapse_config_updated_payload_t

გამოიყენება `FRAMEWORK_EVENT_CONFIG_UPDATED` ივენთთან ერთად, რათა ამცნოს მოდულებს კონფიგურაციის ცვლილების შესახებ.

- `const char* module_name`: იმ მოდულის სახელი, ვისი კონფიგურაციაც განახლდა.
- `const cJSON* new_config`: cJSON ობიექტი, რომელიც შეიცავს ახალ კონფიგურაციას.

## მეხსიერების მართვის ფუნქციები

Payload-ების დინამიური ბუნებიდან გამომდინარე, აუცილებელია მეხსიერების სწორი მართვა. `synapse_event_data_wrap` ფუნქციასთან ერთად გამოიყენება სპეციალური გასუფთავების ფუნქციები.

### synapse_payload_common_free

ეს არის ზოგადი დანიშნულების ფუნქცია, რომელიც ათავისუფლებს `calloc`-ით გამოყოფილ მეხსიერებას payload-ისთვის. ის უნდა გამოიყენოთ მარტივი სტრუქტურებისთვის, რომლებიც არ შეიცავენ დამატებით დინამიურ მეხსიერებას.

### synapse_telemetry_payload_free

ეს ფუნქცია სპეციალურად `synapse_telemetry_payload_t` სტრუქტურისთვისაა შექმნილი. ის ათავისუფლებს არა მხოლოდ თავად სტრუქტურას, არამედ მის შიგნით არსებულ `module_name` და `json_data` სტრიქონებსაც, რომლებიც დინამიურად არის გამოყოფილი.

**გამოყენების მაგალითი:**

```c
// ტელემეტრიის შექმნა
synapse_telemetry_payload_t* telemetry = malloc(sizeof(synapse_telemetry_payload_t));
telemetry->module_name = strdup("my_module");
telemetry->json_data = strdup("{"key":"value"}");

// მონაცემების შეფუთვა და Event Bus-ში გაგზავნა
// synapse_telemetry_payload_free ფუნქცია ავტომატურად გამოიძახება,
// როდესაც მონაცემებს აღარავინ გამოიყენებს.
synapse_event_data_wrapper_t* wrapper;
synapse_event_data_wrap(telemetry, synapse_telemetry_payload_free, &wrapper);
synapse_event_bus_post("TELEMETRY_EVENT", wrapper);

// wrapper-ის გათავისუფლება, როცა ის აღარ გვჭირდება
synapse_event_data_release(wrapper);
```
