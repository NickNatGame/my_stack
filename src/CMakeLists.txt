add_library(stack_lib
    stack.c
)

target_include_directories(stack_lib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(stack_lib
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        $<$<CONFIG:Debug>:-O0 -g -fsanitize=address -fsanitize=undefined>
        $<$<CONFIG:Release>:-O2>
)

target_compile_definitions(stack_lib
    PRIVATE
        LOG_LEVEL_FULL
        "LOG_FILE_NAME=\"stack.txt\""
)

add_executable(stack
    main.c
)

target_link_libraries(stack
    PRIVATE
        stack_lib
)
