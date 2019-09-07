# SubterraneanRose Source
find_package(SDL2 CONFIG REQUIRED)

add_subdirectory(./sys)
add_subdirectory(./wm)
add_subdirectory(./gpu)
add_subdirectory(./srose)
add_subdirectory(./player)
add_subdirectory(./editor)

aux_source_directory(. src_list)

add_executable(srose-e ${src_list})

target_link_libraries(srose-e PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(srose-e PRIVATE "srose-sys")
target_link_libraries(srose-e PRIVATE "srose-wm")
target_link_libraries(srose-e PRIVATE "srose-player")
