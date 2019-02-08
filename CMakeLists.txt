cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

project(scattercc
    VERSION 0.0.0
    LANGUAGES C CXX)


# try_compile features/functions test
include(cmake/Common.cmake)


# try_compile features/functions test
include(cmake/TryCompile.cmake)


# Create infra INTERFACE library
# This is needed by all executables and tests
include(infra/CMakeLists.txt)


# function:
#   scc_set_common_properties(Target)
# Add common properties to `Target`
function(scc_set_common_properties Target)
    scc_init_target_properties(${Target})

    # "Link" to `infra` INTERFACE library
    # This includes necessary headers and definitions
    target_link_libraries(${Target} PRIVATE infra)
endfunction()


# Add src/
include(src/CMakeLists.txt)


if (SCATTERCC_ENABLE_UNITTEST)
    message(STATUS "Unit tests are enabled")
    enable_testing()
    include(test/CMakeLists.txt)
else()
    message(STATUS "Unit tests are disabled")
endif ()
