/*
 * Mikro-ROM file system support for ESP-IDF HTTP server.
 * @copyright © 2022 Henrik Maier. All rights reserved.
 * SPDX-License-Identifier: BSD-3-clause
 */

#pragma once

#include "esp_err.h"
#include "esp_http_server.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t httpd_on_urom_request(httpd_req_t *req);

#ifdef __cplusplus
}
#endif
