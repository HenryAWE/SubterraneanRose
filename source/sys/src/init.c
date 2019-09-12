/**
 * @file init.c
 * @author HenryAWE
 * @brief Initialization 
 */

#include <sr/sys/init.h>
#include <SDL.h>
#include <SDL_mixer.h>


int SRSCALL SR_SYS_InitSDL(int msgbox_on_err)
{
    #define VERSION_NUMBER_2_ARG(x) ((x).major),((x).minor),((x).patch)
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
    SDL_version sdl_version;
    SDL_version sdl_version_linked;
    SDL_VERSION(&sdl_version);
    SDL_GetVersion(&sdl_version_linked);;
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "[SYS] SDL_Init() success\n"
        "SDL Info:\n"
        "  Version %d.%d.%d\n"
        "  Runtime Version: %d.%d.%d",
        VERSION_NUMBER_2_ARG(sdl_version),
        VERSION_NUMBER_2_ARG(sdl_version_linked)
    );

    if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG) == 0)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[SYS] Mix_Init() failed: %s",
            SDL_GetError()
        );
        if(msgbox_on_err)
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Mix_Init() failed",
                SDL_GetError(),
                NULL
            );
        }
        SDL_Quit();

        return 1;
    }

    SDL_version mixer_version;
    const SDL_version* mixer_version_linked;
    MIX_VERSION(&mixer_version);
    mixer_version_linked = Mix_Linked_Version();
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "[SYS] Mix_Init() success\n"
        "Mixer Info:\n"
        "  Version %d.%d.%d\n"
        "  Runtime Version: %d.%d.%d",
        VERSION_NUMBER_2_ARG(mixer_version),
        VERSION_NUMBER_2_ARG(*mixer_version_linked)
    );

    return 0;
}

void SRSCALL SR_SYS_QuitSDL(void)
{
    Mix_Quit();
    SDL_Quit();
}
