include_guard(DIRECTORY)

set(SROSE_SCRIPTS_DIR ${CMAKE_SOURCE_DIR}/scripts)

# Invoke python script for converting
function(file2cstr input output)
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} "${SROSE_SCRIPTS_DIR}/file2cstr.py" "--input" ${input} "--output" ${output} "--cached"
    )
endfunction()
