# HTTP REST Client

A Client library for ESP32 and [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).

**This library is far from complete! It's missing some core features for REST (like DELETE and HEAD) and its probably fully of lurking C rookie mistakes. I only created it to abstract away some of the boilerplate of working with REST endpoints based on the ESP-IDF docs**

_feel free to open a Pull Request to add or fix things_

# Why?

ESP-IDF provides a robust [HTTP Client library](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_client.html)
already, why would I need this library too?

The answer is you don't need it. But because the provided client library is robust, it also means its a bit complicated and for most people, we only need some simple http operations for interacting with REST endpoints. You shouldn't need to know how to managed chunked vs non-chunked requests or how to construct a post request with the proper headers.

# Requirements

This was developed and tested on ESP-IDF 5.x (no guarantee it will work in earlier or future versions)

You will alos need to update your Max HTTP Request Header Length in the menuconfig or `sdkconfig` to something a bit higher than the default `512`.

_Note: Most modern browsers use something between `4096` and `8192` for reference. I find that `2048` is about right for most standard REST services, but tune this for your requirements._

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
