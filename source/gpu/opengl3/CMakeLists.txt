# SubterraneanRose Modern OpenGL Module

aux_source_directory(./src opengl_src_list)

add_library("srose-gpu-opengl3" STATIC ${opengl_src_list})
target_include_directories("srose-gpu-opengl3" PUBLIC ./include)
target_include_directories("srose-gpu-opengl3" PUBLIC ./glm)

target_link_libraries("srose-gpu-opengl3" PUBLIC "srose-core")
target_link_libraries("srose-gpu-opengl3" PUBLIC "srose-util")
target_link_libraries("srose-gpu-opengl3" PUBLIC "srose-wm")
