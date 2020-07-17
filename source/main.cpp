/**
 * @file main.cpp
 * @author HenryAWE
 * @brief Program entry
 */

#include <sr/ui/entry.hpp>
#include <SDL_main.h>
#include "initialize.hpp"
#include <sr/i18n/i18n.hpp>
#include <sr/core/init.hpp>
#include <sr/trace/log.hpp>
#include <sr/trace/trace.hpp>
#include <sr/console/cmdline.hpp>

/*Program entry */
#ifdef _MSC_VER
#   pragma comment(linker, "/subsystem:windows")
#endif
int main(int argc, char* argv[])
{
    using namespace srose;

    trace::RegisterSignalHandler();

    SetWorkingDirectory(argv[0]);
    trace::InitializeLogger();
    trace::RedirectSDLOutput();
    i18n::LoadAllLanguage(filesystem::GetLocaleFolder());
    i18n::SelectLanguage();

    auto& cli = console::CommandLineInterface::GetGlobalInstance();
    cli.ParseArg(argc, argv);
    cli.HandleArg();
    if(cli.QuitRequested())
    {
        #ifdef _WIN32
        if(cli.WinPauseRequested()) std::system("pause");
        #endif
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
    window_flags |= cli.FullscreenRequired()?SDL_WINDOW_FULLSCREEN:0;
    wm::Window window;
    try
    {
        window.Create(
            {1280, 960},
            "Subterranean Rose",
            0,
            window_flags
        );
        if(cli.VSyncRequired())
            window.SetVSync(true);
    }
    catch(const std::runtime_error& e)
    { // Create display failed
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "Failed to create window",
            (std::string(e.what()) + "\nSee log for detailed information").c_str(),
            nullptr
        );

        core::Quit();
        return EXIT_FAILURE;
    }

    InitializeAllSystems(window);

    int exit_code = srose::ProgramEntry(window);

    DeinitializeAllSystems(window);
    window.Destroy();
    core::Quit();
    return exit_code;
}
