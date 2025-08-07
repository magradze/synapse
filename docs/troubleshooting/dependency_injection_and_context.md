# პრობლემა: როგორ გადავცეთ კონტექსტი სერვისის API-ს გამოძახებისას?

## 1. ❓ პრობლემის აღწერა

თქვენ ქმნით მოდულს (მაგ., `Module A`), რომელიც იყენებს სხვა მოდულის (`Module B`) სერვისს `Dependency Injection`-ის (DI) მეშვეობით. `Module B`-ს Service API (`module_b_api_t`) ფუნქციები პირველ პარამეტრად იღებენ `void* context`-ს, რომელიც სინამდვილეში `Module B`-ს `module_t*` ობიექტის მისამართი უნდა იყოს.

თქვენს `Module A`-ში, DI-ს მეშვეობით, წარმატებით მიიღეთ `module_b_api_t*` ობიექტი, მაგრამ არ გაქვთ წვდომა `Module B`-ს `module_t*` ობიექტზე, რომელიც კონტექსტად უნდა გადასცეთ.

**ტიპური შეცდომა:**

```c
// In Module A
// private_data->service_b_api არის module_b_api_t*

// როგორ მივიღო "context" ამ გამოძახებისთვის?
private_data->service_b_api->do_something(CONTEXT_IS_MISSING, ...);
```

## 2. ❌ არასწორი (მაგრამ მაცდური) გადაწყვეტები

1. **`Hardcoded` სახელით მოძიება:**

    ```c
    module_t* module_b_handle = fmw_module_registry_find_by_name("instance_name_of_b");
    private_data->service_b_api->do_something(module_b_handle, ...);
    ```

    - **რატომ არის ცუდი?** ეს ქმნის `hardcoded` დამოკიდებულებას კონკრეტულ `instance_name`-ზე. თუ მომხმარებელი `config.json`-ში სახელს შეუცვლის, თქვენი კოდი გატყდება. ეს არღვევს ფრეიმვორქის მოქნილობის პრინციპს.

## 3. ✅ სწორი არქიტექტურული პატერნი: ორმაგი ინექცია

Synapse Framework-ის DI მექანიზმი შექმნილია ამ პრობლემის ელეგანტურად გადასაჭრელად. გამოსავალია, მოითხოვოთ **ორივე რესურსის** ინექცირება: როგორც სერვისის API-ს, ისე თავად მოდულის `handle`-ის.

### ეტაპი 1: `private_data` სტრუქტურის გაფართოება

თქვენი მოდულის (`Module A`) `private_data` სტრუქტურაში დაამატეთ ორივე მაჩვენებელი:

```c
// module_a.h
typedef struct {
    // ... სხვა ველები ...
    
    // --- Injected Dependencies ---
    module_b_api_t* service_b_api;      // სერვისის API-სთვის
    module_t*       service_b_handle;   // მოდულის handle-ისთვის (კონტექსტი)

} module_a_private_data_t;
```

### ეტაპი 2: `dependency_map`-ის განსაზღვრა

თქვენი მოდულის `.c` ფაილში, აღწერეთ ორივე დამოკიდებულება `dependency_map`-ში:

```c
// module_a.c
static const module_dependency_t s_dependencies[] = {
    {"service_b_api_name", offsetof(module_a_private_data_t, service_b_api)},
    {"service_b_handle_name", offsetof(module_a_private_data_t, service_b_handle)},
    {NULL, 0}
};
```

### ეტაპი 3: `config.json`-ის კონფიგურაცია

თქვენი მოდულის `config.json`-ში, მიუთითეთ, თუ რომელი სერვისი და რომელი მოდული უნდა იქნას ინექცირებული. ხშირ შემთხვევაში, მათი `instance_name` ერთმანეთს ემთხვევა.

```json
// module_a/config.json
{
    "type": "module_a",
    "config": {
        "instance_name": "main_a",
        "service_b_api_name": "main_b",
        "service_b_handle_name": "main_b"
    }
}
```

### ეტაპი 4: გამოყენება

ახლა, თქვენს კოდში შეგიძლიათ უსაფრთხოდ და `hardcode`-ის გარეშე გამოიძახოთ სერვისის ფუნქცია:

```c
// module_a.c
void do_something_with_service_b(module_a_private_data_t* private_data) {
    if (private_data->service_b_api && private_data->service_b_handle) {
        private_data->service_b_api->do_something(
            private_data->service_b_handle, // <-- გადავეცით ინექცირებული კონტექსტი
            ...
        );
    }
}
```

## 4. 💬 დასკვნა

მიუხედავად იმისა, რომ `module_t*` ტიპის პირდაპირი შენახვა მცირე დამოკიდებულებას ქმნის ფრეიმვორქის საბაზისო სტრუქტურაზე, ეს არის **გამართლებული და პრაგმატული კომპრომისი**. ის საშუალებას გვაძლევს, შევინარჩუნოთ API-ების მოქნილობა (`context`-ის გადაცემის შესაძლებლობა) და ამავდროულად, თავიდან ავიცილოთ `hardcoded` დამოკიდებულებები, რაც ფრეიმვორქის ერთ-ერთი მთავარი პრინციპია.
