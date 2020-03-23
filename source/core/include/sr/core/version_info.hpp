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
    const char* SRSCALL GetVersionString() noexcept;

    /**
     * @brief Get the short GIT commit id of current build
     * 
     * @return const char* The string represent the short hash value of the commit
     */
    const char* SRSCALL GitCommitShortID() noexcept;
    /**
     * @brief Get the GIT commit id of current build
     * 
     * @return const char* The string represent the hash value of the commit
     */
    const char* SRSCALL GitCommitID() noexcept;
    /**
     * @brief Get the commit message of current build
     * 
     * @return const char* The message of the last commit
     */
    const char* SRSCALL GitCommitMsg() noexcept;
} // namespace srose::core


#endif
