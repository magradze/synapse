<div align="center">
  <a href="https://github.com/espressif/esp-idf">
    <img src="https://raw.githubusercontent.com/espressif/esp-idf/master/docs/_static/espressif-logo.svg" width="120" alt="ESP-IDF Logo"/>
  </a>
  
# Synapse ESP Framework
  
  _A powerful, modular, and extensible platform for ESP32 devices._
  
  [![Manifesto](https://img.shields.io/badge/-Manifesto-black?style=for-the-badge&logo=gitbook)](manifesto.md)
  [![Getting Started](https://img.shields.io/badge/-Getting%20Started-blue?style=for-the-badge&logo=read-the-docs)](docs/tutorials/getting_started.md)
  [![API Reference](https://img.shields.io/badge/-API%20Reference-green?style=for-the-badge&logo=c)](docs/api_reference/api_index.md)
  [![Contributing](https://img.shields.io/badge/-Contributing-orange?style=for-the-badge&logo=github)](docs/contributing/contributing_guide.md)
</div>

---

## 🔥 Project Overview

Synapse is not just a framework. It is a **design philosophy** for building scalable, maintainable, and robust firmware. To understand the core principles that guide our development, please read our **[Manifesto](manifesto.md)**.

The Synapse ESP Framework is an open-source platform designed to simplify the development of complex IoT systems on the ESP32. It provides a modular architecture, flexible APIs, and enforces best practices for security and performance. The framework's key strengths are its **automatic module discovery** and **decentralized configuration**, making the development process exceptionally flexible and scalable.

---

## 🚀 Getting Started

1. **Prerequisites:**
    * [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) v5.4.1+
    * Python 3.x, Git

2. **Clone the Project:**

    ```sh
    git clone https://github.com/IoTesseract/synapse-esp.git
    cd synapse-esp
    ```

3. **Explore the Structure:**
    * See the [Directory Structure](docs/structure/directory_structure.md) for a detailed overview.
    * Key directories: `main/`, `components/`, `configs/`, `docs/`.

4. **Configure Your Modules:**
    * **New in v5.0.0+:** Modules are configured via their own `config.json` files, located at `components/modules/{category}/{module_name}/config.json`.
    * Follow the detailed instructions in the [Getting Started Guide](docs/tutorials/getting_started.md).

5. **Build and Flash:**
    * Use the standard ESP-IDF commands to build, flash, and monitor your application:

      ```sh
      idf.py build
      idf.py -p <YOUR_PORT> flash monitor
      ```

---

## 📚 API Reference & Documentation

The Synapse Framework is extensively documented to ensure a smooth development experience.

| Document | Description |
| :--- | :--- |
| 📖 **[API Reference Index](docs/api_reference/api_index.md)** | The complete reference for all public APIs, including Core, Modules, and System services. |
| 🏛️ **[Architectural Conventions](docs/convention/convention_index.md)** | The rules that govern the framework, from naming conventions to communication patterns. |
| 🏗️ **[Structural Blueprints](docs/structure/project_overview.md)** | An in-depth look at the project's structure, module architecture, and lifecycle. |
| 🤝 **[Contributing Guide](docs/contributing/contributing_guide.md)** | Everything you need to know to contribute to the project, including branching and commit guidelines. |

---

## ✨ Key Features

* **True Modularity:** All functionalities are independent modules that can be enabled or disabled at compile-time.
* **Strict Isolation:** Direct dependencies between modules are forbidden. Communication is mediated exclusively through the `Service Locator`, `Event Bus`, or `Promise Manager`.
* **Automated Integration:** A powerful CMake-based build system automatically discovers modules, generates the module factory, and integrates configurations.
* **Promise-based Asynchrony:** A clean, robust API for handling asynchronous request-response operations, eliminating "callback hell" and race conditions.
* **Centralized CLI:** A built-in `Command Router` allows any module to register commands, accessible via a unified serial shell.

---

## 🛡️ Security

Security is a first-class citizen in Synapse. We provide and enforce best practices to build secure IoT devices.

* **[Security Best Practices](docs/security/security_best_practices.md):** Our recommendations for secure development.
* **Key Principles:**
  * Use TLS/SSL for all external communication.
  * Store credentials securely using the `Storage Manager` (NVS backend).
  * Implement secure firmware updates via Signed OTA.
  * Validate all external inputs in every module.

---

## ⚡ Performance

The framework is designed to be lightweight and efficient, ensuring optimal performance even on resource-constrained devices.

* **[Performance Best Practices](docs/performance/performance_best_practices.md):** Guidelines for optimizing your application.
* **Key Principles:**
  * Use static allocation where possible to reduce heap fragmentation.
  * Leverage Kconfig to compile only the necessary modules, minimizing firmware size.
  * Utilize lazy initialization to conserve resources.

---

## 🛠️ Troubleshooting

* Encountering an issue? Check our **[Troubleshooting FAQ](docs/troubleshooting/faq.md)** for solutions to common problems.

---

## 📄 License

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.

---

<div align="center">
  <sub>For more detailed information, please refer to the <a href="docs/README.md">full documentation</a>.</sub>
</div>
