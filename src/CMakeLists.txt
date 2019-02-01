# Add executable: scattercc
set(SRC_COMMON_FILES)

add_executable(scattercc
    ${CMAKE_CURRENT_LIST_DIR}/scattercc.cpp
    ${SRC_COMMON_FILES})
scc_set_common_properties(scattercc)
target_include_directories(scattercc PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Add executable: scatterccd
add_executable(scatterccd
    ${CMAKE_CURRENT_LIST_DIR}/scatterccd.cpp
    ${SRC_COMMON_FILES})
scc_set_common_properties(scatterccd)
target_include_directories(scatterccd PRIVATE ${CMAKE_CURRENT_LIST_DIR})
