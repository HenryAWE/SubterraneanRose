# SubterraneanRose Window Manager Module
find_package(imgui CONFIG REQUIRED)

aux_source_directory(./src wm_src_list)
add_library("srose-wm" STATIC ${wm_src_list})
target_include_directories("srose-wm" PUBLIC ./include)

target_link_libraries("srose-wm" PUBLIC "srose-core")
target_link_libraries("srose-wm" PUBLIC imgui::imgui)
