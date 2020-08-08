# SubterraneanRose Core Module
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

aux_source_directory(./src core_src_list)
# Generate GIT information in C source
include(SRoseGitInfo)

sr_git_log(commit_id_abbr "%h")
sr_git_log(commit_id "%H")
sr_git_log(commit_msg_raw "(%s)")
sr_git_log(commit_body_raw "(%b)")
sr_git_output(git_branch "rev-parse" "--abbrev-ref" "HEAD")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/git_info.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/git_info.cpp
    @ONLY
    NEWLINE_STYLE LF
)

add_library("srose-core" STATIC ${core_src_list} ${CMAKE_CURRENT_BINARY_DIR}/git_info.cpp)
target_include_directories("srose-core" PUBLIC ./include)
target_link_libraries("srose-core" PUBLIC glad::glad)
target_link_libraries("srose-core" PUBLIC SDL2::Core)
target_link_libraries("srose-core" PUBLIC SDL2::Mixer)
target_link_libraries("srose-core" PRIVATE fmt)
target_link_libraries("srose-core" PRIVATE "srose-trace")
target_compile_definitions("srose-core" PUBLIC SR_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR})
target_compile_definitions("srose-core" PUBLIC SR_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR})
target_compile_definitions("srose-core" PUBLIC SR_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH})
target_compile_definitions("srose-core" PRIVATE SR_CORE_BUILD_SUFFIX="${SROSE_BUILD_SUFFIX}")
