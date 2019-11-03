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

#define SR_AUDIO_SAMPLE_RATE 44100

/**
 * @brief Open audio device
 * 
 * @param callback Callback function to fill the audio buffer
 * @param user User data
 * @return int 0 means succeeded, -1 means an error occurred
 */
int SRSCALL SR_CORE_OpenAudio(SDL_AudioCallback callback, void* user);
/**
 * @brief Close the audio device
 * 
 * @remark SR_CORE_Quit() will automatically call this function,
 *         if the previous call to SR_CORE_OpenAudio() was successful
 */
void SRSCALL SR_CORE_CloseAudio(void);

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
