# SubterraneanRose Modern OpenGL Module

aux_source_directory(./src opengl3_src_list)

add_library("srose-opengl3" STATIC ${opengl3_src_list})
target_include_directories("srose-opengl3" PUBLIC ./include)

target_link_libraries("srose-opengl3" PUBLIC glm::glm)
target_link_libraries("srose-opengl3" PUBLIC "srose-ext-stb")
target_link_libraries("srose-opengl3" PUBLIC "srose-core")
target_link_libraries("srose-opengl3" PUBLIC "srose-util")
target_link_libraries("srose-opengl3" PUBLIC "srose-wm")
target_link_libraries("srose-opengl3" PUBLIC "srose-ui")
target_link_libraries("srose-opengl3" PUBLIC "srose-graphic")
if(${SROSE_DISABLE_DEMO})
    target_compile_definitions("srose-opengl3" PUBLIC SROSE_DISABLE_DEMO=1)
endif()
