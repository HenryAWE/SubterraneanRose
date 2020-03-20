/**
 * @file entry.cpp
 * @author HenryAWE
 * @brief Program entry
 */

#include <sr/ui/entry.hpp>
#include <cassert>
#include <cstdlib>
#include <SDL.h>
#include <typeinfo>
#include <sr/core/init.hpp>
#include <sr/ui/console/cmdline.hpp>
#include "initialize.hpp"
#include "main_loop.hpp"
#include "i18n/i18n.hpp"


namespace srose
{
    int SRSCALL ProgramEntry(int argc, char* argv[]) noexcept
    {
        using namespace srose;

        SetWorkingDirectory(argv[0]);
        ui::LoadAllLanguage(filesystem::GetLocaleFolder());

        if(ui::console::ParseArg(argc, argv) == 1)
        {
            return EXIT_SUCCESS;
        }

        if(core::Init(argc, argv, true) != 0)
        { // Init failed
            return EXIT_FAILURE;
        }
        else
        {
            // We'll handle deinitialization on normal exit
            at_quick_exit(&core::Quit);
        }

        int window_flags = 0;
        window_flags |= ui::console::FullscreenRequired()?SDL_WINDOW_FULLSCREEN:0;
        wm::Display* display = wm::CreateDisplay(
            "Subterranean Rose",
            window_flags
        );
        if(!display)
        { // Create display failed
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "SR_WM_CreateDisplay() failed",
                "See log for detailed information",
                NULL
            );

            core::Quit();
            return EXIT_FAILURE;
        }

        int exit_code = EXIT_SUCCESS;
        try
        {
            InitializeAllSystems(display);
            exit_code = BeginMainLoop();
        }
        catch(std::exception& ex)
        {
            exit_code = EXIT_FAILURE;
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[UI] Caught exception \"%s\": %s",
                typeid(ex).name(),
                ex.what()
            );
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                typeid(ex).name(),
                ex.what(),
                nullptr
            );
        }
        catch(...)
        {
            exit_code = EXIT_FAILURE;
            SDL_LogError(
                SDL_LOG_CATEGORY_APPLICATION,
                "[UI] Caught unknown exception"
            );

            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "UNCAUGHT EXCEPTION",
                "Unknown exception",
                nullptr
            );
        }

        DeinitializeAllSystems();
        wm::DestroyDisplay(display);
        core::Quit();
        return exit_code;
    }
} // namespace srose
