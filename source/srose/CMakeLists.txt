# SubterraneanRose Main Module

find_package(Boost REQUIRED QUIET COMPONENTS headers)

aux_source_directory(./src srmain_src_list)

add_library("srose-main" STATIC ${srmain_src_list})
target_include_directories("srose-main" PUBLIC ./include)
target_link_libraries("srose-main" PUBLIC "srose-fs")
target_link_libraries("srose-main" PUBLIC Boost::headers)

add_subdirectory(./test)
