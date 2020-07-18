# Header-only part of the Subterranean Rose

add_library("srose-headers" INTERFACE)
target_include_directories("srose-headers" INTERFACE ./include)

# Tests
if(NOT ${SROSE_DISABLE_TEST})
    add_subdirectory(./test/string_tree_test)
    add_subdirectory(./test/tracker_test)
    add_subdirectory(./test/funcqueue_test)
    add_subdirectory(./test/semver_test)
endif()
