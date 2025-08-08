# Synapse MQTT Manager Service API Reference

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის `MQTT Manager`-ის სტანდარტულ API-ს, naming და structure კონვენციების დაცვით.

---

## MQTT Manager API

ამჟამად, `mqtt_manager` მოდული არ აწვდის საჯარო Service API-ს სხვა მოდულებისთვის. მისი მთავარი როლია, იმოქმედოს როგორც "ხიდი" Synapse Framework-ის შიდა `Event Bus`-სა და გარე MQTT ბროკერს შორის.

მოდულის სრული მართვა და მასთან ურთიერთქმედება ხდება შემდეგი მექანიზმებით:

1. **შიდა ივენთების გამოწერა:** `mqtt_manager` ავტომატურად გამოიწერს სისტემურ ივენთებს (მაგ., `SYNAPSE_EVENT_WIFI_STATUS_READY`), რომელთა `payload`-საც შემდეგ აქვეყნებს შესაბამის MQTT თემებზე.
2. **ბრძანებების მიღება MQTT-ით:** ის გამოიწერს ერთ, ცენტრალიზებულ MQTT თემას (`.../cmd/in`) და მიღებულ ბრძანებებს გადაამისამართებს `Command Router`-ში `SYNAPSE_EVENT_EXECUTE_COMMAND_STRING` ივენთის საშუალებით.

ეს არქიტექტურა უზრუნველყოფს, რომ სხვა მოდულებს არ სჭირდებათ `mqtt_manager`-ის პირდაპირი გამოძახება.

### სამომავლო გაფართოება

თუ მომავალში გაჩნდება საჭიროება, რომ რომელიმე მოდულმა პირდაპირ, ივენთის გარეშე გამოაქვეყნოს შეტყობინება MQTT-ში, შესაძლებელია შემდეგი API-ს დამატება:

```c
// პოტენციური API სტრუქტურა
typedef struct
{
    /**
     * @brief პირდაპირ აქვეყნებს შეტყობინებას მითითებულ თემაზე.
     * @param[in] topic MQTT თემა.
     * @param[in] payload შეტყობინების შიგთავსი.
     * @param[in] qos Quality of Service დონე.
     * @param[in] retain Retain ფლაგი.
     */
    esp_err_t (*publish_message)(const char* topic, const char* payload, int qos, bool retain);
} mqtt_api_t;
```

**ამჟამად, ეს API არ არის იმპლემენტირებული.**

---

## Naming & Structure

- ყველა ფუნქცია და ცვლადი უნდა მიჰყვებოდეს [variable_naming.md](../convention/variable_naming.md) და [function_naming.md](../convention/function_naming.md) წესებს.
- იხილეთ [module_structure.md](../convention/module_structure.md) სრული სტრუქტურისთვის.

---

## მაგალითები

ამჟამად API არ არსებობს. მოდულთან ურთიერთქმედება ხდება ივენთებით.

**სტატუსის გამოქვეყნება (არაპირდაპირი გზა):**

```c
// 1. ნებისმიერი მოდული აქვეყნებს თავის სტატუსის ივენთს
//    (მაგ., wifi_manager აქვეყნებს SYNAPSE_EVENT_WIFI_STATUS_READY)
cJSON* status = cJSON_CreateObject();
// ... შეავსეთ status ...
char* json_str = cJSON_PrintUnformatted(status);
synapse_event_bus_post(SYNAPSE_EVENT_WIFI_STATUS_READY, wrapper);

// 2. mqtt_manager ავტომატურად იჭერს ამ ივენთს და აქვეყნებს MQTT-ში.
```

---

დამატებითი დეტალებისთვის იხილეთ [core_api.md](core_api.md), [module_api.md](module_api.md) და [structure.md](structure.md) დოკუმენტები.
