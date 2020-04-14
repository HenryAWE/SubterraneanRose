include_guard(DIRECTORY)

set(SROSE_TOOL_DIR ${CMAKE_SOURCE_DIR}/tools)

# Invoke python script for converting
function(file2cstr input output)
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} "${SROSE_TOOL_DIR}/file2cstr.py" "--input" ${input} "--output" ${output} "--cached"
    )
endfunction()
