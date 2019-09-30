/**
 * @file init.c
 * @author HenryAWE
 * @brief Initialization 
 */

#include <glad/glad.h>
#include <sr/core/init.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_video.h>
#include <sr/core/version_info.h>


int SRSCALL SR_CORE_InitSDL(int msgbox_on_err)
{
    #define VERSION_NUMBER_2_ARG(x) ((x).major),((x).minor),((x).patch)
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[CORE] SDL_Init() failed: %s",
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
        "[CORE] SDL_Init() success\n"
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
            "[CORE] Mix_Init() failed: %s",
            SDL_GetError()
        );
        if(0)
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Mix_Init() failed",
                SDL_GetError(),
                NULL
            );
        }
        // SDL_Quit();

        // return 1;
    }

    SDL_version mixer_version;
    const SDL_version* mixer_version_linked;
    MIX_VERSION(&mixer_version);
    mixer_version_linked = Mix_Linked_Version();
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "[CORE] Mix_Init() success\n"
        "Mixer Info:\n"
        "  Version %d.%d.%d\n"
        "  Runtime Version: %d.%d.%d",
        VERSION_NUMBER_2_ARG(mixer_version),
        VERSION_NUMBER_2_ARG(*mixer_version_linked)
    );

    if(IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[CORE] IMG_Init() failed: %s",
            IMG_GetError()
        );
        if(msgbox_on_err)
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "IMG_Init() failed",
                IMG_GetError(),
                NULL
            );
        }
        Mix_Quit();
        SDL_Quit();

        return 1;
    }

    SDL_version image_version;
    const SDL_version* image_version_linked;
    SDL_IMAGE_VERSION(&image_version);
    image_version_linked = IMG_Linked_Version();
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "[CORE] IMG_Init() success\n"
        "Image Info:\n"
        "  Version %d.%d.%d\n"
        "  Runtime Version: %d.%d.%d",
        VERSION_NUMBER_2_ARG(image_version),
        VERSION_NUMBER_2_ARG(*image_version_linked)
    );

    return 0;
}

int SRSCALL SR_CORE_InitGL(void)
{
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[CORE] GLAD failed"
        );

        return -1;
    }

    return 0;
}

void SRSCALL SR_CORE_QuitSDL(void)
{
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

int SRSCALL SR_CORE_Init(int argc, char* argv[], int msgbox_on_error)
{
    printf(
        "Subterranean Rose Project:\n"
        "  Version %s - %s\n\n"
        "  Please visit https://github.com/HenryAWE/SubterraneanRose for more up-to-date information",
        SR_CORE_GetVersionString(), SR_CORE_GitCommitShortID()
    );
    if(SR_CORE_InitSDL(msgbox_on_error) != 0)
    {
        return -1;
    }

    return 0;
}

void SRSCALL SR_CORE_Quit(void)
{
    SR_CORE_QuitSDL();
}
