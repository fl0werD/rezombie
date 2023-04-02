cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME "messages")
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE "include")

#FetchContent_MakeAvailable(Metamod)
target_link_libraries(${PROJECT_NAME} INTERFACE metamod)

target_sources(${PROJECT_NAME} INTERFACE
        "include/messages/engine_message.h"
        "include/messages/user_message.h"
        "include/messages/temporary_entity.h"
        "include/messages/hud_message_params.h"
        "src/engine_message.cpp"
        "src/user_message.cpp"
        )
