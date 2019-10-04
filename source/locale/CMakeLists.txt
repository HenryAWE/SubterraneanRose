# Subterranean Rose i18n module

# Boost.Spirit is header-only
find_package(Boost REQUIRED COMPONENTS headers)

aux_source_directory(./src locale_src_list)
add_library("srose-locale" STATIC ${locale_src_list})
target_include_directories("srose-locale" PUBLIC ./include)
target_link_libraries("srose-locale" PRIVATE Boost::headers)
target_link_libraries("srose-locale" PUBLIC "srose-util-headers")

add_subdirectory(./test)
