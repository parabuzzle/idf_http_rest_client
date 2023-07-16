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

static char rcv_buffer[CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER];

esp_err_t http_event_handler(esp_http_client_event_t *event_data)
{
  switch (event_data->event_id)
  {
  case HTTP_EVENT_ON_CONNECTED:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
    ESP_LOGV(TAG, "Clearing response buffer");
    memset(rcv_buffer, 0, CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER);
    ESP_LOGV(TAG, "Cleared response buffer");
    break;

  case HTTP_EVENT_ON_DATA:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", event_data->data_len);
    ESP_LOGV(TAG, "DATA: %s", (char *)event_data->data);

    ESP_LOGV(TAG, "Concatenating data to response buffer");
    if (CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER >= strlen(rcv_buffer) + strlen((char *)event_data->data))
    {
      strncat(rcv_buffer, (char *)event_data->data, event_data->data_len);
    }
    else
    {
      ESP_LOGE(TAG, "The provided response buffer is too small to hold the data in the response");
      ESP_LOGE(TAG, "Response buffer size: %d", CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER);
      ESP_LOGE(TAG, "Data size: %d", strlen((char *)event_data->data));
      return ESP_ERR_INVALID_SIZE;
    }
    ESP_LOGV(TAG, "Concatenation complete");
    break;

  case HTTP_EVENT_ON_FINISH:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
    ESP_LOGD(TAG, "DATA BUFFER LEN: %d", strlen(rcv_buffer));
    ESP_LOGV(TAG, "DATA BUFFER: %s", rcv_buffer);

    if (strlen(rcv_buffer) <= CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER)
    {
      ESP_LOGV(TAG, "COPY data to recieve buffer from response buffer");
      strncpy(event_data->user_data, rcv_buffer, CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER);
      ESP_LOGV(TAG, "COPY complete");
    }
    else
    {
      ESP_LOGE(TAG, "The provided Recieve buffer is too small to hold the data in the response buffer");
      ESP_LOGE(TAG, "Recieve buffer size: %d", CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER);
      ESP_LOGE(TAG, "Response buffer size: %d", strlen(rcv_buffer));
      return ESP_ERR_INVALID_SIZE;
    }
    break;

  default:
    ESP_LOGV(TAG, "HTTP_EVENT %d", event_data->event_id);
    break;
  }

  return ESP_OK;
}