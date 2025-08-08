/**
 * @file synapse.h
 * @brief Synapse ESP Framework - The Main Public Header.
 * @author Synapse Framework Team
 * @version 1.1.0
 * @date 2025-08-29
 *
 * @details
 * This is the primary and sole public header file for the Synapse Framework core.
 * Any module that needs to interact with the framework's foundational services
 * (such as the Event Bus, Service Locator, Config Manager, etc.) must include
 * only this single file.
 *
 * This "Facade" design pattern simplifies module implementation by hiding the
 * internal structure and dependencies of the core component, providing a clean
 * and stable API surface.
 *
 * @copyright Copyright (c) 2025
 */

#ifndef SYNAPSE_H
#define SYNAPSE_H

#ifdef __cplusplus
extern "C" {
#endif

  // =================================================================================================
  //      SECTION 1: FUNDAMENTAL TYPES & CONTRACTS
  // =================================================================================================
  // These headers define the core data structures and the "language" through which
  // modules and the framework communicate. They establish the fundamental contracts
  // for building any module within the Synapse ecosystem.

#include "base_module.h"      // Defines the basic module structure (module_t) and its lifecycle.
#include "service_types.h"    // Provides the centralized enum for all service types (synapse_service_type_t).
#include "framework_events.h" // Defines the names of standardized, framework-wide events.
#include "event_payloads.h"   // Defines the data structures (payloads) for standard events.

  // =================================================================================================
  //      SECTION 2: CORE SERVICE APIs
  // =================================================================================================
  // These headers declare the public functions for the framework's main services,
  // forming the primary interaction points for all modules.

#include "service_locator.h"  // For registering and discovering services (synapse_service_*).
#include "event_bus.h"        // For publishing and subscribing to events (synapse_event_bus_*).
#include "config_manager.h"   // For accessing configuration parameters (synapse_config_get_*).
#include "resource_manager.h" // For managing exclusive access to hardware resources like GPIO, I2C (synapse_resource_*).
#include "system_manager.h"   // For system-level control and module management (synapse_module_*, synapse_system_*).
#include "promise_manager.h"  // For consuming asynchronous operations using a clean, promise-based pattern (synapse_promise_*).
#include "task_pool_manager.h" // For scheduling jobs to be executed by a shared pool of worker tasks (synapse_task_pool_*).

  // =================================================================================================
  //      SECTION 3: UTILITIES & INTERNAL MECHANISMS
  // =================================================================================================
  // These headers provide essential utilities and expose internal mechanisms required for
  // the proper functioning and full integration of modules with the framework.

#include "logging.h"            // Provides the essential macro for logging (DEFINE_COMPONENT_TAG).
#include "event_data_wrapper.h" // For safe, reference-counted management of event data (synapse_event_data_*).
#include "module_helpers.h"     // Provides standard, reusable implementations for enable/disable/get_status.
#include "module_factory.h"     // For dynamically creating modules at runtime (synapse_module_create).
#include "module_registry.h"    // For accessing the module registry (synapse_module_registry_*).

// --- For Service Providers Only ---
// The following header is intended for modules that PROVIDE promise-based asynchronous services.
// General consumer modules should not need to include this directly.
#include "promise_manager_internal.h" // For creating and fulfilling promises (synapse_promise_create, synapse_promise_resolve).
#include "task_pool_manager_internal.h" // For internal task pool management functions (synapse_task_pool_init).

  // Note: Headers like `generated_module_factory.h`, `module_factory.h`, `module_registry.h`,
  // and `framework_config.h` are intentionally excluded. They are used internally by the core
  // system and are not part of the public API for modules.

#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_H