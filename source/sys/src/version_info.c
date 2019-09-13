/**
 * @file version_info.c
 * @author HenryAWE
 * @brief Version information
 */

#include <sr/sys/version_info.h>
#include <stdio.h>

const char* SRSCALL SR_SYS_GetVersionString()
{
    static char buf[32];
    snprintf(
        buf, 32,
        "Ver. %d.%d.%d",
        SR_VERSION_MAJOR, SR_VERSION_MINOR, SR_VERSION_PATCH
    );

    return buf;
}
