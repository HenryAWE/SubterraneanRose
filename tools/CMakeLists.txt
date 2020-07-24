# Tools for Subterranean Rose Project

find_package(PythonInterp 3 REQUIRED)

install(
    FILES ./srlc/__init__.py ./srlc/compiler.py
    DESTINATION tools/srlc
    COMPONENT Tools
)
install(
    PROGRAMS ./lccl.py
    DESTINATION tools
    COMPONENT Tools
)
