# SubterraneanRose Core Module
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)

aux_source_directory(./src core_src_list)
# Generate GIT information in C source
include(SRoseGitInfo)
sr_gitinfo_avail(git_avail)
message(STATUS "Git: ${git_avail} ${git_found}")
if(${git_avail})
    sr_abbr_commit_id(commit_id_abbr)
else()
    set(commit_id_abbr "(Git is unavailable)")
endif()
message(STATUS "Git commit id (abbr): ${commit_id_abbr}")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/git_info.c.in ${CMAKE_CURRENT_BINARY_DIR}/git_info.c
    @ONLY
    NEWLINE_STYLE LF
)


add_library("srose-core" STATIC ${core_src_list} ${CMAKE_CURRENT_BINARY_DIR}/git_info.c ./glad/glad.c)
target_include_directories("srose-core" PUBLIC ./include)
target_include_directories("srose-core" PUBLIC ./glad)

target_link_libraries("srose-core" PUBLIC SDL2::SDL2)
target_link_libraries("srose-core" PUBLIC SDL2::SDL2_mixer)
target_link_libraries("srose-core" PUBLIC SDL2::SDL2_image)
