
macro(scc_try_compile Var Name)
    if (SCATTERCC_WIN32_WINNT)
        set(win32_winnt_flag "-D_WIN32_WINNT:STRING=${SCATTERCC_WIN32_WINNT}")
    else()
        set(win32_winnt_flag)
    endif()

    message(STATUS "Testing ${Name}...")
    try_compile(
        ${Var}
        "${CMAKE_CURRENT_BINARY_DIR}/try_compile/${Name}"
        SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/${Name}.cpp"
        CMAKE_FLAGS
            "-DCMAKE_CXX_STANDARD:STRING=14"
            ${win32_winnt_flag}
    )

    if (${Var})
        message(STATUS "Testing ${Name}... [FOUND] (${Var}=1)")
        add_definitions(-D${Var}=1)
    else()
        message(STATUS "Testing ${Name}... [NOT_FOUND]")
    endif()
endmacro()

scc_try_compile(HAVE_LOCALTIME localtime)
scc_try_compile(HAVE_LOCALTIME_R localtime_r)
scc_try_compile(HAVE_LOCALTIME_S localtime_s)

scc_try_compile(HAVE_GMTIME gmtime)
scc_try_compile(HAVE_GMTIME_R gmtime_r)
scc_try_compile(HAVE_GMTIME_S gmtime_s)
