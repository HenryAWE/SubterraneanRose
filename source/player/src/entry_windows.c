/**
 * @file entry_windows.c
 * @author HenryAWE
 * @brief Program entry on windows desktop
 */

#include <stdlib.h>
#include <SDL.h>
#include <sr/sys/init.h>
#include <sr/wm/display.h>


#ifdef _WIN32
int entry_windows(int argc, char* argv[])
{
    if(SR_SYS_InitSDL() != 0)
    {
        return -1;
    }

    SR_WM_display* display = SR_WM_CreateDisplay("Subterranean Rose", 0);

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
    return EXIT_SUCCESS;
}
#endif
