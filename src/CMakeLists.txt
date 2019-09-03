# SubterraneanRose Source

add_subdirectory(./wm)
add_subdirectory(./gpu)
add_subdirectory(./srose)
add_subdirectory(./editor)

aux_source_directory(. src_list)

add_executable(srose ${src_list})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(srose PRIVATE SDL2::SDL2 SDL2::SDL2main)
