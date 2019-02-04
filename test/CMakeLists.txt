macro(add_test_executable TestName #[[SourceFiles]])

    # Add each file in ARGN to source files
    # First, check file existence relative to current CMakeLists.txt directory
    # Next, check file existence relative to remotecc source directory
    # Finally, report error
    set(source_files)
    foreach (source_file ${ARGN})
        if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/${source_file}")
            list(APPEND source_files "${CMAKE_CURRENT_LIST_DIR}/${source_file}")
        elseif (EXISTS "${PROJECT_SOURCE_DIR}/${source_file}")
            list(APPEND source_files "${PROJECT_SOURCE_DIR}/${source_file}")
        else()
            message(FATAL_ERROR "add_test_executable: Unknown source file: ${source_file}")
        endif()
    endforeach()

    # Add this executable
    add_executable("${TestName}"
        ${INFRA_SOURCE_FILES}
        ${source_files})
    target_compile_definitions("${TestName}" PRIVATE THIS_IS_UNITTEST=1)  # define THIS_IS_UNITTEST 1
    scc_set_common_properties("${TestName}")

    # Include Catch for unit testing
    target_include_directories(${TestName} PRIVATE "${PROJECT_SOURCE_DIR}/thirdparty/catch")

    add_test(
        NAME "${TestName}"
        COMMAND "${TestName}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

    message(STATUS "Add test: ${TestName} (Executable)")
endmacro()

add_test_executable(test_infra
    infra/main.cpp
    infra/test_types.cpp
    infra/test_string.cpp
    infra/test_integer_to_string.cpp)
