add_library(stack_lib
    stack.c
)

target_include_directories(stack_lib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(stack_lib
    PUBLIC
        stack_project_options
)

string(TOUPPER "${LOG_LEVEL}" STACK_LOG_LEVEL)

if(STACK_LOG_LEVEL STREQUAL "FULL")
    target_compile_definitions(stack_lib
        PRIVATE
            LOG_LEVEL_FULL
    )
elseif(STACK_LOG_LEVEL STREQUAL "MEDIUM")
    target_compile_definitions(stack_lib
        PRIVATE
            LOG_LEVEL_MEDIUM
    )
elseif(STACK_LOG_LEVEL STREQUAL "NONE")
    target_compile_definitions(stack_lib
        PRIVATE
            LOG_LEVEL_NONE
    )
else()
    message(FATAL_ERROR
        "Unsupported LOG_LEVEL='${LOG_LEVEL}'. Use FULL, MEDIUM, or NONE."
    )
endif()

target_compile_definitions(stack_lib
    PRIVATE
        "LOG_FILE_NAME=\"${LOG_FILE_NAME}\""
)

add_executable(stack
    main.c
)

target_link_libraries(stack
    PRIVATE
        stack_lib
)
