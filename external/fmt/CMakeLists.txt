# Third party library {fmt}
# https://github.com/fmtlib/fmt

add_library(fmt ./src/format.cc)
target_include_directories(fmt PUBLIC ./include)
