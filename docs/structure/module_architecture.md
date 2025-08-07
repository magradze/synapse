# მოდულების არქიტექტურა (Module Architecture)

## ძირითადი პრინციპები

- **მოდულურობა:** ყველა ფუნქციონალი წარმოდგენილია დამოუკიდებელი მოდულების სახით, რომლებიც შეიძლება ჩაერთოს ან გამოირთოს `Kconfig`-ის მეშვეობით.
- **იზოლაცია:** მოდულებს შორის პირდაპირი დამოკიდებულებები აკრძალულია. კომუნიკაცია ხდება მხოლოდ `Service Locator`-ის ან `Event Bus`-ის მეშვეობით.
- **სტანდარტიზაცია:** ყველა მოდული მიჰყვება ერთიან naming, structure და communication სტანდარტებს.
- **გაფართოებადობა:** ახალი მოდულის დამატება შესაძლებელია მინიმალური ძალისხმევით, `create_module.py` სკრიპტის გამოყენებით.

## მოდულის ტიპები

1. **Service Provider Module:**
    - აწვდის API-ს `Service Locator`-ის მეშვეობით.
    - მაგ: `rgb_led_indicator`, `storage_manager`, `system_timer`.
2. **Event-driven Module:**
    - ავრცელებს ან იღებს მოვლენებს `Event Bus`-ის მეშვეობით.
    - მაგ: `sensor_aggregator`, `alarms_manager`.
3. **Hybrid Module:**
    - იყენებს ორივე პატერნს. არის როგორც სერვისის მომწოდებელი, ისე ივენთებზე დამოკიდებული.
    - მაგ: `wifi_manager` (იღებს ივენთებს, მაგრამ მომავალში შეიძლება API-ც ჰქონდეს), `rgb_led_indicator` (იღებს ივენთებს და აწვდის API-ს).

## მოდულების ურთიერთქმედება

- **Service Locator:** გამოიყენება კონკრეტული API-ს მისაღებად და პირდაპირი, სინქრონული ფუნქციური გამოძახებისთვის.
- **Event Bus:** გამოიყენება broadcast/notification სცენარებისთვის, ასინქრონული კომუნიკაციისთვის.
- **აკრძალულია:** პირდაპირი `#include` სხვა მოდულის ჰედერების.

## მოდულის სიცოცხლის ციკლი (Lifecycle)

1. **Create:** `Module Factory` იძახებს მოდულის `_create(config)` ფუნქციას. მოდული იღებს თავისი კონფიგურაციის ობიექტის **მფლობელობას**.
2. **Init:** `System Manager` იძახებს `init(self)` ფუნქციას. აქ ხდება რესურსების დაკავება, სერვისების რეგისტრაცია, ივენთებზე გამოწერა.
3. **Start:** `System Manager` იძახებს `start(self)` ფუნქციას. აქ იწყება მოდულის აქტიური მუშაობა (მაგ., ტასკის გაშვება).
4. **Deinit:** `System Manager` (მომავალში) ან სისტემის გამორთვისას გამოიძახება `deinit(self)` ფუნქცია. აქ ხდება ყველა გამოყოფილი რესურსის (მათ შორის კონფიგურაციის ობიექტის) გათავისუფლება.

## მოდულის სტრუქტურის მაგალითი

```plaintext
components/modules/displays/rgb_led_indicator/
├── CMakeLists.txt
├── Kconfig
├── module.json
├── config.json         # <--- Default runtime კონფიგურაცია
├── README.md
├── include/
│   └── rgb_led_indicator.h
└── src/
    └── rgb_led_indicator.c
```

### `_create` ფუნქციის კონტრაქტი

ყველა მოდულის `_create` ფუნქციამ უნდა დაიცვას მეხსიერების მართვის ახალი წესი.

```c
// src/rgb_led_indicator.c-ში
module_t *rgb_led_indicator_create(const cJSON *config) {
    // 1. მეხსიერების გამოყოფა
    module_t *module = (module_t *)calloc(1, sizeof(module_t));
    // ...

    // 2. კონფიგურაციის ობიექტის მფლობელობის აღება
    module->current_config = (cJSON*)config;

    // 3. კონფიგურაციის პარსინგი
    const cJSON *config_node = cJSON_GetObjectItem(config, "config");
    // ...

    // 4. base ფუნქციების დაყენება
    module->base.init = rgb_led_indicator_init;
    module->base.deinit = rgb_led_indicator_deinit;
    // ...

    return module;
}
```

## მოდულის გაფართოება/დამატება

1. გამოიყენეთ `python3 tools/create_module.py` სკრიპტი ახალი მოდულის ჩონჩხის დასაგენერირებლად.
2. შეავსეთ `config.json` ფაილი default პარამეტრებით.
3. შეავსეთ `_create`, `init`, `start`, `deinit` და `handle_event` ფუნქციები თქვენი ლოგიკით.
4. ჩართეთ მოდული `idf.py menuconfig`-ში.

## აკრძალული და რეკომენდებული პრაქტიკები

❌ **არასდროს:**

- პირდაპირი `#include` სხვა მოდულის ჰედერების.
- არასტანდარტული ფაილური სტრუქტურა.
- კონფიგურაციის მნიშვნელობების hardcode-ირება.
- `_create` ფუნქციაში გადაცემული `config` ობიექტის დუბლირება.

✅ **ყოველთვის:**

- დაიცავით naming და structure კონვენციები.
- გამოიყენეთ `Service Locator` ან `Event Bus` სწორი სცენარისთვის.
- აღწერეთ ყველა public API და კონფიგურაცია `README.md`-ში.
- `_deinit` ფუნქციაში გაათავისუფლეთ ყველა რესურსი, მათ შორის `module->current_config`.

---

შემდეგი ნაბიჯი: დეტალურად განვიხილოთ core კომპონენტები და მათი დანიშნულება.
