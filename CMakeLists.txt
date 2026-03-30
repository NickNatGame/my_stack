cmake_minimum_required(VERSION 3.16.3)
project(stack)

<<<<<<< HEAD
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
=======
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(LOG_LEVEL "FULL" CACHE STRING "Stack log level: FULL, MEDIUM, or NONE")
set(LOG_FILE_NAME "${CMAKE_SOURCE_DIR}/log.txt" CACHE STRING "Log file path")

set_property(CACHE LOG_LEVEL PROPERTY STRINGS FULL MEDIUM NONE)

include(CTest)

add_library(stack_project_options INTERFACE)
target_compile_options(stack_project_options
    INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-fsanitize=address>
        $<$<CONFIG:Debug>:-fsanitize=undefined>
        $<$<CONFIG:Release>:-O2>
)

target_link_options(stack_project_options
    INTERFACE
        $<$<CONFIG:Debug>:-fsanitize=address>
        $<$<CONFIG:Debug>:-fsanitize=undefined>
)

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(test)

    add_custom_target(run-tests
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        USES_TERMINAL
    )
endif()
>>>>>>> e78e216 (some fixes of CMAKE)
