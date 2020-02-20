/**
 * @file init.h
 * @author HenryAWE
 * @brief Initializaton
 */

#ifndef SROSE_CORE_init_h_
#define SROSE_CORE_init_h_


#include <sr/core/macros.h>
#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize SDL and other SDL library
 * 
 * @param msgbox_on_err Show message box on error
 * @return int Zero for success, other value means an error occurred
 */
int SRSCALL SR_CORE_InitSDL(int msgbox_on_err);

/**
 * @brief Deinitialize SDL and other SDL library
 */
void SRSCALL SR_CORE_QuitSDL(void);

/**
 * @brief Initialize OpenGL render context
 * 
 * @warning MUST BE CALLED FROM A THREAD WITH GL CONTEXT!!!
 * 
 * @return int Zero for success, other value means an error occurred
 */
int SRSCALL SR_CORE_InitGL(void);

/**
 * @brief Initialize everything
 * 
 * @param msgbox_on_err Show message box on error
 * @return int Zero for success, other value means an error occurred
 */
int SRSCALL SR_CORE_Init(int argc, char* argv[], int msgbox_on_error);

/**
 * @brief Deinitialize everything
 */
void SRSCALL SR_CORE_Quit(void);

#ifdef __cplusplus
}
#endif

#endif
