/**
 * @file display.c
 * @author HenryAWE
 * @brief Window Manager
 */

#include <sr/wm/display.h>
#include <stddef.h>
#include <stdlib.h>


SR_WM_display* SRSCALL SR_WM_CreateDisplay(
    const char* title,
    int additional_flags
) {
    SR_WM_display* display = (SR_WM_display*)malloc(sizeof(SR_WM_display));
    memset(display, 0, sizeof(SR_WM_display));

    SDL_GL_ResetAttributes();
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    /*Create window */
    additional_flags |= SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    display->win = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1280, 960,
        additional_flags
    );
    if(!display->win)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "SDL_CreateWindow() failed: %s",
            SDL_GetError()
        );

        free(display);
        return NULL;
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow() succeeded");

    display->renderer = SDL_CreateRenderer(display->win, -1, SDL_RENDERER_PRESENTVSYNC);
    if(display->renderer)
    {   SDL_RendererInfo render_info;
        memset(&render_info, 0, sizeof(SDL_RendererInfo));
        SDL_GetRendererInfo(display->renderer, &render_info);

        SDL_LogInfo(
            SDL_LOG_CATEGORY_APPLICATION,
            "SDL_CreateRenderer() succeeded\n"
            "Renderer name: %s",
            render_info.name
        );
    }
    else
    { // Create renderer failed
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "SDL_CreateRenderer() failed: %s",
            SDL_GetError()
        );

        SDL_DestroyWindow(display->win);
        free(display);
        return NULL;
    }

    return display;
}

void SRSCALL SR_WM_DestroyDisplay(
    SR_WM_display* display
) {
    if(!display) return;
    if(display->renderer) SDL_DestroyRenderer(display->renderer);
    if(display->win) SDL_DestroyWindow(display->win);
    free(display);
}
