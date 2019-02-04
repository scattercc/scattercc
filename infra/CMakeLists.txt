# Create an INTERFACE library "infra", other executables "links" to it
add_library(infra INTERFACE)

# Add current directory (which contains "infra.h") to #include directory
target_include_directories(infra BEFORE INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# TODO: Split to different variables

# Source files for infra
target_sources(infra INTERFACE
    # Windows
    ${CMAKE_CURRENT_LIST_DIR}/mswin/target.h

    # Posix-based common
    ${CMAKE_CURRENT_LIST_DIR}/posix_base/target.h

    # Linux
    ${CMAKE_CURRENT_LIST_DIR}/linux/target.h

    # predef
    ${CMAKE_CURRENT_LIST_DIR}/predef/ide.h
    ${CMAKE_CURRENT_LIST_DIR}/predef/maybe_unused.h
    ${CMAKE_CURRENT_LIST_DIR}/predef/static_assert.h
    ${CMAKE_CURRENT_LIST_DIR}/predef/types.h

    # basic
    ${CMAKE_CURRENT_LIST_DIR}/basic/basic_math.h
    ${CMAKE_CURRENT_LIST_DIR}/basic/integer_to_string.h
    ${CMAKE_CURRENT_LIST_DIR}/basic/string.h

    # warnings disable & restore
    ${CMAKE_CURRENT_LIST_DIR}/warnings/conditional_expression_is_constant/disable.h
    ${CMAKE_CURRENT_LIST_DIR}/warnings/conditional_expression_is_constant/restore.h
    ${CMAKE_CURRENT_LIST_DIR}/warnings/integral_overflow/disable.h
    ${CMAKE_CURRENT_LIST_DIR}/warnings/integral_overflow/restore.h

    # Exposed header & source
    ${CMAKE_CURRENT_LIST_DIR}/infra.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra.h)
