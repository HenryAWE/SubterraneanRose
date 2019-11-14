# Dependencies of Subterranean Rose
# Find SDL2
find_package(SDL2 CONFIG REQUIRED)


# Compile stb libraries
add_library("srose-ext-stb" STATIC ./stb/compile_stb.c)
target_include_directories("srose-ext-stb" PUBLIC ./stb)


# Warp GLAD
add_library("srose-ext-glad" STATIC ./glad/glad.c)
target_include_directories("srose-ext-glad" PUBLIC ./glad)


# Wrap GLM
# GLM is header-only
add_library("srose-ext-glm" INTERFACE)
target_include_directories("srose-ext-glm" INTERFACE ./glm)


# Wrap imgui
aux_source_directory(./imgui imgui_src_list)
add_library("srose-ext-imgui" STATIC ${imgui_src_list})
target_include_directories("srose-ext-imgui" PUBLIC ./imgui)
target_compile_definitions("srose-ext-imgui" PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD=1)
target_link_libraries("srose-ext-imgui" PRIVATE SDL2::SDL2)
target_link_libraries("srose-ext-imgui" PRIVATE "srose-ext-glad")


# FFmpeg libraries
# FFmpeg doesn't support find_package() of CMake
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
find_library(AVDEVICE_LIBRARY avdevice)

find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)
find_library(SWSCALE_LIBRARY swscale)

add_library("srose-ext-ffmpeg" INTERFACE)
target_include_directories("srose-ext-ffmpeg" INTERFACE
    ${AVCODEC_INCLUDE_DIR}
    ${AVFORMAT_INCLUDE_DIR}
    ${AVUTIL_INCLUDE_DIR}
    ${AVDEVICE_INCLUDE_DIR}
    ${SWSCALE_INCLUDE_DIR}
)
target_link_libraries("srose-ext-ffmpeg" INTERFACE
    ${AVCODEC_LIBRARY}
    ${AVFORMAT_LIBRARY}
    ${AVUTIL_LIBRARY}
    ${AVDEVICE_LIBRARY}
    ${SWSCALE_LIBRARY}
)

find_package(Boost REQUIRED QUIET COMPONENTS unit_test_framework)
add_library("srose-test" INTERFACE)
target_link_libraries("srose-test" INTERFACE Boost::unit_test_framework)
