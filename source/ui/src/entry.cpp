/**
 * @file entry_windows.cpp
 * @author HenryAWE
 * @brief Program entry on windows desktop
 */

#include <assert.h>
#include <stdlib.h>
#include <SDL.h>
#include <typeinfo>
#include <sr/sys/init.h>
#include <sr/wm/display.h>
#include <sr/ui/entry.h>
#include "main_loop.hpp"
#ifdef SR_ENABLE_CUI
#   include <sr/ui/console/progopt.h>
#endif


int SRSCALL program_entry(int argc, char* argv[])
{
    #ifdef SR_ENABLE_CUI
    if(SR_UI_CONSOLE_ParseArg(argc, argv) == 1)
    {
        return EXIT_SUCCESS;
    }
    #endif
    int exit_code = EXIT_SUCCESS;

    if(SR_SYS_InitSDL(SDL_TRUE) != 0)
    {
        return exit_code;
    }
    else
    {
        at_quick_exit(&SR_SYS_QuitSDL); // We'll handle deinitialization on normal exit
    }

    SR_WM_display* display = SR_WM_CreateDisplay("Subterranean Rose", 0);
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
    SR_SYS_QuitSDL();
    return exit_code;
}
