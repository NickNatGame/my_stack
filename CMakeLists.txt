cmake_minimum_required(VERSION 3.16.3)
project(stack)

# FIXME:: Cmakes are usually splitted across project directories:
#         i.e. src/CMakeLists.txt, tests/CMakeLists.txt
#         this helps keep build system clean, easy to read and maintain

add_library(stack_lib
  OBJECT 
    # FIXME: Why do you need header in an object library (spoiler: you dont)
    include/stack.h
    src/stack.c
)

target_include_directories(stack_lib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(stack_lib
  PRIVATE
  #TODO:  since you're using a cmake here, make two build-types:
  #       one for production build and another for a local debug build
    -std=c17 -DLOG_LEVEL_FULL -Wall -O1 -Wextra -Wpedantic
)

add_executable(tests EXCLUDE_FROM_ALL test/test.c)

target_link_libraries(tests stack_lib)

add_executable(${CMAKE_PROJECT_NAME} src/main.c)

target_link_libraries(${CMAKE_PROJECT_NAME} stack_lib)

# NOTE: nice!
add_custom_target(run-tests
  COMMAND $<TARGET_FILE:tests>
  DEPENDS tests
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
