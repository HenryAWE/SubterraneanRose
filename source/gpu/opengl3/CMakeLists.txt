# SubterraneanRose Modern OpenGL Module

# FFmpeg doesn't support find_package() of CMake
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
find_library(AVDEVICE_LIBRARY avdevice)

aux_source_directory(./src opengl_src_list)

add_library("srose-gpu-opengl3" STATIC ${opengl_src_list})
target_include_directories("srose-gpu-opengl3" PUBLIC ./include)

target_link_libraries("srose-gpu-opengl3" PUBLIC "srose-ext-glm")
target_link_libraries("srose-gpu-opengl3" PUBLIC "srose-ext-ffmpeg")
target_link_libraries("srose-gpu-opengl3" PUBLIC "srose-core")
target_link_libraries("srose-gpu-opengl3" PUBLIC "srose-util")
target_link_libraries("srose-gpu-opengl3" PUBLIC "srose-wm")
target_link_libraries("srose-gpu-opengl3" PUBLIC "srose-gpu")
