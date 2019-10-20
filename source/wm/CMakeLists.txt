# SubterraneanRose Window Manager Module

aux_source_directory(./src wm_src_list)
aux_source_directory(./imgui imgui_src_list)
add_library("srose-wm" STATIC ${imgui_src_list} ${wm_src_list})
target_include_directories("srose-wm" PUBLIC ./include)
target_include_directories("srose-wm" PUBLIC ./imgui)
target_compile_definitions("srose-wm" PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD=1)

target_link_libraries("srose-wm" PUBLIC "srose-core")
