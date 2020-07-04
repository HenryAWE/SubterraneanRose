# SubterraneanRose Main Module


aux_source_directory(./src srmain_src_list)

add_library("srose-main" STATIC ${srmain_src_list})
target_include_directories("srose-main" PUBLIC ./include)
target_link_libraries("srose-main" PUBLIC "srose-fs")
target_link_libraries("srose-main" PUBLIC "srose-ui")

if(NOT ${SROSE_DISABLE_TEST})
    add_subdirectory(./test)
endif()
