# CMP0054
#   Only interpret if() arguments as variables or keywords when unquoted.
# See https://cmake.org/cmake/help/v3.5/policy/CMP0054.html
cmake_policy(SET CMP0054 NEW)

#
# scc_should_not_reach_here(ErrorMessage)
#   Add linker options to Target
#
function(scc_should_not_reach_here ErrorMessage)
    message(FATAL_ERROR "BUG: Should not reach here! ${ErrorMessage}")
endfunction()


#
# scc_option_bool(Option Description DefaultValue)
# scc_option_string(Option Description DefaultValue)
#   For scattercc, they will define desired option, and append it to SCC_INHERIT_OPTIONS
#   For scattercc_try_compile, they will ensure these options are passed to scattercc_try_compile correctly.
#
set(SCC_INHERIT_OPTIONS)

macro(scc_option_bool Option Description DefaultValue)
    if (SCC_THIS_IS_TRY_COMPILE)
        if (NOT DEFINED ${Option})
            scc_should_not_reach_here("${Option} should be defined")
        endif()
    else()
        option(${Option} ${Description} ${DefaultValue})
        list(APPEND SCC_INHERIT_OPTIONS "-D${Option}:BOOL=${${Option}}")
    endif()
endmacro()

macro(scc_option_string Option Description DefaultValue)
    if (SCC_THIS_IS_TRY_COMPILE)
        if (NOT DEFINED ${Option})
            scc_should_not_reach_here("${Option} should be defined")
        endif()
    else()
        set(${Option} ${DefaultValue} CACHE STRING ${Description})
        list(APPEND SCC_INHERIT_OPTIONS "-D${Option}:STRING=${${Option}}")
    endif()
endmacro()

macro(scc_set_bool Var Value)
    if (SCC_THIS_IS_TRY_COMPILE)
        if (NOT DEFINED ${Var})
            scc_should_not_reach_here("${Var} should be defined")
        endif()
    else()
        set(${Var} ${Value})
        list(APPEND SCC_INHERIT_OPTIONS "-D${Var}:BOOL=${${Var}}")
    endif()
endmacro()

macro(scc_set_string Var Value)
    if (SCC_THIS_IS_TRY_COMPILE)
        if (NOT DEFINED ${Var})
            scc_should_not_reach_here("${Var} should be defined")
        endif()
    else()
        set(${Var} ${Value})
        list(APPEND SCC_INHERIT_OPTIONS "-D${Var}:STRING=${${Var}}")
    endif()
endmacro()



# Include Predef.cmake for SCC_XXX predefinitions
include(${CMAKE_CURRENT_LIST_DIR}/common/Predef.cmake)


# Treat warnings as errors? (default: OFF)
# Just enable this when doing continuous integration
scc_option_bool(SCATTERCC_TREAT_WARNINGS_AS_ERRORS
    "Treat compiler warnings as errors, most for CI only (default: OFF)"
    OFF)

if (SCATTERCC_TREAT_WARNINGS_AS_ERRORS)
    message(STATUS "NOTE: Treat all warnings as errors!")

    # Also, use verbose Makefile
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()


# Choose _WIN32_WINNT definition on Windows/Cygwin/MSYS
if (SCC_TARGET_WINDOWS OR SCC_TARGET_CYGWIN OR SCC_TARGET_MSYS)
    scc_option_string(SCATTERCC_WIN32_WINNT
        "Define _WIN32_WINNT on Windows/Cygwin/MSYS (default: Windows 7)"
        "0x0601")
endif()


# Do we build unit tests?
# NOTE:
#   For scattercc, SCC_ENABLE_UNITTEST_DEFAULT will be evaluated to ON
#   For scattercc_try_compile, SCC_ENABLE_UNITTEST_DEFAULT will be passed to it
if ("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    # scattercc is built as top project: enable unit tests by default
    scc_set_bool(SCC_ENABLE_UNITTEST_DEFAULT ON)
else()
    # scattercc is built as "embedded" project: disable unit tests by default
    scc_set_bool(SCC_ENABLE_UNITTEST_DEFAULT OFF)
endif()
scc_option_bool(SCATTERCC_ENABLE_UNITTEST
    "Enable scattercc unit tests"
    ${SCC_ENABLE_UNITTEST_DEFAULT})


# Add necessary CMake modules and scripts
include(${CMAKE_CURRENT_LIST_DIR}/common/Workaround.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/common/ImportedTarget.cmake)


# function:
#   scc_init_target_properties(Target)
# Add initial properties to `Target`
function(scc_init_target_properties Target)
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
        # Set /W4 for MSVC
        target_compile_options(${Target} PRIVATE "/W4")
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
