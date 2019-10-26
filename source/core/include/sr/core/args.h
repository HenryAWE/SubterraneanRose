/**
 * @file args.h
 * @author HenryAWE
 * @brief Managing console argument
 */

#ifndef SROSE_CORE_args_h_
#define SROSE_CORE_args_h_

#include "macros.h"


#ifdef __cplusplus
extern "C" {
#endif

int SRSCALL SR_CORE_GetArgc();
char** SRSCALL SR_CORE_GetArgv();

#ifdef __cplusplus
}
#endif

#endif
