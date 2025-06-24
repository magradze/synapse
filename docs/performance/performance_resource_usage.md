# რესურსების გამოყენების სახელმძღვანელო (Performance Resource Usage Guide)

---

## მიზანი

ეს დოკუმენტი წარმოადგენს Synapse ESP Framework-ის რესურსების (RAM, Flash, CPU, Stack) გამოყენების რჩევებსა და მონიტორინგის მეთოდებს naming/structure კონვენციების დაცვით.

---

## RAM-ის გამოყენება

- გამოიყენეთ static allocation სადაც შესაძლებელია
- მონიტორინგისთვის გამოიყენეთ heap_caps_get_free_size(), heap_caps_print_heap_info()
- თავიდან აიცილეთ დიდი მასივების ან ობიექტების stack-ზე გამოყოფა

## Flash-ის გამოყენება

- firmware-სა და მონაცემების შენახვისთვის გამოიყენეთ მხოლოდ საჭირო მოცულობა
- გამორთეთ არასაჭირო მოდულები და ფუნქციები
- გამოიყენეთ partition table ოპტიმალური განაწილებისთვის

## CPU-ის გამოყენება

- მძიმე ოპერაციები გაანაწილეთ task-ებში ან timer-ებში
- გამოიყენეთ yield() ან delay() ხანგრძლივი ციკლებისას
- მონიტორინგისთვის გამოიყენეთ perf counters ან ESP_LOGI

## Stack-ის გამოყენება

- დააყენეთ ოპტიმალური stack ზომები თითოეული task-ისთვის
- გამოიყენეთ uxTaskGetStackHighWaterMark() stack usage-ის გასაკონტროლებლად

---

## მონიტორინგის მაგალითები

### RAM
```c
ESP_LOGI(TAG, "Free heap: %d bytes", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
```

### Stack
```c
ESP_LOGI(TAG, "Stack high water mark: %d", uxTaskGetStackHighWaterMark(NULL));
```

---

## Best Practices

- რეგულარულად შეამოწმეთ რესურსების გამოყენება
- ჩაწერეთ ყველა მნიშვნელოვანი usage ლოგში
- Bottleneck-ის აღმოჩენისას განახორციელეთ ოპტიმიზაცია

---

## დაკავშირებული დოკუმენტები

- [performance_index.md](performance_index.md)
- [performance_optimization.md](performance_optimization.md)
- [performance_benchmarks.md](performance_benchmarks.md)
- [performance_best_practices.md](performance_best_practices.md)

---

_დამატებითი კითხვებისთვის იხილეთ შესაბამისი performance დოკუმენტი ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს._

