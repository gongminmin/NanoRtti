# Copyright (c) Minmin Gong. All rights reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.18)

set(NR_MAJOR_VERSION 0)
set(NR_MINOR_VERSION 1)
set(NR_PATCH_VERSION 0)
set(NR_VERSION ${NR_MAJOR_VERSION}.${NR_MINOR_VERSION}.${NR_PATCH_VERSION})

project(NanoRtti VERSION "${NR_VERSION}" LANGUAGES CXX)

option(NANO_RTTI_BUILD_TESTS "Build tests of NanoRtti" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NR_CLANGFORMAT)
    find_program(CLANG_FORMAT
        NAMES
            clang-format-10
            clang-format
    )

    if(${CLANG_FORMAT} STREQUAL "CLANG_FORMAT-NOTFOUND")
        message(FATAL_ERROR "No clang-format tool found")
    endif()

    file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h *.hpp)
    add_custom_target(clangformat
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Formatting with ${CLANG_FORMAT} ..."
    )

    return()
endif()

add_library(NanoRtti INTERFACE)
target_include_directories(NanoRtti INTERFACE Include)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set(nano_rtti_files
        Include/NanoRtti/NanoRtti.hpp
        Include/NanoRtti/Detail/Detail.hpp
    )
    source_group(TREE ${PROJECT_SOURCE_DIR}/Include FILES ${nano_rtti_files} PREFIX "Header Files")

    target_sources(NanoRtti
        PRIVATE
            ${nano_rtti_files}
    )
endif()

if (NANO_RTTI_BUILD_TESTS)
    add_subdirectory(Test)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "Test")
