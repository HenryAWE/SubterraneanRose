# FindSDL2.cmake for Subterranean Rose to find the SDL library
# This file is distributed under GPLv3, see LICENSE file in project directory for details
# Modified from the FindSDL2.cmake of https://github.com/opeik/cmake-modern-findsdl2,


# Copyright © 2019 Sandro Stikić

# Permission to use, copy, modify, and/or distribute this software for
# any purpose with or without fee is hereby granted, provided that the
# above copyright notice and this permission notice appear in all
# copies.

# The software is provided "as is" and the author disclaims all
# warranties with regard to this software including all implied
# warranties of merchantability and fitness. In no event shall the
# author be liable for any special, direct, indirect, or consequential
# damages or any damages whatsoever resulting from loss of use, data or
# profits, whether in an action of contract, negligence or other
# tortious action, arising out of or in connection with the use or
# performance of this software.


#[[============================================================================
FindSDL2
---------

Try to find SDL2.

This module defines the following IMPORTED targets:
- SDL2::Core — Libraries should link against this.

This module defines the following variables:
- SDL2_FOUND
- SDL2main_FOUND
- SDL2_VERSION_STRING
- SDL2_LIBRARIES (deprecated)
- SDL2_INCLUDE_DIRS (deprecated)
#============================================================================]]

# Look for SDL2.
find_path(SDL2_INCLUDE_DIR SDL.h
    PATH_SUFFIXES SDL2 include/SDL2 include
    PATHS ${SDL2_PATH}
)

if (NOT SDL2_LIBRARIES)
    # Determine architecture.
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_SDL2_PATH_SUFFIX lib/x64)
    else()
        set(_SDL2_PATH_SUFFIX lib/x86)
    endif()

    # Look for the release version of SDL2.
    find_library(SDL2_LIBRARY_RELEASE
        NAMES SDL2 SDL-2.0
        PATH_SUFFIXES lib ${_SDL2_PATH_SUFFIX}
        PATHS ${SDL2_PATH}
    )

    # Look for the debug version of SDL2.
    find_library(SDL2_LIBRARY_DEBUG
        NAMES SDL2d
        PATH_SUFFIXES lib ${_SDL2_PATH_SUFFIX}
        PATHS ${SDL2_PATH}
    )

    # Modified by Subterranean Rose: Supporting different configuration of SDL2main
    # Look for the release version of SDL2main.
    find_library(SDL2main_LIBRARY_RELEASE
        NAMES SDL2main
        PATH_SUFFIXES lib ${_SDL2_PATH_SUFFIX}
        PATHS ${SDL2_PATH}
    )
    # Look for the debug version of SDL2main.
    find_library(SDL2main_LIBRARY_DEBUG
        NAMES SDL2maind
        PATH_SUFFIXES lib ${_SDL2_PATH_SUFFIX}
        PATHS ${SDL2_PATH}
    )

    include(SelectLibraryConfigurations)
    select_library_configurations(SDL2)
endif()

# Find the SDL2 version.
if(SDL2_INCLUDE_DIR AND EXISTS "${SDL2_INCLUDE_DIR}/SDL_version.h")
    file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL2_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL_MAJOR_VERSION[ \t]+[0-9]+$")
    file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL2_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL_MINOR_VERSION[ \t]+[0-9]+$")
    file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL2_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL_PATCHLEVEL[ \t]+[0-9]+$")
    string(REGEX REPLACE "^#define[ \t]+SDL_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_VERSION_MAJOR "${SDL2_VERSION_MAJOR_LINE}")
    string(REGEX REPLACE "^#define[ \t]+SDL_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_VERSION_MINOR "${SDL2_VERSION_MINOR_LINE}")
    string(REGEX REPLACE "^#define[ \t]+SDL_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL2_VERSION_PATCH "${SDL2_VERSION_PATCH_LINE}")
    set(SDL2_VERSION_STRING ${SDL2_VERSION_MAJOR}.${SDL2_VERSION_MINOR}.${SDL2_VERSION_PATCH})
    unset(SDL2_VERSION_MAJOR_LINE)
    unset(SDL2_VERSION_MINOR_LINE)
    unset(SDL2_VERSION_PATCH_LINE)
    unset(SDL2_VERSION_MAJOR)
    unset(SDL2_VERSION_MINOR)
    unset(SDL2_VERSION_PATCH)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2
    REQUIRED_VARS SDL2_LIBRARIES SDL2_INCLUDE_DIR
    VERSION_VAR SDL2_VERSION_STRING)

find_package_handle_standard_args(SDL2main
    REQUIRED_VARS SDL2main_LIBRARIES SDL2_INCLUDE_DIR
    VERSION_VAR SDL2_VERSION_STRING)

if(SDL2_FOUND)
    set(SDL2_LIBRARIES ${SDL2_LIBRARY})
    set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIR})

    # Define the core SDL2 target.
    if(NOT TARGET SDL2::Core)
        add_library(SDL2::Core UNKNOWN IMPORTED)
        set_target_properties(SDL2::Core PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${SDL2_INCLUDE_DIR})

        # Link against Cocoa on macOS.
        if(APPLE)
            set_property(TARGET SDL2::Core APPEND PROPERTY
              INTERFACE_LINK_OPTIONS -framework Cocoa)
        endif()

        if(SDL2_LIBRARY_RELEASE)
            set_property(TARGET SDL2::Core APPEND PROPERTY
                IMPORTED_CONFIGURATIONS RELEASE)
            set_target_properties(SDL2::Core PROPERTIES
                IMPORTED_LOCATION_RELEASE ${SDL2_LIBRARY_RELEASE})
        endif()

        if(SDL2_LIBRARY_DEBUG)
            set_property(TARGET SDL2::Core APPEND PROPERTY
                IMPORTED_CONFIGURATIONS DEBUG)
            set_target_properties(SDL2::Core PROPERTIES
                IMPORTED_LOCATION_DEBUG ${SDL2_LIBRARY_DEBUG})
        endif()

        if(NOT SDL2_LIBRARY_RELEASE AND NOT SDL2_LIBRARY_DEBUG)
            set_property(TARGET SDL2::Core APPEND PROPERTY
                IMPORTED_LOCATION ${SDL2_LIBRARY})
        endif()
    endif()

    # Modified by Subterranean Rose: Supporting different configuration of SDL2main
    # Define the SDL2::Entry target.
    if(NOT TARGET SDL2::Entry)
        add_library(SDL2::Entry IMPORTED UNKNOWN)
        if(SDL2main_LIBRARY_RELEASE)
            set_property(TARGET SDL2::Entry APPEND PROPERTY
                IMPORTED_CONFIGURATIONS RELEASE)
            set_target_properties(SDL2::Entry PROPERTIES
                IMPORTED_LOCATION_RELEASE ${SDL2main_LIBRARY_RELEASE})
        endif()

        if(SDL2main_LIBRARY_DEBUG)
            set_property(TARGET SDL2::Entry APPEND PROPERTY
                IMPORTED_CONFIGURATIONS DEBUG)
            set_target_properties(SDL2::Entry PROPERTIES
                IMPORTED_LOCATION_DEBUG ${SDL2main_LIBRARY_DEBUG})
        endif()

        if(NOT SDL2main_LIBRARY_RELEASE AND NOT SDL2main_LIBRARY_DEBUG)
        set_property(TARGET SDL2::Entry APPEND PROPERTY
            IMPORTED_LOCATION ${SDL2main_LIBRARY})
        endif()
    endif()
endif()

mark_as_advanced(SDL2_INCLUDE_DIR)
mark_as_advanced(SDL2main_LIBRARY_RELEASE SDL2main_LIBRARY_DEBUG)
