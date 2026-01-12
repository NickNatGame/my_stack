cmake_minimum_required(VERSION 3.16.3)
project(stack)

add_library(stack_lib
  OBJECT 
    include/stack.h
    src/stack.c
)

target_include_directories(stack_lib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(stack_lib
  PRIVATE 
    -std=c17 -DLOG_LEVEL_FULL -Wall -O1 -Wextra -Wpedantic
)

add_executable(tests EXCLUDE_FROM_ALL test/test.c)

target_link_libraries(tests stack_lib)

add_executable(${CMAKE_PROJECT_NAME} src/main.c)

target_link_libraries(${CMAKE_PROJECT_NAME} stack_lib)

add_custom_target(run-tests
  COMMAND $<TARGET_FILE:tests>
  DEPENDS tests
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)