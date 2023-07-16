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

esp_err_t http_rest_client_get(char *url, char *response, size_t response_len)
{

  esp_err_t ret = ESP_OK;

  // char response_buffer[CONFIG_HTTP_REST_CLIENT_MAX_RECEIVE_BUFFER];
  static esp_http_client_handle_t client;

  esp_http_client_config_t config = {
      .url = url,
      .method = HTTP_METHOD_GET,
      .event_handler = http_event_handler,
      .user_agent = "IDF HTTP REST Client/0.1",
      .user_data = response,
  };

#ifdef CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
  config.crt_bundle_attach = esp_crt_bundle_attach;
#endif

  if (certificate != NULL)
  {
    config.cert_pem = certificate;
  }

  client = esp_http_client_init(&config);
  ret = esp_http_client_perform(client);

  ESP_LOGD(TAG, "Get request complete");

  if (ESP_OK != ret)
  {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(ret));
    esp_http_client_cleanup(client);
    return ret;
  }

  // ESP_LOGD(TAG, "DATA BUFFER LEN: %d", strlen(response_buffer));

  ESP_LOGD(TAG, "Cleaning up client before returning");
  // strncpy(response, response_buffer, response_len);
  esp_http_client_cleanup(client);
  return ret;
}