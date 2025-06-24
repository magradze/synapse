# სწრაფი სტარტი (Getting Started)

## მიზანი

ეს სახელმძღვანელო დაგეხმარებათ სწრაფად დაიწყოთ მუშაობა Synapse ESP Framework-ზე, გაიგოთ ძირითადი სტრუქტურა და პირველი მოდულის ჩართვა.

## 1. გარემოს მომზადება

- დააინსტალირეთ [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) (რეკომენდებულია v5.4.1 ან უფრო ახალი)
- დარწმუნდით, რომ გაქვთ Python 3.x და git
- კლონირეთ პროექტის რეპოზიტორია:
  ```sh
  git clone <თქვენი-რეპო-ბმული>
  cd <პროექტის საქაღალდე>
  ```

## 2. პროექტის სტრუქტურის გაცნობა

- იხილეთ [directory_structure.md](../structure/directory_structure.md) დეტალებისთვის
- ძირითადი საქაღალდეებია: main/, components/, docs/

## 3. პირველი მოდულის ჩართვა

- გახსენით `system_config.json` (ან შესაბამისი კონფიგურაციის ფაილი)
- დაამატეთ ან ჩართეთ სასურველი მოდული, მაგალითად:
  ```json
  {
      "type": "relay_module",
      "enabled": true,
      "config": {
          "instance_name": "relay1",
          "pin": 23
      }
  }
  ```
- დარწმუნდით, რომ მოდული ჩართულია Kconfig-ში

## 4. ბილდი და ფლეში

- გააგრძელეთ ბილდი და ფლეში სტანდარტული ESP-IDF ბრძანებებით:
  ```sh
  idf.py build
  idf.py -p /dev/ttyUSB0 flash monitor
  ```
- შეცვალეთ პორტი თქვენი მოწყობილობის მიხედვით

## 5. ლოგების ნახვა და ტესტირება

- მონიტორზე იხილავთ სისტემის ლოგებს
- დარწმუნდით, რომ ჩართული მოდული მუშაობს სწორად
- გამოიყენეთ ლოგირების სტანდარტები ([logging_standards.md](../convention/logging_standards.md))

## 6. დამატებითი რესურსები

- [project_overview.md](../structure/project_overview.md)
- [configuration_flow.md](../structure/configuration_flow.md)
- [module_architecture.md](../structure/module_architecture.md)
- [faq.md](../structure/faq.md)

---

თუ შეგექმნათ პრობლემა, იხილეთ [troubleshooting](../troubleshooting/) ან მიმართეთ გუნდის დოკუმენტაციის პასუხისმგებელ პირს.

