# SubterraneanRose Player Module

aux_source_directory(./src player_src_list)
aux_source_directory(./src/entity player_entity_src_list)
aux_source_directory(./src/component player_component_src_list)
aux_source_directory(./src/system player_system_src_list)
add_library("srose-player" STATIC
    ${player_src_list}
    ${player_entity_src_list}
    ${player_component_src_list}
    ${player_system_src_list}
)
target_include_directories("srose-player" PUBLIC ./include)

target_link_libraries("srose-player" PUBLIC "srose-core")
target_link_libraries("srose-player" PUBLIC "srose-wm")
target_link_libraries("srose-player" PRIVATE "srose-ui")
target_link_libraries("srose-player" PUBLIC glm::glm)
if(${SROSE_DISABLE_DEMO})
    target_compile_definitions("srose-player" PUBLIC SROSE_DISABLE_DEMO=1)
    target_link_libraries("srose-player" PRIVATE "srose-graphic")
endif()

# Tests
if(NOT ${SROSE_DISABLE_TEST})
    add_subdirectory(./test/ecs_test)
endif()
