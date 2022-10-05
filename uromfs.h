/*
 * Mikro-ROM file system support for HTTP server.
 * @copyright © 2021 Henrik Maier. All rights reserved.
 * SPDX-License-Identifier: BSD-3-clause
 */

#pragma once
#include <stddef.h>

#include "fs/uromfs.h"


#ifdef __cplusplus
extern "C" {
#endif

ROMFILE *urom_fopen(const char *name, const char *mode);
int urom_fclose(ROMFILE *romf);
size_t urom_fread(void *buffer, size_t size, size_t count, ROMFILE *romf);
int urom_fseek(ROMFILE *romf, long pos, int whence);
long urom_filelength(ROMFILE *romf);

#ifdef __cplusplus
}
#endif
