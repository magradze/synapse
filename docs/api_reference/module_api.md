# Synapse Module API Reference

## მოდულის შექმნა და სიცოცხლის ციკლი

### `module_t *{module_name}_create(const cJSON *config);`

- **როლი:** ქმნის მოდულის ახალ ინსტანციას. ეს არის მოდულის "კონსტრუქტორი".
- **არგუმენტები:** `config` - მაჩვენებელი მოდულის სრულ კონფიგურაციის cJSON ობიექტზე (`{"type": ..., "config": ...}`).
- **მფლობელობა (Ownership):** **(მნიშვნელოვანი!)** ეს ფუნქცია პასუხისმგებელია, შექმნას გადაცემული `config` ობიექტის **დამოუკიდებელი ასლი** `cJSON_Duplicate()`-ის გამოყენებით და შეინახოს ის `module->current_config`-ში.
- **აბრუნებს:** `module_t*` წარმატების შემთხვევაში, `NULL` შეცდომისას.

### `static esp_err_t {module_name}_init(module_t *self);`

- **როლი:** ახორციელებს მოდულის ინიციალიზაციას (რესურსების დაკავება, სერვისების რეგისტრაცია, ივენთებზე გამოწერა).
- **გამოიძახება:** `System Manager`-ის მიერ, `start` ფაზამდე.

### `static esp_err_t {module_name}_start(module_t *self);`

- **როლი:** იწყებს მოდულის აქტიურ მუშაობას (მაგ., FreeRTOS ტასკის გაშვება).
- **გამოიძახება:** `System Manager`-ის მიერ, `init` ფაზის შემდეგ.

### `static void {module_name}_deinit(module_t *self);`

- **როლი:** ასუფთავებს მოდულის მიერ გამოყოფილ **ყველა შიდა რესურსს**.
- **მფლობელობა (Ownership):** **(მნიშვნელოვანი!)** ამ ფუნქციამ უნდა გაათავისუფლოს `self->current_config`-ის (რომელიც `_create`-ში დაკოპირდა) და `self->private_data`-ს მეხსიერება. მან **არ უნდა** გამოიძახოს `free(self)`, რადგან ამაზე `System Manager`-ი ზრუნავს.
- **გამოიძახება:** `System Manager`-ის მიერ, "Graceful Shutdown" პროცესის დროს.

---

## Service API (Service Locator-ისთვის)

ეს სექცია აღწერს იმ ფუნქციებს, რომლებსაც მოდული არეგისტრირებს `Service Locator`-ში, რათა სხვა მოდულებმა შეძლონ მათი პირდაპირი გამოძახება.

### `esp_err_t {module_name}_api_{action}(...);`

- **მაგალითი:** `esp_err_t relay_api_set_state(void* context, bool is_on);`
- **კონტექსტი:** რეკომენდებულია, რომ API ფუნქციებმა მიიღონ `void* context` პარამეტრი, რათა შეძლონ მუშაობა მოდულის მრავალ ინსტანციასთან.

---

## Event Handling API

### `static void {module_name}_handle_event(module_t *self, const char *event_name, void *event_data);`

- **როლი:** ამუშავებს `Event Bus`-იდან მიღებულ მოვლენებს.
- **არგუმენტები:**
  - `self`: მაჩვენებელი მოდულის ინსტანციაზე.
  - `event_name`: მიღებული ივენთის სახელი (სტრიქონი).
  - `event_data`: მაჩვენებელი `event_data_wrapper_t` სტრუქტურაზე.
- **მეხსიერების მართვა:** **(მნიშვნელოვანი!)** ამ ფუნქციის დასრულებისას, თუ `event_data` არ არის `NULL`, აუცილებელია `synapse_event_data_release((event_data_wrapper_t *)event_data);`-ის გამოძახება.

---

## Configuration Parsing API

### `static esp_err_t parse_{module_name}_config(const cJSON *config_node, {module_name}_private_data_t *p_data);`

- **როლი:** კითხულობს და ამოწმებს მოდულის კონფიგურაციას.
- **არგუმენტები:** `config_node` - მაჩვენებელი `config` ობიექტის შიგთავსზე (და არა სრულ ობიექტზე).

---

## Status & Monitoring API

### `static module_status_t {module_name}_get_status(module_t *self);`

- **როლი:** აბრუნებს მოდულის მიმდინარე სტატუსს (`module_status_t`).

---

## Naming & Structure

- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

### `static void {module_name}_debug_print(module_t *self);`

- **როლი:** (არასავალდებულო) ბეჭდავს მოდულის დეტალურ, დიაგნოსტიკურ ინფორმაციას კონსოლში.
- **გამოიძახება:** `Command Router`-ის მიერ, როდესაც მომხმარებელი შეასრულებს `modules --info <instance_name>` ბრძანებას.
- **იმპლემენტაცია:** ამ ფუნქციამ უნდა დაბეჭდოს `private_data` სტრუქტურის მნიშვნელოვანი ველები (სტატუსი, კონფიგურაციის პარამეტრები, შიდა მდგომარეობის ცვლადები) ადამიანისთვის გასაგებ ფორმატში.

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md) და [structure] დოკუმენტები.
