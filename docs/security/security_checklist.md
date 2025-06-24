# უსაფრთხოების შემოწმების სია (Security Checklist)

---

## მიზანი

ეს დოკუმენტი წარმოადგენს Synapse ESP Framework-ის უსაფრთხოების შემოწმების სიას (release checklist, code review checklist) naming/structure კონვენციების დაცვით.

---

## Release Checklist
- [ ] ყველა გარე კომუნიკაცია დაშიფრულია (TLS/SSL)
- [ ] credentials და პაროლები ინახება მხოლოდ NVS-ში ან დაშიფრულ storage-ში
- [ ] ლოგებში არ იწერება სენსიტიური ინფორმაცია
- [ ] firmware განახლება სიგნირებულია და დაშიფრულია
- [ ] ყველა მოდული ასრულებს input validation-ს
- [ ] ყველა dependency განახლებულია და არ შეიცავს ცნობილ CVE-ს
- [ ] უსაფრთხოების დოკუმენტაცია განახლებულია

## Code Review Checklist
- [ ] არ გამოიყენება hardcoded credentials
- [ ] ყველა API ასრულებს პარამეტრების ვალიდაციას
- [ ] არ ხდება სენსიტიური ინფორმაციის ლოგირება
- [ ] გამოიყენება მხოლოდ public API-ები მოდულებს შორის
- [ ] ყველა უსაფრთხოების ცვლილება აღწერილია დოკუმენტაციაში

---

## Best Practices
- გამოიყენეთ ეს სია ყველა release-ისა და code review-ს დროს
- შეინახეთ შემოწმებული სიები პროექტის არქივში

---

## დაკავშირებული დოკუმენტები
- [security_index.md](security_index.md)
- [security_best_practices.md](security_best_practices.md)
- [security_guidelines.md](security_guidelines.md)
- [security_vulnerability_disclosure.md](security_vulnerability_disclosure.md)

---

_დამატებითი კითხვებისთვის იხილეთ შესაბამისი security დოკუმენტი ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს._

