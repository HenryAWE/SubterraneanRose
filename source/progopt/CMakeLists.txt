# SubterraneanRose Program Options

if(NOT ${SROSE_DISABLE_CLI})
    find_package(Boost REQUIRED COMPONENTS program_options)
endif()
aux_source_directory(./src progopt_src_list)
add_library("srose-progopt" STATIC ${progopt_src_list})
target_include_directories("srose-progopt" PUBLIC ./include)

target_link_libraries("srose-progopt" PUBLIC "srose-core")
target_link_libraries("srose-progopt" PRIVATE "srose-util")
target_link_libraries("srose-progopt" PRIVATE "srose-ui")
target_link_libraries("srose-progopt" PRIVATE "srose-i18n")
if(NOT ${SROSE_DISABLE_CLI})
    target_link_libraries("srose-progopt" PRIVATE Boost::program_options)
else()
    target_compile_definitions("srose-progopt" PRIVATE SROSE_DISABLE_CLI=1)
endif()
