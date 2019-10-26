# SubterraneanRose Utilities

# Header-only library
add_library("srose-util-headers" INTERFACE)
target_include_directories("srose-util-headers" INTERFACE ./headers)

# Utilities
aux_source_directory(./src util_src_list)
add_library("srose-util" STATIC ${util_src_list})
target_include_directories("srose-util" PUBLIC ./include)
target_link_libraries("srose-util" PUBLIC "srose-ext-stb")
target_link_libraries("srose-util" PUBLIC "srose-util-headers")
target_link_libraries("srose-util" PUBLIC "srose-core")

# Tests
add_subdirectory(./test)
