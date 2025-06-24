# Debugging Guide (დიბაგინგის სახელმძღვანელო)

## მიზანი

ეს დოკუმენტი დაგეხმარებათ Synapse Framework-ის დიბაგინგის პროცესში, ლოგირების, დიაგნოსტიკისა და პრობლემების სწრაფად აღმოჩენის საუკეთესო პრაქტიკებით.

---

## 1. ლოგირების ჩართვა და გამოყენება
- გამოიყენეთ DEFINE_COMPONENT_TAG ყველა მოდულში
- ჩართეთ საჭირო log level (INFO, DEBUG, VERBOSE)
- გამოიყენეთ ESP_LOGE, ESP_LOGW, ESP_LOGI, ESP_LOGD, ESP_LOGV შესაბამისი მნიშვნელობისთვის
- ლოგებში მიუთითეთ კონტექსტი და დეტალები

## 2. Debug Build-ის კონფიგურაცია
- გამოიყენეთ `idf.py menuconfig` და ჩართეთ Debug/Verbose ლოგირება
- გადაამოწმეთ UART პარამეტრები და ბაფერის ზომა

## 3. GDB და Breakpoint-ების გამოყენება
- გამოიყენეთ `idf.py gdb` ან VSCode-ის ESP-IDF გაფართოება
- დააყენეთ breakpoints პრობლემურ ადგილებში
- გამოიყენეთ stack trace და variable inspection

## 4. Task და Stack Usage მონიტორინგი
- გამოიყენეთ `uxTaskGetSystemState()` და `uxTaskGetStackHighWaterMark()` FreeRTOS-ისთვის
- მონიტორინგი დაგეხმარებათ stack overflow ან task starvation-ის აღმოჩენაში

## 5. Heap და Memory Usage
- გამოიყენეთ `heap_caps_get_free_size()` და `heap_caps_print_heap_info()` მეხსიერების მონიტორინგისთვის
- ჩართეთ heap corruption detection ESP-IDF-ში

## 6. Event და Service Debugging
- გამოიყენეთ ლოგები Event Bus-ზე მოვლენების გასავლელად
- გადაამოწმეთ Service Locator-ში სერვისების რეგისტრაცია და მოძიება

## 7. Assertion-ები და Error Checking
- გამოიყენეთ assert() და ESP_ERROR_CHECK() კრიტიკულ ადგილებში
- ყველა ფუნქციაში შეამოწმეთ დაბრუნებული მნიშვნელობები

## 8. დამატებითი ინსტრუმენტები
- გამოიყენეთ `idf.py monitor` ლოგების რეალურ დროში სანახავად
- გამოიყენეთ `esp-idf monitor` და სხვა ESP-IDF-ის დიბაგინგის ხელსაწყოები

---

დამატებითი რჩევებისთვის იხილეთ [common_issues.md](common_issues.md), [faq.md](faq.md) ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს.

