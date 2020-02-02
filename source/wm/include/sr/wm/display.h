/**
 * @file display.h
 * @author HenryAWE
 * @brief Window Manager
 */

#ifndef SROSE_WM_window_h_
#define SROSE_WM_window_h_


#include <SDL.h>
#include <sr/core/macros.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SR_WM_display_
{
    SDL_Window* win; // Window
    SDL_GLContext glctx; // OpenGL context (if we're using OpenGL render backend)
} SR_WM_display;

/**
 * @brief Create display
 * 
 * @param title Title string in UTF-8 encoding
 * @param additional_flags Addtional flags passed to SDL_CreateWindow
 * @return SR_WM_display* The created display, or NULL if failed 
 */
SR_WM_display* SRSCALL SR_WM_CreateDisplay(
    const char* title,
    int additional_flags
);

/**
 * @brief Destroy the created display
 * 
 * @param display The instance to delete
 */
void SRSCALL SR_WM_DestroyDisplay(
    SR_WM_display* display
);


#ifdef __cplusplus
}
#endif

#endif
