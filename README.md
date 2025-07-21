<div align="center">
  <a href="https://github.com/espressif/esp-idf">
    <img src="https://raw.githubusercontent.com/espressif/esp-idf/master/docs/_static/espressif-logo.svg" width="120" alt="ESP-IDF Logo"/>
  </a>
  
# Synapse ESP Framework
  
  _მძლავრი, მოდულური და გაფართოებადი პლატფორმა ESP32 მოწყობილობებისთვის_
  
  [![Getting Started](https://img.shields.io/badge/-სწრაფი%20სტარტი-blue?style=for-the-badge&logo=read-the-docs)](docs/tutorials/getting_started.md)
  [![API Reference](https://img.shields.io/badge/-API%20Reference-green?style=for-the-badge&logo=c)](docs/api_reference/api_index.md)
  [![Contributing](https://img.shields.io/badge/-კონტრიბუცია-orange?style=for-the-badge&logo=github)](docs/contributing/contributing_guide.md)
  [![Security](https://img.shields.io/badge/-უსაფრთხოება-red?style=for-the-badge&logo=lock)](docs/security/security_best_practices.md)
  [![Performance](https://img.shields.io/badge/-წარმადობა-yellow?style=for-the-badge&logo=speedtest)](docs/performance/performance_best_practices.md)
</div>

---

## 🔥 პროექტის მიმოხილვა

Synapse ESP Framework არის ღია კოდის პლატფორმა, რომელიც ამარტივებს ESP32-ზე დაფუძნებული IoT სისტემების შექმნას. ის უზრუნველყოფს მოდულურ არქიტექტურას, მოქნილ API-ებს, უსაფრთხოების და წარმადობის საუკეთესო პრაქტიკებს. ფრეიმვორქის მთავარი უპირატესობაა **მოდულების ავტომატური აღმოჩენა** და **დეცენტრალიზებული კონფიგურაცია**, რაც დეველოპმენტის პროცესს უკიდურესად მოქნილს და მასშტაბირებადს ხდის.

---

## 🚀 სწრაფი სტარტი (Getting Started)

1. **საჭიროებები:**
    * [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) v5.4.1+
    * Python 3.x, git

2. **პროექტის კლონირება:**

    ```sh
    git clone https://github.com/IoTesseract/synapse-esp.git
    cd synapse-esp
    ```

3. **სტრუქტურის გაცნობა:**
    * იხილეთ [Directory Structure](docs/structure/directory_structure.md)
    * ძირითადი საქაღალდეები: `main/`, `components/`, `configs/`, `docs/`

4. **მოდულის დამატება და კონფიგურაცია:**
    * **(ახალი!)** მოდულების კონფიგურაცია ხდება მათ საკუთარ `config.json` ფაილებში (`components/modules/{category}/{module_name}/config.json`).
    * იხილეთ დეტალური ინსტრუქციები [სწრაფი სტარტის სახელმძღვანელოში](docs/tutorials/getting_started.md).

5. **ბილდი და ფლეში:**
    * გამოიყენეთ ESP-IDF სტანდარტული build და flash ბრძანებები:

      ```sh
      idf.py build
      idf.py -p <PORT> flash monitor
      ```

---

## 📚 API Reference

* იხილეთ [API Reference Index](docs/api_reference/api_index.md) სრული API დოკუმენტაციისთვის.
* **ძირითადი API-ები:** Core, Event, Configuration, Module, Display, Sensor, Communication, Drivers, Utility, Actuators, Provisioning, System, OTA, Version, Health Monitor, Storage, Time Sync, Local Control, Custom Extensions.

---

## 🤝 კონტრიბუციის სახელმძღვანელო (Contributing Guide)

* იხილეთ [Contributing Guide](docs/contributing/contributing_guide.md) დეტალური ინსტრუქციებისთვის.
* **ძირითადი წესები:**
  * გამოიყენეთ branch-ები და Pull Request-ები.
  * დაიცავით naming/structure კონვენციები.
  * კოდი უნდა იყოს კარგად დოკუმენტირებული.
  * განაახლეთ შესაბამისი დოკუმენტაცია.

---

## 🛡️ უსაფრთხოება (Security)

* იხილეთ [Security Best Practices](docs/security/security_best_practices.md).
* **ძირითადი რეკომენდაციები:**
  * გამოიყენეთ TLS/SSL კომუნიკაციისთვის.
  * credentials შეინახეთ უსაფრთხოდ (NVS).
  * firmware განაახლეთ მხოლოდ უსაფრთხო არხით (Signed OTA).
  * გამოიყენეთ input validation ყველა მოდულში.

---

## ⚡ წარმადობა (Performance)

* იხილეთ [Performance Best Practices](docs/performance/performance_best_practices.md).
* **რეკომენდაციები:**
  * გამოიყენეთ static allocation სადაც შესაძლებელია.
  * მოახდინეთ bottleneck-ების იდენტიფიკაცია და ოპტიმიზაცია.
  * გამოიყენეთ lazy initialization რესურსების დაზოგვისთვის.

---

## 🛠️ პრობლემების მოგვარება (Troubleshooting)

* იხილეთ [Troubleshooting FAQ](docs/troubleshooting/faq.md).
* ხშირად დასმული კითხვები და პრობლემების გადაჭრის გზები.

---

## 📄 ლიცენზია

MIT License

---

<div align="center">
  <sub>დამატებითი ინფორმაცია იხილეთ <a href="docs/README.md">სრულ დოკუმენტაციაში</a></sub>
</div>
