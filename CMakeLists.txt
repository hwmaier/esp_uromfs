idf_component_register(
    SRCS
        uromfs.c httphandler.c
    REQUIRES
        main # for urom.c to resolve
    INCLUDE_DIRS
        ${CMAKE_CURRENT_LIST_DIR}
)

include(${CMAKE_SOURCE_DIR}/cmake/compile_options.cmake)
