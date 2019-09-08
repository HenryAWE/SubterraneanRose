/**
 * @file main_loop.c
 * @author HenryAWE
 * @brief 
 */

#include "main_loop.h"
#include <stdlib.h>


int SRSCALL main_loop(SR_WM_display* display)
{
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

    return EXIT_SUCCESS;
}
