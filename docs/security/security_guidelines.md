# უსაფრთხოების სახელმძღვანელოები (Security Guidelines)

---

## მიზანი

ეს დოკუმენტი წარმოადგენს Synapse ESP Framework-ის უსაფრთხოების სახელმძღვანელოებს naming/structure კონვენციების დაცვით.

---

## TLS/SSL
- გამოიყენეთ მხოლოდ დაშიფრული კავშირი (TLS/SSL) ყველა გარე სერვისთან (MQTT, REST, Cloud)
- სერტიფიკატები შეინახეთ უსაფრთხოდ და განაახლეთ რეგულარულად

## Authentication & Authorization
- გამოიყენეთ ძლიერი პაროლები და access token-ები
- ჩართეთ authentication ყველა გარე სერვისთან
- role-based access control გამოიყენეთ საჭიროების შემთხვევაში

## Input Validation
- შეამოწმეთ ყველა შემომავალი მონაცემი და პარამეტრი
- გამოიყენეთ whitelist/blacklist საჭიროების მიხედვით
- თავიდან აიცილეთ buffer overflow და injection ტიპის შეტევები

## Credentials Management
- credentials შეინახეთ მხოლოდ NVS-ში ან დაშიფრულ storage-ში
- არასდროს ჩაწეროთ პაროლები, ტოკენები ან სენსიტიური ინფორმაცია ლოგებში
- გამოიყენეთ environment variables ან secure provisioning საჭიროების შემთხვევაში

## OTA და Firmware Security
- firmware განახლებებისთვის გამოიყენეთ სიგნირებული და დაშიფრული OTA
- firmware-ს განახლებისას შეამოწმეთ სიგნატურა და ვერსია

## Logging
- გამოიყენეთ მხოლოდ საჭირო log level-ები
- არ ჩაწეროთ სენსიტიური ინფორმაცია ლოგებში
- რეგულარულად შეამოწმეთ ლოგების არქივაცია და წვდომა

---

## Best Practices
- დაიცავით ყველა უსაფრთხოების სახელმძღვანელო და პოლიტიკა
- გამოიყენეთ მხოლოდ public API-ები უსაფრთხოების ინტეგრაციისთვის
- აღწერეთ ყველა უსაფრთხოების ნაბიჯი დოკუმენტაციაში

---

## დაკავშირებული დოკუმენტები
- [security_index.md](security_index.md)
- [security_best_practices.md](security_best_practices.md)
- [security_vulnerability_disclosure.md](security_vulnerability_disclosure.md)
- [security_checklist.md](security_checklist.md)

---

_დამატებითი კითხვებისთვის იხილეთ შესაბამისი security დოკუმენტი ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს._

