/**
 * @file init.c
 * @author HenryAWE
 * @brief Initialization 
 */

#include <sr/sys/init.h>
#include <SDL.h>
// TODO: Restore in future
// #include <SDL_mixer.h>


int SRSCALL SR_SYS_InitSDL(int msgbox_on_err)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[SYS] SDL_Init() failed: %s",
            SDL_GetError()
        );
        if(msgbox_on_err)
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "SDL_Init() failed",
                SDL_GetError(),
                NULL
            );
        }

        return 1;
    }
    // TODO: Restore in future
    // if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG) == 0)
    // {
    //     SDL_LogError(
    //         SDL_LOG_CATEGORY_APPLICATION,
    //         "[SYS] Mix_Init() failed: %s",
    //         SDL_GetError()
    //     );
    //     if(msgbox_on_err)
    //     {
    //         SDL_ShowSimpleMessageBox(
    //             SDL_MESSAGEBOX_ERROR,
    //             "Mix_Init() failed",
    //             SDL_GetError(),
    //             NULL
    //         );
    //     }
    //     SDL_Quit();

    //     return 1;
    // }

    return 0;
}

void SRSCALL SR_SYS_QuitSDL(void)
{
    // TODO: Restore in future
    // Mix_Quit();
    SDL_Quit();
}
