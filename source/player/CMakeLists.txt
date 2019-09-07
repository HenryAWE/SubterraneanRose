# SubterraneanRose Player Module
find_package(SDL2 CONFIG REQUIRED)

aux_source_directory(./src player_src_list)
add_library("srose-player" STATIC ${player_src_list})
target_include_directories("srose-player" PUBLIC ./include)

target_link_libraries("srose-player" PUBLIC "srose-sys")
target_link_libraries("srose-player" PUBLIC SDL2::SDL2)
target_link_libraries("srose-player" PUBLIC "srose-wm")
