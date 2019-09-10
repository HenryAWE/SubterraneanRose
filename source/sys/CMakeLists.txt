# SubterraneanRose Core Module
find_package(SDL2 CONFIG REQUIRED)
# TODO: Restore in future
# find_package(sdl2-mixer CONFIG REQUIRED)

aux_source_directory(./src sys_src_list)
add_library("srose-sys" STATIC ${sys_src_list})
target_include_directories("srose-sys" PUBLIC ./include)

target_link_libraries("srose-sys" PUBLIC SDL2::SDL2)
# target_link_libraries("srose-sys" PUBLIC SDL2::SDL2_mixer)
