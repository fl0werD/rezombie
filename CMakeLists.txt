#------------------------------------------------------------------------------------------------
#                      Copyright © Valve Corporation. All rights reserved.
#------------------------------------------------------------------------------------------------
# Compile with MSVC:
#   cmake --preset x86-windows-msvc-release && cmake --build --preset x86-windows-msvc-release
#
# Compile with GCC:
#   cmake --preset x86-linux-gcc-release && cmake --build --preset x86-linux-gcc-release
#------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.23)

#------------------------------------------------------------------------------------------------
# Project
#------------------------------------------------------------------------------------------------

set(INFO_PROJECT_NAME "Re Zombie")
set(INFO_PROJECT_DESCRIPTION "AMXX Module")
set(INFO_PROJECT_AUTHOR "fl0wer")
set(INFO_PROJECT_URL "https://github.com/fl0werD/rezombie")
set(INFO_VERSION_MAJOR "1")
set(INFO_VERSION_MINOR "0")
set(INFO_VERSION_PATCH "0")
set(BINARY_NAME_SUFFIX_WINDOWS "_amxx.dll")
set(BINARY_NAME_SUFFIX_LINUX "_amxx_i386.so")
set(BINARY_NAME_SUFFIX_DARWIN "_amxx.dylib")

string(TOLOWER "${INFO_PROJECT_NAME}" PROJECT_NAME)
string(REGEX REPLACE "[ \t\r\n]" "" PROJECT_NAME "${PROJECT_NAME}")

project(${PROJECT_NAME}
        VERSION ${INFO_VERSION_MAJOR}.${INFO_VERSION_MINOR}.${INFO_VERSION_PATCH}
        DESCRIPTION "${INFO_PROJECT_DESCRIPTION}"
        HOMEPAGE_URL "${INFO_PROJECT_URL}"
        )

#------------------------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------------------------
option(ENABLE_RTTI "Enable RTTI support" ON)
option(ENABLE_EXCEPTIONS "Enable exception handling table generation" OFF)
option(USE_LINKER_GOLD "Use the Gold linker (with GCC)" ON)
option(LINK_STATIC_GCC "Static linking with the libgcc library" ON)
option(LINK_STATIC_STDCPP "Static linking with the libstdc++ library" ON)
#------------------------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(Dependencies)
include(Config)
include(${CMAKE_HOST_SYSTEM_NAME})

add_subdirectory("libs/common")
add_subdirectory("libs/vhooks")
add_subdirectory("libs/messages")
add_subdirectory("rezombie")
