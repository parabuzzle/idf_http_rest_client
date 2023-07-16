/*
 * @file main.c
 * @author Michael Heijmans
 *
 * MIT License
 *
 * Copyright (c) 2023 Michael Heijmans
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <inttypes.h>
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "event_handlers.h"
#include "http_rest_client.h"
#include "cJSON.h"

#define URL "https://jsonplaceholder.typicode.com/todos/1"

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

static const char *TAG = "main";

void app_main(void)
{
  // create some variables we'll need
  esp_err_t ret = ESP_OK;
  char *response_body;

  ESP_LOGI(TAG, "Starting app_main...");

  // initialize wifi event group
  wifi_event_group = xEventGroupCreate();

  // Initialize NVS flash for wifi
  ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }

  // Check if wer're good to proceed
  ESP_ERROR_CHECK(ret);

  // Create the default event loop for WiFi driver
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  // Initialize network interface for WiFi Station mode
  ESP_ERROR_CHECK(esp_netif_init());
  (void)esp_netif_create_default_wifi_sta();

  // Configure WiFi radio
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  // Register WiFi event handlers
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler, NULL));

  // Set WiFi Station configuration
  wifi_config_t wifi_config = {
      .sta = {
          .ssid = WIFI_SSID,
          .password = WIFI_PASSWORD,
          .threshold.authmode = WIFI_AUTH_WPA2_PSK,
          .pmf_cfg = {
              .capable = true,
              .required = false},
      },
  };

  // Set WiFi mode to Station mode
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

  // Start the WiFi driver
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  // Wait for WiFi connection and ip address before continuing
  ESP_LOGI(TAG, "Waiting for WiFi connection...");
  (void)xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_GOT_IP_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

  ESP_LOGI(TAG, "WiFi connected");

  ESP_LOGI(TAG, "Starting Main Loop...");

  // start main loop
  while (1)
  {
    // Allocate memory on the heap for the response
    response_body = malloc(CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER);

    // get the response from the server
    ESP_LOGI(TAG, "Fetching Data from URL: %s", URL);
    ret = http_rest_client_get(URL, response_body, CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER);

    ESP_LOGI(TAG, "Raw Response string:\n%s", response_body);

    if (ret != ESP_OK)
    {
      ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(ret));
    }

    // parse the response
    cJSON *response = cJSON_Parse(response_body);

    int userId = cJSON_GetObjectItem(response, "userId")->valueint;
    if (userId == 1)
    {
      ESP_LOGI(TAG, "Parsed correctly!");
    }

    // print the response
    ESP_LOGI(TAG, "Response JSON:\n%s", cJSON_Print(response));

    // clean up json
    cJSON_Delete(response);

    // clean up memory
    free(response_body);

    ESP_LOGI(TAG, "Looping in 1 sec...");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
