# SubterraneanRose Player Module

aux_source_directory(./src player_src_list)
add_library("srose-player" STATIC ${player_src_list})
target_include_directories("srose-player" PUBLIC ./include)

target_link_libraries("srose-player" PUBLIC "srose-core")
target_link_libraries("srose-player" PUBLIC "srose-wm")
target_link_libraries("srose-player" PUBLIC "srose-gpu")
target_link_libraries("srose-player" PRIVATE "srose-ui")
target_link_libraries("srose-player" PUBLIC "srose-ext-glm")
if(${SROSE_DISABLE_DEMO})
    target_compile_definitions("srose-player" PUBLIC SROSE_DISABLE_DEMO=1)
endif()
