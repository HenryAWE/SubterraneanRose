/**
 * @file main.cpp
 * @author HenryAWE
 * @brief Program entry
 */

#include <sr/ui/entry.hpp>
#include <SDL_main.h>
#include "initialize.hpp"
#include <sr/ui/i18n/i18n.hpp>
#include <sr/core/init.hpp>
#include <sr/trace/trace.hpp>
#include <sr/ui/console/cmdline.hpp>

/*Program entry */
#ifdef _MSC_VER
#   pragma comment(linker, "/subsystem:windows")
#endif
int main(int argc, char* argv[])
{
    using namespace srose;

    trace::RegisterSignalHandler();

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
            nullptr
        );

        core::Quit();
        return EXIT_FAILURE;
    }

    InitializeAllSystems(display);

    int exit_code = srose::ProgramEntry(argc, argv);

    DeinitializeAllSystems();
    wm::DestroyDisplay(display);
    core::Quit();
    return exit_code;
}
