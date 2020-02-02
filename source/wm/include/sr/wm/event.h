/**
 * @file event.h
 * @author HenryAWE
 * @brief Event handler
 */

#ifndef SROSE_WM_event_h_
#define SROSE_WM_event_h_

#include <sr/core/macros.h>
#include <sr/wm/display.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize event handler
 * 
 * @param display The instance of previously created display by SR_WM_CreateDisplay()
 * @return int SR_WM_InitEventSystem -1 means an error occurred, 0 means succeeded
 */
int SRSCALL SR_WM_InitEventSystem(SR_WM_display* display);

/**
 * @brief Get the instance of display
 * 
 * @return SR_WM_display* SR_WM_GetDisplay The instance of the main display
 */
SR_WM_display* SRSCALL SR_WM_GetDisplay();

/**
 * @brief Quit the event system and release the data
 */
void SRSCALL SR_WM_QuitEventSystem();

/**
 * @brief Process the events 
 * 
 * @return int SR_WM_ProcessEvent 
 */
int SRSCALL SR_WM_ProcessEvent();

/**
 * @brief Begin a new frame
 */
void SRSCALL SR_WM_NewFrame();

/**
 * @brief End a frame
 */
void SRSCALL SR_WM_EndFrame();

/**
 * @brief Update vertex data and draw the UI
 * @remark SDL_WM_EndFrame() must be called before it!
 */
void SRSCALL SR_WM_RenderFrame();

#ifdef __cplusplus
}
#endif

#endif
