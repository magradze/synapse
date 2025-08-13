# Assertions და Guard Clauses კონვენცია

**სტატუსი:** საუკეთესო პრაქტიკა (Best Practice)
**ვერსია:** 1.0.0

## 1. 🎯 მიზანი

ეს დოკუმენტი განსაზღვრავს Synapse Framework-ში კოდის მდგრადობისა და გამართვის გასაუმჯობესებლად განკუთვნილი ორი ძირითადი პატერნის — **Assertions** (`SYNAPSE_ASSERT`) და **Guard Clauses** (`SYNAPSE_GUARD`) — გამოყენების სტანდარტულ კონვენციას.

ამ პატერნების სწორი გამოყენება კოდს ხდის უფრო უსაფრთხოს, თვით-დოკუმენტირებადს და მნიშვნელოვნად ამარტივებს ლოგიკური შეცდომების აღმოჩენას დეველოპმენტის ეტაპზე.

## 2. 🤔 ფუნდამენტური განსხვავება: Assertion vs. Error Handling

სანამ მაკროებს განვიხილავთ, კრიტიკულად მნიშვნელოვანია, გავიაზროთ განსხვავება ამ ორ კონცეფციას შორის:

- **Error Handling (შეცდომის დამუშავება):**
  - **რას აკეთებს?** ამუშავებს შეცდომებს, რომლებიც შეიძლება მოხდეს პროგრამის **ნორმალური მუშაობის დროს** (`runtime`). ეს არის მოსალოდნელი, თუმცა არასასურველი მოვლენები.
  - **მაგალითები:** `malloc`-მა ვერ გამოყო მეხსიერება, I2C მოწყობილობა არ პასუხობს, ფაილი ვერ მოიძებნა.
  - **ინსტრუმენტი:** `if (...) { ... return ESP_ERR_...; }` ბლოკები.
  - **შედეგი:** პროგრამა აკურატულად ამუშავებს შეცდომას და, როგორც წესი, **აგრძელებს მუშაობას**.

- **Assertion (მტკიცებულება):**
  - **რას აკეთებს?** ამოწმებს პირობებს, რომლებიც პროგრამისტის ლოგიკით **არასდროს, არანაირ ვითარებაში არ უნდა იყოს მცდარი**.
  - **მაგალითები:** ფუნქციას გადაეცა `NULL` მაჩვენებელი, სადაც ეს დაუშვებელია; მასივის ინდექსი გავიდა საზღვრებს გარეთ; `switch` ბლოკში შესრულდა `default` შემთხვევა, რომელიც ლოგიკურად მიუღწევადი უნდა იყოს.
  - **ინსტრუმენტი:** `SYNAPSE_ASSERT(...)`.
  - **შედეგი (`debug` რეჟიმში):** პროგრამა **დაუყოვნებლივ ჩერდება** (`panic`), რადგან მისი შიდა მდგომარეობა უკვე კორუმპირებულია და გაგრძელება სახიფათოა.

> **ოქროს წესი:** გამოიყენეთ `if` მოსალოდნელი შეცდომებისთვის და `SYNAPSE_ASSERT` — პროგრამული ბაგების გამოსავლენად.

## 3. `SYNAPSE_ASSERT` მაკრო

`SYNAPSE_ASSERT` არის მძლავრი ინსტრუმენტი დეველოპმენტის ფაზაში ლოგიკური შეცდომების მომენტალური იდენტიფიცირებისთვის.

### 3.1. როდის გამოვიყენოთ?

- ფუნქციის დასაწყისში, კრიტიკულად მნიშვნელოვანი არგუმენტების (`NULL` მაჩვენებლების) შესამოწმებლად.
- კოდის იმ ნაწილებში, რომლებიც ლოგიკურად მიუღწევადი უნდა იყოს.
- შიდა მდგომარეობის (internal state) ვალიდურობის შესამოწმებლად კრიტიკული ოპერაციის შესრულებამდე.

### 3.2. გამოყენების მაგალითები

**არგუმენტის ვალიდაცია:**

```c
void some_critical_function(module_t* self, user_data_t* data) {
    SYNAPSE_ASSERT(self != NULL, "Module handle cannot be NULL");
    SYNAPSE_ASSERT(data != NULL, "User data for module '%s' cannot be NULL", self->name);
    // ...
}
```

**მიუღწევადი კოდის ბლოკი:**

```c
switch (state) {
    case STATE_A: /* ... */ break;
    case STATE_B: /* ... */ break;
    default:
        // თუ ყველა state დაფარულია, აქ არასდროს უნდა მოვხვდეთ.
        SYNAPSE_ASSERT(false, "Unhandled state: %d", state);
        break;
}
```

### 3.3. ქცევა `release` რეჟიმში

`menuconfig`-ში `CONFIG_SYNAPSE_ENABLE_ASSERTS`-ის გამორთვისას, `SYNAPSE_ASSERT` მაკრო გარდაიქმნება ცარიელ ოპერაციად (`(void)0`) და **სრულად ქრება** საბოლოო firmware-იდან. ის არანაირ გავლენას არ ახდენს წარმადობაზე.

## 4. `SYNAPSE_GUARD` მაკროები

`SYNAPSE_GUARD` და `SYNAPSE_GUARD_VOID` არის "სინტაქსური შაქარი", რომელიც ამარტივებს `Error Handling`-ის `if` ბლოკების წერას და კოდს უფრო კომპაქტურს ხდის.

### 4.1. როდის გამოვიყენოთ?

- ფუნქციის დასაწყისში, `runtime` პირობების სწრაფი შემოწმებისთვის.
- როდესაც შეცდომის შემთხვევაში საჭიროა მხოლოდ ლოგის დაბეჭდვა და ფუნქციიდან დაუყოვნებლივ გამოსვლა.

### 4.2. გამოყენების მაგალითები

**ფუნქცია, რომელიც აბრუნებს `esp_err_t`-ს:**

```c
esp_err_t process_data(void* handle, int size) {
    SYNAPSE_GUARD(handle != NULL, TAG, ESP_ERR_INVALID_ARG, "Input handle is NULL");
    SYNAPSE_GUARD(size > 0, TAG, ESP_ERR_INVALID_ARG, "Data size must be positive, got %d", size);

    // ...
    return ESP_OK;
}
```

**ფუნქცია, რომელიც აბრუნებს `void`-ს:**

```c
void update_display(module_t* self) {
    SYNAPSE_GUARD_VOID(self != NULL, TAG, "Module handle is NULL");
    
    ui_private_data_t* private_data = (ui_private_data_t*)self->private_data;
    SYNAPSE_GUARD_VOID(private_data->is_initialized, TAG, "Cannot update display, not initialized");

    // ...
}
```

### 4.3. როდის **არ** გამოვიყენოთ `SYNAPSE_GUARD`?

**არ გამოიყენოთ** `SYNAPSE_GUARD`, თუ შეცდომის დამუშავება მოითხოვს **ერთზე მეტ მოქმედებას**. მაგალითად, თუ შეცდომისას საჭიროა `mutex`-ის გათავისუფლება ან მეხსიერების გასუფთავება, გამოიყენეთ სტანდარტული `if` ბლოკი.

**ცუდი მაგალითი (არ გააკეთოთ):**

```c
// არასწორია, რადგან mutex-ი დარჩება დაკავებული
xSemaphoreTake(mutex, portMAX_DELAY);
SYNAPSE_GUARD(result == SUCCESS, TAG, ESP_FAIL, "Operation failed"); 
xSemaphoreGive(mutex);
```

**სწორი მაგალითი:**

```c
xSemaphoreTake(mutex, portMAX_DELAY);
if (result != SUCCESS) {
    ESP_LOGE(TAG, "Operation failed");
    xSemaphoreGive(mutex); // აუცილებლად ვათავისუფლებთ რესურსს
    return ESP_FAIL;
}
xSemaphoreGive(mutex);
```
