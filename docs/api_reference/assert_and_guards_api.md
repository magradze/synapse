# Synapse Assertions & Guards API Reference

## 🎯 მიზანი

ეს დოკუმენტი აღწერს `synapse_assert.h` და `synapse_utils.h`-ში განსაზღვრული მაკროების API-ს, რომლებიც განკუთვნილია კოდის მდგრადობის გასაუმჯობესებლად.

- **Assertions (`SYNAPSE_ASSERT`):** გამოიყენება პროგრამისტის ლოგიკური შეცდომების აღმოსაჩენად დეველოპმენტის ფაზაში.
- **Guards (`SYNAPSE_GUARD`):** გამოიყენება `runtime` შეცდომების ლაკონური დამუშავებისთვის.

დეტალური ინფორმაციისთვის, თუ **როდის** უნდა გამოიყენოთ თითოეული მათგანი, იხილეთ [Assertions და Guard Clauses კონვენცია](../convention/assertion_and_guards.md).

---

## 1. Assertion API (`synapse_assert.h`)

ეს მაკრო აქტიურია მხოლოდ მაშინ, როდესაც `menuconfig`-ში ჩართულია `CONFIG_SYNAPSE_ENABLE_ASSERTS` ოფცია. `release` ბილდებში ის არანაირ კოდს არ აგენერირებს.

### `SYNAPSE_ASSERT(condition, format, ...)`

ამოწმებს პირობას (`condition`). თუ პირობა მცდარია (`false`), ის ბეჭდავს ფორმატირებულ შეტყობინებას, ფაილის სახელს, ხაზის ნომერს და დაუყოვნებლივ აჩერებს სისტემის მუშაობას (`panic`).

- **`condition`**: შესამოწმებელი ლოგიკური გამოსახულება. თუ `true`-ა, არაფერი ხდება.
- **`format`**: `printf`-ის სტილის ფორმატირების სტრიქონი, რომელიც აღწერს შეცდომას.
- **`...`**: (არასავალდებულო) ცვლადი არგუმენტები `format` სტრიქონისთვის.

**მაგალითი:**

```c
void process_user(user_t *user) {
    // ამოწმებს, რომ user მაჩვენებელი არ არის NULL.
    // თუ NULL-ია, სისტემა გაჩერდება შეტყობინებით:
    // "SYNAPSE_ASSERT FAILED: .../my_module.c:42: User pointer cannot be NULL"
    SYNAPSE_ASSERT(user != NULL, "User pointer cannot be NULL");

    // ამოწმებს, რომ მომხმარებლის ID ვალიდურია.
    SYNAPSE_ASSERT(user->id > 0, "Invalid user ID: %d", user->id);
}
```

---

## 2. Guard Clause API (`synapse_utils.h`)

ეს მაკროები ყოველთვის აქტიურია და წარმოადგენს `if-return` ბლოკების კომპაქტურ ალტერნატივას.

### `SYNAPSE_GUARD(condition, tag, ret_val, format, ...)`

ამოწმებს პირობას (`condition`). თუ პირობა მცდარია, ის `ESP_LOGE`-ს გამოყენებით ბეჭდავს შეცდომის შეტყობინებას და ასრულებს `return ret_val;`-ს.

- **`condition`**: შესამოწმებელი ლოგიკური გამოსახულება.
- **`tag`**: გამომძახებელი მოდულის ლოგირების `TAG`.
- **`ret_val`**: მნიშვნელობა, რომელიც უნდა დაბრუნდეს ფუნქციიდან, თუ პირობა მცდარია (მაგ., `ESP_ERR_INVALID_ARG`, `NULL`, `false`).
- **`format`**: `printf`-ის სტილის ფორმატირების სტრიქონი ლოგისთვის.
- **`...`**: (არასავალდებულო) ცვლადი არგუმენტები `format` სტრიქონისთვის.

**მაგალითი:**

```c
esp_err_t send_data(data_packet_t *packet) {
    // თუ packet არის NULL, დაიბეჭდება Error ლოგი და ფუნქცია დააბრუნებს ESP_ERR_INVALID_ARG-ს.
    SYNAPSE_GUARD(packet != NULL, TAG, ESP_ERR_INVALID_ARG, "Data packet is NULL, cannot send.");

    // ... დანარჩენი ლოგიკა ...
    return ESP_OK;
}
```

### `SYNAPSE_GUARD_VOID(condition, tag, format, ...)`

`SYNAPSE_GUARD`-ის ვერსია იმ ფუნქციებისთვის, რომლებიც აბრუნებენ `void`-ს. თუ პირობა მცდარია, ის ბეჭდავს ლოგს და ასრულებს `return;`-ს.

- **პარამეტრები:** იგივეა, რაც `SYNAPSE_GUARD`-ის, `ret_val`-ის გარდა.

**მაგალითი:**

```c
void update_led(led_handle_t *led) {
    // თუ led არის NULL, დაიბეჭდება Error ლოგი და ფუნქცია დაასრულებს მუშაობას.
    SYNAPSE_GUARD_VOID(led != NULL, TAG, "LED handle is NULL, cannot update.");

    // ... დანარჩენი ლოგიკა ...
}
```
