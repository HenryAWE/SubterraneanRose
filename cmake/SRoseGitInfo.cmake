include_guard(DIRECTORY)

# Find the git executable
find_package(Git QUIET)


# Get abbreviated commit id
function(sr_git_log output format)
    if(${GIT_FOUND} AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
        set(${output} TRUE PARENT_SCOPE)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log --pretty=format:"${format}" -1
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE commit_id
    )
    # Return value
    set(${output} ${commit_id} PARENT_SCOPE)

    else()
        set(${output} "\"\"" PARENT_SCOPE)
    endif()
endfunction()
