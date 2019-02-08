cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)
project(scattercc_try_compile
    LANGUAGES C CXX)

# Set SCC_THIS_IS_TRY_COMPILE to ON
# This indicates we are compiling project "scattercc_try_compile" now
# This must be done before include(Common.cmake)
set(SCC_THIS_IS_TRY_COMPILE ON)
include(../Common.cmake)


macro(add_try_compile_executable Target)
    add_executable(${Target} "${Target}.cpp")
    scc_init_target_properties(${Target})
endmacro()


add_try_compile_executable(localtime)
add_try_compile_executable(localtime_r)
add_try_compile_executable(localtime_s)

add_try_compile_executable(gmtime)
add_try_compile_executable(gmtime_r)
add_try_compile_executable(gmtime_s)
