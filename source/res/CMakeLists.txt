# SubterraneanRose Resource Manager Module

aux_source_directory(./src resmgr_src_list)
add_library("srose-res" STATIC ${resmgr_src_list})
target_include_directories("srose-res" PUBLIC ./include)
target_link_libraries("srose-res" PRIVATE "srose-locale")
target_link_libraries("srose-res" PUBLIC "srose-gpu")
target_link_libraries("srose-res" PUBLIC "srose-fs")

if(NOT ${SROSE_DISABLE_TEST})
    add_subdirectory(./test/vfs_test)
endif()
