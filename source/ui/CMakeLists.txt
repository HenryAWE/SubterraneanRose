# SubterraneanRose User Interface Module

aux_source_directory(./src ui_src_list)

option(ENABLE_CUI "Enable console inferface" ON)
if(${ENABLE_CUI})
    aux_source_directory(./src/console ui_console_src_list)
    list(APPEND ui_src_list ${ui_console_src_list})
    find_package(Boost REQUIRED COMPONENTS program_options)
endif()

add_library("srose-ui" STATIC ${ui_src_list})
target_include_directories("srose-ui" PUBLIC ./include)

target_link_libraries("srose-ui" PUBLIC "srose-core")
target_link_libraries("srose-ui" PUBLIC "srose-wm")
if(${ENABLE_CUI})
    target_link_libraries("srose-ui" PUBLIC Boost::program_options)
    target_compile_definitions("srose-ui" PUBLIC SR_ENABLE_CUI=1)
endif()
