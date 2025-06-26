# CHANGELOG - 2025-06-26

## [Major] Module System Automation & Refactor

### Added
- **create_module.py**: ავტომატური სკრიპტი ახალი მოდულების გენერირებისთვის (header, source, CMake, Kconfig, module.json)
- **components/core/collect_modules.cmake**: მოდულების ავტომატური პოვნა, factory-სა და Kconfig.projbuild-ის გენერაცია
- **components/core/generated_module_factory.[ch].in**: factory-ს template-ები კონდიციური include-ებით
- **components/modules/testing/test_module/**: ტესტური მოდული ავტომატური გენერაციით
- **docs/CREATE_MODULE_DOCUMENTATION.md**: დეტალური დოკუმენტაცია მოდულების ავტომატური სისტემისთვის

### Changed
- **components/core/CMakeLists.txt**: core კომპონენტში COLLECTED_MODULE_DEPENDENCIES ემატება REQUIRES-ში, რათა მოდულის header-ები იყოს ხელმისაწვდომი
- **components/modules/utilities/logger_module/CMakeLists.txt**: დაემატა "სმარტი" conditional registration (configure ეტაპზე placeholder, ჩართვისას სრული, გამორთვისას placeholder)
- **components/interfaces/include/base_module.h**: განახლდა მოდულის ინტერფეისის სტრუქტურა (function pointer-ები, სტატუსები, კონტრაქტი)

### Fixed
- Conditional compilation: build-ში მხოლოდ ჩართული მოდულების კოდი ხვდება, გამორთული მოდულები აგენერირებენ placeholder-ს
- CMake configure ეტაპზე build არ ვარდება, ყველა მოდული რეგისტრირდება სწორად
- collect_modules.cmake-ში დაემატა კონდიციური include-ები და factory entries
- create_module.py-ს მიერ გენერირებული კოდი სრულად შეესაბამება base_module.h-ს

### Detailed File Changes

- **CMakeLists.txt, components/core/CMakeLists.txt**: პროექტის build სისტემაში დაემატა COLLECTED_MODULE_DEPENDENCIES, რათა core-ს ჰქონდეს ყველა ჩართული მოდულის header-ებზე წვდომა და factory-ს სწორი მუშაობა.
- **components/core/collect_modules.cmake**: დაემატა მოდულების ავტომატური პოვნა, თითოეული module.json და Kconfig-ის ინკლუზია, აგენერირდება factory-სა და Kconfig.projbuild-ის ფაილები მხოლოდ ჩართული მოდულებისთვის.
- **components/core/generated_module_factory.[ch].in**: დაემატა template-ები, რომლითაც collect_modules.cmake აგენერირებს მოდულების factory-ს კონდიციური include-ებით.
- **components/modules/utilities/logger_module/CMakeLists.txt**: დაემატა "სმარტი" conditional registration, რათა build-ში მოხვდეს მხოლოდ ჩართული მოდულის კოდი, გამორთვისას კი placeholder.
- **components/interfaces/include/base_module.h**: განახლდა მოდულის ინტერფეისის სტრუქტურა (function pointer-ები, სტატუსები, კონტრაქტი), რათა ყველა მოდული იყოს ერთიანი კონტრაქტის ქვეშ.
- **components/modules/testing/test_module/**: შეიქმნა ტესტური მოდული ავტომატური გენერაციით, ახალი სისტემის ტესტირებისთვის.
- **docs/CREATE_MODULE_DOCUMENTATION.md**: დაემატა დეტალური დოკუმენტაცია მოდულების ავტომატური სისტემისთვის, გამოყენების მაგალითებითა და best practices-ით.
- **create_module.py**: დაემატა ახალი მოდულის ავტომატური გენერაცია (header, source, CMake, Kconfig, module.json), კოდი სრულად შეესაბამება base_module.h-ს და conditional compilation-ს.

**ყველა ეს ცვლილება მიზნად ისახავს მოდულების მართვის სრულ ავტომატიზაციას, შეცდომების მინიმიზაციას და დეველოპმენტის სიჩქარის ზრდას.**

### Why/How
- **რატომ:** მოდულების დამატება/ამოღება უნდა იყოს ავტომატური, build-ში მოხვდეს მხოლოდ ჩართული კოდი, და ინტერფეისი იყოს ერთიანი
- **როგორ:** collect_modules.cmake პოულობს ყველა module.json/Kconfig-ს, აგენერირებს საჭირო ფაილებს, core-ს CMakeLists.txt ავტომატურად აერთიანებს დამოკიდებულებებს, ხოლო თითოეული მოდულის CMakeLists.txt conditional logic-ით მართავს build-ში მოხვედრას

### Testing
- შექმნილია test_module ავტომატურად, build/reconfigure/menuconfig ციკლი წარმატებით გაიარა
- გამორთული მოდულის კოდი არ ხვდება firmware-ში (დამოწმებულია build log-ით)
- ყველა ცვლილება დოკუმენტირებულია და ტესტირებულია
