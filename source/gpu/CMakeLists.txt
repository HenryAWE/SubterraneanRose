# SubterraneanRose GPU Module

add_subdirectory(./opengl3)

add_library("srose-gpu" INTERFACE)
target_link_libraries("srose-gpu" INTERFACE "srose-gpu-opengl3")
