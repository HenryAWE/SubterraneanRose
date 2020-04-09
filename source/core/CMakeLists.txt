# SubterraneanRose Core Module
find_package(SDL2 CONFIG REQUIRED)
find_path(
    SDL2_MIXER_INCLUDE_DIR SDL_mixer.h
    PATH_SUFFIXES SDL2
)
find_library(
    SDL2_MIXER_LIBRARY SDL2_mixer
    PATH_SUFFIXES lib
)
mark_as_advanced(SDL2_MIXER_INCLUDE_DIR SDL2_MIXER_LIBRARY)
add_library("srose-ext-sdl2_mixer" INTERFACE)
target_include_directories("srose-ext-sdl2_mixer" INTERFACE ${SDL2_MIXER_INCLUDE_DIR})
target_link_libraries("srose-ext-sdl2_mixer" INTERFACE ${SDL2_MIXER_LIBRARY})

aux_source_directory(./src core_src_list)
# Generate GIT information in C source
include(SRoseGitInfo)

sr_git_log(commit_id_abbr "%h")
sr_git_log(commit_id "%H")
sr_git_log(commit_msg_raw "(%s)")
sr_git_log(commit_body_raw "(%b)")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/git_info.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/git_info.cpp
    @ONLY
    NEWLINE_STYLE LF
)

add_library("srose-core" STATIC ${core_src_list} ${CMAKE_CURRENT_BINARY_DIR}/git_info.cpp)
target_include_directories("srose-core" PUBLIC ./include)
target_link_libraries("srose-core" PUBLIC "srose-ext-glad")
target_link_libraries("srose-core" PUBLIC SDL2::SDL2)
target_link_libraries("srose-core" PUBLIC "srose-ext-sdl2_mixer")
target_compile_definitions("srose-core" PUBLIC SR_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR})
target_compile_definitions("srose-core" PUBLIC SR_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR})
target_compile_definitions("srose-core" PUBLIC SR_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH})
