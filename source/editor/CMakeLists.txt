# SubterraneanRose Editor Module

find_package(Boost REQUIRED QUIET COMPONENTS serialization)

aux_source_directory(./src editor_src_list)

add_library("srose-editor" STATIC ${editor_src_list})
target_include_directories("srose-editor" PUBLIC ./include)
target_link_libraries("srose-editor" PUBLIC Boost::serialization)

if(NOT ${SROSE_DISABLE_TEST})
    add_subdirectory(./test)
endif()
