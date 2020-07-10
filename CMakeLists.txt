# SubterraneanRose Project Root

cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/;${CMAKE_MODULE_PATH}")
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    message(STATUS "Use vcpkg toolchain from $ENV{VCPKG_ROOT}")
endif()

project(
    "Subterranean Rose"
    VERSION 0.1.0
    DESCRIPTION "The Subterranean Rose Project"
    HOMEPAGE_URL "https://github.com/HenryAWE/SubterraneanRose"

    LANGUAGES C CXX
)

option(SROSE_DISABLE_TEST "Disable test cases" OFF)

if(NOT ${SROSE_DISABLE_TEST})
    enable_testing()
endif()

# Dependencies
add_subdirectory(./external)

# WRANING: Don't change the order of the add_subdirectory(...)
# The compilation of locale scripts require python scripts in the ./tools directory
# The the embedded English locale file, which is generated from the locale script, is required by the UI module
add_subdirectory(./tools)
add_subdirectory(./locale)
add_subdirectory(./source)
