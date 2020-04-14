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

file(SIZE "${CMAKE_BINARY_DIR}/embedded_en.srlc" embedded_en_len)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/i18n/embedded_en.cpp.in ${CMAKE_BINARY_DIR}/embedded_en.cpp
    @ONLY
    NEWLINE_STYLE LF
)

add_library("srose-ui" STATIC ${ui_src_list} ${CMAKE_BINARY_DIR}/embedded_en.cpp)
target_include_directories("srose-ui" PUBLIC ./include)

target_link_libraries("srose-ui" PUBLIC "srose-core")
target_link_libraries("srose-ui" PUBLIC "srose-util")
target_link_libraries("srose-ui" PUBLIC "srose-wm")
target_link_libraries("srose-ui" PUBLIC "srose-gpu")
target_link_libraries("srose-ui" PUBLIC "srose-locale")
target_link_libraries("srose-ui" PUBLIC "srose-audio")
target_link_libraries("srose-ui" PUBLIC "srose-res")
target_link_libraries("srose-ui" PUBLIC "srose-player")
target_link_libraries("srose-ui" PUBLIC "srose-main")
if(NOT ${SROSE_DISABLE_CUI})
    target_link_libraries("srose-ui" PUBLIC Boost::program_options)
else()
    target_compile_definitions("srose-ui" PUBLIC SROSE_DISABLE_CUI=1)
endif()
if(${SROSE_DISABLE_DEMO})
    target_compile_definitions("srose-ui" PUBLIC SROSE_DISABLE_DEMO=1)
endif()
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options("srose-ui" PRIVATE "-Wno-format-security")
endif()

add_subdirectory(./test)
