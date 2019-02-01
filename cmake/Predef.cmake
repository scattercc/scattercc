# CMP0054
#   Only interpret if() arguments as variables or keywords when unquoted.
# See https://cmake.org/cmake/help/v3.5/policy/CMP0054.html
cmake_policy(SET CMP0054 NEW)

#
# scc_should_not_reach_here(ErrorMessage)
#   Add linker options to Target
#
macro(scc_should_not_reach_here ErrorMessage)
    message(FATAL_ERROR "BUG: Should not reach here! ${ErrorMessage}")
endmacro()


#
# Detect CMake version
#
macro(scc_set_cmake_version_at_least Major Minor Patch)
    if ("${CMAKE_VERSION}" VERSION_GREATER "${Major}.${Minor}.${Patch}" OR
        "${CMAKE_VERSION}" VERSION_EQUAL   "${Major}.${Minor}.${Patch}")
        message(STATUS "CMake version detection: set SCC_CMAKE_VERSION_AT_LEAST_${Major}_${Minor}_${Patch} to ON")
        set(SCC_CMAKE_VERSION_AT_LEAST_${Major}_${Minor}_${Patch} ON)
    else()
        message(STATUS "CMake version detection: set SCC_CMAKE_VERSION_AT_LEAST_${Major}_${Minor}_${Patch} to nothing")
        set(SCC_CMAKE_VERSION_AT_LEAST_${Major}_${Minor}_${Patch})
    endif()
endmacro()

message(STATUS "CMake version: ${CMAKE_VERSION}")
scc_set_cmake_version_at_least(3 6 0)
scc_set_cmake_version_at_least(3 7 0)
scc_set_cmake_version_at_least(3 8 0)
scc_set_cmake_version_at_least(3 9 0)
scc_set_cmake_version_at_least(3 10 0)
scc_set_cmake_version_at_least(3 11 0)
scc_set_cmake_version_at_least(3 12 0)
scc_set_cmake_version_at_least(3 13 0)


#
# Detect target system
#
message(STATUS "Target system: ${CMAKE_SYSTEM_NAME}")
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    message(STATUS "Set SCC_TARGET_WINDOWS to ON")
    set(SCC_TARGET_WINDOWS ON)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "CYGWIN")
    message(STATUS "Set SCC_TARGET_CYGWIN to ON")
    set(SCC_TARGET_CYGWIN ON)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "MSYS")
    message(STATUS "Set SCC_TARGET_MSYS to ON")
    set(SCC_TARGET_MSYS ON)
else()
    message(FATAL_ERROR "Unknown target system: ${CMAKE_SYSTEM_NAME}")
endif ()


#
# Detect copmiler
#
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    message(STATUS "Set SCC_COMPILER_MSVC to ON")
    set(SCC_COMPILER_MSVC ON)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(STATUS "Set SCC_COMPILER_GNU to ON")
    set(SCC_COMPILER_GNU ON)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Set SCC_COMPILER_CLANG to ON")
    set(SCC_COMPILER_CLANG ON)
else()
    message(FATAL_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
endif ()


#
# MSVC patch: remove defaulted "/W3" flag
#
if (SCC_COMPILER_MSVC)
    # Remove the default warning level (if any)
    #   See: https://stackoverflow.com/questions/45995784/how-to-set-compiler-options-with-cmake-in-visual-studio-2017
    # This is hacky!
    # NOTE: CMAKE_<LANG>_FLAGS_INIT is not documented in earlier versions of CMake (CMake < 3.13.x), but works
    # NOTE: regex \s (whitespace chars) are NOT recognized by CMake...
    string(REGEX REPLACE "( |\t)+/W[0-4]( |\t)+" " " CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT}")
    string(REGEX REPLACE "( |\t)+/W[0-4]( |\t)+" " " CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT}")
endif()
