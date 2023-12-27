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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NR_CLANGFORMAT)
    find_program(clang_format
        NAMES
            clang-format-10
            clang-format
    )

    if(${clang_format} STREQUAL "CLANG_FORMAT-NOTFOUND")
        message(FATAL_ERROR "No clang-format tool found")
    endif()

    message(STATUS "Generating clang-format target using ${clang_format}")
    file(GLOB_RECURSE all_source_files *.cpp *.h *.hpp)
    add_custom_target(clang_format
        COMMAND ${clang_format} -i ${all_source_files}
        COMMENT "Formatting with ${clang_format} ..."
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

    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "Test")
endif()
