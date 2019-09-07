/**
 * @file entry_windows.c
 * @author HenryAWE
 * @brief Program entry on windows desktop
 */

#include <assert.h>
#include <stdlib.h>
#include <SDL.h>
#include <sr/sys/init.h>
#include <sr/wm/display.h>
#include <sr/player/entry.h>


#ifdef _WIN32
int entry_windows(int argc, char* argv[])
{
    if(SR_SYS_InitSDL(SDL_TRUE) != 0)
    {
        return EXIT_FAILURE;
    }
    else
    {
        at_quick_exit(&SR_SYS_QuitSDL); // 
    }
    

    SR_WM_display* display = SR_WM_CreateDisplay("Subterranean Rose", 0);
    if(!display)
    {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "SR_WM_CreateDisplay() failed",
            "See log for detailed information",
            NULL
        );
        SR_SYS_QuitSDL();
        return EXIT_FAILURE;
    }

    int loop = SDL_TRUE;
    SDL_Event event;
    SDL_zero(event);
    while(loop)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                loop = SDL_FALSE;
            }
        }

        SDL_RenderPresent(display->renderer);
    }

    SR_WM_DestroyDisplay(display);
    SR_SYS_QuitSDL();
    return EXIT_SUCCESS;
}
#else
int entry_windows(int argc, char* argv[]) 
{ // Dummy
    assert(false);
    return EXIT_FAILURE;
}
#endif
