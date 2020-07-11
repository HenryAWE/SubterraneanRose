# Dependencies of Subterranean Rose
# Find SDL2
find_package(SDL2 REQUIRED)


# Compile stb libraries
add_library("srose-ext-stb" STATIC ./stb/compile_stb.c)
target_include_directories("srose-ext-stb" PUBLIC ./stb)


# Warp GLAD
add_library(glad STATIC ./glad/glad.c)
add_library(glad::glad ALIAS glad)
target_include_directories(glad PUBLIC ./glad)


# Wrap GLM
# GLM is header-only
add_library(glm INTERFACE)
add_library(glm::glm ALIAS glm)
target_include_directories(glm INTERFACE ./glm)


# Wrap imgui
aux_source_directory(./imgui imgui_src_list)
add_library(imgui STATIC ${imgui_src_list})
add_library(imgui::imgui ALIAS imgui)
target_include_directories(imgui PUBLIC ./imgui)
target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD=1)
target_link_libraries(imgui PRIVATE SDL2::Core)
target_link_libraries(imgui PRIVATE glad::glad)

add_library("srose-test" INTERFACE)
if(NOT ${SROSE_DISABLE_TEST})
    find_package(Boost REQUIRED QUIET COMPONENTS unit_test_framework)
    target_link_libraries("srose-test" INTERFACE Boost::unit_test_framework)
endif()

# {fmt}
add_subdirectory(./fmt)
