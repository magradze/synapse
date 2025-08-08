# Synapse Manifesto

## Vision: A Manifest of Rigorous Embedded Architecture

Synapse is not just a framework. It is a **design philosophy** for building scalable, maintainable, and robust firmware on ESP32 platforms.

This manifesto outlines the principles that guide Synapse development:

> "We don't just ship working systems. We define the architectural discipline that governs them."

---

## 1. The Core is the Law — Architecture over Convention

At the heart of Synapse lies a deterministic architecture. The core does not dictate what modules should do — it defines **how** modules must operate.

* The `Module Factory` and `Registry` define an enforced lifecycle for every component.
* JSON + CMake-based integration auto-wires modules into the system without hidden wiring.
* Asynchronous operations are mediated through the `Promise Manager`, eliminating undefined behavior.

The core is **the contract** — not a suggestion.

## 2. Configuration as Contract — Not Just Metadata

Synapse treats configuration files (`config.json`, etc.) as declarative contracts:

* Each module declares its configuration schema and constraints.
* The system generates a single source of truth at build time.
* Configuration is not manually curated — it's discovered, embedded, and typed.

This promotes integrity across builds and removes hidden dependencies.

## 3. Asynchrony is Structural — Not an Afterthought

While many embedded platforms add async patterns as utility, Synapse treats **asynchrony as a first-class design constraint**:

* The `Promise Manager` ensures immutable async flow.
* Callbacks are explicit and deterministic — designed to avoid race conditions.
* Asynchronous interactions are not "opt-in" — they are systemically enforced.

The async system is part of the architecture — not a convenience library.

## 4. Extensible Without Being Generic

Synapse does not aim to be "everything for everyone". It is opinionated:

* It does not include UI abstractions, cloud services, or business logic.
* It does not prescribe transport layers (MQTT, HTTP, etc.).
* Instead, it **provides a composable, enforceable system** that ensures every layer obeys the rules.

Modules can be added or removed freely, but **they must conform** to the structure.

## 5. Evolution by Design

Synapse's build system (CLI + CMake + Codegen) allows the platform to grow without sacrificing discipline:

* New modules are discovered automatically.
* Configs are generated — not duplicated.
* The framework is ready for cloud compilation, module hot-swapping, and white-labeling scenarios.

It is **future-ready** by design — not by retrofit.

---

## Closing Statement

Synapse is not a library collection. It is a declaration of architectural intent:

* Modules obey lifecycle rules.
* Async operations are governed.
* Configuration is declared and enforced.
* The core remains untouched even as the surface grows.

This is not the easy path. This is the correct path.

> "We do not seek flexibility. We enforce structure to unlock true modularity."

Synapse is not just how you build firmware.

It's how you **discipline** firmware.
