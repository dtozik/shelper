cmake_minimum_required(VERSION 2.8)

project(zlib)

set(ZLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib-1.2.5)

set(DEBUG_BUILD_CONFIG 0)

set(MY_ZCALLOC 0)

set(SOURCE_FILES ${SOURCE_FILES}
        ${ZLIB_DIR}/adler32.c
        ${ZLIB_DIR}/compress.c
        ${ZLIB_DIR}/crc32.c
        ${ZLIB_DIR}/deflate.c
        ${ZLIB_DIR}/gzclose.c
        ${ZLIB_DIR}/gzlib.c
        ${ZLIB_DIR}/gzread.c
        ${ZLIB_DIR}/gzwrite.c
        ${ZLIB_DIR}/infback.c
        ${ZLIB_DIR}/inffast.c
        ${ZLIB_DIR}/inflate.c
        ${ZLIB_DIR}/inftrees.c
        ${ZLIB_DIR}/minigzip.c
        ${ZLIB_DIR}/trees.c
        ${ZLIB_DIR}/uncompr.c
        ${ZLIB_DIR}/zutil.c
)

include_directories(${ZLIB_DIR})

add_definitions(-Dunix)
add_definitions(-DUSE_FILE32API)
if(MY_ZCALLOC)
	add_definitions(-DMY_ZCALLOC)
endif()

#include(xCode)

# setup zlib target
add_library(zlib STATIC ${HEADER_FILES} ${SOURCE_FILES})

#add_xcode_attribute(zlib 1)
#fix_lib_output_location(zlib)