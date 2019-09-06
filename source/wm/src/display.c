/**
 * @file display.c
 * @author HenryAWE
 * @brief Window Manager
 */

#include <sr/wm/display.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define SR_WM_SafeAssign(ptr, val) do{ if((ptr)) *ptr=val; }while(0)

/**
 * @brief [Internal] Get the window size that suit the framework
 * 
 * @param out_w [out] Output the width
 * @param out_h [out] Output the height
 * @param flags The flag of the window
 * @param display_index The index of the display
 */
static void SRSCALL SR_WM_GetSRoseWindowSize(int* out_w, int* out_h, int flags, int display_index)
{
    SDL_Rect system_size = {0, 0, 0, 0};
    SDL_GetDisplayBounds(display_index, &system_size);

    /* Currently support display size
     * | width | height |
     * | 640   | 480    |
     * | 1280  | 960    |
     * | 1920  | 1080   |
     */

    #define SR_WM_COMP_DISPLAY_SIZE(comp, width, height) \
    (((system_size.w)comp((width)))&&((system_size.h)comp((height))))
    #define SR_WM_CALC_WINDOW_SIZE(comparator) \
    do \
    { \
        if(SR_WM_COMP_DISPLAY_SIZE(comparator, 1920, 1080)) \
        { /*1920x1080 */ \
            SR_WM_SafeAssign(out_w, 1920); \
            SR_WM_SafeAssign(out_h, 1080); \
            return; \
        } \
        else if(SR_WM_COMP_DISPLAY_SIZE(comparator, 1280, 960)) \
        { /*1280x960 */ \
            SR_WM_SafeAssign(out_w, 1280); \
            SR_WM_SafeAssign(out_h, 960); \
            return; \
        } \
        else if(SR_WM_COMP_DISPLAY_SIZE(comparator, 640, 480)) \
        { /*640x480 */ \
            SR_WM_SafeAssign(out_w, 640); \
            SR_WM_SafeAssign(out_h, 480); \
            return; \
        } \
        else \
        { /*Too small */ \
            SR_WM_SafeAssign(out_w, 640); \
            SR_WM_SafeAssign(out_h, 480); \
            SDL_LogWarn( \
                SDL_LOG_CATEGORY_APPLICATION, \
                "[WM] The size of the display %02d (%dx%d) is too small", \
                display_index, \
                system_size.w, system_size.y \
            ); \
            return; \
        } \
    }while(0) /*End of the big macro */

    if(flags&SDL_WINDOW_FULLSCREEN)
        SR_WM_CALC_WINDOW_SIZE(>=);
    else
        SR_WM_CALC_WINDOW_SIZE(>);

    #undef SR_WM_CALC_WINDOW_SIZE
    #undef SR_WM_COMP_DISPLAY_SIZE
}

/**
 * @brief [Internal] Get the index of the OpenGL driver
 * 
 * @return int The index of the OpenGL driver, or -1 if error occurred
 */
static int SRSCALL SR_WM_GetOpenGLRendererDriver()
{
    int total = SDL_GetNumRenderDrivers();
    SDL_RendererInfo info;
    for(int i = 0; i < total; ++i)
    {
        SDL_zero(info);
        SDL_GetRenderDriverInfo(i, &info);
        //TODO: Use OpenGL ES on mobile platform
        if(strspn(info.name, "opengl") != 0)
            return i;
    }

    SDL_LogError(
        SDL_LOG_CATEGORY_VIDEO,
        "[WM] Cannot find a OpenGL render driver"
    );
    return -1;
}

SR_WM_display* SRSCALL SR_WM_CreateDisplay(
    const char* title,
    int additional_flags
) { // TODO: Stable behavior
    SR_WM_display* display = (SR_WM_display*)malloc(sizeof(SR_WM_display));
    SDL_zerop(display);

    /*Set the OpenGL attributes for window&context creation*/
    SDL_GL_ResetAttributes();
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    /*Create window */
    additional_flags |= SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI; // Add necessary flags

    const int display_index = 0;
    int window_w = 640, window_h = 480;
    SR_WM_GetSRoseWindowSize(&window_w, &window_h, additional_flags, display_index);
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "[WM] Display %02d - %s",
        display_index,
        SDL_GetDisplayName(display_index)
    );

    display->win = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED_DISPLAY(display_index),
        SDL_WINDOWPOS_CENTERED_DISPLAY(display_index),
        window_w,
        window_h,
        additional_flags
    );
    if(!display->win)
    { // Create window failed
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[WM] SDL_CreateWindow() failed: %s",
            SDL_GetError()
        );

        free(display);
        return NULL;
    }
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "[WM] SDL_CreateWindow() succeeded");

    int renderer_index = SR_WM_GetOpenGLRendererDriver();
    if(renderer_index == -1)
    {
        SDL_DestroyWindow(display->win);
        free(display);
        return NULL;
    }
    display->renderer = SDL_CreateRenderer(display->win, renderer_index, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if(display->renderer)
    { // Renderer successfully created
        SDL_RendererInfo render_info;
        memset(&render_info, 0, sizeof(SDL_RendererInfo));
        SDL_GetRendererInfo(display->renderer, &render_info);

        SDL_LogInfo(
            SDL_LOG_CATEGORY_APPLICATION,
            "[WM] SDL_CreateRenderer() successfully created %s renderer\n",
            render_info.name
        );
    }
    else
    { // Create renderer failed
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[WM] SDL_CreateRenderer() failed: %s",
            SDL_GetError()
        );

        SDL_DestroyWindow(display->win);
        free(display);
        return NULL;
    }

    /*Create OpenGL context */
    display->glctx = SDL_GL_CreateContext(display->win);
    if(!display->glctx)
    { // Create context failed
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[WM] SDL_GL_CreateContext() failed: %s",
            SDL_GetError()
        );

        SDL_DestroyRenderer(display->renderer);
        SDL_DestroyWindow(display->win);
        free(display);
        return NULL;
    }
    else
    {
        SDL_LogInfo(
            SDL_LOG_CATEGORY_APPLICATION,
            "[WM] SDL_CreateRenderer() succeeded\n"
        );
    }

    /*Return the result */
    return display;
}

void SRSCALL SR_WM_DestroyDisplay(
    SR_WM_display* display
) {
    if(!display) return;
    if(display->glctx) SDL_GL_DeleteContext(display->glctx);
    if(display->renderer) SDL_DestroyRenderer(display->renderer);
    if(display->win) SDL_DestroyWindow(display->win);
    free(display);
}
