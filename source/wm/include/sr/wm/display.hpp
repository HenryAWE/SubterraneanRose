/**
 * @file display.hpp
 * @author HenryAWE
 * @brief Window Manager
 */

#ifndef SROSE_WM_window_hpp_
#define SROSE_WM_window_hpp_

#include <SDL.h>
#include <sr/core/macros.hpp>


namespace srose::wm
{
    struct Display
    {
        SDL_Window* win; // Window
        SDL_GLContext glctx; // OpenGL context (if we're using OpenGL render backend)
    };

    /**
     * @brief Create display
     * 
     * @param title Title string in UTF-8 encoding
     * @param additional_flags Addtional flags passed to SDL_CreateWindow
     * @return Display* The created display, or NULL if failed 
     */
    Display* SRSCALL CreateDisplay(
        const char* title,
        int additional_flags
    );

    /**
     * @brief Destroy the created display
     * 
     * @param display The instance to delete
     */
    void SRSCALL DestroyDisplay(
        Display* display
    );
} // namespace srose::wm


#endif
