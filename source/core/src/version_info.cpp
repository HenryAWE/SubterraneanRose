/**
 * @file version_info.cpp
 * @author HenryAWE
 * @brief Version information
 */

#include <sr/core/version_info.hpp>
#include <stdio.h>


extern "C"
extern const char* sr_git_commit_id_short;

namespace srose::core
{
    const char* SRSCALL GetVersionString()
    {
        static char buf[32];
        snprintf(
            buf, 32,
            "Ver. %d.%d.%d",
            SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH
        );

        return buf;
    }

    const char* SRSCALL GitCommitShortID()
    {
        return sr_git_commit_id_short;
    }
} // namespace srose::core
