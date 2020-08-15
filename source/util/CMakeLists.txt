# SubterraneanRose Utilities

# Utilities
aux_source_directory(./src util_src_list)
add_library("srose-util" STATIC ${util_src_list})
target_include_directories("srose-util" PUBLIC ./include)
target_link_libraries("srose-util" PUBLIC "srose-core")
target_link_libraries("srose-util" PUBLIC "srose-fs")
target_link_libraries("srose-util" PRIVATE "srose-locale")

# Tests
if(NOT ${SROSE_DISABLE_TEST})
    add_subdirectory(./test/semver_test)
endif()
