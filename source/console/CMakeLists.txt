# SubterraneanRose Console UI

if(NOT ${SROSE_DISABLE_CLI})
    find_package(Boost REQUIRED COMPONENTS program_options)
endif()
aux_source_directory(./src console_src_list)
add_library("srose-console" STATIC ${console_src_list})
target_include_directories("srose-console" PUBLIC ./include)

target_link_libraries("srose-console" PUBLIC "srose-core")
target_link_libraries("srose-console" PRIVATE "srose-ui")
target_link_libraries("srose-console" PRIVATE "srose-i18n")
if(NOT ${SROSE_DISABLE_CLI})
    target_link_libraries("srose-console" PRIVATE Boost::program_options)
else()
    target_compile_definitions("srose-console" PRIVATE SROSE_DISABLE_CLI=1)
endif()
