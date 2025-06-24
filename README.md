<div align="center">
  <img src="https://raw.githubusercontent.com/espressif/esp-idf/master/docs/_static/espressif-logo.svg" width="120" alt="ESP-IDF Logo"/>
  
# Synapse ESP Framework
  
  _მძლავრი, მოდულური და გაფართოებადი პლატფორმა ESP32 მოწყობილობებისთვის_
  
  [![Getting Started](https://img.shields.io/badge/-Getting%20Started-blue?style=for-the-badge)](#getting-started)
  [![API Reference](https://img.shields.io/badge/-API%20Reference-green?style=for-the-badge)](#api-reference)
  [![Contributing](https://img.shields.io/badge/-Contributing-orange?style=for-the-badge)](#%E1%83%99%E1%83%9D%E1%83%9C%E1%83%A2%E1%83%A0%E1%83%98%E1%83%91%E1%83%A3%E1%83%AA%E1%83%98%E1%83%98%E1%83%A1-%E1%83%A1%E1%83%90%E1%83%AE%E1%83%94%E1%83%9A%E1%83%9B%E1%83%AC%E1%83%A6%E1%83%95%E1%83%90%E1%83%9C%E1%83%94%E1%83%9A%E1%83%9D-contributing-guide)
  [![Security](https://img.shields.io/badge/-Security-red?style=for-the-badge)](#security)
  [![Performance](https://img.shields.io/badge/-Performance-yellow?style=for-the-badge)](#performance)
</div>

---

## 🔥 პროექტის მიმოხილვა

Synapse ESP Framework არის ღია კოდის პლატფორმა, რომელიც ამარტივებს ESP32-ზე დაფუძნებული IoT სისტემების შექმნას. ის უზრუნველყოფს მოდულურ არქიტექტურას, მოქნილ API-ებს, უსაფრთხოების და წარმადობის საუკეთესო პრაქტიკებს.

---

## 🚀 Getting Started

1. **საჭიროებები:**
   - [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) v5.4.1+
   - Python 3.x, git

2. **პროექტის კლონირება:**

   ```sh
   git clone https://github.com/IoTesseract/synapse-esp.git
   cd synapse-esp
   ```

3. **სტრუქტურის გაცნობა:**
   - იხილეთ [Directory Structure](docs/structure/directory_structure.md)
   - ძირითადი საქაღალდეები: `main/`, `components/`, `docs/`

4. **მოდულის დამატება:**
   - კონფიგურაცია: `system_config.json`
   - იხილეთ მაგალითი და დეტალები [getting_started.md](docs/tutorials/getting_started.md)

5. **ბილდი და ფლეში:**
   - გამოიყენეთ ESP-IDF სტანდარტული build და flash ბრძანებები:

     ```sh
     idf.py build
     idf.py -p <PORT> flash
     ```

---

## 📚 API Reference

- იხილეთ [API Reference Index](docs/api_reference/api_index.md) სრული API დოკუმენტაციისთვის.
- ძირითადი API-ები:
  - Core, Event, Configuration, Module, Display, Sensor, Communication, Drivers, Utility, Actuators, Provisioning, System, OTA, Version, Health Monitor, Storage, Time Sync, Local Control, Custom Extensions

---

## 🤝 კონტრიბუციის სახელმძღვანელო (Contributing Guide)

- იხილეთ [Contributing Guide](docs/contributing/contributing_guide.md) დეტალური ინსტრუქციებისთვის.
- ძირითადი წესები:
  - გამოიყენეთ branch-ები და Pull Request-ები
  - დაიცავით naming/structure convetion-ები
  - კოდი უნდა იყოს კარგად დოკუმენტირებული
  - README და სხვა დოკუმენტაცია განახლებული

---

## 🛡️ Security

- იხილეთ [Security Best Practices](docs/security/security_best_practices.md)
- ძირითადი რეკომენდაციები:
  - გამოიყენეთ TLS/SSL კომუნიკაციისთვის
  - credentials შეინახეთ უსაფრთხოდ (NVS/Storage)
  - firmware განაახლეთ მხოლოდ უსაფრთხო არხით (OTA)
  - input validation ყველა მოდულში

---

## ⚡ Performance

- იხილეთ [Performance Best Practices](docs/performance/performance_best_practices.md)
- რეკომენდაციები:
  - გამოიყენეთ static allocation სადაც შესაძლებელია
  - bottleneck-ების იდენტიფიკაცია და ოპტიმიზაცია
  - lazy initialization რესურსების დაზოგვისთვის

---

## 🛠️ Troubleshooting

- იხილეთ [Troubleshooting FAQ](docs/troubleshooting/faq.md)
- ხშირად დასმული კითხვები და პრობლემების გადაჭრის გზები

---

## 📄 ლიცენზია

MIT License

---

<div align="center">
  <sub>დამატებითი ინფორმაცია იხილეთ [დოკუმენტაციაში](docs/README.md)</sub>
</div>
