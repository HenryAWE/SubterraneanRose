# Subterranean Rose i18n module

# Boost.Spirit is header-only
find_package(Boost REQUIRED COMPONENTS locale)

aux_source_directory(./src locale_src_list)
aux_source_directory(./src/v1 locale_impl_v1_src_list)
add_library("srose-locale" STATIC ${locale_src_list} ${locale_impl_v1_src_list})
target_include_directories("srose-locale" PUBLIC ./include)
target_link_libraries("srose-locale" PUBLIC Boost::locale)
target_link_libraries("srose-locale" PUBLIC "srose-headers")
target_link_libraries("srose-locale" PUBLIC "srose-core")
target_link_libraries("srose-locale" PUBLIC "srose-fs")

if(NOT ${SROSE_DISABLE_TEST})
    add_subdirectory(./test/parser_test)
    add_subdirectory(./test/language_test)
endif()
