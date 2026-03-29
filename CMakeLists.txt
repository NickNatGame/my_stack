cmake_minimum_required(VERSION 3.16.3)
project(stack C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
endif()

option(ENABLE_TESTS "Enable tests" OFF)
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(test)

    add_custom_target(run-tests
        COMMAND $<TARGET_FILE:tests>
        DEPENDS tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

add_subdirectory(src)