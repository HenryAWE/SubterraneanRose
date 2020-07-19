include_guard(DIRECTORY)

# Find the git executable
find_package(Git QUIET)


# Get log output of git with specific format
function(sr_git_log output format)
    if(${GIT_FOUND} AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} log --pretty=format:"${format}" -1
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE git_output
        )
        # Return value
        set(${output} ${git_output} PARENT_SCOPE)

    else()
        set(${output} "\"\"" PARENT_SCOPE)
    endif()
endfunction()

# Get output of git
function(sr_git_output output)
    if(${GIT_FOUND} AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} ${ARGN}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE git_output
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        # Return value
        set(${output} ${git_output} PARENT_SCOPE)

    else()
        set(${output} "\"\"" PARENT_SCOPE)
    endif()
endfunction()
