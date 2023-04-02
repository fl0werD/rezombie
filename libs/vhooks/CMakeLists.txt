cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME "vhooks")
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE "include")

#FetchContent_MakeAvailable(Metamod)
target_link_libraries(${PROJECT_NAME} INTERFACE metamod)

target_sources(${PROJECT_NAME} INTERFACE
        "include/vhooks/vhooks.h"
        "include/vhooks/memory_unlocker.h"
        "include/vhooks/entity_virtual_table.h"
        "include/vhooks/hook_index.h"
        "src/vhooks.cpp"
        )
