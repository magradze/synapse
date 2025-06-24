# ბენჩმარკების ჩატარების სახელმძღვანელო (Performance Benchmarks Guide)

---

## მიზანი

ეს დოკუმენტი წარმოადგენს Synapse ESP Framework-ის ბენჩმარკების ჩატარების წესებს, შედეგების აღწერას და მაგალითებს naming/structure კონვენციების დაცვით.

---

## ბენჩმარკის ჩატარების ძირითადი ნაბიჯები

1. **ბენჩმარკის მიზნის განსაზღვრა**
   - განსაზღვრეთ, რომელი კომპონენტის ან ოპერაციის ბენჩმარკი გსურთ (მაგ: მოდულის ინიციალიზაცია, სენსორის კითხვა, MQTT publish latency)

2. **ტესტის გარემოს მომზადება**
   - გამოიყენეთ სტანდარტული ან განმეორებადი გარემო (ერთნაირი კონფიგურაცია, იგივე მოწყობილობა)
   - გამორთეთ არასაჭირო მოდულები

3. **ბენჩმარკის კოდის იმპლემენტაცია**
   - გამოიყენეთ esp_timer_get_time() ოპერაციების დროის გასაზომად
   - ჩაწერეთ შედეგები ლოგში
   - მაგალითი:
     ```c
     int64_t start = esp_timer_get_time();
     // ...operation...
     int64_t end = esp_timer_get_time();
     ESP_LOGI(TAG, "Operation took %lld us", (end - start));
     ```

4. **შედეგების ანალიზი**
   - შეადარეთ სხვადასხვა კონფიგურაციის ან მოდულის შედეგები
   - მოძებნეთ bottleneck-ები და ოპტიმიზაციის შესაძლებლობები

5. **დოკუმენტირება**
   - აღწერეთ ბენჩმარკის შედეგები performance_benchmarks.md-ში
   - მიუთითეთ გარემო, პარამეტრები და გამოყენებული კოდი

---

## მაგალითები

### სენსორის კითხვა
```c
int64_t start = esp_timer_get_time();
read_sensor();
int64_t end = esp_timer_get_time();
ESP_LOGI(TAG, "Sensor read: %lld us", (end - start));
```

### MQTT publish latency
```c
int64_t start = esp_timer_get_time();
mqtt_module_publish(topic, payload);
int64_t end = esp_timer_get_time();
ESP_LOGI(TAG, "MQTT publish: %lld us", (end - start));
```

---

## Best Practices

- გამოიყენეთ განმეორებადი გარემო და პარამეტრები
- ჩაწერეთ ყველა შედეგი და გარემო დეტალურად
- Bottleneck-ის აღმოჩენისას განახორციელეთ ოპტიმიზაცია და ხელახლა ჩაატარეთ ბენჩმარკი

---

## დაკავშირებული დოკუმენტები

- [performance_index.md](performance_index.md)
- [performance_optimization.md](performance_optimization.md)
- [performance_resource_usage.md](performance_resource_usage.md)
- [performance_best_practices.md](performance_best_practices.md)

---

_დამატებითი კითხვებისთვის იხილეთ შესაბამისი performance დოკუმენტი ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს._

