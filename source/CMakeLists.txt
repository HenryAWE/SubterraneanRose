# SubterraneanRose Source
find_package(SDL2 CONFIG REQUIRED)

if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

add_compile_definitions(SR_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR})
add_compile_definitions(SR_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR})
add_compile_definitions(SR_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH})

add_subdirectory(./core)
add_subdirectory(./wm)
add_subdirectory(./gpu)
add_subdirectory(./res)
add_subdirectory(./srose)
add_subdirectory(./player)
add_subdirectory(./editor)
add_subdirectory(./ui)

aux_source_directory(. src_list)

add_executable("srose-exec" ${src_list})

target_link_libraries("srose-exec" PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries("srose-exec" PRIVATE "srose-core")
target_link_libraries("srose-exec" PRIVATE "srose-wm")
target_link_libraries("srose-exec" PRIVATE "srose-player")
target_link_libraries("srose-exec" PRIVATE "srose-ui")

install(
    TARGETS "srose-exec"
    RUNTIME DESTINATION bin
)
include(CPack)
