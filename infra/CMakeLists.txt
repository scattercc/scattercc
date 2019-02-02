# Create an INTERFACE library "infra", other executables "links" to it
add_library(infra INTERFACE)

# Add current directory (which contains "infra.h") to #include directory
target_include_directories(infra BEFORE INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Source files for infra
target_sources(infra INTERFACE
    # Windows
    ${CMAKE_CURRENT_LIST_DIR}/mswin/target.h

    # Posix-based common
    ${CMAKE_CURRENT_LIST_DIR}/posix_base/target.h

    # Linux
    ${CMAKE_CURRENT_LIST_DIR}/linux/target.h

    # Infra
    ${CMAKE_CURRENT_LIST_DIR}/infra/ide.h
    ${CMAKE_CURRENT_LIST_DIR}/infra/maybe_unused.h

    # Exposed header & source
    ${CMAKE_CURRENT_LIST_DIR}/infra.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra.h)
