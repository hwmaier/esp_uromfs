/*
 * Mikro-ROM file system support for HTTP server.
 *
 * Copyright (c) 2022 by Henrik Maier. All rights reserved.
 * Based on Nut/OS' uromfs.c from the Ethernet project, hence portions
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "uromfs.h"


#ifdef HTTP_UROM_TESTIMG_BIN
/* Adopted Michael Fischer's concept of a built-in performance test image */
static ROMENTRY testImage = {
    NULL, "testimg.bin", (1024L * 1024L), (const char*)0x08000000
};
#endif


/*!
 * \brief Read from device.
 */
size_t urom_fread(void *buffer, size_t size, size_t count, ROMFILE *romf)
{
    ROMENTRY *rome = romf->romf_entry;

    // We only support bytewise reads
    if (size != 1 || count == 0)
        return 0;

    size = count;

    if ((unsigned int) size > rome->rome_size - romf->romf_pos)
        size = rome->rome_size - romf->romf_pos;
    if (size) {
#ifdef HTTP_UROM_TESTIMG_BIN
        if (romf->romf_entry == &testImage) {
            // Avoid leaking memory content
            memset(buffer, 0, size);
        }
        else
#endif
        memcpy(buffer, (rome->rome_data + romf->romf_pos), size);
        romf->romf_pos += size;
    }
    return size;
}


/*!
 * \brief Open a file.
 */
ROMFILE *urom_fopen(const char *name, const char * mode)
{
    ROMENTRY *rome;
    ROMFILE *romf = 0;
    (void)mode;

    if (*name == '/') {
        name++;
    }

    for (rome = romEntryList; rome; rome = rome->rome_next) {
        if (strcmp(name, rome->rome_name) == 0)
            break;
    }
    if (rome) {
        if ((romf = calloc(1, sizeof(ROMFILE))) != 0)
        {
            romf->romf_entry = rome;
        }
    }

#ifdef HTTP_UROM_TESTIMG_BIN
    if (romf == 0) {
        if (strcmp(name, "testimg.bin") == 0) {
            if ((romf = calloc(1, sizeof(ROMFILE))) != 0) {
                romf->romf_entry = &testImage;
            }
        }
    }
#endif

   return romf;
}


/*!
 * \brief Close a file.
 */
int urom_fclose(ROMFILE *romf)
{
   if (romf)
      free(romf);

   return 0;
}


/*!
 * \brief Query file size.
 */
long urom_filelength(ROMFILE *romf)
{
    ROMENTRY *rome = romf->romf_entry;

    return (long) rome->rome_size;
}
