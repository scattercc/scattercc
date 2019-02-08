
macro(scc_try_compile Var Name)
    message(STATUS "Testing ${Name}...")
    try_compile(
        ${Var}
        "${CMAKE_CURRENT_BINARY_DIR}/try_compile"
        "${CMAKE_CURRENT_LIST_DIR}/try_compile"
        scattercc_try_compile
        ${Name}
        CMAKE_FLAGS
            ${SCC_INHERIT_OPTIONS}
    )

    if (${Var})
        message(STATUS "Testing ${Name}... [FOUND] (${Var}=1)")
        add_definitions(-D${Var}=1)
    else()
        message(STATUS "Testing ${Name}... [NOT_FOUND]")
    endif()
endmacro()


message(STATUS "Test features/functions/... inherit flags: ${SCC_INHERIT_OPTIONS}")

scc_try_compile(HAVE_LOCALTIME localtime)
scc_try_compile(HAVE_LOCALTIME_R localtime_r)
scc_try_compile(HAVE_LOCALTIME_S localtime_s)

scc_try_compile(HAVE_GMTIME gmtime)
scc_try_compile(HAVE_GMTIME_R gmtime_r)
scc_try_compile(HAVE_GMTIME_S gmtime_s)
