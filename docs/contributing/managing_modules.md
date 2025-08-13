# მოდულების მართვა Git Submodule-ის საშუალებით

## 1. 🏛️ არქიტექტურული პრინციპი

Synapse Framework-ის არქიტექტურა ეფუძნება **მოდულების სრულ დამოუკიდებლობას**. ამის მისაღწევად, თითოეული მოდული ინახება საკუთარ, ცალკე Git რეპოზიტორიაში და მთავარ პროექტს (`synapse-esp`) ემატება **Git Submodule**-ის მექანიზმით.

ეს მიდგომა გვაძლევს რამდენიმე უმნიშვნელოვანეს უპირატესობას:

- **დამოუკიდებელი ვერსიონირება:** თითოეულ მოდულს აქვს საკუთარი ვერსიების ისტორია.
- **კოდის გადაყენებადობა:** ერთი და იგივე მოდული შეიძლება, გამოყენებულ იქნას მრავალ სხვადასხვა პროექტში.
- **წვდომის კონტროლი:** შესაძლებელია, ზოგიერთი მოდული იყოს პრივატული, ზოგი კი — საჯარო.
- **სუფთა ისტორია:** მთავარი პროექტის `git log` არ იტვირთება მოდულების შიდა ცვლილებებით.

## 2. 🚀 ახალი მოდულის დამატება

პროცესი შედგება ორი ძირითადი ეტაპისგან:

### ეტაპი 1: მოდულის რეპოზიტორიის მომზადება

სანამ მოდულს დაამატებთ, დარწმუნდით, რომ ის არსებობს როგორც დამოუკიდებელი Git რეპოზიტორია (მაგ., GitHub-ზე) და მიჰყვება Synapse-ის [მოდულის სტრუქტურის კონვენციას](../convention/module_structure.md).

### ეტაპი 2: Submodule-ის დამატება მთავარ პროექტში

1. **გადადით პროექტის root დირექტორიაში.**

2. **(თუ საჭიროა) წაშალეთ არსებული ლოკალური ვერსია:**
    თუ მოდული უკვე არსებობს `components/modules/` დირექტორიაში, როგორც ჩვეულებრივი საქაღალდე, წაშალეთ ის:

    ```bash
    rm -rf components/modules/{category}/{module_name}
    ```

3. **დაამატეთ Submodule:**
    გამოიყენეთ `git submodule add` ბრძანება. მიუთითეთ რეპოზიტორიის URL და ლოკალური გზა.

    ```bash
    git submodule add <repository_url> components/modules/{category}/{module_name}
    ```

    **მაგალითი:**

    ```bash
    git submodule add git@github.com:YourUsername/synapse_module_relay.git components/modules/actuators/relay_actuator
    ```

4. **ცვლილებების დაკომიტება:**
    ეს ბრძანება ქმნის ორ ფაილს: `.gitmodules` და ახალ ჩანაწერს Git-ის ინდექსში. დააკომიტეთ ეს ცვლილება:

    ```bash
    git add .gitmodules components/modules/{category}/{module_name}
    git commit -m "feat(modules): Add {{module_name}} as a submodule"
    git push
    ```

### ⚙️ Build სისტემასთან ინტეგრაცია

Synapse Framework-ის `CMake` build სისტემა **ავტომატურად** აღმოაჩენს `components/modules/` დირექტორიაში არსებულ ყველა მოდულს. შესაბამისად, `submodule`-ის დამატების შემდეგ, **არანაირი დამატებითი კონფიგურაცია საჭირო არ არის**. `idf.py menuconfig` და `idf.py build` ბრძანებები ჩვეულებრივად იმუშავებს.

## 3. 🔄 პროექტის კლონირება Submodule-ებთან ერთად

როდესაც პროექტს, რომელიც `submodule`-ებს შეიცავს, ახალ კომპიუტერზე კლონავთ, გამოიყენეთ `--recurse-submodules` ფლაგი:

```bash
git clone --recurse-submodules <synapse_project_url>
```

თუ პროექტი უკვე დაკლონილია, გაუშვით:

```bash
git submodule update --init --recursive
```

## 4. 📈 Submodule-ის განახლება

როდესაც მოდულის საკუთარ რეპოზიტორიაში ცვლილებას შეიტანთ და გსურთ, რომ მთავარმა პროექტმა ეს ცვლილება "დაინახოს":

1. **გადადით submodule-ის დირექტორიაში:**

    ```bash
    cd components/modules/{category}/{module_name}
    ```

2. **განაახლეთ კოდი (მაგ., `main` branch-იდან):**

    ```bash
    git pull origin main
    ```

3. **დაბრუნდით მთავარ პროექტში:**

    ```bash
    cd ../../../..
    ```

4. **დააკომიტეთ submodule-ის განახლებული ვერსია:**
    მთავარი რეპოზიტორია ახლა ხედავს, რომ `submodule` მიუთითებს ახალ `commit`-ზე.

    ```bash
    git add components/modules/{category}/{module_name}
    git commit -m "chore(modules): Update {{module_name}} to latest version"
    git push
    ```

## 5. 💡 დამატებითი რჩევები

- **VSCode:** თუ იყენებთ VSCode-ს, `Git` გაფართოება, როგორც წესი, კარგად მუშაობს `submodule`-ებთან და ვიზუალურად გაჩვენებთ ცვლილებებს.
- **CI/CD:** თქვენს CI/CD სკრიპტებში (მაგ., GitHub Actions), დარწმუნდით, რომ `checkout` ეტაპზე იყენებთ `--recurse-submodules` ფლაგს, რათა ყველა მოდული ხელმისაწვდომი იყოს `build`-ისთვის.
