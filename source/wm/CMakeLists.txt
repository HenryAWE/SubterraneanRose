# SubterraneanRose Window Manager Module
aux_source_directory(./src wm_src_list)
add_library("srose-wm" STATIC ${wm_src_list})
target_include_directories("srose-wm" PUBLIC ./include)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries("srose-wm" PUBLIC "srose-sys")
target_link_libraries("srose-wm" PUBLIC SDL2::SDL2 SDL2::SDL2main)
