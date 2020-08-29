include_guard(DIRECTORY)

set(SROSE_TOOL_DIR ${CMAKE_SOURCE_DIR}/tools)

# Invoke python script for compiling locale script to binary
function(compile_lc output config)
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} "${SROSE_TOOL_DIR}/lccl.py" "--action=compile" "--incremental" "-o" ${output} "-c" ${config} "-i" ${ARGN}
    )
endfunction()
