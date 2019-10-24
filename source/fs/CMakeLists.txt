# Subterranean Rose filesystem module

aux_source_directory(./src fs_src_list)
add_library("srose-fs" STATIC ${fs_src_list})
target_include_directories("srose-fs" PUBLIC ./include)

# GCC toolset cannot link to the C++17 filesystem library automatically
if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries("srose-fs" INTERFACE stdc++fs)
endif()
