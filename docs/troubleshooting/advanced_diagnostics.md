# Advanced Diagnostics (გაფართოებული დიაგნოსტიკა)

## მიზანი

ეს დოკუმენტი აღწერს Synapse Framework-ის სისტემის და მოდულების გაღრმავებული დიაგნოსტიკის მეთოდებს, ინსტრუმენტებსა და best practices-ს naming/structure კონვენციების დაცვით.

---

## 1. Stack Overflow და Task Watchdog
- გამოიყენეთ FreeRTOS stack overflow checking (`configCHECK_FOR_STACK_OVERFLOW`)
- ჩართეთ Task Watchdog და მონიტორინგი
- გამოიყენეთ `uxTaskGetStackHighWaterMark()` stack usage-ის გასაკონტროლებლად

## 2. Heap Corruption და Memory Leak
- ჩართეთ heap corruption detection ESP-IDF-ში (`CONFIG_HEAP_POISONING`)
- გამოიყენეთ `heap_caps_check_integrity_all()` და `heap_caps_print_heap_info()`
- გამოიყენეთ `esp_err_t heap_trace_start()` memory leak-ის დიაგნოსტიკისთვის

## 3. Core Dumps და Crash Reporting
- ჩართეთ core dump ფუნქციონალი ESP-IDF-ში
- გამოიყენეთ `espcoredump.py` crash-ის დეტალური ანალიზისთვის
- შეინახეთ crash log-ები და გააგზავნეთ საჭიროების შემთხვევაში

## 4. Custom Health Checks
- დაამატეთ custom health check ფუნქციები Health Monitor მოდულში
- გამოიყენეთ რეგულარული სტატუსის შემოწმება და ლოგირება

## 5. Peripheral Diagnostics
- გამოიყენეთ შესაბამისი peripheral-ის (I2C, SPI, UART) დიაგნოსტიკური ფუნქციები
- ჩართეთ hardware-level ლოგირება საჭიროების შემთხვევაში

## 6. Network Diagnostics
- გამოიყენეთ WiFi და MQTT მოდულების სტატუსის API-ები
- ჩართეთ დეტალური ლოგირება კავშირის პრობლემების დროს

## 7. Performance Metrics
- გამოიყენეთ `esp_timer_get_time()` ოპერაციების დროის გასაზომად
- ჩაწერეთ latency და throughput მონაცემები ლოგში

---

დამატებითი რჩევებისთვის იხილეთ [debugging_guide.md](debugging_guide.md), [common_issues.md](common_issues.md), [faq.md](faq.md) ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს.

