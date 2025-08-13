# Synapse Local Setup Guide

---

## მიმოხილვა

ეს დოკუმენტი შეიცავს დეტალურ ინსტრუქციებს Synapse Framework-ის (ESP-IDF პროექტი) ლოკალური განვითარების გარემოს მოსამზადებლად. გთხოვთ, მიჰყვეთ ყველა ნაბიჯს, რათა თავიდან აიცილოთ პრობლემები და უზრუნველყოთ სტანდარტებთან შესაბამისობა.

---

## სისტემური მოთხოვნები

- ოპერაციული სისტემა: Linux, macOS ან Windows
- Git
- Python 3.8+ (მხოლოდ ESP-IDF-ის ინსტალაციისთვის)
- ESP-IDF (რეკომენდებული ვერსია: 5.4.1)
- Serial/USB დრაივერები (საჭიროების შემთხვევაში)

---

## ESP-IDF-ის ინსტალაცია

1. გადმოწერეთ ESP-IDF ოფიციალური GitHub რეპოზიტორიიდან ან გამოიყენეთ ოფიციალური ინსტალერი:

```bash
git clone --recursive https://github.com/espressif/esp-idf.git -b v5.4.1
cd esp-idf
./install.sh
```

2. გააქტიურეთ გარემო:

```bash
. ./export.sh
```

---

## პროექტის კლონირება

```bash
git clone https://github.com/magradze/synapse.git
cd synapse
```

---

## sdkconfig ფაილის კონფიგურაცია

- დააკოპირეთ საჭიროების შემთხვევაში `sdkconfig.rename` ან სხვა ნიმუში `sdkconfig` სახელით:

```bash
cp sdkconfig.rename sdkconfig
```

- კონფიგურაციის შესაცვლელად გამოიყენეთ:

```bash
idf.py menuconfig
```

---

## პროექტის აგება და გაშვება

- ასაწყობად:

```bash
idf.py build
```

- ჩასაწერად (flash):

```bash
idf.py -p /dev/ttyUSB0 flash
```

- მონიტორინგისთვის:

```bash
idf.py -p /dev/ttyUSB0 monitor
```

---

## ტესტირება

- გამოიყენეთ ერთეული ტესტები ან integration ტესტები, თუ პროექტში არსებობს.
- ტესტირების ინსტრუქციები იხილეთ [components/unity/README.md](../../components/unity/README.md) ან შესაბამის დოკუმენტაციაში.

---

## პრობლემების შემთხვევაში

- იხილეთ [troubleshooting](../troubleshooting/) დოკუმენტაცია
- მიმართეთ გუნდის პასუხისმგებელ პირს ან გახსენით Issue

---

## რესურსები

- [კონტრიბუციის სახელმძღვანელო](contributing_guide.md)
- [კონვენციების ინდექსი](../convention/convention_index.md)
- [ქცევის კოდექსი](code_of_conduct.md)

---

_დაიცავით ეს ინსტრუქციები, რათა თქვენი გარემო იყოს სტანდარტებთან შესაბამისი და მზად კონტრიბუციისთვის._
