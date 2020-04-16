# SubterraneanRose Source
find_package(SDL2 REQUIRED)

# Compile options
# Force UTF-8
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
# Make sure macro "__cplusplus" is correctly defined
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")


# Build options
option(SROSE_DISABLE_CUI "Disable console inferface" OFF)
option(SROSE_DISABLE_DEMO "Disable demo codes" OFF)
set(SROSE_RENDER_BACKEND "OpenGL3" CACHE STRING "Choose a render backend from: OpenGL3")


# Dependencies
add_subdirectory(./ext)


# Libraries
add_subdirectory(./trace)
add_subdirectory(./core)
add_subdirectory(./util)
add_subdirectory(./fs)
add_subdirectory(./wm)
add_subdirectory(./gpu)
add_subdirectory(./locale)
add_subdirectory(./audio)
add_subdirectory(./res)
add_subdirectory(./srose)
add_subdirectory(./player)
add_subdirectory(./editor)
add_subdirectory(./ui)


# Executable
set(sr_exec_src_list main.cpp initialize.cpp)
if(MSVC)
    include(SRoseGitInfo)
    sr_git_log(commit_id_abbr_str "%h")
    string(REPLACE "\"" "" commit_id_abbr ${commit_id_abbr_str})
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/verinfo.rc.in" "verinfo.rc")
    list(APPEND sr_exec_src_list "${CMAKE_CURRENT_BINARY_DIR}/verinfo.rc")
endif()
add_executable("srose-exec" ${sr_exec_src_list})

if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries("srose-exec" PRIVATE pthread)
endif()
target_link_libraries("srose-exec" PRIVATE SDL2::Core SDL2::Entry)
target_link_libraries("srose-exec" PRIVATE "srose-core")
target_link_libraries("srose-exec" PRIVATE "srose-wm")
target_link_libraries("srose-exec" PRIVATE "srose-player")
target_link_libraries("srose-exec" PRIVATE "srose-ui")
set_target_properties("srose-exec" PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)


# Installation
install(
    TARGETS "srose-exec"
    RUNTIME DESTINATION bin
)

# Set the information of the package
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE" DESTINATION ./)
include(CPack)
if(WIN32)
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION bin)
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE CONFIGURATIONS Debug)
        set(CMAKE_INSTALL_DEBUG_LIBRARIES_ONLY TRUE CONFIGURATIONS Debug)
    endif()
    include(InstallRequiredSystemLibraries)
    install(
        DIRECTORY ${PROJECT_BINARY_DIR}/bin
        DESTINATION .
        FILES_MATCHING PATTERN "*.dll"
    )
endif()
