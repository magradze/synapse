# 🔌 Relay Actuator API Reference

## 🎯 მიზანი

ეს დოკუმენტი აღწერს `relay_actuator` მოდულის საჯარო ინტერფეისებს, რომელთა მეშვეობითაც შესაძლებელია მისი მართვა.

---

## 1. ⌨️ CLI (Command Line Interface)

`relay_actuator` მოდულის მართვის ძირითადი მეთოდი არის `Command Router`-ის მეშვეობით რეგისტრირებული `relay` ბრძანება.

- **ბრძანება:** `relay`
- **სინტაქსი:** `relay <instance_name> <action>`
- **არგუმენტები:**
  - `<instance_name>`: იმ რელეს უნიკალური სახელი, რომლის მართვაც გსურთ (განსაზღვრულია `system_config.json`-ში).
  - `<action>`: შესასრულებელი მოქმედება. შესაძლო მნიშვნელობებია:
    - `on`: რელეს ჩართვა.
    - `off`: რელეს გამორთვა.
    - `toggle`: მიმდინარე მდგომარეობის შეცვლა (თუ ჩართულია - გამორთავს, თუ გამორთულია - ჩართავს).

**მაგალითები:**

```bash
# "main_light" რელეს ჩართვა
SYNAPSE> relay main_light on

# "water_pump" რელეს გამორთვა
SYNAPSE> relay water_pump off

# "main_light" რელეს მდგომარეობის შეცვლა
SYNAPSE> relay main_light toggle
```

---

## 2. 📡 MQTT ინტერფეისი

მოდულის მართვა და მონიტორინგი შესაძლებელია MQTT-ით, `mqtt_manager`-თან ინტეგრაციის წყალობით.

### ბრძანების გაგზავნა

- **Topic:** `synapse/devices/{device_id}/cmd/in`
- **Payload:** იგივე სტრიქონი, რაც CLI-ში. მაგალითად: `"relay main_light on"`

### სტატუსის მიღება

- **Topic:** `synapse/devices/{id}/state/relay/{instance_name}/status`
- **Payload:** `{"state":"on"}` ან `{"state":"off"}`

---

## 3. 📢 ივენთები (Events)

- **`FMW_EVENT_RELAY_STATE_CHANGED`**: ეს ივენთი ქვეყნდება `Event Bus`-ზე ყოველ ჯერზე, როდესაც რელეს მდგომარეობა იცვლება. მისი `payload` არის `fmw_telemetry_payload_t` ტიპის, რომელიც შეიცავს რელეს `instance_name`-ს და მის ახალ სტატუსს JSON ფორმატში.
