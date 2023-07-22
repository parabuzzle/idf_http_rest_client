FILE(GLOB_RECURSE app_sources "./src/*.c")

idf_component_register(
                    SRCS ${app_sources}
                    INCLUDE_DIRS "./include"
                    REQUIRES esp_http_client esp-tls json
                    )
