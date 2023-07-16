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

#define WIFI_CONNECTED_BIT (1 << 0)
#define WIFI_GOT_IP_BIT (1 << 1)
#define URL "https://jsonplaceholder.typicode.com/todos/1"

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

static const char *TAG = "main";
static EventGroupHandle_t wifi_event_group;

void app_main(void)
{
  ESP_LOGI(TAG, "Starting app_main...")
  // initialize wifi event group
  wifi_event_group = xEventGroupCreate();

  /* Initialize NVS flash */
  esp_err_t ret = nvs_flash_init();

  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }

  ESP_ERROR_CHECK(ret);

  /* Initialize WiFi */
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
  assert(sta_netif);

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  /* Register WiFi event handler */
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler, NULL));

  /* Set WiFi configuration */
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

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

  /* Start WiFi */
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  /* Wait for WiFi connection and ip address before continuing */
  ESP_LOGI(TAG, "Waiting for WiFi connection...");
  (void)xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_GOT_IP_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

  ESP_LOGI(TAG, "WiFi connected");

  // start main loop
  ESP_LOGI(TAG, "Starting Main Loop...");

  char response_body[CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER] = {0};

  while (1)
  {
    // clear the response buffer before starting to prevent old data from being parsed
    memset(response_body, 0, CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER);

    // get the response from the server
    esp_err_t err = http_rest_client_get(URL, response_body, CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER);

    if (err != ESP_OK)
    {
      ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }

    // parse the response
    cJSON *response = cJSON_Parse(response_body);

    int userId = cJSON_GetObjectItem(response, "userId")->valueint;
    if (userId == 1)
    {
      ESP_LOGI(TAG, "Parsed correctly!");
    }

    // print the response
    ESP_LOGI(TAG, "RESPONSE: %s", cJSON_Print(response));

    // clean up json
    cJSON_Delete(response);

    ESP_LOGI(TAG, "Looping in 1 sec...");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
