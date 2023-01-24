cmake_minimum_required(VERSION 3.0.0)
project(ServerUI VERSION 0.1.0) 

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

#include(${CMAKE_SOURCE_DIR}/build/conanbuildinfo.cmake)

add_subdirectory(src)

message([Main] "${CONAN_LIBS}**********************************************")