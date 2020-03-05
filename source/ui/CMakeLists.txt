# SubterraneanRose User Interface Module

aux_source_directory(./src ui_src_list)
aux_source_directory(./src/i18n ui_i18n_src_list)
list(APPEND ui_src_list ${ui_i18n_src_list})
aux_source_directory(./src/gui ui_gui_src_list)
list(APPEND ui_src_list ${ui_gui_src_list})
aux_source_directory(./src/gui/editor ui_gui_editor_src_list)
list(APPEND ui_src_list ${ui_gui_editor_src_list})


# Console UI
if(NOT ${SROSE_DISABLE_CUI})
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
target_link_libraries("srose-ui" PUBLIC "srose-audio")
target_link_libraries("srose-ui" PUBLIC "srose-res")
if(NOT ${SROSE_DISABLE_CUI})
    target_link_libraries("srose-ui" PUBLIC Boost::program_options)
else()
    target_compile_definitions("srose-ui" PUBLIC SROSE_DISABLE_CUI=1)
endif()
if(${SROSE_DISABLE_DEMO})
    target_compile_definitions("srose-ui" PUBLIC SROSE_DISABLE_DEMO=1)
endif()

add_subdirectory(./test)
