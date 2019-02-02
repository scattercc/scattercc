cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

project(scattercc
    VERSION 0.0.0
    LANGUAGES C CXX)


# Include Predef.cmake for RCC_XXX predefinitions
include(cmake/Predef.cmake)


# Choose _WIN32_WINNT definition on Windows/Cygwin/MSYS
if (SCC_TARGET_WINDOWS OR SCC_TARGET_CYGWIN OR SCC_TARGET_MSYS)
    set(SCATTERCC_WIN32_WINNT "0x0601" CACHE STRING
        "Define _WIN32_WINNT on Windows/Cygwin/MSYS (default: Windows 7)")
endif()


# Add necessary CMake modules and scripts
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cmake/Workaround.cmake)
include(cmake/ImportedTarget.cmake)


# Treat warning as error (default: OFF)
# Just enable this when doing continuous integration
option(SCATTERCC_TREAT_WARNINGS_AS_ERRORS
    "Treat compiler warnings as errors, most for CI only (default: OFF)"
    OFF)
if (SCATTERCC_TREAT_WARNINGS_AS_ERRORS)
    message(STATUS "NOTE: Treat all warnings as errors!")

    # Also, use verbose Makefile
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()


# Create infra INTERFACE library
# This is needed by all executables and tests
include(infra/CMakeLists.txt)


# function:
#   scc_set_common_properties(Target)
# Add common properties to `Target`
function(scc_set_common_properties Target)
    # "Link" to `infra` INTERFACE library
    # This includes necessary headers and definitions
    target_link_libraries(${Target} PRIVATE infra)

    # Add definition __THIS_IS_${Target:Upper}__=1 to Target:
    string(TOUPPER ${Target} TARGET_UPPERCASE)
    target_compile_definitions(${Target} PRIVATE __THIS_IS_${TARGET_UPPERCASE}__=1)

    # Link with thread library
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    find_package(Threads REQUIRED)
    target_link_libraries(${Target} PRIVATE Threads::Threads)

    # Add some definitions on Windows/Cygwin/MSYS
    if (SCC_TARGET_WINDOWS OR SCC_TARGET_CYGWIN OR SCC_TARGET_MSYS)
        target_compile_definitions(${Target} PRIVATE _WIN32_WINNT=${SCATTERCC_WIN32_WINNT})
        target_compile_definitions(${Target} PRIVATE NOMINMAX)
    endif()

    # Enable C11, C++14
    set_property(TARGET ${Target} PROPERTY C_STANDARD 11)
    set_property(TARGET ${Target} PROPERTY CXX_STANDARD 14)

    # We don't set C_STANDARD_REQUIRED or CXX_STANDARD_REQUIRED to ON here, to avoid potential (unnecessarily) failure.
    # If a required language feature is not supported by compiler, compilation just fails.
    #set_property(TARGET ${Target} PROPERTY C_STANDARD_REQUIRED ON)
    #set_property(TARGET ${Target} PROPERTY CXX_STANDARD_REQUIRED ON)

    # Don't disable C/C++ compiler specific extensions
    # We use these features to workaround flaws in C++
    # e.g., how to deal with comma before __VA_ARGS__ in variadic macros
    set_property(TARGET ${Target} PROPERTY C_EXTENSIONS ON)
    set_property(TARGET ${Target} PROPERTY CXX_EXTENSIONS ON)

    # Enable all compiler warnings by default
    if (SCC_COMPILER_MSVC)
        # Then set /W4 for MSVC
        target_compile_options(${Target} PRIVATE "/W4")

        # Disable C4127: conditional expression is constant
        # https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4127
        target_compile_options(${Target} PRIVATE "/wd4127")
    elseif (SCC_COMPILER_GNU OR SCC_COMPILER_CLANG)
        target_compile_options(${Target} PRIVATE "-Wall")
        target_compile_options(${Target} PRIVATE "-Wextra")
    else()
        scc_should_not_reach_here("Unknown compiler?")
    endif()

    # Treat warnings as errors?
    if (SCATTERCC_TREAT_WARNINGS_AS_ERRORS)
        if (SCC_COMPILER_MSVC)
            # MSVC has "/WX" for both the compiler and the linker
            # (Compiler) /WX: Treat all compiler warnings as errors
            # (Linker) /WX: Treat linker warnings as errors
            # See:
            #   https://docs.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level
            #   https://docs.microsoft.com/en-us/cpp/build/reference/wx-treat-linker-warnings-as-errors
            target_compile_options(${Target} PRIVATE "/WX")
            scc_target_link_options(${Target} PRIVATE "/WX")
        elseif (SCC_COMPILER_GNU OR SCC_COMPILER_CLANG)
            target_compile_options(${Target} PRIVATE "-Werror")
        else()
            scc_should_not_reach_here("Unknown compiler?")
        endif()
    endif()

endfunction()


# Add src/
include(src/CMakeLists.txt)


# Do we build unit tests?
if ("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    # scattercc is built as top project: enable unit tests by default
    set(SCC_ENABLE_UNITTEST_DEFAULT ON)
else()
    # scattercc is built as "embedded" project: disable unit-test by default
    set(SCC_ENABLE_UNITTEST_DEFAULT OFF)
endif()
option(SCATTERCC_ENABLE_UNITTEST
    "Enable scattercc unit tests"
    ${SCC_ENABLE_UNITTEST_DEFAULT})

if (SCATTERCC_ENABLE_UNITTEST)
    message(STATUS "Unit tests are enabled")
    enable_testing()
    include(test/CMakeLists.txt)
else()
    message(STATUS "Unit tests are disabled")
endif ()
