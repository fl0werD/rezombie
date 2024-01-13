cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME "json")
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE "include")

target_sources(${PROJECT_NAME} INTERFACE "include/nlohmann/json.hpp")
