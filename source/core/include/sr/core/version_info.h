/**
 * @file version_info.h
 * @author HenryAWE
 * @brief Version information
 */

#ifndef SROSE_CORE_version_info_h_
#define SROSE_CORE_version_info_h_


#include "macros.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get version string
 * 
 * @return const char* A string that contain version information
 */
const char* SRSCALL SR_CORE_GetVersionString(void);

/**
 * @brief Get the short GIT commit id of current build
 * 
 * @return const char* The string represent the short hash value of the commit
 */
const char* SRSCALL SR_CORE_GitCommitShortID(void);


#ifdef __cplusplus
}
#endif

#endif
