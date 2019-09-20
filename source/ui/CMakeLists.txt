# SubterraneanRose User Interface Module

aux_source_directory(./src ui_src_list)
add_library("srose-ui" STATIC ${ui_src_list})
target_include_directories("srose-ui" PUBLIC ./include)

target_link_libraries("srose-ui" PUBLIC "srose-core")
target_link_libraries("srose-ui" PUBLIC "srose-wm")
