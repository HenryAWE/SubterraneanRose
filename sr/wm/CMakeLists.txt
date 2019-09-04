# SubterraneanRose Window Manager Module
aux_source_directory(./src wm_src_list)
add_library("srose-wm" ${wm_src_list})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries("srose-wm" PUBLIC SDL2::SDL2 SDL2::SDL2main)
