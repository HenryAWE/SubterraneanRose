# SubterraneanRose Window Manager Module

aux_source_directory(./src wm_src_list)
add_library("srose-wm" STATIC ${wm_src_list})
target_include_directories("srose-wm" PUBLIC ./include)

target_link_libraries("srose-wm" PUBLIC "srose-core")
target_link_libraries("srose-wm" PUBLIC "srose-graphic")
target_link_libraries("srose-wm" PUBLIC glm::glm)
target_link_libraries("srose-wm" PUBLIC imgui::imgui)
