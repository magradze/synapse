# Synapse WiFi Manager Service API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის `WiFi Manager`-ის სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## WiFi Manager API

ამჟამად, `wifi_manager` მოდული არ აწვდის საჯარო Service API-ს სხვა მოდულებისთვის. მისი სრული მართვა და სტატუსის მიღება ხდება `Event Bus`-ზე გამოქვეყნებული ივენთების და `Command Router`-ის მეშვეობით რეგისტრირებული CLI ბრძანებების საშუალებით.

ეს არქიტექტურული გადაწყვეტილება ხელს უწყობს მოდულებს შორის მაქსიმალურ იზოლაციას.

### სამომავლო გაფართოება

თუ მომავალში გაჩნდება საჭიროება, რომ სხვა მოდულმა სინქრონულად მოითხოვოს WiFi-ს სტატუსი, შესაძლებელია შემდეგი API-ს დამატება:

```c
// პოტენციური API სტრუქტურა
typedef struct
{
    /**
     * @brief სინქრონულად აბრუნებს WiFi-ს სტატუსს JSON ფორმატში.
     * @param[out] out_json მაჩვენებელი, რომელშიც ჩაიწერება cJSON ობიექტის მისამართი.
     * @note გამომძახებელმა უნდა გაათავისუფლოს out_json cJSON_Delete-ით.
     */
    esp_err_t (*get_status_json)(cJSON **out_json);

    /**
     * @brief სინქრონულად ამოწმებს, არის თუ არა მოწყობილობა დაკავშირებული ქსელთან.
     * @return true თუ დაკავშირებულია, false წინააღმდეგ შემთხვევაში.
     */
    bool (*is_connected)(void);
} wifi_api_t;
```

---

## Naming & Structure

- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

ამჟამად API არ არსებობს. მოდულის მართვა ხდება ივენთებით და CLI ბრძანებებით.

**სტატუსის მოთხოვნა (არაპირდაპირი გზა):**

```c
// 1. გააგზავნეთ "wifi status" ბრძანება Event Bus-ით
fmw_command_payload_t* payload = ...;
// ... შეავსეთ payload ...
fmw_event_bus_post(FMW_EVENT_EXECUTE_COMMAND_STRING, wrapper);

// 2. დაელოდეთ FMW_EVENT_WIFI_STATUS_READY ივენთს თქვენი მოდულის handle_event-ში
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure.md](structure.md) დოკუმენტები.
