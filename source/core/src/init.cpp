/**
 * @file init.cpp
 * @author HenryAWE
 * @brief Initialization 
 */

#include <glad/glad.h>
#include <sr/core/init.hpp>
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_mixer.h>
#include <sr/core/version_info.hpp>


namespace srose::core
{
    int SRSCALL InitSDL(bool msgbox_on_err) noexcept
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
            "[CORE] SDL_Init() successfully initialized\n"
            "SDL Info:\n"
            "  Version %d.%d.%d\n"
            "  Runtime Version: %d.%d.%d",
            VERSION_NUMBER_2_ARG(sdl_version),
            VERSION_NUMBER_2_ARG(sdl_version_linked)
        );

        if(Mix_Init(MIX_INIT_FLAC | MIX_INIT_OGG) == 0)
        {
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[CORE] Mix_Init() failed %s",
                Mix_GetError()
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
        SDL_version mix_version;
        const SDL_version* mix_version_linked;
        SDL_MIXER_VERSION(&mix_version);
        mix_version_linked = Mix_Linked_Version();
        SDL_LogInfo(
            SDL_LOG_CATEGORY_APPLICATION,
            "[CORE] Mix_Init() successfully initialized\n"
            "SDL_Mixer Info:\n"
            "  Version %d.%d.%d\n"
            "  Runtime Version: %d.%d.%d",
            VERSION_NUMBER_2_ARG(mix_version),
            VERSION_NUMBER_2_ARG(*mix_version_linked)
        );

        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[CORE] Mix_OpenAudio() failed %s",
                Mix_GetError()
            );
            if(msgbox_on_err)
            {
                SDL_ShowSimpleMessageBox(
                    SDL_MESSAGEBOX_ERROR,
                    "Mix_OpenAudio() failed",
                    SDL_GetError(),
                    NULL
                );
            }

            Mix_Quit();
            SDL_Quit();
            return 1;
        }

        return 0;
    }

    int SRSCALL InitGL() noexcept
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

    void SRSCALL QuitSDL() noexcept
    {
        Mix_CloseAudio();
        Mix_Quit();
        SDL_Quit();
    }

    // Global variables make it easy to access the console argument
    // Declared in args.c
    extern int sr_argc;
    extern char** sr_argv;

    int SRSCALL Init(int argc, char* argv[], int msgbox_on_error)
    {
        sr_argc = argc; sr_argv = argv;
        if(InitSDL(msgbox_on_error) != 0)
        {
            return -1;
        }

        return 0;
    }

    void SRSCALL Quit()
    {
        QuitSDL();
    }
} // namespace srose::core
