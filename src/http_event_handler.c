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
  switch (event_data->event_id)
  {
  case HTTP_EVENT_ON_DATA:
  {
    http_rest_recv_buffer_t *response_buffer = (http_rest_recv_buffer_t *)event_data->user_data;

    ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", event_data->data_len);
    ESP_LOGV(TAG, "DATA: %s", (char *)event_data->data);

    // Increase the buffer size to fit the new data
    response_buffer->buffer = realloc(response_buffer->buffer, response_buffer->buffer_len + event_data->data_len + 1);
    ESP_LOGV(TAG, "Buffer realloced to %d bytes", response_buffer->buffer_len + event_data->data_len + 1);

    // Copy the new data to the buffer
    memcpy(response_buffer->buffer + response_buffer->buffer_len, (uint8_t *)event_data->data, event_data->data_len);
    ESP_LOGV(TAG, "Data copied to buffer");

    // Increase the buffer length
    response_buffer->buffer_len += event_data->data_len;
    ESP_LOGV(TAG, "Buffer length increased to %d bytes", response_buffer->buffer_len);

    // Add a null terminator to the end of the buffer
    response_buffer->buffer[response_buffer->buffer_len] = '\0';
    ESP_LOGV(TAG, "Null terminator added to buffer");
  }
  break;

  default:
    ESP_LOGV(TAG, "HTTP_EVENT %d", event_data->event_id);
    break;
  }

  return ESP_OK;
}