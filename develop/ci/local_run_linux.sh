#!/bin/bash

set -v
set -e

#
# Arguments:
#   --all           run against all CMake verions (default: only minimum supported version)
#


#
# Find CI script
#
cd "$(dirname "$0")/../.."

SCRIPT="$(pwd)/ci/linux/build.sh"
if [ ! -f "$SCRIPT" ]; then
    echo "ERROR: not found or not a file: $SCRIPT"
    exit 1
fi


#
# Parse options
#
OPT_ALL=""

while [ "$#" -gt 0 ]; do
    arg="$1"; shift
    if [ "$arg" == "--all" ]; then
        OPT_ALL=yes
    else
        echo "ERROR: Unknown argument: $arg"
        exit 1
    fi
done


#
# Decide CMake versions
#
if [ "$OPT_ALL" = yes ]; then
    CMAKE_VERSIONS=(
        3.5.0
        3.6.0
        3.7.0
        3.8.0
        3.9.0
        3.10.0
        3.11.0
        3.12.0
        3.13.0 3.13.3
        )
else
    CMAKE_VERSIONS=( 3.5.0 )
fi


GCC_VERSIONS=(
    5.3 5.4 5.5
    6.1 6.2 6.3 6.4 6.5
    7.1 7.2 7.3 7.4
    8.1 8.2
    )

CLANG_VERSIONS=(
    3.5 3.6 3.7 3.8 3.9
    4.0
    5.0
    6.0
    7
    8
    )


for cmake_version in "${CMAKE_VERSIONS[@]}"; do
    for gcc_version in "${GCC_VERSIONS[@]}"; do
        echo -e "\n\n"
        echo "=================================================================="
        echo "================ CMake $cmake_version Compiler: gcc $gcc_version"
        echo "=================================================================="

        CI_CMAKE_VERSION="$cmake_version" \
        CI_COMPILER_TYPE=gcc \
        CI_COMPILER_VERSION="$gcc_version" \
        CI_BUILD_TYPE=Release \
        "$SCRIPT" --no-pull
    done
    for clang_version in "${CLANG_VERSIONS[@]}"; do
        echo -e "\n\n"
        echo "=================================================================="
        echo "================ CMake $cmake_version Compiler: clang $clang_version"
        echo "=================================================================="

        CI_CMAKE_VERSION="$cmake_version" \
        CI_COMPILER_TYPE=clang \
        CI_COMPILER_VERSION="$clang_version" \
        CI_BUILD_TYPE=Release \
        "$SCRIPT" --no-pull
    done
done
