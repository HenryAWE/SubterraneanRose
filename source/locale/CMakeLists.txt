# Subterranean Rose i18n module

# Boost.Spirit is header-only
find_package(Boost REQUIRED COMPONENTS locale)

aux_source_directory(./src locale_src_list)
add_library("srose-locale" STATIC ${locale_src_list})
target_include_directories("srose-locale" PUBLIC ./include)
target_link_libraries("srose-locale" PUBLIC Boost::locale)
target_link_libraries("srose-locale" PUBLIC "srose-util-headers")
target_link_libraries("srose-locale" PUBLIC "srose-core")
target_link_libraries("srose-locale" PUBLIC "srose-fs")

add_subdirectory(./test)
