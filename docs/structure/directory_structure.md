# საქაღალდეებისა და ფაილების სტრუქტურა (Directory Structure)

## ძირითადი სტრუქტურა

```
project_root/
├── main/                  # მთავარი აპლიკაცია (app_main და ძირითადი ლოგიკა)
│   └── main.c
├── components/            # ყველა კომპონენტი და მოდული
│   ├── core/              # ბირთვი: service locator, event bus, module register და სხვა
│   ├── interfaces/        # გლობალური ინტერფეისები
│   └── modules/           # კატეგორიებად დაყოფილი მოდულები
│       ├── sensors/
│       ├── actuators/
│       ├── communications/
│       ├── display/
│       ├── drivers/
│       ├── provisioning/
│       ├── system/
│       └── utility/
├── docs/                  # დოკუმენტაცია და სტანდარტები
│   ├── convention/
│   └── structure/
├── CMakeLists.txt         # მთავარი ბილდის სკრიპტი
├── Kconfig                # მთავარი კონფიგურაცია
└── ...                    # სხვა ფაილები და საქაღალდეები
```

## ძირითადი საქაღალდეების აღწერა

- **main/** — მთავარი აპლიკაციის საწყისი წერტილი (app_main), სადაც ხდება სისტემის ინიციალიზაცია და მოდულების ჩართვა.
- **components/core/** — ბირთვი: service locator, event bus, module register, module_fabrique და სხვა ინფრასტრუქტურული კომპონენტები.
- **components/interfaces/** — გლობალური ინტერფეისები, რომლებიც უზრუნველყოფს მოდულებს შორის კონტრაქტებს.
- **components/modules/** — კატეგორიებად დაყოფილი მოდულები (სენსორები, აქტუატორები, კომუნიკაციები, ეკრანები, დრაივერები და ა.შ.).
- **docs/** — დოკუმენტაცია, სტანდარტები და არქიტექტურული აღწერები.
- **CMakeLists.txt** — მთავარი ბილდის სკრიპტი, რომელიც განსაზღვრავს პროექტის ბილდს.
- **Kconfig** — მთავარი კონფიგურაციის ფაილი, სადაც ხდება მოდულების ჩართვა/გამორთვა.

## modules საქაღალდის სტრუქტურა

```
components/modules/
├── sensors/
│   └── dht22_module/
├── actuators/
│   └── relay_module/
├── communications/
│   ├── wifi_module/
│   └── mqtt_module/
├── display/
│   └── ssd1306_module/
├── drivers/
│   └── i2c_bus_module/
├── provisioning/
│   └── ble_prov_module/
├── system/
│   └── ota_module/
└── utility/
    └── logger_module/
```

## თითოეული მოდულის სტრუქტურა

```
components/modules/{category}/{module_name}/
├── CMakeLists.txt
├── module.json
├── README.md
├── include/
│   └── {module_name}.h
├── src/
│   └── {module_name}.c
└── ... (სხვა საჭირო ფაილები)
```

- **CMakeLists.txt** — მოდულის ბილდის სკრიპტი
- **module.json** — მოდულის აღწერითი და კონფიგურაციული ფაილი
- **README.md** — მოკლე აღწერა და გამოყენების მაგალითები
- **include/** — ჰედერ ფაილები (public API)
- **src/** — წყარო კოდი (implementation)

## აკრძალული და რეკომენდებული პრაქტიკები

❌ **არასდროს:**
- პირდაპირი #include სხვა მოდულის header-ების
- არასტანდარტული ან არათანმიმდევრული საქაღალდეების შექმნა
- მოდულის ფაილების არასწორი განლაგება

✅ **ყოველთვის:**
- დაიცავით სტანდარტული სტრუქტურა
- გამოიყენეთ მკაფიო და დესკრიპტიული სახელები
- ყველა მოდულს ჰქონდეს თავისი include/ და src/ ქვეკატეგორიები
- გამოიყენეთ README.md თითოეულ მოდულში

---

შემდეგი ნაბიჯი: დეტალურად განვიხილოთ მოდულების არქიტექტურა და მათი ურთიერთქმედება.

