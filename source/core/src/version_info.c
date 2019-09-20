/**
 * @file version_info.c
 * @author HenryAWE
 * @brief Version information
 */

#include <sr/sys/version_info.h>
#include <stdio.h>


extern uint64_t sr_git_commit_id;

const char* SRSCALL SR_SYS_GetVersionString(void)
{
    static char buf[32];
    snprintf(
        buf, 32,
        "Ver. %d.%d.%d",
        SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH
    );

    return buf;
}

uint64_t SRSCALL SR_SYS_GitCommitID(void)
{
    return sr_git_commit_id;
}
