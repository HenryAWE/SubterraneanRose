/**
 * @file display.h
 * @author HenryAWE
 * @brief Window Manager
 */

#ifndef SROSE_WM_window_h_
#define SROSE_WM_window_h_


#include <SDL.h>
#include <sr/sys/macros.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SR_WM_display_
{
    SDL_Window* win;
    SDL_Renderer* renderer;
    SDL_GLContext* glctx;
} SR_WM_display;

SR_WM_display* SRSCALL SR_WM_CreateDisplay(
    const char* title,
    int additional_flags
);

void SRSCALL SR_WM_DestroyDisplay(
    SR_WM_display* display
);


#ifdef __cplusplus
}
#endif

#endif
