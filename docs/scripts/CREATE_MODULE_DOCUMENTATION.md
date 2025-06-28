# 🚀 create_module.py: Synapse მოდულის გენერატორი (v2.0)

## 📋 რა არის create_module.py?

`create_module.py` არის **განახლებული, მძლავრი სკრიპტი**, რომელიც **სრულად ავტომატურად** ქმნის მოდულის ჩონჩხს Synapse Framework-ის **ახალი არქიტექტურისთვის**. ის გენერირებს ყველა საჭირო ფაილს, სწორი სტრუქტურით, ფუნქციის პროტოტიპებით და კონფიგურაციით, რაც დეველოპმენტის პროცესს აჩქარებს და ამარტივებს.

## 🎯 რას აკეთებს?

### ✅ შექმნილი ფაილები (5 ფაილი)

1. **`module.json`** - მოდულის მეტამონაცემები (სახელი, ვერსია, ავტორი, შემქმნელი ფუნქცია).
2. **`CMakeLists.txt`** - გამარტივებული, პირობითი კომპილაციის ლოგიკა.
3. **`Kconfig`** - მოდულის ჩართვა/გამორთვის კონფიგურაცია `menuconfig`-ისთვის.
4. **`include/{module_name}.h`** - Public API-ს Header ფაილი.
5. **`src/{module_name}.c`** - Source ფაილი `base_module` ინტერფეისის სრული, მზა იმპლემენტაციით.

### ✅ ავტომატური ინტეგრაცია

- 🔧 **Build სისტემის მიერ აღმოჩენა** - `idf.py reconfigure` ბრძანებისას, build-სისტემა ავტომატურად პოულობს ახალ მოდულს.
- 🔧 **Kconfig-ში დამატება** - მოდულის `Kconfig` ფაილი ავტომატურად ერთვება პროექტის კონფიგურაციაში.
- 🔧 **პირობითი კომპილაცია** - კოდი კომპილირდება მხოლოდ იმ შემთხვევაში, თუ მოდული ჩართულია `menuconfig`-ში.

## 🛠️ გამოყენება

### 1. ძირითადი სინტაქსი

```bash
python3 scripts/create_module.py "მოდულის სრული სახელი" --category <კატეგორია> --description "მოკლე აღწერა"
```

### 2. მაგალითები

#### 📡 WiFi მოდული

```bash
python3 scripts/create_module.py "WiFi Manager" --category comms --description "WiFi კავშირის მართვა"
```

#### 🌡️ ტემპერატურის სენსორი

```bash
python3 scripts/create_module.py "DHT22 Temperature Sensor" --category sensors --description "ტემპერატურისა და ტენიანობის წაკითხვა"
```

#### 📺 დისპლეის დრაივერი

```bash
python3 scripts/create_module.py "OLED Display" --category displays --description "SSD1306 OLED დისპლეის დრაივერი" --author "შენი სახელი"
```

### 3. პარამეტრები

| პარამეტრი | არგუმენტი | აღწერა | აუცილებელი |
| :--- | :--- | :--- | :--- |
| `name` | `"WiFi Manager"` | მოდულის სრული, ადამიანისთვის გასაგები სახელი. სკრიპტი მას ავტომატურად გადაიყვანს `snake_case`-ში (`wifi_manager`). | ✅ |
| `--category` | `-c` | მოდულის კატეგორია. | ✅ |
| `--description` | `-d` | მოდულის მოკლე, ერთსიტყვიანი აღწერა. | ✅ |
| `--author` | | მოდულის ავტორი. თუ არ მიუთითებთ, გამოიყენებს Git-ის სახელს. | ❌ |

### 4. ხელმისაწვდომი კატეგორიები

- `utils` (დამხმარე)
- `comms` (კომუნიკაცია)
- `sensors` (სენსორები)
- `displays` (დისპლეები)
- `storage` (მეხსიერება)
- `security` (უსაფრთხოება)
- `testing` (სატესტო)

## 🏗️ არქიტექტურა და ლოგიკა (ახალი)

### 1. 🧠 პირობითი კომპილაცია (გამარტივებული)

ძველი, 3-ეტაპიანი CMake ლოგიკა ჩანაცვლდა ბევრად მარტივი და გასაგები პირობით:

```cmake
# ✅ ახალი, გამარტივებული ლოგიკა:
if(CONFIG_MODULE_WIFI_MANAGER_ENABLED)
    idf_component_register(
        SRCS "src/wifi_manager.c"
        INCLUDE_DIRS "include"
        REQUIRES
            core
            interfaces
    )
endif()
```

### 2. 📁 შექმნილი სტრუქტურა

სტრუქტურა უცვლელია, მაგრამ შიგთავსი განახლებულია:

```bash
components/modules/{category}/{module_name}/
├── CMakeLists.txt        # ახალი, მარტივი ლოგიკით
├── Kconfig               # მხოლოდ ჩართვა/გამორთვის ოფცია
├── module.json           # მეტამონაცემები (init_function-ით)
├── include/
│   └── {module_name}.h   # Public API (create ფუნქციით)
└── src/
    └── {module_name}.c   # იმპლემენტაცია (ახალი არქიტექტურით)
```

### 3. 🔗 Base Module Interface (კრიტიკულად განახლებული)

თითოეული მოდული ახლა იმპლემენტირებს `base_module.h`-ის განახლებულ, "ბრტყელ" ინტერფეისს:

```c
// მოდულის შემქმნელი ფუნქცია (Public)
module_t *{module_name}_create(const cJSON *config);

// Base Module ფუნქციები (Private)
static esp_err_t {module_name}_init(module_t *self);
static esp_err_t {module_name}_start(module_t *self);
static void {module_name}_deinit(module_t *self); // ყურადღება: აბრუნებს void!
static void {module_name}_handle_event(module_t *self, const char* event_name, void* event_data); // ყურადღება: აბრუნებს void!
```

### 4. 🎛️ ხელით მორგება (ახალი პატერნი)

ძველი, ჩადგმული `struct`-ების ნაცვლად, ახლა ვიყენებთ `private_data` პატერნს:

```c
// 1. გააფართოვეთ პრივატული მონაცემების სტრუქტურა:
typedef struct {
    // TODO: დაამატეთ თქვენი მოდულის მდგომარეობისთვის საჭირო ველები.
    int sensor_pin;
    TaskHandle_t sensor_task_handle;
    float last_reading;
} my_sensor_private_data_t;

// 2. გამოიყენეთ ის `_create` ფუნქციაში:
module_t* my_sensor_create(const cJSON *config) {
    // ...
    my_sensor_private_data_t *private_data = calloc(1, sizeof(my_sensor_private_data_t));
    module->private_data = private_data;
    // ...
    // წაიკითხეთ პინი კონფიგურაციიდან და შეინახეთ private_data->sensor_pin-ში
}

// 3. მიიღეთ წვდომა სხვა ფუნქციებში:
static esp_err_t my_sensor_init(module_t* self) {
    my_sensor_private_data_t *p_data = (my_sensor_private_data_t*)self->private_data;
    // ახლა შეგიძლიათ გამოიყენოთ p_data->sensor_pin
}
```

## 🔄 სრული Workflow (განახლებული)

### 1. მოდულის შექმნა

```bash
python3 scripts/create_module.py "My New Sensor" --category sensors --description "ახალი სენსორის მართვა"
```

### 2. სისტემის reconfigure

```bash
idf.py reconfigure
```

**რა ხდება:**

- Build-სისტემა **აღმოაჩენს** ახალ მოდულს `module.json`-ის მეშვეობით.
- **ამატებს** მის `Kconfig` ფაილს `menuconfig`-ში.

### 3. კონფიგურაცია

```bash
idf.py menuconfig
```

**სად ნახავთ:** `Component config` → `My New Sensor Module Configuration`

### 4. Build

```bash
idf.py build
```

**რა ხდება:**

- თუ მოდული ჩართულია: კომპილირდება სრული კოდი.
- თუ გამორთულია: `CMakeLists.txt`-ში არსებული `if` პირობა არ სრულდება და კოდი არ ემატება build-ში.

## 🚨 მნიშვნელოვანი შენიშვნები (განახლებული)

### ✅ რაც უნდა გახსოვდეთ

1. **ყოველთვის გაუშვით `idf.py reconfigure`** ახალი მოდულის შექმნის შემდეგ.
2. **მოდულის სახელი (`name`) უნდა იყოს ბრჭყალებში**, თუ ის ერთზე მეტ სიტყვას შეიცავს.
3. **`handle_event` და `deinit` ფუნქციები აბრუნებენ `void`**-ს. არ გამოიყენოთ `return ESP_OK`.
4. **Header ფაილი საჭიროა**, რათა System Manager-მა შეძლოს `_create` ფუნქციის გამოძახება.
5. **ყველა რესურსი უნდა გათავისუფლდეს `_deinit` ფუნქციაში**: `private_data`, `mutex`, `current_config` და ა.შ.

### ❌ ჩვეული შეცდომები

1. **`reconfigure`-ის გამოტოვება** → Build-სისტემა ვერ ხედავს ახალ მოდულს.
2. **`handle_event`-ში `esp_err_t`-ის დაბრუნება** → გამოიწვევს კომპილაციის შეცდომას.
3. **`module.json`-ის წაშლა ან არასწორი `init_function`** → მოდული ვერ ჩაიტვირთება.
4. **`private_data`-სთვის გამოყოფილი მეხსიერების არ გათავისუფლება `_deinit`-ში** → გამოიწვევს Memory Leak-ს.

## 🧪 ტესტირების მეთოდები

ტესტირების მეთოდები იგივე რჩება, მაგრამ ყურადღება მიაქციეთ განახლებულ `Kconfig` მენიუს და `CMake` ლოგიკას.

## 📊 განსხვავება სხვა სისტემებისგან

| ფუნქცია | Manual (ძველი) | create_module.py (ახალი) |
| :--- | :--- | :--- |
| ფაილების შექმნა | ხელით 5 ფაილი | ✅ ავტომატური |
| Interface იმპლემენტაცია | ხელით კოდის წერა | ✅ მზა, სწორი შაბლონი |
| პირობითი კომპილაცია | რთული, 3-ეტაპიანი CMake | ✅ მარტივი `if` პირობა |
| Kconfig ინტეგრაცია | ხელით რედაქტირება | ✅ ავტომატური |
| შეცდომების რისკი | მაღალი | ✅ მინიმალური |
| დახარჯული დრო | 20-30 წუთი | ✅ **10 წამი** |

---

## 🎯 შეჯამება

**create_module.py** არის **სწრაფი, უსაფრთხო და თანამედროვე** გზა Synapse Framework-ის მოდულების შესაქმნელად. ის:

✅ **აჩქარებს დეველოპმენტს** რადიკალურად.
✅ **გამორიცხავს შეცდომებს** არქიტექტურის არცოდნით.
✅ **უზრუნველყოფს კონსისტენტურობას** ყველა მოდულს შორის.
✅ **სრულად თავსებადია** უახლეს არქიტექტურასთან.

**გამოიყენეთ ის ყოველთვის**, როდესაც იწყებთ ახალ მოდულზე მუშაობას! 🚀
