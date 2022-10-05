#
# CMake file to build the crurom command line tool
#

cmake_minimum_required(VERSION 3.0)

project (crurom)
add_executable(${PROJECT_NAME} 
    crurom.c 
    # Following only required for Windows/MSVC
    $<$<C_COMPILER_ID:MSVC>: dirent.c getopt.c>
)
