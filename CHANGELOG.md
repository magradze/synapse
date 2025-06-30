# Changelog

## [v2.1.0] - 2025-06-30

### ✨ Features

- **Module Initialization Prioritization:** Implemented a robust module sorting mechanism based on an `init_level` parameter. The `Module Registry` now automatically sorts all registered modules by their `init_level` before initialization, ensuring that modules with lower `init_level` values (higher priority) are started first. This eliminates dependencies on the module order in `system_config.json`.
- **RGB LED Indicator Enhancements:** The `rgb_led_indicator` module now supports a `PULSE` effect and uses a table-driven approach for event handling, making it more scalable and readable. The `release_control` API now correctly restores the last known system state.

### 🐛 Bug Fixes

- **Fixed Race Condition in Module Initialization:** Resolved a critical timing issue where modules (like `rgb_led_indicator`) could miss early system events (e.g., `WIFI_CREDENTIALS_NOT_FOUND`) because they were initialized after the event-publishing module. The new `init_level` sorting mechanism guarantees the correct initialization order.
- **Corrected LEDC Driver Usage:** Refactored the `rgb_led_indicator` module to initialize LEDC channels only once during `init`. This resolves the `GPIO is not usable, maybe conflict with others` warning and ensures smooth, flicker-free operation of LED effects.
- **Fixed `qsort` Comparator Logic:** Corrected the pointer casting in the `compare_modules_by_init_level` function to ensure proper sorting of the module array.

### ♻️ Refactoring

- **Centralized `init_level` Management:** The responsibility for setting `init_level` is now delegated to each module's `_create` function, reading the value from its corresponding `module.json` (via the merged config object). This makes the module's priority self-contained and independent of `system_config.json`.
- **Improved Event Handling in `rgb_led_indicator`:** Replaced a chain of `if-else` statements with a static `event_map` table for cleaner and more maintainable event-to-command mapping.


---- 
# ცვლილებების ისტორია (Changelog)

## [v2.1.0] - 2025-06-30

### ✨ ახალი ფუნქციონალი

- **მოდულების ინიციალიზაციის პრიორიტეტიზაცია:** დაინერგა მოდულების დალაგების მტკიცე მექანიზმი `init_level` პარამეტრის საფუძველზე. `Module Registry` ახლა ავტომატურად ალაგებს ყველა რეგისტრირებულ მოდულს მათი `init_level`-ის მიხედვით ინიციალიზაციამდე, რაც უზრუნველყოფს, რომ დაბალი `init_level`-ის მქონე (მაღალი პრიორიტეტის) მოდულები პირველები გაეშვან. ეს ხსნის დამოკიდებულებას `system_config.json`-ში მოდულების თანმიმდევრობაზე.
- **RGB LED ინდიკატორის გაუმჯობესება:** `rgb_led_indicator` მოდულს დაემატა `PULSE` (პულსაციის) ეფექტის მხარდაჭერა. ივენთების დამუშავება ახლა ხდება ცხრილზე დაფუძნებული მიდგომით, რაც კოდს უფრო მასშტაბირებადს და წაკითხვადს ხდის. `release_control` API ახლა კორექტულად აღადგენს სისტემის ბოლო ცნობილ მდგომარეობას.

### 🐛 შეცდომების გასწორება

- **გასწორდა Race Condition მოდულების ინიციალიზაციისას:** მოგვარდა კრიტიკული დროის ფაქტორით გამოწვეული პრობლემა, როდესაც მოდულები (მაგ., `rgb_led_indicator`) აცდენდნენ სისტემის ადრეულ ივენთებს (მაგ., `WIFI_CREDENTIALS_NOT_FOUND`), რადგან მათი ინიციალიზაცია ხდებოდა ივენთის გამომქვეყნებელი მოდულის შემდეგ. `init_level`-ზე დაფუძნებული დალაგება უზრუნველყოფს ინიციალიზაციის სწორ თანმიმდევრობას.
- **გასწორდა LEDC დრაივერის გამოყენება:** `rgb_led_indicator` მოდულის კოდი შეიცვალა ისე, რომ LEDC არხების ინიციალიზაცია ხდება მხოლოდ ერთხელ, `init` ფუნქციაში. ამან აღმოფხვრა `GPIO is not usable, maybe conflict with others` გაფრთხილება და უზრუნველყო LED ეფექტების გლუვი, უწყვეტი მუშაობა.
- **გასწორდა `qsort`-ის შედარების ფუნქციის ლოგიკა:** გასწორდა მაჩვენებლების ტიპების კონვერტაცია `compare_modules_by_init_level` ფუნქციაში, მოდულების მასივის სწორად დალაგების უზრუნველსაყოფად.

### ♻️ რეფაქტორინგი

- **`init_level`-ის მართვის ცენტრალიზაცია:** `init_level`-ის დაყენების პასუხისმგებლობა ახლა დელეგირებულია თითოეული მოდულის `_create` ფუნქციაზე, რომელიც კითხულობს მნიშვნელობას შესაბამისი `module.json`-იდან (გაერთიანებული `config` ობიექტის მეშვეობით). ეს მოდულის პრიორიტეტს ხდის თვითკმარს და დამოუკიდებელს `system_config.json`-ისგან.
- **გაუმჯობესდა ივენთების დამუშავება `rgb_led_indicator`-ში:** `if-else` ბრძანებების ჯაჭვი ჩანაცვლდა `static event_map` ცხრილით, რაც უზრუნველყოფს "ივენთი-ბრძანება" კავშირების უფრო სუფთა და ადვილად მართვად იმპლემენტაციას.
