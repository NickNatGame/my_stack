cmake_minimum_required(VERSION 3.16.3)
project(stack C)

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
