# Tools for Subterranean Rose Project

find_package(PythonInterp 3 REQUIRED)

install(
    DIRECTORY ./srlc
    DESTINATION tools
    FILES_MATCHING PATTERN "*.py"
)
install(
    PROGRAMS ./lccl.py
    DESTINATION tools
)
