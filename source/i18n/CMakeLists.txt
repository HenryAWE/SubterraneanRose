# SubterraneanRose Internationalization Module

aux_source_directory(./src i18n_src_list)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/embedded_C.cpp.in ${CMAKE_BINARY_DIR}/embedded_C.cpp
    @ONLY
    NEWLINE_STYLE LF
)

add_library("srose-i18n" STATIC ${i18n_src_list} ${CMAKE_BINARY_DIR}/embedded_C.cpp)
target_include_directories("srose-i18n" PUBLIC ./include)

target_link_libraries("srose-i18n" PUBLIC "srose-core")
target_link_libraries("srose-i18n" PUBLIC "srose-locale")
target_link_libraries("srose-i18n" PRIVATE "srose-trace")
