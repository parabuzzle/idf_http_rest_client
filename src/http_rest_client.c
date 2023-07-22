/*
 * @file http_rest_client.c
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

#include "http_rest_client.h"

static const char *TAG = "http_rest_client";

static char *certificate;

esp_err_t http_rest_client_init_cert(char *cert, size_t cert_len)
{
  if (certificate != NULL)
  {
    ESP_LOGE(TAG, "Certificate already initialized");
    return ESP_FAIL;
  }

  certificate = malloc(cert_len + 1);
  if (certificate == NULL)
  {
    ESP_LOGE(TAG, "Failed to allocate memory for certificate");
    return ESP_FAIL;
  }
  strncpy(certificate, cert, cert_len);
  ESP_LOGD(TAG, "Initialized certificate");
  return ESP_OK;
}

esp_err_t http_rest_client_deinit_cert(void)
{
  free(certificate);
  ESP_LOGD(TAG, "Deinitialized certificate");
  return ESP_OK;
}

esp_err_t http_rest_client_get(char *url, http_rest_recv_buffer_t *http_rest_recv_buffer)
{

  esp_err_t ret = ESP_OK;

  esp_http_client_handle_t client;

  // Zero out the buffer for safety
  memset(http_rest_recv_buffer, 0, sizeof(http_rest_recv_buffer_t));

  ESP_LOGD(TAG, "Initializing client");

  esp_http_client_config_t config = {
      .url = url,
      .method = HTTP_METHOD_GET,
      .event_handler = http_event_handler,
#ifdef CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
      .crt_bundle_attach = esp_crt_bundle_attach,
#endif
      .user_agent = CONFIG_HTTP_REST_CLIENT_USER_AGENT,
      .user_data = http_rest_recv_buffer,
  };

  if (certificate != NULL)
  {
    config.cert_pem = certificate;
  }

  client = esp_http_client_init(&config);

  esp_http_client_set_header(client, "Content-Type", "application/json");

  ret = esp_http_client_perform(client);

  ESP_LOGD(TAG, "Get request complete");

  if (ESP_OK != ret)
  {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(ret));
    esp_http_client_cleanup(client);
    return ret;
  }

  int status_code = esp_http_client_get_status_code(client);

  http_rest_recv_buffer->status_code = status_code;

  ESP_LOGD(TAG, "Cleaning up client before returning");
  esp_http_client_cleanup(client);

  return ret;
}

esp_err_t http_rest_client_delete(char *url, http_rest_recv_buffer_t *http_rest_recv_buffer)
{

  esp_err_t ret = ESP_OK;

  esp_http_client_handle_t client;

  // Zero out the buffer for safety
  memset(http_rest_recv_buffer, 0, sizeof(http_rest_recv_buffer_t));

  ESP_LOGD(TAG, "Initializing client");

  esp_http_client_config_t config = {
      .url = url,
      .method = HTTP_METHOD_DELETE,
      .event_handler = http_event_handler,
#ifdef CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
      .crt_bundle_attach = esp_crt_bundle_attach,
#endif
      .user_agent = CONFIG_HTTP_REST_CLIENT_USER_AGENT,
      .user_data = http_rest_recv_buffer,
  };

  if (certificate != NULL)
  {
    config.cert_pem = certificate;
  }

  client = esp_http_client_init(&config);

  esp_http_client_set_header(client, "Content-Type", "application/json");

  ret = esp_http_client_perform(client);

  ESP_LOGD(TAG, "Get request complete");

  if (ESP_OK != ret)
  {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(ret));
    esp_http_client_cleanup(client);
    return ret;
  }

  int status_code = esp_http_client_get_status_code(client);

  http_rest_recv_buffer->status_code = status_code;

  ESP_LOGD(TAG, "Cleaning up client before returning");
  esp_http_client_cleanup(client);

  return ret;
}

esp_err_t http_rest_client_post(char *url, char *body_data, http_rest_recv_buffer_t *http_rest_recv_buffer)
{
  esp_err_t ret = ESP_OK;

  esp_http_client_handle_t client;

  // Zero out the buffer for safety
  memset(http_rest_recv_buffer, 0, sizeof(http_rest_recv_buffer_t));

  ESP_LOGD(TAG, "Initializing client");

  esp_http_client_config_t config = {
      .url = url,
      .method = HTTP_METHOD_POST,
      .event_handler = http_event_handler,
#ifdef CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
      .crt_bundle_attach = esp_crt_bundle_attach,
#endif
      .user_agent = CONFIG_HTTP_REST_CLIENT_USER_AGENT,
      .user_data = http_rest_recv_buffer,
  };

  if (certificate != NULL)
  {
    config.cert_pem = certificate;
  }

  client = esp_http_client_init(&config);

  esp_http_client_set_header(client, "Content-Type", "application/json");

  esp_http_client_set_post_field(client, body_data, strlen(body_data));

  ret = esp_http_client_perform(client);

  ESP_LOGD(TAG, "Get request complete");

  if (ESP_OK != ret)
  {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(ret));
    esp_http_client_cleanup(client);
    return ret;
  }

  int status_code = esp_http_client_get_status_code(client);

  http_rest_recv_buffer->status_code = status_code;

  ESP_LOGD(TAG, "Cleaning up client before returning");
  esp_http_client_cleanup(client);

  return ret;
}

esp_err_t http_rest_client_put(char *url, char *body_data, http_rest_recv_buffer_t *http_rest_recv_buffer)
{
  esp_err_t ret = ESP_OK;

  esp_http_client_handle_t client;

  // Zero out the buffer for safety
  memset(http_rest_recv_buffer, 0, sizeof(http_rest_recv_buffer_t));

  ESP_LOGD(TAG, "Initializing client");

  esp_http_client_config_t config = {
      .url = url,
      .method = HTTP_METHOD_PUT,
      .event_handler = http_event_handler,
#ifdef CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
      .crt_bundle_attach = esp_crt_bundle_attach,
#endif
      .user_agent = CONFIG_HTTP_REST_CLIENT_USER_AGENT,
      .user_data = http_rest_recv_buffer,
  };

  if (certificate != NULL)
  {
    config.cert_pem = certificate;
  }

  client = esp_http_client_init(&config);

  esp_http_client_set_header(client, "Content-Type", "application/json");

  esp_http_client_set_post_field(client, body_data, strlen(body_data));

  ret = esp_http_client_perform(client);

  ESP_LOGD(TAG, "Get request complete");

  if (ESP_OK != ret)
  {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(ret));
    esp_http_client_cleanup(client);
    return ret;
  }

  int status_code = esp_http_client_get_status_code(client);

  http_rest_recv_buffer->status_code = status_code;

  ESP_LOGD(TAG, "Cleaning up client before returning");
  esp_http_client_cleanup(client);

  return ret;
}

void http_rest_client_cleanup(http_rest_recv_buffer_t *http_rest_recv_buffer)
{
  if (http_rest_recv_buffer->buffer != NULL)
  {
    free(http_rest_recv_buffer->buffer);
  }
  ESP_LOGD(TAG, "Cleaned up http_rest_recv_buffer");
}
