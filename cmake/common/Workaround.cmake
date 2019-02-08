#
# scc_target_link_options(Target, Scope, options...)
#   Add linker options to Target
# target_link_options() is introduced by CMake 3.13.0, after that we may use target_link_options for clear intent.
# Before that, target_link_libraries is used.
#
function(scc_target_link_options Target Scope #[[options...]])
    foreach(opt ${ARGN})
        if (SCC_CMAKE_VERSION_AT_LEAST_3_13_0)
            target_link_options(${Target} ${Scope} ${opt})
        else()  # CMake < 3.13.0
            target_link_libraries(${Target} ${Scope} ${opt})
        endif()
    endforeach()
endfunction()
