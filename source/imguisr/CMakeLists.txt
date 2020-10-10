# ImGui extensions and wrappers of the Subterranean Rose Project

aux_source_directory(./src imguisr_src_list)
aux_source_directory(./src/filebrowser imguisr_filebrowser_src_list)
add_library("srose-imgui" STATIC ${imguisr_src_list} ${imguisr_filebrowser_src_list})
target_include_directories("srose-imgui" PUBLIC ./include)

target_link_libraries("srose-imgui" PUBLIC "srose-core")
target_link_libraries("srose-imgui" PUBLIC "srose-graphic")
target_link_libraries("srose-imgui" PUBLIC glm::glm)
target_link_libraries("srose-imgui" PUBLIC imgui::imgui)
