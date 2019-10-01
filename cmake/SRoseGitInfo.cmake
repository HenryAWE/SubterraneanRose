include_guard(DIRECTORY)

# Find the git executable
find_program(git_found "git" DOC "The git executable path")
mark_as_advanced(git_found)

function(sr_gitinfo_avail output)
    if(NOT EXISTS ${git_found})
        set(${output} FALSE PARENT_SCOPE)
        return()
    endif()
    if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
        set(${output} TRUE PARENT_SCOPE)
    else()
        set(${output} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Get abbreviated commit id
function(sr_abbr_commit_id output)
    execute_process(
        COMMAND ${git_found} log --pretty=format:"%h" -1
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE commit_id_str
    )

    # Remove the quotes around the string
    string(REPLACE "\"" "" commit_id ${commit_id_str})

    # Return value
    set(${output} ${commit_id} PARENT_SCOPE)
endfunction()
