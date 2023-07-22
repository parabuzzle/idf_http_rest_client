# HTTP REST Client

A Client library for ESP32 and [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).

**This library is far from complete! It's missing some core features for REST (like authentication...) and its probably full of lurking C rookie mistakes, security holes, and more bugs than the amazon rainforest. I only created it to abstract away some of the boilerplate of working with REST endpoints based on the ESP-IDF docs**

_feel free to open a Pull Request to add or fix things_

# Why?

ESP-IDF provides a robust [HTTP Client library](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_client.html)
already, why would I need this library too?

The answer is you don't need it. You probably don't even want it. But because the provided client library is robust, it also means its a bit complicated and for most people, we only need some simple http operations for interacting with REST endpoints. You shouldn't need to know how to manage chunked vs non-chunked responses or how to construct the required connection config to post a json object. I just want the ENTIRE response body loaded in a buffer I provide to do with what I want. And i don't want to copy around 40 lines of code for every request to do it!

I made this for me. If you find use in it, GREAT! If you want to improve it for you, EVEN BETTER!

# Requirements

This was developed and tested on ESP-IDF 5.0.2 and 5.1 (no guarantee it will work in earlier or future versions)

You will also will likely need to update your Max HTTP Request Header Length in the menuconfig or `sdkconfig` to something a bit higher than the default `512`.

_Note: Most modern browsers use something between `4096` and `8192` for reference. I find that `2048` is about right for most standard REST services out in the wild, but tune this for your requirements._

# Installation

Using the IDF Component Registry (Recommended)

```
idf.py add-dependency "parabuzzle/http_rest_client^0.1.0"
```

Clone a local copy to your project

```
mkdir components
cd components
git clone https://github.com/parabuzzle/idf_http_rest_client.git

## OR Use Git Submodules
git submodule add https://github.com/parabuzzle/idf_http_rest_client.git
```

# Configuration

All configuration is provided through `menuconfig` and the Kconfig.projbuild file.

# Usage

You can find a runnable working example in the [examples](/examples) directory

This is a basic idea of how it works:

_yes, its literally a single line to load the buffer with the response!_

```c
#include "http_rest_client.h"

void app_main(void)
{

  while (1)
  {
    http_rest_recv_buffer_t response_buffer = {0};

    // do the request
    ESP_ERROR_CHECK(http_rest_client_get("https://catoftheday.com/", &response_buffer));

    // print response status
    sprintf("Status Code %d", response_buffer.status_code);

    // do something with the data
    printf(response_buffer.buffer);

    // clean up
    http_rest_client_cleanup(&response_buffer);

    // wait and loop
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

}
```

## HTTPS?

Yep!

If you have the mbed-tls certificate bundle enabled for the chip in sdkconfig, I just load it in automatically. You don't need to do anything to do https requests aside from make sure `CONFIG_MBEDTLS_CERTIFICATE_BUNDLE` is enabled.

What if you want to provide your own cert bundle pem or you can't use the mbed-tls bundle? I got you beau!

You just need to initialize the pem in the library like this:

```c
#include "http_rest_client.h"

void app_main(void)
{
  char *cert = "----begin certificate--- etc"

  // initialize a cert bundle
  ESP_ERROR_CHECK(http_rest_client_init_cert(cert, strlen(cert)));

  // go about your business...

  // remove the attached bundle ... why? I don't know... but you _can_
  ESP_ERROR_CHECK(http_rest_client_deinit_cert());
}
```

## Do you have a JSON Example?

You Betcha'

You can send that response object directly to cJSON:

```c

#include "http_rest_json_client.h"

void app_main(void)
{

  http_rest_recv_json_t response_buffer = {0};

  // do the request
  ESP_ERROR_CHECK(http_rest_client_get_json("https://rickandmortyapi.com/api/character/1", &response_buffer));

  if (response_buffer.status_code != 200){
    ESP_LOGE(TAG, "an http error occured: %d", response_buffer.status_code);
  }
  else
  {
    char *jsonString = cJSON_Print(response_buffer.json);
    ESP_LOGI(TAG, "Response: %s", jsonString);
    free(jsonString);
  }

  http_rest_client_cleanup_json(&response_buffer);
}

```

What if you want to post json data? yea, it does that too:

```c
#include "http_rest_json_client.h"

void app_main(void)
{

  http_rest_recv_json_t response_buffer = {0};
  cJSON *json_body = cJSON_CreateObject();
  cJSON_AddStringToObject(json_body, "hello", "world");

  // do the request
  ESP_ERROR_CHECK(http_rest_client_get_json("https://httpbin.org/anything", json_body, &response_buffer));

  if (response_buffer.status_code != 200){
    ESP_LOGE(TAG, "an http error occured: %d", response_buffer.status_code);
  }
  else
  {
    char *jsonString = cJSON_Print(response_buffer.json);
    ESP_LOGI(TAG, "Response: %s", jsonString);
    free(jsonString);
    cJSON_Delete(json_body);
  }

  http_rest_client_cleanup_json(&response_buffer);
}

```

# License

MIT License

Copyright (c) 2023 Michael Heijmans

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
