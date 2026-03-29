add_executable(tests
    test.c
)

target_include_directories(tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(tests
    PRIVATE
        stack_lib
)

add_test(NAME stack_tests COMMAND tests)