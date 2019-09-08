# SubterraneanRose Source
find_package(SDL2 CONFIG REQUIRED)

add_compile_definitions(SR_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR})
add_compile_definitions(SR_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR})
add_compile_definitions(SR_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH})

add_subdirectory(./sys)
add_subdirectory(./wm)
add_subdirectory(./gpu)
add_subdirectory(./srose)
add_subdirectory(./editor)

aux_source_directory(. src_list)

add_executable(srose-e ${src_list})

target_link_libraries(srose-e PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(srose-e PRIVATE "srose-sys")
target_link_libraries(srose-e PRIVATE "srose-wm")
