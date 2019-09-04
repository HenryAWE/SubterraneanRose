/**
 * @file init.c
 * @author HenryAWE
 * @brief Initialization 
 */

#include "../init.h"
#include <SDL.h>


int SRSCALL SR_SYS_InitSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "SDL_Init() failed: %s",
            SDL_GetError()
        );

        return 1;
    }

    
    

    return 0;
}