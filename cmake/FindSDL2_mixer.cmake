# FindSDL2_mixer.cmake for Subterranean Rose to find the SDL_mixer library
# This file is distributed under GPLv3, see LICENSE file in project directory for details
# Modified from the FindSDL2_mixer.cmake of https://github.com/opeik/cmake-modern-findsdl2


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


# Licensed under the ISC license. Please see the LICENSE.md file for details.
# Copyright (c) 2019 Sandro Stikić <https://github.com/opeik>

#[[============================================================================
FindSDL2
---------

Try to find SDL2_mixer.

This module defines the following IMPORTED targets:
- SDL2::Mixer — Link against this.

This module defines the following variables:
- SDL2_MIXER_FOUND
- SDL2_MIXER_VERSION_STRING
- SDL2_MIXER_LIBRARIES (deprecated)
- SDL2_MIXER_INCLUDE_DIRS (deprecated)
#============================================================================]]

# Ensure SDL2 is installed.
find_package(SDL2 REQUIRED QUIET)

if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found")
endif()

# Look for SDL2_mixer.
find_path(SDL2_MIXER_INCLUDE_DIR SDL_mixer.h
    PATH_SUFFIXES SDL2 include/SDL2 include
    PATHS ${SDL2_MIXER_PATH}
)

if (NOT SDL2_MIXER_LIBRARIES)
    # Determine architecture.
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_SDL2_MIXER_PATH_SUFFIX lib/x64)
    else()
        set(_SDL2_MIXER_PATH_SUFFIX lib/x86)
    endif()

    # Look for the release version of SDL2.
    find_library(SDL2_MIXER_LIBRARY_RELEASE
        NAMES SDL2_mixer
        PATH_SUFFIXES lib ${_SDL2_MIXER_PATH_SUFFIX}
        PATHS ${SDL2_PATH} ${SDL2_MIXER_PATH}
    )

    # Look for the debug version of SDL2.
    find_library(SDL2_MIXER_LIBRARY_DEBUG
        NAMES SDL2_mixerd
        PATH_SUFFIXES lib ${_SDL2_MIXER_PATH_SUFFIX}
        PATHS ${SDL2_PATH}} ${SDL2_MIXER_PATH}
    )

    include(SelectLibraryConfigurations)
    select_library_configurations(SDL2_MIXER)
endif()

# Find the SDL2_mixer version.
if(SDL2_MIXER_INCLUDE_DIR AND EXISTS "${SDL2_MIXER_INCLUDE_DIR}/SDL_mixer.h")
    file(STRINGS "${SDL2_MIXER_INCLUDE_DIR}/SDL_mixer.h" SDL2_MIXER_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL_MIXER_MAJOR_VERSION[ \t]+[0-9]+$")
    file(STRINGS "${SDL2_MIXER_INCLUDE_DIR}/SDL_mixer.h" SDL2_MIXER_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL_MIXER_MINOR_VERSION[ \t]+[0-9]+$")
    file(STRINGS "${SDL2_MIXER_INCLUDE_DIR}/SDL_mixer.h" SDL2_MIXER_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL_MIXER_PATCHLEVEL[ \t]+[0-9]+$")
    string(REGEX REPLACE "^#define[ \t]+SDL_MIXER_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_MIXER_VERSION_MAJOR "${SDL2_MIXER_VERSION_MAJOR_LINE}")
    string(REGEX REPLACE "^#define[ \t]+SDL_MIXER_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_MIXER_VERSION_MINOR "${SDL2_MIXER_VERSION_MINOR_LINE}")
    string(REGEX REPLACE "^#define[ \t]+SDL_MIXER_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL2_MIXER_VERSION_PATCH "${SDL2_MIXER_VERSION_PATCH_LINE}")
    set(SDL2_MIXER_VERSION_STRING ${SDL2_MIXER_VERSION_MAJOR}.${SDL2_MIXER_VERSION_MINOR}.${SDL2_MIXER_VERSION_PATCH})
    unset(SDL2_MIXER_VERSION_MAJOR_LINE)
    unset(SDL2_MIXER_VERSION_MINOR_LINE)
    unset(SDL2_MIXER_VERSION_PATCH_LINE)
    unset(SDL2_MIXER_VERSION_MAJOR)
    unset(SDL2_MIXER_VERSION_MINOR)
    unset(SDL2_MIXER_VERSION_PATCH)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2_mixer
    REQUIRED_VARS SDL2_MIXER_LIBRARIES SDL2_MIXER_INCLUDE_DIR
    VERSION_VAR SDL2_MIXER_VERSION_STRING)

if(SDL2_MIXER_FOUND)
    set(SDL2_MIXER_LIBRARIES ${SDL2_MIXER_LIBRARY})
    set(SDL2_MIXER_INCLUDE_DIR ${SDL2_MIXER_INCLUDE_DIR})

    # Define the SDL2_mixer target.
    if(NOT TARGET SDL2::Mixer)
        add_library(SDL2::Mixer UNKNOWN IMPORTED)
        set_target_properties(SDL2::Mixer PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${SDL2_MIXER_INCLUDE_DIR})

        if(SDL2_MIXER_LIBRARY_RELEASE)
            set_property(TARGET SDL2::Mixer APPEND PROPERTY
                IMPORTED_CONFIGURATIONS RELEASE)
            set_target_properties(SDL2::Mixer PROPERTIES
                IMPORTED_LOCATION_RELEASE ${SDL2_MIXER_LIBRARY_RELEASE})
        endif()

        if(SDL2_MIXER_LIBRARY_DEBUG)
            set_property(TARGET SDL2::Mixer APPEND PROPERTY
                IMPORTED_CONFIGURATIONS DEBUG)
            set_target_properties(SDL2::Mixer PROPERTIES
                IMPORTED_LOCATION_DEBUG ${SDL2_MIXER_LIBRARY_DEBUG})
        endif()

        if(NOT SDL2_MIXER_LIBRARY_RELEASE AND NOT SDL2_MIXER_LIBRARY_DEBUG)
            set_property(TARGET SDL2::Mixer APPEND PROPERTY
                IMPORTED_LOCATION ${SDL2_MIXER_LIBRARY})
        endif()
    endif()
endif()

mark_as_advanced(SDL2_MIXER_INCLUDE_DIR)
