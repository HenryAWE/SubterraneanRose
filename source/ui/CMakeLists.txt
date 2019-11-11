# SubterraneanRose User Interface Module

aux_source_directory(./src ui_src_list)
aux_source_directory(./src/main_menu ui_main_menu_src_list)
list(APPEND ui_src_list ${ui_main_menu_src_list})
aux_source_directory(./src/i18n ui_i18n_src_list)
list(APPEND ui_src_list ${ui_i18n_src_list})
aux_source_directory(./src/debug ui_debug_src_list)
list(APPEND ui_src_list ${ui_debug_src_list})


# Console UI
if(${SROSE_ENABLE_CUI})
    find_package(Boost REQUIRED COMPONENTS program_options)
endif()
aux_source_directory(./src/console ui_console_src_list)
list(APPEND ui_src_list ${ui_console_src_list})

add_library("srose-ui" STATIC ${ui_src_list})
target_include_directories("srose-ui" PUBLIC ./include)

target_link_libraries("srose-ui" PUBLIC "srose-core")
target_link_libraries("srose-ui" PUBLIC "srose-util")
target_link_libraries("srose-ui" PUBLIC "srose-wm")
target_link_libraries("srose-ui" PUBLIC "srose-gpu")
target_link_libraries("srose-ui" PUBLIC "srose-locale")
if(${SROSE_ENABLE_CUI})
    target_link_libraries("srose-ui" PUBLIC Boost::program_options)
    target_compile_definitions("srose-ui" PUBLIC SR_ENABLE_CUI=1)
endif()

add_subdirectory(./test)
