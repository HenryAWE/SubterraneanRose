# SubterraneanRose Core Module
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)

aux_source_directory(./src sys_src_list)
# Generate GIT information in C source
find_program(git_found git)
if(git_found)
    execute_process(
        COMMAND git log --pretty=format:"%h" -1
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE commit_id_short
    )

    message(STATUS "Git commit id: ${commit_id_short}")
    string(CONCAT git_info
        "/*File generated by source/sys/CMakeLists.txt */\n"
        "const char* sr_git_commit_id_short=${commit_id_short}\;\n"
    )
    file(
        WRITE ${CMAKE_CURRENT_BINARY_DIR}/git_info.c
        ${git_info}
    )
endif()

add_library("srose-core" STATIC ${sys_src_list} ${CMAKE_CURRENT_BINARY_DIR}/git_info.c)
target_include_directories("srose-core" PUBLIC ./include)

target_link_libraries("srose-core" PUBLIC SDL2::SDL2)
target_link_libraries("srose-core" PUBLIC SDL2::SDL2_mixer)
target_link_libraries("srose-core" PUBLIC SDL2::SDL2_image)