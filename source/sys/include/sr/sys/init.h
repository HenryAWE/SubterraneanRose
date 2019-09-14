/**
 * @file init.h
 * @author HenryAWE
 * @brief Initializaton
 */

#ifndef SROSE_SYS_init_h_
#define SROSE_SYS_init_h_


#include <sr/sys/macros.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize SDL and other SDL library
 * 
 * @param msgbox_on_err Show message box on error
 * @return int Zero for success, other value means an error occurred
 */
int SRSCALL SR_SYS_InitSDL(int msgbox_on_err);

/**
 * @brief Deinitialize SDL and other SDL library
 */
void SRSCALL SR_SYS_QuitSDL(void);

/**
 * @brief Initialize everything
 * 
 * @param msgbox_on_err Show message box on error
 * @return int Zero for success, other value means an error occurred
 */
int SRSCALL SR_SYS_Init(int argc, char* argv[], int msgbox_on_error);

#ifdef __cplusplus
}
#endif

#endif
