/*
 * @file event_handlers.h
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

#pragma once

#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_wifi.h"

#define WIFI_CONNECTED_BIT (1 << 0)
#define WIFI_GOT_IP_BIT (1 << 1)

static EventGroupHandle_t wifi_event_group;

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
  switch (event_id)
  {
  case WIFI_EVENT_STA_START:
    ESP_LOGI("WIFI_EVENT", "WiFi station connecting...");
    xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    (void)esp_wifi_connect();
    break;
  case WIFI_EVENT_STA_DISCONNECTED:
    ESP_LOGI("WIFI_EVENT", "WiFi station disconnected");
    xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
    (void)esp_wifi_connect();
    break;
  default:
    break;
  }
}

void ip_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{

  switch (event_id)
  {
  case IP_EVENT_STA_GOT_IP:
    ESP_LOGD("WIFI_EVENT", "WiFi station got IP");
    xEventGroupSetBits(wifi_event_group, WIFI_GOT_IP_BIT);
    break;
  default:
    break;
  }
}