# Synapse Promise Manager API Reference

## 🎯 მიზანი

ეს დოკუმენტი აღწერს `Promise Manager`-ის საჯარო API-ს, რომელიც ხელმისაწვდომია `synapse.h` ჰედერის მეშვეობით. ეს API იყოფა ორ ნაწილად:

1. **Consumer API:** განკუთვნილია იმ მოდულებისთვის, რომლებიც ითხოვენ ასინქრონულ ოპერაციას და ამუშავებენ მის შედეგს (`then`, `catch`).
2. **Provider API:** განკუთვნილია იმ სერვისის მომწოდებელი მოდულებისთვის, რომლებიც ახდენენ ასინქრონული ოპერაციის იმპლემენტაციას და ქმნიან `Promise`-ებს (`create`, `resolve`, `reject`).

---

## 1. Consumer API (მომხმარებლისთვის)

ეს ფუნქციები განკუთვნილია ნებისმიერი მოდულისთვის, რომელსაც სურს გამოიძახოს ასინქრონული ფუნქცია და დაამუშაოს მისი შედეგი.

### `promise_handle_t`

ეს არის "გაუმჭვირვალე" (opaque) `handle`-ი, რომელიც წარმოადგენს `Promise` ობიექტს. თქვენ იღებთ მას ასინქრონული ფუნქციის გამოძახებისას და იყენებთ სხვა API ფუნქციებში.

### `promise_then_cb`

ფუნქციის ტიპი, რომელიც გამოიყენება წარმატებული შედეგის დასამუშავებლად.

```c
typedef void (*promise_then_cb)(void* result_data, void* user_context);
```

- **`result_data`**: მაჩვენებელი შედეგის მონაცემებზე. `callback` ფუნქცია პასუხისმგებელია მის სწორ ტიპზე მიყვანაზე (casting).
- **`user_context`**: მომხმარებლის მიერ განსაზღვრული კონტექსტი, რომელიც `fmw_promise_then`-ის გამოძახებისას გადაეცა.

### `promise_catch_cb`

ფუნქციის ტიპი, რომელიც გამოიყენება შეცდომის დასამუშავებლად.

```c
typedef void (*promise_catch_cb)(void* error_data, void* user_context);
```

- **`error_data`**: მაჩვენებელი შეცდომის აღმწერ მონაცემებზე.
- **`user_context`**: მომხმარებლის კონტექსტი.

---

### `esp_err_t fmw_promise_then(promise_handle_t handle, promise_then_cb then_cb, void* user_context);`

არეგისტრირებს `callback` ფუნქციას, რომელიც შესრულდება `Promise`-ის წარმატებით დასრულებისას.

- **`handle`**: `Promise`-ის `handle`, რომელიც მიიღეთ ასინქრონული ფუნქციიდან.
- **`then_cb`**: ფუნქცია, რომელიც უნდა გამოიძახოს წარმატების შემთხვევაში.
- **`user_context`**: (არასავალდებულო) მაჩვენებელი ნებისმიერ მონაცემზე, რომელიც გსურთ, რომ გადაეცეს თქვენს `callback` ფუნქციას.
- **აბრუნებს:** `ESP_OK` წარმატების შემთხვევაში, `ESP_ERR_INVALID_ARG` თუ `handle` ან `then_cb` არის `NULL`, `ESP_ERR_INVALID_STATE` თუ `then` `callback`-ი უკვე დარეგისტრირებულია.

### `esp_err_t fmw_promise_catch(promise_handle_t handle, promise_catch_cb catch_cb, void* user_context);`

არეგისტრირებს `callback` ფუნქციას, რომელიც შესრულდება `Promise`-ის შეცდომით დასრულებისას.

- **`handle`**: `Promise`-ის `handle`.
- **`catch_cb`**: ფუნქცია, რომელიც უნდა გამოიძახოს შეცდომის შემთხვევაში.
- **`user_context`**: (არასავალდებულო) მომხმარებლის კონტექსტი.
- **აბრუნებს:** `ESP_OK` წარმატების შემთხვევაში, `ESP_ERR_INVALID_ARG` თუ `handle` ან `catch_cb` არის `NULL`, `ESP_ERR_INVALID_STATE` თუ `catch` `callback`-ი უკვე დარეგისტრირებულია.

### `void fmw_promise_destroy(promise_handle_t handle);`

ანადგურებს `Promise`-ს და ათავისუფლებს მასთან დაკავშირებულ რესურსებს.

- **`handle`**: გასანადგურებელი `Promise`-ის `handle`.
- **@note:** ამ ფუნქციის გამოძახება საჭიროა მხოლოდ იმ შემთხვევაში, თუ თქვენ `Promise`-ის შედეგი აღარ გაინტერესებთ მის დასრულებამდე. `Promise Manager`-ი ავტომატურად ასუფთავებს `Promise`-ებს მათი შესრულების (resolve/reject) შემდეგ.

---

## 2. Provider API (სერვისის მომწოდებლისთვის)

ეს ფუნქციები განკუთვნილია მხოლოდ იმ მოდულებისთვის, რომლებიც თავად ქმნიან ასინქრონულ, `Promise`-ზე დაფუძნებულ API-ს. მათზე წვდომისთვის საჭიროა `#include "promise_manager_internal.h"`.

### `promise_handle_t fmw_promise_create(void);`

ქმნის ახალ `Promise` ობიექტს `PENDING` (მომლოდინე) მდგომარეობაში.

- **აბრუნებს:** ახალი `Promise`-ის `handle`-ს, ან `NULL` მეხსიერების გამოყოფის შეცდომის შემთხვევაში.
- **@note:** დაბრუნებული `handle` უნდა გადაეცეს ასინქრონული ფუნქციის გამომძახებელს.

### `esp_err_t fmw_promise_resolve(promise_handle_t handle, void* result_data, void (*free_fn)(void*));`

ასრულებს `Promise`-ს წარმატებით და გადასცემს შედეგს. ეს გამოიწვევს მომხმარებლის `then` `callback`-ის გამოძახებას.

- **`handle`**: შესასრულებელი `Promise`-ის `handle`.
- **`result_data`**: (არასავალდებულო) მაჩვენებელი შედეგის მონაცემებზე. `Promise Manager`-ი ხდება ამ მონაცემების "მფლობელი".
- **`free_fn`**: (არასავალდებულო) ფუნქცია, რომელიც გამოიყენება `result_data`-ს გასათავისუფლებლად. თუ `NULL`-ია, გამოიყენება სტანდარტული `free()`.
- **აბრუნებს:** `ESP_OK` თუ `Promise` წარმატებით დაემატა შესრულების რიგში.

### `esp_err_t fmw_promise_reject(promise_handle_t handle, void* error_data, void (*free_fn)(void*));`

ასრულებს `Promise`-ს შეცდომით. ეს გამოიწვევს მომხმარებლის `catch` `callback`-ის გამოძახებას.

- **`handle`**: შესასრულებელი `Promise`-ის `handle`.
- **`error_data`**: (არასავალდებულო) მაჩვენებელი შეცდომის მონაცემებზე. `Promise Manager`-ი ხდება ამ მონაცემების "მფლობელი".
- **`free_fn`**: (არასავალდებულო) ფუნქცია `error_data`-ს გასათავისუფლებლად.
- **აბრუნებს:** `ESP_OK` თუ `Promise` წარმატებით დაემატა შესრულების რიგში.

---

## 💡 გამოყენების მაგალითი

იხილეთ [promise_pattern.md](../convention/promise_pattern.md) დეტალური გამოყენების მაგალითისთვის.
