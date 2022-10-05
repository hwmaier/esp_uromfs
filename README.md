# Mikro-ROM File System support for ESP-IDF

The *Mikro-ROM File System for ESP-IDF* implements a minimalistic in-ROM file system. It offers an alternative to the more feature-rich VFS layer of ESP-IDF and uses a much simpler file system structure which uses significantly less code space. Also there is no support for directories nor support for writing. It is intended to be used with the ESP-IDF HTTP server.

The file system content is compiled into a firmware application as a C source file. The `crurom` utility (see `tools` sub-folder) is used to convert a directory of files into such a C source file.


## Usage

Register the handler in your HTTP server start function as follows:

        // URI handler for uROM file system
        httpd_uri_t urom_handler_ctx = {
            .uri = "/*", .method = HTTP_GET, .handler = httpd_on_urom_request
        };
        httpd_register_uri_handler(httpdHdl, &urom_handler_ctx);


Use the `crurom` command line tool to create a C source file with the binary content of the files contained in a subdirectory, for example `www`:

    crurom www

`crurom` will create a `urom.c` C source file. Add the `urom.c` source file to the main program's CMakeLists.txt file.


## Credits

The Mikro-ROM file system has been created by the unrelated [Ethernut](http://www.ethernut.de/) project and parts of this library contain verbatim copies of code from the Ethernut project. 
Refer to file `NOTICE`.
