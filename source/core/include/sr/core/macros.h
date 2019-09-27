/**
 * @file macros.h
 * @author HenryAWE
 * @brief Macros
 */

#ifndef SROSE_CORE_macros_h_
#define SROSE_CORE_macros_h_

#include <SDL_platform.h>


#ifdef _MSC_VER
#   define SRSCALL __stdcall
#else
#   define SRSCALL 
#endif

#ifdef __cplusplus
#   define SR_NOEXCEPT noexcept
#else
#   define SR_NOEXCEPT
#endif

#define SR_APPLICATION_NAME "Subterranean Rose"

#endif
