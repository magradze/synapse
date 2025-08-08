# რესურსების ოპტიმიზაციის პატერნი (Shared Task Pool)

**სტატუსი:** საუკეთესო პრაქტიკა (Best Practice)
**ავტორი:** Synapse Framework Team

## 1. 🎯 მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ში **რესურსების ოპტიმიზაციის** სტანდარტულ პატერნს, რომელიც დაფუძნებულია **`Task Pool Manager`** (გაზიარებული ტასკების მენეჯერი) სერვისის გამოყენებაზე. ამ პატერნის მიზანია, რადიკალურად შეამციროს სისტემის მიერ მოხმარებული ოპერატიული მეხსიერება (RAM) და გააუმჯობესოს საერთო წარმადობა.

ეს კონვენცია განსაკუთრებით მნიშვნელოვანია იმ მოდულებისთვის, რომლებიც ასრულებენ **მცირე, პერიოდულ, არაბლოკირებად** სამუშაოს.

## 2. 🤔 პრობლემა: "ერთი მოდული = ერთი ტასკი" ანტი-პატერნი

ჩაშენებულ სისტემებში, განსაკუთრებით ESP32-ზე, მეხსიერება შეზღუდული რესურსია. გავრცელებული, მაგრამ არაეფექტური მიდგომაა, როდესაც თითოეული მოდული, რომელსაც ფონური სამუშაო სჭირდება, ქმნის საკუთარ FreeRTOS ტასკს.

**პრობლემის არსი:**

- **მეხსიერების დიდი დანახარჯი:** თითოეული `xTaskCreate` გამოძახება რეზერვს უკეთებს სტეკის მეხსიერებას (როგორც წესი, 2-4 KB). 10 სენსორის მოდული, რომელთაგან თითოეულს თავისი ტასკი აქვს, მარტივად მოიხმარს **20-40 KB** RAM-ს მხოლოდ სტეკებისთვის.
- **წარმადობის შემცირება:** რაც მეტია ტასკი, მით მეტ დროს ხარჯავს FreeRTOS-ის `scheduler`-ი მათ შორის გადართვაზე (Context Switching), რაც ამცირებს სასარგებლო სამუშაოს შესრულების დროს.
- **არაეფექტურობა:** ამ ტასკების უმეტესობა 99% დროის განმავლობაში "იძინებს" (`vTaskDelay`) და უბრალოდ ადგილს იკავებს მეხსიერებაში.

## 3. 💡 გადაწყვეტა: `Task Pool Manager`

Synapse Framework-ი ამ პრობლემას ჭრის `Task Pool Manager` `core` სერვისის მეშვეობით. ეს არის "მუშა ტასკების" (Worker Tasks) ერთობლიობა, რომელიც მზად არის, შეასრულოს ნებისმიერი "დავალება" (`Job`), რომელსაც მას მოდულები მიაწვდიან.

**პატერნის ლოგიკა:**

1. მოდული, რომელსაც სჭირდება პერიოდული სამუშაოს შესრულება, **არ ქმნის საკუთარ ტასკს**.
2. `_start` ფუნქციაში, ის იძახებს `synapse_task_pool_schedule_job()`-ს.
3. ამ გამოძახებისას, ის გადასცემს:
    - **`job_function`:** ფუნქციის მაჩვენებელს, რომელმაც უნდა შეასრულოს სამუშაო (მაგ., სენსორის წაკითხვა). **მნიშვნელოვანია:** ეს ფუნქცია არ უნდა შეიცავდეს `while(1)` ციკლს ან `vTaskDelay`-ს.
    - **`context`:** მაჩვენებელს თავის `private_data`-ზე.
    - **`interval_ms`:** გამოძახების პერიოდს მილიწამებში.
    - **`is_periodic`:** `true`.
4. `Task Pool Manager`-ი თავად ზრუნავს ამ ფუნქციის პერიოდულად, მითითებული ინტერვალით გამოძახებაზე თავისი ერთ-ერთი "მუშა" ტასკის კონტექსტიდან.
5. `_deinit` ფუნქციაში, მოდული იძახებს `synapse_task_pool_cancel_job()`-ს, რათა გააუქმოს დაგეგმილი სამუშაო.

## 4. 📝 გამოყენების პატერნი

### 4.1. მოდულის `private_data` სტრუქტურა

```c
// my_sensor.c
typedef struct {
    // ... სხვა ველები ...
    synapse_job_handle_t job_handle; // ვინახავთ handle-ს სამუშაოს გასაუქმებლად
} my_sensor_private_data_t;
```

### 4.2. სამუშაო ფუნქციის (`Job`) იმპლემენტაცია

ეს ფუნქცია არ შეიცავს `while` ციკლს.

```c
// my_sensor.c
static void my_sensor_poll_job(void *user_context)
{
    module_t *self = (module_t *)user_context;
    my_sensor_private_data_t *private_data = (my_sensor_private_data_t *)self->private_data;

    // 1. ვკითხულობთ სენსორს
    float temperature = read_temperature_from_hardware();

    // 2. ვაქვეყნებთ ივენთს
    // ... (synapse_event_bus_post logic) ...

    ESP_LOGD(TAG, "Sensor poll job completed for '%s'.", self->name);
}
```

### 4.3. `_start` და `_deinit` ფუნქციები

```c
// my_sensor.c
static esp_err_t my_sensor_start(module_t *self)
{
    my_sensor_private_data_t *private_data = (my_sensor_private_data_t *)self->private_data;
    ESP_LOGI(TAG, "Scheduling sensor poll job.");

    private_data->job_handle = synapse_task_pool_schedule_job(
        my_sensor_poll_job, // შესასრულებელი ფუნქცია
        self,               // კონტექსტი (მოდულის handle)
        30000,              // ინტერვალი (30 წამი)
        true                // პერიოდული
    );

    if (private_data->job_handle == NULL) {
        ESP_LOGE(TAG, "Failed to schedule sensor poll job.");
        return ESP_FAIL;
    }

    self->status = MODULE_STATUS_RUNNING;
    return ESP_OK;
}

static void my_sensor_deinit(module_t *self)
{
    // ...
    my_sensor_private_data_t *private_data = (my_sensor_private_data_t *)self->private_data;
    if (private_data && private_data->job_handle) {
        synapse_task_pool_cancel_job(private_data->job_handle);
    }
    // ...
}
```

## 5. ✅ როდის გამოვიყენოთ `Task Pool` და როდის საკუთარი ტასკი?

| სცენარი | რეკომენდებული მიდგომა | რატომ? |
| :--- | :--- | :--- |
| მოდული ასრულებს მოკლე, პერიოდულ, **არაბლოკირებად** სამუშაოს (მაგ., სენსორის წაკითხვა, GPIO-ს შემოწმება). | **Task Pool** | რესურსების მაქსიმალური ოპტიმიზაცია. |
| მოდული ასრულებს ხანგრძლივ, **ბლოკირებად** ოპერაციებს (მაგ., ქსელური მოთხოვნა, ფაილური ოპერაცია). | **საკუთარი ტასკი** | `Task Pool`-ის "მუშა" ტასკის დიდი ხნით დაკავება დააბლოკავდა სხვა მოდულების სამუშაოებს. |
| მოდულს სჭირდება საკუთარი, კომპლექსური `while` ციკლი და მდგომარეობის მართვა (მაგ., `ui_manager`, `command_router`). | **საკუთარი ტასკი** | `Task Pool`-ი განკუთვნილია "უსახო" (stateless) სამუშაოებისთვის. |
| მოდულს სჭირდება გარანტირებული, მაღალი პრიორიტეტის შესრულება. | **საკუთარი ტასკი** | `Task Pool`-ის ტასკებს აქვთ საშუალო, ზოგადი პრიორიტეტი. |
