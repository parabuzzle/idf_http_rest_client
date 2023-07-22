/*
 * @file http_rest_json_client.h
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

#include "http_rest_client.h"
#include "cJSON.h"

esp_err_t http_rest_client_get_json(char *url, http_rest_recv_json_t *http_rest_recv_json);
esp_err_t http_rest_client_delete_json(char *url, http_rest_recv_json_t *http_rest_recv_json);
esp_err_t http_rest_client_post_json(char *url, cJSON *body_json, http_rest_recv_json_t *http_rest_recv_json);
esp_err_t http_rest_client_put_json(char *url, cJSON *body_json, http_rest_recv_json_t *http_rest_recv_json);

void http_rest_client_cleanup_json(http_rest_recv_json_t *http_rest_recv_json);