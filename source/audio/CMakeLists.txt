# Subterranean Rose audio module

aux_source_directory(./src audio_src_list)
add_library("srose-audio" STATIC ${audio_src_list})
target_include_directories("srose-audio" PUBLIC ./include)
target_link_libraries("srose-audio" PUBLIC "srose-core")
target_link_libraries("srose-audio" PUBLIC "srose-fs")
