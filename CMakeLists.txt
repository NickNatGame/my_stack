cmake_minimum_required(VERSION 3.16.3)

project(stack)

add_executable(${CMAKE_PROJECT_NAME} src/main.c src/stack.c)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_compile_definitions(std=c17 LOG_LEVEL_FULL Wall O1 Wextra Wpedantic Wnull-dereference)