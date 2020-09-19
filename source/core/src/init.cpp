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
#include <fmt/core.h>
#include <sr/core/version_info.hpp>
#include <sr/trace/log.hpp>


namespace srose::core
{
    int SRSCALL InitSDL(bool msgbox_on_err) noexcept
    {
        #define VERSION_NUMBER_2_ARG(x) ((x).major),((x).minor),((x).patch)
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            BOOST_LOG_TRIVIAL(error)
                << "[Core] SDL_Init() failed: "
                << SDL_GetError();
            if(msgbox_on_err)
            {
                SDL_ShowSimpleMessageBox(
                    SDL_MESSAGEBOX_ERROR,
                    "SDL_Init() failed",
                    SDL_GetError(),
                    nullptr
                );
            }

            return 1;
        }
        SDL_version sdl_version;
        SDL_version sdl_version_linked;
        SDL_VERSION(&sdl_version);
        SDL_GetVersion(&sdl_version_linked);;
        BOOST_LOG_TRIVIAL(info) << fmt::format(
            "[Core] SDL_Init() succeeded\n"
            "SDL Info:\n"
            "  Version {}.{}.{}\n"
            "  Runtime Version: {}.{}.{}",
            VERSION_NUMBER_2_ARG(sdl_version),
            VERSION_NUMBER_2_ARG(sdl_version_linked)
        );

        return 0;
    }

    void SRSCALL InitSDLMixer()
    {
        if(Mix_Init(MIX_INIT_MP3) == 0)
        {
            throw std::runtime_error(
                std::string("[Core] Mix_Init() failed: ") +
                Mix_GetError()
            );
        }
        SDL_version mix_version;
        const SDL_version* mix_version_linked;
        SDL_MIXER_VERSION(&mix_version);
        mix_version_linked = Mix_Linked_Version();
        BOOST_LOG_TRIVIAL(info) << fmt::format(
            "[Core] Mix_Init() succeeded\n"
            "SDL_Mixer Info:\n"
            "  Version {}.{}.{}\n"
            "  Runtime Version: {}.{}.{}",
            VERSION_NUMBER_2_ARG(mix_version),
            VERSION_NUMBER_2_ARG(*mix_version_linked)
        );

        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            throw std::runtime_error(
                std::string("[Core] Mix_OpenAudio() failed: ") +
                Mix_GetError()
            );
        }
    }

    void SRSCALL QuitSDLMixer() noexcept
    {
        Mix_CloseAudio();
        Mix_Quit();
    }

    int SRSCALL InitGL() noexcept
    {
        if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            BOOST_LOG_TRIVIAL(error)
                << "[Core] GLAD failed";

            return -1;
        }

        return 0;
    }

    void SRSCALL QuitSDL() noexcept
    {
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
