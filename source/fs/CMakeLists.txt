# Subterranean Rose filesystem module

add_library("srose-fs" INTERFACE)

# GCC toolset cannot link to the C++17 filesystem library automatically
if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries("srose-fs" INTERFACE stdc++fs)
endif()
