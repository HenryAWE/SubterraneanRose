/**
 * @file program_name.h
 * @author HenryAWE
 * @brief Get the full path to the program executable
 */

#ifndef SROSE_UTIL_program_name_h_
#define SROSE_UTIL_program_name_h_

#include <sr/sys/macros.h>


#ifdef __cplusplus
extern "C"{
#endif

const char* SRSCALL SR_UTIL_GetProgramName();

#ifdef __cplusplus
}
#endif

#endif
