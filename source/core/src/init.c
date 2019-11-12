/**
 * @file init.c
 * @author HenryAWE
 * @brief Initialization 
 */

#include <glad/glad.h>
#include <sr/core/init.h>
#include <SDL.h>
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

    if(SR_CORE_OpenAudio(NULL, NULL) == -1)
    {
        return -1;
    }

    return 0;
}

static SDL_AudioSpec obtained;
static SDL_AudioDeviceID dev;

int SRSCALL SR_CORE_OpenAudio(SDL_AudioCallback callback, void* user)
{
    SDL_AudioSpec want;
    memset(&want, 0, sizeof(want));

    want.freq = SR_AUDIO_SAMPLE_RATE;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 4096;
    want.callback = callback;
    want.userdata = user;

    dev = SDL_OpenAudioDevice(NULL, 0, &want, &obtained, 0);
    if(dev == 0)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[CORE] SDL_OpenAudioDevice() failed: %s",
            SDL_GetError()
        );

        return -1;
    }

    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "[CORE] SDL_OpenAudioDevice() succeeded\n"
        "  Device ID: %u - \"%s\"\n"
        "  Driver: %s",
        dev-2,
        SDL_GetAudioDeviceName(dev-2, 0),
        SDL_GetCurrentAudioDriver()
    );

    return 0;
}
void SRSCALL SR_CORE_CloseAudio(void)
{
    if(dev > 1);
    SDL_CloseAudioDevice(dev);
    memset(&obtained, 0, sizeof(obtained));
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
    SR_CORE_CloseAudio();
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
