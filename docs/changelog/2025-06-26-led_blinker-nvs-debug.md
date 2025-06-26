# CHANGELOG: led_blinker ინიციალიზაციის პრობლემის დიაგნოსტიკა და გამოსწორება

**თარიღი:** 2025-06-26  
**ავტორი:** Giorgi Magradze

## პრობლემა

- led_blinker მოდული არ ინიციალიზდებოდა, მიუხედავად იმისა, რომ ყველა კონფიგურაცია (Kconfig, linkage, system_config.json) სწორად იყო მითითებული.
- MODULE_REGISTRY-ში modules array-ს ზომა იყო 1 (მხოლოდ logger_module), led_blinker იკარგებოდა.

## დიაგნოსტიკა

- შედარებულ იქნა led_blinker და logger_module-ის module.json, linkage, კოდი, generated_module_factory, Kconfig, CMakeLists.txt.
- დადასტურდა, რომ ორივე მოდული build-ში შედის და system_config.json-ში enabled:true-ითაა.
- MODULE_REGISTRY.c-ში დაემატა debug ლოგები: modules array iteration, ტიპები, ზომა.
- CONFIG_MANAGER.c-ში დაემატა debug ლოგები: JSON parsing, modules array-ის ზომა, თითოეული მოდულის instance_name და enabled ველი, ასევე დაემატა დროებითი კოდი NVS კონფიგის იძულებითი წაშლისთვის (შემდგომში წაიშალა).
- MODULE_FACTORY.c-ში დადასტურდა, რომ forward declaration სტილი გამოიყენება გენერირებული ჰედერისთვის და დაემატა დეტალური ლოგირება მოდულის შექმნის პროცესზე.
- დადგინდა, რომ CONFIG_MANAGER NVS-დან კითხულობდა ძველ კონფიგურაციას, სადაც მხოლოდ logger_module იყო.
- NVS კონფიგის იძულებითი წაშლის შემდეგ, ჩაიტვირთა ახალი embedded system_config.json, სადაც ორივე მოდული იყო აღწერილი.

## მიზეზი

- NVS-ში იყო შენახული ძველი კონფიგურაცია, რომელიც არ ემთხვეოდა ახალ system_config.json-ს.
- CONFIG_MANAGER ყოველთვის NVS-დან კითხულობს კონფიგურაციას, თუ ის არსებობს, და მხოლოდ მის არარსებობის შემთხვევაში fallback-ად იყენებს embedded defaults-ს.

## გამოსავალი

- იძულებით წაიშალა NVS-ში არსებული კონფიგი (debug კოდით ან idf.py erase-flash).
- სისტემამ ჩაიტვირთა ახალი system_config.json და ორივე მოდული წარმატებით ინიციალიზდა.
- debug ლოგები დაემატა და შემდგომში წაიშალა.
- MODULE_FACTORY.c-ში დაემატა დეტალური ლოგირება მოდულის შექმნის პროცესზე.
- CONFIG_MANAGER.c-ში debug ლოგები და დროებითი NVS წაშლის კოდი წაიშალა საბოლოო ვერსიაში.

## ცვლილებები კოდში

- **components/core/config_manager.c**: დაემატა და შემდგომში წაიშალა debug ლოგები და დროებითი კოდი NVS კონფიგის წასაშლელად; დაემატა დეტალური ლოგირება JSON parsing-ზე და modules array iteration-ზე.
- **components/core/module_factory.c**: დაემატა დეტალური ლოგირება მოდულის შექმნის პროცესზე; დადასტურდა forward declaration სტილის გამოყენება გენერირებული ჰედერისთვის.
- **components/modules/testing/led_blinker/**: დაემატა ახალი led_blinker მოდული შესაბამისი კოდით, module.json-ით, linkage-ით და კონფიგურაციით.
- **configs/system_config.json**: დაემატა led_blinker მოდული enabled:true-ით.

## რეკომენდაცია

- თუ system_config.json იცვლება, აუცილებელია NVS კონფიგის წაშლა (factory reset ან erase-flash), რათა ცვლილებები აისახოს სისტემაში.
- Debug ლოგები სასარგებლოა მსგავსი პრობლემების სწრაფად დიაგნოსტირებისთვის.

---

**შედეგი:**

- led_blinker და logger_module ორივე ინიციალიზდება და მუშაობს.
- სისტემის კონფიგურაცია სწორად იკითხება და აისახება.
- კოდის სტრუქტურა და ლოგირება გაუმჯობესებულია დიაგნოსტიკისთვის.
