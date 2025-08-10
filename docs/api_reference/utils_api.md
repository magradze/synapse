# Synapse Utils API Reference

## 🎯 მიზანი

ეს დოკუმენტი აღწერს `synapse_utils.h`-ში თავმოყრილი დამხმარე ფუნქციების API-ს. ეს ბიბლიოთეკა შეიცავს ხშირად გამოყენებად, უსაფრთხო ფუნქციებს, რომლებიც ამარტივებენ მოდულების წერას და ამცირებენ კოდის დუბლირებას.

---

## 1. უსაფრთხო სტრიქონების მართვა

### `char* synapse_safe_strncpy(char *dest, const char *src, size_t size);`

`strncpy`-ის უსაფრთხო ვერსია, რომელიც **ყოველთვის უზრუნველყოფს** დანიშნულების ბუფერის `null-termination`-ს.

- **`dest`**: დანიშნულების ბუფერი.
- **`src`**: წყარო სტრიქონი.
- **`size`**: დანიშნულების ბუფერის **სრული ზომა**.
- **აბრუნებს:** მაჩვენებელს `dest` ბუფერზე.

---

## 2. კონფიგურაციის პარსინგის დამხმარე ფუნქციები

ეს ფუნქციები შექმნილია `parse_config` ფუნქციებში გამოსაყენებლად, რათა უსაფრთხოდ და მარტივად წაიკითხოთ პარამეტრები `cJSON` ობიექტიდან.

### `bool synapse_config_get_string_from_node(const char* tag, const cJSON *json_node, const char *key, char *dest, size_t dest_size);`

უსაფრთხოდ კითხულობს სტრიქონს `cJSON` კვანძიდან. თუ გასაღები არ არსებობს ან არასწორი ტიპისაა, `dest` ბუფერი უცვლელი რჩება.

- **`tag`**: გამომძახებელი მოდულის ლოგირების `TAG`.
- **`json_node`**: `cJSON` ობიექტი, რომელშიც ვეძებთ.
- **`key`**: საძიებელი პარამეტრის გასაღები.
- **`dest`**: ბუფერი, სადაც ჩაიწერება შედეგი.
- **`dest_size`**: ბუფერის სრული ზომა.
- **აბრუნებს:** `true` თუ წაკითხვა წარმატებით დასრულდა, `false` წინააღმდეგ შემთხვევაში.

### `bool synapse_config_get_int_from_node(const char* tag, const cJSON *json_node, const char *key, int *out_value);`

უსაფრთხოდ კითხულობს `int` ტიპის რიცხვს. თუ გასაღები არ არსებობს ან არასწორი ტიპისაა, `out_value` არ იცვლება.

- **`out_value`**: მაჩვენებელი `int` ცვლადზე, სადაც ჩაიწერება შედეგი.
- **აბრუნებს:** `true` თუ წაკითხვა წარმატებით დასრულდა, `false` წინააღმდეგ შემთხვევაში.

### `bool synapse_config_get_bool_from_node(const char* tag, const cJSON *json_node, const char *key, bool *out_value);`

უსაფრთხოდ კითხულობს `bool` ტიპის მნიშვნელობას. თუ გასაღები არ არსებობს ან არასწორი ტიპისაა, `out_value` არ იცვლება.

- **`out_value`**: მაჩვენებელი `bool` ცვლადზე, სადაც ჩაიწერება შედეგი.
- **აბრუნებს:** `true` თუ წაკითხვა წარმატებით დასრულდა, `false` წინააღმდეგ შემთხვევაში.

## 3. Guard Clause & Validation Helpers

ეს მაკროები ამარტივებენ ფუნქციის დასაწყისში პირობების შემოწმებას და შეცდომის შემთხვევაში ფუნქციიდან გამოსვლას. დეტალური ინფორმაციისთვის, თუ **როდის** უნდა გამოიყენოთ ისინი, იხილეთ [Assertions და Guard Clauses კონვენცია](../convention/assertion_and_guards.md).

### `SYNAPSE_GUARD(condition, tag, ret_val, format, ...)`

ამოწმებს პირობას (`condition`). თუ პირობა მცდარია, ის `ESP_LOGE`-ს გამოყენებით ბეჭდავს შეცდომის შეტყობინებას და ასრულებს `return ret_val;`-ს.

- **`condition`**: შესამოწმებელი ლოგიკური გამოსახულება.
- **`tag`**: გამომძახებელი მოდულის ლოგირების `TAG`.
- **`ret_val`**: მნიშვნელობა, რომელიც უნდა დაბრუნდეს ფუნქციიდან, თუ პირობა მცდარია (მაგ., `ESP_ERR_INVALID_ARG`, `NULL`, `false`).
- **`format`**: `printf`-ის სტილის ფორმატირების სტრიქონი ლოგისთვის.
- **`...`**: (არასავალდებულო) ცვლადი არგუმენტები `format` სტრიქონისთვის.

**მაგალითი:**

```c
esp_err_t process_data(data_packet_t *packet) {
    // თუ packet არის NULL, დაიბეჭდება Error ლოგი და ფუნქცია დააბრუნებს ESP_ERR_INVALID_ARG-ს.
    SYNAPSE_GUARD(packet != NULL, TAG, ESP_ERR_INVALID_ARG, "Data packet is NULL, cannot process.");

    // ... დანარჩენი ლოგიკა ...
    return ESP_OK;
}
```

### `SYNAPSE_GUARD_VOID(condition, tag, format, ...)`

`SYNAPSE_GUARD`-ის ვერსია იმ ფუნქციებისთვის, რომლებიც აბრუნებენ `void`-ს. თუ პირობა მცდარია, ის ბეჭდავს ლოგს და ასრულებს `return;`-ს.

- **პარამეტრები:** იგივეა, რაც `SYNAPSE_GUARD`-ის, `ret_val`-ის გარდა.

**მაგალითი:**

```c
void update_ui(ui_handle_t *ui) {
    // თუ ui არის NULL, დაიბეჭდება Error ლოგი და ფუნქცია დაასრულებს მუშაობას.
    SYNAPSE_GUARD_VOID(ui != NULL, TAG, "UI handle is NULL, cannot update.");

    // ... დანარჩენი ლოგიკა ...
}
```
