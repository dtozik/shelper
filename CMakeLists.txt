cmake_minimum_required (VERSION 2.8.11)
project (shelper)


set (LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/projects/libs)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_subdirectory (shelper)
