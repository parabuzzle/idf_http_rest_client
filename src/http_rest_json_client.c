/*
 * @file http_rest_json_client.c
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

#include "http_rest_json_client.h"

static const char *TAG = "http_rest_json_client";

esp_err_t http_rest_client_get_json(char *url, http_rest_recv_json_t *http_rest_recv_json)
{
  esp_err_t ret = ESP_OK;

  http_rest_recv_buffer_t http_rest_recv_buffer;

  ret = http_rest_client_get(url, &http_rest_recv_buffer);

  if (ESP_OK != ret)
  {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(ret));
    return ret;
  }

  if (http_rest_recv_buffer.status_code >= 300)
  {
    ESP_LOGE(TAG, "HTTP GET request failed with status code %d", http_rest_recv_buffer.status_code);
    return ESP_FAIL;
  }

  ESP_LOGD(TAG, "Parsing JSON");

  cJSON *json = cJSON_Parse((char *)http_rest_recv_buffer.buffer);

  if (json == NULL)
  {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)
    {
      ESP_LOGE(TAG, "Error before: %s", error_ptr);
    }
    ret = ESP_FAIL;
    goto cleanup;
  }

  http_rest_recv_json->json = json;
  http_rest_recv_json->status_code = http_rest_recv_buffer.status_code;

  ESP_LOGD(TAG, "JSON parsed");

  goto cleanup;

cleanup:
  http_rest_client_cleanup(&http_rest_recv_buffer);
  return ret;
}

esp_err_t http_rest_client_delete_json(char *url, http_rest_recv_json_t *http_rest_recv_json)
{
  esp_err_t ret = ESP_OK;

  http_rest_recv_buffer_t http_rest_recv_buffer;

  ret = http_rest_client_delete(url, &http_rest_recv_buffer);

  if (ESP_OK != ret)
  {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(ret));
    return ret;
  }

  if (http_rest_recv_buffer.status_code >= 300)
  {
    ESP_LOGE(TAG, "HTTP GET request failed with status code %d", http_rest_recv_buffer.status_code);
    return ESP_FAIL;
  }

  if (http_rest_recv_buffer.status_code == 204)
  {
    goto cleanup;
  }

  ESP_LOGD(TAG, "Parsing JSON");

  cJSON *json = cJSON_Parse((char *)http_rest_recv_buffer.buffer);

  if (json == NULL)
  {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)
    {
      ESP_LOGE(TAG, "Error before: %s", error_ptr);
    }
    ret = ESP_FAIL;
    goto cleanup;
  }

  http_rest_recv_json->json = json;
  http_rest_recv_json->status_code = http_rest_recv_buffer.status_code;

  ESP_LOGD(TAG, "JSON parsed");

  goto cleanup;

cleanup:
  http_rest_client_cleanup(&http_rest_recv_buffer);
  return ret;
}

esp_err_t http_rest_client_post_json(char *url, cJSON *body_json, http_rest_recv_json_t *http_rest_recv_json)
{
  esp_err_t ret = ESP_OK;

  char *body_data = cJSON_Print(body_json);

  http_rest_recv_buffer_t http_rest_recv_buffer;

  ret = http_rest_client_post(url, body_data, &http_rest_recv_buffer);

  free(body_data);

  if (ESP_OK != ret)
  {
    ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(ret));
    return ret;
  }

  if (http_rest_recv_buffer.status_code >= 300)
  {
    ESP_LOGE(TAG, "HTTP POST request failed with status code %d", http_rest_recv_buffer.status_code);
    return ESP_FAIL;
  }

  if (http_rest_recv_buffer.status_code == 204)
  {
    goto cleanup;
  }

  ESP_LOGD(TAG, "Parsing JSON");

  cJSON *json = cJSON_Parse((char *)http_rest_recv_buffer.buffer);

  if (json == NULL)
  {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)
    {
      ESP_LOGE(TAG, "Error before: %s", error_ptr);
    }
    ret = ESP_FAIL;
    goto cleanup;
  }

  http_rest_recv_json->json = json;
  http_rest_recv_json->status_code = http_rest_recv_buffer.status_code;

  ESP_LOGD(TAG, "JSON parsed");

  goto cleanup;

cleanup:
  http_rest_client_cleanup(&http_rest_recv_buffer);
  return ret;
}

esp_err_t http_rest_client_put_json(char *url, cJSON *body_json, http_rest_recv_json_t *http_rest_recv_json)
{
  esp_err_t ret = ESP_OK;

  char *body_data = cJSON_Print(body_json);

  http_rest_recv_buffer_t http_rest_recv_buffer;

  ret = http_rest_client_put(url, body_data, &http_rest_recv_buffer);

  free(body_data);

  if (ESP_OK != ret)
  {
    ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(ret));
    return ret;
  }

  if (http_rest_recv_buffer.status_code >= 300)
  {
    ESP_LOGE(TAG, "HTTP POST request failed with status code %d", http_rest_recv_buffer.status_code);
    return ESP_FAIL;
  }
  {
    ESP_LOGE(TAG, "HTTP POST request failed with status code %d", http_rest_recv_buffer.status_code);
    return ESP_FAIL;
  }

  ESP_LOGD(TAG, "Parsing JSON");

  cJSON *json = cJSON_Parse((char *)http_rest_recv_buffer.buffer);

  if (json == NULL)
  {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)
    {
      ESP_LOGE(TAG, "Error before: %s", error_ptr);
    }
    ret = ESP_FAIL;
    goto cleanup;
  }

  http_rest_recv_json->json = json;
  http_rest_recv_json->status_code = http_rest_recv_buffer.status_code;

  ESP_LOGD(TAG, "JSON parsed");

  goto cleanup;

cleanup:
  http_rest_client_cleanup(&http_rest_recv_buffer);
  return ret;
}

void http_rest_client_cleanup_json(http_rest_recv_json_t *http_rest_recv_json)
{
  if (http_rest_recv_json->json != NULL)
  {
    cJSON_Delete(http_rest_recv_json->json);
    http_rest_recv_json->status_code = 0;
  }
  ESP_LOGD(TAG, "Cleaned up http_rest_recv_json");
}