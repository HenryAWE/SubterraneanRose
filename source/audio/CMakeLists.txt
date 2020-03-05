# Subterranean Rose audio module

aux_source_directory(./src audio_src_list)
add_library("srose-audio" STATIC ${audio_src_list})
target_include_directories("srose-audio" PUBLIC ./include)
target_link_libraries("srose-audio" PUBLIC "srose-core")
target_link_libraries("srose-audio" PUBLIC "srose-fs")
target_link_libraries("srose-audio" PRIVATE "srose-ui")
if(${SROSE_DISABLE_DEMO})
    target_compile_definitions("srose-audio" PUBLIC SROSE_DISABLE_DEMO=1)
endif()
