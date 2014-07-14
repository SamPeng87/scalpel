cmake_minimum_required(VERSION 3.0.0)
PROJECT(scalpel)

SET(PROJECT_DIRECTORY_LIST src)

#include directories
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/include
    
    /usr/local/include
    /usr/include
    )

#lib directories
LINK_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/build/lib

    /usr/local/lib
    /usr/lib
    )
#global var
#SET(CMAKE_C_FLAGS "-std=c99 -O2 -pipe -Wall -Wextra")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_DEBUG_POSTFIX _debug)
SET(CMAKE_VERBOSE_MAKEFILE ON)

#add sub directories
ADD_SUBDIRECTORY(${PROJECT_DIRECTORY_LIST})
