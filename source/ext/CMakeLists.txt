# Dependencies of Subterranean Rose
# Find SDL2
find_package(SDL2 REQUIRED)


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
target_link_libraries("srose-ext-imgui" PRIVATE SDL2::Core)
target_link_libraries("srose-ext-imgui" PRIVATE "srose-ext-glad")


find_package(Boost REQUIRED QUIET COMPONENTS unit_test_framework)
add_library("srose-test" INTERFACE)
target_link_libraries("srose-test" INTERFACE Boost::unit_test_framework)
