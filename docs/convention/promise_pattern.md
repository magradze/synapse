# asynchronous ოპერაციების მართვის პატერნი (Promise Pattern)

**სტატუსი:** რეკომენდებული (Recommended)
**ავტორი:** Synapse Framework Team

## 1. 🎯 მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ში ასინქრონული "მოთხოვნა-პასუხის" (Request-Response) ოპერაციების მართვის სტანდარტულ პატერნს, რომელიც დაფუძნებულია **Promise** (დაპირების) კონცეფციაზე. ამ პატერნის მიზანია, ჩაანაცვლოს რთული, ივენთებზე დაფუძნებული ლოგიკა უფრო სუფთა, წაკითხვადი და ლოგიკურად გამართული კოდით.

ეს პატერნი იდეალურია იმ სცენარებისთვის, როდესაც ერთი მოდული (მომხმარებელი, Consumer) აგზავნის მოთხოვნას მეორე მოდულთან (სერვისი, Provider) და ელოდება კონკრეტულ, ერთჯერად პასუხს.

## 2. 🤔 პრობლემა: რატომ გვჭირდება Promise-ები?

წარმოვიდგინოთ კლასიკური სცენარი: `ui_manager`-ს სჭირდება `wifi_manager`-ისგან მიმდინარე სტატუსის მიღება. `Event Bus`-ის გამოყენებით, პროცესი ასე გამოიყურება:

1. `ui_manager` აქვეყნებს `EXECUTE_COMMAND` ივენთს `payload`-ით `"wifi status --silent"`.
2. `wifi_manager` იღებს ამ ბრძანებას, ამუშავებს მას და აქვეყნებს პასუხს **სხვა** ივენთით: `FMW_EVENT_WIFI_STATUS_READY`.
3. `ui_manager`-ს აქვს ცალკე ლოგიკა `handle_event` ფუნქციაში, რომელიც უსმენს `FMW_EVENT_WIFI_STATUS_READY` ივენთს და ამუშავებს პასუხს.

**სისუსტეები:**

- **გაფანტული ლოგიკა:** მოთხოვნის გაგზავნა და პასუხის მიღება ხდება კოდის სხვადასხვა, ერთმანეთთან ლოგიკურად დაუკავშირებელ ნაწილებში.
- **კონტექსტის დაკარგვა:** რთულია იმის მიკვლევა, თუ რომელი მოთხოვნა რომელ პასუხს შეესაბამება.
- **"ხმაური" `Event Bus`-ზე:** `FMW_EVENT_WIFI_STATUS_READY` იგზავნება ყველასთან, მაშინაც კი, თუ ის მხოლოდ `ui_manager`-ს სჭირდებოდა.

## 3. 💡 გადაწყვეტა: Promise პატერნი

Promise პატერნი ამ პროცესს გარდაქმნის. `Promise` არის ობიექტი, რომელიც წარმოადგენს **მომავალში დასრულებადი ოპერაციის შედეგს**.

**ახალი სამუშაო პროცესი:**

1. `ui_manager` იძახებს `wifi_manager`-ის API ფუნქციას: `wifi_api_get_status_async()`.
2. `wifi_manager` მომენტალურად აბრუნებს `promise_handle_t` ობიექტს (ცარიელ "ყუთს").
3. `ui_manager` ამ "ყუთზე" არეგისტრირებს ორ `callback` ფუნქციას:
    - `then()`: რა უნდა მოხდეს, თუ ოპერაცია **წარმატებით** დასრულდება.
    - `catch()`: რა უნდა მოხდეს, თუ ოპერაცია **შეცდომით** დასრულდება.
4. როდესაც `wifi_manager`-ი დაასრულებს სტატუსის მიღებას, ის "ავსებს" ამ "ყუთს" შედეგით, რაც ავტომატურად იწვევს `ui_manager`-ის მიერ დარეგისტრირებული `then()` `callback`-ის შესრულებას.

**სარგებელი:**

- **ლოგიკური კავშირი:** მოთხოვნა, წარმატების ლოგიკა და შეცდომის ლოგიკა თავმოყრილია ერთ, კომპაქტურ კოდის ბლოკში.
- **წაკითხვადობა:** კოდი ხდება ბევრად უფრო ინტუიციური და ადვილად გასაგები.
- **მიზანმიმართული კომუნიკაცია:** პასუხი მიეწოდება მხოლოდ იმას, ვინც ის მოითხოვა.

## 4. ⚙️ როგორ მუშაობს იმპლემენტაცია?

ფრეიმვორქის ბირთვში არსებობს **`Promise Manager`** კომპონენტი, რომელიც ამ პროცესს მართავს:

- **`fmw_promise_create()`:** სერვისი (მაგ., `wifi_manager`) იძახებს ამ ფუნქციას, რათა შექმნას ახალი, `PENDING` (მომლოდინე) მდგომარეობაში მყოფი `Promise`.
- **`fmw_promise_then()` / `fmw_promise_catch()`:** მომხმარებელი (მაგ., `ui_manager`) ამ ფუნქციებით არეგისტრირებს თავის `callback`-ებს.
- **`fmw_promise_resolve()` / `fmw_promise_reject()`:** როდესაც სერვისი დაასრულებს ოპერაციას, ის იძახებს ამ ფუნქციებიდან ერთ-ერთს.
- **`Promise Manager Task`:** `resolve` ან `reject` გამოძახებისას, `Promise Manager`-ი შესაბამის `Promise`-ს აგზავნის თავის შიდა რიგში. მისი გამოყოფილი FreeRTOS ტასკი იღებს ამ `Promise`-ს და უსაფრთხო, იზოლირებულ კონტექსტში იძახებს მომხმარებლის მიერ დარეგისტრირებულ `callback` ფუნქციას.

## 5. 📝 გამოყენების პატერნი

### 5.1. სერვისის მომწოდებლის მხარე (Provider - მაგ., `wifi_manager`)

1. **API-ს დეკლარირება (`wifi_interface.h`):**

    ```c
    #include "promise_manager.h" // საჭიროა promise_handle_t-სთვის

    typedef struct {
        // ... სხვა ფუნქციები ...
        promise_handle_t (*get_status_async)(void* context);
    } wifi_api_t;
    ```

2. **API-ს იმპლემენტაცია (`wifi_manager.c`):**

    ```c
    #include "promise_manager_internal.h" // საჭიროა fmw_promise_create-სთვის

    static promise_handle_t wifi_api_get_status_async(void* context) {
        // 1. შევქმნათ ახალი "დაპირება"
        promise_handle_t promise = fmw_promise_create();
        if (!promise) {
            return NULL;
        }

        // 2. გავაგზავნოთ ბრძანება ჩვენს შიდა ტასკში,
        //    თან გადავცეთ promise-ის handle.
        wifi_internal_cmd_t cmd = {
            .type = CMD_GET_STATUS,
            .promise_handle = promise
        };
        xQueueSend(private_data->command_queue, &cmd, 0);

        // 3. დავაბრუნოთ "დაპირება" მომხმარებელს
        return promise;
    }
    ```

3. **Promise-ის შესრულება (`wifi_task`-ში):**

    ```c
    // wifi_task-ში, როდესაც სტატუსი მზად იქნება...
    wifi_status_t* status_data = malloc(sizeof(wifi_status_t));
    // ... შევავსოთ status_data ...

    // შევასრულოთ "დაპირება"
    fmw_promise_resolve(cmd.promise_handle, status_data, free);
    ```

### 5.2. მომხმარებლის მხარე (Consumer - მაგ., `ui_manager`)

```c
// ui_manager.c

// Callback ფუნქცია წარმატებისთვის
static void on_wifi_status_ready(void* result_data, void* user_context) {
    wifi_status_t* status = (wifi_status_t*)result_data;
    ESP_LOGI(TAG, "Promise resolved! WiFi RSSI: %d", status->rssi);
    // აქ მოხდება UI-ს განახლება
}

// Callback ფუნქცია შეცდომისთვის
static void on_wifi_status_failed(void* error_data, void* user_context) {
    ESP_LOGE(TAG, "Promise rejected! Could not get WiFi status.");
}

// მოთხოვნის გაგზავნა
void request_wifi_update() {
    promise_handle_t p = private_data->wifi_service->api->get_status_async(private_data->wifi_service->context);
    
    if (p) {
        fmw_promise_then(p, on_wifi_status_ready, private_data);
        fmw_promise_catch(p, on_wifi_status_failed, private_data);
    }
}
```

## 6. ✅ როდის გამოვიყენოთ Promise და როდის Event Bus?

| სცენარი | რეკომენდებული პატერნი | რატომ? |
| :--- | :--- | :--- |
| ერთი მოდული ითხოვს მონაცემს მეორისგან და ელოდება **კონკრეტულ პასუხს**. | **Promise** | პირდაპირი, ლოგიკური კავშირი მოთხოვნასა და პასუხს შორის. |
| ერთი მოდული აქვეყნებს **ზოგად ინფორმაციას** (მაგ., "WiFi გაითიშა"), რომელიც შეიძლება **ბევრმა** (ან არცერთმა) მოდულმა მოისმინოს. | **Event Bus** | მაქსიმალური იზოლაცია. გამომგზავნმა არ იცის, ვინ უსმენს. |
| საჭიროა ოპერაციის შედეგის (წარმატება/შეცდომა) დამუშავება ერთ კოდის ბლოკში. | **Promise** | `then()` და `catch()` უზრუნველყოფს სტრუქტურირებულ დამუშავებას. |
| სისტემური, "Broadcast" ტიპის შეტყობინებები (მაგ., `SYSTEM_START_COMPLETE`). | **Event Bus** | იდეალურია ფართო აუდიტორიისთვის ინფორმაციის გასავრცელებლად. |
