# `crurom` - Create Mikro-ROM Tool 

A command line tool to convert a directory of files into a Mikro-ROM file system. 
The file system is contained in a single C source file which can be compiled into an embedded application.


## Usage

    Usage: crurom OPTIONS DIRECTORY
    OPTIONS:
    -cp       create __PROG_TYPES_COMPAT__
    -ed       encode to decimal values
    -eh       encode to hex values
    -es       encode to strings
    -eu       ugly but compact encoding
    -l <num>  characters per line
    -o <file> output file
    -r        recursive
    -v        verbose


## Credits

The Mikro-ROM file system has been created by the [Ethernut](http://www.ethernut.de/) project and this *crurom* tool has been copied without modification from the Ethernut project (version 5.2.4).
Refer to file `COPYING`.
