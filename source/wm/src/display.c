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
#include <sr/core/version_info.h>
#include <sr/core/init.h>
#include <sr/wm/event.h>


#define SR_WM_SafeAssign(ptr, val) do{ if((ptr)) *ptr=val; }while(0)

/*Begin Display Creation */

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
        if (SR_WM_COMP_DISPLAY_SIZE(comparator, 1920, 1080)) \
        { /*1920x1080 */ \
            SR_WM_SafeAssign(out_w, 1920); \
            SR_WM_SafeAssign(out_h, 1080); \
            return; \
        } \
        else if (SR_WM_COMP_DISPLAY_SIZE(comparator, 1280, 960)) \
        { /*1280x960 */ \
            SR_WM_SafeAssign(out_w, 1280); \
            SR_WM_SafeAssign(out_h, 960); \
            return; \
        } \
        else if (SR_WM_COMP_DISPLAY_SIZE(comparator, 640, 480)) \
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

    if (flags&SDL_WINDOW_FULLSCREEN)
        SR_WM_CALC_WINDOW_SIZE(>=);
    else
        SR_WM_CALC_WINDOW_SIZE(>);

    #undef SR_WM_CALC_WINDOW_SIZE
    #undef SR_WM_COMP_DISPLAY_SIZE
}

SR_WM_display* SRSCALL SR_WM_CreateDisplay(
    const char* title,
    int additional_flags
) { // TODO: Stable behavior
    SR_WM_display* display = (SR_WM_display*)malloc(sizeof(SR_WM_display));
    SDL_zerop(display);

    /*Set the OpenGL attributes for window&context creation*/
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

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
    if (!display->win)
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
    
    /*Setup OpenGL */
    display->glctx = SDL_GL_CreateContext(display->win);
    if(!display->glctx)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[WM] SDL_GL_CreateContext() failed: %s",
            SDL_GetError()
        );

        SDL_DestroyWindow(display->win);
        free(display);
    }
    if(SR_CORE_InitGL() != 0)
    { // Failed
        SDL_GL_MakeCurrent(display->win, NULL);
        SDL_GL_DeleteContext(display->glctx);
        SDL_DestroyWindow(display->win);
        free(display);

        return NULL;
    }

    /*ImGui */
    if(SR_WM_InitEventSystem(display) != 0)
    {
        SDL_GL_MakeCurrent(display->win, NULL);
        SDL_GL_DeleteContext(display->glctx);
        SDL_DestroyWindow(display->win);
        free(display);

        return NULL;
    }

    /*Return the result */
    return display;
}

void SRSCALL SR_WM_DestroyDisplay(
    SR_WM_display* display
) {
    if (!display) return;
    SR_WM_QuitEventSystem();
    if(display->glctx)
    {
        SDL_GL_MakeCurrent(display->win, NULL);
        SDL_GL_DeleteContext(display->glctx);
    }
    if (display->win) SDL_DestroyWindow(display->win);
    free(display);
}
/*End Display Creation*/
