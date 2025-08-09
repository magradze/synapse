# Logger Module

## Overview

The `logger_module` is a centralized logging and event monitoring module designed for debugging and system observation purposes. It captures all events published on the Event Bus and provides comprehensive debugging output to the serial console, enabling real-time monitoring of system processes and inter-module communication.

## Features

- **Event Monitoring**: Captures and logs all events from the Event Bus
- **Runtime Control**: Enable/disable functionality via framework commands
- **Debug Architecture**: Demonstrates "listener" module pattern
- **Production Ready**: Can be disabled at compile time via Kconfig
- **Configurable**: Extensive Kconfig options for customization
- **Zero Dependencies**: No dependencies on other modules (follows framework architecture)

## Purpose

- **Event Monitoring**: Capture all events published on the Event Bus
- **Debugging**: Provide developers with real-time system communication information  
- **Architecture Demonstration**: Showcase a simple "listener" module implementation
- **Runtime Management**: Enable/disable logging via framework commands during operation

## Kconfig Configuration

The logger module supports extensive configuration through the ESP-IDF Kconfig system. Access these settings via:

```bash
idf.py menuconfig
# Navigate to: Synapse Framework → Logger Module Configuration
```

### Main Configuration Options

| Option | Default | Description |
|--------|---------|-------------|
| `MODULE_LOGGER_ENABLED` | Yes | Enable/disable entire module at compile time |
| `LOGGER_DEFAULT_INSTANCE_NAME` | "main_logger" | Default instance name |
| `LOGGER_INSTANCE_NAME_MAX_LEN` | 32 | Maximum length for instance names |
| `LOGGER_RUNTIME_CONTROL_ENABLED` | Yes | Enable runtime enable/disable commands |
| `LOGGER_CONSOLE_OUTPUT_ENABLED` | Yes | Enable console output |
| `LOGGER_EVENT_BUFFER_SIZE` | 128 | Buffer size for event formatting |
| `LOGGER_MAX_EVENT_DATA_SIZE` | 256 | Maximum event data size to process |

### Compilation Control

When `MODULE_LOGGER_ENABLED` is disabled:
- No logger module code is compiled into the firmware
- `logger_module_create()` returns NULL
- Zero memory and CPU overhead
- Recommended for production builds

## Runtime Configuration

The logger module can also be configured at runtime through `system_config.json`:

### Enable Logger Module

```json
{
  "type": "logger_module",
  "enabled": true,
  "config": {
    "instance_name": "main_logger"
  }
}
```

### Disable Logger Module

```json
{
  "type": "logger_module", 
  "enabled": false,
  "config": {
    "instance_name": "main_logger"
  }
}
```

## Runtime Control

The logger module supports runtime enable/disable functionality through framework command events.

### Command Format

Commands are sent through the framework's telemetry system using the `FRAMEWORK_EVENT_COMMAND_RECEIVED` event.

### Supported Commands

#### Disable Logger

```json
{
  "action": "disable"
}
```

#### Enable Logger

```json
{
  "action": "enable"
}
```

### Behavior

- **When Disabled**: Logger stops outputting event information to console
- **When Enabled**: Logger resumes outputting all captured events to console

## Implementation Details

### Module Structure

- **Type**: Utility/Service Module
- **Init Level**: 10 (Early initialization for comprehensive monitoring)
- **Dependencies**: Core framework, Interfaces, JSON parsing
- **Private Data**: None required

### Event Subscription

The module automatically receives all system events through the System Manager's universal event subscription mechanism. No manual event subscriptions are required.

### Output Format

Event logs follow this format:

```text
I (timestamp) LOGGER_MODULE: Event received: ID=12345
I (timestamp) LOGGER_MODULE: Processing command for module: main_logger
```

## Development Notes

- The module uses minimal memory footprint
- No file logging is implemented (console output only)
- Event data wrappers are not freed by the logger (other modules may need the data)
- Cognitive complexity has been reduced through function decomposition
- Full Doxygen documentation is provided for all public APIs
- No dependencies on other modules (follows framework architecture principles)

## API Reference

### Functions

#### `logger_module_create(const cJSON *config)`

Creates and initializes a new logger module instance.

**Parameters:**

- `config`: JSON configuration object containing module settings

**Returns:**

- `module_t*`: Pointer to created module instance, or NULL on failure

**Example:**

```c
cJSON *config = cJSON_Parse("{\"config\":{\"instance_name\":\"debug_logger\"}}");
module_t *logger = logger_module_create(config);
```

```
/mindlab/device/{device_id}/module/main_logger/cmd
```

### ბრძანებები

**ლოგერის გამორთვა:**

```json
{
  "action": "disable"
}
```

**ლოგერის ჩართვა:**

```json
{
  "action": "enable"
}
```

### ქცევა

- **გამორთვისას:** ლოგერი ჩერდება ყველა ივენთის ბეჭდვას და ESP-IDF-ის ლოგის დონე იყენება `ESP_LOG_NONE`-ზე
- **ჩართვისას:** ლოგერი იწყებს ივენთების ბეჭდვას და ESP-IDF-ის ლოგის დონე იყენება `ESP_LOG_INFO`-ზე

### მაგალითი

MQTT ბრძანების გაგზავნა:

```bash
mosquitto_pub -h 192.168.1.160 -t "/mindlab/device/5C013B7246E4/module/main_logger/cmd" -m '{"action": "disable"}'
```

ეს ბრძანება გამორთავს ლოგერ მოდულს და შეაჩერებს ყველა ლოგის ბეჭდვას.
