# SubterraneanRose GPU Module

add_subdirectory(./opengl3)

aux_source_directory(./src gpu_src_list)
add_library("srose-gpu" STATIC ${gpu_src_list})
target_include_directories("srose-gpu" PUBLIC ./include)
target_link_libraries("srose-gpu" PUBLIC "srose-ext-glm")
target_link_libraries("srose-gpu" PUBLIC "srose-gpu-opengl3")
target_link_libraries("srose-gpu" PUBLIC "srose-player")
target_link_libraries("srose-gpu" PUBLIC "srose-fs")
