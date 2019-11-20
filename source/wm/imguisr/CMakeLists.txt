# Subterranean Rose ImGui extensions

aux_source_directory(./src imguisr_src_list)
add_library("srose-imguisr" STATIC ${imguisr_src_list})

target_include_directories("srose-imguisr" PUBLIC ./include)
target_link_libraries("srose-imguisr" PUBLIC "srose-ext-imgui")
