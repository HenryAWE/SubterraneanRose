# Header-only Subterranean Rose Template Library

add_library("srose-srtl" INTERFACE)
target_include_directories("srose-srtl" INTERFACE ./include)

# Tests
if(NOT ${SROSE_DISABLE_TEST})
    add_subdirectory(./test/string_tree_test)
endif()
