# SubterraneanRose Source
find_package(SDL2 CONFIG REQUIRED)

if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

add_compile_definitions(SR_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR})
add_compile_definitions(SR_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR})
add_compile_definitions(SR_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH})

add_subdirectory(./core)
add_subdirectory(./wm)
add_subdirectory(./gpu)
add_subdirectory(./res)
add_subdirectory(./srose)
add_subdirectory(./player)
add_subdirectory(./editor)
add_subdirectory(./ui)

aux_source_directory(. src_list)

add_executable("srose-exec" ${src_list})

target_link_libraries("srose-exec" PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries("srose-exec" PRIVATE "srose-core")
target_link_libraries("srose-exec" PRIVATE "srose-wm")
target_link_libraries("srose-exec" PRIVATE "srose-player")
target_link_libraries("srose-exec" PRIVATE "srose-ui")
set_target_properties("srose-exec" PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

install(
    TARGETS "srose-exec"
    RUNTIME DESTINATION bin
)

# Set the information of the package
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/license.txt")
install(FILES "${PROJECT_SOURCE_DIR}/license.txt" DESTINATION ./)
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
