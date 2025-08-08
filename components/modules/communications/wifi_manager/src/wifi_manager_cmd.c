/**
 * @file wifi_manager_cmd.c
 * @brief Implements the CLI command handlers for the WiFi Manager module.
 * @author Synapse Framework Team
 * @version 1.0.0
 * @date 2025-09-02
 */
#include "wifi_manager_internal.h"
#include "cmd_router_interface.h" // For cmd_t

DEFINE_COMPONENT_TAG("WIFI_MANAGER_CMD");

// --- Static Forward Declarations ---
static esp_err_t wifi_cmd_handler(int argc, char **argv, void *context);

// --- CLI Handler ---
static void cli_status_then_cb(void *result_data, void *user_context)
{
  char *json_string = (char *)result_data;
  printf("---------------- WiFi Status (CLI) ----------------\n%s\n---------------------------------------------------\n", json_string);
}

static void cli_status_catch_cb(void *error_data, void *user_context)
{
  printf("Error: Failed to retrieve WiFi status via promise.\n");
}

static esp_err_t wifi_cmd_handler(int argc, char **argv, void *context)
{
  module_t *self = (module_t *)context;
  wifi_manager_private_data_t *private_data = (wifi_manager_private_data_t *)self->private_data;

  if (argc < 2)
  {
    printf("Usage: wifi <status|connect|disconnect|erase_creds>\n");
    return ESP_ERR_INVALID_ARG;
  }

  const char *sub_command = argv[1];
  ESP_LOGD(TAG, "Executing 'wifi' command with subcommand: %s", argv[1]);

  if (strcmp(sub_command, "status") == 0)
  {
    ESP_LOGI(TAG, "CLI command 'wifi status' received.");
    printf("Requesting WiFi status asynchronously...\n");
    esp_err_t err = wifi_api_get_status_async(self, cli_status_then_cb, cli_status_catch_cb, NULL);
    if (err != ESP_OK)
    {
      printf("Error: Could not start the status request operation.\n");
    }
    return err;
  }
  else if (strcmp(sub_command, "connect") == 0)
  {
    if (argc != 4)
    {
      printf("Usage: wifi connect <ssid> <password>\n");
      return ESP_ERR_INVALID_ARG;
    }
    if (save_credentials(self, argv[2], argv[3]) == ESP_OK)
    {
      load_credentials(self);
      start_wifi_connection(self);
    }
    return ESP_OK;
  }
  else if (strcmp(sub_command, "disconnect") == 0)
  {
    wifi_queue_msg_t msg = {.type = WIFI_CMD_DISCONNECT};
    xQueueSend(private_data->cmd_queue, &msg, 0);
    return ESP_OK;
  }
  else if (strcmp(sub_command, "erase_creds") == 0)
  {
    if (private_data->storage_handle)
    {
      private_data->storage_handle->erase_key("wifi_manager", "ssid");
      private_data->storage_handle->erase_key("wifi_manager", "password");
      printf("Credentials erased. Please reboot or provision the device.\n");
      memset(&private_data->wifi_config, 0, sizeof(wifi_config_t));
      private_data->has_saved_credentials = false;
    }
    else
    {
      printf("Error: Storage service not available.\n");
    }
    return ESP_OK;
  }

  printf("Error: Unknown or incomplete command.\n");
  return ESP_ERR_INVALID_ARG;
}

void register_cli_commands(module_t *self)
{
  static cmd_t wifi_command;
  wifi_command = (cmd_t){
      .command = "wifi",
      .help = "Manage and query WiFi status and configuration.",
      .usage = "wifi <status|connect|disconnect|erase_creds|...>",
      .min_args = 2,
      .max_args = 4,
      .handler = wifi_cmd_handler,
      .context = self};
  service_handle_t handle = synapse_service_get("main_cmd_router");
  if (handle)
  {
    ((cmd_router_api_t *)handle)->register_command(&wifi_command);
  }
}