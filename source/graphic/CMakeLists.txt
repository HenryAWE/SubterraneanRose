# SubterraneanRose GPU Module

aux_source_directory(./src gpu_src_list)
add_library("srose-graphic" STATIC ${gpu_src_list})
target_include_directories("srose-graphic" PUBLIC ./include)
target_link_libraries("srose-graphic" PUBLIC glm::glm)
target_link_libraries("srose-graphic" PUBLIC "srose-opengl3")
target_link_libraries("srose-graphic" PUBLIC "srose-player")
target_link_libraries("srose-graphic" PUBLIC "srose-fs")
