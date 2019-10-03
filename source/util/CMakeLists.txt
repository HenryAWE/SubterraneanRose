# SubterraneanRose Utilities

aux_source_directory(./src util_src_list)
add_library("srose-util" STATIC ${util_src_list} ./stb/compile_stb.c)
target_include_directories("srose-util" PUBLIC ./include)
target_include_directories("srose-util" PUBLIC ./stb)
target_link_libraries("srose-util" PUBLIC "srose-core")
