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

int SRSCALL SR_WM_InitEventSystem(SR_WM_display* display);

SR_WM_display* SRSCALL SR_WM_GetDisplay();

void SRSCALL SR_WM_QuitEventSystem();

int SRSCALL SR_WM_ProcessEvent();

void SRSCALL SR_WM_NewFrame();

void SRSCALL SR_WM_EndFrame();

void SRSCALL SR_WM_RenderFrame();

#ifdef __cplusplus
}
#endif

#endif
