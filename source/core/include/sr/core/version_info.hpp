/**
 * @file version_info.hpp
 * @author HenryAWE
 * @brief Version information
 */

#ifndef SROSE_CORE_version_info_hpp_
#define SROSE_CORE_version_info_hpp_

#include "macros.hpp"
#include <stdint.h>


namespace srose::core
{
    /**
     * @brief Get version string
     * 
     * @return const char* A string that contain version information
     */
    const char* SRSCALL GetVersionString();

    /**
     * @brief Get the short GIT commit id of current build
     * 
     * @return const char* The string represent the short hash value of the commit
     */
    const char* SRSCALL GitCommitShortID();
} // namespace srose::core


#endif
