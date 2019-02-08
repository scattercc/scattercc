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
    add_definitions(-DSCC_TARGET_WINDOWS=1)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "CYGWIN")
    message(STATUS "Set SCC_TARGET_CYGWIN to ON")
    set(SCC_TARGET_CYGWIN ON)
    add_definitions(-DSCC_TARGET_CYGWIN=1)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "MSYS")
    message(STATUS "Set SCC_TARGET_MSYS to ON")
    set(SCC_TARGET_MSYS ON)
    add_definitions(-DSCC_TARGET_MSYS=1)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    message(STATUS "Set SCC_TARGET_LINUX to ON")
    set(SCC_TARGET_LINUX ON)
    add_definitions(-DSCC_TARGET_LINUX=1)
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
    add_definitions(-DSCC_COMPILER_MSVC=1)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(STATUS "Set SCC_COMPILER_GNU to ON")
    set(SCC_COMPILER_GNU ON)
    add_definitions(-DSCC_COMPILER_GNU=1)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Set SCC_COMPILER_CLANG to ON")
    set(SCC_COMPILER_CLANG ON)
    add_definitions(-DSCC_COMPILER_CLANG=1)
else()
    message(FATAL_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
endif ()


#
# Show compiler features
#
message(STATUS "C compiler features: ${CMAKE_C_COMPILE_FEATURES}")
message(STATUS "C++ compiler features: ${CMAKE_CXX_COMPILE_FEATURES}")


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
