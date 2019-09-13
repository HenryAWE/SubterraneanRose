/**
 * @file version_info.h
 * @author HenryAWE
 * @brief Version information
 */

#ifndef SROSE_SYS_version_info_h_
#define SROSE_SYS_version_info_h_


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
const char* SRSCALL SR_SYS_GetVersionString();

/**
 * @brief Get the GIT commit id of current build
 * 
 * @return uint64_t The hash value of the commit
 */
uint64_t SRSCALL SR_SYS_GitCommitID();


#ifdef __cplusplus
}
#endif

#endif
