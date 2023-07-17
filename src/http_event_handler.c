/*
 * @file http_event_handler.c
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

#include "http_event_handler.h"

static const char *TAG = "http_rest_event_handler";

esp_err_t http_event_handler(esp_http_client_event_t *event_data)
{
  http_rest_recv_buffer_t *response_buffer = (http_rest_recv_buffer_t *)event_data->user_data;
  char *rcv_buffer = (char *)response_buffer->buffer;
  size_t rcv_buffer_len = response_buffer->buffer_len;

  if (rcv_buffer == 0)
  {
    return ESP_OK;
  }

  switch (event_data->event_id)
  {
  case HTTP_EVENT_ON_DATA:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", event_data->data_len);
    ESP_LOGV(TAG, "DATA: %s", (char *)event_data->data);

    if (rcv_buffer_len > 0)
    {
      if (event_data->data_len + strlen(rcv_buffer) >= rcv_buffer_len)
      {
        ESP_LOGE(TAG, "Receive buffer too small, needed %d, got %d", event_data->data_len, rcv_buffer_len);
        return ESP_FAIL;
      }
      ESP_LOGV(TAG, "Appending data to buffer");
      strncat(rcv_buffer, (char *)event_data->data, event_data->data_len);
    }
    break;

  default:
    ESP_LOGV(TAG, "HTTP_EVENT %d", event_data->event_id);
    break;
  }

  return ESP_OK;
}