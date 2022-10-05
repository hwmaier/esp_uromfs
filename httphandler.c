/*
 * Mikro-ROM file system support for ESP-IDF HTTP server.
 * @copyright © 2022 Henrik Maier. All rights reserved.
 * SPDX-License-Identifier: BSD-3-clause
 */

#include "esp_log.h"
#include "uromfs.h"
#include "uromhttp.h"

#define CHUNK_BUFSIZE 1024

static const char TAG[] = "urom";


/**
 * Definition of MIME types for well knwon file extensions
 */
static const struct { const char *ext; const char *type; } mimeTypeMap[] = {
    {".html", "text/html"},
    {".htm", "text/html"},
    {".gif", "image/gif"},
    {".jpg", "image/jpeg"},
    {".ico", "image/x-icon"},
    {".png", "image/png"},
    {".js", "application/x-javascript"},
    {".css", "text/css"},
    {".svg", "image/svg+xml"},
};


/**
 * @brief Set the content type object
 *
 * @param req HTTP request being responded to
 * @param fileName File name of HTTP GET request
 * @return esp_err_t Return code from httpd_resp_set_type()
 */
static esp_err_t set_content_type(httpd_req_t *req, const char *fileName)
{
    for (int i = 0; i < sizeof(mimeTypeMap) / sizeof(*mimeTypeMap); i++)
    {
        if (strcasestr(fileName, mimeTypeMap[i].ext))
        {
            return httpd_resp_set_type(req, mimeTypeMap[i].type);
        }
    }

    return httpd_resp_set_type(req, "text/plain");
}


/**
 * Serve files from Mikro-ROM file system
 *
 * @param req HTTP request being responded to
 * @return esp_err_t  ESP_OK on success or ESP_FAIL on error
 */
esp_err_t httpd_on_urom_request(httpd_req_t *req)
{
    const char *fileName;
    bool firstChunkSent = false;

    //
    // Determine file name
    //
    if (strcmp(req->uri, "/") == 0)
        fileName = "index.html";
    else
    {
        fileName = req->uri;
        // Remove query string
        char *query = strchr(fileName, '?');
        if (query)
        {
            *query = '\0';
        }
        // Remove fragment string
        char *fragment = strchr(fileName, '#');
        if (fragment)
        {
            *fragment = '\0';
        }
    }

    // File requests always close...
    httpd_resp_set_hdr(req, "Connection", "close");

    ROMFILE *fp = urom_fopen(fileName, "r");
    if (fp == 0)
    {
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    char *chunkBuf = malloc(CHUNK_BUFSIZE);
    if (!chunkBuf)
    {
        urom_fclose(fp);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Out of memory");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Sending file : %s (%ld bytes)...", fileName, urom_filelength(fp));
    set_content_type(req, fileName);

    size_t chunkSize;
    do
    {
        // Read file in chunks into the scratch buffer
        chunkSize = urom_fread(chunkBuf, 1, CHUNK_BUFSIZE, fp);
        if (chunkSize != 0)
        {
            // Send the buffer contents as HTTP response chunk
            if (httpd_resp_send_chunk(req, chunkBuf, chunkSize) == ESP_OK)
            {
                firstChunkSent = true;
            }
            else
            {
                free(chunkBuf);
                urom_fclose(fp);
                if (firstChunkSent)
                    httpd_resp_send_chunk(req, NULL, 0); // Terminate chunk
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                return ESP_FAIL;
            }
        }
    } while (chunkSize != 0);
    free(chunkBuf);
    urom_fclose(fp);

    // Respond with an empty chunk to signal HTTP response completion
    if (firstChunkSent)
        httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}
