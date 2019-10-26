/**
 * @file init.c
 * @author HenryAWE
 * @brief Initialization 
 */

#include <glad/glad.h>
#include <sr/core/init.h>
#include <SDL.h>
#include <SDL_mixer.h>
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
    Mix_Quit();
    SDL_Quit();
}

// Global variables make it easy to access the console argument
// Declared in args.c
extern int sr_argc;
extern char** sr_argv;

int SRSCALL SR_CORE_Init(int argc, char* argv[], int msgbox_on_error)
{
    sr_argc = argc; sr_argv = argv;
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
