/**
 * @file entry_windows.cpp
 * @author HenryAWE
 * @brief Program entry on windows desktop
 */

#include <assert.h>
#include <stdlib.h>
#include <SDL.h>
#include <typeinfo>
#include <sr/core/init.h>
#include <sr/wm/display.h>
#include <sr/ui/entry.h>
#include "main_loop.hpp"
#include <sr/ui/console/progopt.h>


int SRSCALL program_entry(int argc, char* argv[])
{
    if(SR_UI_CONSOLE_ParseArg(argc, argv) == 1)
    {
        return EXIT_SUCCESS;
    }
    int exit_code = EXIT_SUCCESS;

    if(SR_CORE_InitSDL(SDL_TRUE) != 0)
    {
        return exit_code;
    }
    else
    {
        at_quick_exit(&SR_CORE_QuitSDL); // We'll handle deinitialization on normal exit
    }

    int window_flags = 0;
    window_flags |= SR_UI_CONSOLE_FullscreenRequired()?SDL_WINDOW_FULLSCREEN:0;
    SR_WM_display* display = SR_WM_CreateDisplay("Subterranean Rose", window_flags);
    if(!display)
    {
        exit_code = EXIT_FAILURE;
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "SR_WM_CreateDisplay() failed",
            "See log for detailed information",
            NULL
        );

        goto quit_program;
    }

    try
    {
        main_loop(display);
    }
    catch(std::exception& ex)
    {
        exit_code = EXIT_FAILURE;
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[PLAYER] Caught exception \"%s\": %s",
            typeid(ex).name(),
            ex.what()
        );
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            typeid(ex).name(),
           ex.what(),
            NULL
        );

        goto quit_program;
    }
    catch(...)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "[PLAYER] Caught unknown exception"
        );

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "UNCAUGHT EXCEPTION",
            "Unknown exception",
            NULL
        );
    }

quit_program:
    SR_WM_DestroyDisplay(display);
    SR_CORE_QuitSDL();
    return exit_code;
}
