# SubterraneanRose Core Module

aux_source_directory(./src sys_src_list)
add_library("srose-sys" STATIC ${sys_src_list})

target_link_libraries(srose-sys PUBLIC SDL2::SDL2 SDL2::SDL2main)
